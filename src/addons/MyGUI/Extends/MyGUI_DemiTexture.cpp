/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#include "MyGUI_Precompiled.h"
#include "MyGUI_DemiTexture.h"
#include "MyGUI_DemiDataManager.h"
#include "MyGUI_DemiRTTexture.h"
#include "MyGUI_DemiDiagnostic.h"

#include "AssetManager.h"
#include "Texture.h"

namespace MyGUI
{
	DemiTexture::DemiTexture(const std::string& _name) :
		mName(_name),
		mNumElemBytes(0),
		mLock(false),
		mRenderTarget(nullptr)
	{
	}

	DemiTexture::~DemiTexture()
	{
		destroy();
	}

	const std::string& DemiTexture::getName() const
	{
		return mName;
	}

	void DemiTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
		destroy();
        
        static int textureid = 0;
        DiString texturename;
        texturename.Format("MyGUITex_%d",textureid++);

		mSize.set(_width, _height);
		mTextureUsage = _usage;
		mPixelFormat = _format;
        
        DiResUsage resusage = RU_WRITE_ONLY;
        DiTextureUsage texusage = TU_TEXURE;
        DiPixelFormat pf = PIXEL_FORMAT_MAX;
        
        if (mTextureUsage == TextureUsage::RenderTarget)
		{
            texusage = TU_RENDER_TARGET;
        }
        else if (mTextureUsage == TextureUsage::Dynamic)
        {
            resusage = RU_DYNAMIC;
        }
        else if (mTextureUsage == TextureUsage::Stream)
        {
            resusage = RU_DYNAMIC;
        }
        
        if (mPixelFormat == PixelFormat::R8G8B8A8)
		{
			pf = PF_A8R8G8B8;
			mNumElemBytes = 4;
		}
		else if (mPixelFormat == PixelFormat::R8G8B8)
		{
			pf = PF_R8G8B8;
			mNumElemBytes = 3;
		}
		else if (mPixelFormat == PixelFormat::L8)
		{
			pf = PF_L8;
			mNumElemBytes = 1;
		}
        else if (mPixelFormat == PixelFormat::L8A8)
		{
			pf = PF_A8L8;
			mNumElemBytes = 2;
		}
		else
		{
			MYGUI_PLATFORM_EXCEPT("Creating texture with unknown pixel format.");
		}
        
        mTexture = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiTexture>(texturename);
        mTexture->SetDimensions(_width, _height);
        mTexture->SetFormat(pf);
        mTexture->SetResourceUsage(resusage);
        mTexture->SetUsage(texusage);
        mTexture->SetAutoMipmap(false);
        mTexture->CreateTexture();
        mTexture->SetAddressing(AM_CLAMP);
        mTexture->SetFilter(FILTER_DEFAULT);
        mTexture->SetNumLevels(1);
	}

    void DemiTexture::loadFromDemiTexture(const std::string& _texName)
    {
        destroy();

        mTextureUsage = TextureUsage::Default;
        mPixelFormat = PixelFormat::R8G8B8A8;
        mNumElemBytes = 4;

        DiAssetManager& assetMgr = DiAssetManager::GetInstance();
        mTexture = assetMgr.GetAsset<DiTexture>(_texName.c_str());

        if (!mTexture)
        {
            DI_WARNING("Cannot load Demi texture: %s", _texName.c_str());
            return;
        }

        DiPixelFormat pf = mTexture->GetFormat();
        if (pf == PF_A8R8G8B8)
        {
            mPixelFormat = PixelFormat::R8G8B8A8;
            mNumElemBytes = 4;
        }
        else if (pf == PF_R8G8B8)
        {
            mPixelFormat = PixelFormat::R8G8B8;
            mNumElemBytes = 3;
        }
        else if (pf == PF_L8)
        {
            mPixelFormat = PixelFormat::L8;
            mNumElemBytes = 1;
        }
        mSize.set(mTexture->GetWidth(), mTexture->GetHeight());
    }

	void DemiTexture::loadFromFile(const std::string& _filename)
	{
        loadFromDemiTexture(_filename);
	}

	void DemiTexture::destroy()
	{
		if (mRenderTarget != nullptr)
		{
			delete mRenderTarget;
			mRenderTarget = nullptr;
		}

        mTexture.reset();
	}

	int DemiTexture::getWidth()
	{
		return mSize.width;
	}

	int DemiTexture::getHeight()
	{
		return mSize.height;
	}

	void* DemiTexture::lock(TextureUsage _access)
	{
        DiLockFlag lockFlag = (_access == TextureUsage::Write) ? LOCK_DISCARD : LOCK_READ_ONLY;
		mLock = true;
        return mTexture->GetTextureDriver()->LockLevel(0,0,lockFlag);
	}

	void DemiTexture::unlock()
	{
        mTexture->GetTextureDriver()->UnlockLevel(0);
		mLock = false;
	}

	bool DemiTexture::isLocked()
	{
		return mLock;
	}

	PixelFormat DemiTexture::getFormat()
	{
		return mPixelFormat;
	}

	size_t DemiTexture::getNumElemBytes()
	{
		return mNumElemBytes;
	}

	TextureUsage DemiTexture::getUsage()
	{
		return mTextureUsage;
	}

	IRenderTarget* DemiTexture::getRenderTarget()
	{
		if (!mTexture)
			return nullptr;

        if(mTexture->GetUsage() & TU_RENDER_TARGET)
        {
            if (!mRenderTarget)
                mRenderTarget = new DemiRTTexture(mTexture->GetRenderTarget());
        }
        return nullptr;
	}
} // namespace MyGUI
