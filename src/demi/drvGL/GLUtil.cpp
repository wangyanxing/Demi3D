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
#include "GLUtil.h"
#include <sstream>

namespace Demi
{
    bool DiGLUtil::CheckMinGLVersion(const DiString& v) const
    {
        unsigned int first, second, third;
        unsigned int cardFirst, cardSecond, cardThird;
        if (v == mVersion)
            return true;

        DiString::size_type pos = v.find(".");
        if (pos == DiString::npos)
            return false;

        DiString::size_type pos1 = v.rfind(".");
        if (pos1 == DiString::npos)
            return false;

        first = ::atoi(v.substr(0, pos).c_str());
        second = ::atoi(v.substr(pos + 1, pos1 - (pos + 1)).c_str());
        third = ::atoi(v.substr(pos1 + 1, v.length()).c_str());

        pos = mVersion.find(".");
        if (pos == DiString::npos)
            return false;

        pos1 = mVersion.rfind(".");
        if (pos1 == DiString::npos)
            return false;

        cardFirst = ::atoi(mVersion.substr(0, pos).c_str());
        cardSecond = ::atoi(mVersion.substr(pos + 1, pos1 - (pos + 1)).c_str());
        cardThird = ::atoi(mVersion.substr(pos1 + 1, mVersion.length()).c_str());

        if (first <= cardFirst && second <= cardSecond && third <= cardThird)
            return true;

        return false;
    }

    bool DiGLUtil::CheckExtension(const DiString& ext) const
    {
        if (mExtensionList.find(ext) == mExtensionList.end())
            return false;

        return true;
    }

    void DiGLUtil::InitExtensions()
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