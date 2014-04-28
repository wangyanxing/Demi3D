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
#include "Capabilities.h"
#include "Image.h"

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
#include "D3D9Device.h"

namespace Demi
{
    DiD3D9StateCache* DiD3D9Driver::StateCache = nullptr;
    IDirect3DDevice9* DiD3D9Driver::Device = nullptr;
    IDirect3D9* DiD3D9Driver::D3D9 = nullptr;

    DiD3D9Driver::DiD3D9Driver()
        : mDevice(nullptr),
        mD3D(nullptr),
        mBackBuffer(nullptr),
        mDepthBuffer(nullptr),
        mAdapter(0xFFFFFFFF),
        mAdapterFormat(D3DFMT_UNKNOWN),
        mDeviceList(nullptr),
        mActiveD3DDevice(nullptr)
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

        if (mAdapter == 0xffffffff)
            mAdapter = D3DADAPTER_DEFAULT;

        D3DDISPLAYMODE adapterMode;
        if (mD3D->GetAdapterDisplayMode(mAdapter, &adapterMode) != D3D_OK)
        {
            DI_WARNING("GetAdapterDisplayMode failed");
            ReleaseGfx();
            return false;
        }

        RECT rect;
        GetClientRect((HWND)wnd, &rect);
        DiIntVec2 buffersize(rect.right - rect.left, rect.bottom - rect.top);

        D3DFORMAT adapterFormat;
        if (!GetPresentParameter(buffersize, mMainParameters, adapterFormat, adapterMode))
        {
            DI_WARNING("Fill present parameter failed");
            ReleaseGfx();
            return false;
        }
        mAdapterFormat = adapterFormat;

        D3DDEVTYPE rasterizer = D3DDEVTYPE_HAL;
        HRESULT result = mD3D->CreateDevice(mAdapter, rasterizer, (HWND)wnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE, &mMainParameters, &mDevice);
        if (result != D3D_OK)
        {
            DI_WARNING("Cannot create device hr:0x%x adap:0x%x rast:0x%x, try another params.", result, mAdapter, rasterizer);

            // Create the D3D device without puredevice
            HRESULT result = mD3D->CreateDevice(mAdapter, rasterizer, (HWND)wnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &mMainParameters, &mDevice);
            if (result != D3D_OK)
            {
                DI_WARNING("Cannot create device without puredevice hr:0x%x adap:0x%x rast:0x%x, try another params.", result, mAdapter, rasterizer);

                // Create the D3D device without puredevice and hardware
                HRESULT result = mD3D->CreateDevice(mAdapter, rasterizer, (HWND)wnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &mMainParameters, &mDevice);
                if (result != D3D_OK)
                {
                    DI_WARNING("Cannot create device without puredevice and hardware hr:0x%x adap:0x%x rast:0x%x", result, mAdapter, rasterizer);
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

        mDeviceList = DI_NEW D3D9DriverList();
        // TODO multi-device
        mActiveD3DDevice = nullptr;
        for (uint32 j = 0; j < mDeviceList->count(); j++)
        {
            if (mDeviceList->item(j)->getAdapterNumber() == mAdapter)
            {
                mActiveD3DDevice = mDeviceList->item(j);
                break;
            }
        }
        if (!mActiveD3DDevice)
        {
            DI_WARNING("No active device detected");
        }

        // get driver version
        mDriverVersion.major = HIWORD(mActiveD3DDevice->getAdapterIdentifier().DriverVersion.HighPart);
        mDriverVersion.minor = LOWORD(mActiveD3DDevice->getAdapterIdentifier().DriverVersion.HighPart);
        mDriverVersion.release = HIWORD(mActiveD3DDevice->getAdapterIdentifier().DriverVersion.LowPart);
        mDriverVersion.build = LOWORD(mActiveD3DDevice->getAdapterIdentifier().DriverVersion.LowPart);

        return true;
    }

    bool DiD3D9Driver::GetPresentParameter(const DiIntVec2& buffersize, D3DPRESENT_PARAMETERS &parameters, 
        D3DFORMAT &adapterFormat, const D3DDISPLAYMODE &adapterMode)
    {
        UINT adapter = (mAdapter == 0xFFFFFFFF) ? D3DADAPTER_DEFAULT : (UINT)mAdapter;

        memset(&parameters, 0, sizeof(D3DPRESENT_PARAMETERS));
        parameters.BackBufferWidth          = buffersize.x;
        parameters.BackBufferHeight         = buffersize.y;
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

        DI_ASSERT(false);

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
        if (mDeviceList)
        {
            DI_DELETE mDeviceList;
            mDeviceList = nullptr;
        }

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

        mD3D = nullptr;
        DI_INFO("Direct3D9 stuff successfully released.");
    }

    bool DiD3D9Driver::CreateDirect3D()
    {
        DI_ASSERT(!mD3D);
        mD3D = Direct3DCreate9(D3D_SDK_VERSION);
        D3D9 = mD3D;
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

        DI_LOG("Try to reset D3D9 device");
        HRESULT res = mDevice->Reset(&mMainParameters);
        DiD3D9Driver::StateCache->DirtyAll();

        if (res != S_OK)
        {
            DI_LOG("Failed to reset D3D9 device: %s", DiD3D9ErrorCheck::GetResultString(res));
        }
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

    DiGfxCaps* DiD3D9Driver::InitGfxCapsImpl()
    {
        DiGfxCaps* rsc = DI_NEW DiGfxCaps();
        mCaps = rsc;

        rsc->setRenderSystemName(DRV_DIRECT3D9);
        rsc->setCategoryRelevant(CAPS_CATEGORY_D3D9, true);
        rsc->setDriverVersion(mDriverVersion);
        rsc->setDeviceName(mActiveD3DDevice->DriverDescription());

        // Init caps to maximum.		
        rsc->setNumTextureUnits(1024);
        rsc->setCapability(RSC_ANISOTROPY);
        rsc->setCapability(RSC_AUTOMIPMAP);
        rsc->setCapability(RSC_DOT3);
        rsc->setCapability(RSC_CUBEMAPPING);
        rsc->setCapability(RSC_SCISSOR_TEST);
        rsc->setCapability(RSC_TWO_SIDED_STENCIL);
        rsc->setCapability(RSC_STENCIL_WRAP);
        rsc->setCapability(RSC_HWOCCLUSION);
        rsc->setCapability(RSC_USER_CLIP_PLANES);
        rsc->setCapability(RSC_32BIT_INDEX);
        rsc->setCapability(RSC_VERTEX_FORMAT_UBYTE4);
        rsc->setCapability(RSC_TEXTURE_1D);
        rsc->setCapability(RSC_TEXTURE_3D);
        rsc->setCapability(RSC_NON_POWER_OF_2_TEXTURES);
        rsc->setNonPOW2TexturesLimited(false);
        rsc->setNumMultiRenderTargets(MAX_MRT);
        rsc->setCapability(RSC_MRT_DIFFERENT_BIT_DEPTHS);
        rsc->setCapability(RSC_POINT_SPRITES);
        rsc->setCapability(RSC_POINT_EXTENDED_PARAMETERS);
        rsc->setMaxPointSize(2.19902e+012f);
        rsc->setCapability(RSC_MIPMAP_LOD_BIAS);
        rsc->setCapability(RSC_PERSTAGECONSTANT);
        rsc->setCapability(RSC_HWSTENCIL);
        rsc->setStencilBufferBitDepth(8);
        rsc->setCapability(RSC_ADVANCED_BLEND_OPERATIONS);
        rsc->setCapability(RSC_RTT_SEPARATE_DEPTHBUFFER);
        rsc->setCapability(RSC_RTT_MAIN_DEPTHBUFFER_ATTACHABLE);
        rsc->setCapability(RSC_RTT_DEPTHBUFFER_RESOLUTION_LESSEQUAL);
        rsc->setCapability(RSC_VERTEX_BUFFER_INSTANCE_DATA);
        rsc->setCapability(RSC_CAN_GET_COMPILED_SHADER_BUFFER);

        // currently we just care about the first device
        IDirect3DDevice9* d3d9Device = Device;

        IDirect3DSurface9* pSurf;

        // Check for hardware stencil support
        d3d9Device->GetDepthStencilSurface(&pSurf);

        if (pSurf != NULL)
        {
            D3DSURFACE_DESC surfDesc;

            pSurf->GetDesc(&surfDesc);
            pSurf->Release();

            if (surfDesc.Format != D3DFMT_D15S1 &&
                surfDesc.Format != D3DFMT_D24S8 &&
                surfDesc.Format != D3DFMT_D24X4S4 &&
                surfDesc.Format != D3DFMT_D24FS8)
                rsc->unsetCapability(RSC_HWSTENCIL);
        }

        // Check for hardware occlusion support
        HRESULT hr = d3d9Device->CreateQuery(D3DQUERYTYPE_OCCLUSION, NULL);
        if (FAILED(hr))
            rsc->unsetCapability(RSC_HWOCCLUSION);

        // Update RS caps using the minimum value found in adapter list.
        for (unsigned int i = 0; i < mDeviceList->count(); ++i)
        {
            D3D9Driver* pCurDriver = mDeviceList->item(i);
            const D3DCAPS9& rkCurCaps = pCurDriver->getD3D9DeviceCaps();

            if (rkCurCaps.MaxSimultaneousTextures < rsc->getNumTextureUnits())
            {
                rsc->setNumTextureUnits(static_cast<uint16>(rkCurCaps.MaxSimultaneousTextures));
            }

            // Check for Anisotropy.
            if (rkCurCaps.MaxAnisotropy <= 1)
                rsc->unsetCapability(RSC_ANISOTROPY);

            // Check automatic mipmap generation.
            if ((rkCurCaps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP) == 0)
                rsc->unsetCapability(RSC_AUTOMIPMAP);

            // Check Dot product 3.
            if ((rkCurCaps.TextureOpCaps & D3DTEXOPCAPS_DOTPRODUCT3) == 0)
                rsc->unsetCapability(RSC_DOT3);

            // Scissor test
            if ((rkCurCaps.RasterCaps & D3DPRASTERCAPS_SCISSORTEST) == 0)
                rsc->unsetCapability(RSC_SCISSOR_TEST);

            // Two-sided stencil
            if ((rkCurCaps.StencilCaps & D3DSTENCILCAPS_TWOSIDED) == 0)
                rsc->unsetCapability(RSC_TWO_SIDED_STENCIL);

            // stencil wrap
            if ((rkCurCaps.StencilCaps & D3DSTENCILCAPS_INCR) == 0 ||
                (rkCurCaps.StencilCaps & D3DSTENCILCAPS_DECR) == 0)
                rsc->unsetCapability(RSC_STENCIL_WRAP);

            // User clip planes
            if (rkCurCaps.MaxUserClipPlanes == 0)
                rsc->unsetCapability(RSC_USER_CLIP_PLANES);

            // D3DFMT_INDEX32 type?
            if (rkCurCaps.MaxVertexIndex <= 0xFFFF)
                rsc->unsetCapability(RSC_32BIT_INDEX);

            // UBYTE4 type?
            if ((rkCurCaps.DeclTypes & D3DDTCAPS_UBYTE4) == 0)
                rsc->unsetCapability(RSC_VERTEX_FORMAT_UBYTE4);

            // Check cube map support.
            if ((rkCurCaps.TextureCaps & D3DPTEXTURECAPS_CUBEMAP) == 0)
                rsc->unsetCapability(RSC_CUBEMAPPING);

            // 3D textures?
            if ((rkCurCaps.TextureCaps & D3DPTEXTURECAPS_VOLUMEMAP) == 0)
                rsc->unsetCapability(RSC_TEXTURE_3D);

            if (rkCurCaps.TextureCaps & D3DPTEXTURECAPS_POW2)
            {
                // Conditional support for non POW2
                if (rkCurCaps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL)
                    rsc->setNonPOW2TexturesLimited(true);

                // Only power of 2 supported.
                else
                    rsc->unsetCapability(RSC_NON_POWER_OF_2_TEXTURES);
            }

            // Number of render targets
            if (rkCurCaps.NumSimultaneousRTs < rsc->getNumMultiRenderTargets())
            {
                rsc->setNumMultiRenderTargets(std::min((uint16)rkCurCaps.NumSimultaneousRTs, (uint16)MAX_MRT));
            }

            if ((rkCurCaps.PrimitiveMiscCaps & D3DPMISCCAPS_MRTINDEPENDENTBITDEPTHS) == 0)
            {
                rsc->unsetCapability(RSC_MRT_DIFFERENT_BIT_DEPTHS);
            }

            // Point sprites 
            if (rkCurCaps.MaxPointSize <= 1.0f)
            {
                rsc->unsetCapability(RSC_POINT_SPRITES);
                // sprites and extended parameters go together in D3D
                rsc->unsetCapability(RSC_POINT_EXTENDED_PARAMETERS);
            }

            // Take the minimum point size.
            if (rkCurCaps.MaxPointSize < rsc->getMaxPointSize())
                rsc->setMaxPointSize(rkCurCaps.MaxPointSize);

            // Mipmap LOD biasing?
            if ((rkCurCaps.RasterCaps & D3DPRASTERCAPS_MIPMAPLODBIAS) == 0)
                rsc->unsetCapability(RSC_MIPMAP_LOD_BIAS);


            // Do we support per-stage src_manual constants?
            // HACK - ATI drivers seem to be buggy and don't support per-stage constants properly?
            // TODO: move this to RSC
            if ((rkCurCaps.PrimitiveMiscCaps & D3DPMISCCAPS_PERSTAGECONSTANT) == 0)
                rsc->unsetCapability(RSC_PERSTAGECONSTANT);

            // Advanced blend operations? min max subtract rev 
            if ((rkCurCaps.PrimitiveMiscCaps & D3DPMISCCAPS_BLENDOP) == 0)
                rsc->unsetCapability(RSC_ADVANCED_BLEND_OPERATIONS);
        }

        // Blending between stages supported
        rsc->setCapability(RSC_BLENDING);


        // We always support compression, D3DX will decompress if device does not support
        rsc->setCapability(RSC_TEXTURE_COMPRESSION);
        rsc->setCapability(RSC_TEXTURE_COMPRESSION_DXT);

        // We always support VBOs
        rsc->setCapability(RSC_VBO);

        ConvertVertexShaderCaps(rsc);

        ConvertPixelShaderCaps(rsc);

        // Adapter details
        const D3DADAPTER_IDENTIFIER9& adapterID = mActiveD3DDevice->getAdapterIdentifier();

        // determine vendor
        // Full list of vendors here: http://www.pcidatabase.com/vendors.php?sort=id
        switch (adapterID.VendorId)
        {
        case 0x10DE:
            rsc->setVendor(GPU_NVIDIA);
            break;
        case 0x1002:
            rsc->setVendor(GPU_AMD);
            break;
        case 0x163C:
        case 0x8086:
            rsc->setVendor(GPU_INTEL);
            break;
        case 0x5333:
            rsc->setVendor(GPU_S3);
            break;
        case 0x3D3D:
            rsc->setVendor(GPU_3DLABS);
            break;
        case 0x102B:
            rsc->setVendor(GPU_MATROX);
            break;
        case 0x1039:
            rsc->setVendor(GPU_SIS);
            break;
        default:
            rsc->setVendor(GPU_UNKNOWN);
            break;
        };

        // Infinite projection?
        // We have no capability for this, so we have to base this on our
        // experience and reports from users
        // Non-vertex program capable hardware does not appear to support it
        if (rsc->hasCapability(RSC_VERTEX_PROGRAM))
        {
            // GeForce4 Ti (and presumably GeForce3) does not
            // render infinite projection properly, even though it does in GL
            // So exclude all cards prior to the FX range from doing infinite
            if (rsc->getVendor() != GPU_NVIDIA || // not nVidia
                !((adapterID.DeviceId >= 0x200 && adapterID.DeviceId <= 0x20F) || //gf3
                (adapterID.DeviceId >= 0x250 && adapterID.DeviceId <= 0x25F) || //gf4ti
                (adapterID.DeviceId >= 0x280 && adapterID.DeviceId <= 0x28F) || //gf4ti
                (adapterID.DeviceId >= 0x170 && adapterID.DeviceId <= 0x18F) || //gf4 go
                (adapterID.DeviceId >= 0x280 && adapterID.DeviceId <= 0x28F)))  //gf4ti go
            {
                rsc->setCapability(RSC_INFINITE_FAR_PLANE);
            }
        }

        // We always support rendertextures bigger than the frame buffer
        rsc->setCapability(RSC_HWRENDER_TO_TEXTURE);

        // Determine if any floating point texture format is supported
        D3DFORMAT floatFormats[6] = { D3DFMT_R16F, D3DFMT_G16R16F,
            D3DFMT_A16B16G16R16F, D3DFMT_R32F, D3DFMT_G32R32F,
            D3DFMT_A32B32G32R32F };

        DiD3D9RenderTarget* backTarget = static_cast<DiD3D9RenderTarget*>(GetMainRenderWindow()->GetRenderBuffer());
        IDirect3DSurface9* bbSurf = backTarget->GetSurface();
        D3DSURFACE_DESC bbSurfDesc;
        bbSurf->GetDesc(&bbSurfDesc);

        for (int i = 0; i < 6; ++i)
        {
            if (SUCCEEDED(mD3D->CheckDeviceFormat(mActiveD3DDevice->getAdapterNumber(),
                D3DDEVTYPE_HAL, bbSurfDesc.Format,
                0, D3DRTYPE_TEXTURE, floatFormats[i])))
            {
                rsc->setCapability(RSC_TEXTURE_FLOAT);
                break;
            }
        }

        // TODO: make convertVertex/Fragment fill in rsc
        // TODO: update the below line to use rsc
        // Vertex textures
        if (rsc->isShaderProfileSupported("vs_3_0"))
        {
            // Run through all the texture formats looking for any which support
            // vertex texture fetching. Must have at least one!
            // All ATI Radeon up to X1n00 say they support vs_3_0, 
            // but they support no texture formats for vertex texture fetch (cheaters!)
            if (CheckVertexTextureFormats(bbSurf))
            {
                rsc->setCapability(RSC_VERTEX_TEXTURE_FETCH);
                // always 4 vertex texture units in vs_3_0, and never shared
                rsc->setNumVertexTextureUnits(4);
                rsc->setVertexTextureUnitsShared(false);
            }
        }
        else
        {
            //True HW Instancing is supported since Shader model 3.0 ATI has a nasty
            //hack for enabling it in their SM 2.0 cards, but we don't (and won't) support it
            rsc->unsetCapability(RSC_VERTEX_BUFFER_INSTANCE_DATA);
        }

        // Check alpha to coverage support
        // this varies per vendor! But at least SM3 is required
        if (rsc->isShaderProfileSupported("ps_3_0"))
        {
            // NVIDIA needs a separate check
            if (rsc->getVendor() == GPU_NVIDIA)
            {
                if (mD3D->CheckDeviceFormat(
                    D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, 0, D3DRTYPE_SURFACE,
                    (D3DFORMAT)MAKEFOURCC('A', 'T', 'O', 'C')) == S_OK)
                {
                    rsc->setCapability(RSC_ALPHA_TO_COVERAGE);
                }

            }
            else if (rsc->getVendor() == GPU_AMD)
            {
                // There is no check on ATI, we have to assume SM3 == support
                rsc->setCapability(RSC_ALPHA_TO_COVERAGE);
            }

            // no other cards have Dx9 hacks for alpha to coverage, as far as I know
        }

        rsc->addShaderProfile("hlsl");

        rsc->LogCaps();

        return rsc;
    }

    D3D9DriverList* DiD3D9Driver::GetD3D9Devices()
    {
        if (!mDeviceList)
            mDeviceList = DI_NEW D3D9DriverList();

        return mDeviceList;
    }

    void DiD3D9Driver::ConvertVertexShaderCaps(DiGfxCaps* rsc) const
    {
        uint16 major = 0xFF;
        uint16 minor = 0xFF;
        D3DCAPS9 minVSCaps;

        // Find the device with the lowest vertex shader caps.
        for (unsigned int i = 0; i < mDeviceList->count(); ++i)
        {
            D3D9Driver* pCurDriver = mDeviceList->item(i);
            const D3DCAPS9& rkCurCaps = pCurDriver->getD3D9DeviceCaps();
            uint16 currMajor = static_cast<uint16>((rkCurCaps.VertexShaderVersion & 0x0000FF00) >> 8);
            uint16 currMinor = static_cast<uint16>(rkCurCaps.VertexShaderVersion & 0x000000FF);

            if (currMajor < major)
            {
                major = currMajor;
                minor = currMinor;
                minVSCaps = rkCurCaps;
            }
            else if (currMajor == major && currMinor < minor)
            {
                minor = currMinor;
                minVSCaps = rkCurCaps;
            }
        }

        // In case we didn't found any vertex shader support
        // try the IDirect3DDevice9 caps instead of the IDirect3D9
        // software vertex processing is reported there
        if (major == 0 && minor == 0)
        {
            IDirect3DDevice9* lpD3DDevice9 = Device;
            D3DCAPS9 d3dDeviceCaps9;
            lpD3DDevice9->GetDeviceCaps(&d3dDeviceCaps9);
            major = static_cast<uint16>((d3dDeviceCaps9.VertexShaderVersion & 0x0000FF00) >> 8);
            minor = static_cast<uint16>(d3dDeviceCaps9.VertexShaderVersion & 0x000000FF);
        }

        bool vs2x = false;
        bool vs2a = false;

        // Special case detection for vs_2_x/a support
        if (major >= 2)
        {
            if ((minVSCaps.VS20Caps.Caps & D3DVS20CAPS_PREDICATION) &&
                (minVSCaps.VS20Caps.DynamicFlowControlDepth > 0) &&
                (minVSCaps.VS20Caps.NumTemps >= 12))
            {
                vs2x = true;
            }

            if ((minVSCaps.VS20Caps.Caps & D3DVS20CAPS_PREDICATION) &&
                (minVSCaps.VS20Caps.DynamicFlowControlDepth > 0) &&
                (minVSCaps.VS20Caps.NumTemps >= 13))
            {
                vs2a = true;
            }
        }

        // Populate max param count
        switch (major)
        {
        case 1:
            // No boolean params allowed
            rsc->setVertexProgramConstantBoolCount(0);
            // No integer params allowed
            rsc->setVertexProgramConstantIntCount(0);
            // float params, always 4D
            rsc->setVertexProgramConstantFloatCount(static_cast<uint16>(minVSCaps.MaxVertexShaderConst));

            break;
        case 2:
            // 16 boolean params allowed
            rsc->setVertexProgramConstantBoolCount(16);
            // 16 integer params allowed, 4D
            rsc->setVertexProgramConstantIntCount(16);
            // float params, always 4D
            rsc->setVertexProgramConstantFloatCount(static_cast<uint16>(minVSCaps.MaxVertexShaderConst));
            break;
        case 3:
            // 16 boolean params allowed
            rsc->setVertexProgramConstantBoolCount(16);
            // 16 integer params allowed, 4D
            rsc->setVertexProgramConstantIntCount(16);
            // float params, always 4D
            rsc->setVertexProgramConstantFloatCount(static_cast<uint16>(minVSCaps.MaxVertexShaderConst));
            break;
        }

        // populate syntax codes in program manager (no breaks in this one so it falls through)
        switch (major)
        {
        case 3:
            rsc->addShaderProfile("vs_3_0");
        case 2:
            if (vs2x)
                rsc->addShaderProfile("vs_2_x");
            if (vs2a)
                rsc->addShaderProfile("vs_2_a");

            rsc->addShaderProfile("vs_2_0");
        case 1:
            rsc->addShaderProfile("vs_1_1");
            rsc->setCapability(RSC_VERTEX_PROGRAM);
        }
    }

    void DiD3D9Driver::ConvertPixelShaderCaps(DiGfxCaps* rsc) const
    {
        uint16 major = 0xFF;
        uint16 minor = 0xFF;
        D3DCAPS9 minPSCaps;

        // Find the device with the lowest pixel shader caps.
        for (unsigned int i = 0; i < mDeviceList->count(); ++i)
        {
            D3D9Driver* pCurDriver = mDeviceList->item(i);
            const D3DCAPS9& currCaps = pCurDriver->getD3D9DeviceCaps();
            uint16 currMajor = static_cast<uint16>((currCaps.PixelShaderVersion & 0x0000FF00) >> 8);
            uint16 currMinor = static_cast<uint16>(currCaps.PixelShaderVersion & 0x000000FF);

            if (currMajor < major)
            {
                major = currMajor;
                minor = currMinor;
                minPSCaps = currCaps;
            }
            else if (currMajor == major && currMinor < minor)
            {
                minor = currMinor;
                minPSCaps = currCaps;
            }
        }

        bool ps2a = false;
        bool ps2b = false;
        bool ps2x = false;

        // Special case detection for ps_2_x/a/b support
        if (major >= 2)
        {
            if ((minPSCaps.PS20Caps.Caps & D3DPS20CAPS_NOTEXINSTRUCTIONLIMIT) &&
                (minPSCaps.PS20Caps.NumTemps >= 32))
            {
                ps2b = true;
            }

            if ((minPSCaps.PS20Caps.Caps & D3DPS20CAPS_NOTEXINSTRUCTIONLIMIT) &&
                (minPSCaps.PS20Caps.Caps & D3DPS20CAPS_NODEPENDENTREADLIMIT) &&
                (minPSCaps.PS20Caps.Caps & D3DPS20CAPS_ARBITRARYSWIZZLE) &&
                (minPSCaps.PS20Caps.Caps & D3DPS20CAPS_GRADIENTINSTRUCTIONS) &&
                (minPSCaps.PS20Caps.Caps & D3DPS20CAPS_PREDICATION) &&
                (minPSCaps.PS20Caps.NumTemps >= 22))
            {
                ps2a = true;
            }

            // Does this enough?
            if (ps2a || ps2b)
            {
                ps2x = true;
            }
        }

        switch (major)
        {
        case 1:
            // no boolean params allowed
            rsc->setFragmentProgramConstantBoolCount(0);
            // no integer params allowed
            rsc->setFragmentProgramConstantIntCount(0);
            // float params, always 4D
            // NB in ps_1_x these are actually stored as fixed point values,
            // but they are entered as floats
            rsc->setFragmentProgramConstantFloatCount(8);
            break;
        case 2:
            // 16 boolean params allowed
            rsc->setFragmentProgramConstantBoolCount(16);
            // 16 integer params allowed, 4D
            rsc->setFragmentProgramConstantIntCount(16);
            // float params, always 4D
            rsc->setFragmentProgramConstantFloatCount(32);
            break;
        case 3:
            // 16 boolean params allowed
            rsc->setFragmentProgramConstantBoolCount(16);
            // 16 integer params allowed, 4D
            rsc->setFragmentProgramConstantIntCount(16);
            // float params, always 4D
            rsc->setFragmentProgramConstantFloatCount(224);
            break;
        }

        // populate syntax codes in program manager (no breaks in this one so it falls through)
        switch (major)
        {
        case 3:
            if (minor > 0)
                rsc->addShaderProfile("ps_3_x");

            rsc->addShaderProfile("ps_3_0");
        case 2:
            if (ps2x)
                rsc->addShaderProfile("ps_2_x");
            if (ps2a)
                rsc->addShaderProfile("ps_2_a");
            if (ps2b)
                rsc->addShaderProfile("ps_2_b");

            rsc->addShaderProfile("ps_2_0");
        case 1:
            if (major > 1 || minor >= 4)
                rsc->addShaderProfile("ps_1_4");
            if (major > 1 || minor >= 3)
                rsc->addShaderProfile("ps_1_3");
            if (major > 1 || minor >= 2)
                rsc->addShaderProfile("ps_1_2");

            rsc->addShaderProfile("ps_1_1");
            rsc->setCapability(RSC_FRAGMENT_PROGRAM);
        }
    }

    bool DiD3D9Driver::CheckVertexTextureFormats(IDirect3DSurface9* bbSurf) const
    {
        bool anySupported = false;

        D3DSURFACE_DESC bbSurfDesc;
        bbSurf->GetDesc(&bbSurfDesc);

        for (uint32 ipf = static_cast<uint32>(PF_R8G8B8); ipf < static_cast<uint32>(PIXEL_FORMAT_MAX); ++ipf)
        {
            DiPixelFormat pf = (DiPixelFormat)ipf;

            D3DFORMAT fmt = DiD3D9Mappings::D3D9FormatMapping[pf];

            if (SUCCEEDED(mD3D->CheckDeviceFormat(
                mActiveD3DDevice->getAdapterNumber(), D3DDEVTYPE_HAL, bbSurfDesc.Format,
                D3DUSAGE_QUERY_VERTEXTEXTURE, D3DRTYPE_TEXTURE, fmt)))
            {
                // cool, at least one supported
                anySupported = true;
                DI_LOG("D3D9: Vertex texture format supported: %s", DiPixelBox::GetPixelTypeName(pf).c_str());
            }
        }

        return anySupported;
    }

    void DiD3D9Driver::PostInit()
    {
        InitGfxCapsImpl();
    }

    bool DiD3D9Driver::ReadyToReset()
    {
        uint32 w = 0;
        uint32 h = 0;
        GetWindowSize(mMainHwnd, w, h);
        if (w == 0 || h == 0)
        {
            return false;
        }

        HRESULT hr = mDevice->TestCooperativeLevel();
        if (hr == D3DERR_DEVICELOST ||
            hr == D3DERR_DRIVERINTERNALERROR)
        {
            return false;
        }
        return true;
    }


}