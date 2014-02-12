


#ifndef DiDrvGLPlugin_h__
#define DiDrvGLPlugin_h__


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

#endif
