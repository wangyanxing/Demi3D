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

#ifndef DiD3D9WindowTarget_h__
#define DiD3D9WindowTarget_h__


#include "D3D9RenderTarget.h"

namespace Demi
{
    // used for render window
    class DiD3D9WindowTarget : public DiD3D9RenderTarget
    {
    public:
        DiD3D9WindowTarget(void);

        virtual                 ~DiD3D9WindowTarget(void);
                                
    public:                     
                                
        virtual void            Create(HWND hwnd);
                                
        HWND                    GetWindowHandle() const { return mWndHandle; }
                                
        void                    OnDeviceLost();
                                
        void                    OnDeviceReset();
                                
        uint32                  GetWidth()const;
                                
        uint32                  GetHeight()const;
                                
        bool                    IsSwapChainWindow();
                                
        void                    ReleaseResources();
                                
        bool                    CheckSizeChanged();
                                
        bool                    SwapBuffer();
                                
    protected:                  
                                
        HRESULT                 Present();

    protected:

        HWND                    mWndHandle;

        IDirect3DSwapChain9*    mSwapChain;

        D3DPRESENT_PARAMETERS   mParameters;

        IDirect3DSurface9*      mD3D9DepthBuffer;
    };
}

#endif
