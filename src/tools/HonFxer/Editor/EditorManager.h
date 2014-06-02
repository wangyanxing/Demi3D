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
    class DiEditorManager
    {
    public:

        DiEditorManager();

        ~DiEditorManager();

    public:

        DiBaseEditorObj*    CreateEditorObject(const DiString& type);

    protected:

        void                InitFactories();

    protected:

        DiStrHash<std::function<DiBaseEditorObj*(const DiString&)>> mObjFactories;
    };
}

#endif
