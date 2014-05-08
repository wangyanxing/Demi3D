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
        
        static DiScriptManager* Get()
        {
            DI_ASSERT(sMgr);
            return sMgr;
        }
        
        void        Init();

        void        Shutdown();

        void        RunFile(const DiString& fileName);

        void        RunBuffer(DiDataStreamPtr data);

        void        RunBuffer(const char* data, uint32 size, const DiString& name);

        lua_State*  GetLuaState() { return mLuaState; }

        void        SetBaseScriptPath(const DiString& path);

    public:

        void    BindMiscLib();

        void    BindGfxLib();

    private:

        int initErrorHandlerFunc();

        int initErrorHandlerFunc(const DiString& func);

        static DiScriptManager* sMgr;
        
        lua_State*  mLuaState;

        /// for other platforms,"../../scripts"
        /// for iOS,"./scripts"
        DiString    mBasePath;

        bool d_ownsState;

        DiString d_errFuncName;
        int d_errFuncIndex;
        DiString d_activeErrFuncName;
        int d_activeErrFuncIndex;
    };
}

#endif
