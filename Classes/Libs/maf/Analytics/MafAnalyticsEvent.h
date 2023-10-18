//
//  MafAnalyticsEvent.h
//  FantasyClicker
//
//  Created by SanHeo on 2016. 9. 23..
//
//

#ifndef MafAnalyticsEvent_h
#define MafAnalyticsEvent_h

/*각 게임별 공용으로 쓸만한것들은 정의하여 사이트분석시 일관성 유지*/
static const bool kRepeatFalse = true;
static const bool kRepeatTrue = false;
static const char* kAnalParamNone = "";
static const int kValue_1 = 1;

static const char* kAnalEventRivivalClick = "z_revival_click";
static const char* kAnalEventRivival = "z_rivival";
static const char* kAnalEventRivivalX1 = "z_rivival_x1";
static const char* kAnalEventRivivalX3 = "z_rivival_x3";
static const char* kAnalEventRivivalX3Free = "z_rivival_x3_free";
static const char* kAnalEventRivivalX5 = "z_rivival_x5";
static const char* kAnalEventRivivalKeyboost = "z_rivival_keyboost";
static const char* kAnalEventPrisonEnter = "z_prison_enter";
static const char* kAnalEventCostumeEnter = "z_costume_enter";
static const char* kAnalEventCostumeBuy = "z_costume_buy";

static const char* kAnalEventTokenEnter = "m_token_enter";
static const char* kAnalEventTokenShop1 = "m_token_shop_1";
static const char* kAnalEventTokenShop2 = "m_token_shop_2";

static const char* kAnalEventWeaponBuy5 = "z_weapon_buy_5";
static const char* kAnalEventWeaponBuy10 = "z_weapon_buy_10";
static const char* kAnalEventWeaponBuy15 = "z_weapon_buy_15";
static const char* kAnalEventWeaponBuy20 = "z_weapon_buy_20";
static const char* kAnalEventWeaponBuy25 = "z_weapon_buy_25";
static const char* kAnalEventWeaponBuy30 = "z_weapon_buy_30";

static const char* kAnalEventNewbieBoxPop = "z_newbie_box_pop";
static const char* kAnalEventNewbieBoxClick = "z_newbie_box_click";

static const char* kAnalEventMineEnter = "z_mine_enter";
static const char* kAnalEventMineBuyNomal = "z_buy_ore_nl";
static const char* kAnalEventMineBuySP = "z_buy_ore_sp";

static const char* kAnalEventAttendDay = "m_attend_day_%d";
static const char* kAnalEventPlayTime = "m_bongsik_life_%d";
static const char* kAnalEventAdsWatch = "m_ads_watch_%d"; //특정 광고 시청 횟수마다 로그 남기기
static const char* kAnalEventSpecialMission = "m_specialmission_%d";

//team raid
static const char* kAnalTeamRaidEnter = "m_raid_enter";
static const char* kAnalTeamRaidShop = "m_raid_shop";
static const char* kAnalTeamRaidChat = "m_raid_chat";
static const char* kAnalTeamRaidAds = "m_ads_slot_raid_team";

static const char* kAnalEventItemBuy = "m_buy_item_%d";
static const char* kAnalEventItemKeyBooster = "m_buy_item_keybooster";

static const char* kAnalEventCostumeDips = "m_costume_cart";
static const char* kAnalEventCurrencyDetailShow = "m_explain_goods";
static const char* kAnalEventWeaponPermanent = "m_weapon_per";

//static const char* kAnalEventDungeon75 = "m_dungeon_75";
static const char* kAnalEventServerMissionDailyClear = "m_dailymission_clear";
static const char* kAnalEventServerMissionDaily = "m_dailymission_%04d_%05d";
static const char* kAnalEventServerMissionWeeklyClear = "m_weeklymission_clear";
static const char* kAnalEventServerMissionWeekly = "m_weeklymission_%04d_%05d";

static const char* kAnalEventDungeon = "m_dungeon_%04d";
static const char* kAnalEventPrison = "z_prison_%04d";
static const char* kAnalEventStoryTuto = "m_f_story_tuto%d";

static const char* kAnalEventBuffRecommend = "m_buff_recommend"; //광고추천버튼 클릭

// ads type
static const char* kAnalEventAdsSlot = "m_ads_slot_%s"; //광고 슬롯(위치) 마다 남기기.
static const char* kAnalEventAdsLoad = "m_ad_load";

static const char* kAnalEventAdsLoadParameterSlot = "slot";
static const char* kAnalEventAdsParameterSkip = "skip";


static const char* kAdsSlotNone = "";
static const char* kAdsSlotOffline = "offline";
static const char* kAdsSlotMailBox = "mailbox";
static const char* kAdsSlotCostumeShop = "costumeshop";
static const char* kAdsSlotPrisonFree = "prisonfree";
static const char* kAdsSlotPrisonFrostAttBonus = "frost_prison_att_bonus";
static const char* kAdsSlotRevival = "revival";
static const char* kAdsSlotBoardGame = "event_boardgame";
static const char* kAdsSlotMineManastone = "mine_mk2_manastone";
static const char* kAdsSlotMiniGame = "minigame";

static const char* kAdsSlotItemGem = "item_gem";
static const char* kAdsSlotItemGold = "item_gold";
static const char* kAdsSlotItemBuff = "item_buff";
static const char* kAdsSlotItemMine = "item_mine";
static const char* kAdsSlotItemSelectGold = "item_select_gold";
static const char* kAdsSlotItemSelectAttack = "item_select_atk";
static const char* kAdsSlotItemSelectMove = "item_select_move";

static const char* kAdsSlotHome = "home_%d";

static const char* kAdsSlotDailyAttend = "dailyattend";
static const char* kAdsSlotBuffRecommend = "buff_recommend";

static const char* kAdsSlotAttend2x = "attend_2x";
static const char* kAdsSlotAttendRe = "attend_re";

static const char* kAdsSlotRaidTeam = "raid_team";

static const char* kAdsSlotBuffAtk = "buff_atk";
static const char* kAdsSlotBuffGold = "buff_gold";
static const char* kAdsSlotBuffMove = "buff_move";

static const char* kAdsSlotExpedPlus = "exped_plus";
static const char* kAdsSlotExpedJump = "exped_jump";
static const char* kAdsSlotPrisonPlus = "prison_plus";

static const char* kAdsSlotGoldPop = "gold_pop";

static const char* kAdsSlotNewRaidCharging = "m_ads_slot_new_raid_att";
static const char* kAdsSlotNewRaidSummon = "m_ads_slot_new_raid_summon";
static const char* kAdsSlotNewRaidDragonBall = "m_ads_slot_new_raid_dragonorb";

static const char* kAdsSlotEventRaceHiddenAds = "m_ads_slot_event_race_hidden";
static const char* kAdsSlotEventRaceDoubleAds = "m_ads_slot_event_race_double";

static const char* kAdsSlotEventVote = "m_ads_slot_event_vote";
static const char* kAdsSlotEventLuckyBag = "m_ads_slot_event_enhance_draw";
//초기 접속시  _f 로 시작.
static const char* kAnalEventAgree = "m_f_agree";
static const char* kAnalEventStory = "m_f_story"; //아직 미사용

//코스튬추천기능
static const char* kAnalEventRecommendCostumePop = "m_cosrec_pop";
static const char* kAnalEventRecommendCostumeClick = "m_cosrec_click";
//static const char* kAnalEventRecommendCostumeYes = "m_cosrec_yes"; //현재는 거절버튼이 없기 때문에 미사용


static const char* kAnalPediaEnter = "m_pedia_enter";
static const char* kAnalPediaLevel1 = "m_pedia_monup_001";
static const char* kAnalPediaLevel2 = "m_pedia_monup_002";
static const char* kAnalPediaLevel3 = "m_pedia_monup_003";
static const char* kAnalPediaLevel4 = "m_pedia_monup_004";
static const char* kAnalPediaLevel5 = "m_pedia_monup_005";
static const char* kAnalPediaChange = "m_pedia_mon_ex";

//치트 관련
static const char* kCheatStone = "evt_cheat_stone";

static const char* kNewRaidError = "evt_new_raid_error";

//타이밍 패키지
static const char* kAnalTimingPackageWelcome = "m_welcome_%02d";
static const char* kAnalTimingPackageActivate = "m_pop_inapp";
static const char* kAnalEventTimingPackageParameterIdx = "idx";

//돌파
static const char* kAnalTimeTravelUse = "m_dailycount_timejump";
static const char* kAnalTimeTravelFirst = "m_first_timejump";

/*
 SHOP_GEM_ADS=0,
 SHOP_GOLD_ADS,
 SHOP_BUFF_ADS,
 SHOP_MINE_ADS,
 SHOP_GEM_TNKADS,
 */



#endif /* MafAnalyticsEvent_h */
