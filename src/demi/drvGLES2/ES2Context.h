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

#ifndef DiGLES2Context_h__
#define DiGLES2Context_h__

#include "ES2Prerequisites.h"

namespace Demi
{
    class DI_GLES2_API DiGLES2Context
    {
    public:

        DiGLES2Context(): mInited(false) {}

        virtual                 ~DiGLES2Context(){}

    public:

        virtual void            BeginContext() = 0;

        virtual void            EndContext() = 0;

        virtual void            Release() = 0;

        virtual DiGLES2Context* Clone() const = 0;

        bool GetInitialized() { return mInited; };

        void SetInitialized() { mInited = true; };

    protected:

        bool mInited;
    };
}

#endif
