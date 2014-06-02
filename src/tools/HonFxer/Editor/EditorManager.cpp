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

#include "FxerPch.h"
#include "EditorManager.h"
#include "ParticleSystemObj.h"

namespace Demi
{
    DiEditorManager::DiEditorManager()
    {
        InitFactories();
        InitCommands();
    }

    DiEditorManager::~DiEditorManager()
    {

    }

    DiBaseEditorObj* DiEditorManager::CreateEditorObject(const DiString& type)
    {
        DiBaseEditorObj* ret = nullptr;
        auto it = mObjFactories.find(type);
        if (it != mObjFactories.end())
            ret = it->second();
        else
        {
            DI_WARNING("Cannot create the object [type = %s]", type.c_str());
        }
        return ret;
    }

    void DiEditorManager::DeleteEditorObject(DiBaseEditorObj* obj)
    {
        DI_ASSERT(obj);
        obj->GetParent()->RemoveChild(obj);
        DI_DELETE obj;

        mMenuHost = nullptr;
    }

    void DiEditorManager::InitFactories()
    {
        mObjFactories["Base"]           = [](){return DI_NEW DiBaseEditorObj(); };
        mObjFactories["ParticleSystem"] = [](){return DI_NEW DiParticleSystemObj(); };
    }

    DiString DiEditorManager::GenerateSystemName()
    {
        static int id = 0;
        DiString ret;
        ret.Format("ParticleSystem_%d", id++);
        return ret;
    }

    DiString DiEditorManager::GenerateEmitterName()
    {
        static int id = 0;
        DiString ret;
        ret.Format("Emitter_%d", id++);
        return ret;
    }

    DiString DiEditorManager::GenerateControllerName()
    {
        static int id = 0;
        DiString ret;
        ret.Format("Controller_%d", id++);
        return ret;
    }

    void DiEditorManager::InitCommands()
    {
        CommandMgr->AddCommand("removeObj", [&](Demi::DiCmdArgs* args){
            if (mMenuHost)
            {
                DeleteEditorObject(mMenuHost);
                return true;
            }
            else
            {
                DI_WARNING("No object selected, cannot remove!");
                return false;
            }
        });

        CommandMgr->AddCommand("createChild", [&](Demi::DiCmdArgs* args){
            if (mMenuHost)
            {
                DI_ASSERT(args->GetArgCount() == 1);
                mMenuHost->_CreateChild(args->GetArg(0));
                return true;
            }
            else
            {
                DI_WARNING("No object selected, cannot create child!");
                return false;
            }
        });
    }
}