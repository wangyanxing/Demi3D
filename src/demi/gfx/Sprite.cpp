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
#include "Sprite.h"
#include "GfxDriver.h"
#include "RenderTarget.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexDeclaration.h"
#include "RenderWindow.h"
#include "GfxDriver.h"

namespace Demi
{
    DiQuadElement::DiQuadElement( const DiColor& col )
    {
        uint32 c = col.GetAsByte4();
        topLeftColor     = c;
        topRightColor    = c;
        bottomLeftColor  = c;
        bottomRightColor = c;

        absCoord = true;

        left     = 0;
        top      = 0;
        right    = 0;
        bottom   = 0;
                 
        leftUv   = 0;
        topUv    = 0;
        rightUv  = 1;
        bottomUv = 1;
    }

    DiQuadElement::DiQuadElement()
    {
        uint32 c = DiColor::White.GetAsByte4();
        topLeftColor     = c;
        topRightColor    = c;
        bottomLeftColor  = c;
        bottomRightColor = c;

        absCoord = true;

        left     = 0;
        top      = 0;
        right    = 0;
        bottom   = 0;
                  
        leftUv   = 0;
        topUv    = 0;
        rightUv  = 1;
        bottomUv = 1;
    }

    /////////////////////////////////////////////////////////////////////////

    DiSprite::DiSprite()
    {
        mVertexDecl = Driver->CreateVertexDeclaration();
        if (mVertexDecl)
        {
            mVertexDecl->AddElement(0,VERT_TYPE_FLOAT3, VERT_USAGE_POSITION);
            mVertexDecl->AddElement(0,VERT_TYPE_COLOR,  VERT_USAGE_COLOR);
            mVertexDecl->AddElement(0,VERT_TYPE_FLOAT2, VERT_USAGE_TEXCOORD);
            mVertexDecl->Create();
        }

        mPrimitiveType = PT_TRIANGLELIST;

        mIdentityProjection = true;
    
        mGroupType = BATCH_2D;
    }

    DiSprite::~DiSprite()
    {
        ReleaseSourceData();
        ReleaseVertexDeclaration();
    }

    void DiSprite::GetWorldTransform( DiMat4* xform ) const
    {
        *xform = DiMat4::IDENTITY;
    }

    const DiAABB& DiSprite::GetBoundingBox( void ) const
    {
        return DiAABB::BOX_INFINITE;
    }

    void DiSprite::AddToBatchGroup(DiRenderBatchGroup* bg)
    {
        if (!mSourceData.empty())
            bg->AddRenderUnit(this);
    }

    void DiSprite::Update(DiCamera* camera)
    {
        DI_ASSERT(DiRenderWindow::ActiveWindow);
        DiRenderTarget* rt = DiRenderWindow::ActiveWindow->GetRenderBuffer();
        UpdateGeometry(rt);
    }

    void DiSprite::UpdateGeometry(DiRenderTarget* rt)
    {
        if (mQuads.empty())
        {
            mPrimitiveCount = 0;
            return;
        }

        if (!rt)
            return;

        ReleaseSourceData();

        auto vb = Driver->CreateVertexBuffer();
        if (!vb)
            return;

        mSourceData.push_back(vb);

        mVerticesNum = 6 * mQuads.size();
        mPrimitiveCount = 2 * mQuads.size();
        uint32 vertSize = (3 + 2) * sizeof(float) + sizeof(ARGB);

        mSourceData[0]->Create(mVerticesNum*vertSize);
        void* lockedData = mSourceData[0]->Lock(0,mVerticesNum*vertSize);

        uint32 w = (uint32)(rt->GetWidth() * rt->GetViewport().mWidth);
        uint32 h = (uint32)(rt->GetHeight() * rt->GetViewport().mHeight);

        DiVec2 invScreenSize;
        invScreenSize = DiVec2(1.0f / (float)w, 1.0f / (float)h);
    
        DiVec2 posAdjust(1, -1);

        float* dest = (float*)lockedData;
        if (!mMaterial || mMaterial->HasTexture())
        {
            for (auto it = mQuads.begin(); it != mQuads.end(); ++it)
            {
                DiQuadElement quad = *it;

                DiVec2 topLeft, bottomRight, topLeftUv, bottomRightUv;

                quad.top *= -1;
                quad.bottom *= -1;

                if (!quad.absCoord)
                {
                    quad.left *= (float)w;
                    quad.right *= (float)w;
                    quad.top *= (float)h;
                    quad.bottom *= (float)h;
                }

                topLeft = (DiVec2((float)quad.left, (float)quad.top)/*+DiVec2(0.5f,0.5f)*/) * invScreenSize*2;
                bottomRight = (DiVec2((float)quad.right, (float)quad.bottom)/*+DiVec2(0.5f,0.5f)*/) * invScreenSize*2;

                topLeft -= posAdjust;
                bottomRight -= posAdjust;

                topLeftUv = DiVec2((float)quad.leftUv, (float)quad.topUv);
                bottomRightUv = DiVec2((float)quad.rightUv, (float)quad.bottomUv);

                *dest++ = topLeft.x; *dest++ = topLeft.y; *dest++ = 0.0f;
                *((ARGB*)dest) = quad.topLeftColor; dest++;
                *dest++ = topLeftUv.x; *dest++ = topLeftUv.y;

                *dest++ = topLeft.x; *dest++ = bottomRight.y; *dest++ = 0.0f;
                *((ARGB*)dest) = quad.bottomLeftColor; dest++;
                *dest++ = topLeftUv.x; *dest++ = bottomRightUv.y;

                *dest++ = bottomRight.x; *dest++ = topLeft.y; *dest++ = 0.0f;
                *((ARGB*)dest) = quad.topRightColor; dest++;
                *dest++ = bottomRightUv.x; *dest++ = topLeftUv.y;

                *dest++ = bottomRight.x; *dest++ = topLeft.y; *dest++ = 0.0f;
                *((ARGB*)dest) = quad.topRightColor; dest++;
                *dest++ = bottomRightUv.x; *dest++ = topLeftUv.y;

                *dest++ = topLeft.x; *dest++ = bottomRight.y; *dest++ = 0.0f;
                *((ARGB*)dest) = quad.bottomLeftColor; dest++;
                *dest++ = topLeftUv.x; *dest++ = bottomRightUv.y;

                *dest++ = bottomRight.x; *dest++ = bottomRight.y; *dest++ = 0.0f;
                *((ARGB*)dest) = quad.bottomRightColor; dest++;
                *dest++ = bottomRightUv.x; *dest++ = bottomRightUv.y;
            }
        }
        else
        {
            for (auto it = mQuads.begin(); it != mQuads.end(); ++it)
            {
                DiQuadElement quad = *it;
                  quad.top *= -1;
                  quad.bottom *= -1;

                if (!quad.absCoord)
                {
                    quad.left *= (float)w;
                    quad.right *= (float)w;
                    quad.top *= (float)h;
                    quad.bottom *= (float)h;
                }

                DiVec2 topLeft, bottomRight, topLeftUV, bottomRightUV;
                topLeft = (DiVec2((float)quad.left, (float)quad.top)) * invScreenSize*2;
                bottomRight = (DiVec2((float)quad.right, (float)quad.bottom)) * invScreenSize*2;

                topLeft -= posAdjust;
                bottomRight -= posAdjust;

                *dest++ = topLeft.x; *dest++ = topLeft.y; *dest++ = 0.0f;
                *((ARGB*)dest) = quad.topLeftColor; dest++;
                dest += 2;

                *dest++ = topLeft.x; *dest++ = bottomRight.y; *dest++ = 0.0f;
                *((ARGB*)dest) = quad.bottomLeftColor; dest++;
                dest += 2;

                *dest++ = bottomRight.x; *dest++ = topLeft.y; *dest++ = 0.0f;
                *((ARGB*)dest) = quad.topRightColor; dest++;
                dest += 2;

                *dest++ = bottomRight.x; *dest++ = topLeft.y; *dest++ = 0.0f;
                *((ARGB*)dest) = quad.topRightColor; dest++;
                dest += 2;

                *dest++ = topLeft.x; *dest++ = bottomRight.y; *dest++ = 0.0f;
                *((ARGB*)dest) = quad.bottomLeftColor; dest++;
                dest += 2;

                *dest++ = bottomRight.x; *dest++ = bottomRight.y; *dest++ = 0.0f;
                *((ARGB*)dest) = quad.bottomRightColor; dest++;
                dest += 2;
            }
        }

        mSourceData[0]->Unlock();
        mSourceData[0]->SetStride(vertSize);
    }

    void DiSprite::AddQuad( const DiQuadElement& quad )
    {
        mQuads.push_back(quad);
    }

    DiString& DiSprite::GetType()
    {
        static DiString type = "Sprite";
        return type;
    }
}