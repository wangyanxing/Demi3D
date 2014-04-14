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

#ifndef DiScriptManager_h__
#define DiScriptManager_h__

#include "ScriptPrerequisites.h"

namespace Demi
{
    class DI_SCRIPT_API DiScriptManager : public DiBase
    {
    public:

        DiScriptManager();

        ~DiScriptManager();

    public:
        
        DiScriptManager* Get()
        {
            DI_ASSERT(sMgr);
            return sMgr;
        }
        
        void    Init();
        
        void    Shutdown();

    private:
        static DiScriptManager* sMgr;
        
        lua_State*  mLuaState;
    };
}

#endif
