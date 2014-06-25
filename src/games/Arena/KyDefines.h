#ifndef KyDefines_h__
#define KyDefines_h__

#include "ArenaPrerequisites.h"
#include "KyRTTI.h"

class KyMain;
class KyObject;
class KyEntity;
class KyObjectMgr;
class KyUnitMgr;
class KyEffectMgr;


class KyMainContext
{
public:
    KyMain*       GetMain() { DI_ASSERT(m_pkLogicMain); return m_pkLogicMain; }
    const KyMain* GetMain() const { DI_ASSERT(m_pkLogicMain); return m_pkLogicMain; }
    void          SetMain(KyMain* pkLogicMain) { m_pkLogicMain = pkLogicMain; }
private:
    KyMain*       m_pkLogicMain{ nullptr };
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

struct KyPlayerInfo
{
    uint32        uiPlayerID { 0 };
    KyPlayerType  eType{ KPT_INVALID };
    KyPlayerCamp  eCamp{ KPC_INVALID };
};

#endif

