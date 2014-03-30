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
#include "DrvGLES2Pch.h"
#include "GLES2Driver.h"
#include "ES2VertexBuffer.h"
#include "ES2IndexBuffer.h"
#include "GfxDriver.h"
#include "ES2Texture.h"
#include "ES2Context.h"
#include "ES2BufferManager.h"
#include "ES2TypeMappings.h"
#include "ES2FrameBuffer.h"
#include "ES2RenderTarget.h"
#include "ES2Shader.h"
#include "ES2ShaderParam.h"
#include "ES2StateCache.h"
#include "ES2UniformCache.h"

#include "RenderWindow.h"
#include "RenderUnit.h"

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
#   include "Win32/Win32Window.h"
#   include "Win32/Win32GLContext.h"
#   include "Win32/Win32GLUtil.h"
#elif DEMI_PLATFORM == DEMI_PLATFORM_OSX
#   include "OSX/OSXGLUtil.h"
#endif

// Convenience macro from ARB_vertex_buffer_object spec
#define VBO_BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace Demi
{
    DiGLES2FBOManager*    DiGLES2Driver::FBOManager = nullptr;
    DiGLBufferManager* DiGLES2Driver::BufferMgr  = nullptr;

    DiGLES2Driver::DiGLES2Driver()
        :mMainContext(nullptr),
        mGLUtil(nullptr),
        mDepthWrite(true), 
        mStencilMask(0xFFFFFFFF),
        mGLBufferManager(nullptr),
        mCurrentContext(nullptr),
        mGLFBOManager(nullptr),
        mCurrentProgram(nullptr)
    {
        mColourWrite[0] = mColourWrite[1] = mColourWrite[2] = mColourWrite[3] = true;
        mGLBufferManager = DI_NEW DiGLBufferManager();
        mGLUtil = _CreateGLUtil();
        GLUtil = mGLUtil;

        DI_ASSERT(!StateCache);
        DI_ASSERT(!UniformCache);

        StateCache = DI_NEW DiGLES2StateCache();
        UniformCache = DI_NEW DiGLES2UniformCache();
    }
    
    DiGLES2Driver::~DiGLES2Driver()
    {
        mGLUtil = nullptr;
        SAFE_DELETE(StateCache);
        SAFE_DELETE(UniformCache);
    }

    bool DiGLES2Driver::InitGfx(DiWndHandle wnd)
    {
        DI_INFO("OpenGL driver is intializing.");

        _InitMainContext(_CreateContext(wnd));

        mGLFBOManager = DI_NEW DiGLES2FBOManager(true);    // TODO: Check atimode

        return true;
    }

    bool DiGLES2Driver::InitGfx(uint16 width, uint16 height, bool fullscreen)
    {
        DI_INFO("Creating window: %d x %d, %s mode.", width, height, fullscreen?"full screen":"window");
        mWidth = width;
        mHeight = height;

        DI_ASSERT(false);

        return true;
    }

    bool DiGLES2Driver::IsDeviceLost()
    {
        return false;
    }

    void DiGLES2Driver::BeginFrame()
    {
        glEnable(GL_SCISSOR_TEST);
    }
    
    void DiGLES2Driver::EndFrame()
    {
        glDisable(GL_SCISSOR_TEST);
    }

    void DiGLES2Driver::ReleaseGfx()
    {
        for (auto i = mProgramMaps.begin(); i != mProgramMaps.end(); ++i)
            DI_DELETE i->second;
        mProgramMaps.clear();

        DI_DELETE mMainContext;
        mMainContext = nullptr;

        DI_DELETE mGLUtil;
        mGLUtil = nullptr;

        DI_DELETE mGLBufferManager;
        mGLBufferManager = nullptr;

        DI_DELETE mGLFBOManager;
        mGLFBOManager = nullptr;

        DI_INFO("OpenGL stuff successfully released.");
    }

    void DiGLES2Driver::ConvertProjectionMatrix(const DiMat4& matrix, DiMat4& dest)
    {
        dest = matrix;
    }

    bool DiGLES2Driver::IsDeviceLost() const
    {
        return false;
    }

    bool DiGLES2Driver::RenderGeometry(DiRenderUnit* unit)
    {
        if (!_BindVertexBuffer(unit))
            return false;

        GLint primType;
        switch (unit->mPrimitiveType)
        {
        case PT_POINTLIST:
            primType = GL_POINTS;
            break;
        case PT_LINELIST:
            primType = GL_LINES;
            break;
        case PT_LINESTRIP:
            primType = GL_LINE_STRIP;
            break;
        case PT_TRIANGLELIST:
            primType = GL_TRIANGLES;
            break;
        case PT_TRIANGLESTRIP:
            primType = GL_TRIANGLE_STRIP;
            break;
        case PT_TRIANGLEFAN:
            primType = GL_TRIANGLE_FAN;
            break;
        default:
            primType = GL_TRIANGLES;
            DI_WARNING("Invalid primitive type");
        }

        if (!unit->mIndexBuffer)
        {
            glDrawArrays(primType, 0, unit->mVerticesNum);
        }
        else
        {
            unit->mIndexBuffer->Bind();
            GLenum indexType = unit->mIndexBuffer->GetType() == IB_16BITS ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
            uint32 indexSize = unit->mIndexBuffer->GetType() == IB_16BITS ? sizeof(unsigned short) : sizeof(unsigned int);

            void* pBufferData = nullptr;
            pBufferData = VBO_BUFFER_OFFSET(unit->mIndexOffset * indexSize);

            glDrawElements(primType, unit->mIndexBuffer->GetMaxIndices(), indexType, pBufferData);
        }

#if 0
        GLenum glErr = glGetError();
        if (glErr != GL_NO_ERROR)
            DiGLShaderInstance::LogGLSLError(glErr, "renderGeometry: ", 0);
#endif

        return true;
    }

    const DiVec2& DiGLES2Driver::GetTexelOffset() const
    {
        static DiVec2 texelOffset(-0.5f, -0.5f);
        return texelOffset;
    }

    bool DiGLES2Driver::ResetDevice(uint16 w, uint16 h)
    {
        return true;
    }

    void DiGLES2Driver::SetViewport(int x, int y, int w, int h, float minz, float maxz)
    {
        glViewport(x, y, w, h);
        glScissor(x, y, w, h);
    }

    void DiGLES2Driver::SetFillMode(DiFillMode mode)
    {
        switch (mode)
        {
        case FM_SOLID:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case FM_WIREFRAME:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case FM_POINT:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;
        }
    }

    DiTextureDrv* DiGLES2Driver::CreateTextureDriver(DiTexture* texture)
    {
        return DI_NEW DiGLTextureDrv(texture);
    }

    DiIndexBuffer* DiGLES2Driver::CreateIndexBuffer()
    {
        return DI_NEW DiGLIndexBuffer();
    }

    DiVertexBuffer* DiGLES2Driver::CreateVertexBuffer()
    {
        return DI_NEW DiGLVertexBuffer();
    }

    DiVertexDeclaration* DiGLES2Driver::CreateVertexDeclaration()
    {
        return DI_NEW DiGLVertexDeclaration();
    }

    DiShaderInstance* DiGLES2Driver::CreateVSInstance(DiShaderProgram* prog)
    {
        return DI_NEW DiGLShaderInstance(SHADER_VERTEX, prog);
    }

    DiShaderInstance* DiGLES2Driver::CreatePSInstance(DiShaderProgram* prog)
    {
        return DI_NEW DiGLShaderInstance(SHADER_PIXEL, prog);
    }

    DiRenderTarget* DiGLES2Driver::CreateRenderTarget()
    {
        auto rt = DI_NEW DiGLES2RenderTarget();
        rt->Init();
        return rt;
    }

    void DiGLES2Driver::CreateWindowTarget(DiRenderTarget*& outRT, DiWndHandle wnd)
    {
        auto wt = DI_NEW DiGLWindowTarget();
        wt->Create(wnd, mCurrentContext);
        outRT = wt;
    }

    void DiGLES2Driver::BindMaterialStates(const DiMaterial* mat)
    {
        // filling mode
        bool finalwireframe = false;
        if (mat->GetForceWireframe())
            finalwireframe = mat->IsWireframe();
        else
        {
            if (mat->IsWireframe())
                finalwireframe = true;
        }

        glPolygonMode(GL_FRONT_AND_BACK, finalwireframe ? GL_LINE : GL_FILL);

        // culling
        bool flip = ((mNeedTextureFlipping && !mInvertVertexWinding) ||
                     (!mNeedTextureFlipping && mInvertVertexWinding));
        if (mat->GetCullMode() == CULL_NONE)
            glDisable(GL_CULL_FACE);
        else
        {
            glEnable(GL_CULL_FACE);
            if (mat->GetCullMode() == CULL_CCW)
                flip = !flip;
            if (flip)
                glCullFace(GL_FRONT);
            else
                glCullFace(GL_BACK);
        }

        // depth check/write
        if (mat->GetDepthCheck())
        {
            glClearDepth(1.0f);
            glEnable(GL_DEPTH_TEST);
        }
        else
            glDisable(GL_DEPTH_TEST);

        glDepthMask(mat->GetDepthWrite() ? GL_TRUE : GL_FALSE);

        // blending mode
        switch(mat->GetBlendMode())
        {
        case BLEND_REPLACE:
            glDisable(GL_BLEND);
            break;
        case BLEND_ADD:
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);
            break;
        case BLEND_MULTIPLY:
            glEnable(GL_BLEND);
            glBlendFunc(GL_DST_COLOR, GL_ZERO);
            break;
        case BLEND_ALPHA:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
            break;
        case BLEND_TRANS_COLOR:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
            break;
        case  BLEND_ONE_INV_ALPHA:
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            break;
        default:
            glDisable(GL_BLEND);
        }

        GLint func = GL_FUNC_ADD;
        if (GLEW_VERSION_1_4 || GLEW_ARB_imaging)
            glBlendEquation(func);
        else if (GLEW_EXT_blend_minmax && (func == GL_MIN || func == GL_MAX))
            glBlendEquationEXT(func);
    }

    void DiGLES2Driver::Clear(uint32 flag, const DiColor& col, float depth, unsigned short stencil /*= 0*/)
    {
        bool colourMask = !mColourWrite[0] || !mColourWrite[1]
            || !mColourWrite[2] || !mColourWrite[3];

        GLbitfield clearBuf = 0;
        if (flag & CLEAR_COLOR)
        {
            clearBuf |= GL_COLOR_BUFFER_BIT;
            if (colourMask)
                glColorMask(true, true, true, true);

            glClearColor(col.r, col.g, col.b, col.a);
        }
        
        if (flag & CLEAR_DEPTH)            
        {
            clearBuf |= GL_DEPTH_BUFFER_BIT;
            if (!mDepthWrite)
                glDepthMask(GL_TRUE);
            
            glClearDepth(depth);
        }

        if (flag & CLEAR_STENCIL)
        {
            clearBuf |= GL_STENCIL_BUFFER_BIT;

            glStencilMask(0xFFFFFFFF);
            glClearStencil(stencil);
        }

        GLboolean scissorTestEnabled = glIsEnabled(GL_SCISSOR_TEST);
        if (!scissorTestEnabled)
            glEnable(GL_SCISSOR_TEST);

        // Sets the scissor box as same as viewport
        GLint viewport[4] = { 0, 0, 0, 0 };
        GLint scissor[4] = { 0, 0, 0, 0 };
        glGetIntegerv(GL_VIEWPORT, viewport);
        glGetIntegerv(GL_SCISSOR_BOX, scissor);
        bool scissorBoxDifference =
            viewport[0] != scissor[0] || viewport[1] != scissor[1] ||
            viewport[2] != scissor[2] || viewport[3] != scissor[3];
        if (scissorBoxDifference)
            glScissor(viewport[0], viewport[1], viewport[2], viewport[3]);

        glClear(clearBuf);

#if 1
        GLenum glErr = glGetError();
        if (glErr != GL_NO_ERROR)
            DiGLShaderInstance::LogGLSLError(glErr, "glClear error: ", 0);
#endif

        if (scissorBoxDifference)
            glScissor(scissor[0], scissor[1], scissor[2], scissor[3]);

        if (!scissorTestEnabled)
            glDisable(GL_SCISSOR_TEST);

        // Reset buffer write state
        if (!mDepthWrite && (flag & CLEAR_DEPTH))
            glDepthMask(GL_FALSE);
        
        if (colourMask && (flag & CLEAR_COLOR))
            glColorMask(mColourWrite[0], mColourWrite[1], mColourWrite[2], mColourWrite[3]);
        
        if (flag & CLEAR_STENCIL)
            glStencilMask(mStencilMask);
    }

    DiWindow* DiGLES2Driver::CreateWnd()
    {
        return mGLUtil->CreateNewWindow();
    }

    DiGLContext* DiGLES2Driver::_CreateContext(DiWndHandle wnd)
    {
        DiGLContext* ret = nullptr;
#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
        ret = DI_NEW DiEGLContext(static_cast<DiEGLUtil*>(mGLUtil), wnd);
        DI_INFO("Win32 GL context created.");
#elif DEMI_PLATFORM == DEMI_PLATFORM_OSX
        //ret = DI_NEW DiOSXCocoaContext(static_cast<DiOSXGLUtil*>(mGLUtil), wnd);
        DI_INFO("OSX GL context created.");
#endif
        mContextMap[wnd] = ret;
        return ret;
    }

    DiGLES2Util* DiGLES2Driver::_CreateGLUtil()
    {
        DiGLES2Util* ret = nullptr;
#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
        ret = DI_NEW DiEGLUtil();
#elif DEMI_PLATFORM == DEMI_PLATFORM_OSX
        ret = DI_NEW DiOSXGLUtil();
#endif
        return ret;
    }

    void DiGLES2Driver::SetColorBufferWriteEnabled(bool r, bool g, bool b, bool a)
    {
        glColorMask(r, g, b, a);
        mColourWrite[0] = r;
        mColourWrite[1] = g;
        mColourWrite[2] = b;
        mColourWrite[3] = a;
    }

    bool DiGLES2Driver::_BindVertexBuffer(DiRenderUnit* unit)
    {
        if (unit->mSourceData.empty() || !unit->mVertexDecl)
            return false;

        if (!unit->mPrimitiveCount)
            return false;

        // yes it's ugly now, should be fixed later
        static bool attriIndex[16];
        for (auto i = 0; i < 16; i++) attriIndex[i] = false;

        DiVertexElements& elements = unit->mVertexDecl->GetElements();

        for (auto it = unit->mSourceData.begin(); it != unit->mSourceData.end(); ++it)
        {
            DiGLVertexBuffer* vb = static_cast<DiGLVertexBuffer*>(*it);
            vb->Bind();
            GLsizei stride = static_cast<GLsizei>(vb->GetStride());

            DiVertexElements::ElementsList e;
            elements.GetElementsAtStream(vb->GetStreamId(), e);

            for (auto i = e.begin(); i != e.end(); ++i)
            {
                void* pBufferData = nullptr;
                pBufferData = VBO_BUFFER_OFFSET(i->Offset);

                auto gltype  = DiGLTypeMappings::GetGLType((DiVertexType)i->Type);

                GLint attrib = DiGLTypeMappings::GetFixedAttributeIndex(i->Usage, i->UsageIndex);
                attriIndex[attrib] = true;

                uint16 count = elements.GetElementTypeCount((DiVertexType)i->Type);

                GLboolean normalised = GL_FALSE;
                if(i->Type == VERT_TYPE_COLOR)
                    normalised = GL_TRUE;

                glVertexAttribPointerARB(attrib, count, gltype, normalised, stride, pBufferData);
                glEnableVertexAttribArrayARB(attrib);
            }
        }

        for (auto i = 0; i < 16; i++)
            if (!attriIndex[i]) glDisableVertexAttribArrayARB(i);

        return true;
    }

    void DiGLES2Driver::_InitMainContext(DiGLContext* context)
    {
        mMainContext = context;
        mCurrentContext = mMainContext;

        if (mCurrentContext)
            mCurrentContext->BeginContext();

        mGLUtil->InitExtensions();

        glewContextInit(mGLUtil);
    }

    void DiGLES2Driver::GetDepthStencilFormatFor(DiPixelFormat format,
        GLenum *depthFormat, GLenum *stencilFormat)
    {
        mGLFBOManager->GetBestDepthStencil(format, depthFormat, stencilFormat);
    }

    DiGLContext* DiGLES2Driver::GetContext(DiWndHandle wnd)
    {
        return mContextMap[wnd];
    }

    void DiGLES2Driver::BindShaders(DiShaderProgram* vs, DiShaderProgram* ps)
    {
        DiGLShaderLinker* prog = GetShaderLinker(vs->GetShader(), ps->GetShader());
        prog->Bind();
    }

    DiGLShaderLinker* DiGLES2Driver::GetShaderLinker(DiShaderInstance* vs, DiShaderInstance* ps)
    {
        auto p = DiPair<DiShaderInstance*, DiShaderInstance*>(vs, ps);
        auto it = mProgramMaps.find(p);
        if (it != mProgramMaps.end())
            return it->second;

        // create a new one
        DiGLShaderLinker* ret = new DiGLShaderLinker(static_cast<DiGLShaderInstance*>(vs),
            static_cast<DiGLShaderInstance*>(ps));

        mProgramMaps[p] = ret;
        return ret;
    }

    DiShaderParameter* DiGLES2Driver::CreateShaderParams(DiMaterial& mat)
    {
        return DI_NEW DiGLShaderParam(mat);
    }

    const DiString& DiGLES2Driver::GetShaderFileExtension() const
    {
        static DiString shaderext = ".glsl";
        return shaderext;
    }

    DiString& DiGLES2Driver::GetGlslVersion()
    {
        static DiString version = "120";
        return version;
    }

    void DiGLES2Driver::SetDepthBias(float constantBias, float slopeScaledBias)
    {
        if (constantBias != 0 || slopeScaledBias != 0)
        {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(-slopeScaledBias, -constantBias);
        }
        else
        {
            glDisable(GL_POLYGON_OFFSET_FILL);
        }
    }

    DiPair<float, float> DiGLES2Driver::GetDepthInputRange() const
    {
        return DiPair<float, float>(-1.0f, 1.0f);
    }

    void DiGLES2Driver::UnregisterContext(DiGLES2Context *context)
    {
        //TODO:
    }

    DiGLES2Util* DiGLES2Driver::GLUtil = nullptr;
    DiGLES2UniformCache* DiGLES2Driver::UniformCache = nullptr;
    DiGLES2StateCache* DiGLES2Driver::StateCache = nullptr;

}