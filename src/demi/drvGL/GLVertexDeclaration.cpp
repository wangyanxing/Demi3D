
#include "GLVertexDeclaration.h"
#include "GLDriver.h"

namespace Demi
{
    DiGLVertexDeclaration::DiGLVertexDeclaration()
    {

    }

    DiGLVertexDeclaration::~DiGLVertexDeclaration()
    {
        Release();
    }

    void DiGLVertexDeclaration::Release()
    {
        mVertexElements.ClearElements();
    }

    void DiGLVertexDeclaration::Create()
    {

    }

}