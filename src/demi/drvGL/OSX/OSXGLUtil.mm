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
#include "DrvGLPch.h"
#include "OSX/OSXGLUtil.h"
#include "OSX/OSXWindow.h"

#include <dlfcn.h>
#include <OpenGL/OpenGL.h>

namespace Demi
{
    DiOSXGLUtil::DiOSXGLUtil()
    {
        
    }
    
    DiOSXGLUtil::~DiOSXGLUtil()
    {
        
    }
    
    void* DiOSXGLUtil::GetProcAddress(const DiString& procname)
    {
        void *symbol;
        symbol = NULL;
        
        void *handle = dlopen("DiDrvGL.dylib", RTLD_LAZY | RTLD_GLOBAL);
        if(handle)
        {
            symbol = dlsym (handle, procname.c_str());
        }
        else
        {
            DI_WARNING("Cannot locate the OpenGL render plugin.");
        }
        dlclose(handle);
        
        return symbol;
    }

	DiWindow* DiOSXGLUtil::CreateNewWindow()
	{
		return DI_NEW DiOSXWindow();
	}
}