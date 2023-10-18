#ifndef ConfigGameDevil_h
#define ConfigGameDevil_h

#include "GameUIPopup/Common/PopupDefault.h"
#include "GameUIPopup/Common/PopupLoading.h"
#include "GameUIPopup/Common/PopupLoadingLock.h"
#include "GameUIPopup/Common/PopupScreenLock.h"

#include "GameUIPopup/Common/PopupToast.h"
#include "GameUIPopup/Common/PopupToastItems.h"
#include "GameUIPopup/Common/PopupToastBuff.h"
#include "GameUIPopup/Common/PopupToastPet.h"

static const int DevilScale = 3;

#define DEVIL_FLOOR_LIMIT 10001
#define DEVIL_JOIN_FLOOR   10000

#define C_DEVIL_BONUS_KEY       5
#define C_DEVIL_BONUS_SEMI_KEY  10
#define C_DEVIL_BONUS_DRG_KEY   100

#define C_COUNT_GAME_STATE_DEVIL   9

#define C_COUNT_QUEST_DEVIL     15
#define C_COUNT_EQUIP_DEVIL 17 //각 장비당 갯수
#define C_COUNT_SHOP_DEVIL        5
#define C_COUNT_PET_DEVIL        4


#define C_COUNT_EQUIP_TYPE_DEVIL 4
#define C_MAX_EQUIP_REINFORCE_DEVIL 20 //최대 강화수치 +10강화
#define C_MAX_EQUIP_LEVEL_DEVIL 999

#define BUY_D_ATK_SPD   250
#define BUY_D_MOVE_SPD  250
#define BUY_D_ATK_DMG       100
#define BUY_D_QUEST_GOLD    100
#define BUY_D_GOLD      100

#define C_COUNT_ARTIFACT_TYPE_SORT  4 //테이블에서 임시소트위해
#define C_COUNT_ARTIFACT_TYPE_DEVIL  5
#define C_COUNT_ARTIFACT_DEVIL 251//161//141 //보물의 총 수량



#endif /* ConfigGameDevil_h */



