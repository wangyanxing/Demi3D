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

namespace Demi
{
    DiEditorManager::DiEditorManager()
    {
        InitFactories();
    }

    DiEditorManager::~DiEditorManager()
    {

    }

    DiBaseEditorObj* DiEditorManager::CreateEditorObject(const DiString& type)
    {
        DiBaseEditorObj* ret = nullptr;
        auto it = mObjFactories.find(type);
        if (it != mObjFactories.end())
            ret = it->second(type);
        else
        {
            DI_WARNING("Cannot create the object [type = %s]", type.c_str());
        }
        return ret;
    }

    void DiEditorManager::InitFactories()
    {

    }
}