
/********************************************************************
    File:       D3D9StateCache.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "StateCache.h"

#define D3D9_RENDERSTATE_COUNT  ( D3DRS_BLENDOPALPHA + 1 )
#define D3D9_TEXTURESTATE_COUNT ( D3DTSS_CONSTANT + 1 )
#define D3D9_SAMPLERSTATE_COUNT ( D3DSAMP_DMAPOFFSET + 1 )
#define D3D9_TEXTURE_COUNT      16
#define D3D9_VERTEXSTREAM_COUNT 4
#define D3D9_MAX_SAMPLERS       16

namespace Demi
{
    typedef DiStateCache<DWORD, D3D9_RENDERSTATE_COUNT>                     D3D9RSCache;
    typedef DiStateCache<DWORD, D3D9_TEXTURESTATE_COUNT>                    D3D9TSSCache;
    typedef DiStateCache<DWORD, D3D9_SAMPLERSTATE_COUNT>                    D3D9SSCache;
    typedef DiStateCache<IDirect3DBaseTexture9*, D3D9_TEXTURE_COUNT>        D3D9TextureCache;
    typedef DiStateCache<IDirect3DVertexBuffer9*, D3D9_VERTEXSTREAM_COUNT>  D3D9StreamCache;
    typedef DiStateCache<DWORD, D3D9_VERTEXSTREAM_COUNT>                   D3D9StreamFreqCache;

    class DI_D3D9DRV_API DiD3D9StateCache
    {
    public:

        DiD3D9StateCache(LPDIRECT3DDEVICE9 device);

        ~DiD3D9StateCache();

    public:

        void                    DirtyAll();

    public:

        HRESULT                 SetRenderState(D3DRENDERSTATETYPE rs, DWORD val);

        HRESULT                 SetSamplerState(DWORD stage, D3DSAMPLERSTATETYPE ss, DWORD val);

        HRESULT                 SetTextureStageState(DWORD stage, D3DTEXTURESTAGESTATETYPE tss, DWORD val);

        HRESULT                 SetTexture(DWORD stage, IDirect3DBaseTexture9* tex);

        HRESULT                 SetVertexShader(IDirect3DVertexShader9* pShader);

        HRESULT                 SetPixelShader(IDirect3DPixelShader9* pShader);

        HRESULT                 SetVertexDeclaration(LPDIRECT3DVERTEXDECLARATION9 pVertDecl);

        HRESULT                 SetStreamSource(uint32 StreamNumber, 
                                    IDirect3DVertexBuffer9* pStreamData, uint32 OffsetInBytes, uint32 Stride);

        HRESULT                 SetIndices(IDirect3DIndexBuffer9* pIndexData);

        HRESULT                 SetStreamSourceFreq(DWORD index, DWORD freq);

    private:

        LPDIRECT3DDEVICE9               mDevice;

        D3D9RSCache                     mRenderStates;

        D3D9SSCache                     mSamplerStates[D3D9_TEXTURE_COUNT];

        D3D9TSSCache                    mTextureStates[D3D9_TEXTURE_COUNT];

        D3D9TextureCache                mTextures;

        D3D9StreamCache                 mStreams;

        D3D9StreamFreqCache             mStreamFreqs;

        IDirect3DVertexShader9*         mVertexShader;

        IDirect3DPixelShader9*          mPixelShader;

        LPDIRECT3DVERTEXDECLARATION9    mVertDecl;

        IDirect3DIndexBuffer9*          mIndexBuffer;
    };
}