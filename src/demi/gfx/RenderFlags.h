


namespace Demi
{
    enum DiBlendFlags
    {
        F_BLEND_SRC_ZERO              = 0x00000001,
        F_BLEND_SRC_ONE               = 0x00000002,
        F_BLEND_SRC_DSTCOL            = 0x00000003,
        F_BLEND_SRC_ONEMINUSDSTCOL    = 0x00000004,
        F_BLEND_SRC_SRCALPHA          = 0x00000005,
        F_BLEND_SRC_ONEMINUSSRCALPHA  = 0x00000006,
        F_BLEND_SRC_DSTALPHA          = 0x00000007,
        F_BLEND_SRC_ONEMINUSDSTALPHA  = 0x00000008,
        F_BLEND_SRC_ALPHASATURATE     = 0x00000009,
        F_BLEND_SRC_MASK              = 0x0000000F,

        F_BLEND_DEST_ZERO             = 0x00000010,
        F_BLEND_DEST_ONE              = 0x00000020,
        F_BLEND_DEST_SRCCOL           = 0x00000030,
        F_BLEND_DEST_ONEMINUSSRCCOL   = 0x00000040,
        F_BLEND_DEST_SRCALPHA         = 0x00000050,
        F_BLEND_DEST_ONEMINUSSRCALPHA = 0x00000060,
        F_BLEND_DEST_DSTALPHA         = 0x00000070,
        F_BLEND_DEST_ONEMINUSDSTALPHA = 0x00000080,
        F_BLEND_DEST_MASK             = 0x000000F0,

        F_BLEND_OP_MAX                = 0x00080000,
        F_BLEND_OP_MASK               = 0x00080000,

        F_BLEND_MASK                  = 0x000800FF,
    };

    enum DiPolygonMode
    {
        F_WIREFRAME                   = 0x00010000,
        F_POINT                       = 0x00020000
    };

    enum DiDepthStencilFlags
    {
        F_DEPTH_WRITE                 = 0x00000100,
        F_Z_DISABLE                   = 0x00040000,
        F_STENCIL_ENABLE              = 0x00800000,

        F_DEPTHFUNC_LEQUAL            = 0x00000000,
        F_DEPTHFUNC_EQUAL             = 0x00100000,
        F_DEPTHFUNC_GREAT             = 0x00200000,
        F_DEPTHFUNC_LESS              = 0x00300000,
        F_DEPTHFUNC_GEQUAL            = 0x00400000,
        F_DEPTHFUNC_NOTEQUAL          = 0x00500000,
        F_DEPTHFUNC_MASK              = 0x00700000
    };                                            

}