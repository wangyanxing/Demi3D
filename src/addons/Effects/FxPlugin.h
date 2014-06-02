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

#ifndef DiFxPlugin_h__
#define DiFxPlugin_h__

#include "FxPrerequisites.h"
#include "EnginePlugin.h"

namespace Demi
{
    class DEMI_FX_API DiFxPlugin : public DiPlugin
    {
    public:

        DiFxPlugin(){}

        ~DiFxPlugin(){}

    public:

        const DiString& GetName() const;

        DI_PLUGIN void  Install();

        DI_PLUGIN void  Uninstall();
    };
}

#endif