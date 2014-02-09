
/********************************************************************
    File:       PointLight.h
    Creator:    demiwangya
*********************************************************************/

#ifndef PointLight_h__
#define PointLight_h__

#include "light.h"

namespace Demi 
{
    class DI_GFX_API DiPointLight : public DiLight
    {
    public:

        friend class DiSceneManager;

    protected:

        DiPointLight(DiSceneManager* mgr);

        virtual                 ~DiPointLight();

    public:

        void                     SetPosition(const DiVec3& vec)
        {
            mPosition = vec;
        }

        DiVec3                   GetPosition() const {return mPosition;}

        virtual const DiAABB&    GetBoundingBox(void) const;

        virtual DiString&        GetType();

        void                     SetAttenuation(float end, float begin = 0)
        {
            mAttenuationBegin = begin;
            mAttenuationEnd = end;
        }

        float                    GetAttenuationBegin() const { return mAttenuationBegin; }

        float                    GetAttenuationEnd() const { return mAttenuationEnd; }

    protected:

        DiVec3                   mPosition;
        
        float                    mAttenuationBegin;

        float                    mAttenuationEnd;

        mutable DiAABB           mAABB;
    };

}

#endif // PointLight_h__