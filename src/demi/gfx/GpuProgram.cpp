
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

    void DiShaderEnvironment::SetIdentity(bool rtFliping)
    {
        DiMat4 projMat = DiMat4::IDENTITY;
        DiBase::Driver->ConvertProjectionMatrix(projMat, projMat);

        // actually texture flipping only for openGL
        if (rtFliping)
        {
            projMat[1][0] = -projMat[1][0];
            projMat[1][1] = -projMat[1][1];
            projMat[1][2] = -projMat[1][2];
            projMat[1][3] = -projMat[1][3];
        }
       
        viewMatrix = DiMat4::IDENTITY;
        projMatrix = projMat;
        viewProjMatrix = projMat * viewMatrix;
    }

    void DiCompileDesc::AddMarco( const DiString& name, const DiString& def )
    {
        for (auto it = marcos.begin(); it != marcos.end(); ++it)
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
        for (auto it = marcos.begin(); it != marcos.end(); ++it)
        {
            if (it->first == name)
            {
                marcos.erase(it);
                return;
            }
        }
    }
}