/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#ifndef __MYGUI_DIRECTX_VERTEX_BUFFER_H__
#define __MYGUI_DIRECTX_VERTEX_BUFFER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_DemiRenderManager.h"


namespace MyGUI
{
	class DemiVertexBuffer : public IVertexBuffer
	{
	public:
		DemiVertexBuffer(DemiRenderManager* _pRenderManager);
		virtual ~DemiVertexBuffer();

		virtual void setVertexCount(size_t _count);
		virtual size_t getVertexCount();

		virtual Vertex* lock();
		virtual void unlock();
        
        DiVertexBuffer* getVertexBuffer() {return mVertexBuffer;}

	private:
		bool create();
		void destroy();
		void resize();

	private:
        DiVertexBuffer* mVertexBuffer;

		size_t mNeedVertexCount;
	};

} // namespace MyGUI

#endif // __MYGUI_DIRECTX_VERTEX_BUFFER_H__
