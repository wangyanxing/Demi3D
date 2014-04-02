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
#include "DrvD3D9Pch.h"
#include "DrvD3D9Plugin.h"
#include "D3D9Driver.h"

namespace Demi
{
    const DiString& DiDrvD3D9Plugin::GetName() const
    {
        static DiString plugin = "DrvD3D9";
        return plugin;
    }

    void DiDrvD3D9Plugin::Install()
    {
        DI_ASSERT(!mDriver);
        DI_ASSERT(!DiBase::Driver);
        mDriver = DI_NEW DiD3D9Driver();
    }

    void DiDrvD3D9Plugin::Uninstall()
    {
        DI_ASSERT(mDriver);
        DI_DELETE mDriver;
        mDriver = nullptr;
    }

    DiDrvD3D9Plugin::DiDrvD3D9Plugin()
    {
#ifndef DEMI_STATIC_API
        mDriver = nullptr;
#endif
    }

#ifndef DEMI_STATIC_API

    DiDrvD3D9Plugin* plugin = nullptr;

    extern "C" void DI_D3D9DRV_API PluginBegin() throw()
    {
        plugin = DI_NEW DiDrvD3D9Plugin();
        plugin->Install();
    }

    extern "C" void DI_D3D9DRV_API PluginEnd()
    {
        plugin->Uninstall();
        DI_DELETE plugin;
    }

#else
    DiD3D9Driver* DiDrvD3D9Plugin::mDriver = nullptr;
#endif
}