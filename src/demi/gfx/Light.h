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

#ifndef DiLight_h__
#define DiLight_h__


#include "TransformUnit.h"

namespace Demi 
{
    class DI_GFX_API DiLight : public DiTransformUnit
    {
    public:

        enum Type
        {
            TYPE_DIRECTIONAL = 0,
            TYPE_POINT,
            TYPE_SPOT,
            TYPE_SKY,

            NUM_TYPES
        };

    protected:

        friend class DiRenderPipeline;
        friend class DiSceneManager;

        DiLight(Type type, DiSceneManager* mgr);

        virtual         ~DiLight(void);

    public:

        DiLight::Type   GetType() const { return mType; }

        const DiColor&  GetColor(void) const{return mColor;}

        void            SetColor(const DiColor &color);

    protected:

        virtual void    Update(void) const;

    protected:

        Type            mType;

        DiColor         mColor;

        DiSceneManager* mCreator;

        mutable DiVec3  mDerivedPosition;

        mutable DiVec3  mDerivedDirection;
        
        mutable DiVec3  mDerivedCamRelativePosition;
        
        mutable bool    mDerivedCamRelativeDirty;
        
        mutable bool    mDerivedTransformDirty;

        DiCamera*       mCameraToBeRelativeTo;

        DiVec3          mDirection;
    };

}

#endif
