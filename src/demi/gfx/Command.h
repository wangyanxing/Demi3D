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
    struct string_nocase_lt 
    {
        bool operator()( const char *s1,const char *s2 ) const 
        {
            return stricmp(s1,s2) < 0;
        }

        bool operator()( const DiString& s1,const DiString& s2 ) const 
        {
            return stricmp(s1.c_str(), s2.c_str()) < 0;
        }
    };

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

    typedef std::map<DiString, DiConsoleVar*, string_nocase_lt>  ConsoleVariablesMap;
    typedef ConsoleVariablesMap::iterator                        ConsoleVariablesMapItor;

    typedef std::map<DiString, DiString, string_nocase_lt>       ConfigVars;
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
                                 
        bool                     AddCommand(const char* pcCmd, DiCmdFuntion pFunction, const char* pcHelp);
                                 
        bool                     ExecuteCommand(const char* pcCmd);
                                 
        int                      GetNumVars();
                                 
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

        static bool Quit(DiCmdArgs* pkArgs);
    };
}

#endif // Command_h__
