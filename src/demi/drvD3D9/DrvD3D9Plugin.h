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

#ifndef DiDrvD3D9Plugin_h__
#define DiDrvD3D9Plugin_h__

#include "D3D9Prerequisites.h"
#include "DrvD3D9Defines.h"
#include "EnginePlugin.h"

namespace Demi
{
    class DI_D3D9DRV_API DiDrvD3D9Plugin : public DiPlugin
    {
    public:

        DiDrvD3D9Plugin();

        ~DiDrvD3D9Plugin(){}

    public:

        const DiString& GetName() const;

        DI_PLUGIN void  Install();

        DI_PLUGIN void  Uninstall();

    private:

        DI_PLUGIN DiD3D9Driver* mDriver;
    };
}

#endif
