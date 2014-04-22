/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#include "MyGUI_Precompiled.h"
#include "MyGUI_DemiVertexBuffer.h"
#include "MyGUI_VertexData.h"
#include "MyGUI_DemiDiagnostic.h"

#include "GfxDriver.h"
#include "VertexBuffer.h"

namespace MyGUI
{

	const size_t VERTEX_IN_QUAD = 6;
	const size_t RENDER_ITEM_STEEP_REALLOCK = 5 * VERTEX_IN_QUAD;

	DemiVertexBuffer::DemiVertexBuffer(DemiRenderManager* _pRenderManager) :
		mNeedVertexCount(0),
		mVertexCount(RENDER_ITEM_STEEP_REALLOCK),
		pRenderManager(_pRenderManager),
        mVertexBuffer(nullptr)
	{
	}

	DemiVertexBuffer::~DemiVertexBuffer()
	{
		destroy();
	}
    
	void DemiVertexBuffer::setVertexCount(size_t _count)
	{
		if (_count != mNeedVertexCount)
		{
			mNeedVertexCount = _count;
			resize();
		}
	}

	size_t DemiVertexBuffer::getVertexCount()
	{
		return mNeedVertexCount;
	}

	Vertex* DemiVertexBuffer::lock()
	{
		void* lockPtr = mVertexBuffer->Lock(0,mVertexBuffer->GetBufferSize());
		return (Vertex*)lockPtr;
	}

	void DemiVertexBuffer::unlock()
	{
        mVertexBuffer->Unlock();
	}

	bool DemiVertexBuffer::create()
	{
		uint32 length = (uint32)(mNeedVertexCount * sizeof(MyGUI::Vertex));
        mVertexBuffer = DiBase::Driver->CreateVertexBuffer();
        mVertexBuffer->Release();
        mVertexBuffer->SetStride(sizeof(MyGUI::Vertex));
        mVertexBuffer->Create(length);
        return true;
	}

	void DemiVertexBuffer::destroy()
	{
		if (mVertexBuffer)
		{
            mVertexBuffer->Release();
            delete mVertexBuffer;
            mVertexBuffer = nullptr;
		}
	}

	void DemiVertexBuffer::resize()
	{
        destroy();
        create();
	}

} // namespace MyGUI
