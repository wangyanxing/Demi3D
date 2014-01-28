
#include "DrvD3D9Pch.h"
#include "D3D9StateCache.h"

namespace Demi
{
    DiD3D9StateCache::DiD3D9StateCache(LPDIRECT3DDEVICE9 device)
        :mDevice(device),
        mVertexShader(nullptr),
        mPixelShader(nullptr),
        mVertDecl(nullptr),
        mIndexBuffer(nullptr)
    {
        DirtyAll();
    }

    DiD3D9StateCache::~DiD3D9StateCache()
    {

    }

    void DiD3D9StateCache::DirtyAll()
    {
        mRenderStates.ClearCache();
        
        for (int i = 0; i < D3D9_TEXTURE_COUNT; ++i)
        {
            mSamplerStates[i].ClearCache();
            mTextureStates[i].ClearCache();
        }

        mTextures.ClearCache();
        mStreams.ClearCache();
        mStreamFreqs.ClearCache();

        mVertexShader = nullptr;
        mPixelShader = nullptr;
        mVertDecl = nullptr;
        mIndexBuffer = nullptr;
    }

    HRESULT DiD3D9StateCache::SetRenderState(D3DRENDERSTATETYPE rs, DWORD val)
    {
        if (mRenderStates.SetCache((int)rs, val))
            return mDevice->SetRenderState(rs, val);
        return S_OK;
    }

    HRESULT DiD3D9StateCache::SetSamplerState(DWORD dwStage, D3DSAMPLERSTATETYPE d3dSamplerState, DWORD dwValue)
    {
        DI_ASSERT(dwStage < D3D9_MAX_SAMPLERS);
        if (mSamplerStates[dwStage].SetCache((int)d3dSamplerState, dwValue))
            return mDevice->SetSamplerState(dwStage, d3dSamplerState, dwValue);
        return S_OK;
    }

    HRESULT DiD3D9StateCache::SetTextureStageState(DWORD dwStage, D3DTEXTURESTAGESTATETYPE d3dTextureStageState, DWORD dwValue)
    {
        DI_ASSERT(dwStage < D3D9_MAX_SAMPLERS);
        if (mTextureStates[dwStage].SetCache((int)d3dTextureStageState, dwValue))
            return mDevice->SetTextureStageState(dwStage, d3dTextureStageState, dwValue);
        return S_OK;
    }

    HRESULT DiD3D9StateCache::SetTexture(DWORD stage, IDirect3DBaseTexture9* tex)
    {
        if (mTextures.SetCache(int(stage), tex))
            return mDevice->SetTexture(stage, tex);
        return S_OK;
    }

    HRESULT DiD3D9StateCache::SetVertexShader(IDirect3DVertexShader9* shader)
    {
        if (mVertexShader != shader)
        {
            mVertexShader = shader;
            return mDevice->SetVertexShader(shader);
        }
        return S_OK;
    }

    HRESULT DiD3D9StateCache::SetPixelShader(IDirect3DPixelShader9* shader)
    {
        if (mPixelShader != shader)
        {
            mPixelShader = shader;
            return mDevice->SetPixelShader(shader);
        }
        return S_OK;
    }

    HRESULT DiD3D9StateCache::SetVertexDeclaration(LPDIRECT3DVERTEXDECLARATION9 pVertDecl)
    {
        if (pVertDecl != mVertDecl)
        {
            HRESULT hr = mDevice->SetVertexDeclaration(pVertDecl);
            mVertDecl = pVertDecl;
            return hr;
        }
        return S_OK;
    }

    HRESULT DiD3D9StateCache::SetStreamSource(uint32 StreamNumber, 
        IDirect3DVertexBuffer9* pStreamData, uint32 OffsetInBytes, uint32 Stride)
    {
        DI_ASSERT(StreamNumber < D3D9_VERTEXSTREAM_COUNT);
        if (mStreams.SetCache((int)StreamNumber, pStreamData))
        {
            HRESULT hr = mDevice->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
            return hr;
        }
        return S_OK;
    }

    HRESULT DiD3D9StateCache::SetIndices(IDirect3DIndexBuffer9* pIndexData)
    {
        if (mIndexBuffer != pIndexData)
        {
            HRESULT hr = mDevice->SetIndices(pIndexData);
            mIndexBuffer = pIndexData;
            return hr;
        }
        return S_OK;
    }

    HRESULT DiD3D9StateCache::SetStreamSourceFreq(DWORD index, DWORD freq)
    {
        if (mStreamFreqs.SetCache((int)index, freq))
            return mDevice->SetStreamSourceFreq(index, freq);
        return S_OK;
    }
}