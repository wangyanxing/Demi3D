
#include "ShaderProgram.h"
#include "AssetManager.h"
#include "GfxDriver.h"

namespace Demi 
{
    DiString DiShaderProgram::sDefaultPsProfile = "ps_3_0";
    DiString DiShaderProgram::sDefaultVsProfile = "vs_3_0";

    DiShaderProgram::DiShaderProgram( const DiString& name )
        : mFileName(name),
        mShaderType(SHADER_UNKNOW),
        mShader(NULL)
    {

    }

    DiShaderProgram::~DiShaderProgram()
    {
        SAFE_DELETE(mShader);
    }

    Demi::DiAssetType DiShaderProgram::GetAssetType() const
    {
        return ASSET_SHADER;
    }

    BOOL DiShaderProgram::Load( DiDataStreamPtr data )
    {
        mShaderFileName = data->GetName();
        mShaderFileName.ToLower();

        if (mShaderFileName.CheckFileExtension("vsh"))
        {
            mShaderType = SHADER_VERTEX;
        }
        else if (mShaderFileName.CheckFileExtension("psh"))
        {
            mShaderType = SHADER_PIXEL;
        }

        mRowCodes = data->GetAsString();
        return mRowCodes.empty()?FALSE:TRUE;
    }

    BOOL DiShaderProgram::Load()
    {
        return Load(mFileName);
    }

    BOOL DiShaderProgram::Load( const DiString& filename )
    {
        DiDataStreamPtr buf = DiAssetManager::GetInstance().OpenArchive(filename);
        if (!buf)
        {
            return FALSE;
        }

        Load(buf);

        return TRUE;
    }

    BOOL DiShaderProgram::LoadingComplete() const
    {
        return mRowCodes.empty()?FALSE:TRUE;
    }

    void DiShaderProgram::SetCode( DiShaderType type, const DiString& str )
    {
        mShaderType = type;
        mRowCodes = str;
    }

    void DiShaderProgram::Compile( const DiCompileDesc& desc )
    {
        if (!Driver)
            return;

        SAFE_DELETE(mShader);

        if(mShaderType == SHADER_VERTEX)
        {
            mShader = Driver->CreateVSInstance(this);
        }
        else if (mShaderType == SHADER_PIXEL)
        {
            mShader = Driver->CreatePSInstance(this);
        }
        else
        {
            DI_WARNING("Unsupported shader type.");
            return;
        }

        mShader->mCompileDesc = desc;
        ParseHelperScript();

        if (mCodes.empty())
        {
            DI_WARNING("Empty shader code.");
            return;
        }

        mShader->Compile(mCodes);
    }

    void DiShaderProgram::Compile( )
    {
        if (!Driver)
        {
            return;
        }

        DiCompileDesc desc;
        
        if(mShaderType == SHADER_VERTEX)
        {
            desc.entryName    = "vs_main";
            desc.profile    = sDefaultVsProfile;
        }
        else
        {
            desc.entryName = "ps_main";
            desc.profile    = sDefaultPsProfile;
        }

        Compile(desc);
    }

    void DiShaderProgram::Bind( const DiShaderEnvironment& shaderEnv )
    {
        if (mShader)
        {
            mShader->Bind(shaderEnv);
        }
    }

    DiShaderInstance* DiShaderProgram::GetShader( )
    {
        return mShader;
    }

    void DiShaderProgram::ParseHelperScript()
    {
        if(mRowCodes.length() < 2)
        {
            return;
        }

        if (!mShader)
        {
            return;
        }

        mVariableScripts.clear();
        mCodes = mRowCodes;

        const char* cd = mCodes.c_str();
        int currentBlockBegin = -1;
        int currentBlockEnd = -1;
        DiVector<DiIntVec2> blocks;

        int c = 0;
        while (c < mCodes.length()-1)
        {
            if (currentBlockBegin != -1)
            {
                if(cd[c] == '-' && cd[c+1] == '>')
                {
                    currentBlockEnd = c+1;

                    mCodes[c] = '*';
                    mCodes[c+1] = '/';

                    blocks.push_back(DiIntVec2(currentBlockBegin,currentBlockEnd));
                    
                    currentBlockBegin = -1;
                    currentBlockEnd = -1;
                    c+=2;
                    continue;
                }
            }
            else
            {
                if(cd[c] == '<' && cd[c+1] == '-')
                {
                    mCodes[c] = '/';
                    mCodes[c+1] = '*';

                    currentBlockBegin = c;
                    c+=2;
                    continue;
                }
            }
            c++;
        }

        if (currentBlockBegin != -1 || currentBlockEnd != -1)
        {
            DI_ERROR("shader编译错误，参数脚本标记代码块不匹配");
        }

        if (blocks.empty())
        {
            return;
        }

        for (size_t i=0; i < blocks.size(); i++)
        {
            if (blocks[i].y - blocks[i].x < 3)
            {
                DI_ERROR("shader编译错误，参数脚本标记代码块错误");
            }
            DiString script = mCodes.SubString(blocks[i].x+2,blocks[i].y-blocks[i].x-3);
            
            int vnEnd = -1;
            int vnStart = -1;
            for (int cx = blocks[i].x-1;cx>=0;cx--)
            {
                char t = mCodes[cx];
                if (t == ';')
                {
                    vnEnd = cx;
                    continue;
                }

                if (vnEnd != -1 )
                {
                    if(t <= 32)
                    {
                        vnStart = cx;
                        break;
                    }
                }
            }

            if (vnStart == -1 || vnEnd == -1)
            {
                DI_ERROR("shader编译错误，参数脚本标记代码块错误，找不到对应的变量名");
            }
            DiString varName = mCodes.SubString(vnStart+1,vnEnd-vnStart-1);
            
            mVariableScripts[varName] = script;
        }
    }

}