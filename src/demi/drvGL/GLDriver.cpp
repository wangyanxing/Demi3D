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
#include "DrvGLPch.h"
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

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
#   include "Win32/Win32Window.h"
#   include "Win32/Win32GLContext.h"
#   include "Win32/Win32GLUtil.h"
#elif DEMI_PLATFORM == DEMI_PLATFORM_OSX
#   include "OSX/OSXGLUtil.h"
#endif

// Convenience macro from ARB_vertex_buffer_object spec
#define VBO_BUFFER_OFFSET(i) ((char *)NULL + (i))

GLenum glewContextInit(DiGLUtil*);

namespace Demi
{
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
        mGLUtil = _CreateGLUtil();
    }
    
    DiGLDriver::~DiGLDriver()
    {
    }

    bool DiGLDriver::InitGfx(DiWndHandle wnd)
    {
        DI_INFO("OpenGL driver is intializing.");

        _InitMainContext(_CreateContext(wnd));

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

        mGLFBOManager = DI_NEW DiGLFBOManager(true);    // TODO: Check atimode

        return true;
    }

    bool DiGLDriver::InitGfx(uint16 width, uint16 height, bool fullscreen)
    {
        DI_INFO("Creating window: %d x %d, %s mode.", width, height, fullscreen?"full screen":"window");

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

        DI_INFO("OpenGL stuff successfully released.");
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
        if (!_BindVertexBuffer(unit))
            return false;

        GLint primType = 0;
        GLint indicesCount = 0;
        switch (unit->mPrimitiveType)
        {
        case PT_POINTLIST:
            primType = GL_POINTS;
            indicesCount = unit->mPrimitiveCount;
            break;
        case PT_LINELIST:
            primType = GL_LINES;
            indicesCount = unit->mPrimitiveCount * 2;
            break;
        case PT_LINESTRIP:
            primType = GL_LINE_STRIP;
            indicesCount = unit->mPrimitiveCount + 1;
            break;
        case PT_TRIANGLELIST:
            primType = GL_TRIANGLES;
            indicesCount = unit->mPrimitiveCount * 3;
            break;
        case PT_TRIANGLESTRIP:
            primType = GL_TRIANGLE_STRIP;
            indicesCount = unit->mPrimitiveCount + 2;
            break;
        case PT_TRIANGLEFAN:
            primType = GL_TRIANGLE_FAN;
            indicesCount = unit->mPrimitiveCount + 2;
            break;
        default:
            primType = GL_TRIANGLES;
            DI_WARNING("Invalid primitive type");
        }

        if (!unit->mIndexBuffer)
        {
            //DI_DEBUG("Calling glDrawArrays :%d",unit->mVerticesNum);
            glDrawArrays(primType, 0, unit->mVerticesNum);
        }
        else
        {
            unit->mIndexBuffer->Bind();
            GLenum indexType = unit->mIndexBuffer->GetType() == IB_16BITS ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
            uint32 indexSize = unit->mIndexBuffer->GetType() == IB_16BITS ? sizeof(unsigned short) : sizeof(unsigned int);

            void* pBufferData = VBO_BUFFER_OFFSET(unit->mIndexOffset * indexSize);

            //DI_DEBUG("Calling glDrawElements :%d,%d",indicesCount, indexType);
            glDrawElements(primType, indicesCount, indexType, pBufferData);
        }

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

    void DiGLDriver::BindMaterialStates(const DiMaterial* mat)
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
        DiBlendMode blendmode = mat->GetBlendMode();
        switch(blendmode)
        {
        case BLEND_REPLACE:
            {
            glDisable(GL_BLEND);
            }
            break;
        case BLEND_ADD:
            {
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);
            }
            break;
        case BLEND_MULTIPLY:
            {
            glEnable(GL_BLEND);
            glBlendFunc(GL_DST_COLOR, GL_ZERO);
            }
            break;
        case BLEND_ALPHA:
            {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            break;
        case BLEND_TRANS_COLOR:
            {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
            }
            break;
        case BLEND_ONE_INV_ALPHA:
            {
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            }
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

        //DI_DEBUG("Calling glClear");
        glClear(clearBuf);

#if 0
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
        return mGLUtil->CreateNewWindow();
    }

    DiGLContext* DiGLDriver::_CreateContext(DiWndHandle wnd)
    {
        DiGLContext* ret = nullptr;
#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
        ret = DI_NEW DiWin32GLContext(static_cast<DiWin32GLUtil*>(mGLUtil), wnd);
        DI_INFO("Win32 GL context created.");
#elif DEMI_PLATFORM == DEMI_PLATFORM_OSX
        //ret = DI_NEW DiOSXCocoaContext(static_cast<DiOSXGLUtil*>(mGLUtil), wnd);
        DI_INFO("OSX GL context created.");
#endif
        mContextMap[wnd] = ret;
        return ret;
    }

    DiGLUtil* DiGLDriver::_CreateGLUtil()
    {
        DiGLUtil* ret = nullptr;
#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
        ret = DI_NEW DiWin32GLUtil();
#elif DEMI_PLATFORM == DEMI_PLATFORM_OSX
        ret = DI_NEW DiOSXGLUtil();
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

    void DiGLDriver::_InitMainContext(DiGLContext* context)
    {
        mMainContext = context;
        mCurrentContext = mMainContext;

        if (mCurrentContext)
            mCurrentContext->BeginContext();

        mGLUtil->InitExtensions();

        glewContextInit(mGLUtil);
    }

    void DiGLDriver::GetDepthStencilFormatFor(DiPixelFormat format,
        GLenum *depthFormat, GLenum *stencilFormat)
    {
        mGLFBOManager->GetBestDepthStencil(format, depthFormat, stencilFormat);
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

    DiString& DiGLDriver::GetGlslVersion()
    {
        static DiString version = "120";
        return version;
    }

    void DiGLDriver::SetDepthBias(float constantBias, float slopeScaledBias)
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

    DiPair<float, float> DiGLDriver::GetDepthInputRange() const
    {
        return DiPair<float, float>(-1.0f, 1.0f);
    }

    DiGfxCaps* DiGLDriver::InitGfxCaps()
    {
        DiGfxCaps* rsc = DI_NEW DiGfxCaps();
        mCaps = rsc;

        rsc->setCategoryRelevant(CAPS_CATEGORY_GL, true);
        rsc->setDriverVersion(mDriverVersion);
        const char* deviceName = (const char*)glGetString(GL_RENDERER);
        const char* vendorName = (const char*)glGetString(GL_VENDOR);
        rsc->setDeviceName(deviceName);
        rsc->setRenderSystemName(DRV_OPENGL);

        // determine vendor
        if (strstr(vendorName, "NVIDIA"))
            rsc->setVendor(GPU_NVIDIA);
        else if (strstr(vendorName, "ATI"))
            rsc->setVendor(GPU_AMD);
        else if (strstr(vendorName, "AMD"))
            rsc->setVendor(GPU_AMD);
        else if (strstr(vendorName, "Intel"))
            rsc->setVendor(GPU_INTEL);
        else if (strstr(vendorName, "S3"))
            rsc->setVendor(GPU_S3);
        else if (strstr(vendorName, "Matrox"))
            rsc->setVendor(GPU_MATROX);
        else if (strstr(vendorName, "3DLabs"))
            rsc->setVendor(GPU_3DLABS);
        else if (strstr(vendorName, "SiS"))
            rsc->setVendor(GPU_SIS);
        else
            rsc->setVendor(GPU_UNKNOWN);

        // Check for hardware mipmapping support.
        if (GLEW_VERSION_1_4 || GLEW_SGIS_generate_mipmap)
        {
            bool disableAutoMip = false;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE || OGRE_PLATFORM == OGRE_PLATFORM_LINUX
            // Apple & Linux ATI drivers have faults in hardware mipmap generation
            if (rsc->getVendor() == GPU_AMD)
                disableAutoMip = true;
#endif
            // The Intel 915G frequently corrupts textures when using hardware mip generation
            // I'm not currently sure how many generations of hardware this affects,
            // so for now, be safe.
            if (rsc->getVendor() == GPU_INTEL)
                disableAutoMip = true;

            // SiS chipsets also seem to have problems with this
            if (rsc->getVendor() == GPU_SIS)
                disableAutoMip = true;

            if (!disableAutoMip)
                rsc->setCapability(RSC_AUTOMIPMAP);
        }

        // Check for blending support
        if (GLEW_VERSION_1_3 ||
            GLEW_ARB_texture_env_combine ||
            GLEW_EXT_texture_env_combine)
        {
            rsc->setCapability(RSC_BLENDING);
        }

        // Check for Multitexturing support and set number of texture units
        if (GLEW_VERSION_1_3 ||
            GLEW_ARB_multitexture)
        {
            GLint units;
            glGetIntegerv(GL_MAX_TEXTURE_UNITS, &units);

            if (GLEW_ARB_fragment_program)
            {
                // Also check GL_MAX_TEXTURE_IMAGE_UNITS_ARB since NV at least
                // only increased this on the FX/6x00 series
                GLint arbUnits;
                glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &arbUnits);
                if (arbUnits > units)
                    units = arbUnits;
            }
            rsc->setNumTextureUnits(std::min<uint16>(16, units));
        }
        else
        {
            // If no multitexture support then set one texture unit
            rsc->setNumTextureUnits(1);
        }

        // Check for Anisotropy support
        if (GLEW_EXT_texture_filter_anisotropic)
        {
            rsc->setCapability(RSC_ANISOTROPY);
        }

        // Check for DOT3 support
        if (GLEW_VERSION_1_3 ||
            GLEW_ARB_texture_env_dot3 ||
            GLEW_EXT_texture_env_dot3)
        {
            rsc->setCapability(RSC_DOT3);
        }

        // Check for cube mapping
        if (GLEW_VERSION_1_3 ||
            GLEW_ARB_texture_cube_map ||
            GLEW_EXT_texture_cube_map)
        {
            rsc->setCapability(RSC_CUBEMAPPING);
        }


        // Point sprites
        if (GLEW_VERSION_2_0 || GLEW_ARB_point_sprite)
        {
            rsc->setCapability(RSC_POINT_SPRITES);
        }
        // Check for point parameters
        if (GLEW_VERSION_1_4)
        {
            rsc->setCapability(RSC_POINT_EXTENDED_PARAMETERS);
        }
        if (GLEW_ARB_point_parameters)
        {
            rsc->setCapability(RSC_POINT_EXTENDED_PARAMETERS_ARB);
        }
        if (GLEW_EXT_point_parameters)
        {
            rsc->setCapability(RSC_POINT_EXTENDED_PARAMETERS_EXT);
        }

        // Check for hardware stencil support and set bit depth
        GLint stencil;
        glGetIntegerv(GL_STENCIL_BITS, &stencil);

        if (stencil)
        {
            rsc->setCapability(RSC_HWSTENCIL);
            rsc->setStencilBufferBitDepth(stencil);
        }


        if (GLEW_VERSION_1_5 || GLEW_ARB_vertex_buffer_object)
        {
            if (!GLEW_ARB_vertex_buffer_object)
            {
                rsc->setCapability(RSC_GL1_5_NOVBO);
            }
            rsc->setCapability(RSC_VBO);
            rsc->setCapability(RSC_32BIT_INDEX);
        }

        if (GLEW_ARB_vertex_program)
        {
            rsc->setCapability(RSC_VERTEX_PROGRAM);

            // Vertex Program Properties
            rsc->setVertexProgramConstantBoolCount(0);
            rsc->setVertexProgramConstantIntCount(0);

            GLint floatConstantCount;
            glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB, &floatConstantCount);
            rsc->setVertexProgramConstantFloatCount(floatConstantCount);

            rsc->addShaderProfile("arbvp1");
            if (GLEW_NV_vertex_program2_option)
            {
                rsc->addShaderProfile("vp30");
            }

            if (GLEW_NV_vertex_program3)
            {
                rsc->addShaderProfile("vp40");
            }

            if (GLEW_NV_vertex_program4)
            {
                rsc->addShaderProfile("gp4vp");
                rsc->addShaderProfile("gpu_vp");
            }
        }

        if (GLEW_NV_register_combiners2 &&
            GLEW_NV_texture_shader)
        {
            rsc->setCapability(RSC_FRAGMENT_PROGRAM);
            rsc->addShaderProfile("fp20");
        }

        // NFZ - check for ATI fragment shader support
        if (GLEW_ATI_fragment_shader)
        {
            rsc->setCapability(RSC_FRAGMENT_PROGRAM);
            // no boolean params allowed
            rsc->setFragmentProgramConstantBoolCount(0);
            // no integer params allowed
            rsc->setFragmentProgramConstantIntCount(0);

            // only 8 Vector4 constant floats supported
            rsc->setFragmentProgramConstantFloatCount(8);

            rsc->addShaderProfile("ps_1_4");
            rsc->addShaderProfile("ps_1_3");
            rsc->addShaderProfile("ps_1_2");
            rsc->addShaderProfile("ps_1_1");
        }

        if (GLEW_ARB_fragment_program)
        {
            rsc->setCapability(RSC_FRAGMENT_PROGRAM);

            // Fragment Program Properties
            rsc->setFragmentProgramConstantBoolCount(0);
            rsc->setFragmentProgramConstantIntCount(0);

            GLint floatConstantCount;
            glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB, &floatConstantCount);
            rsc->setFragmentProgramConstantFloatCount(floatConstantCount);

            rsc->addShaderProfile("arbfp1");
            if (GLEW_NV_fragment_program_option)
            {
                rsc->addShaderProfile("fp30");
            }

            if (GLEW_NV_fragment_program2)
            {
                rsc->addShaderProfile("fp40");
            }

            if (GLEW_NV_fragment_program4)
            {
                rsc->addShaderProfile("gp4fp");
                rsc->addShaderProfile("gpu_fp");
            }
        }

        DiString shadingLangVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
        auto tokens = shadingLangVersion.Tokenize(". ");
        int shaderVersion = tokens[0].AsInt() * 100 + tokens[1].AsInt();

        // NFZ - Check if GLSL is supported
        if (GLEW_VERSION_2_0 ||
            (GLEW_ARB_shading_language_100 &&
            GLEW_ARB_shader_objects &&
            GLEW_ARB_fragment_shader &&
            GLEW_ARB_vertex_shader))
        {
            rsc->addShaderProfile("glsl");
            if (shaderVersion >= 120)
                rsc->addShaderProfile("glsl120");
            if (shaderVersion >= 110)
                rsc->addShaderProfile("glsl110");
            if (shaderVersion >= 100)
                rsc->addShaderProfile("glsl100");
        }

        // Check if geometry shaders are supported
        if (GLEW_VERSION_2_0 &&
            GLEW_EXT_geometry_shader4)
        {
            rsc->setCapability(RSC_GEOMETRY_PROGRAM);
            rsc->addShaderProfile("nvgp4");

            //Also add the CG profiles
            rsc->addShaderProfile("gpu_gp");
            rsc->addShaderProfile("gp4gp");

            rsc->setGeometryProgramConstantBoolCount(0);
            rsc->setGeometryProgramConstantIntCount(0);

            GLint floatConstantCount = 0;
            glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_EXT, &floatConstantCount);
            rsc->setGeometryProgramConstantFloatCount(floatConstantCount);

            GLint maxOutputVertices;
            glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &maxOutputVertices);
            rsc->setGeometryProgramNumOutputVertices(maxOutputVertices);
        }

        if (mGLUtil->CheckExtension("GL_ARB_get_program_binary"))
        {
            // states 3.0 here: http://developer.download.nvidia.com/opengl/specs/GL_ARB_get_program_binary.txt
            // but not here: http://www.opengl.org/sdk/docs/man4/xhtml/glGetProgramBinary.xml
            // and here states 4.1: http://www.geeks3d.com/20100727/opengl-4-1-allows-the-use-of-binary-shaders/
            GLint formats;
            glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);

            if (formats > 0)
                rsc->setCapability(RSC_CAN_GET_COMPILED_SHADER_BUFFER);
        }

        if (GLEW_VERSION_3_3 || GLEW_ARB_instanced_arrays)
        {
            // states 3.3 here: http://www.opengl.org/sdk/docs/man3/xhtml/glVertexAttribDivisor.xml
            rsc->setCapability(RSC_VERTEX_BUFFER_INSTANCE_DATA);
        }

        //Check if render to vertex buffer (transform feedback in OpenGL)
        if (GLEW_VERSION_2_0 &&
            GLEW_NV_transform_feedback)
        {
            rsc->setCapability(RSC_HWRENDER_TO_VERTEX_BUFFER);
        }

        // Check for texture compression
        if (GLEW_VERSION_1_3 || GLEW_ARB_texture_compression)
        {
            rsc->setCapability(RSC_TEXTURE_COMPRESSION);

            // Check for dxt compression
            if (GLEW_EXT_texture_compression_s3tc)
            {
#if defined(__APPLE__) && defined(__PPC__)
                // Apple on ATI & PPC has errors in DXT
                if (mGLSupport->getGLVendor().find("ATI") == std::string::npos)
#endif
                    rsc->setCapability(RSC_TEXTURE_COMPRESSION_DXT);
            }
            // Check for vtc compression
            if (GLEW_NV_texture_compression_vtc)
            {
                rsc->setCapability(RSC_TEXTURE_COMPRESSION_VTC);
            }
        }

        // Scissor test is standard in GL 1.2 (is it emulated on some cards though?)
        rsc->setCapability(RSC_SCISSOR_TEST);
        // As are user clipping planes
        rsc->setCapability(RSC_USER_CLIP_PLANES);

        // 2-sided stencil?
        if (GLEW_VERSION_2_0 || GLEW_EXT_stencil_two_side)
        {
            rsc->setCapability(RSC_TWO_SIDED_STENCIL);
        }
        // stencil wrapping?
        if (GLEW_VERSION_1_4 || GLEW_EXT_stencil_wrap)
        {
            rsc->setCapability(RSC_STENCIL_WRAP);
        }

        // Check for hardware occlusion support
        if (GLEW_VERSION_1_5 || GLEW_ARB_occlusion_query)
        {
            // Some buggy driver claim that it is GL 1.5 compliant and
            // not support ARB_occlusion_query
            if (!GLEW_ARB_occlusion_query)
            {
                rsc->setCapability(RSC_GL1_5_NOHWOCCLUSION);
            }

            rsc->setCapability(RSC_HWOCCLUSION);
        }
        else if (GLEW_NV_occlusion_query)
        {
            // Support NV extension too for old hardware
            rsc->setCapability(RSC_HWOCCLUSION);
        }

        // UBYTE4 always supported
        rsc->setCapability(RSC_VERTEX_FORMAT_UBYTE4);

        // Infinite far plane always supported
        rsc->setCapability(RSC_INFINITE_FAR_PLANE);

        // Check for non-power-of-2 texture support
        if (GLEW_ARB_texture_non_power_of_two)
        {
            rsc->setCapability(RSC_NON_POWER_OF_2_TEXTURES);
        }

        // Check for Float textures
        if (GLEW_ATI_texture_float || GLEW_ARB_texture_float)
        {
            rsc->setCapability(RSC_TEXTURE_FLOAT);
        }

        // 3D textures should be supported by GL 1.2, which is our minimum version
        rsc->setCapability(RSC_TEXTURE_1D);
        rsc->setCapability(RSC_TEXTURE_3D);

        // Check for framebuffer object extension
        if (GLEW_EXT_framebuffer_object)
        {
            // Probe number of draw buffers
            // Only makes sense with FBO support, so probe here
            if (GLEW_VERSION_2_0 ||
                GLEW_ARB_draw_buffers ||
                GLEW_ATI_draw_buffers)
            {
                GLint buffers;
                glGetIntegerv(GL_MAX_DRAW_BUFFERS_ARB, &buffers);
                rsc->setNumMultiRenderTargets(std::min<int>(buffers, (GLint)MAX_MRT));
                rsc->setCapability(RSC_MRT_DIFFERENT_BIT_DEPTHS);
                if (!GLEW_VERSION_2_0)
                {
                    // Before GL version 2.0, we need to get one of the extensions
                    if (GLEW_ARB_draw_buffers)
                        rsc->setCapability(RSC_FBO_ARB);
                    if (GLEW_ATI_draw_buffers)
                        rsc->setCapability(RSC_FBO_ATI);
                }
                // Set FBO flag for all 3 'subtypes'
                rsc->setCapability(RSC_FBO);

            }
            rsc->setCapability(RSC_HWRENDER_TO_TEXTURE);
        }

        // Check GLSupport for PBuffer support
        if (mGLUtil->SupportsPBuffers())
        {
            // Use PBuffers
            rsc->setCapability(RSC_HWRENDER_TO_TEXTURE);
            rsc->setCapability(RSC_PBUFFER);
        }

        // Point size
        if (GLEW_VERSION_1_4)
        {
            float ps;
            glGetFloatv(GL_POINT_SIZE_MAX, &ps);
            rsc->setMaxPointSize(ps);
        }
        else
        {
            GLint vSize[2];
            glGetIntegerv(GL_POINT_SIZE_RANGE, vSize);
            rsc->setMaxPointSize(vSize[1]);
        }

        // Vertex texture fetching
        if (mGLUtil->CheckExtension("GL_ARB_vertex_shader"))
        {
            GLint vUnits;
            glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB, &vUnits);
            rsc->setNumVertexTextureUnits(static_cast<uint16>(vUnits));
            if (vUnits > 0)
            {
                rsc->setCapability(RSC_VERTEX_TEXTURE_FETCH);
            }
            // GL always shares vertex and fragment texture units (for now?)
            rsc->setVertexTextureUnitsShared(true);
        }

        // Mipmap LOD biasing?
        if (GLEW_VERSION_1_4 || GLEW_EXT_texture_lod_bias)
        {
            rsc->setCapability(RSC_MIPMAP_LOD_BIAS);
        }

        // Alpha to coverage?
        if (mGLUtil->CheckExtension("GL_ARB_multisample"))
        {
            // Alpha to coverage always 'supported' when MSAA is available
            // although card may ignore it if it doesn't specifically support A2C
            rsc->setCapability(RSC_ALPHA_TO_COVERAGE);
        }

        // Advanced blending operations
        if (GLEW_VERSION_2_0)
        {
            rsc->setCapability(RSC_ADVANCED_BLEND_OPERATIONS);
        }

        rsc->LogCaps();

        return rsc;
    }

    uint32 DiGLDriver::GetNativeColor(const DiColor& col)
    {
        return col.GetAsABGR();
    }

}