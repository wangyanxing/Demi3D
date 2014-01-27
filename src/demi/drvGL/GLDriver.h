
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
            VBO = 0;
            IBO = 0;
        }

        void init()
        {
            DiVec3 Vertices[4];
            Vertices[0] = DiVec3(-1.0f, -1.0f, 0.0f);
            Vertices[1] = DiVec3(0.0f, -1.0f, 1.0f);
            Vertices[2] = DiVec3(1.0f, -1.0f, 0.0f);
            Vertices[3] = DiVec3(0.0f, 1.0f, 0.0f);

            glGenBuffersARB(1, &VBO);
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, VBO);
            glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

            unsigned int Indices[] = { 0, 3, 1,
                1, 3, 2,
                2, 3, 0,
                0, 1, 2 };

            glGenBuffersARB(1, &IBO);
            glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, IBO);
            glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(Indices), Indices, GL_STATIC_DRAW);
        }

        void render()
        {
            glEnableVertexAttribArrayARB(0);
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, VBO);
            glVertexAttribPointerARB(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, IBO);

            glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

            glDisableVertexAttribArrayARB(0);
        }

        GLuint VBO;
        GLuint IBO;

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