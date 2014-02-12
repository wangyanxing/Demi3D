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