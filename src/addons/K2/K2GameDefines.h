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

#ifndef DiK2GameDefines_h__
#define DiK2GameDefines_h__

#include "K2Prerequisites.h"

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
#    define ATTR_NORETURN
#    define ATTR_UNUSED( name ) name
#else
#   define  ATTR_NORETURN           __attribute__((noreturn))
#   define  ATTR_UNUSED( name )     name __attribute__((unused))
#endif

namespace Demi
{
    typedef uint32  ObjID_t;

    const ObjID_t   ATTR_UNUSED(INVALID_OBJ_ID) = 0xFFFFFFFF;

    const DiString  K2_DEFAULT_ANIM = "idle";

    /** prime types
     */
    enum K2ObjTypes
    {
        INVALID_TYPE = -1,

        GAME_HERO,
        GAME_NPC,
        GAME_SCENE_ITEM, // interactive item in scene, e.g. tower
        GAME_STATIC,     // e.g. trees, static models, cliffs
        GAME_LOGIC,      // logic objects, no render object

        MAX_GAME_ENTITY_TYPES
    };

    /** sub types
     */
    struct K2ObjSubTypes
    {
        enum Type
        {
            SUB_STATIC_TREE,
            SUB_STATIC_MODEL,
            SUB_STATIC_CLIFF,
            SUB_STATIC_WATER,

            SUB_SCENE_ITEM,

            SUB_NPC,

            SUB_HERO,

            SUB_LOGIC_TRIGGER,
            SUB_LOGIC_ENEMY_POINT,
        };

        static Type FromString(const DiString& str)
        {
            static DiStrHash<Type> table;
            if (table.empty())
            {
                table["Prop_Tree"]   = SUB_STATIC_TREE;
                table["Prop_Cliff"]  = SUB_STATIC_CLIFF;    // cliff
                table["Prop_Cliff2"] = SUB_STATIC_CLIFF;    // ramp
                table["Prop_Water"]  = SUB_STATIC_WATER;
                table["Prop_Static"] = SUB_STATIC_MODEL;
            }
            auto it = table.find(str);
            if (it == table.end())
                return SUB_SCENE_ITEM;
            return it->second;
        }

        static K2ObjTypes GetObjType(Type t)
        {
            switch (t)
            {
            case Demi::K2ObjSubTypes::SUB_STATIC_TREE:
            case Demi::K2ObjSubTypes::SUB_STATIC_MODEL:
            case Demi::K2ObjSubTypes::SUB_STATIC_CLIFF:
            case Demi::K2ObjSubTypes::SUB_STATIC_WATER:
                return GAME_STATIC;
            case Demi::K2ObjSubTypes::SUB_SCENE_ITEM:
                return GAME_SCENE_ITEM;
            case Demi::K2ObjSubTypes::SUB_NPC:
                return GAME_NPC;
            case Demi::K2ObjSubTypes::SUB_HERO:
                return GAME_HERO;
            case Demi::K2ObjSubTypes::SUB_LOGIC_TRIGGER:
            case Demi::K2ObjSubTypes::SUB_LOGIC_ENEMY_POINT:
                return GAME_LOGIC;
            default:
                return INVALID_TYPE;
            }
        }
    };
}

#endif