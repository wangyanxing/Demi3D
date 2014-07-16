/**********************************************************************
This source file is a part of Demi3D
   __  ___  __  __  __
  |  \|_ |\/||   _)|  \ 
  |__/|__|  ||  __)|__/ 

Copyright (c) 2013-2014 Demi team
https://github.com/wangyanxing/Demi3D

Released under the MIT License
https://github.com/wangyanxing/Demi3D/blob/master/License.txt
***********************************************************************/

#ifndef DiRenderTarget_h__
#define DiRenderTarget_h__


#include "DeviceLostListener.h"
#include "Texture.h"

namespace Demi 
{
    /** A simple wrapper of the viewport
     */
    class DI_GFX_API DiViewport
    {
    public:

        DiViewport(DiRenderTarget* rt)
        :mParent(rt)
        {}

        float mLeft{ 0 };
              
        float mTop{ 0 };
              
        float mWidth{ 1 };
              
        float mHeight{ 1 };
        
        DiRenderTarget* mParent{ nullptr };
    };

    /** Render target class, created from the texture
     */
    class DI_GFX_API DiRenderTarget : public DiDeviceLostListener
    {
    public:

        DiRenderTarget();

        virtual                 ~DiRenderTarget();

    public:

        virtual void            AttachSurface() = 0;

        virtual void            DetachSurface() = 0;

        void                    SetViewport(float left = 0.0f, float top = 0.0f,
                                            float width = 1.0f, float height = 1.0f)
        {
            mViewport.mLeft   = left;
            mViewport.mTop    = top;
            mViewport.mWidth  = width;
            mViewport.mHeight = height;
        }

        DiViewport&             GetViewport() { return mViewport; }

        uint32                  GetWidth(void) const { return mWidth; }
                                
        uint32                  GetHeight(void) const { return mHeight; }

        void                    AttachTexture(DiTexturePtr texture);

        void                    DetachTexture();

        virtual void            OnDeviceLost();

        virtual void            OnDeviceReset() {}

        virtual void            PreBind() = 0;

        virtual void            Bind(uint8 mrtID = 0);

        void                    SetDepthBufferPool( uint16 poolId );

        uint16                  GetDepthBufferPool() const;

        DiDepthBuffer*          GetDepthBuffer() const;

        virtual bool            AttachDepthBuffer( DiDepthBuffer *depthBuffer );

        virtual void            DetachDepthBuffer();

        virtual void            ClearDepthBuffer();

        virtual bool            BindRenderTarget(uint8 mrtid) = 0;

        virtual bool            BindDepthStencil() = 0;

        virtual DiDepthBuffer*  CreateDepthBuffer() = 0;

        virtual bool            IsCompatibleWith(DiDepthBuffer* db);

        bool                    GetClearPerFrame() const { return mClearPerFrame; }

        void                    SetClearPerFrame(bool val) { mClearPerFrame = val; }

        DiColor                 GetClearColor() const { return mClearColor; }

        void                    SetClearColor(const DiColor& val) { mClearColor = val; }

        float                   GetDepthClearValue() const { return mDepthClearValue; }

        void                    SetDepthClearValue(float val) { mDepthClearValue = val; }

        uint32                  GetClearFlag() const { return mClearFlag; }

        void                    SetClearFlag(uint32 val) { mClearFlag = val; }

        virtual bool            SwapBuffer() { return true; }

        bool                    IsActive() const { return mIsActive; }

        void                    SetActive(bool val) { mIsActive = val; }

        bool                    IsFlippingUV() const { return mFlippingUV; }

        void                    SetFlippingUV(bool val) { mFlippingUV = val; }

        DiTexturePtr            GetParentTexture() { return mParentTex; }

    protected:

        uint32                  mWidth;

        uint32                  mHeight;
        
        DiViewport              mViewport;

        DiTexturePtr            mParentTex;
        
        uint16                  mDepthBufferPoolId;

        DiDepthBuffer*          mDepthBuffer;

        DiColor                 mClearColor;

        float                   mDepthClearValue;

        bool                    mClearPerFrame;

        uint32                  mClearFlag;

        bool                    mIsActive;

        bool                    mFlippingUV;
    };
}

#endif
