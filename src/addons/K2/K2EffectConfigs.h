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

#ifndef DiEffectConfigs_h__
#define DiEffectConfigs_h__

#include "K2Prerequisites.h"

namespace Demi
{
    struct DiK2BillboardConfig
    {
        void        Load(DiXMLElement& element);

        int         expirelife{ 0 };
        DiString    bone;
        int         delay{ 0 };
        DiVec3      position;
        int         life{ 0 };
        DiColor     color;
        DiColor     startcolor;
        DiColor     midcolor;
        DiColor     endcolor;
        int         size{ 5 };
        float       rollspeed{ 0 };
        int         depthbias{ 0 };
        DiString    material;
        DiString    directionalspace{ "local" };
        int         yaw{ 0 };
        int         pitch{ 0 };
        int         roll{ 0 };
        bool        lockup{ false };
        bool        lockright{ false };
        float       minheight{ 0 };
        float       maxheight{ 0 };
    };

    struct DiK2ParticleConfig
    {
        void        Load(DiXMLElement& element);

        DiColor     startcolor;
        DiColor     midcolor;
        DiColor     endcolor;
        float       midcolorpos{ 0 };
        int         minangle{ 0 };
        int         maxangle{ 0 };
        int         minanglespeed{ 0 };
        int         maxanglespeed{ 0 };
        int         startsize{ 0 };
        int         endsize{ 0 };
        bool        lockup{ false };
        bool        lockright{ false };
        DiString    directionalspace{ "local" };
        int         yaw{ 0 };
        int         pitch{ 0 };
        int         roll{ 0 };
    };

    struct DiK2SimpleEmitterConfig
    {
        void        Load(DiXMLElement& element);

        int         delay{ 0 };
        DiString    bone;
        DiVec3      position;
        int         spawnrate{ 0 };
        int         particlelife{ 0 };
        float       gravity{ 0.0f };
        int         offsetsphere{ 0 };
        int         speed{ 0 };
        int         drag{ 0 };
        int         acceleration{ 0 };
        DiString    material;
        int         minangle{ 0 };
        int         maxangle{ 0 };
        DiVec3      direction;

        DiVector<DiK2ParticleConfig> particles;
    };
}

#endif
