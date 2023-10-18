#ifndef ConfigGameNormal_h
#define ConfigGameNormal_h

#include "GameUIPopup/Common/PopupDefault.h"
#include "GameUIPopup/Common/PopupLoading.h"
#include "GameUIPopup/Common/PopupLoadingLock.h"
#include "GameUIPopup/Common/PopupScreenLock.h"

#include "GameUIPopup/Common/PopupToast.h"
#include "GameUIPopup/Common/PopupToastItems.h"
#include "GameUIPopup/Common/PopupToastBuff.h"
#include "GameUIPopup/Common/PopupToastPet.h"

static const int NormalScale = 4;

#define C_BACKGROUND_MAXTIME 86400      // 24 시간
#define C_COUNT_GAME_STATE   12
#define C_COUNT_QUEST           25//24
#define C_COUNT_ARTIFACT    106//104//102//100//78//73//68//63//58//53//52//49//45//43//38//34//33//32//31//9
#define C_COUNT_SHOP        7//6//5//8
#define C_COUNT_CASH        6
#define C_COUNT_PET         8
#define C_COUNT_PET_SKIN    1
#define C_COUNT_RELIC       9
#define C_COUNT_BUILDING    6
#define C_COUNT_JELLY       3
#define C_COUNT_MONSTER       225

#define C_COUNT_COSTUME_TYPE 6
#define C_COUNT_COSTUME      40//38//35//26//21//9
#define C_COUNT_COSTUME_JEWEL  22//17//13

#define C_COMPLETE_DAY 1
#define C_COUNT_DAILY_QUEST 6
#define C_COUNT_MONTH_QUEST 0
#define C_COUNT_DAILY_TOTAL 8//12

const int S_RESET_DAILY_QUEST[9] = {0,1,1,0,0,1,3,7,1};

#define C_COUNT_MINE 7
#define C_COUNT_MINE_MINERAL 11
#define C_COUNT_MINE_QUEST 7 //일반광물6+특수광물1

#define C_BUY_MINERAL 100
const int S_BUY_MINERAL_COUNT[C_COUNT_MINE_QUEST+1] = {0,1000,1000,1000,400,400,130,5};

/* GAME DATA */

#define C_MIN_PRESTIGE_FLOOR 100
#define C_MAX_QUEST        999
#define C_MAX_EQUIP        5
#define C_MAX_FLOOR_COUNT  5
#define C_MAX_HERO_COUNT 5
#define C_MAX_PRESTIGE_POINT 200
#define C_MAX_FEVER_TIME 30
#define C_MAX_FEVER_DAY 3
#define C_MAX_CUBE_DAY 3
#define C_MAX_MYHOME 5//7
#define C_MAX_PET_GRADE 5
#define C_MAX_PET_EVOLUTION 10
#define C_MAX_PET_EQUIP_LEVEL 40

#define C_MIDDLE_POSITION   0//70//0
#define C_BAR_MISSION_SIZE 120
#define C_BAR_RANK_SIZE   70
#define C_BAR_SIZE        135//145//150//156
#define C_BAR_SIZE_COSTUME  132//156
#define C_BAR_SIZE_MINE  115//156
#define C_BAR_SIZE_QUEST  100
#define C_BAR_SIZE_RAID_QUEST  80
#define C_MOVE_MAP_SPEED  3
#define C_MOVE_TIME       1

#define C_BONUS_ADS_GEM  200

#define C_DOUBLE_COIN     3
#define C_ATTACK          6
#define C_PRESTIGE_GEM    500
#define C_PRESTIGE_GEM2    1000
#define C_KEY_BOOSTER_GEM    300

#define C_NORMAL_ACHIEVE_COUNT 6

#define BUY_WEAPON_PERMANENT_GEM 200
#define BUY_WEAPON_PERMANENT_GOLD_GRADE 2 //지금 무기의 +2등급 무기의 최초구매가격

#define TIME_BUFF_BONUS   1800

#define TIME_ADS_GEM_BONUS    3600
#define TIME_ADS_GOLD_BONUS   3600
#define TIME_ADS_BUFF_BONUS   3600
#define TIME_ADS_MINE_BONUS   10800

#define TIME_MOVE_BONUS_SHOP_BUFF  10800   //??
#define TIME_COSTUME_RENT 86400      // 24 시간

#define C_DEX_EFFECT_MAX 4

#define C_BOSS_HP       5
#define C_BOSS_S_HP     10
#define C_BOSS_D_HP     50

#define C_COUNT_ENCHANT_STONE   7
#define C_ENCHANT_STONE_STORAGE_PRICE  200

#define C_TIME_UP_MOVE_15 0.85f
#define C_TIME_UP_MOVE_10 0.9f
#define C_TIME_DOWN_MOVE_50 1.5f

#define C_BUFF_ATTACK   8
#define C_BUFF_MOVE     0.5
#define C_BUFF_CRI      25
#define C_PRINCESS_COUNT 10
#define C_MISSION_TYPE 3

#define HP_CALCULATION    std::pow(2, 1000)
#define QUEST_CALCULATION std::pow(2, 1000)
#define FLOOR_100 100





#endif /* ConfigGameNormal_h */
