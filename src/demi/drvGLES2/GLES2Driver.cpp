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
#include "ES2TypeMappings.h"
#include "ES2FrameBuffer.h"
#include "ES2RenderTarget.h"
#include "ES2Shader.h"
#include "ES2ShaderParam.h"
#include "ES2StateCache.h"
#include "ES2UniformCache.h"
#include "ES2Util.h"
#include "ES2ShaderPipeline.h"
#include "ES2VertexDeclaration.h"
#include "Capabilities.h"

#include "RenderWindow.h"
#include "RenderUnit.h"
#include "Window.h"

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
#   include "Win32/Win32EGLContext.h"
#   include "Win32/Win32EGLUtil.h"
#   include "Win32/Win32EGLWindow.h"
#elif DEMI_PLATFORM == DEMI_PLATFORM_IOS
#   include "EAGL/EAGLES2Context.h"
#   include "EAGL/EAGL2Util.h"
#   include "EAGL/EAGL2Window.h"
#endif

// Convenience macro from ARB_vertex_buffer_object spec
#define VBO_BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace Demi
{
    DiGLES2FBOManager* DiGLES2Driver::FBOManager = nullptr;

    DiGLES2Driver::DiGLES2Driver()
        :mMainContext(nullptr),
        mGLUtil(nullptr),
        mCurrentContext(nullptr),
        mGLFBOManager(nullptr),
        mActiveShaderPipeline(nullptr)
    {
        mColourWrite[0] = mColourWrite[1] = mColourWrite[2] = mColourWrite[3] = true;
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

        DiWindow* window = mWndManager->GetWindow(wnd);

        DiGLES2Context* context = nullptr;
#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
        context = static_cast<DiEGLWindow*>(window)->GetContext();
#else
        context = static_cast<DiEAGL2Window*>(window)->GetContext();
#endif
        _InitMainContext(context);

        auto tokens = mGLUtil->GetGLVersion().Tokenize(".");
        if (!tokens.empty())
        {
            mDriverVersion.major = tokens[0].AsInt();
            if (tokens.size() > 1)
                mDriverVersion.minor = tokens[1].AsInt();
            if (tokens.size() > 2)
                mDriverVersion.release = tokens[2].AsInt();
        }
        mDriverVersion.build = 0;

        mGLFBOManager = DI_NEW DiGLES2FBOManager(true);

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
        StateCache->setEnabled(GL_SCISSOR_TEST);
        StateCache->setDisabled(GL_DITHER);
    }
    
    void DiGLES2Driver::EndFrame()
    {
        StateCache->setDisabled(GL_SCISSOR_TEST);
        
#if DEMI_PLATFORM == DEI_PLATFORM_IOS
        // msaa
#endif
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

        DI_DELETE mGLFBOManager;
        mGLFBOManager = nullptr;

        DI_INFO("OpenGLES2 stuff successfully released.");
    }

    void DiGLES2Driver::ConvertProjectionMatrix(const DiMat4& matrix, DiMat4& dest)
    {
        dest = matrix;
    }

    bool DiGLES2Driver::IsDeviceLost() const
    {
        return false;
    }

    DiGfxCaps* DiGLES2Driver::InitGfxCaps()
    {
        DiGfxCaps* rsc = DI_NEW DiGfxCaps();

        rsc->setCategoryRelevant(CAPS_CATEGORY_GL, true);
        rsc->setDriverVersion(mDriverVersion);

        const char* deviceName = (const char*)glGetString(GL_RENDERER);
        const char* vendorName = (const char*)glGetString(GL_VENDOR);
        if (deviceName)
        {
            rsc->setDeviceName(deviceName);
        }

        rsc->setRenderSystemName(DRV_OPENGL_ES2);

        // Determine vendor
        if (strstr(vendorName, "Imagination Technologies"))
            rsc->setVendor(GPU_IMAGINATION_TECHNOLOGIES);
        else if (strstr(vendorName, "Apple Computer, Inc."))
            rsc->setVendor(GPU_APPLE);  // iOS Simulator
        else if (strstr(vendorName, "NVIDIA"))
            rsc->setVendor(GPU_NVIDIA);
        else if (strstr(vendorName, "ARM"))
            rsc->setVendor(GPU_ARM);
        else if (strstr(vendorName, "Qualcomm"))
            rsc->setVendor(GPU_QUALCOMM);
        else
            rsc->setVendor(GPU_UNKNOWN);

        // Multitexturing support and set number of texture units
        GLint units;
        CHECK_GL_ERROR(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &units));
        rsc->setNumTextureUnits(std::min<uint16>(16, units));

        // Check for hardware stencil support and set bit depth
        GLint stencil;

        CHECK_GL_ERROR(glGetIntegerv(GL_STENCIL_BITS, &stencil));

        if (stencil)
        {
            rsc->setCapability(RSC_HWSTENCIL);
            rsc->setCapability(RSC_TWO_SIDED_STENCIL);
            rsc->setStencilBufferBitDepth(stencil);
        }

        // Scissor test is standard
        rsc->setCapability(RSC_SCISSOR_TEST);

        // Vertex Buffer Objects are always supported by OpenGL ES
        rsc->setCapability(RSC_VBO);
        if (mGLUtil->CheckExtension("GL_OES_element_index_uint"))
            rsc->setCapability(RSC_32BIT_INDEX);

        // Check for hardware occlusion support
        if (mGLUtil->CheckExtension("GL_EXT_occlusion_query_boolean") || gleswIsSupported(3, 0))
        {
            rsc->setCapability(RSC_HWOCCLUSION);
        }

        // OpenGL ES - Check for these extensions too
        // For 2.0, http://www.khronos.org/registry/gles/api/2.0/gl2ext.h

        if (mGLUtil->CheckExtension("GL_IMG_texture_compression_pvrtc") ||
            mGLUtil->CheckExtension("GL_EXT_texture_compression_dxt1") ||
            mGLUtil->CheckExtension("GL_EXT_texture_compression_s3tc") ||
            mGLUtil->CheckExtension("GL_OES_compressed_ETC1_RGB8_texture") ||
            mGLUtil->CheckExtension("GL_AMD_compressed_ATC_texture"))
        {
            rsc->setCapability(RSC_TEXTURE_COMPRESSION);

            if (mGLUtil->CheckExtension("GL_IMG_texture_compression_pvrtc") ||
                mGLUtil->CheckExtension("GL_IMG_texture_compression_pvrtc2"))
                rsc->setCapability(RSC_TEXTURE_COMPRESSION_PVRTC);

            if (mGLUtil->CheckExtension("GL_EXT_texture_compression_dxt1") &&
                mGLUtil->CheckExtension("GL_EXT_texture_compression_s3tc"))
                rsc->setCapability(RSC_TEXTURE_COMPRESSION_DXT);

            if (mGLUtil->CheckExtension("GL_OES_compressed_ETC1_RGB8_texture"))
                rsc->setCapability(RSC_TEXTURE_COMPRESSION_ETC1);

            if (gleswIsSupported(3, 0))
                rsc->setCapability(RSC_TEXTURE_COMPRESSION_ETC2);

            if (mGLUtil->CheckExtension("GL_AMD_compressed_ATC_texture"))
                rsc->setCapability(RSC_TEXTURE_COMPRESSION_ATC);
        }

        if (mGLUtil->CheckExtension("GL_EXT_texture_filter_anisotropic"))
            rsc->setCapability(RSC_ANISOTROPY);

        rsc->setCapability(RSC_FBO);
        rsc->setCapability(RSC_HWRENDER_TO_TEXTURE);
        rsc->setNumMultiRenderTargets(1);

        // Cube map
        rsc->setCapability(RSC_CUBEMAPPING);

        // Stencil wrapping
        rsc->setCapability(RSC_STENCIL_WRAP);

        // GL always shares vertex and fragment texture units (for now?)
        rsc->setVertexTextureUnitsShared(true);

        // Hardware support mipmapping
        rsc->setCapability(RSC_AUTOMIPMAP);

        // Blending support
        rsc->setCapability(RSC_BLENDING);
        rsc->setCapability(RSC_ADVANCED_BLEND_OPERATIONS);

        // DOT3 support is standard
        rsc->setCapability(RSC_DOT3);

        // Point size
        GLfloat psRange[2] = { 0.0, 0.0 };
        CHECK_GL_ERROR(glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, psRange));
        rsc->setMaxPointSize(psRange[1]);

        if (mGLUtil->CheckExtension("GL_EXT_texture_filter_anisotropic"))
        {
            // Max anisotropy
            GLfloat aniso = 0;
            CHECK_GL_ERROR(glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso));
            rsc->setMaxSupportedAnisotropy(aniso);
        }

        // Point sprites
        rsc->setCapability(RSC_POINT_SPRITES);
        rsc->setCapability(RSC_POINT_EXTENDED_PARAMETERS);

        // GLSL ES is always supported in GL ES 2
        rsc->addShaderProfile("glsles");
        DI_LOG("GLSL ES support detected");


        // UBYTE4 always supported
        rsc->setCapability(RSC_VERTEX_FORMAT_UBYTE4);

        // Infinite far plane always supported
        rsc->setCapability(RSC_INFINITE_FAR_PLANE);

        // Vertex/Fragment Programs
        rsc->setCapability(RSC_VERTEX_PROGRAM);
        rsc->setCapability(RSC_FRAGMENT_PROGRAM);

        // Separate shader objects
        if (mGLUtil->CheckExtension("GL_EXT_separate_shader_objects"))
            rsc->setCapability(RSC_SEPARATE_SHADER_OBJECTS);

        // Separate shader objects don't work properly on Tegra
        if (rsc->getVendor() == GPU_NVIDIA)
            rsc->unsetCapability(RSC_SEPARATE_SHADER_OBJECTS);

        GLfloat floatConstantCount = 0;
        glGetFloatv(GL_MAX_VERTEX_UNIFORM_VECTORS, &floatConstantCount);

        rsc->setVertexProgramConstantFloatCount((uint16)floatConstantCount);
        rsc->setVertexProgramConstantBoolCount((uint16)floatConstantCount);
        rsc->setVertexProgramConstantIntCount((uint16)floatConstantCount);

        // Fragment Program Properties
        floatConstantCount = 0;
        glGetFloatv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &floatConstantCount);
        rsc->setFragmentProgramConstantFloatCount((uint16)floatConstantCount);
        rsc->setFragmentProgramConstantBoolCount((uint16)floatConstantCount);
        rsc->setFragmentProgramConstantIntCount((uint16)floatConstantCount);

        // Check for Float textures
        if (mGLUtil->CheckExtension("GL_OES_texture_float") || mGLUtil->CheckExtension("GL_OES_texture_half_float") || gleswIsSupported(3, 0))
            rsc->setCapability(RSC_TEXTURE_FLOAT);

        rsc->setCapability(RSC_TEXTURE_1D);

        // ES 3 always supports NPOT textures
        if (mGLUtil->CheckExtension("GL_OES_texture_npot") || mGLUtil->CheckExtension("GL_ARB_texture_non_power_of_two") || gleswIsSupported(3, 0))
        {
            rsc->setCapability(RSC_NON_POWER_OF_2_TEXTURES);
            rsc->setNonPOW2TexturesLimited(false);
        }
        else
        {
            rsc->setNonPOW2TexturesLimited(true);
        }

        // Alpha to coverage always 'supported' when MSAA is available
        // although card may ignore it if it doesn't specifically support A2C
        rsc->setCapability(RSC_ALPHA_TO_COVERAGE);

        // No point sprites, so no size
        rsc->setMaxPointSize(0.f);

        if (mGLUtil->CheckExtension("GL_OES_vertex_array_object") || gleswIsSupported(3, 0))
            rsc->setCapability(RSC_VAO);

        if (mGLUtil->CheckExtension("GL_EXT_instanced_arrays") || gleswIsSupported(3, 0))
        {
            rsc->setCapability(RSC_VERTEX_BUFFER_INSTANCE_DATA);
        }

        rsc->LogCaps();

        return rsc;
    }

    bool DiGLES2Driver::RenderGeometry(DiRenderUnit* unit)
    {
        if (!_BindVertexBuffer(unit))
            return false;
        
        GLenum polyMode = StateCache->getPolygonMode();

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
            CHECK_GL_ERROR(glDrawArrays(polyMode==GL_FILL?primType:polyMode, 0, unit->mVerticesNum));
        }
        else
        {
            unit->mIndexBuffer->Bind();
            GLenum indexType = unit->mIndexBuffer->GetType() == IB_16BITS ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
            uint32 indexSize = unit->mIndexBuffer->GetType() == IB_16BITS ? 16 : 32;

            void* pBufferData = nullptr;
            pBufferData = VBO_BUFFER_OFFSET(unit->mIndexOffset * indexSize);

            CHECK_GL_ERROR(glDrawElements(polyMode==GL_FILL?primType:polyMode, unit->mIndexBuffer->GetMaxIndices(), indexType, pBufferData));
        }

        for (auto ai = mRenderAttribsBound.begin(); ai != mRenderAttribsBound.end(); ++ai)
            StateCache->setVertexAttribDisabled(*ai);
        mRenderAttribsBound.clear();

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
        CHECK_GL_ERROR(glViewport(x, y, w, h));
        CHECK_GL_ERROR(glScissor(x, y, w, h));
    }

    void DiGLES2Driver::SetFillMode(DiFillMode mode)
    {
        switch (mode)
        {
        case FM_SOLID:
            StateCache->setPolygonMode(GL_FILL);
            break;
        case FM_WIREFRAME:
            StateCache->setPolygonMode(GL_LINE_STRIP);
            break;
        case FM_POINT:
            StateCache->setPolygonMode(GL_POINTS);
            break;
        }
    }

    DiTextureDrv* DiGLES2Driver::CreateTextureDriver(DiTexture* texture)
    {
        return DI_NEW DiGLES2TextureDrv(texture);
    }

    DiIndexBuffer* DiGLES2Driver::CreateIndexBuffer()
    {
        return DI_NEW DiGLES2IndexBuffer();
    }

    DiVertexBuffer* DiGLES2Driver::CreateVertexBuffer()
    {
        return DI_NEW DiGLES2VertexBuffer();
    }

    DiVertexDeclaration* DiGLES2Driver::CreateVertexDeclaration()
    {
        return DI_NEW DiGLES2VertexDeclaration();
    }

    DiShaderInstance* DiGLES2Driver::CreateVSInstance(DiShaderProgram* prog)
    {
        return DI_NEW DiGLES2ShaderInstance(SHADER_VERTEX, prog);
    }

    DiShaderInstance* DiGLES2Driver::CreatePSInstance(DiShaderProgram* prog)
    {
        return DI_NEW DiGLES2ShaderInstance(SHADER_PIXEL, prog);
    }

    DiRenderTarget* DiGLES2Driver::CreateRenderTarget()
    {
        auto rt = DI_NEW DiGLES2RenderTarget();
        rt->Init();
        return rt;
    }

    void DiGLES2Driver::CreateWindowTarget(DiRenderTarget*& outRT, DiWndHandle wnd)
    {
        auto wt = DI_NEW DiGLES2WindowTarget();
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

        StateCache->setPolygonMode(finalwireframe ? GL_LINE_STRIP : GL_FILL);

        // culling
        bool flip = ((mNeedTextureFlipping && !mInvertVertexWinding) ||
                     (!mNeedTextureFlipping && mInvertVertexWinding));
        if (mat->GetCullMode() == CULL_NONE)
        {
            StateCache->setDisabled(GL_CULL_FACE);
        }
        else
        {
            StateCache->setEnabled(GL_CULL_FACE);
            
            if (mat->GetCullMode() == CULL_CCW)
                flip = !flip;
            if (flip)
                StateCache->setCullFace(GL_FRONT);
            else
                StateCache->setCullFace(GL_BACK);
        }

        // depth check/write
        if (mat->GetDepthCheck())
        {
            StateCache->setClearDepth(1.0f);
            StateCache->setEnabled(GL_DEPTH_TEST);
        }
        else
            StateCache->setDisabled(GL_DEPTH_TEST);

        StateCache->setDepthMask(mat->GetDepthWrite() ? GL_TRUE : GL_FALSE);
        
        StateCache->setDepthFunc(GL_LEQUAL);

        // blending mode
        switch(mat->GetBlendMode())
        {
        case BLEND_REPLACE:
            StateCache->setDisabled(GL_BLEND);
            break;
        case BLEND_ADD:
            StateCache->setEnabled(GL_BLEND);
            StateCache->setBlendFunc(GL_ONE, GL_ONE);
            break;
        case BLEND_MULTIPLY:
            StateCache->setEnabled(GL_BLEND);
            StateCache->setBlendFunc(GL_DST_COLOR, GL_ZERO);
            break;
        case BLEND_ALPHA:
            StateCache->setEnabled(GL_BLEND);
            StateCache->setBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
            break;
        case BLEND_TRANS_COLOR:
            StateCache->setEnabled(GL_BLEND);
            StateCache->setBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
            break;
        case  BLEND_ONE_INV_ALPHA:
            StateCache->setBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            break;
        default:
            StateCache->setDisabled(GL_BLEND);
        }

        GLint func = GL_FUNC_ADD;
        StateCache->setBlendEquation(func);
    }

    void DiGLES2Driver::Clear(uint32 flag, const DiColor& col, float depth, unsigned short stencil /*= 0*/)
    {
        bool colourMask = !mColourWrite[0] || !mColourWrite[1]
            || !mColourWrite[2] || !mColourWrite[3];
        GLuint stencilMask = StateCache->getStencilMask();

        GLbitfield clearBuf = 0;
        if (flag & CLEAR_COLOR)
        {
            clearBuf |= GL_COLOR_BUFFER_BIT;
            if (colourMask)
                StateCache->setColourMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            StateCache->setClearColour(col.r, col.g, col.b, col.a);
        }
        
        if (flag & CLEAR_DEPTH)            
        {
            clearBuf |= GL_DEPTH_BUFFER_BIT;

            StateCache->setDepthMask(GL_TRUE);
            StateCache->setClearDepth(depth);
        }

        if (flag & CLEAR_STENCIL)
        {
            clearBuf |= GL_STENCIL_BUFFER_BIT;

            StateCache->setStencilMask(0xFFFFFFFF);
            CHECK_GL_ERROR(glClearStencil(stencil));
        }

        GLboolean scissorTestEnabled = glIsEnabled(GL_SCISSOR_TEST);
        if (!scissorTestEnabled)
            StateCache->setEnabled(GL_SCISSOR_TEST);

        // Sets the scissor box as same as viewport
        GLint viewport[4] = { 0, 0, 0, 0 };
        GLint scissor[4] = { 0, 0, 0, 0 };
        CHECK_GL_ERROR(glGetIntegerv(GL_VIEWPORT, viewport));
        CHECK_GL_ERROR(glGetIntegerv(GL_SCISSOR_BOX, scissor));
        bool scissorBoxDifference =
            viewport[0] != scissor[0] || viewport[1] != scissor[1] ||
            viewport[2] != scissor[2] || viewport[3] != scissor[3];
        if (scissorBoxDifference)
            CHECK_GL_ERROR(glScissor(viewport[0], viewport[1], viewport[2], viewport[3]));

        StateCache->setDiscardBuffers(flag);

        CHECK_GL_ERROR(glClear(clearBuf));

        // Restore scissor box
        if (scissorBoxDifference)
        {
            CHECK_GL_ERROR(glScissor(scissor[0], scissor[1], scissor[2], scissor[3]));
        }

        // Restore scissor test
        StateCache->setDisabled(GL_SCISSOR_TEST);

        // Reset buffer write state
        if (!StateCache->getDepthMask() && (flag & CLEAR_DEPTH))
        {
            StateCache->setDepthMask(GL_FALSE);
        }

        if (colourMask && (flag & CLEAR_COLOR))
        {
            StateCache->setColourMask(mColourWrite[0], mColourWrite[1], mColourWrite[2], mColourWrite[3]);
        }

        if (flag & CLEAR_STENCIL)
        {
            StateCache->setStencilMask(stencilMask);
        }
    }

    DiWindow* DiGLES2Driver::CreateWnd()
    {
        return mGLUtil->CreateNewWindow();
    }

    DiGLES2Util* DiGLES2Driver::_CreateGLUtil()
    {
        DiGLES2Util* ret = nullptr;
#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
        ret = DI_NEW DiWin32EGLUtil();
#elif DEMI_PLATFORM == DEMI_PLATFORM_IOS
        ret = DI_NEW DiEAGL2Util();
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

        DiVertexElements& elements = unit->mVertexDecl->GetElements();

        // sources
        for (auto it = unit->mSourceData.begin(); it != unit->mSourceData.end(); ++it)
        {
            DiGLES2VertexBuffer* vb = static_cast<DiGLES2VertexBuffer*>(*it);
            vb->Bind();
            GLsizei stride = static_cast<GLsizei>(vb->GetStride());

            DiVertexElements::ElementsList e;
            elements.GetElementsAtStream(vb->GetStreamId(), e);

            for (auto i = e.begin(); i != e.end(); ++i)
            {
                void* pBufferData = nullptr;
                pBufferData = VBO_BUFFER_OFFSET(i->Offset);

                auto gltype  = DiGLTypeMappings::GetGLType((DiVertexType)i->Type);

                GLint attrib = -1;
                
                if (mActiveShaderPipeline)
                    attrib = mActiveShaderPipeline->GetAttributeIndex((DiVertexUsage)i->Usage, i->UsageIndex);
                
                if(attrib < 0)
                    continue;

                uint16 count = elements.GetElementTypeCount((DiVertexType)i->Type);

                GLboolean normalised = GL_FALSE;
                if(i->Type == VERT_TYPE_COLOR)
                    normalised = GL_TRUE;

                CHECK_GL_ERROR(glVertexAttribPointer(attrib, count, gltype, normalised, stride, pBufferData));
                StateCache->setVertexAttribEnabled(attrib);
                mRenderAttribsBound.push_back(attrib);
            }
        }

        return true;
    }

    void DiGLES2Driver::_InitMainContext(DiGLES2Context* context)
    {
        mMainContext = context;
        mCurrentContext = mMainContext;

        if (mCurrentContext)
            mCurrentContext->BeginContext();

        mGLUtil->InitExtensions();
    }

    void DiGLES2Driver::GetDepthStencilFormatFor(DiPixelFormat format,
        GLenum *depthFormat, GLenum *stencilFormat)
    {
        mGLFBOManager->GetBestDepthStencil(format, depthFormat, stencilFormat);
    }

    DiGLES2Context* DiGLES2Driver::GetContext(DiWndHandle wnd)
    {
        return mContextMap[wnd];
    }

    void DiGLES2Driver::BindShaders(DiShaderProgram* vs, DiShaderProgram* ps)
    {
        auto* prog = GetShaderLinker(vs->GetShader(), ps->GetShader());
        prog->Bind();
        mActiveShaderPipeline = prog;
    }

#ifdef GLES2_USE_PIPELINE

    DiGLES2ShaderPipeline* DiGLES2Driver::GetShaderLinker(DiShaderInstance* vs, DiShaderInstance* ps)
    {
        auto p = DiPair<DiShaderInstance*, DiShaderInstance*>(vs, ps);
        auto it = mProgramMaps.find(p);
        if (it != mProgramMaps.end())
            return it->second;

        // create a new one
        DiGLES2ShaderPipeline* ret = DI_NEW DiGLES2ShaderPipeline(static_cast<DiGLES2ShaderInstance*>(vs),
            static_cast<DiGLES2ShaderInstance*>(ps));

        mProgramMaps[p] = ret;
        return ret;
    }

#else

    DiGLES2ShaderLinker* DiGLES2Driver::GetShaderLinker(DiShaderInstance* vs, DiShaderInstance* ps)
    {
        auto p = DiPair<DiShaderInstance*, DiShaderInstance*>(vs, ps);
        auto it = mProgramMaps.find(p);
        if (it != mProgramMaps.end())
            return it->second;

        // create a new one
        DiGLES2ShaderLinker* ret = DI_NEW DiGLES2ShaderLinker(static_cast<DiGLES2ShaderInstance*>(vs),
            static_cast<DiGLES2ShaderInstance*>(ps));

        mProgramMaps[p] = ret;
        return ret;
    }

#endif

    DiShaderParameter* DiGLES2Driver::CreateShaderParams(DiMaterial& mat)
    {
        return DI_NEW DiGLES2ShaderParam(mat);
    }

    const DiString& DiGLES2Driver::GetShaderFileExtension() const
    {
        static DiString shaderext = ".glsl";
        return shaderext;
    }

    DiString& DiGLES2Driver::GetGlslVersion()
    {
        static DiString version = "100";
        return version;
    }

    void DiGLES2Driver::SetDepthBias(float constantBias, float slopeScaledBias)
    {
        if (constantBias != 0 || slopeScaledBias != 0)
        {
            StateCache->setEnabled(GL_POLYGON_OFFSET_FILL);
            CHECK_GL_ERROR(glPolygonOffset(-slopeScaledBias, -constantBias));
        }
        else
        {
            StateCache->setDisabled(GL_POLYGON_OFFSET_FILL);
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