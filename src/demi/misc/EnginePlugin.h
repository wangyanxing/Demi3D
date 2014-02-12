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

#ifndef DiEnginePlugin_h__
#define DiEnginePlugin_h__


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

#endif
