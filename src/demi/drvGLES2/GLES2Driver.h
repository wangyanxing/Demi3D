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

#ifndef GLES2Driver_h__
#define GLES2Driver_h__

#include "ES2Prerequisites.h"
#include "GfxDriver.h"

namespace Demi
{
    class DI_GLES2_API DiGLES2Driver : public DiGfxDriver
    {
    public:

        DiGLES2Driver();

        virtual                 ~DiGLES2Driver();

    public:

        DiGfxDriverType         GetDriverType() const { return DRV_OPENGL_ES2; }

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

        void                    SetDepthBias(float constantBias, float slopeScaledBias);

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

        DiPair<float, float>    GetDepthInputRange() const;

        void                    SetColorBufferWriteEnabled(bool r, bool g, bool b, bool a);

        void                    GetDepthStencilFormatFor(DiPixelFormat format, GLenum *depthFormat,
                                    GLenum *stencilFormat);
        
        void                    CreateWindowTarget(DiRenderTarget*& outRT, DiWndHandle wnd);

        void                    BindMaterialStates(const DiMaterial* mat);

        void                    Clear(uint32 flag, const DiColor& col, float depth, unsigned short stencil = 0);

        DiGfxCaps*              InitGfxCaps();

        // Create a new one if not existed
#ifdef GLES2_USE_PIPELINE
        DiGLES2ShaderPipeline*  GetShaderLinker(DiShaderInstance* vs, DiShaderInstance* ps);
#else
        DiGLES2ShaderLinker*    GetShaderLinker(DiShaderInstance* vs, DiShaderInstance* ps);
#endif

        DiGLES2Context*         GetContext(DiWndHandle wnd);

        DiGLES2Context*         GetCurrentContext() { return mCurrentContext; }

        DiGLES2Context*         GetMainContext() { return mMainContext; }

        void                    UnregisterContext(DiGLES2Context *context);

        DiWindow*               CreateWnd();

        static DiGLES2StateCache*   StateCache;

        static DiGLES2UniformCache* UniformCache;

        static DiString&            GetGlslVersion();

        static DiGLES2FBOManager*   FBOManager;

        static DiGLES2Util*         GLUtil;

    private:

        DiGLES2Util*            _CreateGLUtil();

        bool                    _BindVertexBuffer(DiRenderUnit* unit);

        void                    _InitMainContext(DiGLES2Context* context);

    private:

        DiString                mWndClass;

        DiGLES2Context*         mMainContext;

        DiGLES2Context*         mCurrentContext;
        
        typedef DiMap<DiWndHandle, DiGLES2Context*> ContextMap;

        ContextMap              mContextMap;

        DiGLES2Util*            mGLUtil;

        bool                    mColourWrite[4];

        DiGLES2FBOManager*      mGLFBOManager;

#ifdef GLES2_USE_PIPELINE
        typedef DiMap<DiPair<DiShaderInstance*, DiShaderInstance*>, DiGLES2ShaderPipeline*> ProgramMap;
#else
        typedef DiMap<DiPair<DiShaderInstance*, DiShaderInstance*>, DiGLES2ShaderLinker*> ProgramMap;
#endif

        ProgramMap              mProgramMaps;

        DiVector<GLuint>        mRenderAttribsBound;

        DiVector<GLuint>        mRenderInstanceAttribsBound;
    };
}

#endif