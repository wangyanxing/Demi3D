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

#include "SceneCuller.h"
#include "OctreeCuller.h"

namespace Demi
{
    DiSceneCullerFactory::DiSceneCullerFactory(DiSceneManager* sm)
    {
        // default scene culler
        Register("Octree", [=]{
            return DI_NEW DiOctreeCuller(sm);
        });
    }
    
    void DiSceneCullerFactory::Register(const DiString& name,
                                        const std::function<DiSceneCuller*()>& entry)
    {
        mEntries[name] = entry;
    }
    
    DiSceneCuller* DiSceneCullerFactory::CreateSceneCuller(const DiString& name)
    {
        auto it = mEntries.find(name);
        if(it != mEntries.end())
            return it->second();
        else
            return nullptr;
    }
}