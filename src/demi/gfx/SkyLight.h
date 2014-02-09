
/********************************************************************
    File:       SkyLight.h
    Creator:    demiwangya
*********************************************************************/

#ifndef SkyLight_h__
#define SkyLight_h__

#include "light.h"

namespace Demi 
{
    class DI_GFX_API DiSkyLight : public DiLight
    {
    public:

        friend class DiSceneManager;

    protected:

        DiSkyLight(DiSceneManager* mgr);

        virtual                 ~DiSkyLight();

    public:

        void                     SetPosition(const DiVec3& vec)
        {
            mPosition = vec;
        }

        DiVec3                   GetPosition() const {return mPosition;}

        virtual const DiAABB&    GetBoundingBox(void) const;

        virtual DiString&        GetType();

    protected:

        DiVec3                   mPosition;
    };

}

#endif // SkyLight_h__