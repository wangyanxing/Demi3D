
/********************************************************************
    File:       Texture2D.h
    Creator:    demiwangya
*********************************************************************/

#ifndef DiTexture_h__
#define DiTexture_h__

#include "Asset.h"
#include "DeviceLostListener.h"

namespace Demi 
{
    enum DiTextureType
    {
        TEXTURE_2D,
        TEXTURE_CUBE
    };

    class DiTextureDrv : public DiBase
    {
    public:
        
        DiTextureDrv(){}
        
        virtual         ~DiTextureDrv(){}

        virtual void    CreateTexture() = 0;

        virtual void    Release() = 0;

        virtual void*   LockLevel(uint32 level, uint32 surface = 0, DiLockFlag lockflag = LOCK_NORMAL) = 0;

        virtual void    UnlockLevel(uint32 level, uint32 surface = 0) = 0;

        virtual void    Bind(uint32 samplerIndex) = 0;
        
        virtual void    CopyToMemory(const DiBox &srcBox, const DiPixelBox &dst) = 0;

        void            CopyFromMemory(const DiPixelBox &srcBox, uint32 level, uint32 surface = 0);

        virtual void    CopyFromMemory(const DiPixelBox &srcBox, const DiBox &dst, uint32 level, uint32 surface = 0) = 0;

        virtual void*   GetSurfaceHandle() = 0;

        DiTexture*      mParent;
    };

    /////////////////////////////////////////////////////////////////////////////////////////

    typedef shared_ptr<DiTexture> DiTexturePtr;
    class DI_GFX_API DiTexture : public DiAsset, 
                                 public DiDeviceLostListener, 
                                 public enable_shared_from_this<DiTexture>
    {
    public:

        DiTexture(const DiString& name);

        virtual                 ~DiTexture();

    public:

        void                    CreateTexture();

        void                    Release();

        void                    SetTextureType(DiTextureType type){mTextureType = type;}

        inline DiTextureType    GetTextureType(){ return mTextureType; }

        inline DiFilterType     GetFilter(void) const { return mFilter; }

        void                    SetFilter(DiFilterType ft);

        void                    SetAddressingU(DiAddMode ad);

        void                    SetAddressingV(DiAddMode ad);

        void                    SetAddressing(DiAddMode ad);

        inline DiAddMode        GetAddressingU(void) const { return mAddressingU; }

        inline DiAddMode        GetAddressingV(void) const { return mAddressingV; }

        inline DiColor          GetBorderColor(void) const { return mBorderColor; }

        void                    SetBorderColor(DiColor val) { mBorderColor = val; }

        void                    SetFormat(DiPixelFormat fmt);

        DiPixelFormat           GetFormat(void) const { return mPixelFormat; }
        
        bool                    IsAutoMipmap(void) const { return mAutoGenerateMipmap; }
        
        void                    SetAutoMipmap( bool automip ) { mAutoGenerateMipmap = automip; }

        uint32                  GetWidth(void) const { return mWidth; }

        uint32                  GetHeight(void) const { return mHeight; }

        void                    SetDimensions(uint32 width, uint32 height);

        uint32                  GetNumLevels(void) const { return mNumLevels; }

        void                    SetNumLevels(uint32 numlevel);

        uint32                  GetWidthInBlocks(void) const;

        uint32                  GetHeightInBlocks(void) const;

        uint32                  GetBlockSize(void) const;

        DiTextureUsage          GetUsage(void) const { return mUsage; }

        void                    SetUsage(DiTextureUsage val) { mUsage = val; }

        DiResUsage              GetResourceUsage(void) const { return mResUsage; }

        void                    SetResourceUsage(DiResUsage val) { mResUsage = val; }

        void                    OnDeviceLost(void);

        void                    OnDeviceReset(void);

        void*                   LockLevel(uint32 level, uint32 surface=0);

        void                    UnlockLevel(uint32 level, uint32 surface=0);

        void                    Bind(uint32 samplerIndex);

        bool                    LoadingComplete(void) const;

        DiAssetType             GetAssetType(void) const {return TYPE;}

        bool                    Load(DiDataStreamPtr data);

        bool                    Load(void);

        DiRenderTarget*         GetRenderTarget(void);

        uint16                  GetUvSet(void) const { return mUvSet; }

        void                    SetUvSet(uint16 val) { mUvSet = val; }

        bool                    IsAdaptedRT(void) const { return mAdaptedRT != nullptr; }

        void                    SetAdaptedRT(DiRenderTarget* val) { mAdaptedRT = val; }

        DiVec2                  GetViewportScale(void) const { return mViewportScale; }

        void                    SetViewportScale(DiVec2 val) { mViewportScale = val; }

        void                    CopyToMemory(const DiPixelBox &dst);

        void                    CopyToMemory(const DiBox &srcBox, const DiPixelBox &dst);

        void*                   GetSurfaceHandle(void);

        DiTextureDrv*           GetTextureDriver(void) { return mTextureDrv; }

    public:

        static DiAssetType      TYPE;

    protected:

        DiFilterType            mFilter;
                                
        DiAddMode               mAddressingU;
                                
        DiAddMode               mAddressingV;
                                
        DiColor                 mBorderColor;
                                
        DiPixelFormat           mPixelFormat;
                                
        uint32                  mWidth;
                                
        uint32                  mHeight;
                                
        uint32                  mNumLevels;
                                
        DiTextureUsage          mUsage;
                                
        DiResUsage              mResUsage;
                                
        DiRenderTarget*         mRenderTarget;
                                
        uint16                  mUvSet;
                                
        DiVec2                  mViewportScale;
                                
        DiRenderTarget*         mAdaptedRT;
                                
        DiTextureType           mTextureType;
                                
        DiTextureDrv*           mTextureDrv;
        
        bool                    mAutoGenerateMipmap;
    };
}
#endif // Texture_h__