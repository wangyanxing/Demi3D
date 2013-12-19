
/********************************************************************
    File:       VertexFormat.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

namespace Demi
{
    enum DiVertexUsage
    {
        VERT_USAGE_POSITION    = 0,
        VERT_USAGE_BLENDWEIGHT ,  
        VERT_USAGE_BLENDINDICES, 
        VERT_USAGE_NORMAL   ,       
        VERT_USAGE_TEXCOORD ,     
        VERT_USAGE_TANGENT  ,      
        VERT_USAGE_BINORMAL ,     
        VERT_USAGE_COLOR       
    };

    enum DiVertexType
    {
        VERT_TYPE_FLOAT1  = 0,
        VERT_TYPE_FLOAT2  ,
        VERT_TYPE_FLOAT3  ,
        VERT_TYPE_FLOAT4  ,
        VERT_TYPE_COLOR   ,
        VERT_TYPE_UBYTE4  ,
        VERT_TYPE_SHORT2  ,
        VERT_TYPE_SHORT4  ,

        VERT_TYPE_UNUSED
    };

    // Basically there are thee streams used in this engine
    enum DiStreamID
    {
        SID_GENERAL = 0,
        SID_TANGENT,
        SID_SKINNING
    };

#define GENERAL_DECLAR_START  0
#define TANGENT_DECLAR_START  20
#define SKINNING_DECLAR_START 24

    // We use a 32 bits unsigned int to save the all combinations of vertex decelarations
    enum DiDefaultVertexFormat
    {
        // General stream, 20bits reserved
        VF_POS3F_NORMAL3F_UV2F          = 1 << (GENERAL_DECLAR_START  + 0),
        VF_POS3F_NORMAL3F_COLOR_UV2F    = 1 << (GENERAL_DECLAR_START  + 1),
        VF_POS3F_NORMAL3F_COLOR         = 1 << (GENERAL_DECLAR_START  + 2),
        VF_POS3F_UV2F_UF2F              = 1 << (GENERAL_DECLAR_START  + 3), // for lightmap
        VF_POS3F_COLOR                  = 1 << (GENERAL_DECLAR_START  + 4), 
        VF_POS3F_COLOR_UV2F             = 1 << (GENERAL_DECLAR_START  + 5), 
        VF_POS1F_COLOR_UV4B_UV4B        = 1 << (GENERAL_DECLAR_START  + 6), // for terrain
        VF_POS1F_UV4B_UV4B              = 1 << (GENERAL_DECLAR_START  + 7), // for water
        // add new defaults here...

        // Tangent stream, 4bits reserved
        VF_TANG3F                       = 1 << (TANGENT_DECLAR_START  + 0), 
                                                                            
        // Skinning stream, 4bits reserved
        VF_BONEID4B_WEIGHTS4F           = 1 << (SKINNING_DECLAR_START + 0) // for skinning
    };
}