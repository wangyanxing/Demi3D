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

#ifndef DiScriptPlugin_h__
#define DiScriptPlugin_h__

#include "GfxPch.h"
#include "ScriptDefines.h"
#include "EnginePlugin.h"

namespace Demi
{
    class DI_SCRIPT_API DiScriptPlugin : public DiPlugin
    {
    public:

        DiScriptPlugin();

        ~DiScriptPlugin(){}

    public:

        const DiString& GetName() const;

        DI_PLUGIN void  Install();

        DI_PLUGIN void  Uninstall();
    };
}

#endif
