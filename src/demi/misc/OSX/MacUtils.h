
/********************************************************************
    File:       MacUtils.h
    Creator:    demiwangya
*********************************************************************/

#ifndef MacUtils_h__
#define MacUtils_h__

#include "MiscPrerequisites.h"

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