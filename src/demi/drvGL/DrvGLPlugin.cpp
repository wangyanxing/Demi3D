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