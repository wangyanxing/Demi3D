
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
        case DiGpuVariable::VARIABLE_COLOR:         size = sizeof(float)*4;   break;
        case DiGpuVariable::VARIABLE_FLOAT4:        size = sizeof(float)*4;   break;
        case DiGpuVariable::VARIABLE_SAMPLER2D:
        case DiGpuVariable::VARIABLE_SAMPLERCUBE:   size = sizeof(DiTexture*);break;
        }
        DI_ASSERT(size > 0);
        return size;
    }

    DiGpuVariable::DiGpuVariable( const DiString& name, Type type )
    {
        mName    = name;
        mType    = type;
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
}