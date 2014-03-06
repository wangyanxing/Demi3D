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
#include "Q3BspShader.h"
#include "Material.h"
#include "AssetManager.h"
#include "ShaderManager.h"
#include "ShaderParam.h"

namespace Demi
{
    DiQ3Material::DiQ3Material(const DiString& name)
        :mName(name)
        , numPasses(0)
        , deformFunc(DEFORM_FUNC_NONE)
        , farbox(false)
        , skyDome(false)
        , flags(0)
        , fog(false)
    {
    }
    
    DiQ3Material::~DiQ3Material()
    {
    }
    
    DiMaterialPtr DiQ3Material::CreateAsMaterial(int lightmapNumber, DiTexturePtr lightmap)
    {
        DiString matName;
        matName.Format("%s_%d", mName.c_str(), lightmapNumber);

        DI_DEBUG("Converting Q3 shader: %s", mName.c_str());

        DiMaterialPtr mat = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiMaterial>(matName);
        DiTexturePtr map, lm; // record to set

        for (int p = 0; p < numPasses; ++p)
        {
            DiTexturePtr t;

            // lightmap
            if (pass[p].textureName == "$lightmap")
            {
                t = lightmap;
                lm = t;
            }
            // animated texture
            else if (pass[p].animNumFrames > 0)
            {
                float sequenceTime = pass[p].animNumFrames / pass[p].animFps;
                // TODO
            }
            // ordinary texture
            else
            {
                if (!DiAssetManager::GetInstance().AssetExists(pass[p].textureName))
                {
                    // Try alternate extension
                    pass[p].textureName = GetAlternateTextureName(pass[p].textureName);
                    if (!DiAssetManager::GetInstance().AssetExists(pass[p].textureName))
                    {
                        DI_DEBUG("Cannot locate the texture: %s", pass[p].textureName.c_str());
                        continue;
                    }
                }
                t = DiAssetManager::GetInstance().GetAsset<DiTexture>(pass[p].textureName);
                map = t;
            }

            // Blending
            if (p == 0)
            {
                mat->SetBlendMode(pass[p].blendMode);
                if (mat->IsTransparent())
                    mat->SetDepthWrite(false);

                // TODO alpha test
            }
            else
            {
                
            }

            // Tex coords
            if (pass[p].texGen == TEXGEN_BASE)
            {
                t->SetUvSet(0);
            }
            else if (pass[p].texGen == TEXGEN_LIGHTMAP)
            {
                t->SetUvSet(1);
            }
            else if (pass[p].texGen == TEXGEN_ENVIRONMENT)
            {
            }

            // texture uv scale
            t->SetUScale(pass[p].tcModScale[0]);
            t->SetVScale(pass[p].tcModScale[1]);

            if (pass[p].texGen != TEXGEN_ENVIRONMENT)
            {
                if (pass[p].tcModRotate)
                {
                    //t->setRotateAnimation(pass[p].tcModRotate);
                }
                if (pass[p].tcModScroll[0] || pass[p].tcModScroll[1])
                {
                    if (pass[p].tcModTurbOn)
                    {
                        // Turbulent scroll
                        if (pass[p].tcModScroll[0])
                        {
                            //t->setTransformAnimation(TextureUnitState::TT_TRANSLATE_U, WFT_SINE,
                            //    pass[p].tcModTurb[0], pass[p].tcModTurb[3], pass[p].tcModTurb[2], pass[p].tcModTurb[1]);
                        }
                        if (pass[p].tcModScroll[1])
                        {
                            //t->setTransformAnimation(TextureUnitState::TT_TRANSLATE_V, WFT_SINE,
                            //    pass[p].tcModTurb[0], pass[p].tcModTurb[3], pass[p].tcModTurb[2], pass[p].tcModTurb[1]);
                        }
                    }
                    else
                    {
                        // Constant scroll
                        //t->setScrollAnimation(pass[p].tcModScroll[0], pass[p].tcModScroll[1]);
                    }
                }
                if (pass[p].tcModStretchWave != SHADER_FUNC_NONE)
                {
                    DiWaveformType wft = WFT_SINE;
                    switch (pass[p].tcModStretchWave)
                    {
                    case SHADER_FUNC_SIN:
                        wft = WFT_SINE;
                        break;
                    case SHADER_FUNC_TRIANGLE:
                        wft = WFT_TRIANGLE;
                        break;
                    case SHADER_FUNC_SQUARE:
                        wft = WFT_SQUARE;
                        break;
                    case SHADER_FUNC_SAWTOOTH:
                        wft = WFT_SAWTOOTH;
                        break;
                    case SHADER_FUNC_INVERSESAWTOOTH:
                        wft = WFT_INVERSE_SAWTOOTH;
                        break;
                    default:
                        break;
                    }
                    // Create wave-based stretcher
                    //t->setTransformAnimation(TextureUnitState::TT_SCALE_U, wft, pass[p].tcModStretchParams[3],
                    //    pass[p].tcModStretchParams[0], pass[p].tcModStretchParams[2], pass[p].tcModStretchParams[1]);
                    //t->setTransformAnimation(TextureUnitState::TT_SCALE_V, wft, pass[p].tcModStretchParams[3],
                    //    pass[p].tcModStretchParams[0], pass[p].tcModStretchParams[2], pass[p].tcModStretchParams[1]);
                }
            }

            //(pass[p].addressMode);
            t->SetAddressing(pass[p].addressMode);
        }

        mat->SetShaderFlag(lm ? SHADER_FLAG_USE_LIGHTMAP : 0);
        mat->LoadShader("base_v", "base_p");

        DiShaderParameter* sm = mat->GetShaderParameter();

        if (map)
            sm->WriteTexture2D("map", map);
        if (lm)
            sm->WriteTexture2D("lightmap", lm);

        return mat;
    }

    DiString DiQ3Material::GetAlternateTextureName(const DiString& texName)
    {
        DiString name = texName;
        name = name.ExtractBaseName();
        if (texName.CheckFileExtension("jpg"))
            return name + ".tga";
        else
            return name + ".jpg";
        return name;
    }
}