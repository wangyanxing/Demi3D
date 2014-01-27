
#include "GLDriver.h"
#include "GLVertexBuffer.h"
#include "GLIndexBuffer.h"
#include "GfxDriver.h"
#include "GLTexture.h"
#include "GLContext.h"
#include "GLBufferManager.h"
#include "GLTypeMappings.h"
#include "GLFrameBuffer.h"
#include "GLRenderTarget.h"
#include "GLShader.h"
#include "GLShaderParam.h"

#include "RenderWindow.h"
#include "RenderUnit.h"

#ifdef WIN32
#   include "Win32Window.h"
#   include "Win32GLContext.h"
#   include "Win32GLUtil.h"
#endif

// Convenience macro from ARB_vertex_buffer_object spec
#define VBO_BUFFER_OFFSET(i) ((char *)NULL + (i))

GLenum glewContextInit(DiGLUtil*);

namespace Demi
{
    GLfloat testvbo::vertices[] = { 1, 1, 1, -1, 1, 1, -1, -1, 1,      // v0-v1-v2 (front)
        -1, -1, 1, 1, -1, 1, 1, 1, 1,      // v2-v3-v0

        1, 1, 1, 1, -1, 1, 1, -1, -1,      // v0-v3-v4 (right)
        1, -1, -1, 1, 1, -1, 1, 1, 1,      // v4-v5-v0

        1, 1, 1, 1, 1, -1, -1, 1, -1,      // v0-v5-v6 (top)
        -1, 1, -1, -1, 1, 1, 1, 1, 1,      // v6-v1-v0

        -1, 1, 1, -1, 1, -1, -1, -1, -1,      // v1-v6-v7 (left)
        -1, -1, -1, -1, -1, 1, -1, 1, 1,      // v7-v2-v1

        -1, -1, -1, 1, -1, -1, 1, -1, 1,      // v7-v4-v3 (bottom)
        1, -1, 1, -1, -1, 1, -1, -1, -1,      // v3-v2-v7

        1, -1, -1, -1, -1, -1, -1, 1, -1,      // v4-v7-v6 (back)
        -1, 1, -1, 1, 1, -1, 1, -1, -1 };    // v6-v5-v4

    GLfloat testvbo::normals[] = { 0, 0, 1, 0, 0, 1, 0, 0, 1,      // v0-v1-v2 (front)
        0, 0, 1, 0, 0, 1, 0, 0, 1,      // v2-v3-v0

        1, 0, 0, 1, 0, 0, 1, 0, 0,      // v0-v3-v4 (right)
        1, 0, 0, 1, 0, 0, 1, 0, 0,      // v4-v5-v0

        0, 1, 0, 0, 1, 0, 0, 1, 0,      // v0-v5-v6 (top)
        0, 1, 0, 0, 1, 0, 0, 1, 0,      // v6-v1-v0

        -1, 0, 0, -1, 0, 0, -1, 0, 0,      // v1-v6-v7 (left)
        -1, 0, 0, -1, 0, 0, -1, 0, 0,      // v7-v2-v1

        0, -1, 0, 0, -1, 0, 0, -1, 0,      // v7-v4-v3 (bottom)
        0, -1, 0, 0, -1, 0, 0, -1, 0,      // v3-v2-v7

        0, 0, -1, 0, 0, -1, 0, 0, -1,      // v4-v7-v6 (back)
        0, 0, -1, 0, 0, -1, 0, 0, -1 };    // v6-v5-v4

    GLfloat testvbo::colors[] = { 1, 1, 1, 1, 1, 0, 1, 0, 0,      // v0-v1-v2 (front)
        1, 0, 0, 1, 0, 1, 1, 1, 1,      // v2-v3-v0

        1, 1, 1, 1, 0, 1, 0, 0, 1,      // v0-v3-v4 (right)
        0, 0, 1, 0, 1, 1, 1, 1, 1,      // v4-v5-v0

        1, 1, 1, 0, 1, 1, 0, 1, 0,      // v0-v5-v6 (top)
        0, 1, 0, 1, 1, 0, 1, 1, 1,      // v6-v1-v0

        1, 1, 0, 0, 1, 0, 0, 0, 0,      // v1-v6-v7 (left)
        0, 0, 0, 1, 0, 0, 1, 1, 0,      // v7-v2-v1

        0, 0, 0, 0, 0, 1, 1, 0, 1,      // v7-v4-v3 (bottom)
        1, 0, 1, 1, 0, 0, 0, 0, 0,      // v3-v2-v7

        0, 0, 1, 0, 0, 0, 0, 1, 0,      // v4-v7-v6 (back)
        0, 1, 0, 0, 1, 1, 0, 0, 1 };    // v6-v5-v4;


    DiGLFBOManager*    DiGLDriver::FBOManager = nullptr;
    DiGLBufferManager* DiGLDriver::BufferMgr  = nullptr;

    DiGLDriver::DiGLDriver()
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
    }
    
    DiGLDriver::~DiGLDriver()
    {
    }

    bool DiGLDriver::InitGfx(DiWndHandle wnd)
    {
        DI_LOG("OpenGL driver is intializing.");

        mGLUtil = _CreateGLUtil();
        _InitMainContext(_CreateContext(wnd));

        mGLFBOManager = DI_NEW DiGLFBOManager(true);    // TODO: Check atimode

        testdat.init();

        return true;
    }

    bool DiGLDriver::InitGfx(uint16 width, uint16 height, bool fullscreen)
    {
        DI_LOG("Creating window: %d x %d, %s mode.", width, height, fullscreen?"full screen":"window");
        mWidth = width;
        mHeight = height;

        return true;
    }

    bool DiGLDriver::IsDeviceLost()
    {
        return false;
    }

    void DiGLDriver::BeginFrame()
    {
        glEnable(GL_SCISSOR_TEST);
    }
    
    void DiGLDriver::EndFrame()
    {
        glDisable(GL_SCISSOR_TEST);
        BufferMgr->ReleaseScratchBuffers();
    }

    void DiGLDriver::ReleaseGfx()
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

        DI_LOG("OpenGL stuff successfully released.");
    }

    void DiGLDriver::SetShaderConsts(DiShaderType shaderType, int regID, const DiMat4& mat)
    {
        if (regID < 0)
            return;

        static DiMat4 m;
        mat.transpose(m);
    }

    void DiGLDriver::SetShaderConsts(DiShaderType shaderType, int regID, const DiVec2& vec)
    {
        static DiVec4 t;

        if (regID < 0)
            return;

        t.x = vec.x;
        t.y = vec.y;
        t.z = 0;
        t.w = 0;

        SetShaderConsts(shaderType, regID, t);
    }

    void DiGLDriver::SetShaderConsts(DiShaderType shaderType, int regID, const DiVec3& vec)
    {
        static DiVec4 t;

        if (regID < 0)
            return;

        t.x = vec.x;
        t.y = vec.y;
        t.z = vec.z;
        t.w = 0;

        SetShaderConsts(shaderType, regID, t);
    }

    void DiGLDriver::SetShaderConsts(DiShaderType shaderType, int regID, const DiVec4& vec)
    {
        if (regID < 0)
            return;
    }

    void DiGLDriver::SetShaderConsts(DiShaderType shaderType, int regID, float val)
    {
        if (regID < 0)
            return;

        static DiVec4 t;
        t.x = val;
        t.y = 0;
        t.z = 0;
        t.w = 0;

        SetShaderConsts(shaderType, regID, t);
    }

    void DiGLDriver::SetShaderConsts(DiShaderType shaderType, int regID, int val)
    {
        if (regID < 0)
            return;
    }

    void DiGLDriver::SetShaderConsts(DiShaderType shaderType, int regID, const DiColor& col)
    {
        if (regID < 0)
            return;

        static DiVec4 t;
        t.x = col.r;
        t.y = col.g;
        t.z = col.b;
        t.w = col.a;

        SetShaderConsts(shaderType, regID, t);
    }

    void DiGLDriver::ConvertProjectionMatrix(const DiMat4& matrix, DiMat4& dest)
    {
        dest = matrix;
    }

    bool DiGLDriver::IsDeviceLost() const
    {
        return false;
    }

    bool DiGLDriver::RenderGeometry(DiRenderUnit* unit)
    {
#if 0
        if (!_BindSourceData(unit))
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
#else
        testdat.render();
#endif

#if 0
        GLenum glErr = glGetError();
        if (glErr != GL_NO_ERROR)
            DiGLShaderInstance::LogGLSLError(glErr, "renderGeometry: ", 0);
#endif

        return true;
    }

    const DiVec2& DiGLDriver::GetTexelOffset() const
    {
        static DiVec2 texelOffset(-0.5f, -0.5f);
        return texelOffset;
    }

    bool DiGLDriver::ResetDevice(uint16 w, uint16 h)
    {
        return true;
    }

    void DiGLDriver::SetViewport(int x, int y, int w, int h, float minz, float maxz)
    {
        glViewport(x, y, w, h);
        glScissor(x, y, w, h);
    }

    void DiGLDriver::SetFillMode(DiFillMode mode)
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

    DiTextureDrv* DiGLDriver::CreateTextureDriver(DiTexture* texture)
    {
        return DI_NEW DiGLTextureDrv(texture);
    }

    DiIndexBuffer* DiGLDriver::CreateIndexBuffer()
    {
        return DI_NEW DiGLIndexBuffer();
    }

    DiVertexBuffer* DiGLDriver::CreateVertexBuffer()
    {
        return DI_NEW DiGLVertexBuffer();
    }

    DiVertexDeclaration* DiGLDriver::CreateVertexDeclaration()
    {
        return DI_NEW DiGLVertexDeclaration();
    }

    DiShaderInstance* DiGLDriver::CreateVSInstance(DiShaderProgram* prog)
    {
        return DI_NEW DiGLShaderInstance(SHADER_VERTEX, prog);
    }

    DiShaderInstance* DiGLDriver::CreatePSInstance(DiShaderProgram* prog)
    {
        return DI_NEW DiGLShaderInstance(SHADER_PIXEL, prog);
    }

    DiRenderTarget* DiGLDriver::CreateRenderTarget()
    {
        auto rt = DI_NEW DiGLRenderTarget();
        rt->Init();
        return rt;
    }

    void DiGLDriver::CreateWindowTarget(DiRenderTarget*& outRT, DiWndHandle wnd)
    {
        auto wt = DI_NEW DiGLWindowTarget();
        wt->Create(wnd, mCurrentContext);
        outRT = wt;
    }

    void DiGLDriver::GetWindowDimension(DiWndHandle wnd, uint32& w, uint32& h)
    {
        RECT rect;
        GetClientRect((HWND)wnd, &rect);
        w = rect.right - rect.left;
        h = rect.bottom - rect.top;
    }

    void DiGLDriver::BindMaterialStates(const DiMaterial* mat)
    {
        // filling mode
        bool finalwireframe = false;
        if (mat->GetForceWireframe())
        {
            finalwireframe = mat->IsWireframe();
        }
        else
        {
            if (mat->IsWireframe())
                finalwireframe = true;
        }
        glPolygonMode(GL_FRONT_AND_BACK, finalwireframe ? GL_LINE : GL_FILL);

        // culling mode
        GLenum cullMode;
        switch (mat->GetCullMode())
        {
        case CULL_NONE:
            glDisable(GL_CULL_FACE);
            return;
            break;
        case CULL_CW:
            cullMode = GL_BACK;
            break;
        case CULL_CCW:
            cullMode = GL_FRONT;
            break;
        }
        glEnable(GL_CULL_FACE);
        glCullFace(cullMode);

        // depth check/write
        if (mat->GetDepthCheck())
        {
            glClearDepth(1.0f);
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }
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

    void DiGLDriver::Clear(uint32 flag, const DiColor& col, float depth, unsigned short stencil /*= 0*/)
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

    DiWindow* DiGLDriver::CreateWnd()
    {
#ifdef WIN32
        return DI_NEW DiWin32Window();
#else
        return nullptr;
#endif
    }

    DiGLContext* DiGLDriver::_CreateContext(DiWndHandle wnd)
    {
        DiGLContext* ret = nullptr;
#ifdef WIN32
        ret = DI_NEW DiWin32GLContext(static_cast<DiWin32GLUtil*>(mGLUtil), wnd);
        DI_LOG("Win32 GL context created.");
#endif
        mContextMap[wnd] = ret;
        return ret;
    }

    DiGLUtil* DiGLDriver::_CreateGLUtil()
    {
        DiGLUtil* ret = nullptr;
#ifdef WIN32
        ret = DI_NEW DiWin32GLUtil();
#endif
        return ret;
    }

    void DiGLDriver::SetColorBufferWriteEnabled(bool r, bool g, bool b, bool a)
    {
        glColorMask(r, g, b, a);
        mColourWrite[0] = r;
        mColourWrite[1] = g;
        mColourWrite[2] = b;
        mColourWrite[3] = a;
    }

    bool DiGLDriver::_BindVertexBuffer(DiRenderUnit* unit)
    {
        if (unit->mSourceData.empty() || !unit->mVertexDecl)
            return false;

        if (!unit->mPrimitiveCount)
            return false;

        static bool attriIndex[16];
        for (auto i = 0; i < 16; i++) attriIndex[i] = false;

        DiVertexElements& elements = unit->mVertexDecl->GetElements();

        for (auto it = unit->mSourceData.begin(); it != unit->mSourceData.end(); ++it)
        {
            DiGLVertexBuffer* vb = static_cast<DiGLVertexBuffer*>(*it);
            vb->Bind();

            DiVertexElements::ElementsList e;
            elements.GetElementsAtStream(vb->GetStreamId(), e);

            for (auto i = e.begin(); i != e.end(); ++i)
            {
                void* pBufferData = nullptr;
                pBufferData = VBO_BUFFER_OFFSET(i->Offset);

                auto elemNum = DiVertexElements::GetElementTypeCount((DiVertexType)i->Type);
                auto gltype  = DiGLTypeMappings::GetGLType((DiVertexType)i->Type);
                GLsizei buffersize = static_cast<GLsizei>(vb->GetBufferSize());

                GLint attrib = DiGLTypeMappings::GetFixedAttributeIndex(i->Usage, i->UsageIndex);
                attriIndex[attrib] = true;

                uint16 count = elements.GetElementTypeCount((DiVertexType)i->Type);

                GLboolean normalised = GL_FALSE;
                switch (i->Type)
                {
                case VERT_TYPE_COLOR:
                    // Because GL takes these as a sequence of single unsigned bytes, count needs to be 4
                    // VertexElement::getTypeCount treats them as 1 (RGBA)
                    // Also need to normalise the fixed-point data
                    count = 4;
                    normalised = GL_TRUE;
                    break;
                default:
                    break;
                };

                glVertexAttribPointerARB(attrib, count, gltype, normalised, buffersize, pBufferData);
                glEnableVertexAttribArrayARB(attrib);
            }
        }

        for (auto i = 0; i < 16; i++)
            if (!attriIndex[i]) glDisableVertexAttribArrayARB(i);

        return true;
    }

    bool DiGLDriver::_BindSourceData(DiRenderUnit* unit)
    {
        if (unit->mSourceData.empty() || !unit->mVertexDecl)
            return false;

        if (!unit->mPrimitiveCount)
            return false;

        DiVertexElements& elements = unit->mVertexDecl->GetElements();

        for (auto it = unit->mSourceData.begin(); it != unit->mSourceData.end(); ++it)
        {
            DiGLVertexBuffer* vb = static_cast<DiGLVertexBuffer*>(*it);
            vb->Bind();

            DiVertexElements::ElementsList e;
            elements.GetElementsAtStream(vb->GetStreamId(), e);
            
            for (auto i = e.begin(); i != e.end(); ++i)
            {
                void* pBufferData = nullptr;
                pBufferData = VBO_BUFFER_OFFSET(i->Offset);

                bool isCustomAttr = DiGLTypeMappings::IsAttributeValid(i->Usage);
                auto elemNum = DiVertexElements::GetElementTypeCount((DiVertexType)i->Type);
                auto gltype = DiGLTypeMappings::GetGLType((DiVertexType)i->Type);
                GLsizei buffersize = static_cast<GLsizei>(vb->GetBufferSize());

                if (isCustomAttr)
                {
                    GLint attrib = DiGLTypeMappings::GetFixedAttributeIndex(i->Usage, i->UsageIndex);
                    uint16 count = elements.GetElementTypeCount((DiVertexType)i->Type);

                    GLboolean normalised = GL_FALSE;
                    switch (i->Type)
                    {
                    case VERT_TYPE_COLOR:
                        // Because GL takes these as a sequence of single unsigned bytes, count needs to be 4
                        // VertexElement::getTypeCount treats them as 1 (RGBA)
                        // Also need to normalise the fixed-point data
                        count = 4;
                        normalised = GL_TRUE;
                        break;
                    default:
                        break;
                    };

                    glVertexAttribPointerARB(
                        attrib, count,
                        gltype, normalised,
                        buffersize, pBufferData);
                    glEnableVertexAttribArrayARB(attrib);
                }
                else
                {
                    switch (i->Usage)
                    {
                    case VERT_USAGE_POSITION:
                        glVertexPointer(elemNum, gltype, buffersize, pBufferData);
                        glEnableClientState(GL_VERTEX_ARRAY);
                        break;
                    case VERT_USAGE_NORMAL:
                        glNormalPointer(gltype, buffersize, pBufferData);
                        glEnableClientState(GL_NORMAL_ARRAY);
                        break;
                    case VERT_USAGE_COLOR:
                        glColorPointer(4, gltype, buffersize, pBufferData);
                        glEnableClientState(GL_COLOR_ARRAY);
                        break;
                    case VERT_USAGE_SECONDARY_COLOR:
                        if (GLEW_EXT_secondary_color)
                        {
                            glSecondaryColorPointerEXT(4, gltype, buffersize, pBufferData);
                            glEnableClientState(GL_SECONDARY_COLOR_ARRAY);
                        }
                        break;
                    case VERT_USAGE_TEXCOORD:
                        glClientActiveTextureARB(GL_TEXTURE0 + i->UsageIndex);
                        glTexCoordPointer(elemNum, gltype, buffersize, pBufferData);
                        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                        break;
                    default:
                        break;
                    };
                }
            }
        }

        return true;
    }

    void DiGLDriver::_InitMainContext(DiGLContext* context)
    {
        mMainContext = context;
        mCurrentContext = mMainContext;

        if (mCurrentContext)
            mCurrentContext->BeginContext();

        mGLUtil->InitExtensions();

        glewContextInit(mGLUtil);
    }

    void DiGLDriver::GetDepthStencilFormatFor(GLenum internalColourFormat, 
        GLenum *depthFormat, GLenum *stencilFormat)
    {
        mGLFBOManager->GetBestDepthStencil(internalColourFormat, depthFormat, stencilFormat);
    }

    DiGLContext* DiGLDriver::GetContext(DiWndHandle wnd)
    {
        return mContextMap[wnd];
    }

    void DiGLDriver::BindShaders(DiShaderProgram* vs, DiShaderProgram* ps)
    {
        DiGLShaderLinker* prog = GetShaderLinker(vs->GetShader(), ps->GetShader());
        prog->Bind();
    }

    DiGLShaderLinker* DiGLDriver::GetShaderLinker(DiShaderInstance* vs, DiShaderInstance* ps)
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

    DiShaderParameter* DiGLDriver::CreateShaderParams(DiMaterial& mat)
    {
        return DI_NEW DiGLShaderParam(mat);
    }

    const DiString& DiGLDriver::GetShaderFileExtension() const
    {
        static DiString shaderext = ".glsl";
        return shaderext;
    }

}