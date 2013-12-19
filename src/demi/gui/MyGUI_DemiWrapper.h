
/********************************************************************
    File:       MyGUI_DemiWrapper.h
    Creator:    demiwangya
*********************************************************************/

#ifndef __MYGUI_DEMIWRAPPER_H__
#define __MYGUI_DEMIWRAPPER_H__

#include "MyGUI_Prerequest.h"

namespace Demi
{
    class DiGraphics;
}

namespace MyGUI
{
    class DirectXPlatform;

    class MYGUI_EXPORT DemiWrapper
    {
    public:
        DemiWrapper();
        virtual ~DemiWrapper();

    public:

        void addResourceLocation(const std::string& _name, bool _recursive = false);

        void init(const std::string& resflename);
        void createGui();
        void destroyGui();
        void setupResources();
        void render();

    private:
        Gui* mGUI;
        DirectXPlatform* mPlatform;
        std::string mResourceFileName;
        void* mD3D9Device;
    };
}

#endif