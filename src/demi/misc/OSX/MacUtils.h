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

#ifndef MacUtils_h__
#define MacUtils_h__

#include "MiscPrerequisites.h"
#include "str.h"

#include <CoreFoundation/CoreFoundation.h>

namespace Demi
{
    CFBundleRef mac_loadExeBundle(const char *path);
    
    void*       mac_getBundleSym(CFBundleRef bundle, const char *name);
    
    bool        mac_unloadExeBundle(CFBundleRef bundle);
    
    void*       mac_loadDylib(const char* name);

    DI_MISC_API DiString macPluginPath();

    DI_MISC_API DiString macBundlePath();
}

#endif // MacUtils_h__