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

#ifndef DiK2GameCamera_h__
#define DiK2GameCamera_h__

#include "K2Prerequisites.h"

namespace Demi
{
    class DEMI_K2_API DiK2GameCamera : public DiBase
    {
    public:

        DiK2GameCamera(DiCamera* camera);

        ~DiK2GameCamera();

    public:

        void        SetTarget(DiK2Hero* hero) { mTarget = hero; }

        void        Update();

    private:

        DiCamera*   mCamera;

        DiK2Hero*   mTarget;

        float       mHeight;
    };
}

#endif
