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
            cf->attacknumanims = node.AsFloat();
        };
        sPropOps["attackoffset"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->attackoffset = node.AsVector3();
        };
        sPropOps["attackprojectile"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->attackprojectile = node;
        };
        sPropOps["attackrange"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->attackrange = node.AsFloat();
        };
        sPropOps["attackstarteffect"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->attackstarteffect = node;
        };
        sPropOps["attackactioneffect"] = [](const DiString& node, ArEntityConfigs* cf){
            cf->attackactioneffect = node;
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
    
    void ArEntityConfigs::Load(const DiXMLElement& node)
    {
        if(sPropOps.empty())
        {
            InitPropOperations();
        }
        
        node.IterateAttributes([this](const DiString& attrName, const DiString& attrVal){
            auto it = sPropOps.find(attrName);
            if(it != sPropOps.end())
                it->second(attrVal, this);
        });
        
#if 0
        DiXMLElement child = node.GetChild();
        while (child)
        {
            child = child.GetNext();
        }
#endif
    }
}