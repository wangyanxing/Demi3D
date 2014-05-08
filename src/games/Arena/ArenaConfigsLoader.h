
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

#ifndef ArenaConfigs__h__
#define ArenaConfigs__h__

namespace Demi{

struct ArConfigLoaderBase
{
    void Load(const DiXMLElement& node)
    {
        DiXMLElement child = node.GetChild();
        while (child)
        {
            DiString name = child.GetName();
            DiString value = child.GetValue();
            
            auto i = mPropOps.find(name);
            if(i != mPropOps.end())
            {
                i->second(value);
            }
            
            child = child.GetNext();
        }
    }
    
    DiMap<DiString, std::function<void(const DiString&)>> mPropOps;
};

struct ArConfigModelLoader : public ArConfigLoaderBase
{
    ArConfigModelLoader(ArConfigModel* obj)
    {
        mModel = obj;

        mPropOps["path"] = [this](const DiString& val){
            mModel->path = val.c_str();
        }
        mPropOps["scale"] = [this](const DiString& val){
            mModel->scale = val.AsFloat();
        }
    }

    ArConfigModel* mModel;
};

struct ArConfigMotionLoader : public ArConfigLoaderBase
{
    ArConfigMotionLoader(ArConfigMotion* obj)
    {
        mMotion = obj;

        mPropOps["runspeed"] = [this](const DiString& val){
            mMotion->runspeed = val.AsFloat();
        }
        mPropOps["turnspeed"] = [this](const DiString& val){
            mMotion->turnspeed = val.AsFloat();
        }
    }

    ArConfigMotion* mMotion;
};

struct ArConfigHeroLoader : public ArConfigLoaderBase
{
    ArConfigHeroLoader(ArConfigHero* obj)
    {
        mHero = obj;

        mPropOps["name"] = [this](const DiString& val){
            mHero->name = val.c_str();
        }
        mPropOps["model"] = [this](const DiString& val){
            mHero->model = val.;
        }
        mPropOps["motion"] = [this](const DiString& val){
            mHero->motion = val.;
        }
    }

    ArConfigHero* mHero;
};

struct ArConfigMapLoader : public ArConfigLoaderBase
{
    ArConfigMapLoader(ArConfigMap* obj)
    {
        mMap = obj;

        mPropOps["name"] = [this](const DiString& val){
            mMap->name = val.c_str();
        }
        mPropOps["path"] = [this](const DiString& val){
            mMap->path = val.c_str();
        }
        mPropOps["spwanpoint"] = [this](const DiString& val){
            mMap->spwanpoint = val.AsInt();
        }
        mPropOps["npcpoint"] = [this](const DiString& val){
            mMap->npcpoint.push_back(val.AsInt());
        }
    }

    ArConfigMap* mMap;
};


} // namespace Demi
#endif
