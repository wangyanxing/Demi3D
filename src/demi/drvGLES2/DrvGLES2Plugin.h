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

#ifndef DiDrvGLES2Plugin_h__
#define DiDrvGLES2Plugin_h__

#include "EnginePlugin.h"
#include "ES2Prerequisites.h"

namespace Demi
{
    class DI_GLES2_API DiDrvGLES2Plugin : public DiPlugin
    {
    public:

        DiDrvGLES2Plugin();

        ~DiDrvGLES2Plugin(){}

    public:

        const DiString& GetName() const;

        DI_PLUGIN void  Install();

        DI_PLUGIN void  Uninstall();

    private:

        DI_PLUGIN DiGLES2Driver* mDriver;
    };
}

#endif
