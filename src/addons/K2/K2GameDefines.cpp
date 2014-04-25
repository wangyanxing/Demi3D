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
#include "K2GameDefines.h"

namespace Demi
{
    

    Demi::K2ObjSubTypes::Type K2ObjSubTypes::FromString(const DiString& str)
    {
        static DiStrHash<Type> table;
        if (table.empty())
        {
            table["Prop_Tree"] = SUB_STATIC_TREE;
            table["Prop_Cliff"] = SUB_STATIC_CLIFF;    // cliff
            table["Prop_Cliff2"] = SUB_STATIC_CLIFF;    // ramp
            table["Prop_Water"] = SUB_STATIC_WATER;
            table["Prop_Static"] = SUB_STATIC_MODEL;
            table["Prop_Scenery"] = SUB_STATIC_SCENERY;

            table["Trigger_SpawnPoint"] = SUB_LOGIC_TRIGGER_SPAWN_POINT;
        }

        auto it = table.find(str);
        if (it == table.end())
        {
            if (DiString::StartsWith(str, "Building_"))
            {
                table[str] = SUB_STATIC_BUILDING;
                return SUB_STATIC_BUILDING;
            }
            else if (DiString::StartsWith(str, "Entity_"))
            {
                table[str] = SUB_LOGIC_ENTITY;
                return SUB_LOGIC_ENTITY;
            }
            else if (DiString::StartsWith(str, "Teleport_"))
            {
                table[str] = SUB_LOGIC_TELPORT;
                return SUB_LOGIC_TELPORT;
            }
            else
            {
                return SUB_SCENE_ITEM;
            }
        }
        return it->second;
    }

    Demi::K2ObjTypes K2ObjSubTypes::GetObjType(Type t)
    {
        switch (t)
        {
        case Demi::K2ObjSubTypes::SUB_STATIC_TREE:
        case Demi::K2ObjSubTypes::SUB_STATIC_MODEL:
        case Demi::K2ObjSubTypes::SUB_STATIC_CLIFF:
        case Demi::K2ObjSubTypes::SUB_STATIC_WATER:
        case Demi::K2ObjSubTypes::SUB_STATIC_SCENERY:
        case Demi::K2ObjSubTypes::SUB_STATIC_BUILDING:
            return GAME_STATIC;
        case Demi::K2ObjSubTypes::SUB_SCENE_ITEM:
            return GAME_SCENE_ITEM;
        case Demi::K2ObjSubTypes::SUB_NPC:
            return GAME_NPC;
        case Demi::K2ObjSubTypes::SUB_HERO:
            return GAME_HERO;
        case Demi::K2ObjSubTypes::SUB_LOGIC_TRIGGER_SPAWN_POINT:
        case Demi::K2ObjSubTypes::SUB_LOGIC_ENTITY:
        case Demi::K2ObjSubTypes::SUB_LOGIC_TELPORT:
            return GAME_LOGIC;
        default:
            return INVALID_TYPE;
        }
    }


    K2PrefabClip::Clips K2PrefabClip::FromString(const DiString& str)
    {
        static DiStrHash<K2PrefabClip::Clips> sTable;
        if (sTable.empty())
        {
            sTable["idle"] = ANIM_IDLE;
            sTable["portrait"] = ANIM_PORTRAIT;
            sTable["walk_1"] = ANIM_WALK;
            sTable["death_1"] = ANIM_DEATH;
            sTable["bored_1"] = ANIM_BORED;
            sTable["taunt_1"] = ANIM_TAUNT;
            sTable["item_1"] = ANIM_ITEM;
            sTable["knock_1"] = ANIM_KNOCK;
            sTable["attack_1"] = ANIM_ATTACK_1;
            sTable["attack_2"] = ANIM_ATTACK_2;
            sTable["attack_3"] = ANIM_ATTACK_3;
            sTable["attack_4"] = ANIM_ATTACK_4;
            sTable["ability_1"] = ANIM_ABILITY_1;
            sTable["ability_2"] = ANIM_ABILITY_2;
            sTable["ability_3"] = ANIM_ABILITY_3;
            sTable["ability_4"] = ANIM_ABILITY_4;
        }

        auto i = sTable.find(str);
        if (i != sTable.end())
            return i->second;

        return MAX_PREFAB_ANIM;
    }

    Demi::DiString K2PrefabClip::ToString(Clips c)
    {
        static DiVector<DiString> sTable;
        if (sTable.empty())
        {
            sTable.resize(MAX_PREFAB_ANIM);
            sTable[ANIM_IDLE] = "idle";
            sTable[ANIM_PORTRAIT] = "portrait";
            sTable[ANIM_WALK] = "walk_1";
            sTable[ANIM_DEATH] = "death_1";
            sTable[ANIM_BORED] = "bored_1";
            sTable[ANIM_TAUNT] = "taunt_1";
            sTable[ANIM_ITEM] = "item_1";
            sTable[ANIM_KNOCK] = "knock_1";
            sTable[ANIM_ATTACK_1] = "attack_1";
            sTable[ANIM_ATTACK_2] = "attack_2";
            sTable[ANIM_ATTACK_3] = "attack_3";
            sTable[ANIM_ATTACK_4] = "attack_4";
            sTable[ANIM_ABILITY_1] = "ability_1";
            sTable[ANIM_ABILITY_2] = "ability_2";
            sTable[ANIM_ABILITY_3] = "ability_3";
            sTable[ANIM_ABILITY_4] = "ability_4";
        }

        if (c < MAX_PREFAB_ANIM)
            return sTable[c];
        else
            return DiString::BLANK;
    }

}