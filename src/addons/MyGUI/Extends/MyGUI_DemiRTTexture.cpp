/*!
	@file
	@author		Albert Semenov
	@date		12/2009
*/

#include "MyGUI_Precompiled.h"
#include "MyGUI_DemiRTTexture.h"
#include "MyGUI_DemiRenderManager.h"

#include "RenderTarget.h"

namespace MyGUI
{

	DemiRTTexture::DemiRTTexture(DiRenderTarget* _renderTarget) :
        renderTarget(_renderTarget)
	{
		int width = renderTarget->GetWidth();
		int height = renderTarget->GetHeight();

		mRenderTargetInfo.maximumDepth = 0.0f;
		mRenderTargetInfo.hOffset = -0.5f / float(width);
		mRenderTargetInfo.vOffset = -0.5f / float(height);
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

	void DemiRTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		DemiRenderManager::getInstance().doRender(_buffer, _texture, _count);
	}

} // namespace MyGUI
