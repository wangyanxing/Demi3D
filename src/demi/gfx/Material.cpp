
#include "Material.h"
#include "Memory.h"
#include "LogManager.h"
#include "ShaderParam.h"
#include "MathMisc.h"
#include "Texture.h"
#include "GfxDriver.h"
#include "MaterialSerial.h"
#include "AssetManager.h"
#include "MaterialPredefine.h"
#include "ShaderManager.h"
#include "GfxDriver.h"

namespace Demi 
{
    DiAssetType DiMaterial::TYPE = ASSET_MATERIAL;

    DiMaterial::DiMaterial(const DiString& name) :
        DiAsset(name),mShaderParameter(NULL)
    {
        mOpacity        = 1.0f;
        mDepthWrite     = TRUE;
        mDepthCheck     = TRUE;
        mCullMode       = CULL_CW;
        mWireframe      = FALSE;
        mForceWireframe = FALSE;
        mBlendMode      = BLEND_REPLACE;
        mEnableVertColor= FALSE;
        mInstanceState  = INSTANCE_DISABLE;
        mShaderFlag     = 0;
        mCommonVsType   = CVT_MAX_MATS;
        mVertexShader   = NULL;
        mPixelShader    = NULL;
        mShininess      = 32.0f;

        mShaderParameter = DI_NEW DiShaderParameter(*this);
    }

    DiMaterial::~DiMaterial(void)
    {
        SAFE_DELETE(mShaderParameter);
    }

    void DiMaterial::SetCullMode( DiCullMode val )
    {
        mCullMode = val;
    }

    void DiMaterial::Bind() const
    {
        if (!mVertexShader || !mPixelShader)
            return;

        Driver->BindMaterialStates(this);

        DiShaderEnvironment &shaderEnv = *Driver->GetShaderEnvironment();

        // common lighting parameters
        shaderEnv.ambientColor   = mAmbient;
        shaderEnv.diffuseColor   = mDiffuse;
        shaderEnv.specularColor  = mSpecular;
        shaderEnv.opacity        = mOpacity;
        shaderEnv.shininess      = mShininess;

        Driver->BindShaders(mVertexShader, mPixelShader, shaderEnv);
        mShaderParameter->Bind();
    }

    BOOL DiMaterial::Load( DiDataStreamPtr data )
    {
        DiMaterialSerializer ms;
        if(ms.ParseMaterial(data,this) == FALSE)
        {
            DI_ERROR("Failed to load the material : %s",data->GetName().c_str());
            return FALSE;
        }
        return TRUE;
    }

    BOOL DiMaterial::Load()
    {
        return TRUE;    
    }

    void DiMaterial::LoadShader( const DiString& vsname,
                                const DiString& psname)
    {
        LoadVertexShader(vsname);
        LoadPixelShader(psname);
    }

    void DiMaterial::LoadVertexShader( const DiString& vsname)
    {
        if(!DiShaderManager::GetInstancePtr())
            return;

        mVertexShader = DiShaderManager::GetInstance().LoadShader(vsname,mShaderFlag);

        if (mVertexShader && mVertexShader->GetShader())
        {
            mShaderParameter->LoadVariables(mVertexShader,SHADER_VERTEX);
        }
    }

    void DiMaterial::LoadPixelShader( const DiString& psname)
    {
        if(!DiShaderManager::GetInstancePtr())
            return;

        mPixelShader = DiShaderManager::GetInstance().LoadShader(psname,mShaderFlag);

        if (mPixelShader->GetShader())
            mShaderParameter->LoadVariables(mPixelShader,SHADER_PIXEL);
    }

    BOOL DiMaterial::LoadingComplete() const
    {
        return (mVertexShader && mPixelShader)?TRUE:FALSE;
    }

    DiString DiMaterial::GetVertexShaderName() const
    {
        if (!mVertexShader)
        {
            return "";
        }
        else
        {
            return mVertexShader->GetName();
        }
    }

    DiString DiMaterial::GetPixelShaderName() const
    {
        if (!mPixelShader)
        {
            return "";
        }
        else
        {
            return mPixelShader->GetName();
        }
    }

    bool DiMaterial::HasTexture()
    {
        if (!mShaderParameter)
        {
            return false;
        }

        return mShaderParameter->HasVariableType(DiGpuVariable::VARIABLE_SAMPLER2D) || 
            mShaderParameter->HasVariableType(DiGpuVariable::VARIABLE_SAMPLERCUBE);
    }

    void DiMaterial::SetEnableVertColor( bool val )
    {
        if (mEnableVertColor != val)
        {
            mEnableVertColor = val;
        }
    }

    void DiMaterial::SetExtraParams(DiCompileDesc& desc)
    {
        desc.AddMarco(DiMaterialDefine::PARAM_VERTEX_COLOR,mEnableVertColor?"1":"0");
    }

    void DiMaterial::RecompileShaderWithMarco( DiShaderType type, 
                                              const DiPair<DiString,DiString>& marco )
    {
        DiShaderProgram* shader;
        if (type == SHADER_VERTEX)
        {
            shader = mVertexShader;
        }
        else if (type == SHADER_PIXEL)
        {
            shader = mPixelShader;
        }

        if (shader)
        {
            DiShaderInstance* si = shader->GetShader();
            si->mCompileDesc.AddMarco(marco.first,marco.second);
            si->Compile(shader->GetCode());
            mShaderParameter->LoadVariables(shader,type);
        }
    }

    DiMaterialPtr DiMaterial::QuickCreate( const DiString& vs, const DiString& ps , uint64 flag)
    {
        static int quickcreateID = 0;
        DiString name;
        name.Format("quick_%d",quickcreateID++);
        DiMaterialPtr mat = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiMaterial>(name);

        mat->SetShaderFlag(flag);
        mat->LoadShader(vs,ps);

        return mat;
    }

    DiMaterialPtr DiMaterial::QuickCreate(const DiString& name, const DiString& vs, const DiString& ps, uint64 flag )
    {
        DiMaterialPtr mat = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiMaterial>(name);

        mat->SetShaderFlag(flag);
        mat->LoadShader(vs,ps);

        return mat;
    }

    DiMaterialPtr DiMaterial::Clone( const DiString& newname )
    {
        DiMaterialPtr mat = DiAssetManager::GetInstance().CreateManualAsset<DiMaterial>(newname);

        if (mVertexShader)
            mat->LoadVertexShader(mVertexShader->GetName());
        else
        {
            DI_WARNING("No Vertex Shader in this material");
        }

        if (mPixelShader)
            mat->LoadPixelShader(mPixelShader->GetName());
        else
        {
            DI_WARNING("No Pixel Shader in this material");
        }

        mat->mCullMode         = mCullMode;
        mat->mDepthWrite     = mDepthWrite;
        mat->mDepthCheck     = mDepthCheck;
        mat->mWireframe         = mWireframe;
        mat->mForceWireframe = mForceWireframe;
        mat->mEnableVertColor= mEnableVertColor;
        mat->mBlendMode         = mBlendMode;

        mShaderParameter->CloneVarsTo(mat->GetShaderParameter());
        
        return mat;
    }

    void DiMaterial::SetInstanceType( DiInstanceType val )
    {
        if (mInstanceState != val)
            mInstanceState = val;
    }
}

bool DiMaterial::IsTransparent() const
{
    return mBlendMode == BLEND_ONE_INV_ALPHA || mBlendMode == BLEND_ALPHA;
}