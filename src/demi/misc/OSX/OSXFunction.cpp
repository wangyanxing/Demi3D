
#include "MiscPch.h"
#include "OSXFunction.h"

namespace Demi 
{
    bool DiPlatformFunc::AssertDlg(const char* msg, const char* file, int line, bool& ignore)
    {
        return true;
    }

    void DiPlatformFunc::ErrorDlg(const char* msg)
    {

    }
}