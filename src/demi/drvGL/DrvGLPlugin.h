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

#include "GLPrerequisites.h"
#include "EnginePlugin.h"

namespace Demi
{
    class DI_GLDRV_API DiDrvGLPlugin : public DiPlugin
    {
    public:

        DiDrvGLPlugin();

        ~DiDrvGLPlugin(){}

    public:

        const DiString& GetName() const;

        DI_PLUGIN void  Install();

        DI_PLUGIN void  Uninstall();

    private:

        DI_PLUGIN DiGLDriver* mDriver;
    };
}

#endif
