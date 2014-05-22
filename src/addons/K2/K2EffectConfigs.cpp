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

#include "K2Pch.h"
#include "K2EffectConfigs.h"

namespace Demi
{
    void DiK2BillboardConfig::Load(DiXMLElement& element)
    {
        DI_ASSERT(element.CheckName("billboard"));

        element.GetInt("expirelife", expirelife);
        element.GetAttribute("bone", bone);
        element.GetInt("delay", delay);
        element.GetVector3("position", position);
        element.GetInt("life", life);
        element.GetColor("color", color);
        element.GetColor("startcolor", startcolor);
        element.GetColor("midcolor", midcolor);
        element.GetColor("endcolor", endcolor);
        element.GetInt("size", size);
        element.GetFloat("rollspeed", rollspeed);
        element.GetInt("depthbias", depthbias);
        element.GetAttribute("material", material);
        element.GetAttribute("directionalspace", directionalspace);
        element.GetInt("yaw", yaw);
        element.GetInt("pitch", pitch);
        element.GetInt("roll", roll);
        element.GetBool("lockup", lockup);
        element.GetBool("lockright", lockright);
        element.GetFloat("minheight", minheight);
        element.GetFloat("minheight", minheight);
    }

    void DiK2SimpleEmitterConfig::Load(DiXMLElement& element)
    {
        DI_ASSERT(element.CheckName("simpleemitter"));

        element.GetInt("delay", delay);
        element.GetAttribute("bone", bone);
        element.GetVector3("position", position);
        element.GetInt("spawnrate", spawnrate);
        element.GetInt("particlelife", particlelife);
        element.GetFloat("gravity", gravity);
        element.GetInt("offsetsphere", offsetsphere);
        element.GetInt("speed", speed);
        element.GetInt("drag", drag);
        element.GetInt("acceleration", acceleration);
        element.GetAttribute("material", material);
        element.GetInt("minangle", minangle);
        element.GetInt("maxangle", maxangle);
        element.GetVector3("direction", direction);

        DiXMLElement child = element.GetChild();
        while (child)
        {
            if (child.CheckName("particle"))
            {
                particles.push_back(DiK2ParticleConfig());
                particles.back().Load(child);
            }

            child = child.GetNext();
        }
    }

    void DiK2ParticleConfig::Load(DiXMLElement& element)
    {
        DI_ASSERT(element.CheckName("particle"));

        element.GetColor("startcolor", startcolor);
        element.GetColor("midcolor", midcolor);
        element.GetColor("endcolor", endcolor);
        element.GetFloat("midcolorpos", midcolorpos);
        element.GetInt("minangle", minangle);
        element.GetInt("maxangle", maxangle);
        element.GetInt("minanglespeed", minanglespeed);
        element.GetInt("maxanglespeed", maxanglespeed);
        element.GetInt("startsize", startsize);
        element.GetInt("endsize", endsize);
        element.GetBool("lockup", lockup);
        element.GetBool("lockright", lockright);
        element.GetAttribute("directionalspace", directionalspace);
        element.GetInt("yaw", yaw);
        element.GetInt("pitch", pitch);
        element.GetInt("roll", roll);
    }
}