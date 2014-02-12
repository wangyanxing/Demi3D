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
#include "DemoPch.h"

#if 0
#include "Info.h"

namespace Demi
{
    DiInfo::DiInfo() 
        : mInfo(nullptr)
        , mOffset(20, 20)
    {
        const std::string layer = "Statistic";
        //if (!MyGUI::LayerManager::getInstance().isExist(layer))
        //    return;

        //mInfo = MyGUI::Gui::getInstance().createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(), MyGUI::Align::Default, layer);
        //mInfo->setTextColour(MyGUI::Colour::White);
        //mInfo->setTextShadow(true);
    }

    DiInfo::~DiInfo()
    {
        if (mInfo != nullptr)
        {
            //MyGUI::Gui::getInstance().destroyChildWidget(mInfo);
            mInfo = nullptr;
        }
    }

    void DiInfo::Remove(const std::string& key)
    {
        /*for (MyGUI::VectorStringPairs::iterator iter = mParams.begin(); iter != mParams.end(); ++iter)
        {
            if (iter->first == key)
            {
                mParams.erase(iter);
                return;
            }
        }*/
    }

    void DiInfo::Update(float delta)
    {
        //Change("FPS", 1.0f / delta);

        if (mInfo != nullptr)
        {
            //std::ostringstream stream;
            //for (MyGUI::VectorStringPairs::iterator iter = mParams.begin(); iter != mParams.end(); ++iter)
            //{
            //    if (iter != mParams.begin())
            //        stream << "\n";
            //    stream << iter->first << " :\t" << iter->second;
            //}

            //mInfo->setCaption(stream.str());
            DiString info = DiProfiler::GetInstancePtr()->GetData();
            DiString fps;
            fps.Format("\nFPS: %f", 1.0f / delta);
            info += fps;
            mInfo->setCaption(info.c_str());

            DI_PROFILE_BEGIN_INTERVAL

            MyGUI::ISubWidgetText* text = mInfo->getSubWidgetText();
            if (text != nullptr)
            {
                const MyGUI::IntSize& size = text->getTextSize() + mInfo->getSize() - text->getSize();
                const MyGUI::IntSize& size_view = MyGUI::RenderManager::getInstance().getViewSize();
                mInfo->setCoord(10, 10, size.width, size.height);
            }
        }
    }

    void DiInfo::Clear(const std::string& _key)
    {
        /*for (MyGUI::VectorStringPairs::iterator iter = mParams.begin(); iter != mParams.end(); ++iter)
        {
            if (iter->first == _key)
            {
                mParams.erase(iter);
                return;
            }
        }*/
    }
}

#endif