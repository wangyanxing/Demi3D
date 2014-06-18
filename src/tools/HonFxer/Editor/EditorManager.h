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

#include "FxerPrerequisites.h"

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

        DiString            GenerateSystemName();

        DiString            GenerateElementName();

        DiString            GenerateEmitterName(const DiString& type);

        DiString            GenerateControllerName(const DiString& type);

        DiBaseEditorObj*    GetMenuHost() { return mMenuHost; }

        void                SetMenuHost(DiBaseEditorObj* sel) { mMenuHost = sel; }

        DiBaseEditorObj*    GetRootObject() { return mRootObject; }
        
        DiBaseEditorObj*    GetLastCreatedObject() { return mLastCreatedObject; }
        
        void                Update();

    protected:

        void                InitFactories();

        void                InitCommands();

    protected:

        DiStrHash<std::function<DiBaseEditorObj*()>> mObjFactories;

        DiBaseEditorObj*    mMenuHost{ nullptr };

        DiBaseEditorObj*    mRootObject{ nullptr };
        
        DiBaseEditorObj*    mLastCreatedObject{ nullptr };

        DiGridPlanePtr      mGridPlane;
        
        static DiEditorManager* sEditorMgr;
    };
}

#endif
