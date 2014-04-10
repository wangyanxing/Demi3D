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

#ifndef DiCommonEnum_h__
#define DiCommonEnum_h__

#include "math/vector3.h"
#include "math/quaternion.h"

namespace Demi 
{

// ES2.0's limitation
#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
#   define MAX_BONE_NUM     32
#   define MAX_WORLD_MATS   32
#else
#   define MAX_BONE_NUM     75
#   define MAX_WORLD_MATS   75
#endif
    
#define MAX_TEXTURE_LAYERS  16
    
#define MAX_UV_SETS         8    
    
#define MAX_LIGHTS          8
    
#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
#   define MAX_MRT          1
#else
#   define MAX_MRT          4
#endif
    
    /** widely used
     */
    struct Trans
    {
        Trans()
        {
            pos = DiVec3::ZERO;
            scale = DiVec3::UNIT_SCALE;
            rot = DiQuat::IDENTITY;
        }
        DiVec3 pos;
        DiVec3 scale;
        DiQuat rot;
    };

    enum DiArchiveType
    {
        ARCHIVE_FILE,
        ARCHIVE_ZIP
    };

    enum DiShaderType
    {
        SHADER_VERTEX = 1,
        SHADER_PIXEL,

        SHADER_UNKNOW
    };

    enum DiInstanceType
    {
        INSTANCE_SHADER_BASED,
        INSTANCE_HARDWARE_BASED,
        INSTANCE_DISABLE,
    };

    enum
    { 
        TERRAIN_LAYER_NUM = 2, 
        CHUNK_GRID_SIZE = 32,  
        WATER_MAP_SIZE = 512,  
        FOLIAGE_DENSITY_MAP_SIZE    = 512, 
        CLIFF_SIZE = 128
    };
    
    enum DiBlendMode
    {
        BLEND_REPLACE = 0,
        BLEND_ADD,
        BLEND_MULTIPLY,
        BLEND_ALPHA,
        BLEND_TRANS_COLOR,
        BLEND_ONE_INV_ALPHA,

        BLEND_MAX
    };

    enum DiPrimitiveType
    {
        PT_UNKNOWN = 0,

        PT_POINTLIST,
        PT_LINELIST,
        PT_LINESTRIP,
        PT_TRIANGLELIST,
        PT_TRIANGLESTRIP,
        PT_TRIANGLEFAN,

        PT_MAX
    };

    enum DiResUsage
    {
        RU_NONE         = 0,
        RU_STATIC       = 1<<1,
        RU_DYNAMIC      = 1<<2,
        RU_WRITE_ONLY   = 1<<3
    };

    enum DiFilterType
    {
        FILTER_NEAREST = 0,
        FILTER_BILINEAR,
        FILTER_TRILINEAR,
        FILTER_ANISOTROPIC,
        FILTER_DEFAULT,

        FILTER_MAX
    };

    enum DiFillMode
    {
        FM_SOLID,
        FM_WIREFRAME,
        FM_POINT
    };

    enum DiAddMode
    {
        AM_WRAP = 0,
        AM_MIRROR,
        AM_CLAMP,
        AM_BORDER,
        AM_MIRRORONCE,

        AM_MAX
    };

    enum DiIndexBufType
    {
        IB_16BITS,
        IB_32BITS
    };

    enum DiPixelFormat
    {
        PF_R8G8B8,
        PF_A8R8G8B8,
        PF_A8B8G8R8,
        PF_X8R8G8B8,
        
        PF_A8,
        PF_L8,
        
        PF_DXT1,
        PF_DXT2,
        PF_DXT3,
        PF_DXT4,
        PF_DXT5,

        PF_R16F,
        PF_G16R16F,
        PF_A16B16G16R16F,
        PF_R32F,
        PF_G32R32F,
        PF_A32B32G32R32F,

        /// PVRTC (PowerVR) RGB 2 bpp
        PF_PVRTC_RGB2,
        /// PVRTC (PowerVR) RGBA 2 bpp
        PF_PVRTC_RGBA2,
        /// PVRTC (PowerVR) RGB 4 bpp
        PF_PVRTC_RGB4,
        /// PVRTC (PowerVR) RGBA 4 bpp
        PF_PVRTC_RGBA4,
        /// PVRTC (PowerVR) Version 2, 2 bpp
        PF_PVRTC2_2BPP,
        /// PVRTC (PowerVR) Version 2, 4 bpp
        PF_PVRTC2_4BPP,

        /// ETC1 (Ericsson Texture Compression)
        PF_ETC1_RGB8,
        /// ETC2 (Ericsson Texture Compression)
        PF_ETC2_RGB8,
        /// ETC2 (Ericsson Texture Compression)
        PF_ETC2_RGBA8,
        /// ETC2 (Ericsson Texture Compression)
        PF_ETC2_RGB8A1,

        /// ATC (AMD_compressed_ATC_texture)
        PF_ATC_RGB,
        /// ATC (AMD_compressed_ATC_texture)
        PF_ATC_RGBA_EXPLICIT_ALPHA,
        /// ATC (AMD_compressed_ATC_texture)
        PF_ATC_RGBA_INTERPOLATED_ALPHA,
        
        PF_DEPTH,

        PIXEL_FORMAT_MAX
    };

    enum DiCompFunc
    {
        CMP_NEVER,
        CMP_LESS,
        CMP_EQUAL,
        CMP_LESSEQUAL,
        CMP_GREATER,
        CMP_NOTEQUAL,
        CMP_GREATEREQUAL,
        CMP_ALWAYS,

        CMP_MAX
    };

    enum DiLockFlag
    {
        LOCK_NORMAL = 0,
        LOCK_DISCARD,
        LOCK_READ_ONLY,
        LOCK_NO_OVERWRITE,

        LOCK_MAX
    };

    enum DiCullMode
    {
        CULL_NONE,
        CULL_CW,
        CULL_CCW,

        CULL_MAX
    };

    enum DiClearFlag
    {
        CLEAR_COLOR = 1 << 0,
        CLEAR_DEPTH = 1 << 1,
        CLEAR_STENCIL = 1 << 2
    };


    enum DiTextureUsage
    {
        TU_TEXURE        = 0,
        TU_DEPTH_STENCIL = 1<<0,
        TU_RENDER_TARGET = 1<<1
    };

    enum DiSortMode
    {
        SM_DIRECTION,
        SM_DISTANCE
    };

    enum DiAxisConstrains
    {
        AC_AXIS_X    = 1,
        AC_AXIS_Y,
        AC_AXIS_Z,
        AC_AXIS_XY,
        AC_AXIS_YZ,
        AC_AXIS_XZ,
        AC_AXIS_XYZ,

        AC_AXIS_TERRAIN,
    };

    /** Enumerates the wave types for animation */
    enum DiWaveformType
    {
        /// Standard sine wave which smoothly changes from low to high and back again.
        WFT_SINE,
        /// An angular wave with a constant increase / decrease speed with pointed peaks.
        WFT_TRIANGLE,
        /// Half of the time is spent at the min, half at the max with instant transition between.
        WFT_SQUARE,
        /// Gradual steady increase from min to max over the period with an instant return to min at the end.
        WFT_SAWTOOTH,
        /// Gradual steady decrease from max to min over the period, with an instant return to max at the end.
        WFT_INVERSE_SAWTOOTH,
        /// Pulse Width Modulation. Works like WFT_SQUARE, except the high to low transition is controlled by duty cycle. 
        /// With a duty cycle of 50% (0.5) will give the same output as WFT_SQUARE.
        WFT_PWM
    };

    enum SceneMemoryMgrTypes
    {
        SCENE_DYNAMIC = 0,
        SCENE_STATIC,
        NUM_SCENE_MEMORY_MANAGER_TYPES
    };
}


#endif
