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

#include "ArenaPch.h"
#include "ArenaEntityConfig.h"
#include "K2Configs.h"
#include "XMLFile.h"

namespace Demi
{
    DiMap<DiString, std::function<void(const DiString&, ArEntityConfigs*)>> ArEntityConfigs::sPropOps;
    
    void ArEntityConfigs::InitPropOperations()
    {
        sPropOps["name"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->name = node;
        };
        sPropOps["model"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->model = node.Tokenize(",");
        };
        sPropOps["preglobalscale"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->preglobalscale = node.AsVector2();
        };
        
        sPropOps["modelscale"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->modelscale = node.AsFloat();
        };
        sPropOps["effectscale"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->effectscale = node.AsFloat();
        };
        sPropOps["boundsheight"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->boundsheight = node.AsFloat();
        };
        sPropOps["boundsradius"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->boundsradius = node.AsFloat();
        };
        sPropOps["selectionradius"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->selectionradius = node.AsFloat();
        };
        sPropOps["targetoffset"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->targetoffset = node.AsVector3();
        };
        

        sPropOps["movespeed"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->movespeed = node.AsFloat();
        };
        sPropOps["turnrate"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->turnrate = node.AsInt();
        };
        sPropOps["turnsmoothing"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->turnsmoothing = node.AsFloat();
        };
        
        sPropOps["attackduration"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->attackduration = node.AsFloat();
        };
        sPropOps["attackactiontime"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->attackactiontime = node.AsFloat();
        };
        sPropOps["attackcooldown"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->attackcooldown = node.AsFloat();
        };
        sPropOps["attackdamagemin"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->attackdamagemin = node.AsVector2();
        };
        sPropOps["attackdamagemax"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->attackdamagemax = node.AsVector2();
        };
        sPropOps["attacknumanims"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->attacknumanims = node.AsInt();
            DI_ASSERT(cf->attacknumanims <= 4);
        };
        sPropOps["attackoffset"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->attackoffset = node.AsVector3();
        };
        sPropOps["attackprojectile"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->attackprojectile = node.Tokenize(",");
        };
        sPropOps["attackrange"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->attackrange = node.AsFloat();
        };
        sPropOps["attackstarteffect"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->attackstarteffect = node.Tokenize(",");
        };
        sPropOps["attackactioneffect"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->attackactioneffect = node.Tokenize(",");
        };
        sPropOps["attackimpacteffect"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->attackimpacteffect = node.Tokenize(",");
        };
        sPropOps["attacktype"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->attacktype = node;
        };
        sPropOps["combattype"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->combattype = node;
        };

        sPropOps["aggrorange"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->aggrorange = node.AsFloat();
        };
        sPropOps["proximityrange"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->proximityrange = node.AsFloat();
        };
        sPropOps["sightrangeday"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->sightrangeday = node.AsFloat();
        };
        sPropOps["sightrangenight"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->sightrangenight = node.AsFloat();
        };
    }
    
    void ArEntityConfigs::Load(const DiString& relPath)
    {
        auto stream = DiK2Configs::GetDataStream(relPath, K2_RES_XML);
        
        shared_ptr<DiXMLFile> xmlfile(new DiXMLFile());
        xmlfile->Load(stream->GetAsString());
        Load(xmlfile->GetRoot());
    }
    
    float ArEntityConfigs::GetMaxPreGlobalScale() const
    {
        float ret = 1.0f;
        ret = DiMath::Max(ret, DiMath::Max(preglobalscale.x, preglobalscale.y));
        return ret;
    }
    
    ArEntityConfigs::~ArEntityConfigs()
    {
        for (auto& i : mProjectileConfigs)
        {
            DI_DELETE i.second;
        }
        mProjectileConfigs.clear();
    }
    
    void ArFxProjectileConfig::Load(const DiXMLElement& node)
    {
        if (!node.CheckName("projectile"))
        {
            DI_WARNING("Bad projectile config file!");
            return;
        }
        
        name = node.GetAttribute("name");
        speed = node.GetFloat("speed");
        gravity = node.GetFloat("gravity");
        modelscale = node.GetFloat("modelscale");
        model = node.GetAttribute("model");
        traileffect = node.GetAttribute("traileffect");
        impacteffect = node.GetAttribute("impacteffect");
    }
    
    void ArEntityConfigs::Load(const DiXMLElement& node)
    {
        if(sPropOps.empty())
        {
            InitPropOperations();
        }
        
        node.IterateAttributes([this](const DiString& attrName, const DiString& attrVal)
        {
            auto it = sPropOps.find(attrName);
            if(it != sPropOps.end())
            {
                it->second(attrVal, this);
            }
        });
        
        // load projectile configs
        if(path.empty())
        {
            return;
        }
        
        if(path[path.size()-1] != '\\' && path[path.size()-1] != '/')
        {
            path.Append("/");
        }
        
        static StringVec pjfiles = {
                                    "projectile/projectile.entity",
                                    "projectile/attack_projectile.entity",
                                    "projectile/attack_projectile_mega.entity"
                                   };
        for (auto& str : pjfiles)
        {
            DiString file = path + str;
            if(!DiK2Configs::K2ArchiveExists(file, false))
            {
                continue;
            }
            
            DI_LOG("Loading projectile entity config %s", file.c_str());
            
            auto pjCfg = DiK2Configs::GetDataStream(file, K2_RES_XML);
            auto ret = DI_NEW ArFxProjectileConfig();
            
            shared_ptr<DiXMLFile> xmlfile(DI_NEW DiXMLFile());
            xmlfile->Load(pjCfg->GetAsString());
            DiXMLElement root = xmlfile->GetRoot();
            ret->Load(root);
            
            mProjectileConfigs[ret->name] = ret;
        }
    }
    
    ArFxProjectileConfig* ArEntityConfigs::GetProjectile(const DiString& name)
    {
        auto it = mProjectileConfigs.find(name);
        if(it == mProjectileConfigs.end())
        {
            return nullptr;
        }
        else
        {
            return it->second;
        }
    }
}