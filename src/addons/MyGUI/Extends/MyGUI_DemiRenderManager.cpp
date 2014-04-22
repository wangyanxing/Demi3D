/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#include "MyGUI_Precompiled.h"
#include "MyGUI_DemiRenderManager.h"
#include "MyGUI_DemiTexture.h"
#include "MyGUI_DemiVertexBuffer.h"
#include "MyGUI_DemiDiagnostic.h"
#include "MyGUI_Gui.h"
#include "MyGUI_Timer.h"

#include "GfxDriver.h"
#include "RenderUnit.h"
#include "Material.h"
#include "ShaderParam.h"
#include "VertexDeclaration.h"
#include "RenderWindow.h"

namespace MyGUI
{
	DemiRenderManager::DemiRenderManager() :
		mIsInitialise(false),
		mUpdate(false),
        mSharedUnit(nullptr)
	{
	}

	void DemiRenderManager::initialise()
	{
		MYGUI_PLATFORM_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		mVertexFormat = VertexColourType::ColourARGB;

		memset(&mInfo, 0, sizeof(mInfo));
        uint32 w = DiBase::Driver->GetMainRenderWindow()->GetWidth();
        uint32 h = DiBase::Driver->GetMainRenderWindow()->GetHeight();
        setViewSize(w,h);
        
        mSharedUnit = new DiRenderUnit();
        mSharedUnit->mSourceData.push_back(nullptr);
        mSharedUnit->mVertexDecl = DiBase::Driver->CreateVertexDeclaration();
        mSharedUnit->mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_POSITION);
        mSharedUnit->mVertexDecl->AddElement(0, VERT_TYPE_COLOR,  VERT_USAGE_COLOR);
        mSharedUnit->mVertexDecl->AddElement(0, VERT_TYPE_FLOAT2, VERT_USAGE_TEXCOORD);
        mSharedUnit->mVertexDecl->Create();
        
        mSharedMaterial = DiMaterial::QuickCreate("gui_v", "gui_p");
        mSharedMaterial->SetBlendMode(BLEND_ALPHA);
        mSharedMaterial->SetCullMode(CULL_NONE);
        mSharedMaterial->SetDepthCheck(false);
        mSharedMaterial->SetDepthWrite(false);

		mUpdate = false;

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void DemiRenderManager::shutdown()
	{
		MYGUI_PLATFORM_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		destroyAllResources();
        
        delete mSharedUnit;
        mSharedUnit = nullptr;

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	IVertexBuffer* DemiRenderManager::createVertexBuffer()
	{
		return new DemiVertexBuffer(this);
	}

	void DemiRenderManager::destroyVertexBuffer(IVertexBuffer* _buffer)
	{
		delete _buffer;
	}

	void DemiRenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		DemiTexture* tex = static_cast<DemiTexture*>(_texture);
        mSharedMaterial->GetShaderParameter()->WriteTexture2D("map", tex->getTexture());
//        mSharedMaterial->GetShaderParameter()->Bind();
        mSharedMaterial->Bind();
        
		DemiVertexBuffer* vb = static_cast<DemiVertexBuffer*>(_buffer);
        mSharedUnit->mSourceData[0] = vb->getVertexBuffer();
        mSharedUnit->mVerticesNum = (uint32)vb->getVertexCount();
        mSharedUnit->mPrimitiveCount = mSharedUnit->mVerticesNum / 3;
		
        DiBase::Driver->RenderGeometry(mSharedUnit);
	}

	void DemiRenderManager::drawOneFrame()
	{
		Gui* gui = Gui::getInstancePtr();
		if (gui == nullptr)
			return;

		static Timer timer;
		static unsigned long last_time = timer.getMilliseconds();
		unsigned long now_time = timer.getMilliseconds();
		unsigned long time = now_time - last_time;

		onFrameEvent((float)((double)(time) / (double)1000));

		last_time = now_time;

		begin();
		onRenderToTarget(this, mUpdate);
		end();

		mUpdate = false;
	}

	void DemiRenderManager::begin()
	{
        
	}

	void DemiRenderManager::end()
	{
	}

	ITexture* DemiRenderManager::createTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		MYGUI_PLATFORM_ASSERT(item == mTextures.end(), "Texture '" << _name << "' already exist");

		DemiTexture* texture = new DemiTexture(_name);
		mTextures[_name] = texture;
		return texture;
	}

	void DemiRenderManager::destroyTexture(ITexture* _texture)
	{
		if (_texture == nullptr)
			return;

		MapTexture::iterator item = mTextures.find(_texture->getName());
		MYGUI_PLATFORM_ASSERT(item != mTextures.end(), "Texture '" << _texture->getName() << "' not found");

		mTextures.erase(item);
		delete _texture;
	}

	ITexture* DemiRenderManager::getTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		if (item == mTextures.end())
			return nullptr;
		return item->second;
	}

	bool DemiRenderManager::isFormatSupported(PixelFormat _format, TextureUsage _usage)
	{
		return true;
	}

	void DemiRenderManager::destroyAllResources()
	{
		for (MapTexture::const_iterator item = mTextures.begin(); item != mTextures.end(); ++item)
		{
			delete item->second;
		}
		mTextures.clear();
	}

	void DemiRenderManager::setViewSize(int _width, int _height)
	{
		if (_height == 0)
			_height = 1;
		if (_width == 0)
			_width = 1;

		mViewSize.set(_width, _height);

        auto drvType = DiBase::Driver->GetDriverType();
        mInfo.maximumDepth = (drvType == DRV_DIRECT3D9 || drvType == DRV_DIRECT3D11) ? 0 : 1;
        mInfo.hOffset = 0;
        mInfo.vOffset = 0;
		mInfo.aspectCoef = float(mViewSize.height) / float(mViewSize.width);
		mInfo.pixScaleX = 1.0f / float(mViewSize.width);
		mInfo.pixScaleY = 1.0f / float(mViewSize.height);

		onResizeView(mViewSize);

		mUpdate = true;
	}

} // namespace MyGUI
