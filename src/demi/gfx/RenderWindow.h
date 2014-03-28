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

#ifndef DiRenderWindow_h__
#define DiRenderWindow_h__


#include "Texture.h"

namespace Demi 
{
    class DI_GFX_API DiRenderWindow : public DiBase
    {
    public:

        DiRenderWindow(const DiString& name);

        virtual                 ~DiRenderWindow();

    public:

        static DiRenderWindow*  ActiveWindow;

        void                    Create(DiWndHandle hwnd);

        void                    Create(uint32 width, uint32 height, const DiString& title, bool fullscreen);

        void                    Render();

        void                    SwapBuffer();

        void                    AddUpdateRenderTarget(DiRenderTarget* rt);

        void                    RemoveUpdateRenderTarget(DiRenderTarget* rt);

        DiSceneManager*         GetSceneManager() { return mSceneManager; }

        DiPostEffectManager*    GetPostEffectManager() {return mPostEffectMgr;}

        DiRenderTarget*         GetRenderBuffer() {return mRenderBuffer;}
        
        DiRenderTarget*         GetSceneCanvas() {return mSceneCanvas;}

        DiGBuffer*              GetGBuffer() const {return mGBuffer;}

        DiTexturePtr            GetCanvasTexture() const { return mCanvasTexture; }

        void                    OnDeviceLost();

        void                    OnDeviceReset();

        uint32                  GetWidth();

        uint32                  GetHeight();

        bool                    GetEnable() const { return mEnable; }

        void                    SetEnable(bool val) { mEnable = val; }

        void                    SetCurrentBatchesNum( uint32 val ) {mCurrentBatchesNum = val;}

        void                    SetCurrentPrimitivesNum( uint32 val ){mCurrentPrimitivesNum = val;}

        uint32                  GetCurrentBatchesNum() const { return mCurrentBatchesNum; }

        uint32                  GetCurrentPrimitivesNum() const { return mCurrentPrimitivesNum; }
        
        uint64                  GetFrameNumber() const { return mFrameNumber; }

        const DiString&         GetName(){return mName;}

        void                    Closing();

        void                    SetUpdateCallback(std::function<void(void)> val) { mUpdateCb = val; }

    protected:

        void                    Init();

        void                    Shutdown();

    protected:

        DiSceneManager*         mSceneManager;
        
        DiRenderTarget*         mSceneCanvas;

        DiRenderTarget*         mRenderBuffer;

        DiCamera*               mMainCamera;

        DiPostEffectManager*    mPostEffectMgr;

        DiTexturePtr            mCanvasTexture;

        typedef DiVector<DiRenderTarget*> UpdateRTs;

        UpdateRTs               mUpdateRTs;

        DiGBuffer*              mGBuffer;

        DiString                mName;

        bool                    mEnable;

        uint32                  mCurrentBatchesNum;

        uint32                  mCurrentPrimitivesNum;
        
        uint32                  mWidth;

        uint32                  mHeight;
        
        uint64                  mFrameNumber;

        DiWndHandle             mWndHandle;

        DiWindow*               mWindow;

        std::function<void(void)> mUpdateCb;
        
    };
}


#endif
