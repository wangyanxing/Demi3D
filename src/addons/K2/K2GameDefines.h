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
#include "K2StateDefines.h"

#if DEMI_PLATFORM == DEMI_PLATFORM_WIN32
#    define ATTR_NORETURN
#    define ATTR_UNUSED( name ) name
#else
#   define  ATTR_NORETURN           __attribute__((noreturn))
#   define  ATTR_UNUSED( name )     name __attribute__((unused))
#endif

namespace Demi
{
    typedef uint32  K2ObjID;

    const K2ObjID   ATTR_UNUSED(INVALID_OBJ_ID) = 0xFFFFFFFF;
    const int32     INVALID_INT_VALUE           = 0x7fffffff;
    const float     INVALID_FLOAT_VALUE         = 1.7014117e+38f;

    enum K2WalkMode
    {
        ENUM_WALK_MODE_STOP = 0,
        ENUM_WALK_MODE_WALK,
    };

    enum K2ModalityType
    {
        MODALITY_STAND = 0,
        MODALITY_WALK = 1,
        MODALITY_RUN = 2,
        MODALITY_JUMP = 3,
    };

    /** general animation list
     */
    struct K2PrefabClip
    {
        enum Clips
        {
            ANIM_IDLE,
            ANIM_PORTRAIT,
            ANIM_WALK,
            ANIM_DEATH,
            ANIM_BORED,
            ANIM_TAUNT,
            ANIM_ITEM,
            ANIM_KNOCK,

            ANIM_ATTACK_1,
            ANIM_ATTACK_2,
            ANIM_ATTACK_3,
            ANIM_ATTACK_4,

            ANIM_ABILITY_1,
            ANIM_ABILITY_2,
            ANIM_ABILITY_3,
            ANIM_ABILITY_4,

            MAX_PREFAB_ANIM
        };

        static Clips FromString(const DiString& str);

        static DiString ToString(Clips c);
    }; 

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
            SUB_STATIC_SCENERY,
            SUB_STATIC_BUILDING,

            SUB_SCENE_ITEM,

            SUB_NPC,

            SUB_HERO,

            SUB_LOGIC_TRIGGER_SPAWN_POINT,
            SUB_LOGIC_ENTITY,
            SUB_LOGIC_TELPORT,
        };

        static Type FromString(const DiString& str);

        static K2ObjTypes GetObjType(Type t);
    };

}

#endif