/*!
	@file
	@author		Albert Semenov
	@date		12/2009
*/

#include "MyGUI_Precompiled.h"
#include "MyGUI_DemiRTTexture.h"
#include "MyGUI_DemiRenderManager.h"

#include "RenderTarget.h"
#include "GfxDriver.h"

namespace MyGUI
{

	DemiRTTexture::DemiRTTexture(DiRenderTarget* _renderTarget) :
        renderTarget(_renderTarget)
	{
		int width = renderTarget->GetWidth();
		int height = renderTarget->GetHeight();

        auto drvType = DiBase::Driver->GetDriverType();
        mRenderTargetInfo.maximumDepth = (drvType == DRV_DIRECT3D9 || drvType == DRV_DIRECT3D11) ? 0 : 1;
        if (drvType == DRV_DIRECT3D9)
        {
            mRenderTargetInfo.hOffset = -0.5f / float(width);
            mRenderTargetInfo.vOffset = -0.5f / float(height);
        }
        else
        {
            mRenderTargetInfo.hOffset = 0;
            mRenderTargetInfo.vOffset = 0;
        }
		mRenderTargetInfo.aspectCoef = float(height) / float(width);
		mRenderTargetInfo.pixScaleX = 1.0f / float(width);
		mRenderTargetInfo.pixScaleY = 1.0f / float(height);
	}

	DemiRTTexture::~DemiRTTexture()
	{
	}

	void DemiRTTexture::begin()
	{
        renderTarget->Bind();
	}

	void DemiRTTexture::end()
	{
	}

    void DemiRTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count, bool renderLineList)
	{
        DemiRenderManager::getInstance().doRender(_buffer, _texture, _count, renderLineList);
	}

} // namespace MyGUI
