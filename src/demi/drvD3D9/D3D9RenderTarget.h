


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
