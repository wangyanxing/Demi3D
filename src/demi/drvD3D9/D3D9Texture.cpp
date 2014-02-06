
#include "DrvD3D9Pch.h"
#include "D3D9Texture.h"
#include "Image.h"
#include "D3D9Driver.h"
#include "D3D9StateCache.h"
#include "D3D9TypeMappings.h"
#include "Command.h"
#include "ConsoleVariable.h"

namespace Demi
{
    static RECT ToD3DRECT(const DiBox &lockBox)
    {
        RECT prect;
        prect.left = static_cast<LONG>(lockBox.left);
        prect.right = static_cast<LONG>(lockBox.right);
        prect.top = static_cast<LONG>(lockBox.top);
        prect.bottom = static_cast<LONG>(lockBox.bottom);
        return prect;
    }

    DiD3D9TextureDrv::DiD3D9TextureDrv(DiTexture* parent)
        :mD3DUsage(0),
        mPool(D3DPOOL_MANAGED),
        mSurface(nullptr),
        mTexture(nullptr)
    {
        mParent = parent;
    }

    DiD3D9TextureDrv::~DiD3D9TextureDrv()
    {
        Release();
    }

    static void FromD3DLock(DiPixelBox &rval, const D3DLOCKED_RECT &lrect)
    {
        size_t bpp = DiPixelBox::GetNumElemBytes(rval.format);
        if (bpp != 0)
        {
            rval.rowPitch = lrect.Pitch / bpp;
            rval.slicePitch = rval.rowPitch * rval.GetHeight();
            DI_ASSERT((lrect.Pitch % bpp) == 0);
        }
        else if (DiPixelBox::IsCompressedFormat(rval.format))
        {
            rval.rowPitch = rval.GetWidth();
            rval.slicePitch = rval.GetWidth() * rval.GetHeight();
        }
        else
        {
            DI_WARNING("Unsupported FromD3DLock() calling.");
        }

        rval.data = lrect.pBits;
    }

    void DiD3D9TextureDrv::CopyToMemory(const DiBox &srcBox, const DiPixelBox &dst)
    {
        if (mParent->GetTextureType() == TEXTURE_CUBE)
        {
            DI_WARNING("Cannot blit cube texture to memory.");
            return;
        }

        DI_ASSERT(mSurface);

        D3DLOCKED_RECT lrect;

        HRESULT res = mSurface->LockRect(&lrect, NULL, D3DLOCK_READONLY);
        DX9_CHKERR(res);

        DiPixelBox locked(dst.GetWidth(), dst.GetHeight(), mParent->GetFormat());
        FromD3DLock(locked, lrect);
        DiPixelBox::BulkPixelConversion(locked, dst);

        mSurface->UnlockRect();
    }

    void DiD3D9TextureDrv::Release()
    {
        DI_DEBUG("D3D9 texture released: %x", (void*)mTexture);
        SAFE_RELEASE(mTexture);
        if (mD3DUsage & D3DUSAGE_DEPTHSTENCIL)
        {
            DI_DEBUG("D3D9 depth/stencil buffer released: %x", (void*)mSurface);
            SAFE_RELEASE(mSurface);
        }
    }

    void DiD3D9TextureDrv::CreateTexture()
    {
        mD3DUsage = 0;
        mPool = D3DPOOL_MANAGED;
        if (mParent->GetResourceUsage() & RU_DYNAMIC)
        {
            mD3DUsage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
            mPool = D3DPOOL_DEFAULT;
        }

        if (mParent->GetUsage() & TU_DEPTH_STENCIL)
        {
            mD3DUsage |= D3DUSAGE_DEPTHSTENCIL;
            mPool = D3DPOOL_DEFAULT;
        }
        if (mParent->GetUsage() & TU_RENDER_TARGET)
        {
            mD3DUsage |= D3DUSAGE_RENDERTARGET;
            mPool = D3DPOOL_DEFAULT;
        }

        uint32 width = mParent->GetWidth();
        uint32 height = mParent->GetHeight();
        uint32 numLevels = mParent->GetNumLevels();
        DiPixelFormat fmt = mParent->GetFormat();

        D3DFORMAT d3dfmt = DiD3D9Mappings::D3D9FormatMapping[fmt];

        D3DRESOURCETYPE resType = D3DRTYPE_TEXTURE;
        if (mParent->GetTextureType() == TEXTURE_CUBE)
            resType = D3DRTYPE_CUBETEXTURE;
        
        if (mParent->IsAutoMipmap() && static_cast<DiD3D9Driver*>(Driver)->CanAutoGenMipmaps(mD3DUsage, resType, d3dfmt))
        {
            mD3DUsage |= D3DUSAGE_AUTOGENMIPMAP;
            numLevels = 0;
        }

        if (mD3DUsage & D3DUSAGE_DEPTHSTENCIL)
        {
            mTexture = nullptr;
            mSurface = DiD3D9Driver::CreateDepthStencil(width, height, d3dfmt);
            DI_ASSERT(mSurface);
            DI_DEBUG("D3D9 Depth stencil buffer created: (%d,%d), ptr:%x", width, height, (void*)mSurface);
        }
        else
        {
            if (mParent->GetTextureType() == TEXTURE_2D)
            {
                IDirect3DTexture9* tex2D = DiD3D9Driver::CreateTexture(width, height,
                    numLevels, mD3DUsage, d3dfmt, mPool);
                DI_ASSERT(tex2D);
                tex2D->GetSurfaceLevel(0, &mSurface);
                mTexture = tex2D;
                DI_DEBUG("D3D9 2D texture created: (%d,%d), pool:%d, usage:%d, ptr:%x", width, height, mPool, mD3DUsage, (void*)tex2D);
            }
            else if (mParent->GetTextureType() == TEXTURE_CUBE)
            {
                DI_ASSERT(width == height);
                IDirect3DCubeTexture9* texCUBE = DiD3D9Driver::CreateCubeTexture(width, numLevels,
                    mD3DUsage, d3dfmt, mPool);
                DI_DEBUG("D3D9 cube texture created: (%d,%d), pool:%d, usage:%d, ptr:%x", width, width, mPool, mD3DUsage, (void*)texCUBE);
                DI_ASSERT(texCUBE);
                mTexture = texCUBE;
                mSurface = nullptr;
            }
        }
    }

    void* DiD3D9TextureDrv::GetSurfaceHandle()
    {
        return (void*)mSurface;
    }

    void DiD3D9TextureDrv::Bind(uint32 samplerIndex)
    {
        DiD3D9Driver::StateCache->SetTexture(samplerIndex, mTexture);
        if (mTexture)
        {
            DiFilterType filter = mParent->GetFilter();
            if (filter == FILTER_DEFAULT)
            {
                auto filterVar = CommandMgr->GetConsoleVar("def_filter");
                if (filterVar)
                    filter = (DiFilterType)filterVar->GetAsInt();
                else
                    filter = FILTER_BILINEAR;
            }

            D3DTEXTUREFILTERTYPE minMag = DiD3D9Mappings::D3D9MinMagFilter[filter];
            D3DTEXTUREFILTERTYPE mip = DiD3D9Mappings::D3D9MipFilter[filter];
            DiD3D9Driver::StateCache->SetSamplerState((DWORD)samplerIndex, D3DSAMP_MINFILTER, minMag);
            DiD3D9Driver::StateCache->SetSamplerState((DWORD)samplerIndex, D3DSAMP_MAGFILTER, minMag);
            DiD3D9Driver::StateCache->SetSamplerState((DWORD)samplerIndex, D3DSAMP_MIPFILTER, mip);

            DiD3D9Driver::StateCache->SetSamplerState((DWORD)samplerIndex, D3DSAMP_ADDRESSU,  DiD3D9Mappings::D3D9AddModeMapping[mParent->GetAddressingU()]);
            DiD3D9Driver::StateCache->SetSamplerState((DWORD)samplerIndex, D3DSAMP_ADDRESSV,  DiD3D9Mappings::D3D9AddModeMapping[mParent->GetAddressingV()]);

            if (mParent->GetAddressingU() == AM_BORDER ||
                mParent->GetAddressingV() == AM_BORDER)
            {
                DiD3D9Driver::StateCache->SetSamplerState((DWORD)samplerIndex, D3DSAMP_BORDERCOLOR,
                    mParent->GetBorderColor().GetAsARGB());
            }
        }
    }

    void* DiD3D9TextureDrv::LockLevel(uint32 level, uint32 surface, DiLockFlag lockflag)
    {
        void *buffer = nullptr;
        if (mTexture)
        {
            DWORD d3dlockflag = 0;
            switch (lockflag)
            {
            case LOCK_DISCARD:
                // D3D only likes D3DLOCK_DISCARD if you created the texture with D3DUSAGE_DYNAMIC
                // debug runtime flags this up, could cause problems on some drivers
                if (mParent->GetResourceUsage() & RU_DYNAMIC)
                    d3dlockflag |= D3DLOCK_DISCARD;
                break;
            case LOCK_READ_ONLY:
                d3dlockflag |= D3DLOCK_READONLY;
                break;
            default:
                break;
            };

            if (mParent->GetTextureType() == TEXTURE_2D)
            {
                DI_ASSERT(surface == 0);
                IDirect3DTexture9* tex2D = static_cast<IDirect3DTexture9*>(mTexture);
                D3DLOCKED_RECT lockedRect;
                HRESULT result = tex2D->LockRect((DWORD)level, &lockedRect, 0, d3dlockflag);
                DX9_CHKERR(result);
                if (result == D3D_OK)
                {
                    buffer = lockedRect.pBits;
                }
            }
            else if (mParent->GetTextureType() == TEXTURE_CUBE)
            {
                DI_ASSERT(surface >= 0 && surface <= 5);
                IDirect3DCubeTexture9* texCUBE = static_cast<IDirect3DCubeTexture9*>(mTexture);
                D3DLOCKED_RECT lockedRect;
                HRESULT result = texCUBE->LockRect((D3DCUBEMAP_FACES)surface, (DWORD)level,
                    &lockedRect, 0, d3dlockflag);
                DX9_CHKERR(result);
                if (result == D3D_OK)
                {
                    buffer = lockedRect.pBits;
                }
            }
        }
        return buffer;
    }

    void DiD3D9TextureDrv::UnlockLevel(uint32 level, uint32 surface)
    {
        if (mTexture)
        {
            if (mParent->GetTextureType() == TEXTURE_2D)
            {
                IDirect3DTexture9* tex2D = static_cast<IDirect3DTexture9*>(mTexture);
                tex2D->UnlockRect(level);
            }
            else if (mParent->GetTextureType() == TEXTURE_CUBE)
            {
                IDirect3DCubeTexture9* texCUBE = static_cast<IDirect3DCubeTexture9*>(mTexture);
                texCUBE->UnlockRect((D3DCUBEMAP_FACES)surface, level);
            }
        }
    }

    void DiD3D9TextureDrv::CopyFromMemory(const DiPixelBox &srcBox, const DiBox &dst, uint32 level, uint32 surface /*= 0*/)
    {
        size_t rowWidth;
        if (DiPixelBox::IsCompressedFormat(srcBox.format))
        {
            if (srcBox.format == PF_DXT1)
                rowWidth = (srcBox.rowPitch / 4) * 8;
            else
                rowWidth = (srcBox.rowPitch / 4) * 16;
        }
        else
        {
            rowWidth = srcBox.rowPitch * DiPixelBox::GetNumElemBytes(srcBox.format);
        }

        RECT destRect, srcRect;
        srcRect = ToD3DRECT(srcBox);
        destRect = ToD3DRECT(dst);

        IDirect3DSurface9* dxsurface = nullptr;

        if (mParent->GetTextureType() == TEXTURE_2D)
        {
            IDirect3DTexture9* tex2D = static_cast<IDirect3DTexture9*>(mTexture);
            DX9_CHKERR(tex2D->GetSurfaceLevel(level, &dxsurface));
        }
        else if (mParent->GetTextureType() == TEXTURE_CUBE)
        {
            IDirect3DCubeTexture9* texCUBE = static_cast<IDirect3DCubeTexture9*>(mTexture);
            DX9_CHKERR(texCUBE->GetCubeMapSurface((D3DCUBEMAP_FACES)surface, level, &dxsurface));
        }

        if (D3DXLoadSurfaceFromMemory(dxsurface, NULL, &destRect,
            srcBox.data, DiD3D9Mappings::D3D9FormatMapping[srcBox.format],
            static_cast<UINT>(rowWidth),
            NULL, &srcRect, D3DX_DEFAULT, 0) != D3D_OK)
        {
            DI_WARNING("D3DXLoadSurfaceFromMemory failed, cannot copy row stuff from memory.");
        }
    }

}