
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

/****************************************************
  This module was originally from Particle Universe
  Repo: https://github.com/scrawl/particleuniverse
  License: MIT 
****************************************************/

#ifndef BillboardRender_h__
#define BillboardRender_h__

#include "K2Prerequisites.h"
#include "BillboardSet.h"
#include "ParticleRenderer.h"
#include "ParticleRendererFactory.h"

namespace Demi
{
    class DEMI_K2_API DiBillboardRenderer : public DiParticleRenderer
    {
    public:

        static const BillboardType          DEFAULT_BILLBOARD_TYPE;
        static const bool                   DEFAULT_ACCURATE_FACING;
        static const BillboardOrigin        DEFAULT_ORIGIN;
        static const BillboardRotationType  DEFAULT_ROTATION_TYPE;
        static const DiVec3                 DEFAULT_COMMON_DIRECTION;
        static const DiVec3                 DEFAULT_COMMON_UP_VECTOR;
        static const bool                   DEFAULT_POINT_RENDERING;

        DiBillboardRenderer(void);

        virtual                      ~DiBillboardRenderer(void);

    public:

        virtual void                 Prepare(DiParticleElement* ele);

        virtual void                 Unprepare(DiParticleElement* ele);

        void                         SetBillboardType(BillboardType bbt);

        BillboardType                GetBillboardType(void) const;

        void                         SetBillboardOrigin(BillboardOrigin origin);

        BillboardOrigin              GetBillboardOrigin(void) const;

        void                         SetBillboardRotationType(BillboardRotationType rotationType);

        BillboardRotationType        GetBillboardRotationType(void) const;

        void                         SetCommonDirection(const DiVec3& vec);

        const DiVec3&                GetCommonDirection(void) const;

        void                         SetCommonUpVector(const DiVec3& vec);

        const DiVec3&                GetCommonUpVector(void) const;
        
        virtual void                 Update(DiCamera*, DiParticlePool*);
        
        virtual void                 AddToBatchGroup(DiRenderBatchGroup*);

        virtual void                 NotifyAttached(DiNode* parent);

        virtual void                 SetMaterialName(const DiString& materialName);

        virtual void                 NotifyCurrentCamera(DiCamera* cam);

        virtual void                 NotifyParticleQuota(size_t quota);

        virtual void                 NotifyDefaultDimensions(float width, float height, float depth);

        virtual void                 NotifyParticleResized(void);

        virtual void                 NotifyParticleZRotated(void);

        virtual void                 SetBatchGroupID(uint8 queueId);

        virtual DiSortMode           GetSortMode(void) const;

        virtual void                 CopyTo (DiParticleRenderer* renderer);

        DiBillboardSetPtr            GetBillboardSet(void) const { return mBillboardSet; }

        virtual void                 SetVisible(bool visible);

    protected:

        DiBillboardSetPtr            mBillboardSet;

        BillboardType                mBillboardType;
    };

    //////////////////////////////////////////////////////////////////////////

    class DEMI_K2_API DiBillboardRendererFactory : public DiParticleRendererFactory
    {
    public:
        DiBillboardRendererFactory(void) {}

        virtual    ~DiBillboardRendererFactory(void) {}

        DiString   GetRendererType(void) const
        {
            static DiString type = "Billboard";
            return type;
        }

        DiParticleRenderer* CreateRenderer(void)
        {
            return CreateRendererImpl<DiBillboardRenderer>();
        }
    };
}

#endif // BillboardRender_h__
