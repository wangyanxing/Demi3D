
#include "GfxPch.h"
#include "LineList.h"
#include "AxisAlignedBox.h"
#include "AssetManager.h"
#include "Material.h"
#include "ShaderParam.h"
#include "GfxDriver.h"
#include "VertexBuffer.h"
#include "VertexDeclaration.h"
#include "Node.h"
#include "GfxDriver.h"

namespace Demi
{
    DiLineList::DiLineList(void)
        :DiTransformUnit(),DiRenderUnit()
    {
        Init();
    }

    DiLineList::DiLineList( const DiString& name )
        :DiTransformUnit(name),DiRenderUnit()
    {
        Init();
    }

    DiLineList::~DiLineList(void)
    {
        ReleaseSourceData();
        ReleaseVertexDeclaration();
    }

    void DiLineList::Init()
    {
        mMaxVerts           = 0;
        mLockedPositions    = 0;
        mPositionStride     = 0;
        mLockedColors       = 0;
        mColorStride        = 0;

        mSourceData.push_back(Driver->CreateVertexBuffer());

        mVertexDecl = Driver->CreateVertexDeclaration();
        mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_POSITION);
        mVertexDecl->AddElement(0, VERT_TYPE_COLOR,  VERT_USAGE_COLOR);
        mVertexDecl->Create();

        SetMaterial("unlighting_color.mtl");
    }

    void DiLineList::AddToBatchGroup(DiRenderBatchGroup* bg)
    {
        bg->AddRenderUnit(this);
    }

    void DiLineList::CullingUpdate( DiRenderBatchGroup* group,DiCamera*  )
    {
        group->AddRenderUnit(this);
    }

    void DiLineList::CheckResizeLine( size_t maxVerts )
    {
        if (maxVerts > mMaxVerts)
        {
            mMaxVerts = maxVerts + (size_t)(maxVerts*0.2f);

            mSourceData[0]->Release();
            mSourceData[0]->SetStride(sizeof(float) * 3 + sizeof(UINT8) * 4);
            mSourceData[0]->Create(mMaxVerts*mSourceData[0]->GetStride());

            mPrimitiveType = PT_LINELIST;
        }
    }

    const DiAABB& DiLineList::GetBoundingBox( void ) const
    {
        return DiAABB::BOX_INFINITE;
    }

    void DiLineList::ClearLine( void )
    {
        mVertices.clear();
        mVerticesNum = 0;
        mPrimitiveCount = 0;
    }

    void DiLineList::AddLine( const DiVec3 &p0,const DiVec3 &p1,const DiColor& color,bool nolock )
    {
        CheckResizeLine(mVertices.size() + 2);
        AddPoint(p0,color);
        AddPoint(p1,color);

        mVerticesNum    = mVertices.size();
        mPrimitiveCount = mVertices.size() / 2;

        if (!nolock)
        {
            void* data = mSourceData[0]->Lock(0,mVerticesNum*mSourceData[0]->GetStride());
            ::memcpy(data,&mVertices[0],mVerticesNum*mSourceData[0]->GetStride());
            mSourceData[0]->Unlock();
        }
    }

    void DiLineList::AddPoint( const DiVec3 &p, const DiColor &color )
    {
        DiLineList::Vertex v;
        v.pos = p;
        v.color = color.GetAsARGB();

        mVertices.push_back(v);
    }

    void DiLineList::GetWorldTransform( DiMat4* xform ) const
    {
        *xform = GetTransform();
    }

    void DiLineList::FlushSourceData()
    {
        if (!mVertices.empty())
        {
            FlushSourceData(0,GetLineNums()-1);
        }
    }

    void DiLineList::FlushSourceData( int beginLine, int endLine )
    {
        beginLine = DiMath::Clamp(beginLine,0,GetLineNums());
        endLine = DiMath::Clamp(endLine,0,GetLineNums());
        DI_ASSERT(beginLine <= endLine);

        int stride = mSourceData[0]->GetStride();
        int verNum = (endLine-beginLine+1)*2;

        void* data = mSourceData[0]->Lock(beginLine*2*stride,verNum*stride);
        ::memcpy(data,&mVertices[beginLine*2],verNum*stride);
        mSourceData[0]->Unlock();
    }

    int DiLineList::GetLineNums()
    {
        return (int)mVertices.size()/2;
    }

    void DiLineList::AlterLine( int id,const Demi::DiVec3 &p0,const Demi::DiVec3 &p1,const Demi::DiColor& color )
    {
        if (id > GetLineNums())
        {
            return;
        }

        mVertices[id*2].color   = color.GetAsARGB();
        mVertices[id*2].pos     = p0;
        mVertices[id*2+1].color = color.GetAsARGB();
        mVertices[id*2+1].pos   = p1;
    }

    DiString& DiLineList::GetType()
    {
        static DiString type = "LineList";
        return type;
    }
}