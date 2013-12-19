
/********************************************************************
    File:       D3D9VertexDeclaration.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "VertexDeclaration.h"

namespace Demi
{
    class DI_D3D9DRV_API DiD3D9VertexDeclaration : public DiVertexDeclaration
    {
    public:

        DiD3D9VertexDeclaration(void);

        ~DiD3D9VertexDeclaration(void);

    public:

        void                Bind();

        void                Release();

        void                Create();

    private:

        IDirect3DVertexDeclaration9*    mVertexDecl;
    };
}