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
