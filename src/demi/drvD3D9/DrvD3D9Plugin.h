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

#endif
