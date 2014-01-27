
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
    struct testvbo
    {
        testvbo()
        {
            vboId = 0;
        }

        void init()
        {
            glGenBuffersARB(1, &vboId);
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
            glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vertices)+sizeof(normals)+sizeof(colors), 0, GL_STATIC_DRAW_ARB);
            glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, sizeof(vertices), vertices);                             // copy vertices starting from 0 offest
            glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vertices), sizeof(normals), normals);                // copy normals after vertices
            glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vertices)+sizeof(normals), sizeof(colors), colors);  // copy colours after normals
        }

        void render()
        {
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);

            glEnableClientState(GL_NORMAL_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);
            glEnableClientState(GL_VERTEX_ARRAY);

            glNormalPointer(GL_FLOAT, 0, (void*)sizeof(vertices));
            glColorPointer(3, GL_FLOAT, 0, (void*)(sizeof(vertices)+sizeof(normals)));
            glVertexPointer(3, GL_FLOAT, 0, 0);

            glDrawArrays(GL_TRIANGLES, 0, 36);

            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_COLOR_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);
        }

        GLuint vboId;
        static GLfloat vertices[9 * 12];
        static GLfloat normals[9 * 12];
        static GLfloat colors[9 * 12];
    };

    class DI_GLDRV_API DiGLDriver : public DiGfxDriver
    {
    public:

        DiGLDriver();

        virtual                 ~DiGLDriver();

        testvbo                 testdat;

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

        void                    GetWindowDimension(DiWndHandle wnd, uint32& w, uint32& h);

        void                    Clear(uint32 flag, const DiColor& col, float depth, unsigned short stencil = 0);

        // Create a new one if not existed
        DiGLShaderLinker*       GetShaderLinker(DiShaderInstance* vs, DiShaderInstance* ps);

        DiGLContext*            GetContext(DiWndHandle wnd);

        DiGLContext*            GetCurrentContext() { return mCurrentContext; }

        DiWindow*               CreateWnd();

        static DiGLBufferManager* BufferMgr;

        static DiGLFBOManager*    FBOManager;

    private:

        DiGLContext*            _CreateContext(DiWndHandle wnd);

        DiGLUtil*               _CreateGLUtil();

        bool                    _BindSourceData(DiRenderUnit* unit);

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