
/********************************************************************
    File:       GfxDriver.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

namespace Demi
{
    enum DiGfxDriverType
    {
        DRV_DIRECT3D9,
        DRV_DIRECT3D11,
        DRV_OPENGL,
        DRV_NULL
    };

    struct DiGfxMode
    {
        bool            mFullscreen;
        uint16          mWidth;
        uint16          mHeight;
        DiGfxDriverType mDriverType;
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

    class DI_GFX_API DiGfxDriver : public DiBase
    {
    public:

        DiGfxDriver();

        virtual                     ~DiGfxDriver();

    public:

        virtual DiGfxDriverType     GetDriverType() const = 0;

        uint16                      GetWidth() const { return mWidth; }

        uint16                      GetHeight() const { return mHeight; }

        bool                        IsWindowed() const { return !mFullScreen; }

        virtual void                BeginFrame() = 0;
        
        virtual void                EndFrame() = 0;

        virtual bool                Init(DiWndHandle wnd);

        virtual bool                Init(uint32 w, uint32 h, const char* title, bool fullscreen = false);

        virtual void                Shutdown(bool destroyWindows = true);

        virtual void                Render(DiRenderWindow* render = nullptr);

        virtual void                ConvertProjectionMatrix(const DiMat4& matrix, DiMat4& dest) = 0;

        virtual void                SetShaderConsts(DiShaderType type, int regID, const DiMat4& mat) = 0;

        virtual void                SetShaderConsts(DiShaderType type, int regID, const DiVec2& vec) = 0;

        virtual void                SetShaderConsts(DiShaderType type, int regID, const DiVec3& vec) = 0;

        virtual void                SetShaderConsts(DiShaderType type, int regID, const DiVec4& vec) = 0;

        virtual void                SetShaderConsts(DiShaderType type, int regID, float val) = 0;

        virtual void                SetShaderConsts(DiShaderType type, int regID, int val) = 0;

        virtual void                SetShaderConsts(DiShaderType type, int regID, const DiColor& col) = 0;

        virtual void                BindShaders(DiShaderProgram* vs, DiShaderProgram* ps) = 0;

        virtual void                Clear(uint32 flag, const DiColor& col, float depth, unsigned short stencil = 0) = 0;

        virtual bool                IsDeviceLost() const = 0;

        virtual bool                ResetDevice(uint16 w, uint16 h) = 0;

        virtual void                SetViewport(int x, int y, int w, int h, float minz = 0.0f, float maxz = 1.0f) = 0;

        virtual void                SetFillMode(DiFillMode mode) = 0;

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

        virtual const DiString&     GetShaderFileExtension() const = 0;

        virtual void                BindMaterialStates(const DiMaterial* mat) = 0;

        virtual void                GetWindowDimension(DiWndHandle wnd, uint32& w, uint32& h) = 0;

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

        void                        AddGpuResource(DiDeviceLostListener* res);

        void                        RemoveGpuResource(DiDeviceLostListener* res);

        DiRenderPipeline*           GetPipeline() const { return mPipeline; }

        DiSceneManager*             GetSceneManager();

        DiShaderEnvironment*        GetShaderEnvironment() const;

        float                       GetDeltaSecond() { return mDeltaTime; }
                                    
        float                       GetElapsedSecond(){ return mElapsedTime; }
                                    
        unsigned long               GetFrameNum() const { return mFrameNum; }

        void                        MarkClosing() { mClosing = true; }

        virtual void                SetColorBufferWriteEnabled(bool r, bool g, bool b, bool a) = 0;

    public:

        typedef DiVector<DiDepthBuffer*>            DepthBufferVec;
        typedef DiMap<uint16, DepthBufferVec>       DepthBufferMap;
        typedef DiVector<DiDeviceLostListener*>     GpuResources;
        typedef DiMap<DiWndHandle, DiRenderWindow*> RenderWindows;

        uint16                      mWidth;           

        uint16                      mHeight;

        bool                        mFullScreen;

        bool                        mDeviceLost;

    protected:

        // Init the graphics driver with a existed window handle
        virtual bool                InitGfx(DiWndHandle wnd) = 0;

        // Init the graphcis driver and create a new window
        virtual bool                InitGfx(uint16 width, uint16 height, bool fullscreen) = 0;

        virtual void                ReleaseGfx() = 0;

        void                        OnDeviceLost();

        void                        OnDeviceReset();

    protected:

        DepthBufferMap              mDepthBufferPool;

        DiRenderPipeline*           mPipeline;

        DiShaderManager*            mShaderManager;

        GpuResources                mGpuResources;

        RenderWindows               mRenderWindows;

        DiWindow*                   mMainWindow;

        DiCommandManager*           mCommandMgr;

        DiTimer                     mTimer;

        float                       mDeltaTime;

        float                       mElapsedTime;

        unsigned long               mFrameNum;

        bool                        mClosing;
    };
}