#ifndef KyDefines_h__
#define KyDefines_h__

#include "ArenaPrerequisites.h"
#include "KyRTTI.h"

const size_t npos = static_cast<size_t>(~0);

class KyMain;
class KyObject;
class KyEntity;
class KyObjectMgr;
class KyUnitMgr;
class KyEffectMgr;


class KyMainContext
{
public:
    KyMainContext(KyMain* mainContext = nullptr) : mLogicMain(mainContext) {}
    KyMain*       GetMain() const { DI_ASSERT(mLogicMain); return mLogicMain; }
    void          SetMain(KyMain* logicMain) { mLogicMain = logicMain; }

private:
    KyMain*       mLogicMain{ nullptr };
};

enum KyObjectType
{
    KOT_INVALID  = 0,

    KOT_UNIT        ,
    KOT_EFFECT      ,
    KOT_SKILL       ,
};

enum KyPlayerType
{
    KPT_INVALID     = 0,

    KPT_DISCONNECTED   ,
    KPT_COMPUTER       ,
    KPT_NORMAL         ,
};

enum KyPlayerCamp
{
    KPC_INVALID      = 0,

    KPC_CAMP_A          ,
    KPC_CAMP_B          ,

    KPC_NEUTRAL_PASSIVE ,
    KPC_NEUTRAL_ALLY    ,
    KPC_NEUTRAL_ENEMY   ,

    KPC_MAX_COUNT       ,
};

const size_t MAX_ACTIVE_PLAYER_COUNT { 2 };

const size_t MAX_PLAYER_COUNT { 2 + 5 };

const size_t CAMP_A_SOLDIER_PLAYER_INDEX { MAX_PLAYER_COUNT };
const size_t CAMP_B_SOLDIER_PLAYER_INDEX { MAX_PLAYER_COUNT + 1 };
const size_t NEUTRAL_PASSIVE_SOLDIER_PLAYER_INDEX { MAX_PLAYER_COUNT + 2 };
const size_t NEUTRAL_ALLY_SOLDIER_PLAYER_INDEX { MAX_PLAYER_COUNT + 3 };
const size_t NEUTRAL_ENEMY_SOLDIER_PLAYER_INDEX { MAX_PLAYER_COUNT + 4 };

struct KyPlayer
{
    uint32        uiPlayerID { 0 };
    KyPlayerType  eType{ KPT_INVALID };
    KyPlayerCamp  eCamp{ KPC_INVALID };
};

#endif

