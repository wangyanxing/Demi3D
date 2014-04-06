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

#ifndef DiES2VertexDeclaration_h__
#define DiES2VertexDeclaration_h__

#include "VertexDeclaration.h"

namespace Demi
{
    class DI_GLES2_API DiGLES2VertexDeclaration : public DiVertexDeclaration
    {
    public:

        DiGLES2VertexDeclaration(void);

        ~DiGLES2VertexDeclaration(void);

    public:

        void    Bind();

        void    Release();

        void    Create();

        bool    IsInitialised(void) { return mIsInitialised; }

        void    SetInitialised(bool flag) { mIsInitialised = flag; }

    protected:

        GLuint  mVAO;

        bool    mIsInitialised;
    };
}

#endif
