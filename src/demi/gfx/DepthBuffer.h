


#ifndef DiDepthBuffer_h__
#define DiDepthBuffer_h__


namespace Demi
{
    class DI_GFX_API DiDepthBuffer : public DiBase
    {
    public:

        enum PoolId
        {
            POOL_NO_DEPTH       = 0,
            POOL_MANUAL_USAGE   = 0,
            POOL_DEFAULT        = 1
        };

        DiDepthBuffer();

        virtual             ~DiDepthBuffer();

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

#endif
