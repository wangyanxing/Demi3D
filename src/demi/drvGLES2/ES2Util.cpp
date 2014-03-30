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

#include "DrvGLES2Pch.h"
#include "ES2Util.h"
#include <sstream>

namespace Demi
{
    bool DiGLES2Util::CheckExtension(const DiString& ext) const
    {
        if (mExtensionList.find(ext) == mExtensionList.end())
            return false;

        return true;
    }

    void DiGLES2Util::InitExtensions()
    {
        const GLubyte* pcVer = glGetString(GL_VERSION);
        DI_ASSERT(pcVer);

        // Version
        DiString tmpStr((const char*)pcVer);
        DI_INFO("GL_VERSION = %s", (const char*)pcVer);
        mVersion = tmpStr.substr(0, tmpStr.find(" "));

        // Vendor
        const GLubyte* pcVendor = glGetString(GL_VENDOR);
        tmpStr = (const char*)pcVendor;
        DI_INFO("GL_VENDOR = %s", (const char*)pcVendor);
        mVendor = tmpStr.substr(0, tmpStr.find(" "));

        // Renderer
        const GLubyte* pcRenderer = glGetString(GL_RENDERER);
        tmpStr = (const char*)pcRenderer;
        DI_INFO("GL_RENDERER = %s", (const char*)pcRenderer);

        // Extension list
        std::stringstream ext;
        std::string str;

        const GLubyte* pcExt = glGetString(GL_EXTENSIONS);
        DI_INFO("Supported GL extensions:");
        DiLogManager::GetInstancePtr()->Output(LOG_LEVEL_LOG, (const char*)pcExt);

        ext << pcExt;

        while (ext >> str)
        {
            mExtensionList.insert(str.c_str());
        }
    }
}