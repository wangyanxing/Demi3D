
/********************************************************************
    File:       GLVertexDeclaration.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "VertexDeclaration.h"

namespace Demi
{
    class DI_GLDRV_API DiGLVertexDeclaration : public DiVertexDeclaration
    {
    public:

        DiGLVertexDeclaration(void);

        ~DiGLVertexDeclaration(void);

    public:

        void                Bind(){}

        void                Release();

        void                Create();
    };
}