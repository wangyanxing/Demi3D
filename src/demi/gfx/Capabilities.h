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

#ifndef DiCapabilities_h__
#define DiCapabilities_h__

/**
    This file was copied from Ogre3D
    http://www.ogre3d.org/
*/

#define CAPS_CATEGORY_SIZE 4
#define DEMI_CAPS_BITSHIFT (32 - CAPS_CATEGORY_SIZE)
#define CAPS_CATEGORY_MASK (((1 << CAPS_CATEGORY_SIZE) - 1) << DEMI_CAPS_BITSHIFT)
#define DEMI_CAPS_VALUE(cat, val) ((cat << DEMI_CAPS_BITSHIFT) | (1 << val))

namespace Demi 
{
    /// Enumerates the categories of capabilities
    enum CapabilitiesCategory
    {
        CAPS_CATEGORY_COMMON = 0,
        CAPS_CATEGORY_COMMON_2 = 1,
        CAPS_CATEGORY_D3D9 = 2,
        CAPS_CATEGORY_GL = 3,
        /// Placeholder for max value
        CAPS_CATEGORY_COUNT = 4
    };

    /// Enum describing the different hardware capabilities we want to check for
    /// DEMI_CAPS_VALUE(a, b) defines each capability
    // a is the category (which can be from 0 to 15)
    // b is the value (from 0 to 27)
    enum Capabilities
    {
        /// Supports generating mipmaps in hardware
        RSC_AUTOMIPMAP              = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 0),
        RSC_BLENDING                = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 1),
        /// Supports anisotropic texture filtering
        RSC_ANISOTROPY              = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 2),
        /// Supports fixed-function DOT3 texture blend
        RSC_DOT3                    = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 3),
        /// Supports cube mapping
        RSC_CUBEMAPPING             = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 4),
        /// Supports hardware stencil buffer
        RSC_HWSTENCIL               = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 5),
        /// Supports hardware vertex and index buffers
        RSC_VBO                     = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 7),
        /// Supports 32bit hardware index buffers
        RSC_32BIT_INDEX             = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 8),
        /// Supports vertex programs (vertex shaders)
        RSC_VERTEX_PROGRAM          = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 9),
        /// Supports fragment programs (pixel shaders)
        RSC_FRAGMENT_PROGRAM        = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 10),
        /// Supports performing a scissor test to exclude areas of the screen
        RSC_SCISSOR_TEST            = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 11),
        /// Supports separate stencil updates for both front and back faces
        RSC_TWO_SIDED_STENCIL       = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 12),
        /// Supports wrapping the stencil value at the range extremeties
        RSC_STENCIL_WRAP            = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 13),
        /// Supports hardware occlusion queries
        RSC_HWOCCLUSION             = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 14),
        /// Supports user clipping planes
        RSC_USER_CLIP_PLANES        = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 15),
        /// Supports the VET_UBYTE4 vertex element type
        RSC_VERTEX_FORMAT_UBYTE4    = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 16),
        /// Supports infinite far plane projection
        RSC_INFINITE_FAR_PLANE      = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 17),
        /// Supports hardware render-to-texture (bigger than framebuffer)
        RSC_HWRENDER_TO_TEXTURE     = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 18),
        /// Supports float textures and render targets
        RSC_TEXTURE_FLOAT           = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 19),
        /// Supports non-power of two textures
        RSC_NON_POWER_OF_2_TEXTURES = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 20),
        /// Supports 3d (volume) textures
        RSC_TEXTURE_3D              = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 21),
        /// Supports basic point sprite rendering
        RSC_POINT_SPRITES           = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 22),
        /// Supports extra point parameters (minsize, maxsize, attenuation)
        RSC_POINT_EXTENDED_PARAMETERS = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 23),
        /// Supports vertex texture fetch
        RSC_VERTEX_TEXTURE_FETCH = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 24),
        /// Supports mipmap LOD biasing
        RSC_MIPMAP_LOD_BIAS = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 25),
        /// Supports hardware geometry programs
        RSC_GEOMETRY_PROGRAM = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 26),
        /// Supports rendering to vertex buffers
        RSC_HWRENDER_TO_VERTEX_BUFFER = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 27),
        /// Supports different texture bindings
        RSC_COMPLETE_TEXTURE_BINDING = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON, 28),

        /// Supports compressed textures
        RSC_TEXTURE_COMPRESSION = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 0),
        /// Supports compressed textures in the DXT/ST3C formats
        RSC_TEXTURE_COMPRESSION_DXT = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 1),
        /// Supports compressed textures in the VTC format
        RSC_TEXTURE_COMPRESSION_VTC = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 2),
        /// Supports compressed textures in the PVRTC format
        RSC_TEXTURE_COMPRESSION_PVRTC = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 3),
        /// Supports compressed textures in the ATC format
        RSC_TEXTURE_COMPRESSION_ATC = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 4),
        /// Supports compressed textures in the ETC1 format
        RSC_TEXTURE_COMPRESSION_ETC1 = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 5),
        /// Supports compressed textures in the ETC2 format
        RSC_TEXTURE_COMPRESSION_ETC2 = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 6),
        /// Supports compressed textures in BC4 and BC5 format (DirectX feature level 10_0)
        RSC_TEXTURE_COMPRESSION_BC4_BC5 = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 7),
        /// Supports compressed textures in BC6H and BC7 format (DirectX feature level 11_0)
        RSC_TEXTURE_COMPRESSION_BC6H_BC7 = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 8),
        /// Supports fixed-function pipeline
        RSC_FIXED_FUNCTION = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 9),
        /// Supports MRTs with different bit depths
        RSC_MRT_DIFFERENT_BIT_DEPTHS = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 10),
        /// Supports Alpha to Coverage (A2C)
        RSC_ALPHA_TO_COVERAGE = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 11),
        /// Supports Blending operations other than +
        RSC_ADVANCED_BLEND_OPERATIONS = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 12),
        /// Supports a separate depth buffer for RTTs. D3D 9 & 10, OGL w/FBO (RSC_FBO implies this flag)
        RSC_RTT_SEPARATE_DEPTHBUFFER = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 13),
        /// Supports using the MAIN depth buffer for RTTs. D3D 9&10, OGL w/FBO support unknown
        /// (undefined behavior?), OGL w/ copy supports it
        RSC_RTT_MAIN_DEPTHBUFFER_ATTACHABLE = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 14),
        /// Supports attaching a depth buffer to an RTT that has width & height less or equal than RTT's.
        /// Otherwise must be of _exact_ same resolution. D3D 9, OGL 3.0 (not 2.0, not D3D10)
        RSC_RTT_DEPTHBUFFER_RESOLUTION_LESSEQUAL = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 15),
        /// Supports using vertex buffers for instance data
        RSC_VERTEX_BUFFER_INSTANCE_DATA = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 16),
        /// Supports using vertex buffers for instance data
        RSC_CAN_GET_COMPILED_SHADER_BUFFER = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 17),
        /// Supports dynamic linkage/shader subroutine
        RSC_SHADER_SUBROUTINE = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 18),

        RSC_HWRENDER_TO_TEXTURE_3D = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 19),
        /// Supports 1d textures
        RSC_TEXTURE_1D              = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 20),
        /// Supports hardware tessellation hull programs
        RSC_TESSELLATION_HULL_PROGRAM = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 21),
        /// Supports hardware tessellation domain programs
        RSC_TESSELLATION_DOMAIN_PROGRAM = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 22),
        /// Supports hardware compute programs
        RSC_COMPUTE_PROGRAM = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 23),
        /// Supports asynchronous hardware occlusion queries
        RSC_HWOCCLUSION_ASYNCHRONOUS = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 24),
        /// Supports asynchronous hardware occlusion queries
        RSC_ATOMIC_COUNTERS = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 25),
        /// Supports reading back the inactive depth-stencil buffer as texture
        RSC_READ_BACK_AS_TEXTURE = DEMI_CAPS_VALUE(CAPS_CATEGORY_COMMON_2, 26),
        // ***** DirectX specific caps *****
        /// Is DirectX feature "per stage constants" supported
        RSC_PERSTAGECONSTANT = DEMI_CAPS_VALUE(CAPS_CATEGORY_D3D9, 0),

        // ***** GL Specific Caps *****
        /// Supports OpenGL version 1.5
        RSC_GL1_5_NOVBO    = DEMI_CAPS_VALUE(CAPS_CATEGORY_GL, 1),
        /// Support for Frame Buffer Objects (FBOs)
        RSC_FBO              = DEMI_CAPS_VALUE(CAPS_CATEGORY_GL, 2),
        /// Support for Frame Buffer Objects ARB implementation (regular FBO is higher precedence)
        RSC_FBO_ARB          = DEMI_CAPS_VALUE(CAPS_CATEGORY_GL, 3),
        /// Support for Frame Buffer Objects ATI implementation (ARB FBO is higher precedence)
        RSC_FBO_ATI          = DEMI_CAPS_VALUE(CAPS_CATEGORY_GL, 4),
        /// Support for PBuffer
        RSC_PBUFFER          = DEMI_CAPS_VALUE(CAPS_CATEGORY_GL, 5),
        /// Support for GL 1.5 but without HW occlusion workaround
        RSC_GL1_5_NOHWOCCLUSION = DEMI_CAPS_VALUE(CAPS_CATEGORY_GL, 6),
        /// Support for point parameters ARB implementation
        RSC_POINT_EXTENDED_PARAMETERS_ARB = DEMI_CAPS_VALUE(CAPS_CATEGORY_GL, 7),
        /// Support for point parameters EXT implementation
        RSC_POINT_EXTENDED_PARAMETERS_EXT = DEMI_CAPS_VALUE(CAPS_CATEGORY_GL, 8),
        /// Support for Separate Shader Objects
        RSC_SEPARATE_SHADER_OBJECTS = DEMI_CAPS_VALUE(CAPS_CATEGORY_GL, 9),
        /// Support for Vertex Array Objects (VAOs)
        RSC_VAO              = DEMI_CAPS_VALUE(CAPS_CATEGORY_GL, 10)
    };

    /// DriverVersion is used by RenderSystemCapabilities and both GL and D3D9
    /// to store the version of the current GPU driver
    struct DI_GFX_API DriverVersion
    {
        int major;
        int minor;
        int release;
        int build;

        DriverVersion() 
        {
            major = minor = release = build = 0;
        }

        DiString toString() const 
        {
            DiString ret;
            ret.Format("%d.%d.%d.%d", major, minor, release, build);
            return ret;
        }

        void fromString(const DiString& versionString)
        {
            auto tokens = versionString.Tokenize(".");
            if (!tokens.empty())
            {
                major = tokens[0].AsInt();
                if (tokens.size() > 1)
                    minor = tokens[1].AsInt();
                if (tokens.size() > 2)
                    release = tokens[2].AsInt();
                if (tokens.size() > 3)
                    build = tokens[3].AsInt();
            }
        }
    };

    /** Enumeration of GPU vendors. */
    enum GPUVendor
    {
        GPU_UNKNOWN = 0,
        GPU_NVIDIA = 1,
        GPU_AMD = 2,
        GPU_INTEL = 3,
        GPU_S3 = 4,
        GPU_MATROX = 5,
        GPU_3DLABS = 6,
        GPU_SIS = 7,
        GPU_IMAGINATION_TECHNOLOGIES = 8,
        GPU_APPLE = 9,  // Apple Software Renderer
        GPU_NOKIA = 10,
        GPU_MS_SOFTWARE = 11, // Microsoft software device
        GPU_MS_WARP = 12, // Microsoft WARP (Windows Advanced Rasterization Platform) software device - http://msdn.microsoft.com/en-us/library/dd285359.aspx
        GPU_ARM = 13, // For the Mali chipsets
        GPU_QUALCOMM = 14,

        /// placeholder
        GPU_VENDOR_COUNT = 15
    };

    /** singleton class for storing the capabilities of the graphics card. 
    @remarks
    This class stores the capabilities of the graphics card.  This
    information is set by the individual render systems.
    */
    class DI_GFX_API DiGfxCaps : public DiBase
    {

    public:

        typedef DiSet<DiString> ShaderProfiles;

    private:
        /// This is used to build a database of RSC's
        /// if a RSC with same name, but newer version is introduced, the older one 
        /// will be removed
        DriverVersion mDriverVersion;
        /// GPU Vendor
        GPUVendor mVendor;

        static StringVec msGPUVendorStrings;
        static void initVendorStrings();

        /// The number of world matrices available
        uint16 mNumWorldMatrices;
        /// The number of texture units available
        uint16 mNumTextureUnits;
        /// The stencil buffer bit depth
        uint16 mStencilBufferBitDepth;
        /// The number of matrices available for hardware blending
        uint16 mNumVertexBlendMatrices;
        /// Stores the capabilities flags.
        int mCapabilities[CAPS_CATEGORY_COUNT];
        /// Which categories are relevant
        bool mCategoryRelevant[CAPS_CATEGORY_COUNT];
        /// The name of the device as reported by the render system
        DiString mDeviceName;
        /// The identifier associated with the render system for which these capabilities are valid
        DiString mRenderSystemName;

        /// The number of floating-point constants vertex programs support
        uint16 mVertexProgramConstantFloatCount;           
        /// The number of integer constants vertex programs support
        uint16 mVertexProgramConstantIntCount;           
        /// The number of boolean constants vertex programs support
        uint16 mVertexProgramConstantBoolCount;           
        /// The number of floating-point constants geometry programs support
        uint16 mGeometryProgramConstantFloatCount;           
        /// The number of integer constants vertex geometry support
        uint16 mGeometryProgramConstantIntCount;           
        /// The number of boolean constants vertex geometry support
        uint16 mGeometryProgramConstantBoolCount;           
        /// The number of floating-point constants fragment programs support
        uint16 mFragmentProgramConstantFloatCount;           
        /// The number of integer constants fragment programs support
        uint16 mFragmentProgramConstantIntCount;           
        /// The number of boolean constants fragment programs support
        uint16 mFragmentProgramConstantBoolCount;
        /// The number of simultaneous render targets supported
        uint16 mNumMultiRenderTargets;
        /// The maximum point size
        float mMaxPointSize;
        /// Are non-POW2 textures feature-limited?
        bool mNonPOW2TexturesLimited;
        /// The maximum supported anisotropy
        float mMaxSupportedAnisotropy;
        /// The number of vertex texture units supported
        uint16 mNumVertexTextureUnits;
        /// Are vertex texture units shared with fragment processor?
        bool mVertexTextureUnitsShared;
        /// The number of vertices a geometry program can emit in a single run
        int mGeometryProgramNumOutputVertices;

        /// The list of supported shader profiles
        ShaderProfiles mSupportedShaderProfiles;

        // Support for new shader stages in shader model 5.0
        /// The number of floating-point constants tessellation Hull programs support
        uint16 mTessellationHullProgramConstantFloatCount;           
        /// The number of integer constants tessellation Hull programs support
        uint16 mTessellationHullProgramConstantIntCount;           
        /// The number of boolean constants tessellation Hull programs support
        uint16 mTessellationHullProgramConstantBoolCount;
        /// The number of floating-point constants tessellation Domain programs support
        uint16 mTessellationDomainProgramConstantFloatCount;           
        /// The number of integer constants tessellation Domain programs support
        uint16 mTessellationDomainProgramConstantIntCount;           
        /// The number of boolean constants tessellation Domain programs support
        uint16 mTessellationDomainProgramConstantBoolCount;
        /// The number of floating-point constants compute programs support
        uint16 mComputeProgramConstantFloatCount;           
        /// The number of integer constants compute programs support
        uint16 mComputeProgramConstantIntCount;           
        /// The number of boolean constants compute programs support
        uint16 mComputeProgramConstantBoolCount;

    public:    
        DiGfxCaps ();
        virtual ~DiGfxCaps ();

        virtual size_t calculateSize() const {return 0;}

        /** Set the driver version. */
        void setDriverVersion(const DriverVersion& version)
        {
            mDriverVersion = version;
        }

        void parseDriverVersionFromString(const DiString& versionString)
        {
            DriverVersion version;
            version.fromString(versionString);
            setDriverVersion(version);
        }


        DriverVersion getDriverVersion() const
        {
            return mDriverVersion;
        }

        GPUVendor getVendor() const
        {
            return mVendor;
        }

        void setVendor(GPUVendor v)
        {
            mVendor = v;
        }

        /// Parse and set vendor
        void parseVendorFromString(const DiString& vendorString)
        {
            setVendor(vendorFromString(vendorString));
        }

        /// Convert a vendor string to an enum
        static GPUVendor vendorFromString(const DiString& vendorString);
        /// Convert a vendor enum to a string
        static DiString vendorToString(GPUVendor v);

        bool isDriverOlderThanVersion(const DriverVersion &v) const
        {
            if (mDriverVersion.major < v.major)
                return true;
            else if (mDriverVersion.major == v.major && 
                mDriverVersion.minor < v.minor)
                return true;
            else if (mDriverVersion.major == v.major && 
                mDriverVersion.minor == v.minor && 
                mDriverVersion.release < v.release)
                return true;
            else if (mDriverVersion.major == v.major && 
                mDriverVersion.minor == v.minor && 
                mDriverVersion.release == v.release &&
                mDriverVersion.build < v.build)
                return true;
            return false;
        }

        void setNumWorldMatrices(uint16 num)
        {
            mNumWorldMatrices = num;
        }

        void setNumTextureUnits(uint16 num)
        {
            mNumTextureUnits = num;
        }

        void setStencilBufferBitDepth(uint16 num)
        {
            mStencilBufferBitDepth = num;
        }

        void setNumVertexBlendMatrices(uint16 num)
        {
            mNumVertexBlendMatrices = num;
        }

        /// The number of simultaneous render targets supported
        void setNumMultiRenderTargets(uint16 num)
        {
            mNumMultiRenderTargets = num;
        }

        uint16 getNumWorldMatrices(void) const
        { 
            return mNumWorldMatrices;
        }

        /** Returns the number of texture units the current output hardware
        supports.

        For use in rendering, this determines how many texture units the
        are available for multitexturing (i.e. rendering multiple 
        textures in a single pass). Where a Material has multiple 
        texture layers, it will try to use multitexturing where 
        available, and where it is not available, will perform multipass
        rendering to achieve the same effect. This property only applies
        to the fixed-function pipeline, the number available to the 
        programmable pipeline depends on the shader model in use.
        */
        uint16 getNumTextureUnits(void) const
        {
            return mNumTextureUnits;
        }

        /** Determines the bit depth of the hardware accelerated stencil 
        buffer, if supported.
        @remarks
        If hardware stencilling is not supported, the software will
        provide an 8-bit software stencil.
        */
        uint16 getStencilBufferBitDepth(void) const
        {
            return mStencilBufferBitDepth;
        }

        /** Returns the number of matrices available to hardware vertex 
        blending for this rendering system. */
        uint16 getNumVertexBlendMatrices(void) const
        {
            return mNumVertexBlendMatrices;
        }

        /// The number of simultaneous render targets supported
        uint16 getNumMultiRenderTargets(void) const
        {
            return mNumMultiRenderTargets;
        }

        /** Returns true if capability is render system specific
        */
        bool isCapabilityRenderSystemSpecific(const Capabilities c) const
        {
            int cat = c >> DEMI_CAPS_BITSHIFT;
            if(cat == CAPS_CATEGORY_GL || cat == CAPS_CATEGORY_D3D9)
                return true;
            return false;
        }

        /** Adds a capability flag
        */
        void setCapability(const Capabilities c) 
        { 
            int index = (CAPS_CATEGORY_MASK & c) >> DEMI_CAPS_BITSHIFT;
            // zero out the index from the stored capability
            mCapabilities[index] |= (c & ~CAPS_CATEGORY_MASK);
        }

        /** Remove a capability flag
        */
        void unsetCapability(const Capabilities c) 
        { 
            int index = (CAPS_CATEGORY_MASK & c) >> DEMI_CAPS_BITSHIFT;
            // zero out the index from the stored capability
            mCapabilities[index] &= (~c | CAPS_CATEGORY_MASK);
        }

        /** Checks for a capability
        */
        bool hasCapability(const Capabilities c) const
        {
            int index = (CAPS_CATEGORY_MASK & c) >> DEMI_CAPS_BITSHIFT;
            // test against
            if(mCapabilities[index] & (c & ~CAPS_CATEGORY_MASK))
                return true;
            else
                return false;
        }

        /** Adds the profile to the list of supported profiles
        */
        void addShaderProfile(const DiString& profile)
        {
            mSupportedShaderProfiles.insert(profile);

        }

        /** Remove a given shader profile, if present.
        */
        void removeShaderProfile(const DiString& profile)
        {
            mSupportedShaderProfiles.erase(profile);
        }

        /** Returns true if profile is in the list of supported profiles
        */
        bool isShaderProfileSupported(const DiString& profile) const
        {
            return (mSupportedShaderProfiles.end() != mSupportedShaderProfiles.find(profile));
        }

        /** Returns a set of all supported shader profiles
        * */
        const ShaderProfiles& getSupportedShaderProfiles() const
        {
            return mSupportedShaderProfiles;
        }

        /// The number of floating-point constants vertex programs support
        uint16 getVertexProgramConstantFloatCount(void) const
        {
            return mVertexProgramConstantFloatCount;           
        }
        /// The number of integer constants vertex programs support
        uint16 getVertexProgramConstantIntCount(void) const
        {
            return mVertexProgramConstantIntCount;           
        }
        /// The number of boolean constants vertex programs support
        uint16 getVertexProgramConstantBoolCount(void) const
        {
            return mVertexProgramConstantBoolCount;           
        }
        /// The number of floating-point constants geometry programs support
        uint16 getGeometryProgramConstantFloatCount(void) const
        {
            return mGeometryProgramConstantFloatCount;           
        }
        /// The number of integer constants geometry programs support
        uint16 getGeometryProgramConstantIntCount(void) const
        {
            return mGeometryProgramConstantIntCount;           
        }
        /// The number of boolean constants geometry programs support
        uint16 getGeometryProgramConstantBoolCount(void) const
        {
            return mGeometryProgramConstantBoolCount;           
        }
        /// The number of floating-point constants fragment programs support
        uint16 getFragmentProgramConstantFloatCount(void) const
        {
            return mFragmentProgramConstantFloatCount;           
        }
        /// The number of integer constants fragment programs support
        uint16 getFragmentProgramConstantIntCount(void) const
        {
            return mFragmentProgramConstantIntCount;           
        }
        /// The number of boolean constants fragment programs support
        uint16 getFragmentProgramConstantBoolCount(void) const
        {
            return mFragmentProgramConstantBoolCount;           
        }

        /// sets the device name for Render system
        void setDeviceName(const DiString& name)
        {
            mDeviceName = name;
        }

        /// gets the device name for render system
        DiString getDeviceName() const
        {
            return mDeviceName;
        }

        /// The number of floating-point constants vertex programs support
        void setVertexProgramConstantFloatCount(uint16 c)
        {
            mVertexProgramConstantFloatCount = c;           
        }
        /// The number of integer constants vertex programs support
        void setVertexProgramConstantIntCount(uint16 c)
        {
            mVertexProgramConstantIntCount = c;           
        }
        /// The number of boolean constants vertex programs support
        void setVertexProgramConstantBoolCount(uint16 c)
        {
            mVertexProgramConstantBoolCount = c;           
        }
        /// The number of floating-point constants geometry programs support
        void setGeometryProgramConstantFloatCount(uint16 c)
        {
            mGeometryProgramConstantFloatCount = c;           
        }
        /// The number of integer constants geometry programs support
        void setGeometryProgramConstantIntCount(uint16 c)
        {
            mGeometryProgramConstantIntCount = c;           
        }
        /// The number of boolean constants geometry programs support
        void setGeometryProgramConstantBoolCount(uint16 c)
        {
            mGeometryProgramConstantBoolCount = c;           
        }
        /// The number of floating-point constants fragment programs support
        void setFragmentProgramConstantFloatCount(uint16 c)
        {
            mFragmentProgramConstantFloatCount = c;           
        }
        /// The number of integer constants fragment programs support
        void setFragmentProgramConstantIntCount(uint16 c)
        {
            mFragmentProgramConstantIntCount = c;           
        }
        /// The number of boolean constants fragment programs support
        void setFragmentProgramConstantBoolCount(uint16 c)
        {
            mFragmentProgramConstantBoolCount = c;           
        }
        /// Maximum point screen size in pixels
        void setMaxPointSize(float s)
        {
            mMaxPointSize = s;
        }
        /// Maximum point screen size in pixels
        float getMaxPointSize(void) const
        {
            return mMaxPointSize;
        }
        /// Non-POW2 textures limited
        void setNonPOW2TexturesLimited(bool l)
        {
            mNonPOW2TexturesLimited = l;
        }
        /** Are non-power of two textures limited in features?
        @remarks
        If the RSC_NON_POWER_OF_2_TEXTURES capability is set, but this
        method returns true, you can use non power of 2 textures only if:
        <ul><li>You load them explicitly with no mip maps</li>
        <li>You don't use DXT texture compression</li>
        <li>You use clamp texture addressing</li></ul>
        */
        bool getNonPOW2TexturesLimited(void) const
        {
            return mNonPOW2TexturesLimited;
        }
        /// Set the maximum supported anisotropic filtering
        void setMaxSupportedAnisotropy(float s)
        {
            mMaxSupportedAnisotropy = s;
        }
        /// Get the maximum supported anisotropic filtering
        float getMaxSupportedAnisotropy()
        {
            return mMaxSupportedAnisotropy;
        }

        /// Set the number of vertex texture units supported
        void setNumVertexTextureUnits(uint16 n)
        {
            mNumVertexTextureUnits = n;
        }
        /// Get the number of vertex texture units supported
        uint16 getNumVertexTextureUnits(void) const
        {
            return mNumVertexTextureUnits;
        }
        /// Set whether the vertex texture units are shared with the fragment processor
        void setVertexTextureUnitsShared(bool shared)
        {
            mVertexTextureUnitsShared = shared;
        }
        /// Get whether the vertex texture units are shared with the fragment processor
        bool getVertexTextureUnitsShared(void) const
        {
            return mVertexTextureUnitsShared;
        }

        /// Set the number of vertices a single geometry program run can emit
        void setGeometryProgramNumOutputVertices(int numOutputVertices)
        {
            mGeometryProgramNumOutputVertices = numOutputVertices;
        }
        /// Get the number of vertices a single geometry program run can emit
        int getGeometryProgramNumOutputVertices(void) const
        {
            return mGeometryProgramNumOutputVertices;
        }

        /// Get the identifier of the rendersystem from which these capabilities were generated
        DiString getRenderSystemName(void) const
        {
            return mRenderSystemName;
        }
        ///  Set the identifier of the rendersystem from which these capabilities were generated
        void setRenderSystemName(uint32 rs);

        /// Mark a category as 'relevant' or not, ie will it be reported
        void setCategoryRelevant(CapabilitiesCategory cat, bool relevant)
        {
            mCategoryRelevant[cat] = relevant;
        }

        /// Return whether a category is 'relevant' or not, ie will it be reported
        bool isCategoryRelevant(CapabilitiesCategory cat)
        {
            return mCategoryRelevant[cat];
        }

        /** Write the capabilities to the pass in Log */
        void LogCaps();

        // Support for new shader stages in shader model 5.0
        /// The number of floating-point constants tessellation Hull programs support
        void setTessellationHullProgramConstantFloatCount(uint16 c)
        {
            mTessellationHullProgramConstantFloatCount = c;           
        }
        /// The number of integer constants tessellation Domain programs support
        void setTessellationHullProgramConstantIntCount(uint16 c)
        {
            mTessellationHullProgramConstantIntCount = c;           
        }
        /// The number of boolean constants tessellation Domain programs support
        void setTessellationHullProgramConstantBoolCount(uint16 c)
        {
            mTessellationHullProgramConstantBoolCount = c;           
        }
        /// The number of floating-point constants fragment programs support
        uint16 getTessellationHullProgramConstantFloatCount(void) const
        {
            return mTessellationHullProgramConstantFloatCount;           
        }
        /// The number of integer constants fragment programs support
        uint16 getTessellationHullProgramConstantIntCount(void) const
        {
            return mTessellationHullProgramConstantIntCount;           
        }
        /// The number of boolean constants fragment programs support
        uint16 getTessellationHullProgramConstantBoolCount(void) const
        {
            return mTessellationHullProgramConstantBoolCount;           
        }

        /// The number of floating-point constants tessellation Domain programs support
        void setTessellationDomainProgramConstantFloatCount(uint16 c)
        {
            mTessellationDomainProgramConstantFloatCount = c;           
        }
        /// The number of integer constants tessellation Domain programs support
        void setTessellationDomainProgramConstantIntCount(uint16 c)
        {
            mTessellationDomainProgramConstantIntCount = c;           
        }
        /// The number of boolean constants tessellation Domain programs support
        void setTessellationDomainProgramConstantBoolCount(uint16 c)
        {
            mTessellationDomainProgramConstantBoolCount = c;           
        }
        /// The number of floating-point constants fragment programs support
        uint16 getTessellationDomainProgramConstantFloatCount(void) const
        {
            return mTessellationDomainProgramConstantFloatCount;           
        }
        /// The number of integer constants fragment programs support
        uint16 getTessellationDomainProgramConstantIntCount(void) const
        {
            return mTessellationDomainProgramConstantIntCount;           
        }
        /// The number of boolean constants fragment programs support
        uint16 getTessellationDomainProgramConstantBoolCount(void) const
        {
            return mTessellationDomainProgramConstantBoolCount;           
        }

        /// The number of floating-point constants compute programs support
        void setComputeProgramConstantFloatCount(uint16 c)
        {
            mComputeProgramConstantFloatCount = c;           
        }
        /// The number of integer constants compute programs support
        void setComputeProgramConstantIntCount(uint16 c)
        {
            mComputeProgramConstantIntCount = c;           
        }
        /// The number of boolean constants compute programs support
        void setComputeProgramConstantBoolCount(uint16 c)
        {
            mComputeProgramConstantBoolCount = c;           
        }
        /// The number of floating-point constants fragment programs support
        uint16 getComputeProgramConstantFloatCount(void) const
        {
            return mComputeProgramConstantFloatCount;           
        }
        /// The number of integer constants fragment programs support
        uint16 getComputeProgramConstantIntCount(void) const
        {
            return mComputeProgramConstantIntCount;           
        }
        /// The number of boolean constants fragment programs support
        uint16 getComputeProgramConstantBoolCount(void) const
        {
            return mComputeProgramConstantBoolCount;           
        }

    };

} 

#endif

