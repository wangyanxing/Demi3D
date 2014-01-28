
#include "DrvGLPch.h"
#include "DrvGLPlugin.h"
#include "GLDriver.h"

namespace Demi
{
    const DiString& DiGLPlugin::GetName() const
    {
        static DiString plugin = "DrvD3D9";
        return plugin;
    }

    void DiGLPlugin::Install()
    {
        DI_ASSERT(!mDriver);
        DI_ASSERT(!DiBase::Driver);
        mDriver = DI_NEW DiGLDriver();
    }

    void DiGLPlugin::Uninstall()
    {
        DI_ASSERT(mDriver);
        DI_DELETE mDriver;
        mDriver = nullptr;
    }

    DiGLPlugin* plugin = nullptr;

    extern "C" void DI_GLDRV_API PluginBegin() throw()
    {
        plugin = DI_NEW DiGLPlugin();
        plugin->Install();
    }

    extern "C" void DI_GLDRV_API PluginEnd()
    {
        plugin->Uninstall();
        DI_DELETE plugin;
    }
}