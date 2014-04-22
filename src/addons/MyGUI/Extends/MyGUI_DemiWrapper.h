
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

#ifndef __MYGUI_DEMIWRAPPER_H__
#define __MYGUI_DEMIWRAPPER_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{
    class DemiPlatform;

    class MYGUI_EXPORT DemiWrapper
    {
    public:
        DemiWrapper();
        virtual ~DemiWrapper();

    public:

        void init(const std::string& resflename);
        void createGui();
        void destroyGui();
        void setupResources();

    private:
        Gui* mGUI;
        DemiPlatform* mPlatform;
        std::string mResourceFileName;
    };
}

#endif