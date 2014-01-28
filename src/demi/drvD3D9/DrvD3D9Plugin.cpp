
#include "DrvD3D9Pch.h"
#include "DrvD3D9Plugin.h"
#include "D3D9Driver.h"

namespace Demi
{
    const DiString& DiD3D9Plugin::GetName() const
    {
        static DiString plugin = "DrvD3D9";
        return plugin;
    }

    void DiD3D9Plugin::Install()
    {
        DI_ASSERT(!mDriver);
        DI_ASSERT(!DiBase::Driver);
        mDriver = DI_NEW DiD3D9Driver();
    }

    void DiD3D9Plugin::Uninstall()
    {
        DI_ASSERT(mDriver);
        DI_DELETE mDriver;
        mDriver = nullptr;
    }

    DiD3D9Plugin* plugin = nullptr;

    extern "C" void DI_D3D9DRV_API PluginBegin() throw()
    {
        plugin = DI_NEW DiD3D9Plugin();
        plugin->Install();
    }

    extern "C" void DI_D3D9DRV_API PluginEnd()
    {
        plugin->Uninstall();
        DI_DELETE plugin;
    }
}