
#include "ConsoleVariable.h"
#include "Command.h"

namespace Demi
{
    DiConsoleVar::DiConsoleVar(DiCommandManager* consoleVar, const DiString& varName, int flags, const DiString& description)
    {
        DI_ASSERT(consoleVar);

        mDesc = description;
        mChangeFunc = nullptr;

        mConsole = consoleVar;
        mFlags = flags;
        mName = varName;
    }

    DiConsoleVar::~DiConsoleVar()
    {
    }

    void DiConsoleVar::ClearFlags (int flags)
    {
        mFlags&=~flags;
    }

    int DiConsoleVar::GetFlags() const
    {
        return mFlags;
    }

    int DiConsoleVar::SetFlags( int flags )
    {
        mFlags = flags;
        return mFlags ;
    }

    const DiString& DiConsoleVar::GetName() const
    {
        return mName;
    }

    const DiString& DiConsoleVar::GetHelp()
    {
        return mDesc;
    }

    void DiConsoleVar::Release()
    {
        mConsole->UnregisterVariable(mName);
    }

    void DiConsoleVar::SetOnChangeCallback( ConsoleVarFunc callFunc )
    {
        mChangeFunc = callFunc;
    }

    ConsoleVarFunc DiConsoleVar::GetOnChangeCallback()
    {
        return mChangeFunc;
    }

    void DiConsoleVariableString::Set( const char* s )
    {
        if (mFlags & VAR_FLAG_READ_ONLY)
            return;

        if (!s)
            return;

        if (mValue == s)
            return;

        if(mConsole->BeforeChangingVar(this,s))
        {
            mFlags |= VAR_FLAG_MODIFIED;
                mValue = s;

            if (mChangeFunc)
                mChangeFunc(this);
            
            mConsole->AfterChangingVar(this);
        }
    }

    void DiConsoleVariableString::Set( const float f )
    {
        DiString s;
        s.Format("%g", f);

        if (mValue == s.c_str())
            return;
        
        mFlags |= VAR_FLAG_MODIFIED;
        Set(s.c_str());
    }

    void DiConsoleVariableString::Set( const int i )
    {
        DiString s;
        s.Format("%d", i);

        if (mValue == s.c_str()) 
        {
            return;
        }

        mFlags |= VAR_FLAG_MODIFIED;
        Set(s.c_str());
    }

    const char * DiConsoleVariableInt::GetString()
    {
        static char szReturnString[256];

        sprintf(szReturnString,"%d",GetAsInt());
        return szReturnString;
    }

    void DiConsoleVariableInt::Set( const char* s )
    {
        DiString v = s;
        Set(v.AsInt());
    }

    void DiConsoleVariableInt::Set( const float f )
    {
        Set((int)f);
    }

    void DiConsoleVariableInt::Set( const int i )
    {
        if (i == mValue)
            return;

        DiString s;
        s.Format("%d", i);

        if(mConsole->BeforeChangingVar(this,s.c_str()))
        {
            mFlags |= VAR_FLAG_MODIFIED;
            mValue=i; 

            if (mChangeFunc)
                mChangeFunc(this);
      
            mConsole->AfterChangingVar(this);
        }
    }

    const char * DiConsoleVariableFloat::GetString()
    {
        static char szReturnString[256];

        sprintf(szReturnString,"%g",mValue);
        return szReturnString;
    }

    void DiConsoleVariableFloat::Set( const char* s )
    {
        float fValue = 0;
        if (s)
            fValue = (float)atof(s);

        if (fValue == mValue)
            return;

        if(mConsole->BeforeChangingVar(this,s))
        {
            mFlags |= VAR_FLAG_MODIFIED;
            mValue = fValue;

            if (mChangeFunc)
                mChangeFunc(this);
            
            mConsole->AfterChangingVar(this);
        }
    }

    void DiConsoleVariableFloat::Set( const float f )
    {
        if (f == mValue)
            return;

        DiString s;
        s.Format("%g", f);

        if(mConsole->BeforeChangingVar(this,s.c_str()))
        {
            mFlags |= VAR_FLAG_MODIFIED;
            mValue=f; 
            if (mChangeFunc)
                mChangeFunc(this);
           
            mConsole->AfterChangingVar(this);
        }
    }

    void DiConsoleVariableFloat::Set( const int i )
    {
        if ((float)i == mValue)
            return;

        char sTemp[128];
        sprintf(sTemp,"%d",i);

        if(mConsole->BeforeChangingVar(this,sTemp))
        {
            mFlags |= VAR_FLAG_MODIFIED;
            mValue = (float)i; 

            if (mChangeFunc)
                mChangeFunc(this);
            
            mConsole->AfterChangingVar(this);
        }
    }
}