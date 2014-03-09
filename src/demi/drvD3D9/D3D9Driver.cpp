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
#include "DrvD3D9Pch.h"
#include "D3D9Driver.h"
#include "D3D9StateCache.h"
#include "RenderTarget.h"
#include "RenderUnit.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexDeclaration.h"
#include "GfxDriver.h"
#include "RenderWindow.h"
#include "ShaderProgram.h"
#include "Win32/Win32Window.h"

#include "D3D9TypeMappings.h"
#include "D3D9VertexBuffer.h"
#include "D3D9IndexBuffer.h"
#include "D3D9VertexDeclaration.h"
#include "D3D9RenderTarget.h"
#include "D3D9WindowTarget.h"
#include "D3D9Texture.h"
#include "D3D9Shader.h"
#include "D3D9DepthBuffer.h"
#include "D3D9ShaderParam.h"

namespace Demi
{
    DiD3D9StateCache* DiD3D9Driver::StateCache = nullptr;
    IDirect3DDevice9* DiD3D9Driver::Device = nullptr;

    DiD3D9Driver::DiD3D9Driver()
        : mDevice(nullptr),
        mD3D(nullptr),
        mBackBuffer(nullptr),
        mDepthBuffer(nullptr),
        mAdapter(0xFFFFFFFF),
        mAdapterFormat(D3DFMT_UNKNOWN)
    {
        memset(&mDeviceCaps, 0, sizeof(mDeviceCaps));
    }
    
    DiD3D9Driver::~DiD3D9Driver()
    {
    }

    static bool CheckDepthFormat(LPDIRECT3D9 d3d, UINT adapter, D3DDEVTYPE rasterizer, 
        D3DFORMAT DepthFormat,
        D3DFORMAT AdapterFormat,
        D3DFORMAT BackBufferFormat)
    {
        // Verify that the depth format exists
        HRESULT hr = d3d->CheckDeviceFormat(adapter,
            rasterizer,
            AdapterFormat,
            D3DUSAGE_DEPTHSTENCIL,
            D3DRTYPE_SURFACE,
            DepthFormat);

        if (FAILED(hr)) return FALSE;

        // Verify that the depth format is compatible
        hr = d3d->CheckDepthStencilMatch(adapter,
            (D3DDEVTYPE)rasterizer,
            AdapterFormat,
            BackBufferFormat,
            DepthFormat);

        return SUCCEEDED(hr);
    }

    bool DiD3D9Driver::InitGfx(DiWndHandle wnd)
    {
        DI_INFO("Direct3D9 driver is intializing.");

        DI_ASSERT(wnd);

        if (!CreateDirect3D())
        {
            DI_ERROR("Cannot create the D3D9 object!");
            return false;
        }
        
        DI_INFO("Direct3D9 object successfully initialized.");
        DI_ASSERT(!mDevice);

        UINT adapter = (mAdapter == 0xffffffff) ? D3DADAPTER_DEFAULT : (UINT)mAdapter;

        D3DDISPLAYMODE adapterMode;
        if (mD3D->GetAdapterDisplayMode(adapter, &adapterMode) != D3D_OK)
        {
            DI_WARNING("GetAdapterDisplayMode failed");
            ReleaseGfx();
            return false;
        }

        D3DFORMAT adapterFormat;
        if (!GetPresentParameter(mMainParameters, adapterFormat, adapterMode))
        {
            DI_WARNING("Fill present parameter failed");
            ReleaseGfx();
            return false;
        }
        mAdapterFormat = adapterFormat;

        D3DDEVTYPE rasterizer = D3DDEVTYPE_HAL;
        HRESULT result = mD3D->CreateDevice(adapter, rasterizer, (HWND)wnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE, &mMainParameters, &mDevice);
        if (result != D3D_OK)
        {
            DI_WARNING("Cannot create device hr:0x%x adap:0x%x rast:0x%x, try another params.", result, adapter, rasterizer);

            // Create the D3D device without puredevice
            HRESULT result = mD3D->CreateDevice(adapter, rasterizer, (HWND)wnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &mMainParameters, &mDevice);
            if (result != D3D_OK)
            {
                DI_WARNING("Cannot create device without puredevice hr:0x%x adap:0x%x rast:0x%x, try another params.", result, adapter, rasterizer);

                // Create the D3D device without puredevice and hardware
                HRESULT result = mD3D->CreateDevice(adapter, rasterizer, (HWND)wnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &mMainParameters, &mDevice);
                if (result != D3D_OK)
                {
                    DI_WARNING("Cannot create device without puredevice and hardware hr:0x%x adap:0x%x rast:0x%x", result, adapter, rasterizer);
                    ReleaseGfx();
                    return false;
                }
            }
        }

        HRESULT hr = mDevice->GetDeviceCaps(&mDeviceCaps);
        if (FAILED(hr))
        {
            DI_WARNING("Cannot create d3d9 device caps");
        }

        DI_ASSERT(!StateCache);
        StateCache = DI_NEW DiD3D9StateCache(mDevice);

        DI_ASSERT(!Device);
        Device = mDevice;
        return true;
    }

    bool DiD3D9Driver::GetPresentParameter(D3DPRESENT_PARAMETERS &parameters, 
        D3DFORMAT &adapterFormat, const D3DDISPLAYMODE &adapterMode)
    {
        UINT adapter = (mAdapter == 0xFFFFFFFF) ? D3DADAPTER_DEFAULT : (UINT)mAdapter;

        memset(&parameters, 0, sizeof(D3DPRESENT_PARAMETERS));
        parameters.BackBufferWidth          = mWidth;
        parameters.BackBufferHeight         = mHeight;
        parameters.BackBufferCount          = 1;
        parameters.MultiSampleType          = D3DMULTISAMPLE_NONE;
        parameters.MultiSampleQuality       = 0;
        parameters.SwapEffect               = D3DSWAPEFFECT_DISCARD;
        parameters.hDeviceWindow            = NULL;
        parameters.Windowed                 = !mFullScreen;
        parameters.EnableAutoDepthStencil   = TRUE;
        parameters.FullScreen_RefreshRateInHz = 0;
        parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

        const int bitsPerPixel = 32;
        const uint32 depthIndex = (bitsPerPixel == 16) ? 0 : (bitsPerPixel == 24) ? 1 : 2;

        const uint32 numFormat = 3;
        D3DFORMAT backBufferFormats[numFormat] =
        { D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8 };
        
        bool found = false;

        D3DDEVTYPE rasterizer = D3DDEVTYPE_HAL;

        if (!mFullScreen)
        {
            uint32 backBuffer;
            for (backBuffer = 0; backBuffer<numFormat; backBuffer++)
            {
                if (mD3D->CheckDeviceType(adapter, rasterizer, adapterMode.Format, backBufferFormats[backBuffer], TRUE) == D3D_OK)
                {
                    parameters.BackBufferFormat = backBufferFormats[backBuffer];
                    adapterFormat = adapterMode.Format;
                    found = true;
                    break;
                }
            }
        }
        else
        {
            uint32 backBuffer;
            for (backBuffer = 0; backBuffer<numFormat; backBuffer++)
            {
                uint32 display;
                for (display = 0; display<numFormat; display++)
                {
                    if (mD3D->CheckDeviceType(adapter, rasterizer, backBufferFormats[display], backBufferFormats[display], FALSE) == D3D_OK)
                    {
                        parameters.BackBufferFormat = backBufferFormats[backBuffer];
                        adapterFormat = backBufferFormats[display];
                        found = true;
                        break;
                    }
                }
                if (found)
                    break;
            }
        }

        if (!found)
        {
            DI_WARNING("Cannot create backbuffer, unsupported format.");
            return false;
        }

        DI_INFO("Backbuffer format : %d", parameters.BackBufferFormat);

       /* D3DFORMAT zbufferFormats[] =
        {
            D3DFMT_D24S8,
            D3DFMT_D24X4S4,
            D3DFMT_D24FS8,
        };

        const uint32 zbufferFormatCount = sizeof(zbufferFormats) / sizeof(D3DFORMAT);
        uint32 zbID;
        for (zbID = 0; zbID < zbufferFormatCount; zbID++)
        {
            if (CheckDepthFormat(mD3D, adapter, rasterizer, zbufferFormats[zbID], adapterFormat, parameters.BackBufferFormat))
                break;
        }

        if (zbID >= zbufferFormatCount)
        {
            DI_WARNING("Cannot create depth buffer, unsupported format.");
            return false;
        }*/
        parameters.AutoDepthStencilFormat = GetDepthStencilFormatFor(adapterFormat, parameters.BackBufferFormat);// zbufferFormats[zbID];

        DI_INFO("Depth stencil format : %d", parameters.AutoDepthStencilFormat);

        return true;
    }

    bool DiD3D9Driver::InitGfx(uint16 width, uint16 height, bool fullscreen)
    {
        DI_INFO("Creating window: %d x %d, %s mode.", width, height, fullscreen?"full screen":"window");
        mWidth = width;
        mHeight = height;

        return true;
    }

    bool DiD3D9Driver::IsDeviceLost()
    {
        HRESULT hr = mDevice->TestCooperativeLevel();

        if (hr == D3DERR_DEVICELOST ||
            hr == D3DERR_DEVICENOTRESET)
        {
            return true;
        }
        return false;
    }

    void DiD3D9Driver::BeginFrame()
    {
        HRESULT hRlt = mDevice->BeginScene();
        DX9_CHKERR(hRlt);
    }
    
    void DiD3D9Driver::EndFrame()
    {
        HRESULT hRlt = mDevice->EndScene();
        DX9_CHKERR(hRlt);
    }

    void DiD3D9Driver::ReleaseGfx()
    {
        if (StateCache)
        {
            DI_DELETE StateCache;
            StateCache = nullptr;
        }

        if (mBackBuffer)
        {
            mBackBuffer->Release();
            mBackBuffer = nullptr;
        }

        if (mDepthBuffer)
        {
            mDepthBuffer->Release();
            mDepthBuffer = nullptr;
        }
        
        if (mDevice)
        {
            mDevice->Release();
            mDevice = nullptr;
            Device = nullptr;
        }

        DI_INFO("Direct3D9 stuff successfully released.");
    }

    bool DiD3D9Driver::CreateDirect3D()
    {
        DI_ASSERT(!mD3D);
        mD3D = Direct3DCreate9(D3D_SDK_VERSION);
        return true;
    }

    void DiD3D9Driver::SetShaderConsts(DiShaderType shaderType, int regID, const DiMat4& mat)
    {
        if (regID < 0)
            return;

        static DiMat4 m;
        mat.transpose(m);

        if (shaderType == SHADER_VERTEX)
            mDevice->SetVertexShaderConstantF(regID, m[0], 4);
        else
            mDevice->SetPixelShaderConstantF(regID, m[0], 4);
    }

    void DiD3D9Driver::SetShaderConsts(DiShaderType shaderType, int regID, const DiVec2& vec)
    {
        static DiVec4 t;

        if (regID < 0)
            return;

        t.x = vec.x;
        t.y = vec.y;
        t.z = 0;
        t.w = 0;

        SetShaderConsts(shaderType, regID, t);
    }

    void DiD3D9Driver::SetShaderConsts(DiShaderType shaderType, int regID, const DiVec3& vec)
    {
        static DiVec4 t;

        if (regID < 0)
            return;

        t.x = vec.x;
        t.y = vec.y;
        t.z = vec.z;
        t.w = 0;

        SetShaderConsts(shaderType, regID, t);
    }

    void DiD3D9Driver::SetShaderConsts(DiShaderType shaderType, int regID, const DiVec4& vec)
    {
        if (regID < 0)
            return;

        if (shaderType == SHADER_VERTEX)
            mDevice->SetVertexShaderConstantF(regID, &vec.x, 1);
        else
            mDevice->SetPixelShaderConstantF(regID, &vec.x, 1);
    }

    void DiD3D9Driver::SetShaderConsts(DiShaderType shaderType, int regID, float val)
    {
        if (regID < 0)
            return;

        static DiVec4 t;
        t.x = val;
        t.y = 0;
        t.z = 0;
        t.w = 0;

        SetShaderConsts(shaderType, regID, t);
    }

    void DiD3D9Driver::SetShaderConsts(DiShaderType shaderType, int regID, int val)
    {
        if (regID < 0)
            return;

        static int t;
        t = val;

        if (shaderType == SHADER_VERTEX)
            mDevice->SetVertexShaderConstantI(regID, &t, 1);
        else
            mDevice->SetPixelShaderConstantI(regID, &t, 1);
    }

    void DiD3D9Driver::SetShaderConsts(DiShaderType shaderType, int regID, const DiColor& col)
    {
        if (regID < 0)
            return;

        static DiVec4 t;
        t.x = col.r;
        t.y = col.g;
        t.z = col.b;
        t.w = col.a;

        SetShaderConsts(shaderType, regID, t);
    }

    void DiD3D9Driver::SetShaderConsts(DiShaderType type, int regID, bool val)
    {
        if (regID < 0)
            return;

        static BOOL t;
        t = val ? TRUE : FALSE;

        if (type == SHADER_VERTEX)
            mDevice->SetVertexShaderConstantB(regID, &t, 1);
        else
            mDevice->SetPixelShaderConstantB(regID, &t, 1);
    }

    void DiD3D9Driver::ConvertProjectionMatrix(const DiMat4& matrix, DiMat4& dest)
    {
        dest = matrix;
        dest[2][0] = (dest[2][0] + dest[3][0]) / 2;
        dest[2][1] = (dest[2][1] + dest[3][1]) / 2;
        dest[2][2] = (dest[2][2] + dest[3][2]) / 2;
        dest[2][3] = (dest[2][3] + dest[3][3]) / 2;
    }

    bool DiD3D9Driver::IsDeviceLost() const
    {
        HRESULT hr;
        hr = mDevice->TestCooperativeLevel();
        return (hr == D3DERR_DEVICELOST || hr == D3DERR_DEVICENOTRESET);
    }

    bool DiD3D9Driver::RenderGeometry(DiRenderUnit* unit)
    {
        if (unit->mSourceData.empty())
            return false;

        if (!unit->mPrimitiveCount)
            return false;

        for (auto it = unit->mSourceData.begin(); it != unit->mSourceData.end(); ++it)
            (*it)->Bind();

        if (unit->mVertexDecl)
            unit->mVertexDecl->Bind();

        D3DPRIMITIVETYPE primType = DiD3D9Mappings::D3D9PrimTypeMapping[unit->mPrimitiveType];

        if (!unit->mIndexBuffer)
        {
            HRESULT hRlt = mDevice->DrawPrimitive(
                primType, unit->mVertexOffset, unit->mPrimitiveCount);  
            DX9_CHKERR(hRlt);
        }
        else
        {
            unit->mIndexBuffer->Bind();

            HRESULT hRlt = mDevice->DrawIndexedPrimitive(primType,
                unit->mVertexOffset, 0,
                unit->mVerticesNum, unit->mIndexOffset, unit->mPrimitiveCount);
            DX9_CHKERR(hRlt);
        }

        return true;
    }

    const DiVec2& DiD3D9Driver::GetTexelOffset() const
    {
        static DiVec2 texelOffset(-0.5f, -0.5f);
        return texelOffset;
    }

    bool DiD3D9Driver::ResetDevice(uint16 w, uint16 h)
    {
        CleanUpDepthBuffers();

        mMainParameters.BackBufferWidth = (uint32)w;
        mMainParameters.BackBufferHeight = (uint32)h;

        DI_INFO("Reset D3D9 device.");
        HRESULT res = mDevice->Reset(&mMainParameters);
        DiD3D9Driver::StateCache->DirtyAll();

        return res == S_OK;
    }

    D3DFORMAT ddDepthStencilFormats[] =
    {
        D3DFMT_D24FS8,
        D3DFMT_D24S8,
        D3DFMT_D24X4S4,
        D3DFMT_D24X8,
        D3DFMT_D15S1,
        D3DFMT_D16,
        D3DFMT_D32
    };

    D3DFORMAT DiD3D9Driver::GetDepthStencilFormatFor(D3DFORMAT backBufFmt, D3DFORMAT fmt)
    {
        typedef DiHashMap<unsigned int, D3DFORMAT> DepthStencilHash;
        static DepthStencilHash mDepthStencilHash;

        DepthStencilHash::iterator i = mDepthStencilHash.find((unsigned int)fmt);
        if (i != mDepthStencilHash.end())
            return i->second;

        D3DFORMAT dsfmt = D3DFMT_UNKNOWN;

        DWORD adapter = GetAdapter();

        int numFmts = sizeof(ddDepthStencilFormats) / sizeof(D3DFORMAT);
        for (int x = 0; x < numFmts; ++x)
        {
            if (mD3D->CheckDeviceFormat(
                adapter,
                D3DDEVTYPE_HAL,
                backBufFmt,
                D3DUSAGE_DEPTHSTENCIL,
                D3DRTYPE_SURFACE,
                ddDepthStencilFormats[x]) != D3D_OK)
            {
                continue;
            }

            if (mD3D->CheckDepthStencilMatch(
                adapter,
                D3DDEVTYPE_HAL,
                backBufFmt,
                fmt, ddDepthStencilFormats[x]) == D3D_OK)
            {
                dsfmt = ddDepthStencilFormats[x];
                break;
            }
        }

        mDepthStencilHash[(unsigned int)fmt] = dsfmt;
        return dsfmt;
    }

    D3DFORMAT DiD3D9Driver::GetDepthStencilFormatFor(D3DFORMAT fmt)
    {
        DiD3D9RenderTarget* d3d9rt = static_cast<DiD3D9RenderTarget*>(Driver->GetMainRenderWindow()->GetRenderBuffer());
        D3DSURFACE_DESC srfDesc;
        if (!FAILED(d3d9rt->GetSurface()->GetDesc(&srfDesc)))
        {
            return GetDepthStencilFormatFor(srfDesc.Format, fmt);
        }
        return D3DFMT_UNKNOWN;
    }

    DiDepthBuffer* DiD3D9Driver::AddManualDepthBuffer(IDirect3DSurface9* depthSurface)
    {
        auto itor = mDepthBufferPool[DiDepthBuffer::POOL_DEFAULT].begin();
        auto end = mDepthBufferPool[DiDepthBuffer::POOL_DEFAULT].end();

        while (itor != end)
        {
            DiD3D9DepthBuffer* db = static_cast<DiD3D9DepthBuffer*>((*itor));
            if (db->GetDepthBufferSurface() == depthSurface)
                return *itor;

            ++itor;
        }

        D3DSURFACE_DESC dsDesc;
        if (FAILED(depthSurface->GetDesc(&dsDesc)))
            return NULL;

        DiDepthBuffer *newDepthBuffer = DI_NEW DiD3D9DepthBuffer(
            DiDepthBuffer::POOL_DEFAULT, dsDesc.Width, dsDesc.Height,
            depthSurface, dsDesc.Format, 
            dsDesc.MultiSampleType, dsDesc.MultiSampleQuality, false);

        mDepthBufferPool[newDepthBuffer->GetPoolId()].push_back(newDepthBuffer);

        return newDepthBuffer;
    }

    void DiD3D9Driver::SetViewport(int x, int y, int w, int h, float minz, float maxz)
    {
        static D3DVIEWPORT9 d3dvp;
        d3dvp.X = x;
        d3dvp.Y = y;
        d3dvp.Width = w;
        d3dvp.Height = h;
        d3dvp.MinZ = minz;
        d3dvp.MaxZ = maxz;
        mDevice->SetViewport(&d3dvp);
    }

    void DiD3D9Driver::SetFillMode(DiFillMode mode)
    {
        D3DFILLMODE fm = D3DFILL_SOLID;
        if (mode == FM_WIREFRAME)
            fm = D3DFILL_WIREFRAME;
        else if (mode == FM_POINT)
            fm = D3DFILL_POINT;
        StateCache->SetRenderState(D3DRS_FILLMODE, fm);
    }

    DiTextureDrv* DiD3D9Driver::CreateTextureDriver(DiTexture* texture)
    {
        return DI_NEW DiD3D9TextureDrv(texture);
    }

    DiIndexBuffer* DiD3D9Driver::CreateIndexBuffer()
    {
        return DI_NEW DiD3D9IndexBuffer();
    }

    DiVertexBuffer* DiD3D9Driver::CreateVertexBuffer()
    {
        return DI_NEW DiD3D9VertexBuffer();
    }

    DiVertexDeclaration* DiD3D9Driver::CreateVertexDeclaration()
    {
        return DI_NEW DiD3D9VertexDeclaration();
    }

    DiShaderInstance* DiD3D9Driver::CreateVSInstance(DiShaderProgram* prog)
    {
        return DI_NEW DiD3D9VSInstance(prog);
    }

    DiShaderInstance* DiD3D9Driver::CreatePSInstance(DiShaderProgram* prog)
    {
        return DI_NEW DiD3D9PSInstance(prog);
    }

    DiRenderTarget* DiD3D9Driver::CreateRenderTarget()
    {
        return DI_NEW DiD3D9RenderTarget();
    }

    void DiD3D9Driver::CreateWindowTarget(DiRenderTarget*& outRT, DiWndHandle wnd)
    {
        DiD3D9WindowTarget* rb = DI_NEW DiD3D9WindowTarget();
        outRT = rb;
        rb->Create((HWND)wnd);
    }

    IDirect3DSwapChain9* DiD3D9Driver::GetSwapChain(uint32 iSwapChain)
    {
        IDirect3DSwapChain9* chain = NULL;
        HRESULT hRlt;
        hRlt = mDevice->GetSwapChain(iSwapChain, &chain);
        DX9_CHKERR(hRlt);
        return chain;
    }

    IDirect3DSwapChain9* DiD3D9Driver::CreateSwapChain(DiWndHandle hwnd)
    {
        HRESULT hRlt;
        D3DPRESENT_PARAMETERS param = mMainParameters;
        GetWindowSize(hwnd, param.BackBufferWidth, param.BackBufferHeight);
        IDirect3DSwapChain9* chain = 0;
        hRlt = mDevice->CreateAdditionalSwapChain(&param, &chain);
        DX9_CHKERR(hRlt);
        return chain;
    }

    void DiD3D9Driver::Clear(uint32 flag, const DiColor& col, float depth, unsigned short stencil)
    {
        DWORD clearBuf = 0;
        if (flag & CLEAR_COLOR)
            clearBuf |= D3DCLEAR_TARGET;
        if (flag & CLEAR_DEPTH)
            clearBuf |= D3DCLEAR_ZBUFFER;
        if (flag & CLEAR_STENCIL)
            clearBuf |= D3DCLEAR_STENCIL;

        mDevice->Clear(0, nullptr, flag, col.GetAsARGB(), depth, stencil);
    }

    IDirect3DVertexShader9* DiD3D9Driver::CreateVertexShader(const DWORD* pFunction)
    {
        IDirect3DVertexShader9* vs = nullptr;
        HRESULT result = Device->CreateVertexShader(pFunction, &vs);
        DX9_CHKERR(result);
        return vs;
    }

    IDirect3DPixelShader9* DiD3D9Driver::CreatePixelShader(const DWORD* pFunction)
    {
        IDirect3DPixelShader9* vs = nullptr;
        HRESULT result = Device->CreatePixelShader(pFunction, &vs);
        DX9_CHKERR(result);
        return vs;
    }

    IDirect3DTexture9* DiD3D9Driver::CreateTexture(uint32 Width, uint32 Height, uint32 Levels, DWORD Usage,
        D3DFORMAT Format, D3DPOOL Pool)
    {
        IDirect3DTexture9* tex = nullptr;
        HRESULT result = Device->CreateTexture(Width, Height,
            Levels, Usage, Format, Pool, &tex, 0);
        DX9_CHKERR(result);
        return tex;
    }

    IDirect3DCubeTexture9* DiD3D9Driver::CreateCubeTexture(uint32 Length, uint32 Levels, DWORD Usage,
        D3DFORMAT Format, D3DPOOL Pool)
    {
        IDirect3DCubeTexture9* tex = nullptr;
        HRESULT result = Device->CreateCubeTexture(Length,
            Levels, Usage, Format, Pool, &tex, 0);
        DX9_CHKERR(result);
        return tex;
    }

    IDirect3DVertexDeclaration9* DiD3D9Driver::CreateVertexDeclaration(D3DVERTEXELEMENT9* elements)
    {
        IDirect3DVertexDeclaration9* vl = nullptr;
        HRESULT result = Device->CreateVertexDeclaration(elements, &vl);
        DX9_CHKERR(result);
        return vl;
    }

    IDirect3DSurface9* DiD3D9Driver::CreateDepthStencil(uint32 Width, uint32 Height, D3DFORMAT format)
    {
        HRESULT hRlt;
        IDirect3DSurface9* result = nullptr;
        hRlt = Device->CreateDepthStencilSurface(
            Width, Height, format, D3DMULTISAMPLE_NONE, 0, FALSE, &result, 0);
        DX9_CHKERR(hRlt);
        return result;
    }

    IDirect3DVertexBuffer9* DiD3D9Driver::CreateVertexBuffer(uint32 Length, DWORD Usage, D3DPOOL Pool)
    {
        IDirect3DVertexBuffer9* vb = nullptr;
        HRESULT result = Device->CreateVertexBuffer(Length, Usage, 0, Pool, &vb, 0);
        DX9_CHKERR(result);
        return vb;
    }

    IDirect3DIndexBuffer9* DiD3D9Driver::CreateIndexBuffer(uint32 Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool)
    {
        IDirect3DIndexBuffer9* id = nullptr;
        HRESULT result = Device->CreateIndexBuffer(Length, Usage, Format, Pool, &id, 0);
        DX9_CHKERR(result);
        return id;
    }

    void DiD3D9Driver::BindMaterialStates(const DiMaterial* mat)
    {
        BOOL finalwireframe = FALSE;
        if (mat->GetForceWireframe())
        {
            finalwireframe = mat->IsWireframe();
        }
        else
        {
            if (mat->IsWireframe() /*|| stateCache->IsWireframe()*/)
                finalwireframe = TRUE;
        }

        StateCache->SetRenderState(D3DRS_FILLMODE, finalwireframe ? D3DFILL_WIREFRAME : D3DFILL_SOLID);

        bool flip = mInvertVertexWinding;
        switch (mat->GetCullMode())
        {
        case CULL_NONE:
            StateCache->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
            break;
        case CULL_CW:
            StateCache->SetRenderState(D3DRS_CULLMODE, flip ? D3DCULL_CCW : D3DCULL_CW);
            break;
        case CULL_CCW:
            StateCache->SetRenderState(D3DRS_CULLMODE, flip ? D3DCULL_CW : D3DCULL_CCW);
            break;
        }

        StateCache->SetRenderState(D3DRS_ZWRITEENABLE, mat->GetDepthWrite());
        StateCache->SetRenderState(D3DRS_ZENABLE, mat->GetDepthCheck());

        StateCache->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        switch(mat->GetBlendMode())
        {
        case BLEND_REPLACE:
            {
                StateCache->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
                StateCache->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
                StateCache->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
            }
            break;
        case BLEND_ADD:
            {
                StateCache->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
                StateCache->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE,FALSE);
                StateCache->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
                StateCache->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
            }
            break;
        case BLEND_MULTIPLY:
            {
                StateCache->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
                StateCache->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE,FALSE);
                StateCache->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_DESTCOLOR);
                StateCache->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
            }
            break;
        case BLEND_ALPHA:
            {
                StateCache->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
                StateCache->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE,FALSE);
                StateCache->SetRenderState(D3DRS_BLENDOPALPHA,D3DBLENDOP_ADD);
                StateCache->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
                StateCache->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
            }
            break;
        case BLEND_TRANS_COLOR:
            {
                StateCache->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
                StateCache->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE,FALSE);
                StateCache->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCCOLOR);
                StateCache->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCCOLOR);
            }
            break;
        case  BLEND_ONE_INV_ALPHA:
            {
                StateCache->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
                StateCache->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE,FALSE);
                StateCache->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
                StateCache->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
            }
            break;
        default:
            {
                StateCache->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
                StateCache->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
                StateCache->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
            }
        }
    }

    DiWindow* DiD3D9Driver::CreateWnd()
    {
        return DI_NEW DiWin32Window();
    }

    void DiD3D9Driver::SetColorBufferWriteEnabled(bool r, bool g, bool b, bool a)
    {
        DWORD val = 0;
        if (r)
            val |= D3DCOLORWRITEENABLE_RED;
        if (g)
            val |= D3DCOLORWRITEENABLE_GREEN;
        if (b)
            val |= D3DCOLORWRITEENABLE_BLUE;
        if (a)
            val |= D3DCOLORWRITEENABLE_ALPHA;
        StateCache->SetRenderState(D3DRS_COLORWRITEENABLE, val);
    }

    void DiD3D9Driver::BindShaders(DiShaderProgram* vs, DiShaderProgram* ps)
    {
        vs->Bind(*GetShaderEnvironment());
        ps->Bind(*GetShaderEnvironment());
    }

    DiShaderParameter* DiD3D9Driver::CreateShaderParams(DiMaterial& mat)
    {
        return DI_NEW DiD3D9ShaderParam(mat);
    }

    const DiString& DiD3D9Driver::GetShaderFileExtension() const
    {
        static DiString shaderext = ".hlsl";
        return shaderext;
    }

    bool DiD3D9Driver::CanAutoGenMipmaps(DWORD srcUsage, D3DRESOURCETYPE srcType, D3DFORMAT srcFormat)
    {
        if (mDeviceCaps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP)
        {
            HRESULT hr;
            // check for auto gen. mip maps support
            hr = mD3D->CheckDeviceFormat(
                mDeviceCaps.AdapterOrdinal,
                mDeviceCaps.DeviceType,
                mMainParameters.BackBufferFormat,
                srcUsage | D3DUSAGE_AUTOGENMIPMAP,
                srcType,
                srcFormat);
            // this HR could a SUCCES
            // but mip maps will not be generated
            if (hr == D3D_OK)
                return true;
            else
                return false;
        }
        else
            return false;
    }

    void DiD3D9Driver::SetDepthBias(float constantBias, float slopeScaledBias)
    {
        StateCache->SetRenderState(D3DRS_DEPTHBIAS, constantBias);
        StateCache->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, slopeScaledBias);
    }

    DiPair<float, float> DiD3D9Driver::GetDepthInputRange() const
    {
        return DiPair<float, float>(0.0f, 1.0f);
    }
}