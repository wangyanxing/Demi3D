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

#ifndef DiD3D9RenderTarget_h__
#define DiD3D9RenderTarget_h__


#include "RenderTarget.h"

namespace Demi
{
    class DI_D3D9DRV_API DiD3D9RenderTarget : public DiRenderTarget
    {
    public:

        DiD3D9RenderTarget();

        virtual                 ~DiD3D9RenderTarget();

    public:

        void                    AttachSurface();

        void                    DetachSurface();

        bool                    BindRenderTarget(uint8 mrtid);

        bool                    BindDepthStencil();

        bool                    IsCompatibleWith(DiDepthBuffer* db);

        IDirect3DSurface9*      GetSurface() const { return mSurface; }

        DiDepthBuffer*          CreateDepthBuffer();

        void                    PreBind();

    protected:

        IDirect3DSurface9*      mSurface;
    };
}

#endif
