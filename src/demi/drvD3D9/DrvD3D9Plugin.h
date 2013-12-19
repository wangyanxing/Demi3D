
/********************************************************************
    File:       DrvD3D9Plugin.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "EnginePlugin.h"

namespace Demi
{
    class DI_D3D9DRV_API DiD3D9Plugin : public DiPlugin
    {
    public:

        DiD3D9Plugin() : mDriver(nullptr){}

        ~DiD3D9Plugin(){}

    public:

        const DiString& GetName() const;

        void            Install();

        void            Uninstall();

    private:

        DiD3D9Driver*   mDriver;
    };
}