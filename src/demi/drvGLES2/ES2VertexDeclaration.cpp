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

#include "DrvGLES2Pch.h"
#include "ES2VertexDeclaration.h"

// Convenience macro from ARB_vertex_buffer_object spec
#define VBO_BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace Demi
{
    DiGLES2VertexDeclaration::DiGLES2VertexDeclaration()
        : mVAO(0)
        , mIsInitialised(false)
    {

    }

    DiGLES2VertexDeclaration::~DiGLES2VertexDeclaration()
    {
        Release();
        mVertexElements.ClearElements();
    }

    void DiGLES2VertexDeclaration::Bind()
    {
#   if defined(GL_OES_vertex_array_object)
        CHECK_GL_ERROR(glBindVertexArrayOES(mVAO));
#   endif
    }

    void DiGLES2VertexDeclaration::Release()
    {
#   if defined(GL_OES_vertex_array_object)
        CHECK_GL_ERROR(glDeleteVertexArraysOES(1, &mVAO));
#   endif
    }

    void DiGLES2VertexDeclaration::Create()
    {
#   if defined(GL_OES_vertex_array_object)
        CHECK_GL_ERROR(glGenVertexArraysOES(1, &mVAO));
        if (!mVAO)
        {
            DI_WARNING("Cannot create GL ES Vertex Array Object");
        }
        CHECK_GL_ERROR(glBindVertexArrayOES(mVAO));
#   endif
    }
}