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

#ifndef DiGfxDriver_h__
#define DiGfxDriver_h__

#include "Capabilities.h"

namespace Demi
{
    enum DiGfxDriverType
    {
        DRV_DIRECT3D9,
        DRV_DIRECT3D11,
        DRV_OPENGL,
        DRV_OPENGL_ES2,
        DRV_NULL
    };

    struct DiGfxAdapter
    {
    public:
        DiString    mDriver;
        int64       mDriverVersion;
        DiString    mDescription;
        DiString    mDeviceName;
        DiString    mVendor;
        uint32      mVendorId;
        uint32      mDeviceId;
        uint32      mSubSysId;
        uint32      mRevision;
    };

    /** Graphics driver class
        The abstract layer of the graphics APIs
     */
    class DI_GFX_API DiGfxDriver : public DiBase
    {
    public:

        DiGfxDriver();

        virtual                     ~DiGfxDriver();

    public:

        virtual DiGfxDriverType     GetDriverType() const = 0;

        bool                        IsWindowed() const { return !mFullScreen; }

        virtual void                BeginFrame() = 0;
        
        virtual void                EndFrame() = 0;

        virtual bool                Init(DiWndHandle wnd);

        virtual bool                Init(uint32 w, uint32 h, const DiString& title, bool fullscreen = false);

        virtual void                PostInit(){}

        virtual void                Shutdown(bool destroyWindows = true);

        virtual void                Render(DiRenderWindow* render = nullptr);

        virtual void                ConvertProjectionMatrix(const DiMat4& matrix, DiMat4& dest) = 0;

        virtual void                BindShaders(DiShaderProgram* vs, DiShaderProgram* ps) = 0;

        virtual void                Clear(uint32 flag, const DiColor& col, float depth, unsigned short stencil = 0) = 0;

        virtual bool                IsDeviceLost() const = 0;

        virtual bool                ResetDevice(uint16 w, uint16 h) = 0;

        virtual void                SetViewport(int x, int y, int w, int h, float minz = 0.0f, float maxz = 1.0f) = 0;

        virtual void                SetFillMode(DiFillMode mode) = 0;

        virtual void                SetDepthBias(float constantBias, float slopeScaledBias) = 0;

        virtual bool                RenderGeometry(DiRenderUnit* unit) = 0;

        virtual const DiVec2&       GetTexelOffset() const = 0;

        virtual DiTextureDrv*       CreateTextureDriver(DiTexture* texture) = 0;

        virtual DiIndexBuffer*      CreateIndexBuffer() = 0;

        virtual DiVertexBuffer*     CreateVertexBuffer() = 0;

        virtual DiVertexDeclaration*CreateVertexDeclaration() = 0;

        virtual DiShaderInstance*   CreateVSInstance(DiShaderProgram* prog) = 0;

        virtual DiShaderInstance*   CreatePSInstance(DiShaderProgram* prog) = 0;

        virtual DiRenderTarget*     CreateRenderTarget() = 0;

        virtual DiShaderParameter*  CreateShaderParams(DiMaterial& mat) = 0;

        virtual void                CreateWindowTarget(DiRenderTarget*& outRT, DiWndHandle wnd) = 0;

        /** .hlsl for Direct3D, .glsl for OpenGL(ES)
         */
        virtual const DiString&     GetShaderFileExtension() const = 0;

        virtual void                BindMaterialStates(const DiMaterial* mat) = 0;

        virtual void                GetWindowSize(DiWndHandle wnd, uint32& w, uint32& h);

        virtual DiWindow*           CreateWnd() = 0;

        void                        CleanUpDepthBuffers(bool bCleanManualBuffers = true);

        void                        SetDepthBufferFor(DiRenderTarget *renderTarget);

        DiRenderWindow*             FindRenderWindow(DiWndHandle hwnd);

        DiRenderWindow*             GetMainRenderWindow();

        DiRenderWindow*             CreateRenderWindow(const DiString& name, DiWndHandle hwnd);

        void                        DestroyRenderWindow(DiWndHandle hwnd);

        void                        DestroyRenderWindows();

        void                        SwapBuffer(DiRenderTarget* renderwnd);

        virtual void                NotifyDeviceLost();

        void                        AddDeviceLostListener(DiDeviceLostListener* res);

        void                        RemoveDeviceLostListener(DiDeviceLostListener* res);

        DiRenderPipeline*           GetPipeline() const { return mPipeline; }

        /** Get the scene manager from the main render window
         */
        DiSceneManager*             GetSceneManager();

        DiShaderEnvironment*        GetShaderEnvironment() const;

        double                      GetDeltaSecond() { return mDeltaTime; }
                                    
        double                      GetElapsedSecond(){ return mElapsedTime; }
                                    
        unsigned long               GetFrameNum() const { return mFrameNum; }

        void                        MarkClosing() { mClosing = true; }

        DiWindowManager*            GetWindowManager() const { return mWndManager; }

        virtual void                SetColorBufferWriteEnabled(bool r, bool g, bool b, bool a) = 0;

        bool                        NeedTextureFlipping() const { return mNeedTextureFlipping; }

        virtual void                SetTextureFlipping(bool val) { mNeedTextureFlipping = val; }

        /** For camera reflection, etc.
        */
        bool                        IsInvertVertexWinding() const { return mInvertVertexWinding; }

        void                        SetInvertVertexWinding(bool val) { mInvertVertexWinding = val; }

        virtual DiPair<float,float> GetDepthInputRange() const = 0;

        virtual DiGfxCaps*          InitGfxCaps() = 0;

        DiGfxCaps*                  GetGfxCaps() { return mCaps; }

    public:

        typedef DiVector<DiDepthBuffer*>            DepthBufferVec;
        typedef DiMap<uint16, DepthBufferVec>       DepthBufferMap;
        typedef DiVector<DiDeviceLostListener*>     GpuResources;
        typedef DiMap<DiWndHandle, DiRenderWindow*> RenderWindows;

        bool                        mFullScreen;

        bool                        mDeviceLost;

    protected:

        // Init the graphics driver with a existed window handle
        virtual bool                InitGfx(DiWndHandle wnd) = 0;

        // Init the graphics driver and create a new window
        virtual bool                InitGfx(uint16 width, uint16 height, bool fullscreen) = 0;

        virtual void                ReleaseGfx() = 0;

        void                        OnDeviceLost();

        void                        OnDeviceReset();

        void                        InitCommandVars();

    protected:

        DepthBufferMap              mDepthBufferPool;

        DiRenderPipeline*           mPipeline;

        DiShaderManager*            mShaderManager;

        GpuResources                mGpuResources;

        RenderWindows               mRenderWindows;

        DiWindow*                   mMainWindow;

        DiWindowManager*            mWndManager;
        
        DiCommandManager*           mCommandMgr;

        DiTimer                     mTimer;

        double                      mDeltaTime;

        double                      mElapsedTime;

        unsigned long               mFrameNum;

        bool                        mClosing;

        bool                        mNeedTextureFlipping;

        bool                        mInvertVertexWinding;

        DiGfxCaps*                  mCaps;

        DriverVersion               mDriverVersion;
    };
}
#endif // GfxDriver_h__