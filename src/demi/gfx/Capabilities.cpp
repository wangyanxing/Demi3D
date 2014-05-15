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

#include "GfxPch.h"
#include "Capabilities.h"
#include "GfxDriver.h"

/**
This file was copied from Ogre3D
http://www.ogre3d.org/
*/

namespace Demi
{
    static const char* _BoolToStr(bool v)
    {
        static DiString truestr  = "true";
        static DiString falsestr = "false";
        return v ? truestr.c_str() : falsestr.c_str();
    }

    DiGfxCaps::DiGfxCaps()
        : mVendor(GPU_UNKNOWN)
        , mNumWorldMatrices(0)
        , mNumTextureUnits(0)
        , mStencilBufferBitDepth(0)
        , mNumVertexBlendMatrices(0)
        , mNumMultiRenderTargets(1)
        , mNonPOW2TexturesLimited(false)
        , mMaxSupportedAnisotropy(0)
        , mVertexTextureUnitsShared(0)
        , mGeometryProgramNumOutputVertices(0)
    {
        for(int i = 0; i < CAPS_CATEGORY_COUNT; i++)
        {
            mCapabilities[i] = 0;
        }
        mCategoryRelevant[CAPS_CATEGORY_COMMON] = true;
        mCategoryRelevant[CAPS_CATEGORY_COMMON_2] = true;
        // each rendersystem should enable these
        mCategoryRelevant[CAPS_CATEGORY_D3D9] = false;
        mCategoryRelevant[CAPS_CATEGORY_GL] = false;
    }
    
    DiGfxCaps::~DiGfxCaps()
    {
    }
    
    void DiGfxCaps::LogCaps()
    {
        DI_INFO("-------------------------");
        DI_INFO("--Graphics capabilities--");
        DI_INFO("-------------------------");

        DI_INFO("GfxDriver: %s", getRenderSystemName().c_str());
        DI_INFO("GPU Vendor: %s", vendorToString(getVendor()).c_str());
        DI_INFO("Device Name: %s", getDeviceName().c_str());
        DI_INFO("Driver Version: %s", getDriverVersion().toString().c_str());

        DI_INFO(" * Hardware generation of mipmaps: %s", _BoolToStr(hasCapability(RSC_AUTOMIPMAP)));
        DI_INFO(" * Anisotropic texture filtering: %s", _BoolToStr(hasCapability(RSC_ANISOTROPY)));
        DI_INFO(" * Hardware stencil buffer: %s", _BoolToStr(hasCapability(RSC_HWSTENCIL)));

        if (hasCapability(RSC_HWSTENCIL))
        {
            DI_INFO("   - Stencil depth: %d", getStencilBufferBitDepth());
            DI_INFO("   - Two sided stencil support: %s", _BoolToStr(hasCapability(RSC_TWO_SIDED_STENCIL)));
            DI_INFO("   - Wrap stencil values: %s", _BoolToStr(hasCapability(RSC_STENCIL_WRAP)));
        }
        DI_INFO(" * 32-bit index buffers: %s", +_BoolToStr(hasCapability(RSC_32BIT_INDEX)));

        DI_INFO(" * Vertex shader: %s", _BoolToStr(hasCapability(RSC_VERTEX_PROGRAM)));
        DI_INFO(" * Number of float constants for vertex shader: %d", mVertexProgramConstantFloatCount);
        DI_INFO(" * Number of int constants for vertex shader: %d", mVertexProgramConstantIntCount);
        DI_INFO(" * Number of bool constants for vertex shader: %d", mVertexProgramConstantBoolCount);

        DI_INFO(" * Fragment shader: %s", _BoolToStr(hasCapability(RSC_FRAGMENT_PROGRAM)));
        DI_INFO(" * Number of float constants for fragment shader: %d", mFragmentProgramConstantFloatCount);
        DI_INFO(" * Number of int constants for fragment shader: %d", mFragmentProgramConstantIntCount);
        DI_INFO(" * Number of bool constants for fragment shader: %d", mFragmentProgramConstantBoolCount);

        DiString profileList = "";
        for(ShaderProfiles::iterator iter = mSupportedShaderProfiles.begin(), end = mSupportedShaderProfiles.end();
            iter != end; ++iter)
        {
            profileList += " " + *iter;
        }
        DI_INFO(" * Supported Shader Profiles: %s", profileList.c_str());

        DI_INFO(" * Texture Compression: ", _BoolToStr(hasCapability(RSC_TEXTURE_COMPRESSION)));
        if (hasCapability(RSC_TEXTURE_COMPRESSION))
        {
            DI_INFO("   - DXT: %s", _BoolToStr(hasCapability(RSC_TEXTURE_COMPRESSION_DXT)));
            DI_INFO("   - VTC: %s", _BoolToStr(hasCapability(RSC_TEXTURE_COMPRESSION_VTC)));
            DI_INFO("   - PVRTC: %s", _BoolToStr(hasCapability(RSC_TEXTURE_COMPRESSION_PVRTC)));
            DI_INFO("   - ATC: %s", _BoolToStr(hasCapability(RSC_TEXTURE_COMPRESSION_ATC)));
            DI_INFO("   - ETC1: %s", _BoolToStr(hasCapability(RSC_TEXTURE_COMPRESSION_ETC1)));
            DI_INFO("   - ETC2: %s", _BoolToStr(hasCapability(RSC_TEXTURE_COMPRESSION_ETC2)));
            DI_INFO("   - BC4/BC5: %s", _BoolToStr(hasCapability(RSC_TEXTURE_COMPRESSION_BC4_BC5)));
            DI_INFO("   - BC6H/BC7: %s", _BoolToStr(hasCapability(RSC_TEXTURE_COMPRESSION_BC6H_BC7)));
        }

        DI_INFO(" * Scissor Rectangle: %s", _BoolToStr(hasCapability(RSC_SCISSOR_TEST)));
        DI_INFO(" * Hardware Occlusion Query: %s", _BoolToStr(hasCapability(RSC_HWOCCLUSION)));
        DI_INFO(" * User clip planes: %s", _BoolToStr(hasCapability(RSC_USER_CLIP_PLANES)));
        DI_INFO(" * VET_UBYTE4 vertex element type: %s", _BoolToStr(hasCapability(RSC_VERTEX_FORMAT_UBYTE4)));
        DI_INFO(" * Infinite far plane projection: %s", _BoolToStr(hasCapability(RSC_INFINITE_FAR_PLANE)));
        DI_INFO(" * Hardware render-to-texture: %s", _BoolToStr(hasCapability(RSC_HWRENDER_TO_TEXTURE)));
        DI_INFO(" * Floating point textures: %s", _BoolToStr(hasCapability(RSC_TEXTURE_FLOAT)));
        DI_INFO(" * Non-power-of-two textures: %s %s", _BoolToStr(hasCapability(RSC_NON_POWER_OF_2_TEXTURES)),(mNonPOW2TexturesLimited ? " (limited)" : ""));
        DI_INFO(" * 1D textures: %s", _BoolToStr(hasCapability(RSC_TEXTURE_1D)));
        DI_INFO(" * 3D textures: %s", _BoolToStr(hasCapability(RSC_TEXTURE_3D)));
        DI_INFO(" * Multiple Render Targets: %d", mNumMultiRenderTargets);
        DI_INFO("   - With different bit depths: %s", _BoolToStr(hasCapability(RSC_MRT_DIFFERENT_BIT_DEPTHS)));
        
        DI_INFO(" * Vertex texture fetch: %s", _BoolToStr(hasCapability(RSC_VERTEX_TEXTURE_FETCH)));
        DI_INFO(" * Number of world matrices: %d", mNumWorldMatrices);
        DI_INFO(" * Number of texture units: %d", mNumTextureUnits);
        DI_INFO(" * Stencil buffer depth: %d", mStencilBufferBitDepth);
        DI_INFO(" * Number of vertex blend matrices: %d", mNumVertexBlendMatrices);

        DI_INFO(" * Render to Vertex Buffer : %s", _BoolToStr(hasCapability(RSC_HWRENDER_TO_VERTEX_BUFFER)));
        DI_INFO(" * Hardware Atomic Counters: %s", _BoolToStr(hasCapability(RSC_ATOMIC_COUNTERS)));

        if (mCategoryRelevant[CAPS_CATEGORY_GL])
        {
            DI_INFO(" * GL 1.5 without VBO workaround: %s"
                , _BoolToStr(hasCapability(RSC_GL1_5_NOVBO)));
            DI_INFO(" * Frame Buffer objects: %s"
                , _BoolToStr(hasCapability(RSC_FBO)));
            DI_INFO(" * Frame Buffer objects (ARB extension): %s"
                , _BoolToStr(hasCapability(RSC_FBO_ARB)));
            DI_INFO(" * Frame Buffer objects (ATI extension): %s"
                , _BoolToStr(hasCapability(RSC_FBO_ATI)));
            DI_INFO(" * PBuffer support: %s"
                , _BoolToStr(hasCapability(RSC_PBUFFER)));
            DI_INFO(" * GL 1.5 without HW-occlusion workaround: %s"
                , _BoolToStr(hasCapability(RSC_GL1_5_NOHWOCCLUSION)));
            DI_INFO(" * Vertex Array Objects: %s"
                , _BoolToStr(hasCapability(RSC_VAO)));
            DI_INFO(" * Separate shader objects: %s"
                , _BoolToStr(hasCapability(RSC_SEPARATE_SHADER_OBJECTS)));
        }

        if (mCategoryRelevant[CAPS_CATEGORY_D3D9])
        {
            DI_INFO(" * DirectX per stage constants: %s", _BoolToStr(hasCapability(RSC_PERSTAGECONSTANT)));
        }
    }
    
    StringVec DiGfxCaps::msGPUVendorStrings;
    
    GPUVendor DiGfxCaps::vendorFromString(const DiString& vendorString)
    {
        initVendorStrings();
        GPUVendor ret = GPU_UNKNOWN;
        DiString cmpString = vendorString;
        cmpString.ToLower();
        for (int i = 0; i < GPU_VENDOR_COUNT; ++i)
        {
            // case insensitive (lower case)
            if (msGPUVendorStrings[i] == cmpString)
            {
                ret = static_cast<GPUVendor>(i);
                break;
            }
        }

        return ret;
        
    }
    
    DiString DiGfxCaps::vendorToString(GPUVendor v)
    {
        initVendorStrings();
        return msGPUVendorStrings[v];
    }
    
    void DiGfxCaps::initVendorStrings()
    {
        if (msGPUVendorStrings.empty())
        {
            // Always lower case!
            msGPUVendorStrings.resize(GPU_VENDOR_COUNT);
            msGPUVendorStrings[GPU_UNKNOWN] = "unknown";
            msGPUVendorStrings[GPU_NVIDIA] = "nvidia";
            msGPUVendorStrings[GPU_AMD] = "amd";
            msGPUVendorStrings[GPU_INTEL] = "intel";
            msGPUVendorStrings[GPU_3DLABS] = "3dlabs";
            msGPUVendorStrings[GPU_S3] = "s3";
            msGPUVendorStrings[GPU_MATROX] = "matrox";
            msGPUVendorStrings[GPU_SIS] = "sis";
            msGPUVendorStrings[GPU_IMAGINATION_TECHNOLOGIES] = "imagination technologies";
            msGPUVendorStrings[GPU_APPLE] = "apple";    // iOS Simulator
            msGPUVendorStrings[GPU_NOKIA] = "nokia";
            msGPUVendorStrings[GPU_MS_SOFTWARE] = "microsoft"; // Microsoft software device
            msGPUVendorStrings[GPU_MS_WARP] = "ms warp";
            msGPUVendorStrings[GPU_ARM] = "arm";
            msGPUVendorStrings[GPU_QUALCOMM] = "qualcomm";
        }
    }

    void DiGfxCaps::setRenderSystemName(uint32 rs)
    {
        switch (rs)
        {
        case DRV_DIRECT3D9:
            mRenderSystemName = "Direct3D 9";
            break;
        case DRV_DIRECT3D11:
            mRenderSystemName = "Direct3D 11";
            break;
        case DRV_OPENGL:
            mRenderSystemName = "OpenGL";
            break;
        case DRV_OPENGL_ES2:
            mRenderSystemName = "OpenGLES 2.0";
            break;
        case DRV_NULL:
            mRenderSystemName = "Null";
        default:
            break;
        }
    }

}
