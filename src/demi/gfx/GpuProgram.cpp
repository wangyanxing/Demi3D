
#include "GpuProgram.h"
#include "Memory.h"
#include "Material.h"
#include "AssetManager.h"
#include "GfxDriver.h"

namespace Demi 
{
    static uint32 GetVariableTypeSize(DiGpuVariable::Type type)
    {
        uint32 size = 0;
        switch(type)
        {
        case DiGpuVariable::VARIABLE_FLOAT:         size = sizeof(float)*1;   break;
        case DiGpuVariable::VARIABLE_FLOAT2:        size = sizeof(float)*2;   break;
        case DiGpuVariable::VARIABLE_FLOAT3:        size = sizeof(float)*3;   break;
        case DiGpuVariable::VARIABLE_COLOR:            size = sizeof(float)*4;   break;
        case DiGpuVariable::VARIABLE_FLOAT4:        size = sizeof(float)*4;   break;
        case DiGpuVariable::VARIABLE_SAMPLER2D:
        case DiGpuVariable::VARIABLE_SAMPLERCUBE:    size = sizeof(DiTexture*);break;
        }
        DI_ASSERT(size > 0);
        return size;
    }

    DiGpuVariable::DiGpuVariable( const DiString& name, Type type )
    {
        mName    = name;
        mType    = type;
        mPublicVar = false;
    }

    DiGpuVariable::~DiGpuVariable( void )
    {
    }

    const DiString& DiGpuVariable::GetName( void ) const
    {
        return mName;
    }

    DiGpuVariable::Type DiGpuVariable::GetType( void ) const
    {
        return mType;
    }

    uint32 DiGpuVariable::GetDataSize( void ) const
    {
        return GetVariableTypeSize(mType);
    }

    void DiGpuVariable::ParsePublicHelperScript()
    {
        mPublicVar = true;

        DiVector<DiString> tokens = mHelperScript.Tokenize(";");
        
        for (size_t i=0; i < tokens.size(); i++)
        {
            DiString& str = tokens[i];
            str.Trim(" \t\r\n\0x9");

            if (str.empty())
                continue;

            DiVector<DiString> st = str.Tokenize("=");

            if (st.size() != 2)
            {
                DI_WARNING("Invalid helper shader script!");
            }
            else
            {
                st[0].Trim(" \t\r\n\0x9");
                st[1].Trim(" \t\r\n\0x9");

                if(st[0].CompareNoCase("name") == 0)
                    mHelperData.refName = st[1];
                else if (st[0].CompareNoCase("desc") == 0)
                    mHelperData.desc = st[1];
                else if (st[0].CompareNoCase("color") == 0)
                    mHelperData.asColor = st[1].AsBool();
                else if (st[0].CompareNoCase("min") == 0)
                    mHelperData.minValue = st[1].AsFloat();
                else if (st[0].CompareNoCase("max") == 0)
                    mHelperData.maxValue = st[1].AsFloat();
                else if (st[0].CompareNoCase("step") == 0)
                    mHelperData.stepValue = st[1].AsFloat();
            }
        }
    }

    DiShaderEnvironment::DiShaderEnvironment( void )
    {
        memset(this, 0, sizeof(*this));
    }

    void DiShaderEnvironment::SetIdentity()
    {
        DiMat4 dxMat;
        dxMat.setIdentity();
        dxMat[2][0] = (dxMat[2][0] + dxMat[3][0]) / 2;
        dxMat[2][1] = (dxMat[2][1] + dxMat[3][1]) / 2;
        dxMat[2][2] = (dxMat[2][2] + dxMat[3][2]) / 2;
        dxMat[2][3] = (dxMat[2][3] + dxMat[3][3]) / 2;
        viewMatrix = DiMat4::IDENTITY;
        projMatrix = dxMat;
        viewProjMatrix = dxMat * viewMatrix;
    }

    

    void DiCompileDesc::AddMarco( const DiString& name, const DiString& def )
    {
        DiMarcoDefineList::iterator it;
        DiMarcoDefineList::iterator itEnd = marcos.end();
        for (it = marcos.begin(); it != itEnd; ++it)
        {
            if (it->first == name)
            {
                it->second = def;
                return;
            }
        }
        
        marcos.push_back(DiMarcoDefine(name,def));
    }

    void DiCompileDesc::RemoveMarco( const DiString& name )
    {
        DiMarcoDefineList::iterator it;
        DiMarcoDefineList::iterator itEnd = marcos.end();
        for (it = marcos.begin(); it != itEnd; ++it)
        {
            if (it->first == name)
            {
                marcos.erase(it);
                return;
            }
        }
    }

    DiGpuVariable::HelperStruct::HelperStruct()
    {
        maxValue    = 100;
        minValue    = 0;
        stepValue    = 0.1f;
        asColor        = false;
    }
}