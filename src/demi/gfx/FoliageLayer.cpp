
#include "FoliageLayer.h"
#include "AssetManager.h"
#include "ShaderParam.h"

namespace Demi
{
    DiFoliageLayer::DiFoliageLayer( DiFoliageMap* map , DiFoliageLayerDesc* desc)
        :mDesc(desc),
        mNeedUpdateMaterial(true),
        mDensityMap(NULL)
    {
        mWaveCount = 0;
        mDensityMap = DI_NEW DiByteMap(desc->mDensityMap,FOLIAGE_DENSITY_MAP_SIZE,FOLIAGE_DENSITY_MAP_SIZE);

        CreateMaterial();
    }

    DiFoliageLayer::~DiFoliageLayer()
    {
        SAFE_DELETE(mDensityMap);
    }

    void DiFoliageLayer::SetTextureName( const DiString& val )
    {
        mDesc->DemiureName = val;
        mNeedUpdateMaterial = true;
    }

    void DiFoliageLayer::UpdateMaterial()
    {
        if (!mNeedUpdateMaterial)
            return;

        mNeedUpdateMaterial = false;

        DI_ASSERT(mMaterial);

        DiCompileDesc desc;
        desc.entryName = "vs_main";
        desc.profile = DiShaderProgram::GetDefaultVsProfile();

        desc.marcos.push_back(DiPair<DiString,DiString>("_GRASS_FADE_ALPHA"     ,"0"));
        desc.marcos.push_back(DiPair<DiString,DiString>("_GRASS_FADE_GROW"      ,"1"));
        desc.marcos.push_back(DiPair<DiString,DiString>("_GRASS_FADE_ALPHA_GROW","2"));

        desc.marcos.push_back(DiPair<DiString,DiString>("_GRASS_RENDER_QUAD"    ,"0"));
        desc.marcos.push_back(DiPair<DiString,DiString>("_GRASS_RENDER_SPRITE"  ,"1"));

        DiString fadeMarco;
        fadeMarco = "0";

        desc.marcos.push_back(DiPair<DiString,DiString>("_GRASS_LIGHTING"   , mDesc->mLighting?  "1":"0"));
        desc.marcos.push_back(DiPair<DiString,DiString>("_GRASS_ANIMATE"    , mDesc->mAnimate?   "1":"0"));
        desc.marcos.push_back(DiPair<DiString,DiString>("_GRASS_BLEND"      , "1"));
        desc.marcos.push_back(DiPair<DiString,DiString>("_GRASS_FADE_TECH"  , fadeMarco));
        desc.marcos.push_back(DiPair<DiString,DiString>("_GRASS_RENDER_TECH", mDesc->mRenderTechnique == GRASSTECH_SPRITE ? "1" : "0"));

        DiShaderParameter* msp = mMaterial->GetShaderParameter();
        DiShaderProgram* vp = mMaterial->GetVertexShader();

        if (vp)
        {
            DiShaderInstance* si = vp->GetShader();
            if (si)
            {
                si->mCompileDesc = desc;
                si->Compile(vp->GetCode());
                msp->LoadVariables(vp,SHADER_VERTEX);
            }
        }

        msp->WriteFloat("fadeRange", 100);
        msp->WriteTexture2D("diffuseTexture",mDesc->DemiureName);
    }

    void DiFoliageLayer::CreateMaterial()
    {
        DiString uniqueStr;
        static int uq = 0;
        uniqueStr.Format("_fol_mat_%d",uq++);

        DiCompileDesc desc;
        desc.entryName = "vs_main";
        desc.profile = DiShaderProgram::GetDefaultVsProfile();

        desc.marcos.push_back(DiPair<DiString,DiString>("_GRASS_FADE_ALPHA"     ,"0"));
        desc.marcos.push_back(DiPair<DiString,DiString>("_GRASS_FADE_GROW"      ,"1"));
        desc.marcos.push_back(DiPair<DiString,DiString>("_GRASS_FADE_ALPHA_GROW","2"));

        desc.marcos.push_back(DiPair<DiString,DiString>("_GRASS_LIGHTING"       ,"0"));
        desc.marcos.push_back(DiPair<DiString,DiString>("_GRASS_ANIMATE"        ,"0"));
        desc.marcos.push_back(DiPair<DiString,DiString>("_GRASS_BLEND"          ,"0"));
        desc.marcos.push_back(DiPair<DiString,DiString>("_GRASS_FADE_TECH"      ,"0"));
        desc.marcos.push_back(DiPair<DiString,DiString>("_GRASS_RENDER_TECH"    ,"0"));

        mMaterial = DiAssetManager::GetInstance().CreateManualAsset<DiMaterial>(uniqueStr);

        mMaterial->SetBlendMode(BLEND_ALPHA);
        mMaterial->SetCullMode(CULL_NONE);

        mMaterial->LoadShader("grass.vsh", "unlighting_diffuse_color.psh");

        mNeedUpdateMaterial = true;
    }

    void DiFoliageLayer::SetMinimumSize( float width, float height )
    {
        mDesc->mMinWidth = width;
        mDesc->mMinHeight = height;
    }

    void DiFoliageLayer::SetMaximumSize( float width, float height )
    {
        mDesc->mMaxWidth = width;
        if (mDesc->mMaxHeight != height)
        {
            mDesc->mMaxHeight = height;
            mNeedUpdateMaterial = true;
        }
    }

    void DiFoliageLayer::SetRenderTechnique( GrassTechnique style, BOOL blendBase /*= false*/ )
    {
        if (mDesc->mRenderTechnique != style)
        {
            mDesc->mRenderTechnique = (uint8)style;
            mNeedUpdateMaterial = true;
        }
    }

    void DiFoliageLayer::SetAnimationEnabled( BOOL enabled )
    {
        if (mDesc->mAnimate != enabled)
        {
            mDesc->mAnimate = enabled;
            mNeedUpdateMaterial = true;
        }
    }

    void DiFoliageLayer::SetLightingEnabled( BOOL enabled )
    {
        if (mDesc->mLighting != enabled)
        {
            mDesc->mLighting = enabled;
            mNeedUpdateMaterial = true;
        }
    }

    void DiFoliageLayer::SetDensity( float density )
    {
        mDesc->mDensity = density;
    }

    void DiFoliageLayer::SetSwayLength( float mag )
    {
        mDesc->mAnimMag = mag;
    }

    void DiFoliageLayer::SetSwaySpeed( float speed )
    {
        mDesc->mAnimSpeed = speed;
    }

    void DiFoliageLayer::SetSwayDistribution( float freq )
    {
        mDesc->mAnimFreq = freq;
    }

    void DiFoliageLayer::SetColor( const DiColor& c )
    {
        mDesc->mColor = c.GetAsARGB();
    }
}