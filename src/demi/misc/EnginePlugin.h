
/********************************************************************
    File:       EnginePlugin.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

namespace Demi
{
    class DI_MISC_API DiPlugin
    {
    public:

        DiPlugin(){}

        virtual                 ~DiPlugin(){}

    public:

        virtual const DiString& GetName() const = 0;

        virtual void            Install() = 0;

        virtual void            Uninstall() = 0;

        static void             LoadPlugin(const DiString& pgName);

        static void             UnloadPlugin(const DiString& pgName);
    };
}