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

#ifndef DiD3D9VertexDeclaration_h__
#define DiD3D9VertexDeclaration_h__


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

#endif
