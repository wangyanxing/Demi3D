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

#ifndef ArenaEntityConfig_h__
#define ArenaEntityConfig_h__

#include "ArenaPrerequisites.h"
#include "XMLElement.h"

namespace Demi
{
    /** Entity configs
        For now we don't need all attributes
     */
    struct ArEntityConfigs
    {
        void Load(const DiXMLElement& element);
        
        void Load(const DiString& relPath);
        
        float GetMaxPreGlobalScale() const;
        
        DiString    name;
        
        StringVec   model;
        DiVec2      preglobalscale;
        
        float       modelscale {1.0f};
        float       effectscale {1.0f};
        float       boundsheight {0.0f};
        float   	boundsradius {0.0f};
        float   	selectionradius {0.0f};
        DiVec3      targetoffset {0, 0, 0};
        
        float       movespeed {0.0f};
        int         turnrate {0};
        float   	turnsmoothing {0.0f};
        
        float       attackduration {0.0f};
        float       attackactiontime {0.0f};
        float       attackcooldown {0.0f};
        DiVec2      attackdamagemin {0, 0};
        DiVec2      attackdamagemax {0, 0};
        float       attacknumanims {0.0f};
        DiVec3      attackoffset {0.0f};
        DiString    attackprojectile;
        float       attackrange {0.0f};
        DiString    attackstarteffect;
        DiString    attackactioneffect;
        StringVec   attackimpacteffect;
        DiString    attacktype;
        DiString    combattype;
        
        static DiMap<DiString, std::function<void(const DiString&, ArEntityConfigs*)>> sPropOps;
        static void InitPropOperations();
    };
}

#endif