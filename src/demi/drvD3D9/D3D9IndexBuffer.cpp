
#include "D3D9IndexBuffer.h"
#include "D3D9Driver.h"
#include "D3D9StateCache.h"
#include "D3D9TypeMappings.h"

namespace Demi
{
    DiD3D9IndexBuffer::DiD3D9IndexBuffer()
        :mIndexBuffer(nullptr)
    {

    }

    DiD3D9IndexBuffer::~DiD3D9IndexBuffer()
    {
        Release();
    }

    void DiD3D9IndexBuffer::Create(uint32 size, DiIndexBufType type, uint32 usage)
    {
        if (mIndexBuffer)
            Release();

        mResUsage = (DiResUsage)usage;
        mFormat = type;
        D3DFORMAT idFmt = (type == IB_16BITS) ? D3DFMT_INDEX16 : D3DFMT_INDEX32;

        mUsage = 0;
        mPool = D3DPOOL_MANAGED;
        if (usage & RU_DYNAMIC)
        {
            mUsage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
            mPool = D3DPOOL_DEFAULT;
        }

        mBufferSize = size;
        if (mFormat == D3DFMT_INDEX16)
            mMaxIndices = mBufferSize / sizeof(uint16);
        else
            mMaxIndices = mBufferSize / sizeof(uint32);

        mIndexBuffer = DiD3D9Driver::CreateIndexBuffer(mBufferSize, mUsage, idFmt, mPool);
    }

    void DiD3D9IndexBuffer::Bind()
    {
        DiD3D9Driver::StateCache->SetIndices(mIndexBuffer);
    }

    void DiD3D9IndexBuffer::Release()
    {
        if (mIndexBuffer)
        {
            mIndexBuffer->Release();
            mIndexBuffer = nullptr;
        }
    }

    void DiD3D9IndexBuffer::OnDeviceLost()
    {
        if (mPool != D3DPOOL_MANAGED && mIndexBuffer)
        {
            mIndexBuffer->Release();
            mIndexBuffer = NULL;
            mDeviceLost = true;
        }
    }

    void DiD3D9IndexBuffer::OnDeviceReset()
    {
        if (mDeviceLost)
        {
            D3DFORMAT idFmt = (mFormat == IB_16BITS) ? D3DFMT_INDEX16 : D3DFMT_INDEX32;
            mIndexBuffer = DiD3D9Driver::CreateIndexBuffer(
                mBufferSize, mUsage, idFmt, mPool);
            mDeviceLost = false;
        }
    }

    void* DiD3D9IndexBuffer::Lock(uint32 offset, uint32 size, DiLockFlag flag)
    {
        void* lk = nullptr;
        DWORD lock = DiD3D9Mappings::GetLockOption(flag, mResUsage);
        if (mIndexBuffer)
        {
            HRESULT res = mIndexBuffer->Lock(offset, size, &lk, lock);
            DX9_CHKERR(res);
        }
        return lk;
    }

    void DiD3D9IndexBuffer::Unlock()
    {
        if (mIndexBuffer)
            mIndexBuffer->Unlock();
    }
}