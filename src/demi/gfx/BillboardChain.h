
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

#ifndef _EffectBillboardChain_H__
#define _EffectBillboardChain_H__

#include "GfxPrerequisites.h"
#include "TransformUnit.h"
#include "RenderUnit.h"

namespace Demi
{
    /** Contains the data of an element of the DiBillboardChain.
     */
    class EffectBillboardChainElement
    {
        
    public:
        
        EffectBillboardChainElement();
        
        EffectBillboardChainElement(DiVec3 position,
                                    float width,
                                    float uTexCoord,
                                    DiColor colour);
        
        DiVec3 position;
        float width{ 0 };
        float uTexCoord{ 0 };
        DiColor colour;
        
    };
    
    /** Allows the rendering of a chain of connected billboards.
     */
    class DiBillboardChain : public DiRenderUnit, public DiTransformUnit
    {
        
    public:
        
        typedef DiVector<EffectBillboardChainElement> EffectBillboardChainElementList;
        
        DiBillboardChain(int maxNbChainElements = 10);
        
        virtual         ~DiBillboardChain();
        
        void            GetWorldTransform(DiMat4* xform) const;
        
        const DiAABB&   GetBoundingBox(void) const;
        
        void            Update(DiCamera* camera);
        
        void            AddToBatchGroup(DiRenderBatchGroup* bg);
        
        void            UpdateHardwareBuffers(DiCamera* camera);
        
        virtual float   GetBoundingRadius(void) const;
        
        void            SetNbChainElements(unsigned int nbChainElements);
        
        void            SetChainElement(unsigned int elementIndex, const EffectBillboardChainElement& billboardChainElement);
        
        EffectBillboardChainElement GetChainElement(unsigned int id);
        
        void            UpdateBoundingBox();
        
    protected:
        
        void            CreateBuffer(void);
        
    protected:
        
        float           mRadius{ 0 };
        
        DiAABB          mBounds;
        
        int             mCurrentNbChainElements{ 0 };
        
        EffectBillboardChainElementList mChainElementList;
        
        uint32          mVBSize{ 0 };
        
        void            updateHardwareBuffers();
    };
    
} // namespace

#endif

