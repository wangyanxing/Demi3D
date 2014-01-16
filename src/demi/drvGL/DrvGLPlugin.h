
/********************************************************************
    File:       DrvGLPlugin.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "EnginePlugin.h"

namespace Demi
{
    class DI_GLDRV_API DiGLPlugin : public DiPlugin
    {
    public:

        DiGLPlugin() : mDriver(nullptr){}

        ~DiGLPlugin(){}

    public:

        const DiString& GetName() const;

        void            Install();

        void            Uninstall();

    private:

        DiGLDriver*     mDriver;
    };
}