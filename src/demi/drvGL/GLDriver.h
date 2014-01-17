
/********************************************************************
    File:       GLDriver.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "DrvGLDefines.h"
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

        void                    SetShaderConsts(DiShaderType type, int regID, const DiMat4& mat);

        void                    SetShaderConsts(DiShaderType type, int regID, const DiVec2& vec);

        void                    SetShaderConsts(DiShaderType type, int regID, const DiVec3& vec);

        void                    SetShaderConsts(DiShaderType type, int regID, const DiVec4& vec);

        void                    SetShaderConsts(DiShaderType type, int regID, float val);

        void                    SetShaderConsts(DiShaderType type, int regID, int val);

        void                    SetShaderConsts(DiShaderType type, int regID, const DiColor& col);

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

        void                    SetColorBufferWriteEnabled(bool r, bool g, bool b, bool a);
        
        void                    CreateWindowTarget(DiRenderTarget*& outRT, DiWndHandle wnd);

        void                    BindMaterialStates(const DiMaterial* mat);

        void                    GetWindowDimension(DiWndHandle wnd, uint32& w, uint32& h);

        void                    Clear(uint32 flag, const DiColor& col, float depth, unsigned short stencil = 0);

        DiWindow*               CreateWnd();

        static DiGLBufferManager*   BufferMgr;

    private:

        DiGLContext*            _CreateContext(DiWndHandle wnd);

        DiGLUtil*               _CreateGLUtil();

    private:

        DiString                mWndClass;

        DiGLContext*            mMainContext;

        DiGLUtil*               mGLUtil;

        bool                    mColourWrite[4];

        bool                    mDepthWrite;

        uint32                  mStencilMask;

        DiGLBufferManager*      mGLBufferManager;
    };
}