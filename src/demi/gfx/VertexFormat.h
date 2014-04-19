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

#ifndef DiVertexFormat_h__
#define DiVertexFormat_h__


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
        VERT_USAGE_COLOR    ,
        VERT_USAGE_SECONDARY_COLOR,

        MAX_VERT_USAGE
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

    // Basically there are thee streams used in demi3d
    enum DiStreamID
    {
        STREAM_GENERAL = 0,
        STREAM_SKINNING
    };

    struct DiVFElement
    {
        enum
        {
            VF_ELEMENT_POS3F     = 1<<0,
            VF_ELEMENT_NORMAL3F  = 1<<1,
            VF_ELEMENT_TANGENT3F = 1<<2,
            
            VF_ELEMENT_UV02F     = 1<<3,
            VF_ELEMENT_UV12F     = 1<<4,
            VF_ELEMENT_COLOR     = 1<<5,
            VF_ELEMENT_BONEID4B  = 1<<6,
            VF_ELEMENT_WEIGHTS4F = 1<<7,
        
            VF_ELEMENT_NUMS = 8
        };
        
        inline static int ToVFElement(DiVertexType type, DiVertexUsage usage, uint8 id)
        {
            uint32 ret = 0;
            
            if(id == 0)
            {
                if (type == VERT_TYPE_FLOAT2 && usage == VERT_USAGE_TEXCOORD)
                    return VF_ELEMENT_UV02F;
                else if(type == VERT_TYPE_FLOAT3 && usage == VERT_USAGE_POSITION)
                    return VF_ELEMENT_POS3F;
                else if(type == VERT_TYPE_FLOAT3 && usage == VERT_USAGE_NORMAL)
                    return VF_ELEMENT_NORMAL3F;
                else if(type == VERT_TYPE_COLOR && usage == VERT_USAGE_COLOR)
                    return VF_ELEMENT_COLOR;
                else if(type == VERT_TYPE_UBYTE4 && usage == VERT_USAGE_BLENDINDICES)
                    return VF_ELEMENT_BONEID4B;
                else if(type == VERT_TYPE_FLOAT4 && usage == VERT_USAGE_BLENDWEIGHT)
                    return VF_ELEMENT_WEIGHTS4F;
            }
            else if(id == 1)
            {
                if (type == VERT_TYPE_FLOAT2 && usage == VERT_USAGE_TEXCOORD)
                    return VF_ELEMENT_UV12F;
            }
            
            return ret;
        }
        
        inline static bool IsSoftBlendingElement(int vfElement)
        {
            if(vfElement == VF_ELEMENT_POS3F ||
               vfElement == VF_ELEMENT_NORMAL3F ||
               vfElement == VF_ELEMENT_TANGENT3F)
                return true;
            else
                return false;
        }
        
        inline static void FromVFElement(int vfElement, DiVertexType& outType, DiVertexUsage& outUsage, uint8& outId)
        {
            switch(vfElement)
            {
                case VF_ELEMENT_POS3F:
                    outType = VERT_TYPE_FLOAT3;
                    outUsage = VERT_USAGE_POSITION;
                    outId = 0;
                    return;
                case VF_ELEMENT_NORMAL3F:
                    outType = VERT_TYPE_FLOAT3;
                    outUsage = VERT_USAGE_NORMAL;
                    outId = 0;
                    return;
                case VF_ELEMENT_TANGENT3F:
                    outType = VERT_TYPE_FLOAT3;
                    outUsage = VERT_USAGE_TANGENT;
                    outId = 0;
                    return;
                case VF_ELEMENT_UV02F:
                    outType = VERT_TYPE_FLOAT2;
                    outUsage = VERT_USAGE_TEXCOORD;
                    outId = 0;
                    return;
                case VF_ELEMENT_UV12F:
                    outType = VERT_TYPE_FLOAT2;
                    outUsage = VERT_USAGE_TEXCOORD;
                    outId = 1;
                    return;
                case VF_ELEMENT_COLOR:
                    outType = VERT_TYPE_COLOR;
                    outUsage = VERT_USAGE_COLOR;
                    outId = 0;
                    return;
                case VF_ELEMENT_BONEID4B:
                    outType = VERT_TYPE_UBYTE4;
                    outUsage = VERT_USAGE_BLENDINDICES;
                    outId = 0;
                    return;
                case VF_ELEMENT_WEIGHTS4F:
                    outType = VERT_TYPE_FLOAT4;
                    outUsage = VERT_USAGE_BLENDWEIGHT;
                    outId = 0;
                    return;
                default:
                    outType = VERT_TYPE_UNUSED;
                    outUsage = MAX_VERT_USAGE;
                    outId = 0;
                    return;
            }
        }
    };
}

#endif
