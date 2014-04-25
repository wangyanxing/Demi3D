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
#include "GfxPch.h"
#include "Command.h"
#include "Console.h"
#include "ConsoleVariable.h"

inline int GetCharPrio( char x ) 
{
    if(x>='a' && x<='z')
    {
        x+='A'-'a';
    }

    if(x=='_')
    {
        return 300;
    }
    else return x;
}

inline bool less_CVar( const char* left,const char* right )
{
    for(;;)
    {
        uint32 l=GetCharPrio(*left), r=GetCharPrio(*right);

        if(l<r)
            return true;
        if(l>r)
            return false;

        if(*left==0 || *right==0)
            break;

        ++left;++right;
    }

    return false;
}

namespace Demi
{
    DiConsoleLogger* DiCommandManager::sConsoleLogger = nullptr;

    int DiCmdArgs::GetArgCount() const
    {
        int iArgCount = mKeyList.size();
        return iArgCount;
    }

    const DiString& DiCmdArgs::GetArg(uint32 index) const
    {
        static DiString unfound;

        if (index < mKeyList.size())
            return mKeyList[index];
        else
            return unfound;
    }

    const DiString& DiCmdArgs::GetCommandLine() const
    {
        return mCommand;
    }

    DiKeyList& DiCmdArgs::GetKeyList()
    {
        return mKeyList;
    }

    DiString& DiCmdArgs::GetCommand()
    {
        return mCommand;
    }

    DiCmdArgs::DiCmdArgs(int argc, const char** argv)
    {
        if (argc <= 0)
            return;

        mCommand = argv[0];

        for (int i = 1; i < argc; i++)
            mKeyList.push_back(argv[i]);
    }

    DiCommandManager::DiCommandManager(void)
    {
        Init();
    }

    DiCommandManager::~DiCommandManager(void)
    {
        Shutdown();
    }

    bool DiCommandManager::Init()
    {
        DI_INFO("Command manager is initializing...");

        CommandMgr = this;
        DiSystemCmd::BindAllSystemCmd(this);
        
#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
        InitConsoleLogger(true);
#endif
        return true;
    }

    void DiCommandManager::Shutdown()
    {
#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
        DestroyConsoleLogger();
#endif

        for (auto it = mMapVariables.begin(); it != mMapVariables.end(); ++it)
        {
            DI_DELETE it->second;
        }

        mMapVariables.clear();
        CommandMgr = nullptr;
    }

    bool DiCommandManager::AddCommand(const DiString& cmd, DiCmdFuntion pFunction, const char* pcHelp)
    {
        DI_ASSERT(pcHelp);
        DI_ASSERT(pFunction);

        if (!pFunction || !pcHelp)
            return false;

        DiVector<DiString> splitList = cmd.Tokenize(" ");

        if (splitList.empty())
            return false;

        DiCmdInfo kCmdInfo;

        kCmdInfo.mName = splitList[0];
        kCmdInfo.mDesc = pcHelp;
        kCmdInfo.mFunc = pFunction;

        kCmdInfo.mName.ToLower();

        mCmdInfos[kCmdInfo.mName] = kCmdInfo;

        return true;
    }

    bool DiCommandManager::ExecuteCommand(const DiString& cmd)
    {
        if (cmd.size() < 1)
            return false;
        
        // this is a comment
        if (cmd[0] == '#')
            return true;

        DiCmdArgs argus;

        DiString& command = argus.GetCommand();
        DiKeyList& splitList = argus.GetKeyList();

        DiString cmds = cmd;
        cmds.Tokenize(" ", splitList);

        if (splitList.empty())
            return true;

        command = cmd;

        DiString& kName = splitList.at(0);

        kName.ToLower();

        DiStringCmdInfoMap::iterator it = mCmdInfos.find(kName);
        if (it == mCmdInfos.end())
        {
            DI_WARNING( "can't find command. [%s]", kName.c_str());
            return false;
        }

        DiCmdInfo kCmdInfo = it->second;

        bool iRlt = kCmdInfo.mFunc(&argus);
        if (!iRlt)
        {
            DI_WARNING( "execute failed. [%s]", command.c_str());
            return false;
        }

        return true;
    }

    int DiCommandManager::GetNumVars()
    {
        return (int)mMapVariables.size();
    }

    void DiCommandManager::RegisterVar( DiConsoleVar *consVar,ConsoleVarFunc callFunc/*=0 */ )
    {
        auto it = mConfigVars.find( consVar->GetName() );
        if (it != mConfigVars.end())
        {
            consVar->Set( it->second.c_str() );
            mConfigVars.erase(it);
        }
        else
            consVar->ClearFlags(VAR_FLAG_MODIFIED);

        if (callFunc)
            consVar->SetOnChangeCallback(callFunc);
        
        mMapVariables.insert(ConsoleVariablesMapItor::value_type(consVar->GetName(),consVar));
    }

    DiConsoleVar* DiCommandManager::RegisterString( const DiString& varName,const DiString& varValue,int flags, 
        const DiString& desc /*= ""*/,ConsoleVarFunc callFunc/*=0 */ )
    {
        DiConsoleVar *consVar = nullptr;

        auto it = mMapVariables.find(varName);
        if (it != mMapVariables.end())
            consVar = it->second;

        if (consVar)
            return consVar;

        consVar = DI_NEW DiConsoleVariableString(this,varName,varValue,flags,desc);
        RegisterVar( consVar,callFunc );
        return consVar;
    }

    DiConsoleVar* DiCommandManager::RegisterInt( const DiString& varName,int iValue,int flags, 
        const DiString& desc /*= ""*/,ConsoleVarFunc callFunc/*=0 */ )
    {
        DiConsoleVar *consVar = nullptr;

        auto it = mMapVariables.find(varName);
        if (it != mMapVariables.end())
            consVar = it->second;
        
        if (consVar)
            return consVar;

        consVar = DI_NEW DiConsoleVariableInt(this,varName,iValue,flags,desc);
        RegisterVar( consVar,callFunc );
        return consVar;
    }

    DiConsoleVar* DiCommandManager::RegisterFloat( const DiString& varName,float fValue,int flags,
        const DiString& desc /*= ""*/,ConsoleVarFunc callFunc/*=0 */ )
    {
        DiConsoleVar *consVar = nullptr;

        auto it = mMapVariables.find(varName);
        if (it != mMapVariables.end())
            consVar = it->second;
        
        if (consVar)
            return consVar;

        consVar = DI_NEW DiConsoleVariableFloat(this,varName,fValue,flags,desc);
        RegisterVar( consVar,callFunc );
        return consVar;
    }

    bool DiCommandManager::BeforeChangingVar(DiConsoleVar *variable, const DiString& newValue)
    {
        if (!mConsoleVarListener.empty())
        {
            for (auto it = mConsoleVarListener.begin(); it != mConsoleVarListener.end(); ++it)
            {
                if (!(*it)->BeforeChangingVar(variable,newValue))
                    return false;
            }
        }
        return true;
    }

    void DiCommandManager::AfterChangingVar( DiConsoleVar* variable )
    {
        if (!mConsoleVarListener.empty())
        {
            for (auto it = mConsoleVarListener.begin(); it != mConsoleVarListener.end(); ++it)
            {
                (*it)->AfterChangingVar(variable);
            }
        }
    }

    void DiCommandManager::AddConsoleVarLinstener( DiConsoleVarListener *pSink )
    {
        mConsoleVarListener.push_back(pSink);
    }

    void DiCommandManager::RemoveConsoleVarListener( DiConsoleVarListener *pSink )
    {
        mConsoleVarListener.remove(pSink);
    }

    DiConsoleVar* DiCommandManager::GetConsoleVar(const DiString& varName)
    {
        DI_ASSERT(this);
        DI_ASSERT(varName);

        auto it = mMapVariables.find(varName);
        if(it != mMapVariables.end())
            return it->second;

        return nullptr;
    }
    
    int DiCommandManager::GetIntVar(const DiString& name)
    {
        DiConsoleVar* var = GetConsoleVar(name);
        if(var)
            return var->GetAsInt();
        else
        {
            DI_WARNING("Cannot locate the command variable: %s", name.c_str());
            return 0;
        }
    }
    
    DiString DiCommandManager::GetStringVar(const DiString& name)
    {
        DiConsoleVar* var = GetConsoleVar(name);
        if(var)
            return var->GetString();
        else
        {
            DI_WARNING("Cannot locate the command variable: %s", name.c_str());
            return 0;
        }
    }
    
    float DiCommandManager::GetFloatVar(const DiString& name)
    {
        DiConsoleVar* var = GetConsoleVar(name);
        if(var)
            return var->GetAsFloat();
        else
        {
            DI_WARNING("Cannot locate the command variable: %s", name.c_str());
            return 0;
        }
    }
    
    bool DiCommandManager::SetIntVar(const DiString& name, int var)
    {
        DiConsoleVar* cv = GetConsoleVar(name);
        if(!cv)
            return false;
        
        if(cv->GetType() != CONSOLE_VAR_INT)
        {
            DI_WARNING("Cannot set come variable : %s, bad type cast", name.c_str());
            return false;
        }
        DiConsoleVariableInt* v = (DiConsoleVariableInt*)cv;
        v->Set(var);
        return true;
    }
    
    bool DiCommandManager::SetStringVar(const DiString& name, const DiString& var)
    {
        DiConsoleVar* cv = GetConsoleVar(name);
        if(!cv)
            return false;
        
        if(cv->GetType() != CONSOLE_VAR_STRING)
        {
            DI_WARNING("Cannot set come variable : %s, bad type cast", name.c_str());
            return false;
        }
        DiConsoleVariableString* v = (DiConsoleVariableString*)cv;
        v->Set(var.c_str());
        return true;
    }
    
    bool DiCommandManager::SetFloatVar(const DiString& name, float var)
    {
        DiConsoleVar* cv = GetConsoleVar(name);
        if(!cv)
            return false;
        
        if(cv->GetType() != CONSOLE_VAR_FLOAT)
        {
            DI_WARNING("Cannot set come variable : %s, bad type cast", name.c_str());
            return false;
        }
        DiConsoleVariableFloat* v = (DiConsoleVariableFloat*)cv;
        v->Set(var);
        return true;
    }

    void DiCommandManager::UnregisterVariable(const DiString& varName)
    {
        auto itor = mMapVariables.find(varName);

        if(itor == mMapVariables.end())
            return;

        DiConsoleVar *consVar = itor->second;

        mMapVariables.erase(itor);

        DI_DELETE consVar;
    }

    void DiSystemCmd::BindAllSystemCmd(DiCommandManager* cm)
    {
        cm->AddCommand("quit", DiSystemCmd::Quit, "close application");
        DI_INFO("All built-in commands binded");
    }

    bool DiCommandManager::HasCommand(const DiString& varName)
    {
        return mMapVariables.find(varName) != mMapVariables.end();
    }

    bool DiSystemCmd::Quit(DiCmdArgs*)
    {
#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
        ::SendMessage((HWND)mMainHwnd, WM_CLOSE, 0, 0);
#endif
        return true;
    }

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
    
    void DiCommandManager::InitConsoleLogger(bool createWnd, const DiString& consoleFile)
    {
        if (!sConsoleLogger)
        {
            if (consoleFile.empty())
                sConsoleLogger = DI_NEW DiConsoleLogger(createWnd);
            else
                sConsoleLogger = DI_NEW DiConsoleLogger(createWnd, consoleFile);

            sConsoleLogger->OutputLog("Init console window", "", "", 0);
            DiLogManager::GetInstance().RegisterLogger(sConsoleLogger);

            DI_INFO("New external console window created");
        }
        else
        {
            if (createWnd)
            {
                sConsoleLogger->CreateConsoleWindow();
            }
        }
    }

    void DiCommandManager::DestroyConsoleLogger()
    {
        if (sConsoleLogger)
        {
            DiLogManager::GetInstance().UnregisterLogger(sConsoleLogger);
            sConsoleLogger->OutputLog("Quiting", 0, 0, 0);

            SAFE_DELETE(sConsoleLogger);
        }
    }
#endif
}