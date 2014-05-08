
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
/*** Generated time: 21:06:57 05/07/2014 ***/

#ifndef ArenaConfigs__h__
#define ArenaConfigs__h__

#include "ArenaPrerequisites.h"
#include "str.h"

namespace Demi{

/**
  Model properties
 **/
struct ArConfigModel
{
    ArConfigModel()
    {
        scale = 1;
    }

    DiString path;
    float scale;
};

/**
  Motion properties
 **/
struct ArConfigMotion
{
    ArConfigMotion()
    {
        runspeed = 8;
        turnspeed = 500;
    }

    float runspeed;
    int turnspeed;
};

/**
  Hero configs
 **/
struct ArConfigHero
{
    ArConfigHero()
    {
    }

    DiString name;
    ArConfigModel model;
    ArConfigMotion motion;
};

/**
  NPC configs
 **/
struct ArConfigNpc
{
    ArConfigNpc()
    {
    }

    DiString name;
    int npcpoint;
    int number;
    int range;
    ArConfigModel model;
    ArConfigMotion motion;
};

/**
  Map configs
 **/
struct ArConfigMap
{
    ArConfigMap()
    {
        spwanpoint = 0;
    }

    DiString name;
    DiString path;
    int spwanpoint;
    DiVector<int> npcpoint;
};

/**
  NPC spawning list
 **/
struct ArConfigSpawnNpc
{
    ArConfigSpawnNpc()
    {
    }

    DiVector<ArConfigNpc> npcs;
};

} // namespace Demi
#endif
