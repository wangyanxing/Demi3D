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

#ifndef DiD3D9Driver_h__
#define DiD3D9Driver_h__


#include "DrvD3D9Defines.h"
#include "D3D9Prerequisites.h"
#include "GfxDriver.h"

namespace Demi
{
    class DI_D3D9DRV_API DiD3D9Driver : public DiGfxDriver
    {
    public:

        DiD3D9Driver();

        virtual                 ~DiD3D9Driver();

    public:

        DiGfxDriverType         GetDriverType() const { return DRV_DIRECT3D9; }

        bool                    InitGfx(DiWndHandle wnd);

        bool                    InitGfx(uint16 width, uint16 height, bool fullscreen);

        void                    PostInit();

        void                    ReleaseGfx();

        bool                    IsDeviceLost();

        void                    BeginFrame();

        void                    EndFrame();

        void                    ConvertProjectionMatrix(const DiMat4& matrix, DiMat4& dest);

        void                    SetShaderConsts(DiShaderType type, int regID, const DiMat4& mat);

        void                    SetShaderConsts(DiShaderType type, int regID, const DiVec2& vec);

        void                    SetShaderConsts(DiShaderType type, int regID, const DiVec3& vec);

        void                    SetShaderConsts(DiShaderType type, int regID, const DiVec4& vec);

        void                    SetShaderConsts(DiShaderType type, int regID, float val);

        void                    SetShaderConsts(DiShaderType type, int regID, bool val);

        void                    SetShaderConsts(DiShaderType type, int regID, int val);

        void                    SetShaderConsts(DiShaderType type, int regID, const DiColor& col);

        void                    BindShaders(DiShaderProgram* vs, DiShaderProgram* ps);

        void                    SetViewport(int x, int y, int w, int h, float minz = 0.0f, float maxz = 1.0f);

        DiWindow*               CreateWnd();

        void                    SetFillMode(DiFillMode mode);

        void                    SetDepthBias(float constantBias, float slopeScaledBias);

        bool                    IsDeviceLost() const;

        bool                    ResetDevice(uint16 w, uint16 h);

        bool                    RenderGeometry(DiRenderUnit* unit);

        DiDepthBuffer*          AddManualDepthBuffer(IDirect3DSurface9* surf);

        D3DFORMAT               GetDepthStencilFormatFor(D3DFORMAT backBufFmt, D3DFORMAT fmt);

        D3DFORMAT               GetDepthStencilFormatFor(D3DFORMAT fmt);

        const DiVec2&           GetTexelOffset() const;

        DiTextureDrv*           CreateTextureDriver(DiTexture* texture);

        DiIndexBuffer*          CreateIndexBuffer();

        DiVertexBuffer*         CreateVertexBuffer();

        DiVertexDeclaration*    CreateVertexDeclaration();

        DiShaderInstance*       CreateVSInstance(DiShaderProgram* prog);

        DiShaderInstance*       CreatePSInstance(DiShaderProgram* prog);

        DiRenderTarget*         CreateRenderTarget();

        DiShaderParameter*      CreateShaderParams(DiMaterial& mat);

        const DiString&         GetShaderFileExtension() const;

        void                    CreateWindowTarget(DiRenderTarget*& outRT, DiWndHandle wnd);

        void                    BindMaterialStates(const DiMaterial* mat);

        void                    Clear(uint32 flag, const DiColor& col, float depth, unsigned short stencil = 0);

        IDirect3DSwapChain9*    CreateSwapChain(DiWndHandle hwnd);

        IDirect3DSwapChain9*    GetSwapChain(uint32 iSwapChain);

        DWORD                   GetAdapter()
        {
            UINT adapter = (mAdapter == 0xFFFFFFFF) ? D3DADAPTER_DEFAULT : (UINT)mAdapter;
            return adapter;
        }

        D3DDEVTYPE              GetDeviceType() { return D3DDEVTYPE_HAL; }

        D3DFORMAT               GetAdapterFormat() { return mAdapterFormat; }

        void                    SetColorBufferWriteEnabled(bool r, bool g, bool b, bool a);

        bool                    CanAutoGenMipmaps(DWORD srcUsage, D3DRESOURCETYPE srcType, D3DFORMAT srcFormat);

        /** The texture flipping should be always false in dx renderer
         */
        void                    SetTextureFlipping(bool val) { mNeedTextureFlipping = false; }

        DiPair<float, float>    GetDepthInputRange() const;

        DiGfxCaps*              InitGfxCaps(){ return nullptr; }

        D3D9DriverList*         GetD3D9Devices();

    public:

        static IDirect3DVertexShader9*         CreateVertexShader(const DWORD* pFunction);

        static IDirect3DPixelShader9*          CreatePixelShader(const DWORD* pFunction);

        static IDirect3DTexture9*              CreateTexture(uint32 Width, uint32 Height, uint32 Levels, DWORD Usage,
                                                    D3DFORMAT Format, D3DPOOL Pool);

        static IDirect3DCubeTexture9*          CreateCubeTexture(uint32 Length, uint32 Levels, DWORD Usage,
                                                    D3DFORMAT Format, D3DPOOL Pool);

        static IDirect3DVertexDeclaration9*    CreateVertexDeclaration(D3DVERTEXELEMENT9* elements);

        static IDirect3DSurface9*              CreateDepthStencil(uint32 Width, uint32 Height, D3DFORMAT format);

        static IDirect3DVertexBuffer9*         CreateVertexBuffer(uint32 Length, DWORD Usage, D3DPOOL Pool);
        
        static IDirect3DIndexBuffer9*          CreateIndexBuffer(uint32 Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool);
        
    public:

        IDirect3DDevice9*       mDevice;

        LPDIRECT3D9             mD3D;

    private:

        DiGfxCaps*              InitGfxCapsImpl();

        bool                    GetPresentParameter(const DiIntVec2& buffersize, D3DPRESENT_PARAMETERS &parameters,
                                    D3DFORMAT &adapterFormat, const D3DDISPLAYMODE &adapterMode);

        bool                    CreateDirect3D();

        void                    ConvertVertexShaderCaps(DiGfxCaps* rsc) const;

        void                    ConvertPixelShaderCaps(DiGfxCaps* rsc) const;

        bool                    CheckVertexTextureFormats(IDirect3DSurface9* surface) const;

    private:

        D3DFORMAT               mAdapterFormat;

        DiString                mWndClass;

        IDirect3DSurface9*      mBackBuffer;

        IDirect3DSurface9*      mDepthBuffer;

        uint32                  mAdapter;

        D3DPRESENT_PARAMETERS   mMainParameters;

        D3DCAPS9                mDeviceCaps;

        D3D9DriverList*         mDeviceList;

        D3D9Driver*             mActiveD3DDevice;

    public:

        static DiD3D9StateCache* StateCache;

        static IDirect3DDevice9* Device;

        static IDirect3D9*       D3D9;
    };
}

#endif
