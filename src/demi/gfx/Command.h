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
#ifndef Command_h__
#define Command_h__

#include <functional>

namespace Demi
{
    typedef DiVector<DiString> DiKeyList;

    class DI_GFX_API DiCmdArgs
    {
    public:

        DiCmdArgs(){}

        DiCmdArgs(int argc, const char** argv);

    public:

        int                GetArgCount() const;

        const DiString&    GetArg(uint32 index) const;
        
        const DiString&    GetCommandLine() const;

        DiKeyList&         GetKeyList();
        
        DiString&          GetCommand();

    protected:
        DiKeyList          mKeyList;

        DiString           mCommand;
    };
    
    typedef std::function<bool(Demi::DiCmdArgs*)> DiCmdFuntion;

    struct DI_GFX_API DiCmdInfo
    {
        DiString        mName;
        DiString        mDesc;
        DiCmdFuntion    mFunc;

        DiCmdInfo() :   mFunc(nullptr) {}
    };
        
    typedef DiMap<DiString, DiCmdInfo> DiStringCmdInfoMap;

    typedef DiMap<DiString, DiConsoleVar*, string_nocase_lt>  ConsoleVariablesMap;
    typedef ConsoleVariablesMap::iterator                        ConsoleVariablesMapItor;

    typedef DiMap<DiString, DiString, string_nocase_lt>       ConfigVars;
    typedef DiList<DiConsoleVarListener*>                        ConsoleVarSinks;

    //////////////////////////////////////////////////////////////////////////

    class DI_GFX_API DiCommandManager : public DiBase
    {
    public:

        DiCommandManager(void);

        ~DiCommandManager(void);

    public:

        static void              InitConsoleLogger(bool createWnd = false, const DiString& consoleFile = "");
                                 
        static void              DestroyConsoleLogger();
                                 
        bool                     AddCommand(const DiString& command, DiCmdFuntion function, const DiString& help = "");
                                 
        bool                     ExecuteCommand(const DiString& command);
                                 
        int                      GetNumVars();

        bool                     HasCommand(const DiString& varName);
                                 
        DiConsoleVar*            RegisterString(const DiString& varName,const DiString& varValue,int flags, 
                                     const DiString& desc = "",std::function<void(DiConsoleVar*)> callFunc = nullptr );
                                 
        DiConsoleVar*            RegisterInt(const DiString& varName,int iValue,int flags, 
                                    const DiString& desc = "",std::function<void(DiConsoleVar*)> callFunc = nullptr );
                                 
        DiConsoleVar*            RegisterFloat(const DiString& varName,float fValue,int flags, 
                                    const DiString& desc = "",std::function<void(DiConsoleVar*)> callFunc = nullptr );
                                 
        void                     UnregisterVariable(const DiString& VarName);
                                 
        bool                     BeforeChangingVar( DiConsoleVar* variable, const DiString& newValue );
                                 
        void                     AfterChangingVar( DiConsoleVar* variable );
                                 
        void                     AddConsoleVarLinstener(DiConsoleVarListener *pSink);
                                 
        void                     RemoveConsoleVarListener(DiConsoleVarListener *pSink);
                                 
        DiConsoleVar*            GetConsoleVar(const DiString& name);
        
        int                      GetIntVar(const DiString& name);
        
        DiString                 GetStringVar(const DiString& name);
        
        float                    GetFloatVar(const DiString& name);
        
        bool                     SetIntVar(const DiString& name, int var);
        
        bool                     SetStringVar(const DiString& name, const DiString& var);
        
        bool                     SetFloatVar(const DiString& name, float var);
                                 
    protected:                   
                                 
        bool                     Init();
                                 
        void                     Shutdown();
                                 
        void                     RegisterVar( DiConsoleVar *consVar,std::function<void(DiConsoleVar*)> callFunc=0 );
                                 
    protected:                   
                                 
        DiStringCmdInfoMap       mCmdInfos;
                                 
        ConsoleVariablesMap      mMapVariables;
                                 
        ConfigVars               mConfigVars;
                                 
        ConsoleVarSinks          mConsoleVarListener;

        static DiConsoleLogger*  sConsoleLogger;
    };    

    class DiSystemCmd : public DiBase
    {
    public:
        static void BindAllSystemCmd(DiCommandManager* cm);
    };
}

#endif // Command_h__
