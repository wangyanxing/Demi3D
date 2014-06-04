/*!
	@file
	@author		Albert Semenov
	@date		12/2009
*/

#ifndef __MYGUI_DIRECTX_RTTEXTURE_H__
#define __MYGUI_DIRECTX_RTTEXTURE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IRenderTarget.h"

namespace MyGUI
{

	class DemiRTTexture : public IRenderTarget
	{
	public:
		DemiRTTexture(DiRenderTarget* rt);
		virtual ~DemiRTTexture();

		virtual void begin();
		virtual void end();

        virtual void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count, bool renderLineList = false);

		virtual const RenderTargetInfo& getInfo()
		{
			return mRenderTargetInfo;
		}

	private:
        DiRenderTarget* renderTarget;
		RenderTargetInfo mRenderTargetInfo;
	};

} // namespace MyGUI

#endif // __MYGUI_DIRECTX_RTTEXTURE_H__
