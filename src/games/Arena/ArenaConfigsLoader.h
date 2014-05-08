
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
    
/*** !!!! This file was generated automatically by ConfigGen !!!! ***/

#ifndef ArenaConfigsLoader__h__
#define ArenaConfigsLoader__h__

#include "ArenaPrerequisites.h"
#include "XMLElement.h"

namespace Demi{

struct ArConfigLoaderBase
{
    void Load(const DiXMLElement& node)
    {
        DiXMLElement child = node.GetChild();
        while (child)
        {
            DiString name = child.GetName();
            auto i = mPropOps.find(name);
            if(i != mPropOps.end())
            {
                i->second(child);
            }
            child = child.GetNext();
        }
    }
    
    DiMap<DiString, std::function<void(const DiXMLElement&)>> mPropOps;
};

struct ArConfigModelLoader : public ArConfigLoaderBase
{
    ArConfigModelLoader(ArConfigModel* obj)
    {
        mModel = obj;

        mPropOps["path"] = [this](const DiXMLElement& node){
            mModel->path = node.GetValue().c_str();
        };
        mPropOps["scale"] = [this](const DiXMLElement& node){
            mModel->scale = node.GetValue().AsFloat();
        };
    }

    ArConfigModel* mModel;
};

struct ArConfigMotionLoader : public ArConfigLoaderBase
{
    ArConfigMotionLoader(ArConfigMotion* obj)
    {
        mMotion = obj;

        mPropOps["runspeed"] = [this](const DiXMLElement& node){
            mMotion->runspeed = node.GetValue().AsFloat();
        };
        mPropOps["turnspeed"] = [this](const DiXMLElement& node){
            mMotion->turnspeed = node.GetValue().AsInt();
        };
    }

    ArConfigMotion* mMotion;
};

struct ArConfigHeroLoader : public ArConfigLoaderBase
{
    ArConfigHeroLoader(ArConfigHero* obj)
    {
        mHero = obj;

        mPropOps["name"] = [this](const DiXMLElement& node){
            mHero->name = node.GetValue().c_str();
        };
        mPropOps["model"] = [this](const DiXMLElement& node){
            ArConfigModelLoader ld(&mHero->model);
            ld.Load(node);
        };
        mPropOps["motion"] = [this](const DiXMLElement& node){
            ArConfigMotionLoader ld(&mHero->motion);
            ld.Load(node);
        };
    }

    ArConfigHero* mHero;
};

struct ArConfigMapLoader : public ArConfigLoaderBase
{
    ArConfigMapLoader(ArConfigMap* obj)
    {
        mMap = obj;

        mPropOps["name"] = [this](const DiXMLElement& node){
            mMap->name = node.GetValue().c_str();
        };
        mPropOps["path"] = [this](const DiXMLElement& node){
            mMap->path = node.GetValue().c_str();
        };
        mPropOps["spwanpoint"] = [this](const DiXMLElement& node){
            mMap->spwanpoint = node.GetValue().AsInt();
        };
        mPropOps["npcpoint"] = [this](const DiXMLElement& node){
            mMap->npcpoint.push_back(node.GetValue().AsInt());
        };
    }

    ArConfigMap* mMap;
};


} // namespace Demi
#endif
