/**********************************************************************
This source file is a part of Demi3D
   __  ___  __  __  __
  |  \|_ |\/||   _)|  \ 
  |__/|__|  ||  __)|__/ 

Copyright (c) 2013-2014 Demi team
https://github.com/wangyanxing/Demi3D

Released under the MIT License
https://github.com/wangyanxing/Demi3D/blob/master/License.txt
***********************************************************************/
#include "GfxPch.h"
#include "SimpleShape.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexDeclaration.h"
#include "ShaderParam.h"
#include "GfxDriver.h"

namespace Demi 
{
    DiSimpleShape::DiSimpleShape(void)
    {
    }

    DiSimpleShape::DiSimpleShape( const DiString& name )
        :DiTransformUnit(name)
    {
    }

    DiSimpleShape::~DiSimpleShape(void)
    {
        ReleaseSourceData();
        ReleaseIndexBuffer();
        ReleaseVertexDeclaration();
    }

    const DiAABB& DiSimpleShape::GetBoundingBox( void ) const
    {
        return mAABB;
    }

    void DiSimpleShape::AddToBatchGroup(DiRenderBatchGroup* bg)
    {
        if (!mSourceData.empty())
            bg->AddRenderUnit(this);
    }

    void DiSimpleShape::CreateSphere(float SPHERE_RADIUS, int segments, int rings)
    {
        ReleaseSourceData();
        ReleaseIndexBuffer();
        ReleaseVertexDeclaration();

        int NUM_SEGMENTS  = DiMath::Max(segments,3);
        int NUM_RINGS = DiMath::Max(rings,2);

        auto vb = Driver->CreateVertexBuffer();
        if (!vb)
            return;

        mSourceData.push_back(vb);
        mIndexBuffer = Driver->CreateIndexBuffer();

        mVertexDecl = Driver->CreateVertexDeclaration();
        if (mVertexDecl)
        {
            mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_POSITION);
            mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_NORMAL);
            mVertexDecl->AddElement(0, VERT_TYPE_FLOAT2, VERT_USAGE_TEXCOORD);
            mVertexDecl->Create();
        }

        mPrimitiveType = PT_TRIANGLELIST;

        mSourceData[0]->Release();
        mIndexBuffer->Release();
        mAABB.SetNull();

        mVerticesNum = (NUM_SEGMENTS + 1) * (NUM_RINGS+1);
        mPrimitiveCount = 2 * NUM_RINGS * (NUM_SEGMENTS + 1);

        // data

        float fDeltaRingAngle = (DiMath::PI / NUM_RINGS);
        float fDeltaSegAngle = (2 * DiMath::PI / NUM_SEGMENTS);
        unsigned short wVerticeIndex = 0 ;

        uint32 vbsize = mVerticesNum * sizeof(float) * (3 + 3 + 2);
        mSourceData[0]->Create(vbsize);
        float* vBuffer = DI_NEW float[mVerticesNum * (3 + 3 + 2)];
        float* pVertex = vBuffer;

        uint32 ibsize = mPrimitiveCount * 3 * sizeof(uint16);
        mIndexBuffer->Create(ibsize);
        uint16* iBuffer = DI_NEW uint16[mPrimitiveCount * 3];
        uint16* pIndices = iBuffer;

        for( int ring = 0; ring <= NUM_RINGS; ring++ ) 
        {
            float r0 = SPHERE_RADIUS * sinf(ring * fDeltaRingAngle);
            float y0 = SPHERE_RADIUS * cosf(ring * fDeltaRingAngle);

            for(int seg = 0; seg <= NUM_SEGMENTS; seg++) 
            {
                float x0 = r0 * sinf(seg * fDeltaSegAngle);
                float z0 = r0 * cosf(seg * fDeltaSegAngle);

                *pVertex++ = x0;
                *pVertex++ = y0;
                *pVertex++ = z0;

                DiVec3 vNormal = DiVec3(x0, y0, z0).normalisedCopy();
                *pVertex++ = vNormal.x;
                *pVertex++ = vNormal.y;
                *pVertex++ = vNormal.z;

                *pVertex++ = (float) seg / (float) NUM_SEGMENTS;
                *pVertex++ = (float) ring / (float) NUM_RINGS;

                if (ring != NUM_RINGS) 
                {
                    *pIndices++ = wVerticeIndex + NUM_SEGMENTS + 1;
                    *pIndices++ = wVerticeIndex;               
                    *pIndices++ = wVerticeIndex + NUM_SEGMENTS;
                    *pIndices++ = wVerticeIndex + NUM_SEGMENTS + 1;
                    *pIndices++ = wVerticeIndex + 1;
                    *pIndices++ = wVerticeIndex;
                    wVerticeIndex ++;
                }
            }; // end for seg
        }
        
        mSourceData[0]->WriteData(0, vbsize, vBuffer);
        DI_DELETE[] vBuffer;
        mSourceData[0]->SetStride(8 * sizeof(float));
        
        mIndexBuffer->WriteData(0, ibsize, iBuffer);
        DI_DELETE[] iBuffer;

        mAABB.SetMaximum(SPHERE_RADIUS,SPHERE_RADIUS,SPHERE_RADIUS);
        mAABB.SetMinimum(-SPHERE_RADIUS,-SPHERE_RADIUS,-SPHERE_RADIUS);
    }

    void DiSimpleShape::CreateTorus(float radius, float tube, int radialSegments, int tubularSegments, float arc)
    {
        ReleaseSourceData();
        ReleaseIndexBuffer();
        ReleaseVertexDeclaration();

        auto vb = Driver->CreateVertexBuffer();
        if (!vb)
            return;

        mSourceData.push_back(vb);
        mIndexBuffer = Driver->CreateIndexBuffer();

        mVertexDecl = Driver->CreateVertexDeclaration();
        mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_POSITION);
        mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_NORMAL);
        mVertexDecl->AddElement(0, VERT_TYPE_FLOAT2, VERT_USAGE_TEXCOORD);
        mVertexDecl->Create();

        mPrimitiveType = PT_TRIANGLELIST;

        mSourceData[0]->Release();
        mIndexBuffer->Release();
        mAABB.SetNull();

        mVerticesNum = (radialSegments + 1) * (tubularSegments + 1);
        mPrimitiveCount = 2 * radialSegments * tubularSegments;

        uint32 vbsize = mVerticesNum * sizeof(float) * (3 + 3 + 2);
        mSourceData[0]->Create(vbsize);

        float* vBuffer = DI_NEW float[mVerticesNum * (3 + 3 + 2)];
        float* pVertex = vBuffer;

        DiVec3 center(0,0,0);

        for ( int j = 0; j <= radialSegments; j++ )
        {
            for ( int i = 0; i <= tubularSegments; i++ ) 
            {
                float u = i / (float)tubularSegments * arc;
                float v = j / (float)radialSegments * DiMath::PI * 2;

                center.x = radius * DiMath::Cos( u );
                center.y = radius * DiMath::Sin( u );

                DiVec3 vertex;
                vertex.x = ( radius + tube * DiMath::Cos( v ) ) * DiMath::Cos( u );
                vertex.y = ( radius + tube * DiMath::Cos( v ) ) * DiMath::Sin( u );
                vertex.z = tube * DiMath::Sin( v );

                DiVec3 normal = vertex - center;
                normal.normalise();

                *pVertex++ = vertex.x;
                *pVertex++ = vertex.y;
                *pVertex++ = vertex.z;
                
                *pVertex++ = normal.x;
                *pVertex++ = normal.y;
                *pVertex++ = normal.z;

                *pVertex++ = i / (float)tubularSegments;
                *pVertex++ = j / (float)radialSegments;

            }
        }

        mSourceData[0]->WriteData(0, vbsize, vBuffer);
        DI_DELETE[] vBuffer;
        mSourceData[0]->SetStride((3 + 3 + 2) * sizeof(float));
        
        uint32 ibsize = mPrimitiveCount * 3 * sizeof(uint16);
        mIndexBuffer->Create(ibsize);
        uint16* iBuffer = DI_NEW uint16[mPrimitiveCount * 3];
        uint16* pIndices = iBuffer;

        for ( uint16 j = 1; j <= radialSegments; j++ ) 
        {
            for ( uint16 i = 1; i <= tubularSegments; i++ ) 
            {

                uint16 a = ( tubularSegments + 1 ) * j + i - 1;
                uint16 b = ( tubularSegments + 1 ) * ( j - 1 ) + i - 1;
                uint16 c = ( tubularSegments + 1 ) * ( j - 1 ) + i;
                uint16 d = ( tubularSegments + 1 ) * j + i;

                *pIndices++ = a;
                *pIndices++ = b;
                *pIndices++ = d;
                *pIndices++ = b;
                *pIndices++ = c;
                *pIndices++ = d;
            }
        }

        mIndexBuffer->WriteData(0, ibsize, iBuffer);
        DI_DELETE[] iBuffer;

        mAABB.SetMaximum(radius,radius,radius);
        mAABB.SetMinimum(-radius,-radius,-radius);
    }

    void DiSimpleShape::CreatePlane(float sizeX, float sizeY)
    {
        ReleaseSourceData();
        ReleaseIndexBuffer();
        ReleaseVertexDeclaration();

        mSourceData.push_back(Driver->CreateVertexBuffer());
        mIndexBuffer = Driver->CreateIndexBuffer();

        mVertexDecl = Driver->CreateVertexDeclaration();
        mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_POSITION);
        mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_NORMAL);
        mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_TANGENT);
        mVertexDecl->AddElement(0, VERT_TYPE_FLOAT2, VERT_USAGE_TEXCOORD);
        mVertexDecl->Create();

        mPrimitiveType = PT_TRIANGLELIST;

        mSourceData[0]->Release();
        mIndexBuffer->Release();
        mAABB.SetNull();

        sizeX /= 2;
        sizeY /= 2;

        const int vtElements = 4 * (3+3+3+2);

        float vertices[vtElements] =
        {
            -sizeX, 0, -sizeY,  // pos
            0, 1, 0,            // normal
            1, 0, 0,            // tangent
            0, 1,              // texcoord
            sizeX, 0,-sizeY,
            0,1,0,
            1, 0, 0,
            1, 1,
            sizeX, 0,sizeY,
            0,1,0,
            1, 0, 0,
            1, 0,
            -sizeX, 0, sizeY ,
            0,1,0,
            1, 0, 0,
            0, 0
        };

        unsigned short faces[6] = {0,2,1,
            0,3,2 };

        mVerticesNum = 4;
        mPrimitiveCount = 2;

        uint32 vbsize = vtElements*sizeof(float);
        mSourceData[0]->Create(vbsize);
        mSourceData[0]->WriteData(0, vbsize, vertices);
        mSourceData[0]->SetStride((3 + 3 + 3 + 2) * sizeof(float));

        uint32 ibsize = 6*sizeof(uint16);
        mIndexBuffer->Create(ibsize);
        mIndexBuffer->WriteData(0, ibsize, faces);

        mAABB.SetMaximum(sizeX,0,sizeY);
        mAABB.SetMinimum(-sizeX,0,-sizeY);
    }

    void DiSimpleShape::CreateBox( float size /*= 100*/ )
    {
        ReleaseSourceData();
        ReleaseIndexBuffer();
        ReleaseVertexDeclaration();

        mSourceData.push_back(Driver->CreateVertexBuffer());
        mIndexBuffer = Driver->CreateIndexBuffer();

        mVertexDecl = Driver->CreateVertexDeclaration();
        mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_POSITION);
        mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_NORMAL);
        mVertexDecl->AddElement(0, VERT_TYPE_FLOAT2, VERT_USAGE_TEXCOORD);
        mVertexDecl->Create();

        mPrimitiveType = PT_TRIANGLELIST;

        mSourceData[0]->Release();
        mIndexBuffer->Release();
        mAABB.SetNull();

        const int NUM_VERTICES = 4 * 6;
        const int NUM_ENTRIES_PER_VERTEX = 8;
        const int NUM_VERTEX_ENTRIES = NUM_VERTICES * NUM_ENTRIES_PER_VERTEX;
        const int NUM_INDICES = 3 * 2 * 6;

        float CUBE_HALF_SIZE = size/2;
        float vertices[NUM_VERTEX_ENTRIES] = 
        {
            -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE,
            0,0,1,
            0,1,
            CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE,
            0,0,1,
            1,1,
            CUBE_HALF_SIZE,  CUBE_HALF_SIZE, CUBE_HALF_SIZE,
            0,0,1,
            1,0,
            -CUBE_HALF_SIZE,  CUBE_HALF_SIZE, CUBE_HALF_SIZE ,
            0,0,1,
            0,0,

            CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            0,0,-1,
            0,1,
            -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            0,0,-1,
            1,1,
            -CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            0,0,-1,
            1,0,
            CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            0,0,-1,
            0,0,

            -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            -1,0,0,
            0,1,
            -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE,
            -1,0,0,
            1,1,
            -CUBE_HALF_SIZE, CUBE_HALF_SIZE, CUBE_HALF_SIZE,
            -1,0,0,
            1,0,
            -CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            -1,0,0,
            0,0, 

            CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE,
            1,0,0,
            0,1,
            CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            1,0,0,
            1,1,
            CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            1,0,0,
            1,0,
            CUBE_HALF_SIZE, CUBE_HALF_SIZE, CUBE_HALF_SIZE,
            1,0,0,
            0,0,

            -CUBE_HALF_SIZE, CUBE_HALF_SIZE, CUBE_HALF_SIZE,
            0,1,0,
            0,1,
            CUBE_HALF_SIZE, CUBE_HALF_SIZE, CUBE_HALF_SIZE,
            0,1,0,
            1,1,
            CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            0,1,0,
            1,0,
            -CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            0,1,0,
            0,0,

            -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            0,-1,0,
            0,1,
            CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            0,-1,0,
            1,1,
            CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE,
            0,-1,0,
            1,0,
            -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE,
            0,-1,0,
            0,0 
        };

        uint32 vbsize = NUM_VERTEX_ENTRIES*sizeof(float);
        mSourceData[0]->Create(vbsize);
        mSourceData[0]->WriteData(0, vbsize, vertices);
        mSourceData[0]->SetStride(NUM_ENTRIES_PER_VERTEX * sizeof(float));

        uint16 faces[NUM_INDICES] = 
        {
            0,1,2,
            0,2,3,
            4,5,6,
            4,6,7,
            8,9,10,
            8,10,11,
            12,13,14,
            12,14,15,
            16,17,18,
            16,18,19,
            20,21,22,
            20,22,23
        };

        uint32 ibsize = NUM_INDICES*sizeof(uint16);
        mIndexBuffer->Create(ibsize);
        mIndexBuffer->WriteData(0, ibsize, faces);

        mVerticesNum = NUM_VERTICES;
        mPrimitiveCount = NUM_INDICES / 3;

        mAABB.SetMaximum(CUBE_HALF_SIZE,CUBE_HALF_SIZE,CUBE_HALF_SIZE);
        mAABB.SetMinimum(-CUBE_HALF_SIZE,-CUBE_HALF_SIZE,-CUBE_HALF_SIZE);
    }

    void DiSimpleShape::GetWorldTransform( DiMat4* xform ) const
    {
        *xform = GetTransform();
    }
    
    void DiSimpleShape::CreateColorBox( float size, const DiColor& color )
    {
        ReleaseSourceData();
        ReleaseIndexBuffer();
        ReleaseVertexDeclaration();

        mSourceData.push_back(Driver->CreateVertexBuffer());
        mIndexBuffer = Driver->CreateIndexBuffer();

        mVertexDecl = Driver->CreateVertexDeclaration();
        mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_POSITION);
        mVertexDecl->AddElement(0, VERT_TYPE_COLOR,  VERT_USAGE_COLOR);
        mVertexDecl->Create();

        mPrimitiveType = PT_TRIANGLELIST;

        mSourceData[0]->Release();
        mIndexBuffer->Release();
        mAABB.SetNull();

        struct Vert
        {
            float x,y,z;
            ARGB col;
        };

        const int NUM_VERTICES = 4 * 6;
        const int STRIDE = sizeof(float)*3 + sizeof(ARGB);
        const int NUM_INDICES = 3 * 2 * 6;

        uint32 col = DiBase::Driver->GetNativeColor(color);

        float CUBE_HALF_SIZE = size/2;
        Vert vertices[NUM_VERTICES] = 
        {
            {-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE,
            col},
            {CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE,
            col},
            {CUBE_HALF_SIZE,  CUBE_HALF_SIZE, CUBE_HALF_SIZE,
            col},
            {-CUBE_HALF_SIZE,  CUBE_HALF_SIZE, CUBE_HALF_SIZE ,
            col},

            {CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            col},
            {-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            col},
            {-CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            col},
            {CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            col},

            {-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            col},
            {-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE,
            col},
            {-CUBE_HALF_SIZE, CUBE_HALF_SIZE, CUBE_HALF_SIZE,
            col},
            {-CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            col}, 

            {CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE,
            col},
            {CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            col},
            {CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            col},
            {CUBE_HALF_SIZE, CUBE_HALF_SIZE, CUBE_HALF_SIZE,
            col},

            {-CUBE_HALF_SIZE, CUBE_HALF_SIZE, CUBE_HALF_SIZE,
            col},
            {CUBE_HALF_SIZE, CUBE_HALF_SIZE, CUBE_HALF_SIZE,
            col},
            {CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            col},
            {-CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            col},

            {-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            col},
            {CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE,
            col},
            {CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE,
            col},
            {-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE,
            col} 
        };

        uint32 vbsize = STRIDE*NUM_VERTICES;
        mSourceData[0]->Create(vbsize);
        mSourceData[0]->WriteData(0, vbsize, vertices);
        mSourceData[0]->SetStride(STRIDE);

        uint16 faces[NUM_INDICES] = 
        {
            0,1,2,
            0,2,3,
            4,5,6,
            4,6,7,
            8,9,10,
            8,10,11,
            12,13,14,
            12,14,15,
            16,17,18,
            16,18,19,
            20,21,22,
            20,22,23
        };

        uint32 ibsize = NUM_INDICES*sizeof(uint16);
        mIndexBuffer->Create(ibsize);
        mIndexBuffer->WriteData(0, ibsize, faces);

        mVerticesNum = NUM_VERTICES;
        mPrimitiveCount = NUM_INDICES / 3;

        mAABB.SetMaximum(CUBE_HALF_SIZE,CUBE_HALF_SIZE,CUBE_HALF_SIZE);
        mAABB.SetMinimum(-CUBE_HALF_SIZE,-CUBE_HALF_SIZE,-CUBE_HALF_SIZE);
    }

    DiString& DiSimpleShape::GetType()
    {
        static DiString type = "SimpleShape";
        return type;
    }
}
