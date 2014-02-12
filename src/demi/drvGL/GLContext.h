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

#ifndef DiGLContext_h__
#define DiGLContext_h__


namespace Demi
{
    class DI_GLDRV_API DiGLContext
    {
    public:

        DiGLContext() {}

        virtual                 ~DiGLContext(){}

    public:

        virtual void            BeginContext() = 0;

        virtual void            EndContext() = 0;

        virtual void            Release() = 0;

        virtual DiGLContext*    Clone() const = 0;
    };
}

#endif
