
#include "GfxPch.h"
#include "GpuProgram.h"
#include "Memory.h"
#include "Material.h"
#include "AssetManager.h"
#include "GfxDriver.h"

namespace Demi 
{
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