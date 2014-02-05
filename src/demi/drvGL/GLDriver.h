
/********************************************************************
    File:       GLDriver.h
    Creator:    demiwangya
*********************************************************************/

#ifndef GLDriver_h__
#define GLDriver_h__

#include "GLPrerequisites.h"
#include "GfxDriver.h"

namespace Demi
{
    class DI_GLDRV_API DiGLDriver : public DiGfxDriver
    {
    public:

        DiGLDriver();

        virtual                 ~DiGLDriver();

    public:

        DiGfxDriverType         GetDriverType() const { return DRV_OPENGL; }

        bool                    InitGfx(DiWndHandle wnd);

        bool                    InitGfx(uint16 width, uint16 height, bool fullscreen);

        void                    ReleaseGfx();

        bool                    IsDeviceLost();

        void                    BeginFrame();

        void                    EndFrame();

        void                    ConvertProjectionMatrix(const DiMat4& matrix, DiMat4& dest);

        void                    BindShaders(DiShaderProgram* vs, DiShaderProgram* ps);

        void                    SetViewport(int x, int y, int w, int h, float minz = 0.0f, float maxz = 1.0f);

        void                    SetFillMode(DiFillMode mode);

        bool                    IsDeviceLost() const;

        bool                    ResetDevice(uint16 w, uint16 h);

        bool                    RenderGeometry(DiRenderUnit* unit);

        const DiVec2&           GetTexelOffset() const;

        DiTextureDrv*           CreateTextureDriver(DiTexture* texture);

        DiIndexBuffer*          CreateIndexBuffer();

        DiVertexBuffer*         CreateVertexBuffer();

        DiVertexDeclaration*    CreateVertexDeclaration();

        DiShaderInstance*       CreateVSInstance(DiShaderProgram* prog);

        DiShaderInstance*       CreatePSInstance(DiShaderProgram* prog);

        DiRenderTarget*         CreateRenderTarget();

        DiShaderParameter*      CreateShaderParams(DiMaterial& mat);

        const DiString&         GetShaderFileExtension() const;

        void                    SetColorBufferWriteEnabled(bool r, bool g, bool b, bool a);

        void                    GetDepthStencilFormatFor(GLenum internalColourFormat, GLenum *depthFormat,
                                    GLenum *stencilFormat);
        
        void                    CreateWindowTarget(DiRenderTarget*& outRT, DiWndHandle wnd);

        void                    BindMaterialStates(const DiMaterial* mat);

        void                    Clear(uint32 flag, const DiColor& col, float depth, unsigned short stencil = 0);

        // Create a new one if not existed
        DiGLShaderLinker*       GetShaderLinker(DiShaderInstance* vs, DiShaderInstance* ps);

        DiGLContext*            GetContext(DiWndHandle wnd);

        DiGLContext*            GetCurrentContext() { return mCurrentContext; }
        
        DiGLContext*            GetMainContext() { return mMainContext; }

        DiWindow*               CreateWnd();

        static DiString&        GetGlslVersion();

        static DiGLBufferManager* BufferMgr;

        static DiGLFBOManager*    FBOManager;

    private:

        DiGLContext*            _CreateContext(DiWndHandle wnd);

        DiGLUtil*               _CreateGLUtil();

        bool                    _BindVertexBuffer(DiRenderUnit* unit);

        void                    _InitMainContext(DiGLContext* context);

    private:

        DiString                mWndClass;

        DiGLContext*            mMainContext;

        DiGLContext*            mCurrentContext;
        
        typedef DiMap<DiWndHandle, DiGLContext*> ContextMap;

        ContextMap              mContextMap;

        DiGLUtil*               mGLUtil;

        bool                    mColourWrite[4];

        bool                    mDepthWrite;

        uint32                  mStencilMask;

        DiGLBufferManager*      mGLBufferManager;

        DiGLFBOManager*         mGLFBOManager;

        typedef DiMap<DiPair<DiShaderInstance*, DiShaderInstance*>, DiGLShaderLinker*> ProgramMap;

        ProgramMap              mProgramMaps;

        DiGLShaderLinker*       mCurrentProgram;
    };
}

#endif