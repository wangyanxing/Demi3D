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
#include "MaterialPredefine.h"

namespace Demi
{
    DiString DiMaterialDefine::PARAM_VERTEX_COLOR       = "USE_VERT_COLOR";
    DiString DiMaterialDefine::PARAM_VERTEX_INSTANCE    = "INSTANCE_TYPE";
    DiString DiMaterialDefine::PARAM_INSTANCE_NONE      = "0";
    DiString DiMaterialDefine::PARAM_INSTANCE_SHADER    = "1";
    DiString DiMaterialDefine::PARAM_INSTANCE_HARDWARE  = "2";

    // TODO: GET RID OF THEM

    DiString DiMaterialDefine::STATIC_MESH_SHADER_NAME = "static_color_v";

    DiString DiMaterialDefine::TERRAIN_VERTEX_SHADER = "terrain_color_v";
    DiString DiMaterialDefine::TERRAIN_PIXEL_SHADER  = "terrain_color_p";

    //

    DiString DiMaterialDefine::SCREEN_QUAD_VERT_SHADER  = "screenquad_v";
    DiString DiMaterialDefine::SCREEN_QUAD_PIXEL_SHADER = "screenquad_p";

    DiString DiMaterialDefine::WATER_PIXEL_SHADER = "water_p";
    DiString DiMaterialDefine::WATER_VERTEX_SHADER = "water_v";

    DiString DiMaterialDefine::DEFAULT_MATERIAL = "defaultMat.mtl";

    DiString DiMaterialDefine::SKY_VERTEX_SHADER = "skybox_v";
    DiString DiMaterialDefine::SKY_PIXEL_SHADER = "skybox_p";
}