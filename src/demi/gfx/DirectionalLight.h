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

#ifndef DiDirectionalLight_h__
#define DiDirectionalLight_h__


#include "light.h"

namespace Demi 
{
    class DI_GFX_API DiDirLight : public DiLight
    {
    public:

        friend class DiSceneManager;

    protected:

        DiDirLight(DiSceneManager* mgr);

        virtual ~DiDirLight();

    public:

        virtual void             Bind(DiShaderEnvironment* env);

        void                     SetDirection(const DiVec3& vec){
            mDirection = vec;
        }

        DiVec3                   GetDirection() const {return mDirection;}

        virtual const DiAABB&    GetBoundingBox(void) const;

        virtual DiString&        GetType();

    protected:

        DiVec3                   mDirection;
        
    };

}

#endif
