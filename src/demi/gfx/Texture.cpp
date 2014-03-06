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
        , mFilter(FILTER_DEFAULT)
        , mTextureDrv(nullptr)
        , mResUsage(RU_NONE)
        , mAutoGenerateMipmap(true)
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

        mUScroll       = 0;
        mVScroll       = 0;
        mRotateSpeed   = 0;
        mRotateDegree  = 0;
        mUScale        = 0;
        mVScale        = 0;
        mUTrans        = 0;
        mVTrans        = 0;
        mRotation      = 0;
        mUScale        = 1;
        mVScale        = 1;

        mNeedRecalcTexMat = false;

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
                DI_WARNING("Render target cannot be compressed format.");
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

        if (mUsage & TU_RENDER_TARGET)
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

    void DiTexture::SetUScale(float val)
    {
        mUScale = val;
        mNeedRecalcTexMat = true;
    }

    void DiTexture::SetVScale(float val)
    {
        mVScale = val;
        mNeedRecalcTexMat = true;
    }

    void DiTexture::SetRotateDegree(float val)
    {
        mRotateDegree = val;
        mNeedRecalcTexMat = true;
    }

    void DiTexture::_UpdateUVAnimation(float delta)
    {
        if (!DiMath::RealEqual(mUScroll, 0))
        {
            mUTrans -= delta * mUScroll;
            while (mUTrans >= 1.0)
                mUTrans -= 1.0;
            while (mUTrans < 0.0)
                mUTrans += 1.0;
            mNeedRecalcTexMat = true;
        }
        else
        {
            if (!DiMath::RealEqual(mUTrans, 0))
            {
                mUTrans = 0;
                mNeedRecalcTexMat = true;
            }
        }
        
        if (!DiMath::RealEqual(mVScroll, 0))
        {
            mVTrans -= delta * mVScroll;
            while (mVTrans >= 1.0)
                mVTrans -= 1.0;
            while (mVTrans < 0.0)
                mVTrans += 1.0;
            mNeedRecalcTexMat = true;
        }
        else
        {
            if (!DiMath::RealEqual(mVTrans, 0))
            {
                mVTrans = 0;
                mNeedRecalcTexMat = true;
            }
        }
        
        if (!DiMath::RealEqual(mRotateSpeed, 0))
        {
            mRotation -= delta * mRotateSpeed * 360;
            while (mRotation >= 360)
                mRotation -= 360;
            while (mRotation < 360)
                mRotation += 360;
            mNeedRecalcTexMat = true;
        }
        else
        {
            if (!DiMath::RealEqual(mRotation, 0))
            {
                mRotation = 0;
                mNeedRecalcTexMat = true;
            }
        }
    }

    void DiTexture::_RecalcTextureMatrix()
    {
        DiMat4 xform;

        xform = DiMat4::IDENTITY;
        if (mUScale != 1 || mVScale != 1)
        {
            xform[0][0] = 1 / mUScale;
            xform[1][1] = 1 / mVScale;

            xform[0][3] = (-0.5f * xform[0][0]) + 0.5f;
            xform[1][3] = (-0.5f * xform[1][1]) + 0.5f;
        }

        if (mUTrans != 0 || mVTrans != 0)
        {
            DiMat4 xlate = DiMat4::IDENTITY;

            xlate[0][3] = mUTrans;
            xlate[1][3] = mVTrans;

            xform = xlate * xform;
        }

        float rotate = mRotateDegree + mRotation;
        if (!DiMath::RealEqual(rotate, 0))
        {
            DiMat4 rot = DiMat4::IDENTITY;
            DiDegree theta(rotate);
            float rad = theta.valueRadians();

            float cosTheta = DiMath::Cos(rad);
            float sinTheta = DiMath::Sin(rad);

            rot[0][0] = cosTheta;
            rot[0][1] = -sinTheta;
            rot[1][0] = sinTheta;
            rot[1][1] = cosTheta;

            rot[0][3] = 0.5f + ((-0.5f * cosTheta) - (-0.5f * sinTheta));
            rot[1][3] = 0.5f + ((-0.5f * sinTheta) + (-0.5f * cosTheta));

            xform = rot * xform;
        }

        mTextureMatrix = xform;
        mNeedRecalcTexMat = false;
    }

    void DiTexture::SetRotateSpeed(float speed)
    {
        mRotateSpeed = speed;
    }

    void DiTexture::_Update(float delta)
    {
        _UpdateUVAnimation(delta);

        if (mNeedRecalcTexMat)
            _RecalcTextureMatrix();
    }

    void DiTextureDrv::CopyFromMemory(const DiPixelBox& srcBox, uint32 level, uint32 surface /*= 0*/)
    {
        DiBox dest(0, 0, srcBox.GetWidth(), srcBox.GetHeight());
        CopyFromMemory(srcBox, dest, level, surface);
    }
}