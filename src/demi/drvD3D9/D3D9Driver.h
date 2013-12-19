
/********************************************************************
    File:       D3D9Driver.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

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

        void                    SetShaderConsts(DiShaderType type, int regID, int val);

        void                    SetShaderConsts(DiShaderType type, int regID, const DiColor& col);

        void                    SetViewport(int x, int y, int w, int h, float minz = 0.0f, float maxz = 1.0f);

        void                    SetFillMode(DiFillMode mode);

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

        void                    CreateWindowTarget(DiRenderTarget*& outRT, DiWndHandle wnd);

        void                    BindMaterialStates(const DiMaterial* mat);

        void                    GetWindowDimension(DiWndHandle wnd, uint32& w, uint32& h);

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

        bool                    GetPresentParameter(D3DPRESENT_PARAMETERS &parameters,
                                    D3DFORMAT &adapterFormat, const D3DDISPLAYMODE &adapterMode);

        bool                    CreateDirect3D();

    private:

        D3DFORMAT               mAdapterFormat;

        DiString                mWndClass;

        IDirect3DSurface9*      mBackBuffer;

        IDirect3DSurface9*      mDepthBuffer;

        uint32                  mAdapter;

        D3DPRESENT_PARAMETERS   mMainParameters;

    public:

        static DiD3D9StateCache* StateCache;

        static IDirect3DDevice9* Device;
    };
}