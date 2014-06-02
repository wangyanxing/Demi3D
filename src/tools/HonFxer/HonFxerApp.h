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

#ifndef HonFxerApp_h__
#define HonFxerApp_h__

#include "FxerPrerequisites.h"
#include "Application.h"

namespace Demi 
{
    class HonFxerApp : public DemiDemo
    {
    public:

        HonFxerApp();

        virtual ~HonFxerApp();

    public:

        virtual void      Update();

        virtual void      Close();

        virtual void      OpenImpl();

        void              SetResourceLocation(const DiString& resPack, const DiString& texPack);

        static HonFxerApp* GetFxApp();

        static DiEditorManager* GetEditorManager() { return mEditorMgr; }

        virtual void      mouseMoved(const OIS::MouseEvent& evt);

        virtual void      mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);

        virtual void      mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id);

        virtual void      keyPressed(const OIS::KeyEvent &arg);

        MainPaneControl*  GetMainPane(){ return mMainPane; };

    private:

        bool CheckCommand();

        void Command_QuitApp(const MyGUI::UString& _commandName, bool& _result);

        void Command_Export(const MyGUI::UString& _commandName, bool& _result);

        void Command_ResLocation(const MyGUI::UString& _commandName, bool& _result);

        void Command_GameLocation(const MyGUI::UString& _commandName, bool& _result);

        void Command_ViewHelp(const MyGUI::UString& _commandName, bool& _result);

        void NotifySetResLocWindowEndDialog(Dialog* _dialog, bool _result);

        void NotifySetGameLocWindowEndDialog(Dialog* _dialog, bool _result);

    private:

        MainPaneControl*    mMainPane{ nullptr };

        static DiEditorManager* mEditorMgr;
    };
}

#endif
