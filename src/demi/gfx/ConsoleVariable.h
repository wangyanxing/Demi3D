
/********************************************************************
    File:       ConsoleVariable.h
    Creator:    demiwangya
*********************************************************************/

#ifndef ConsoleVariable_h__
#define ConsoleVariable_h__

#include "Callback.h"
#include <functional>

#if DEMI_COMPILER == DEMI_COMPILER_MSVC
#   pragma warning(disable:4512)
#endif

namespace Demi
{
    enum ConsoleVarFlags
    {
        VAR_FLAG_NULL       = 0,
        VAR_FLAG_AS_BOOL    = 1<<0,
        VAR_FLAG_CONST      = 1<<1,
        VAR_FLAG_READ_ONLY  = 1<<2,
        VAR_FLAG_MODIFIED   = 1<<3
    };

    enum ConsoleVarType
    {
        CONSOLE_VAR_INT,
        CONSOLE_VAR_FLOAT,
        CONSOLE_VAR_STRING,
        CONSOLE_VAR_BOOL
    };

    typedef std::function<void(DiConsoleVar*)> ConsoleVarFunc;

    class DiConsoleVarListener
    {
    public:

        virtual bool BeforeChangingVar(DiConsoleVar *variable, const DiString& newValue) = 0;

        virtual void AfterChangingVar(DiConsoleVar *variable) = 0;
    };

    class DI_GFX_API DiConsoleVar
    {                     
    public:

        DiConsoleVar(DiCommandManager* consoleVar, const DiString& varName, 
            int flags, const DiString& description);

        virtual                 ~DiConsoleVar();

        virtual ConsoleVarType  GetType() = 0;

        virtual void            ClearFlags(int flags);

        virtual int             GetFlags() const;

        virtual int             SetFlags( int flags );

        virtual const DiString& GetName() const;

        virtual const DiString& GetHelp();

        virtual void            Release();

        virtual void            Set(const char* s) = 0;
                                
        virtual void            Set(const float f) = 0;
                                
        virtual void            Set(const int i) = 0;

        virtual void            SetOnChangeCallback(ConsoleVarFunc callFunc);

        virtual ConsoleVarFunc  GetOnChangeCallback();

        virtual int             GetAsInt() const = 0;

        virtual float           GetAsFloat() const = 0;

        virtual int             GetRealIVal() const 
        { 
            return GetAsInt(); 
        }

        virtual bool            IsConstCVar() const 
        { 
            return (mFlags & VAR_FLAG_CONST) != 0; 
        }

        virtual bool            IsIntAsBool()
        { 
            return (mFlags & VAR_FLAG_AS_BOOL) && GetType() == CONSOLE_VAR_INT; 
        }

    protected:

        DiString                mName;
        
        DiString                mDesc;

        int                     mFlags;

        ConsoleVarFunc          mChangeFunc;

        DiCommandManager*       mConsole;        
    };

    class DiConsoleVariableString : public DiConsoleVar
    {
    public:
        DiConsoleVariableString( DiCommandManager *consoleVar, const DiString& varName, 
            const DiString&  defaultVal, int flags, const DiString& desc)
            :DiConsoleVar(consoleVar,varName,flags,desc)
        {
            mValue = defaultVal;
        }

        virtual int            GetAsInt() const { return mValue.AsInt(); }

        virtual float          GetAsFloat() const { return mValue.AsFloat(); }

        virtual const char*    GetString() { return mValue.c_str(); }

        virtual void           Set( const char* s );

        virtual void           Set( const float f );

        virtual void           Set( const int i );

        virtual ConsoleVarType GetType() { return CONSOLE_VAR_STRING; }

    private:
        DiString               mValue;
    };

    class DiConsoleVariableInt :public DiConsoleVar
    {
    public:
        DiConsoleVariableInt( DiCommandManager *consoleVar, const DiString& varName, 
            const int iDefault, int flags, const DiString& desc)
            :DiConsoleVar(consoleVar,varName,flags,desc), mValue(iDefault)
        {
        }

        virtual int            GetAsInt() const { return mValue; }
                               
        virtual float          GetAsFloat() const { return (float)GetAsInt(); }
                               
        virtual const char*    GetString();
                               
        virtual void           Set( const char* s );
                               
        virtual void           Set( const float f );
                               
        virtual void           Set( const int i );

        virtual ConsoleVarType GetType() { return CONSOLE_VAR_INT; }

    protected:

        int                    mValue;
    };

    class DiConsoleVariableFloat :public DiConsoleVar
    {
    public:
        DiConsoleVariableFloat(DiCommandManager *consoleVar, const DiString& varName,
            const float fDefault, int flags, const DiString& desc)
            :DiConsoleVar(consoleVar,varName,flags,desc), mValue(fDefault)
        {
        }

        virtual int            GetAsInt() const { return (int)mValue; }

        virtual float          GetAsFloat() const { return mValue; }

        virtual const char*    GetString();

        virtual void           Set( const char* s );

        virtual void           Set( const float f );

        virtual void           Set( const int i );

        virtual ConsoleVarType GetType() { return CONSOLE_VAR_FLOAT; }

    private:

        float                  mValue;
    };
}
#endif // ConsoleVariable_h__