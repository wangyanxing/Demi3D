
#include "Skybox.h"
#include "SceneManager.h"
#include "CullNode.h"
#include "RenderBatchGroup.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexDeclaration.h"
#include "Material.h"
#include "MaterialPredefine.h"
#include "ShaderParam.h"
#include "GfxDriver.h"

namespace Demi
{
    DiSkybox::DiSkybox(DiSceneManager* sm)
        :mDistance(0),
        mNode(NULL),
        mSceneManager(sm),
        mSkybox(NULL)
    {
        mSkybox = make_shared<DiSkyboxShape>();
        mNode = sm->GetRootNode()->CreateChild("_sky_node");
        mNode->AttachObject(mSkybox);
        mNode->SetVisible(false);
    }

    DiSkybox::~DiSkybox()
    {
    }

    void DiSkybox::Enable( float distance, const DiString& textureName )
    {
        mDistance    = distance;
        mTextureName = textureName;

        mSkybox->Create(distance);

        CreateMaterial();

        Enable(true);
    }

    void DiSkybox::Enable( bool enable )
    {
        mNode->SetVisible(enable);
    }

    void DiSkybox::CreateMaterial()
    {
        if (!mSkyMaterial)
        {
            mSkyMaterial = DiMaterial::QuickCreate("_sky_mat",
                "skybox.vsh"/*DiMaterialDefine::STATIC_MESH_SHADER_NAME*/,
                DiMaterialDefine::SKY_PIXEL_SHADER);
        }
        
        DiShaderParameter* sp = mSkyMaterial->GetShaderParameter();
        sp->WriteTextureCUBE("environmentTexture",mTextureName);

        mSkybox->SetMaterial(mSkyMaterial);
    }

    void DiSkybox::Update()
    {
        mNode->SetPosition(mSceneManager->GetCamera()->GetDerivedPosition());
    }

    DiString& DiSkybox::DiSkyboxShape::GetType()
    {
        static DiString name = "DiSkyboxShape";
        return name;
    }

    void DiSkybox::DiSkyboxShape::AddToBatchGroup(DiRenderBatchGroup* bg)
    {
        bg->AddRenderUnit(this);
    }

    void DiSkybox::DiSkyboxShape::CullingUpdate( Demi::DiRenderBatchGroup* group,Demi::DiCamera* )
    {
        group->AddRenderUnit(this);
    }

    const DiAABB& DiSkybox::DiSkyboxShape::GetBoundingBox( void ) const
    {
        return DiAABB::BOX_INFINITE;
    }

    DiSkybox::DiSkyboxShape::DiSkyboxShape()
        :DiTransformUnit(),
        DiRenderUnit()
    {
        SetBatchGroup(BATCH_PRE_TERRAIN);
    }

    DiSkybox::DiSkyboxShape::~DiSkyboxShape()
    {
        ReleaseSourceData();
        ReleaseIndexBuffer();
        ReleaseVertexDeclaration();
    }

    void DiSkybox::DiSkyboxShape::Create( float distance, const DiQuat& quat )
    {
        ReleaseSourceData();
        ReleaseIndexBuffer();
        ReleaseVertexDeclaration();

        mSourceData.push_back(Driver->CreateVertexBuffer());
        mIndexBuffer = Driver->CreateIndexBuffer();

        mVertexDecl = Driver->CreateVertexDeclaration();
        mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_POSITION);
        mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_TEXCOORD);
        mVertexDecl->Create();

        mPrimitiveType = PT_TRIANGLELIST;

        mSourceData[0]->Release();
        mIndexBuffer->Release();

        const int NUM_VERTICES = 4 * 6;
        const int NUM_ENTRIES_PER_VERTEX = 6;
        const int NUM_VERTEX_ENTRIES = NUM_VERTICES * NUM_ENTRIES_PER_VERTEX;
        const int NUM_INDICES = 3 * 2 * 6;

        float vertices[NUM_VERTEX_ENTRIES];

        const int BP_FRONT    = 0;
        const int BP_BACK    = 1;
        const int BP_LEFT    = 2;
        const int BP_RIGHT    = 3;
        const int BP_UP        = 4;
        const int BP_DOWN    = 5;

        for (int i = 0; i < 6; ++i)
        {
            DiPlane plane;
            DiVec3 middle;
            DiVec3 up, right;

            switch(i)
            {
            case BP_FRONT:
                middle    = DiVec3(0, 0, -distance);
                up        = DiVec3::UNIT_Y * distance;
                right    = DiVec3::UNIT_X * distance;
                break;
            case BP_BACK:
                middle    = DiVec3(0, 0, distance);
                up        = DiVec3::UNIT_Y * distance;
                right    = DiVec3::NEGATIVE_UNIT_X * distance;
                break;
            case BP_LEFT:
                middle    = DiVec3(-distance, 0, 0);
                up        = DiVec3::UNIT_Y * distance;
                right    = DiVec3::NEGATIVE_UNIT_Z * distance;
                break;
            case BP_RIGHT:
                middle    = DiVec3(distance, 0, 0);
                up        = DiVec3::UNIT_Y * distance;
                right    = DiVec3::UNIT_Z * distance;
                break;
            case BP_UP:
                middle    = DiVec3(0, distance, 0);
                up        = DiVec3::UNIT_Z * distance;
                right    = DiVec3::UNIT_X * distance;
                break;
            case BP_DOWN:
                middle    = DiVec3(0, -distance, 0);
                up        = DiVec3::NEGATIVE_UNIT_Z * distance;
                right    = DiVec3::UNIT_X * distance;
                break;
            }

            // Modify by orientation
            middle    = quat * middle;
            up        = quat * up;
            right    = quat * right;

            float* pointer = &vertices[i*NUM_VERTICES];

            DiVec3 pos;
            DiVec3 uv;
            pos = middle + up - right;
            uv  = pos.normalisedCopy() * DiVec3(1,1,-1);
            *pointer++ = pos.x;
            *pointer++ = pos.y;
            *pointer++ = pos.z;
            *pointer++ = uv.x;
            *pointer++ = uv.y;
            *pointer++ = uv.z;

            pos = middle - up - right;
            uv  = pos.normalisedCopy() * DiVec3(1,1,-1);
            *pointer++ = pos.x;
            *pointer++ = pos.y;
            *pointer++ = pos.z;
            *pointer++ = uv.x;
            *pointer++ = uv.y;
            *pointer++ = uv.z;

            pos = middle - up + right;
            uv  = pos.normalisedCopy() * DiVec3(1,1,-1);
            *pointer++ = pos.x;
            *pointer++ = pos.y;
            *pointer++ = pos.z;
            *pointer++ = uv.x;
            *pointer++ = uv.y;
            *pointer++ = uv.z;

            pos = middle + up + right;
            uv  = pos.normalisedCopy() * DiVec3(1,1,-1);
            *pointer++ = pos.x;
            *pointer++ = pos.y;
            *pointer++ = pos.z;
            *pointer++ = uv.x;
            *pointer++ = uv.y;
            *pointer++ = uv.z;
        }

        UINT vbsize = NUM_VERTEX_ENTRIES*sizeof(float);
        mSourceData[0]->Create(vbsize);
        void* data = mSourceData[0]->Lock(0,vbsize);
        memcpy(data,vertices,vbsize);
        mSourceData[0]->Unlock();
        mSourceData[0]->SetStride(NUM_ENTRIES_PER_VERTEX * sizeof(float));

        uint16 faces[NUM_INDICES] = 
        {
            0,    1,    2,
            2,    3,    0,
            4,    5,    6,
            6,    7,    4,
            8,    9,    10,
            10,    11,    8,
            12,    13,    14,
            14,    15,    12,
            16,    17,    18,
            18,    19,    16,
            20,    21,    22,
            22,    23,    20
        };

        UINT ibsize = NUM_INDICES*sizeof(uint16);
        mIndexBuffer->Create(ibsize);

        void* dataib = mIndexBuffer->Lock(0,ibsize);
        memcpy(dataib,faces,ibsize);
        mIndexBuffer->Unlock();

        mVerticesNum = NUM_VERTICES;
        mPrimitiveCount = NUM_INDICES / 3;
    }

}