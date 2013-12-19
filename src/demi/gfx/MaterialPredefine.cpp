
#include "MaterialPredefine.h"

namespace Demi
{
    DiString DiMaterialDefine::PARAM_VERTEX_COLOR        = "USE_VERT_COLOR";
    DiString DiMaterialDefine::PARAM_VERTEX_INSTANCE    = "INSTANCE_TYPE";
    DiString DiMaterialDefine::PARAM_INSTANCE_NONE        = "0";
    DiString DiMaterialDefine::PARAM_INSTANCE_SHADER    = "1";
    DiString DiMaterialDefine::PARAM_INSTANCE_HARDWARE    = "2";

    DiString DiMaterialDefine::TEXT_SHADER_NAME = "text.psh";

    DiString DiMaterialDefine::STATIC_MESH_SHADER_NAME = "static_color.vsh";

    DiString DiMaterialDefine::TERRAIN_VERTEX_SHADER = "terrain_color.vsh";
    DiString DiMaterialDefine::TERRAIN_PIXEL_SHADER = "terrain_color.psh";

    DiString DiMaterialDefine::SKINING_1_WEIGHT_SHADER    = "skining_1_weight.vsh";
    DiString DiMaterialDefine::SKINING_2_WEIGHTS_SHADER    = "skining_2_weights.vsh";
    DiString DiMaterialDefine::SKINING_4_WEIGHTS_SHADER    = "skining_4_weights.vsh";

    DiString DiMaterialDefine::DIFFUSE_PIXEL_SHADER            = "diffuse.psh";
    DiString DiMaterialDefine::DIFFUSE_GLOW_PIXEL_SHADER    = "diffuse_glow.psh";
    DiString DiMaterialDefine::DIFFUSE_NORMAL_PIXEL_SHADER    = "diffuse_normal.psh";
    DiString DiMaterialDefine::DIFFUSE_NORMAL_SPECULAR_PIXEL_SHADER = "diffuse_normal_specular.psh";
    DiString DiMaterialDefine::DIFFUSE_NORMAL_GLOW_PIXEL_SHADER     = "diffuse_normal_glow.psh";
    DiString DiMaterialDefine::DIFFUSE_NORMAL_SPECULAR_GLOW_PIXEL_SHADER = "diffuse_normal_specular_glow.psh";
    DiString DiMaterialDefine::DIFFUSE_LIGHTMAP_SHADER        = "diffuse_lightmap.psh";

    DiString DiMaterialDefine::SCREEN_QUAD_VERT_SHADER = "screen_quad.vsh"; 
    DiString DiMaterialDefine::SCREEN_QUAD_PIXEL_SHADER = "screen_quad.psh";

    DiString DiMaterialDefine::WATER_PIXEL_SHADER = "water.psh";
    DiString DiMaterialDefine::WATER_VERTEX_SHADER = "water.vsh";

    DiString DiMaterialDefine::DEFAULT_MATERIAL = "defaultMat.mtl";

    DiString DiMaterialDefine::SKY_PIXEL_SHADER = "skybox.psh";
}