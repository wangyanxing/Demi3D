
/********************************************************************
    File:       DepthBuffer.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

namespace Demi
{
    class DI_GFX_API DiDepthBuffer : public DiBase
    {
    public:

        enum PoolId
        {
            POOL_NO_DEPTH        = 0,
            POOL_MANUAL_USAGE    = 0,
            POOL_DEFAULT        = 1
        };

        DiDepthBuffer();

        virtual             ~DiDepthBuffer();

        virtual void        Init(uint16 poolId, uint32 width, uint32 height,
                                void* depthBufferSurf, DiPixelFormat fmt,
                                uint32 fsaa, uint32 multiSampleQuality, bool manual) = 0;

        void                SetPoolId( uint16 poolId );

        uint16              GetPoolId() const;

        uint16              GetBitDepth() const;
        
        uint32              GetWidth() const;
        
        uint32              GetHeight() const;

        bool                IsManual() const;

        void                NotifyRenderTargetAttached( DiRenderTarget *renderTarget );

        void                NotifyRenderTargetDetached( DiRenderTarget *renderTarget );

        uint32              GetMSAAQuality(){ return mMultiSampleQuality; }

    protected:
        typedef DiSet<DiRenderTarget*> RenderTargetSet;

        void                DetachFromAllRenderTargets();

        uint16              mPoolId;
        
        uint16              mBitDepth;
        
        uint32              mWidth;
        
        uint32              mHeight;

        bool                mManual;

        RenderTargetSet     mAttachedRenderTargets;

        uint32              mMultiSampleQuality;
    };
}