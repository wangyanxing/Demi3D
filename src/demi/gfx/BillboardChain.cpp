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

#include "BillboardChain.h"
#include "CullNode.h"
#include "RenderBatchGroup.h"
#include "VertexBuffer.h"
#include "VertexDeclaration.h"

namespace Demi
{
    EffectBillboardChainElement::EffectBillboardChainElement()
        : position(DiVec3::ZERO)
        , colour(DiColor::Black)
    {
    }
    
    EffectBillboardChainElement::EffectBillboardChainElement(DiVec3 _position,
                                                             float _width,
                                                             float _uTexCoord,
                                                             DiColor _colour)
        :position(_position),
        width(_width),
        uTexCoord(_uTexCoord),
        colour(_colour)
    {
    }
    
    DiBillboardChain::DiBillboardChain(int maxNbChainElements)
    {
        mCurrentNbChainElements = maxNbChainElements;
        
        CreateBuffer();
    }
    
    DiBillboardChain::~DiBillboardChain()
    {
        ReleaseSourceData();
        ReleaseVertexDeclaration();
    }
    
    const DiAABB& DiBillboardChain::GetBoundingBox(void) const
    {
        return mBounds;
    }
    
    void DiBillboardChain::Update(DiCamera* camera)
    {
        UpdateHardwareBuffers(camera);
    }
    
    void DiBillboardChain::AddToBatchGroup(DiRenderBatchGroup* bg)
    {
        bg->AddRenderUnit(this);
    }
    
    float DiBillboardChain::GetBoundingRadius(void) const
    {
        return mRadius;
    }
    
    void DiBillboardChain::SetNbChainElements(unsigned int nbChainElements)
    {
        mCurrentNbChainElements = nbChainElements;
        
        CreateBuffer();
        
        mChainElementList.resize(mCurrentNbChainElements);
    }
    
    void DiBillboardChain::SetChainElement(unsigned int elementIndex, const EffectBillboardChainElement& billboardChainElement)
    {
        mChainElementList[elementIndex] = billboardChainElement;
    }
    
    void DiBillboardChain::UpdateBoundingBox()
    {
        if (mChainElementList.size() < 2)
            return;
        
        float width = mChainElementList[0].width;
        DiVec3 widthVector = DiVec3(width, width, width);
        const DiVec3& position = mChainElementList[0].position;
        DiVec3 minimum = position - widthVector;
        DiVec3 maximum = position + widthVector;
        
        for (unsigned int i = 1; i < mChainElementList.size(); i++)
        {
            // Update the bounds of the bounding box
            //float width = mChainElementList[i].width;
            //DiVec3 widthVector = DiVec3(width, width, width);
            const DiVec3& pos = mChainElementList[i].position;
            
            minimum.makeFloor(pos - widthVector);
            maximum.makeCeil(pos + widthVector);
        }
        
        // Set the current bounding box
        mBounds.SetExtents(minimum, maximum);
        
        // Set the current radius
        mRadius = DiMath::Sqrt(std::max(minimum.squaredLength(), maximum.squaredLength()));
    }
    
    void DiBillboardChain::UpdateHardwareBuffers(DiCamera* camera)
    {
        if (mChainElementList.size() < 2)
            return;
        
        void* pBufferStart = mSourceData[0]->Lock(0, mVBSize);
        
        const DiVec3& camPos = camera->GetDerivedPosition();
        int stride = sizeof(DiVec3) + sizeof(uint32) + sizeof(DiVec2);

        DI_ASSERT(mParentNode);
        DiVec3 eyePos = mParentNode->GetDerivedOrientation().Inverse() *
            (camPos - mParentNode->GetDerivedPosition()) / mParentNode->GetDerivedScale();
        
        unsigned int chainSize = mChainElementList.size();
        for (unsigned int i = 0; i < chainSize; i++)
        {
            void* pBase = static_cast<void*>(static_cast<char*>(pBufferStart) +
                                             stride * i * 2);
            
            DiVec3 chainTangent;
            if (i == 0)
            {
                chainTangent = mChainElementList[1].position - mChainElementList[0].position;
            }
            else if (i == chainSize - 1)
            {
                chainTangent = mChainElementList[chainSize - 1].position - mChainElementList[chainSize - 2].position;
            }
            else
            {
                chainTangent = mChainElementList[i + 1].position - mChainElementList[i - 1].position;
            }
            
            const DiVec3& p1 = mChainElementList[i].position;
            
            DiVec3 vP1ToEye = eyePos - p1;
            DiVec3 vPerpendicular = chainTangent.crossProduct(vP1ToEye);
            vPerpendicular.normalise();
            vPerpendicular *= mChainElementList[i].width;
            
            DiVec3 pos0 = p1 - vPerpendicular;
            DiVec3 pos1 = p1 + vPerpendicular;
            
            // vertex 1
            // pos
            float* pFloat = static_cast<float*>(pBase);
            
            *pFloat++ = pos0.x;
            *pFloat++ = pos0.y;
            *pFloat++ = pos0.z;
            
            // colour
            DiColor colour = mChainElementList[i].colour;
            RGBA* pColour = static_cast<RGBA*>( static_cast<void*>(pFloat) );
            *pColour = Driver->GetNativeColor(colour);
            pColour++;
            
            // tex coord
            pFloat = static_cast<float*>( static_cast<void*>(pColour) );
            *pFloat++ = mChainElementList[i].uTexCoord;
            *pFloat++ = 0.0f;
            
            // vertex 2
            // pos
            *pFloat++ = pos1.x;
            *pFloat++ = pos1.y;
            *pFloat++ = pos1.z;
            
            // colour
            pColour = static_cast<RGBA*>( static_cast<void*>(pFloat) );
            *pColour = Driver->GetNativeColor(colour);
            pColour++;
            
            // tex coord
            pFloat = static_cast<float*>( static_cast<void*>(pColour) );
            *pFloat++ = mChainElementList[i].uTexCoord;
            *pFloat++ = 1.0f;
        }
        
        mSourceData[0]->Unlock();
    }
    
    void DiBillboardChain::CreateBuffer(void)
    {
        if(!mVertexDecl)
        {
            mVertexDecl = Driver->CreateVertexDeclaration();
            mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_POSITION);
            mVertexDecl->AddElement(0, VERT_TYPE_COLOR,  VERT_USAGE_COLOR);
            mVertexDecl->AddElement(0, VERT_TYPE_FLOAT2, VERT_USAGE_TEXCOORD);
            mVertexDecl->Create();
        }
        
        mPrimitiveType = PT_TRIANGLESTRIP;
        
        mVerticesNum = mCurrentNbChainElements * 2;
        int stride = sizeof(DiVec3) + sizeof(uint32) + sizeof(DiVec2);
        mVBSize = stride * mVerticesNum;
        
        if (!mSourceData.empty())
        {
            ReleaseSourceData();
        }

        mSourceData.push_back(Driver->CreateVertexBuffer());
        mSourceData[0]->SetStride(stride);
        mSourceData[0]->Create(mVBSize);
    }
    
    EffectBillboardChainElement DiBillboardChain::GetChainElement( unsigned int id )
    {
        return mChainElementList[id];
    }
    
    void DiBillboardChain::GetWorldTransform( DiMat4* xform ) const
    {
        *xform = GetTransform();
    }
}
