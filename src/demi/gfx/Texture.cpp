
#include "GfxPch.h"
#include "Texture.h"
#include "GfxDriver.h"
#include "Image.h"
#include "RenderTarget.h"
#include "GfxDriver.h"

namespace Demi
{
    DiAssetType DiTexture::TYPE = ASSET_TEXTURE;

    DiTexture::DiTexture(const DiString& name)
        :DiAsset(name)
        , mTextureDrv(nullptr)
        , mFilter(FILTER_DEFAULT)
        , mResUsage(RU_NONE)
    {
        mAddressingU   = AM_WRAP;
        mAddressingV   = AM_WRAP;
        mBorderColor   = DiColor(0, 0, 0, 0);
        mPixelFormat   = PIXEL_FORMAT_MAX;
                       
        mTextureType   = TEXTURE_2D;
        mUsage         = TU_TEXURE;
                       
        mTextureDrv    = Driver ? Driver->CreateTextureDriver(this) : nullptr;
        mRenderTarget  = nullptr;
                       
        mWidth         = 0;
        mHeight        = 0;
        mNumLevels     = 1;
        mUvSet         = 0;

        mViewportScale = DiVec2::UNIT_SCALE;

        mAdaptedRT     = nullptr;

        if (Driver)
            Driver->AddGpuResource(this);
    }

    DiTexture::~DiTexture()
    {
        Release();

        DI_DELETE mTextureDrv;
        mTextureDrv = nullptr;

        if (Driver)
            Driver->RemoveGpuResource(this);
    }

    void DiTexture::SetFilter(DiFilterType ft)
    {
        mFilter = ft;
    }

    void DiTexture::SetAddressingU(DiAddMode ad)
    {
        mAddressingU = ad;
    }

    void DiTexture::SetAddressingV(DiAddMode ad)
    {
        mAddressingV = ad;
    }

    void DiTexture::SetAddressing(DiAddMode ad)
    {
        mAddressingU = ad;
        mAddressingV = ad;
    }

    uint32 DiTexture::GetWidthInBlocks(void) const
    {
        return DiPixelBox::GetFormatNumBlocks(GetWidth(), GetFormat());
    }

    uint32 DiTexture::GetHeightInBlocks(void) const
    {
        return DiPixelBox::GetFormatNumBlocks(GetHeight(), GetFormat());
    }

    uint32 DiTexture::GetBlockSize(void) const
    {
        return DiPixelBox::GetFormatBlockSize(GetFormat());
    }

    void DiTexture::SetFormat(DiPixelFormat fmt)
    {
        mPixelFormat = fmt;
    }

    void DiTexture::SetNumLevels(uint32 numlevel)
    {
        mNumLevels = numlevel;
    }

    void DiTexture::SetDimensions(uint32 width, uint32 height)
    {
        mWidth = width;
        mHeight = height;
    }

    bool DiTexture::LoadingComplete() const
    {
        return true;
    }

    bool DiTexture::Load(DiDataStreamPtr data)
    {
        if (Driver)
        {
            DiImage image(data);
            return image.LoadToTexture(this);
        }
        else
            return true;
    }

    bool DiTexture::Load()
    {
        return true;
    }

    DiRenderTarget* DiTexture::GetRenderTarget()
    {
        return mRenderTarget;
    }

    void DiTexture::CopyToMemory(const DiPixelBox &dst)
    {
        CopyToMemory(DiBox(0, 0, mWidth, mHeight), dst);
    }

    void DiTexture::CreateTexture()
    {
        if (!mTextureDrv)
            return;

        mTextureDrv->Release();

        if (mUsage & (TU_DEPTH_STENCIL | TU_RENDER_TARGET))
        {
            if (DiPixelBox::IsCompressedFormat(mPixelFormat))
            {
                DI_ERROR("Render target cannot be compressed format.");
                return;
            }
        }

        if (mAdaptedRT)
        {
            uint32 width  = mAdaptedRT->GetWidth()  * mAdaptedRT->GetViewport().mWidth;
            uint32 height = mAdaptedRT->GetHeight() * mAdaptedRT->GetViewport().mHeight;
            mWidth  = width  * mViewportScale.x;
            mHeight = height * mViewportScale.y;
        }

        mTextureDrv->CreateTexture();

        if (mUsage & (TU_RENDER_TARGET))
        {
            if (!mRenderTarget)
                mRenderTarget = Driver->CreateRenderTarget();
            mRenderTarget->AttachTexture(shared_from_this());
        }
    }

    void DiTexture::Release()
    {
        if (mTextureDrv)
            mTextureDrv->Release();
        SAFE_DELETE(mRenderTarget);
    }

    void DiTexture::CopyToMemory(const DiBox &srcBox, const DiPixelBox &dst)
    {
        mTextureDrv->CopyToMemory(srcBox, dst);
    }

    void DiTexture::Bind(uint32 samplerIndex)
    {
        mTextureDrv->Bind(samplerIndex);
    }

    void* DiTexture::GetSurfaceHandle()
    {
        return mTextureDrv->GetSurfaceHandle();
    }

    void DiTexture::UnlockLevel(uint32 level, uint32 surface)
    {
        mTextureDrv->UnlockLevel(level, surface);
    }

    void DiTexture::OnDeviceLost(void)
    {
        if (mUsage == TU_TEXURE && (mResUsage & (RU_DYNAMIC | RU_WRITE_ONLY)))
            return;

        if (mTextureDrv)
            mTextureDrv->Release();
        mDeviceLost = true;

        if (mRenderTarget)
            mRenderTarget->DetachTexture();
    }

    void DiTexture::OnDeviceReset(void)
    {
        if (mDeviceLost)
        {
            CreateTexture();
            mDeviceLost = false;
        }
    }

    void* DiTexture::LockLevel(uint32 level, uint32 surface)
    {
        return mTextureDrv->LockLevel(level,surface);
    }

    void DiTextureDrv::CopyFromMemory(const DiPixelBox &srcBox, uint32 level, uint32 surface /*= 0*/)
    {
        DiBox dest(0, 0, srcBox.GetWidth(), srcBox.GetHeight());
        CopyFromMemory(srcBox, dest, level, surface);
    }
}