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

#ifndef EditorManager_h__
#define EditorManager_h__

#include "DemoPrerequisites.h"
#include "FxerPrerequisites.h"
#include "TransGizmo.h"
#include "MyGUI_UString.h"

namespace Demi 
{
    class DiEditorManager : public DiBase
    {
    public: 

        DiEditorManager();

        ~DiEditorManager();

    public:
        
        static DiEditorManager*    Get() {return sEditorMgr;}

        DiBaseEditorObj*    CreateEditorObject(const DiString& type);

        void                DeleteEditorObject(DiBaseEditorObj* obj);
        
        /// Create all objects from the loaded particle system
        DiBaseEditorObj*    LoadParticleSystem(DiParticleSystemPtr ps);

        DiString            GenerateSystemName();

        DiString            GenerateElementName();

        DiString            GenerateEmitterName(const DiString& type);

        DiString            GenerateControllerName(const DiString& type);

        DiBaseEditorObj*    GetCurrentSelection() { return mCurrentSel; }

        void                SetCurrentSelection(DiBaseEditorObj* sel);

        DiBaseEditorObj*    GetRootObject() { return mRootObject; }
        
        DiBaseEditorObj*    GetLastCreatedObject() { return mLastCreatedObject; }
        
        void                Update();
        
        void                SetGizmoMode(DiTransGizmo::GizmoMode);
        
        DiTransGizmo::GizmoMode GetGizmoMode(){ return mGlobalGizmoMode; }
        
        void                SetCurrentFileName(const DiString& name);
        
        void                SetK2ResourcePack(const DiString& resPack, const DiString& texturePack = DiString::BLANK);
        
        void                SetWorldSpaceGizmo(bool val);
        
        bool                IsGizmoInWorldSpace(){return mWorldSpaceGizmoOrientation;}
        
        void                Command_ToolPlay(const MyGUI::UString& _commandName, bool& _result);
        
        void                Command_ToolPause(const MyGUI::UString& _commandName, bool& _result);
        
        void                Command_ToolStop(const MyGUI::UString& _commandName, bool& _result);
        
        void                Command_ToolNew(const MyGUI::UString& _commandName, bool& _result);
        
        void                Command_ToolSave(const MyGUI::UString& _commandName, bool& _result);
        
        void                Command_ToolOpen(const MyGUI::UString& _commandName, bool& _result);
        
        void                Command_ToolReset(const MyGUI::UString& _commandName, bool& _result);
        
        void                Command_ToolSelect(const MyGUI::UString& _commandName, bool& _result);
        
        void                Command_ToolRotate(const MyGUI::UString& _commandName, bool& _result);
        
        void                Command_ToolScale(const MyGUI::UString& _commandName, bool& _result);
        
        void                Command_ToolMove(const MyGUI::UString& _commandName, bool& _result);

    protected:

        void                InitFactories();

        void                InitCommands();
        
        void                SaveAll(const DiString& fxFileName);
        
        void                SaveAll();
        
        void                OpenFx(const DiString& fxFileName);
        
        void                NewFx();
        
        void                Reset();

    protected:

        DiStrHash<std::function<DiBaseEditorObj*()>> mObjFactories;

        DiBaseEditorObj*    mCurrentSel{ nullptr };

        DiBaseEditorObj*    mRootObject{ nullptr };
        
        DiBaseEditorObj*    mLastCreatedObject{ nullptr };

        DiGridPlanePtr      mGridPlane;
        
        DiParticleSystemPtr mSelectingFx;
        
        static DiEditorManager* sEditorMgr;
        
        DiString            mFxFileName;
        
        bool                mWorldSpaceGizmoOrientation{false};
        
        DiTransGizmo::GizmoMode mGlobalGizmoMode{ DiTransGizmo::GIZMO_SELECT };
    };
}

#endif
