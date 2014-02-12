


#ifndef DiCommonEnum_h__
#define DiCommonEnum_h__


namespace Demi 
{

#define MAX_BONE_NUM            75
#define MAX_WORLD_MATS          75    
#define MAX_TEXTURE_LAYERS      16
#define MAX_LIGHTS              8    
#define MAX_MRT                 8

    enum DiArchiveType
    {
        ARCHIVE_FILE
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

    enum DiAssetType
    {
        ASSET_MATERIAL = 0,
        ASSET_SHADER,
        ASSET_TEXTURE,
        ASSET_MODEL,
        ASSET_MOTION,
        ASSET_SCENE,

        NUM_TYPES
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
    
    #define CLIFF_A 0
    #define CLIFF_B 2
    #define CLIFF_C 4
    #define CLIFF_H 1

    enum DiCommonVsType
    {
        CVT_STATIC_MESH       = 0,
        CVT_SKEL_1_WEIGHT     = 1,
        CVT_SKEL_2_WEIGHTS    = 2,
        CVT_SKEL_4_WEIGHTS    = 3,

        CVT_MAX_MATS
    };
}


#endif
