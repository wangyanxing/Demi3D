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

    const char* DiCmdArgs::GetArg(int iIndex) const
    {
        const char* pcArg = nullptr;

        if ((size_t)iIndex >= 0 && (size_t)iIndex < mKeyList.size())
        {
            pcArg = mKeyList.at(iIndex).c_str();
        }

        return pcArg;
    }

    const char* DiCmdArgs::GetCommandLine() const
    {
        return mStrCommand.c_str();
    }

    DiKeyList& DiCmdArgs::GetKeyList()
    {
        return mKeyList;
    }

    DiString& DiCmdArgs::GetCommand()
    {
        return mStrCommand;
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

    bool DiCommandManager::AddCommand(const char* cmd, DiCmdFuntion pFunction, const char* pcHelp)
    {
        DI_ASSERT(cmd);
        DI_ASSERT(pcHelp);
        DI_ASSERT(pFunction);

        if (!cmd || !pFunction || !pcHelp)
            return false;

        DiString cmds = cmd;
        DiVector<DiString> splitList = cmds.Tokenize(" ");

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

    bool DiCommandManager::ExecuteCommand(const char* pcCmd)
    {
        DI_ASSERT(pcCmd != nullptr);

        if (!pcCmd)
            return false;
        
        // this is a comment
        if (pcCmd[0] == '#')
            return true;

        DiCmdArgs kArgs;

        DiString& kCommand = kArgs.GetCommand();
        DiKeyList& kSplitList = kArgs.GetKeyList();

        DiString cmds = pcCmd;
        cmds.Tokenize(" ", kSplitList);

        if (kSplitList.empty())
            return true;

        kCommand = pcCmd;

        DiString& kName = kSplitList.at(0);

        kName.ToLower();

        DiStringCmdInfoMap::iterator it = mCmdInfos.find(kName);
        if (it == mCmdInfos.end())
        {
            DI_WARNING( "can't find command. [%s]", kName.c_str());
            return false;
        }

        DiCmdInfo kCmdInfo = it->second;

        bool iRlt = kCmdInfo.mFunc(&kArgs);
        if (!iRlt)
        {
            DI_WARNING( "execute failed. [%s]", kCommand.c_str());
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
        cm->AddCommand("quit", functor_ret(&DiSystemCmd::Quit), "close application");
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
        }
        else
        {
            if (createWnd)
                sConsoleLogger->CreateConsoleWindow();
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