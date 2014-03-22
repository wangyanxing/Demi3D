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

#ifndef DiMaterialPredefine_h__
#define DiMaterialPredefine_h__


namespace Demi
{
    class DI_GFX_API DiMaterialDefine
    {
    public:

        // default material
        static DiString        DEFAULT_MATERIAL;

        // some additional parameter about shader macro
        static DiString        PARAM_VERTEX_COLOR;
        static DiString        PARAM_VERTEX_INSTANCE;

        static DiString        PARAM_INSTANCE_NONE;
        static DiString        PARAM_INSTANCE_SHADER;
        static DiString        PARAM_INSTANCE_HARDWARE;

        // terrain shaders
        static DiString        TERRAIN_VERTEX_SHADER;
        static DiString        TERRAIN_PIXEL_SHADER;
        static DiString        WATER_PIXEL_SHADER;
        static DiString        WATER_VERTEX_SHADER;

        // for sky box
        static DiString        SKY_VERTEX_SHADER;
        static DiString        SKY_PIXEL_SHADER;

        // for the output of the post effect
        static DiString        SCREEN_QUAD_VERT_SHADER;
        static DiString        SCREEN_QUAD_PIXEL_SHADER;
    };
}

#endif
