#ifndef ConfigEnum_h
#define ConfigEnum_h

#define MAGIC_ENUM_RANGE_MIN -1
#define MAGIC_ENUM_RANGE_MAX 256
#include "Libs/magic_enum/magic_enum.h"

template <typename T>
std::string enumToString(T eType) {
    auto color_name = magic_enum::enum_name(eType);
    return color_name.data();
}

template <typename T>
int enumToInt(T eType) {
    return (int)eType;
}

template <typename T>
T enumFromInt(int value) {
    auto cast = magic_enum::enum_cast<T>(value);
    if ( cast.has_value() ) {
        return cast.value();
    }
    else if ( value > magic_enum::customize::enum_range<T>::max )
    {
        return (T)value;
    }
    else
    {
        return (T)value;
    }
}
    
template <typename T>
T enumFromString(std::string name) {
    auto cast = magic_enum::enum_cast<T>(name);
    if ( cast.has_value() ) {
        return cast.value();
    }
    else
    {
        return magic_enum::enum_value<T>(0);
    }
}

template <typename T>
T enumFromIndex(int value) {
    return magic_enum::enum_value<T>(value);
}

template <typename T>
int enumCount() {
    return magic_enum::enum_count<T>();
}

#pragma mark - Enum Normal ============================================================================================================
enum E_PLACE{
    EMPTY=-1,
    DG_NORMAL=0,
    DG_DEVIL_DOM=1,
    DG_PRISON=3,
};

enum E_ZORER{
    MAP,
    MONSTER,
    HERO,
    UI,
    UI_SCROLL,
    UI_BOTTOM,
    EXIT,
    POPUP,
    COMIC,
    TUTORIAL_Z,
};

enum E_QUEST{
    QUEST_NONE = 0,
    QUEST_1    = 1,
    QUEST_2,
    QUEST_3,
    QUEST_4,
    QUEST_5,
    QUEST_6,
    QUEST_7,
    QUEST_8,
    QUEST_9,
    QUEST_10,
    QUEST_11,
    QUEST_12,
    QUEST_13,
    QUEST_14,
    QUEST_15,
    QUEST_16,
    QUEST_17,
    QUEST_18,
    QUEST_19,
    QUEST_20,
    QUEST_21,
    QUEST_22,
    QUEST_23,
    QUEST_24,
    QUEST_25,
};

enum E_WEAPON{
    WEAPON_NONE=0, //맨주먹코스튬 세팅위해
    WEAPON_1 = 1,
    WEAPON_2 = 2,
    WEAPON_3 = 3,
    WEAPON_4 = 4,
    WEAPON_5 = 5,
    WEAPON_6 = 6,
    WEAPON_7 = 7,
    WEAPON_8 = 8,
    WEAPON_9 = 9,
    WEAPON_10 = 10,
    WEAPON_11 = 11,
    WEAPON_12 = 12,
    WEAPON_13 = 13,
    WEAPON_14 = 14,
    WEAPON_15 = 15,
    WEAPON_20 = 20,
    WEAPON_22 = 22,
    WEAPON_25 = 25,
    WEAPON_29 = 29,
    WEAPON_30 = 30,
    WEAPON_40 = 40,
    WEAPON_46 = 46,
    WEAPON_50 = 50,
    WEAPON_51 = 51,
    WEAPON_56 = 56,
    WEAPON_58 = 58,
    WEAPON_59 = 59,
    WEAPON_60 = 60,
    WEAPON_66 = 66,
    WEAPON_68 = 68,
    WEAPON_76_MINE = 76,
    WEAPON_77_L = 77,
    WEAPON_78_L = 78,
    WEAPON_82_L = 82,
    WEAPON_89 = 89,
    WEAPON_90_L = 90,
    WEAPON_92_L = 92,
    WEAPON_93 = 93,
    WEAPON_94 = 94,
    WEAPON_95 = 95,
    WEAPON_96 = 96,
    WEAPON_97 = 97,
    WEAPON_98 = 98,
    WEAPON_99 = 99,
    WEAPON_100_L = 100,
    WEAPON_104_L = 104,
    WEAPON_106 = 106,
    WEAPON_108 = 108,
    WEAPON_114 = 114,
    WEAPON_116_L = 116,
    WEAPON_118 = 118,
    WEAPON_119_L = 119,
    WEAPON_120 = 120,
    WEAPON_122_L = 122,
    WEAPON_124_L = 124,
    WEAPON_125_L = 125,
    WEAPON_126_L = 126,
    WEAPON_127_L = 127,
    WEAPON_128_L = 128,
    WEAPON_130_L = 130,
    WEAPON_133_L = 133,
    
    WEAPON_180_L = 180,
};

enum E_PET{
    PET_NONE,
    CHICK_1 = 1,
    HAMSTER_2,
    DRAGON_3,
    PRINCESS_4,
    PURIRING_5,
    HONG_6,
    MOLE_7,
    HANDSOME_8,
    
    D_1_PENGUIN,
    D_2_HEDGEHOG,
    D_3_GATEKEEPER,
    D_4_FLASH,
    
};

enum class E_PET_EQUIP_BUY
{
    HONOR = 1,
    BONG = 2,
    COIN = 3,
    FREE = 4,
    CASH,
};

enum E_HONOR{ //1001을 그대로 사용하고, E_COSTUME_IDX에 HONOR를 넣엇음. 서버로 보낼때만 예전처럼 1,2,3으로 보낸다.
    HONOR_1 = 1001,
    HONOR_2 = 1002,
    HONOR_3 = 1003,
};

enum E_ARTIFACT_TYPE
{
    ARTI_TYPE_NONE=0,
    ARTI_TYPE_ATK=1,
    ARTI_TYPE_CRI=2,
    ARTI_TYPE_QUEST=3,
    ARTI_TYPE_KILL=4,
    ARTI_TYPE_CROSS=5,
    ARTI_TYPE_DISCOUNT=6,
};

enum E_ARTIFACT{  //힘반,칼날,독수리,수정의 이름을 몇차수인지 확인가능하게 변경
    ARTI_NONE = 0,
    ARTI_1_EVOL1 = 1,
    ARTI_2,
    ARTI_3_EVOL1,
    ARTI_4_EVOL1,
    ARTI_5,
    ARTI_6,
    ARTI_7,
    ARTI_8_EVOL1,
    ARTI_9_KEY,
    ARTI_10_KEY,
    ARTI_11_KEY,
    ARTI_12_CROSS_EVOL1,
    
    ARTI_13_EVOL2,
    ARTI_14_EVOL2,
    ARTI_15_EVOL2,
    ARTI_16_EVOL2,
    ARTI_17_CROSS_EVOL2,
    
    ARTI_18_EVOL3,
    ARTI_19_EVOL3,
    ARTI_20_EVOL3,
    ARTI_21_EVOL3,
    ARTI_22_CROSS_EVOL3,
    
    ARTI_23_EVOL4,
    ARTI_24_EVOL4,
    ARTI_25_EVOL4,
    ARTI_26_EVOL4,
    ARTI_27_CROSS_EVOL4,
    
    ARTI_28_EVOL5,
    ARTI_29_EVOL5,
    ARTI_30_EVOL5,
    ARTI_31_EVOL5,

    ARTI_32_PRISON_WEAPON,
    
    ARTI_33_CROSS_EVOL5,
    
    ARTI_34_PICKAX,

    ARTI_35_EVOL6,
    ARTI_36_EVOL6,
    ARTI_37_EVOL6,
    ARTI_38_EVOL6,
    ARTI_39_CROSS_EVOL6,
    
    ARTI_40_EVOL7,
    ARTI_41_EVOL7,
    ARTI_42_EVOL7,
    ARTI_43_EVOL7,
    ARTI_44_CROSS_EVOL7,
    
    ARTI_45,
    
    ARTI_46_EVOL8,
    ARTI_47_EVOL8,
    ARTI_48_EVOL8,
    ARTI_49_EVOL8,
    
    ARTI_50,
    ARTI_51,
    ARTI_52,
    
    ARTI_53_CROSS_EVOL8,
    
    ARTI_54_EVOL9,
    ARTI_55_EVOL9,
    ARTI_56_EVOL9,
    ARTI_57_EVOL9,
    ARTI_58_CROSS_EVOL9,
    
    ARTI_59_EVOL10,
    ARTI_60_EVOL10,
    ARTI_61_EVOL10,
    ARTI_62_EVOL10,
    ARTI_63_CROSS_EVOL10,
    
    ARTI_64_EVOL11,
    ARTI_65_EVOL11,
    ARTI_66_EVOL11,
    ARTI_67_EVOL11,
    ARTI_68_CROSS_EVOL11,
    
    ARTI_69_EVOL12,
    ARTI_70_EVOL12,
    ARTI_71_EVOL12,
    ARTI_72_EVOL12,
    ARTI_73_CROSS_EVOL12,
    
    ARTI_74_EVOL13,
    ARTI_75_EVOL13,
    ARTI_76_EVOL13,
    ARTI_77_EVOL13,
    ARTI_78_CROSS_EVOL13,
    
    ARTI_79_EVOL14,
    ARTI_80_EVOL14,
    ARTI_81_EVOL14,
    ARTI_82_EVOL14,
    ARTI_83_CROSS_EVOL14,
    
    ARTI_84_EVOL15,
    ARTI_85_EVOL15,
    ARTI_86_EVOL15,
    ARTI_87_EVOL15,
    ARTI_88_CROSS_EVOL15,
    
    ARTI_89_DISCOUNT_1,
    ARTI_90_DISCOUNT_2,
    ARTI_91_DISCOUNT_3,
    ARTI_92_DISCOUNT_4,
    ARTI_93_DISCOUNT_5,
    ARTI_94_DISCOUNT_6,
    ARTI_95_DISCOUNT_7,
    ARTI_96_DISCOUNT_8,
    ARTI_97_DISCOUNT_9,
    ARTI_98_DISCOUNT_10,
    ARTI_99_DISCOUNT_11,
    ARTI_100_DISCOUNT_12,
    ARTI_101_DISCOUNT_13,
    ARTI_102_DISCOUNT_14,
    
    ARTI_103_BERSERKER_SPRIT = 103,
    ARTI_104_BERSERKER_AMULET = 104,
    ARTI_105_INFINITE_DISCOUNT = 105,
    
    ARTI_106 = 106,
    
    E_ARTIFACT_999_NONE, //save/load할때 갯수늘어날때 신경안쓰기위해서, 다른곳에선 사용하지 않음.
    
};

enum class INFINITE_ARTIFACT_TYPE
{
    NONE = 0,
    
    INFINITE_ARTI_TYPE_ATK=1,
    INFINITE_ARTI_TYPE_CRI=2,
    INFINITE_ARTI_TYPE_QUEST=3,
    INFINITE_ARTI_TYPE_KILL=4,
    INFINITE_ARTI_TYPE_CROSS=5,
    END
};

enum E_RELIC{
    RELIC_NONE = 0,
    RELIC_1 = 1,
    RELIC_2,
    RELIC_3,
    RELIC_4,
    RELIC_5,
    RELIC_6,
    RELIC_7,
    RELIC_8,
    RELIC_9,
};

enum E_SHOP{
    SHOP_NONE = 0,
    SHOP_HERO_GEM = 1,
    SHOP_FEVER,
    SHOP_ATTACK_BONUS,
    SHOP_MOVE_BONUS,
    SHOP_COIN_BONUS,
    SHOP_GOLD_GEM,
    SHOP_GOLD_GEM_BIG,
    
    SHOP_D_MOVE_SPD,
    SHOP_D_ATK_SPD,
    SHOP_D_ATK_DMG,
    SHOP_D_QUEST_GOLD,
    SHOP_D_GOLD_GEM,
    
};

enum E_SHOP_ADS{
    SHOP_GEM_ADS=0,
    SHOP_GOLD_ADS,
    SHOP_BUFF_ADS,
    SHOP_MINE_ADS,
    SHOP_GEM_TNKADS,
    
    SHOP_D_GEM_ADS,
    SHOP_D_GOLD_ADS,
    SHOP_D_BUFF_ADS,
    SHOP_D_GEM_TNKADS,
    
    SHOP_SELECT_ADS,
    
    SHOP_BUFF_COOLDOWN_ATK = 12,
    SHOP_BUFF_COOLDOWN_GOLD = 13,
    SHOP_BUFF_COOLDOWN_MOVE = 14,
    
    SHOP_BUFF_COOLDOWN_D_ATK = 15,
    SHOP_BUFF_COOLDOWN_D_ASPD = 16,
    SHOP_BUFF_COOLDOWN_D_GOLD = 17,
    SHOP_BUFF_COOLDOWN_D_MOVE = 18,
    
    SHOP_BUFF_GOLD_INTERSTITIAL = 19, // 재화부족 전면배너
};

enum E_CASH{
    CASH_NONE,
    
    CASH_PACKAGE_BEGINNER_1 = 7,
    CASH_PACKAGE_BEGINNER_2 = 8,
    
    CASH_ARTIFACT_300_1 = 10,
    CASH_ARTIFACT_300_2 = 11,
    CASH_ARTIFACT_300_3 = 12,
    CASH_ARTIFACT_300_4 = 13,
    
    CASH_ARTIFACT_3000_1 = 14,
    CASH_ARTIFACT_3000_2 = 15,
    CASH_ARTIFACT_3000_3 = 16,
    CASH_ARTIFACT_3000_4 = 17,
    
    CASH_ARTIFACT_INFINITE_3000_1 = 18,
    CASH_ARTIFACT_INFINITE_3000_2 = 19,
    CASH_ARTIFACT_INFINITE_3000_3 = 20,
    CASH_ARTIFACT_INFINITE_3000_4 = 21,
    
    CASH_DAILY_BONG = 200,
    CASH_DAILY_COIN = 201,
    CASH_DAILY_GEM = 202,
    
    CASH_DAILY_ADS_NEW = 204,
    CASH_DAILY_ADS_CONTINUE = 205,
    
    CASH_PTICKET_1 = 301,
    CASH_PTICKET_2 = 303,
    CASH_PTICKET_3 = 304,
    CASH_PTICKET_4 = 305,
    
    CASH_MINE_7 = 800,
    CASH_MINE_8 = 801,
    CASH_MINE_9 = 802,
    CASH_MINE_10 = 803,
    CASH_MINE_11 = 804,
    CASH_MINE_1 = 805,
    CASH_MINE_2 = 806,
    CASH_MINE_3 = 807,
    CASH_MINE_4 = 808,
    CASH_MINE_5 = 809,
    CASH_MINE_6 = 810,
    
    CASH_RETURN_1 = 400000,
    CASH_RETURN_2 = 400001,
    
    CASH_DUNGEON_PASS_1 = 700000,
    CASH_TEAM_RAID_1 = 80000,
    CASH_TEAM_RAID_2 = 80001,
    CASH_TEAM_RAID_3 = 80002,
    
    CASH_EVENT_RACE_RESET = 90040,
    
};


#pragma mark -Added ENUM

enum E_TOWN{
    TOWN_NONE = 0,
    BUILDING_1 = 1,
    BUILDING_2,
    BUILDING_3,
    BUILDING_4,
    BUILDING_5,
    SATELLITE,
};

enum class E_ITEMS{
    NONE                                = 0,
    GOLD                                = 1,
    KEY                                 = 2,
    GEM                                 = 3,
    COSTUME_COIN                        = 4,
    MATERIAL                            = 5,
    HONOR                               = 6,
    POINT                               = 7, //만능포인트
    CUBE                                = 8,
    D_KEY                               = 9,
    D_GOLD                              = 10,
    EXPEDI_POINT                        = 11,
    FRIEND_POINT                        = 12,
    COSTUME_COUPON                      = 13,//코스튬 쿠폰
    STONECASE                           = 14,//강화석 보관함
    KEY_BOOSTER                         = 15,//열쇠 부스터
    STONE_ALL                           = 16,//강화석
    BUFF_ATK                            = 17,//공격력 버프
    BUFF_COIN                           = 18,//더블 코인 버프
    BUFF_SPD                            = 19,//이속 버프
    HELL                                = 20,//헬트리움
    
    TICKET                              = 22,//TICKET(지하감옥)
    PTICKET                             = 23,//P-TICKET(지하감옥)
    SNOWFLAKE                           = 24,//용 비늘 (눈꽃송이)
    
    BUFF_ATK_9                          = 25,//공격력 버프 9
    BUFF_ATK_10                         = 26,//공격력 버프 10
    
    BUFF_DEVIL_ATK                      = 28,//공격 버프(마왕성)
    BUFF_DEVIL_ASPD                     = 29,//공격속도 버프(마왕성)
    BUFF_DEVIL_COIN                     = 30,//더블 코인 버프(마왕성)
    BUFF_DEVIL_SPD                      = 31,//이속 버프(마왕성
    
    MINE_STONE_IRON                     = 32,
    MINE_STONE_COPPER                   = 33,
    MINE_STONE_SILVER                   = 34,
    MINE_PIECE_IRON                     = 35,
    MINE_PIECE_COPPER                   = 36,
    MINE_PIECE_SILVER                   = 37,
    MINE_SAPPHIRE                       = 38,
    MINE_OPAL                           = 39,
    MINE_AQUAMARINE                     = 40,
    MINE_EMERALD                        = 41,
    MINE_AMETHYST                       = 42,
    
    BUFF_ALL                            = 47,//공격,골드,이동 180초 버프
    
    EVENT_TICKET                        = 49,//이벤트 티켓
    
    PET_EQUIP_BOX                       = 50,//펫 장비 상자
    MONSTER_SOUL                        = 51,//몬스터 정수
    MONSTER_SOUL_PIECE                  = 52,//몬스터 정수 조각
    TEAMRAID_BALL                       = 54,//팀 레이드 공격 단위(봉기옥)
    CASH_SNOWFLAKE                      = 55,//캐시 용 비늘(캐시 눈꽃송이)
    MINE_BOX                            = 56,//광물 상자(봉기옥)
    MINE_SPECIAL_BOX                    = 57,//스페셜 광물 상자(봉기옥)
    EVENT_TOKEN                         = 58,//이벤트 토큰
    
    EVENT_BAIT                          = 65,//낚시 이벤트 미끼
    EVENT_BLACK_PEARL                   = 66,//낚시 이벤트 흑진주
    
    CAT_FOOD                            = 68,//원정대 고양이 캔사료
    LOOT                                = 69,//원정대 고양이 캔사료
    LOOT_PIECE_1                        = 70,//원정대 냥이 전리품 조각( 동 )
    LOOT_PIECE_2                        = 71,//원정대 냥이 전리품 조각( 은 )
    LOOT_PIECE_3                        = 72,//원정대 냥이 전리품 조각( 금 )
    LOOT_PIECE_4                        = 73,//원정대 냥이 전리품 조각( 백금 )
    
    KRAKEN_TICKET                       = 80,//원정대 크라켄 티켓 (원정대 P티켓)
    
    EVENT_HALLOWEEN_CANDY               = 84,//2021 할로윈 캔디
    EVENT_DICE_POINT                    = 85,//보드게임 주사위 포인트
    NEW_RAID_DRAGON_BALL                = 86,//신규 레이드 용옥
    SPIRIT_STON                         = 87,//정령석
    
    SPIRIT_DRAW_TICKET_1                = 89,//정령 단차
    SPIRIT_DRAW_TICKET_11               = 90,//정령 11연차
    EVENT_TRADING_MARKET_COIN           = 91,
    
    ADVENTURE_CORE                      = 93,//어드벤처 코어
    ADVENTURE_TICKET                    = 94,//어드벤처 입장 티켓
    
    MINE_BOX_STONE                      = 101,//광물 상자 [광석]
    MINE_BOX_PIECE                      = 102,//광물 상자 [괴]
    
    EVENT_RACE_COIN                     = 103,//레이스 이벤트 코인
    EVENT_RACE_TICKET                   = 104,//레이스 이벤트 게임 티켓
    EVENT_RACE_SKIN                     = 106,//레이스 스킨 쿠폰
    
    BUFF_HERO                           = 107,//봉신강림 버프
    
    FROST_PRISON_POINT                  = 108,//서리감옥 포인트
    
    EVENT_LUCKEY_BACK                   = 110,//복주머니 이벤트 뽑기권
    
    MYTHRIL                             = 112,//미스릴
    
    LOOT_PIECE_5                        = 114,//원정대 냥이 전리품 조각( 다이아 )
    EVENT_TOKEN_PIECE                   = 115,//이벤트 토큰 조각
    
    ADVENTURE_EXP                       = 116,//모험단 경험치
    ADVENTURE_BOSS_BULLET_FOREST        = 117,//모험단 보스 속성탄[숲]
    ADVENTURE_BOSS_BULLET_EARTH         = 118,//모험단 보스 속성탄[대지]
    ADVENTURE_BOSS_BULLET_GLACIER       = 119,//모험단 보스 속성탄[얼음]
    ADVENTURE_BOSS_BULLET_VOLCANO       = 120,//모험단 보스 속성탄[화산]
    ADVENTURE_BOSS_BULLET_ALL           = 121,//모험단 보스 속성탄[만능]
    
    EVENT_TREASURE_CHANCE               = 123,//보물 찾기 이벤트 모종삽[기회]
    
    
    DEFENSE_COIN                        = 125,//디펜스 코인[클라]
    DEFENSE_DRAW_TICKET_1               = 126,//디펜스 1회 뽑기 티켓
    DEFENSE_DRAW_TICKET_11              = 127,//디펜스 11회 뽑기 티켓
    DEFENSE_DRAW_TICKET_33              = 128,//디펜스 33회 뽑기 티켓
    
    
    SPIRIT_DRAW_TICKET_33              = 132,//정령 33연차
    
    
    DEFENSE_WEAPON                      = 2000,//디펜스 무기
    DEFENSE_CHARACTER                   = 3000,//디펜스 캐릭터
                            
    
    BADGE                               = 10000,//배지
    
    COSTUME_SKIN                        = 11000,//봉식 스킨
    COSTUME_HEAD                        = 11001,//코스튬 투구
    COSTUME_BODY                        = 11002,//코스튬 갑옷
    COSTUME_CAPE                        = 11003,//코스튬 망토
    COSTUME_HAND                        = 11004,//코스튬 장갑
    COSTUME_FOOT                        = 11005,//코스튬 신발
    COSTUME_JEWEL                       = 11006,//코스튬 장신구
    COSTUME_SETITEM                     = 11007,//코스튬 불꽃
    
    ARTIFACT                            = 12000,//보물
    INFINITE_ARTIFACT                   = 12002,//초월 보물
    PET_EQUIP                           = 13000,//펫 장비
    PET_SKIN_RANDOM_BOX                 = 15000,//펫 스킨 랜덤 상자
    MINE_SPECIAL_RANDOM_BOX             = 15001,//스페셜 광물 랜덤 상자
    
    WEAPON_SPIRIT                       = 30000,//정령
    ADVENTURE_RELIC                     = 40000,//모험 유물
};

enum E_BUFF_TYPE{
    BUFF_ATTACK_BONUS = 0,
    BUFF_COIN_BONUS = 1,
    BUFF_MOVE_BONUS = 2,
    BUFF_KEY_BONUS = 3,
    BUFF_HERO_BONUS = 4,
    BUFF_ATTACK_9_BONUS = 5,
    BUFF_ATTACK_10_BONUS = 6,
    
    BUFF_D_ATK_DMG = 7,
    BUFF_D_QUEST_GOLD = 8,
    BUFF_D_MOVE_SPD = 9,
    BUFF_D_ATK_SPD = 10,
};

enum class E_DAILY_QUEST{
    MISSION_E_NONE = 0,
    PRISON_TICKET_USE, //지하감옥 티켓 사용
    RAID_ATT, //용잡이 공격
    ADS_MYHOME,//마이홈 광고
    PLAYTIME_S,//플레이 시간(초)
    FRIEND_SEND,//우정 포인트 보내기
    USE_GEM,//보석 사용
    VISIT_SHOP,//상점 방문
    QUEST_DAILY_DONE,//일퀘 완료
    PET_EQUIP_BOX_USE,// 펫장비 상자 사용
    ESTONE_BUY,//강화석 구입
    REVIVE,//환생
    HELLTRIUM_GET,//헬트리움 얻기
    MINERAL_BUY,//광물 구입
    QUEST_WEEKLY_DONE,//주간 임무 완료
};

enum E_NORMAL_ACHIEVE_MISSION
{
    ACHIEVE_NONE = 0,
    //pet
    ACHIEVE_PET_GET = 1,
    ACHIEVE_PET_LEVELUP = 2,
    ACHIEVE_PET_EVOLUTION = 3,
    ACHIEVE_PET_ENHANCE = 4,
    ACHIEVE_PET_EQUIP_COLLECT_LEVEL = 5,
    
    //Costume
    ACHIEVE_COSTUME_BUY_HEAD = 6,
    ACHIEVE_COSTUME_BUY_BODY = 7,
    ACHIEVE_COSTUME_BUY_CAPE = 8,
    ACHIEVE_COSTUME_BUY_HAND = 9,
    ACHIEVE_COSTUME_BUY_FOOT = 10,
    ACHIEVE_COSTUME_BUY_SETITEM = 11,
    
    //Artifact - ATK
    ACHIEVE_ARTIFACT_ATK_1 = 12,
    ACHIEVE_ARTIFACT_ATK_2 = 13,
    ACHIEVE_ARTIFACT_ATK_3 = 14,
    ACHIEVE_ARTIFACT_ATK_4 = 15,
    ACHIEVE_ARTIFACT_ATK_5 = 16,
    ACHIEVE_ARTIFACT_ATK_6 = 17,
    ACHIEVE_ARTIFACT_ATK_7 = 18,
    ACHIEVE_ARTIFACT_ATK_8 = 19,
    ACHIEVE_ARTIFACT_ATK_9 = 20,
    ACHIEVE_ARTIFACT_ATK_10 = 21,
    ACHIEVE_ARTIFACT_ATK_11 = 22,
    ACHIEVE_ARTIFACT_ATK_12 = 23,
    ACHIEVE_ARTIFACT_ATK_13 = 24,
    ACHIEVE_ARTIFACT_ATK_14 = 25,
    ACHIEVE_ARTIFACT_ATK_15 = 26,
    
    //Artifact - CRI
    ACHIEVE_ARTIFACT_CRI_1 = 27,
    ACHIEVE_ARTIFACT_CRI_2 = 28,
    ACHIEVE_ARTIFACT_CRI_3 = 29,
    ACHIEVE_ARTIFACT_CRI_4 = 30,
    ACHIEVE_ARTIFACT_CRI_5 = 31,
    ACHIEVE_ARTIFACT_CRI_6 = 32,
    ACHIEVE_ARTIFACT_CRI_7 = 33,
    ACHIEVE_ARTIFACT_CRI_8 = 34,
    ACHIEVE_ARTIFACT_CRI_9 = 35,
    ACHIEVE_ARTIFACT_CRI_10 = 36,
    ACHIEVE_ARTIFACT_CRI_11 = 37,
    ACHIEVE_ARTIFACT_CRI_12 = 38,
    ACHIEVE_ARTIFACT_CRI_13 = 39,
    ACHIEVE_ARTIFACT_CRI_14 = 40,
    ACHIEVE_ARTIFACT_CRI_15 = 41,
    
    //Artifact - QUEST
    ACHIEVE_ARTIFACT_QUEST_1 = 42,
    ACHIEVE_ARTIFACT_QUEST_2 = 43,
    ACHIEVE_ARTIFACT_QUEST_3 = 44,
    ACHIEVE_ARTIFACT_QUEST_4 = 45,
    ACHIEVE_ARTIFACT_QUEST_5 = 46,
    ACHIEVE_ARTIFACT_QUEST_6 = 47,
    ACHIEVE_ARTIFACT_QUEST_7 = 48,
    ACHIEVE_ARTIFACT_QUEST_8 = 49,
    ACHIEVE_ARTIFACT_QUEST_9 = 50,
    ACHIEVE_ARTIFACT_QUEST_10 = 51,
    ACHIEVE_ARTIFACT_QUEST_11 = 52,
    ACHIEVE_ARTIFACT_QUEST_12 = 53,
    ACHIEVE_ARTIFACT_QUEST_13 = 54,
    ACHIEVE_ARTIFACT_QUEST_14 = 55,
    ACHIEVE_ARTIFACT_QUEST_15 = 56,
    
    //Artifact - KILL
    ACHIEVE_ARTIFACT_KILL_1 = 57,
    ACHIEVE_ARTIFACT_KILL_2 = 58,
    ACHIEVE_ARTIFACT_KILL_3 = 59,
    ACHIEVE_ARTIFACT_KILL_4 = 60,
    ACHIEVE_ARTIFACT_KILL_5 = 61,
    ACHIEVE_ARTIFACT_KILL_6 = 62,
    ACHIEVE_ARTIFACT_KILL_7 = 63,
    ACHIEVE_ARTIFACT_KILL_8 = 64,
    ACHIEVE_ARTIFACT_KILL_9 = 65,
    ACHIEVE_ARTIFACT_KILL_10 = 66,
    ACHIEVE_ARTIFACT_KILL_11 = 67,
    ACHIEVE_ARTIFACT_KILL_12 = 68,
    ACHIEVE_ARTIFACT_KILL_13 = 69,
    ACHIEVE_ARTIFACT_KILL_14 = 70,
    ACHIEVE_ARTIFACT_KILL_15 = 71,
    
    //Building
    ACHIEVE_TOWN_LEVEL_1 = 72,
    ACHIEVE_TOWN_LEVEL_2 = 73,
    ACHIEVE_TOWN_LEVEL_3 = 74,
    ACHIEVE_TOWN_LEVEL_4 = 75,
    ACHIEVE_TOWN_LEVEL_5 = 76,
    
    //Myhome
    ACHIEVE_MYHOME_LEVEL_1 = 77,
    ACHIEVE_MYHOME_LEVEL_2 = 78,
    ACHIEVE_MYHOME_LEVEL_3 = 79,
    ACHIEVE_MYHOME_LEVEL_4 = 80,
    ACHIEVE_MYHOME_LEVEL_5 = 81,
    
    //Raid
    ACHIEVE_NEW_RAID_ACCRUE_STAT = 84,
    ACHIEVE_NEW_RAID_SEASON_LEVEL = 85,
    ACHIEVE_NEW_RAID_SEASON_DAMAGE = 86,
    
};

enum class E_MISSION_RESET_TYPE : int
{
    NONE = -1,
    DAILY = 0,
    WEEK = 1,
    DAILY_EVENT = 2,
    WEEKLY_EVENT = 3,
    
};

#pragma mark -Character ENUM

enum E_HERO_STATE{
    NONE,
    IDLE,
    MOVE,
    ATTACK,
};

enum E_PRINCESS_BUFF{
    BUFF_NONE,
    BUFF_ATTACK,
    BUFF_MOVE,
    BUFF_CRITICAL,
    BUFF_ALL,
};

enum E_GATEKEEPER_BUFF{
    BUFF_NONE_GATEKEEPER,
    BUFF_ATTACK_GATEKEEPER,
    BUFF_MOVESPPED_GATEKEEPER,
    BUFF_ATTACKSPPED_GATEKEEPER,
    BUFF_CRITICAL_GATEKEEPER,
};

enum E_COSTUME{
    HEAD=1,
    BODY,
    CAPE,
    HAND,
    FOOT,
    JEWEL,
    SETITEM,
};

enum E_COSTUME_IDX{
    IDX_1_DEFAULT=1,
    IDX_2,
    IDX_3,
    IDX_4,
    IDX_5,
    IDX_6,
    IDX_7,
    IDX_8,
    IDX_9,
    IDX_10,
    IDX_11,
    IDX_12,
    IDX_13,
    IDX_14,
    IDX_15,
    IDX_16,
    IDX_17,
    IDX_18,
    IDX_19,
    IDX_20,
    IDX_21,
    IDX_22,
    IDX_23,
    IDX_24,
    IDX_25,
    IDX_26,
    IDX_27,
    IDX_28,
    IDX_29,
    IDX_30,
    IDX_31,
    IDX_32,
    IDX_33,
    IDX_34,
    IDX_35,
    IDX_36,
    IDX_37,
    IDX_38,
    IDX_39,
    IDX_LEGEND,
    
    IDX_HONOR_1001 = 1001,
    IDX_HONOR_1002 = 1002,
    IDX_HONOR_1003 = 1003,
    
};

enum class E_STONE{
    NONE = 0,
    
    ATTACK = 1,
    GOLD = 2,
    KEY = 3,
    
    FORCE_ATTACK = 4,
    FORCE_GOLD = 5,
    FORCE_KEY = 6,
    
    FORCE_2_ATTACK = 7,
    FORCE_2_GOLD = 8,
    FORCE_2_KEY = 9,
};

enum E_STONE_TYPE{
    ALL,
    LOCAL,
    SERVER,
};

enum E_MINE{
    MINE_1 = 1,
    MINE_2,
    MINE_3,
    MINE_4,
    MINE_5,
    MINE_6,
    MINE_7
};
enum E_MINERAL{
    MINERAL_NONE = 0,
    MINERAL_1 = 1,
    MINERAL_2,
    MINERAL_3,
    MINERAL_4,
    MINERAL_5,
    MINERAL_6,
    MINERAL_7_SAPPHIRE,
    MINERAL_8_OPAL,
    MINERAL_9_AQUA,
    MINERAL_10_EMERALD,
    MINERAL_11_AMETHYST
};
#define C_MAX_PICKAX    50
#define C_COUNT_MINE_BUFF_ATTACK_COUNT_1  4800//9600
#define C_COUNT_MINE_BUFF_ATTACK_COUNT_2  9600//19200
#define C_COUNT_MINE_BUFF_ATTACK_COUNT_3  14400//28800//28800
#define C_COUNT_MINE_BUFF   2

enum E_EARNGOLD{
    MON_KILL,
    GEM_SHOP,
    GEM_BIG_SHOP,
    ADS_SHOP,
    SECOND_DEFAULT,
};

enum E_CHARACTER{
    CHA_HERO,
    CHA_SHADOW,
    CHA_GAVEL, //재판장의 망치
    CHA_PET_CHICK,
    CHA_PET_CHICK_EVOLUTION,
    CHA_PET_DRAGON,
    CHA_PET_HONG,
    CHA_D_PET_PENGUIN,
    CHA_D_PET_HEDGEHOG,
    CHA_D_PET_FLASH,
};

enum E_CHARACTER_HIT{
    HIT_NORMAL = (0x01<<0),
    HIT_DEATHBLOW = (0x01<<1),
    HIT_FROSTSKILL_CRI = (0x01<<2),
    HIT_FROSTSKILL_50005 = (0x01<<3),
    HIT_FROSTSKILL_50006 = (0x01<<4),
};

enum E_DUNGEON{
    D_NEW_RAID = 1,
    D_PRISON,
    D_DUNGEON_NORMAL,
    D_DUNGEON_DEVIL,
    D_ADVENTURE,
    D_PRISON_FROST,
    D_FLASHMAN,
    D_DEFENSE,
};
enum E_MISSION{
    DAILY = 0,
    WEEK = 1,
    SPECIAL = 2
};

enum E_JELLY{
    JELLY_TIME = 0,
    JELLY_DOUBLE,
    JELLY_EXPAND
};

enum E_TIME_TRAVEL{
    TIME_TRAVEL_TIME_ADD = 0,
    TIME_TRAVEL_CHARGE_DES,
    TIME_TRAVEL_COUNT_ADD
    
};

enum E_MYHOME {
    MYHOME_BONGSIK = 1,
    MYHOME_PRINCESS,
    MYHOME_CHICK,
    MYHOME_HAMSTER,
    MYHOME_HONG,
};

enum E_NOTI {
    NOTI_OFFLINE = 1000,
    NOTI_RAID_BONGGIOK = 1002,
    NOTI_PRISON_ENTER = 1003,
    NOTI_PRISON_JELLY = 1004,
};

enum E_SHORTCUTS
{
    SHORTCUT_NONE = 0,
    SHORTCUT_QUEST,
    SHORTCUT_EQUIP,
    SHORTCUT_PET,
    SHORTCUT_ARTIFACT_NORMAL,
    SHORTCUT_RELIC,
    SHORTCUT_SHOP,
    SHORTCUT_PRESTIGE,
    SHORTCUT_MINE,
    SHORTCUT_BUILDING,
    SHORTCUT_MYHOME,
    SHORTCUT_MENU,
    SHORTCUT_COSTUME,
    SHORTCUT_PRISON,
    SHORTCUT_ARTIFACT_SPECIAL,
    SHORTCUT_RAID,
    SHORTCUT_FRIENDS,
    SHORTCUT_PETEQUIP,
    SHORTCUT_GIFT,
    SHORTCUT_ATTEND,
    SHORTCUT_DEVILDOM,
};

enum E_NEWBIE_STEP{
    STEP_NONE =0,
    STEP_1 = 1, //1,2,3 숫자 바뀌면안됨
    STEP_2 = 2,
    STEP_3 = 3,
    STEP_4 = 4,
    STEP_5 = 5,
    STEP_FINISH,
};

enum E_AREA_MOVE {
    AREA_NONE,
    
    AREA_NORMAL,
    AREA_DEVIL,
    AREA_PRISON,
    AREA_RAID,
    AREA_COSTUME,
    AREA_FRIEND,
    AREA_DRAW,
    AREA_SHOP,
    
    AREA_NORMAL_QUEST,
    AREA_NORMAL_ARTIFACT,
    AREA_NORMAL_WEAPON,
    AREA_NORMAL_PET,
    AREA_NORMAL_MYHOME,
    AREA_NORMAL_TOWN,
    AREA_NORMAL_REVIVE,
    AREA_NORMAL_MINE,
    AREA_NORMAL_MISSION,
    
    
    AREA_DEVIL_QUEST,
    AREA_DEVIL_WEAPON,
    AREA_DEVIL_SLATE,
    AREA_DEVIL_REVIVE,
    
    AREA_NEW_RAID,
    AREA_NEW_RAID_DRAGON_BALL_SHOP,
    AREA_ADVENTURE,
    AREA_ADVENTURE_CORE_SHOP,
    
    AREA_PRISON_FROST,
    
    AREA_DEFENSE,
};

enum class E_BADGE_BUFF
{
    BADGE_NONE=0,
    BADGE_ATK,
    BADGE_GOLD,
    BADGE_KEY,
    BADGE_BONGATK,
};

enum class E_ACHIEVE_NORMAR_TYPE
{
    ACHIEVE_NONE = 0,
    ACHIEVE_PET = 1,
    ACHIEVE_COSTUME = 2,
    ACHIEVE_ARTIFACT = 3,
    ACHIEVE_BUILDING = 4,
    ACHIEVE_MYHOME = 5,
    ACHIEVE_NEW_RAID = 7,
};

enum class E_BAN_TYPE
{
    BAN_PERM = 0,
    BAN_PERIOD,
    BAN_TRANSFER,
    BAN_NONE
};


enum class E_ADS_INTERSTITIAL_TYPE
{
    NONE = 0,
    PRISON_PLUS = 1,
};

enum class E_MAF_ADS_TYPE
{
    NONE = 0,
    GAMEMANG,
};

enum class E_PET_EQUIP_TYPE
{
    //펫 장비 테이블 샘플 > 수식 분류 첫 번째 수식기준으로 타입명 작성
    NONE = 0,
    CHICK_DMG_1,
    HAMSTER_GOLD_2,
    PURIRING_KEY_3,
    MOLE_MINE_4,
    DRAGON_DMG_5,
    PRINCESS_DELAY_6,
    CHICK_PROBABILITY_7,
    CHICK_DELAY_8,
    MONSTER_GOLD_9,
    PURIRING_MONSTER_KEY_10,
    CRI_DMG_11,
    ALL_DMG_12
};

enum class E_PET_BOX_TYPE
{
    NONE = 0,
    COMMON,
    EVENT,
    CASH
};

enum class E_TIMING_PACKAGE_TYPE
{
    NONE = 0,
    FREE = 1,
    TERMS_FREE = 2,
    PURCHASE = 3,
};

enum class E_TIMING_PACKAGE_CONDITION_TYPE
{
    DESTORY = -1,
    NONE = 0,
    HIGH_FLOOR = 1,
    PRISON_ENTER = 2,
    PRISON_FLOOR = 3,
    
    ARTI_ATK_EVOL = 4,
    ARTI_CRI_EVOL = 5,
    ARTI_QUEST_EVOL = 6,
    ARTI_KILL_EVOL = 7,
    ARTI_CROSS_EVOL = 8,
    
    ARTI_ATK_EVOL_BEYOND = 9,
    ARTI_CRI_EVOL_BEYOND = 10,
    ARTI_QUEST_EVOL_BEYOND = 11,
    ARTI_KILL_EVOL_BEYOND = 12,
    ARTI_CROSS_EVOL_BEYOND = 13,
    
    END
};

enum class E_RAID_ACCRUE_EFFECT_TYPE
{
    NONE = 0,
    DAMAGE,
    GOLD,
    KEY
};

enum class E_SPIRIT_BUFF_TYPE
{
    NONE = 0,
    DAMAGE
};

enum class E_ADVENTURE_PROPERTY_TYPE
{
    NONE = 0,
    AREA_CORE_INCREASE,
    ALL_TIME_DECREASE,
    PARTY_TIME_DECREASE,
    AREA_STAGE_OPEN,
    REWARD_BONUS_RATE,
    PARTY_MEMBER_OPEN,
    BASIC_REWARD_INCREASE,
    BONUS_CORE_INCREASE,
    ALL_PARTY_REWARD_INCREASE,
    PARTY_ALL_AREA_REWARD_INCREASE,
    PARTY_GEM_REWARD_INCREASE,
    PARTY_GRASS_REWARD_INCREASE,
    PARTY_SNOW_REWARD_INCREASE,
    PARTY_DESERT_REWARD_INCREASE,
    PARTY_VOLCANO_REWARD_INCREASE,
    ADVENTURE_EXP_INCREASE,
    DUNGEON_ATK,
    CRITICAL_ATK,
    SPIRIT_ATK,
    SPIRIT_COLLECT_ATK,
    CHICK_ATK,
    CHICK_SKILL_ATK,
    REVIVE_POINT_INCREASE,
    REVIVE_KEY_INCREASE,
    PRINCESSES_SKILL_FLOOR_INCREASE,
    PET_EQUIP_COLLECT_ATK_BONUS,
    BADGE_ATK_BONUS,
    ARTIFACT_GRADE_BONUS_KILL_GOLD,
    ARTIFACT_GRADE_BONUS_QUEST_GOLD,
    ARTIFACT_GRADE_BONUS_CRI,
    ARTIFACT_GRADE_BONUS_ATK,
    COIN_BANK_GRADE_BONUS_ATK,
    ADVENTURE_PARTY_LEVEL_BONUS_CRI,
    AREA_GRASS_RELIC_LEVEL_KEY,
    AREA_GRASS_RELIC_LEVEL_GOLD,
    AREA_GRASS_RELIC_LEVEL_ATK,
    AREA_DESERT_RELIC_LEVEL_KEY,
    AREA_DESERT_RELIC_LEVEL_GOLD,
    AREA_DESERT_RELIC_LEVEL_ATK,
    AREA_SNOW_RELIC_LEVEL_KEY,
    AREA_SNOW_RELIC_LEVEL_GOLD,
    AREA_SNOW_RELIC_LEVEL_ATK,
    AREA_VOLCANO_RELIC_LEVEL_KEY,
    AREA_VOLCANO_RELIC_LEVEL_GOLD,
    AREA_VOLCANO_RELIC_LEVEL_ATK,
    ALL_RELIC_LEVEL_DUNGEON_ATK,
    ALL_RELIC_LEVEL_CRITICAL_ATK,
    ALL_RELIC_LEVEL_SPIRIT_ATK,
    ALL_RELIC_LEVEL_SPIRIT_COLLECT_ATK,
    ADVENTURE_LEVEL_ATK,
    ADVENTURE_LEVEL_CRI,
    ADVENTURE_LEVEL_SPIRIT_ATK,
    ADVENTURE_LEVEL_SPIRIT_COLLECT_ATK,
    CORE_LEVEL_GRASS_ATK,
    CORE_LEVEL_DESERT_ATK,
    CORE_LEVEL_SNOW_ATK,
    CORE_LEVEL_VOLCANO_ATK,
    FURNITURE_LEVEL_ATK,
    FURNITURE_LEVEL_CRI,
    FURNITURE_LEVEL_SPIRIT_ATK,
    FURNITURE_LEVEL_SPIRIT_COLLECT_ATK,
    ADVENTURE_EXPLORING_ATK,
    ADVENTURE_EXPLORING_CRI,
    ADVENTURE_EXPLORING_SPIRIT_ATK,
    ADVENTURE_EXPLORING_SPIRIT_COLLECT_ATK
};
enum class E_ADVENTURE_PROPERTY_CALCULATE_TYPE
{
    DUNGEON_ATK,
    DUNGEON_CRI,
    DUNGEON_GOLD,
    SPIRIT_ATK,
    SPIRIT_COLLECT_ATK
};
enum class E_CHAT_TYPE
{
    NORMAL = 0,
    RAID = 1,
};

enum class E_REPORT_TYPE
{
    NONE = 0,
    HACK = 1,
    NICK = 2,
    CHAT = 3,
    
    OTHER = 9
};

enum class E_ADVENTURE_MISSION_GROUP
{
    NONE = 0,
    DAILY,
    WEEKLY
};

enum class E_ADS_WATCH{
    NONE,
    BUFF_COIN,
    BUFF_ATK,
    BUFF_SPD,
    BUFF_ALL,
    BUFF_RAND,
    BUFF_ATK_9,
    BUFF_ATK_10,
    BUFF_DEVIL_RAND,
    BUFF_D_ATK_DMG,
    BUFF_D_ATK_SPD,
    BUFF_D_MOVE_SPD,
    BUFF_D_QUEST_GOLD,
    BUFF_EVENT_TIME,
    ITEM_TICKET,
    ITEM_GOLD,
    ITEM_GEM,
    ITEM_CUBE,
    ITEM_DEVILCOIN,
    ITEM_KEY,
    ITEM_POINT,
    ITEM_DEVIL_GOLD,
    ITEM_PTICKET,
    ITEM_PET_EQUIP_BOX,
    ITEM_BONG,
    MINE_RAND_1,
    MINE_RAND_2,
    MINE_RAND_3,
    MINE_RAND_4,
    MINE_RAND_5,
    MINE_RAND_6,
    ITEM_NEW_RAID_DRAGON_BALL,
};

enum class E_PRODUCT
{
    NONE = 0,
    DEFAULT,
    DAILY,
    BONUS,
    PHASE,
    EVENT_BANNER_NEW,
    EVENT_RETURN,
    PET_EQUIP,
};

enum class E_RANK_LOCAL_TYPE
{
    NONE = 0,
    
    HIGH_FLOOR = 1,
    NEW_RAID = 3,
    EVENT_RACE = 4,
    FLASHMAN = 5,
    END
};

enum class E_RANK_TYPE
{
    NONE = 0,
    
    RANK_HIGH_FLOOR = 1,
    END
};
enum class E_RANK_FLUCTUATION
{
    NONE = 0,
    
    UP,
    DOWN
};
enum class E_RANK_PAGE
{
    NONE = 0,
    
    PAGE_NEXT = 1,
    PAGE_PREV = 2,
};

enum class E_COMIC{
    NONE = 0,
    
    BEGINNING,
    CLEAR_100,
    CLEAR_300,
    CLEAR_600,
    CLEAR_900,
    CLEAR_1000,
    COSTUME_ROOM,
    DUNGEON_JOIN,
    ARTIFACT_EAGLE,
    ARTIFACT_RING,
    CLEAR_1100,
    CLEAR_1200,
    CLEAR_1400,
    CLEAR_1700,
    CLEAR_2000,
    MAX_ARTIFACT_1,
    MAX_ARTIFACT_3,
    MAX_ARTIFACT_4,
    MAX_ARTIFACT_8,
    CLEAR_2200,
    MAX_ARTIFACT_12,
    CLEAR_3000,
    CLEAR_4000,
    CLEAR_5000,
    CLEAR_6000,
    CLEAR_7000,
    CLEAR_8000,
    CLEAR_4500,
    CLEAR_6500,
    MINE_JOIN,
    CLEAR_9000,
    CLEAR_10000,
    CLEAR_REALENDING,
    TOWN_JOIN,
    TUTORIAL,
    DEVIL_START,
    DEVIL_1000,
    DEVIL_2000,
    DEVIL_3000,
    DEVIL_4000,
    CLEAR_777,
    CLEAR_2500,
    DEVIL_5000,
    CLEAR_10,
    QUEST_OPEN_24,
    DEVIL_6000,
    DEVIL_7000,
    DEVIL_8000,
    DEVIL_9000,
    DEVIL_10000,
    MYHOME,
    BAFOMETZ,
    JELLY,
    PRISON_FIRE_JOIN,
    CLEAR_7600,
    CLEAR_5600,
    CLEAR_6600,
    EVENT_SUMMER,
    QUEST_OPEN_7,
    CLEAR_150,
    ARTIFACT_STORMY,
    QUEST_OPEN_11,
    RAID_JOIN,
    REVIVE,
    PET_EQUIP_JOIN,
    BEELZEBUB,
    EVENT_HALLOWEEN,
    EVENT_CHRISTMAS,
    CLEAR_12000,
    CLEAR_14000,
    EXPEDITION_JOIN,
    CLEAR_16000,
    CLEAR_18000,
    EVENT_SUMMER_FISHING,
    TEAM_RAID,
    EXPEDITION_CAT_1,
    EXPEDITION_CAT_2,
    EXPEDITION_KRAKEN,
    EVENT_RETURN_ATTEND,
};

enum class E_REFRESH {
    NONE = 0,
    
    CURRENCY,
    
    TUTORIAL,
    
    COSTUME,
    COSTUME_BUY,
    COSTUME_DUMMY,
    
    ARTIFACT,
    
    QUEST_INFO,
    QUEST_BUY,
    
    WEAPON_INFO,
    WEAPON_BUY,
    WEAPON_BUY_LEGEND,
    WEAPON_BUY_PERMANENT,
    WEAPON_BUY_REINFORCE,
    
    BADGE,
    
    PET_FREE,
    PET,
    PET_EQUIP,
    
    MINE,
    MINE_REFINE_UPGRADE,
    MINE_REFINE_PRODUCTION,
    
    MAIL,
    
    RAID_STAT,
    RAID_ATTACK,
    RAID_ALL,
    
    SPIRIT,
    
    CHATTING,
    
    ITEM_FEVER,
    
    ADVENTURE,
    ADVENTURE_TUTORIAL_RUNNING,
    ADVENTURE_TUTORIAL_COMPLETE,
    
    LOOT_TRADE,
    
    BUFF_BUY,
    BUFF_BUY_DEVIL,
    
    PRISON_FROST_INFO,
    
    FLASHMAN_INFO,
    
    STONE_BUY,
    STONE_STORAGE_BUY,
    
};

enum class E_REFRESH_REDDOT {
    NONE = 0,
    
    EQUIP_CRAFT,
    
    PET_EVOLUTION_CHICK,
    PET_EVOLUTION_HAMSTER,
    PET_EVOLUTION_DRAGON,
    PET_EVOLUTION_PRINCESS,
    PET_EVOLUTION_PURIRING,
    PET_EVOLUTION_HONG,
    PET_EVOLUTION_MOLE,
    PET_EVOLUTION_HANDSOME,
    PET_EVOLUTION_ALL,
    
    PET_REINFORCE_CHICK,
    PET_REINFORCE_HAMSTER,
    PET_REINFORCE_DRAGON,
    PET_REINFORCE_PRINCESS,
    PET_REINFORCE_PURIRING,
    PET_REINFORCE_HONG,
    PET_REINFORCE_MOLE,
    PET_REINFORCE_HANDSOME,
    PET_REINFORCE_ALL,
    
    PET_EQUIP_CHICK,
    PET_EQUIP_HAMSTER,
    PET_EQUIP_DRAGON,
    PET_EQUIP_PRINCESS,
    PET_EQUIP_PURIRING,
    PET_EQUIP_HONG,
    PET_EQUIP_MOLE,
    PET_EQUIP_HANDSOME,
    PET_EQUIP_ALL,
    PET_EQUIP_BOX,
    
    FLASHMAN_MISSION_COMPLETE,
    
    DUNGEONPASS_REWARD,
    
    PURCHASE_BANNER,
    
    DEX_LOOTS_LEVELUP,
    DEX_MONSTER_LEVELUP,
    
    EVENT_RACE_MISSION,
    EVENT_SUNFISH_DEX_ALL,
    EVENT_SUNFISH_DEX_FOOD,
    EVENT_SUNFISH_DEX_ADVENTURE,
};

enum class E_HISTORY_DATA{
    //enum 순서가 변경되면 안된다. 무조건 밑으로 추가하고 정렬을 따로 할것.
    
    K_ATK_SEC = 0 ,      /*저장X 현재의 수치만 구하면된다*/
    K_CRI_SEC,      /*저장X 현재의 수치만 구하면된다*/
    K_QUEST_SEC,    /*저장X 현재의 수치만 구하면된다*/
    K_REWARD_SEC,   /*저장X 현재의 수치만 구하면된다*/
    K_MOVE,         /*저장X 현재의 수치만 구하면된다*/
    
    K_START_DAY, /*지감가입날짜받아온다*/
    K_PLAY_TIME, /**/
    K_WEAPON_HIGH,  /*기존저장중*/
    K_PRISON_METER,
    K_PRISON_COUNT,
    K_RAID_TOTAL_DMG,
    K_RAID_HIGH_DMG,
    K_MINE_HIGH,
    
    K_REVIVAL,  /*기존저장중*/
    K_REVIVAL_2,
    K_REVIVAL_3,
    K_REBORN_KEY,
    K_ONE_SHOT,
    K_HIGH_FLOOR, /*기존저장중*/
    K_TOTAL_FLOOR, /*기존저장중*/
    K_TOTAL_GEM,
    K_TOTAL_KEY,
    K_VIP_DAY,      /*저장X 출책창에서 최신데이터 얻기가능*/
    K_GIFT_COUNT,
    K_BUFF_COUNT,
    
    K_VIDEO_COUNT,
    K_MINE_VIDEO_COUNT, //광물 버프인데, 카운팅용도로만 씌인다. 광물의 랜덤지급 요소를 제거하기위해
    K_LAST_DEVICE_TIME, //기기의 최종시간을 저장. 오프라인 골드보상시 이용.
    
    D_REVIVAL,
    D_REVIVAL_2,
    D_REVIVAL_3,
    D_REBORN_KEY,
    D_ONE_SHOT,
    D_HIGH_FLOOR,
    D_TOTAL_FLOOR,
    D_TOTAL_KEY,
    
    D_WEAPON_HIGH,
    D_HEAD_HIGH,
    D_BODY_HIGH,
    D_CAPE_HIGH,
    
    D_PET_LEVEL_1,
    D_PET_LEVEL_2,
    D_PET_LEVEL_3,
    D_PET_LEVEL_4,
    
    D_ARTIFACT_HIGH_ATK,
    D_ARTIFACT_HIGH_CRI,
    D_ARTIFACT_HIGH_GOLD,
    D_ARTIFACT_HIGH_ATKSPD,
    D_ARTIFACT_HIGH_MOVESPD,
    
    K_NONE_ONLY_COUNT,  //카운트 용도로만 씌인다
    
    K_PRISON_HELL_METER,
    
    END
};

#pragma mark - Enum Devil ============================================================================================================
struct stEvolutionDevil{
    double devilKey;
    int costumeCoin;
};

enum E_EQUIP_TYPE{
    E_D_NONE=-1,
    E_D_WEAPON=0,
    E_D_HEAD,
    E_D_BODY,
    E_D_CAPE,
};

enum E_EQUIP_IDX{
    E_D_IDX_1=1,
    E_D_IDX_2,
    E_D_IDX_3,
    E_D_IDX_4,
    E_D_IDX_5,
    E_D_IDX_6,
    E_D_IDX_7,
    E_D_IDX_8,
    E_D_IDX_9,
    E_D_IDX_10,
    E_D_IDX_11,
    E_D_IDX_12,
    E_D_IDX_13,
    E_D_IDX_14,
    E_D_IDX_15,
    E_D_IDX_16,
    E_D_IDX_17,
};

enum E_EQUIP_OPTION{
    E_D_CRITICAL,
    E_D_DMG,
    E_D_QUEST_TIME,
    E_D_QUEST_REWARD,
    E_D_SPEED,
    E_D_KEY,
    E_D_CRI_DMG,
};

enum E_D_ARTIFACT_TYPE{
    E_D_ARTI_TYPE_NONE=-1,
    E_D_TYPE_1_ATK=0,
    E_D_TYPE_2_CRI,
    E_D_TYPE_3_GOLD,
    E_D_TYPE_4_ATK_SPD,
    E_D_TYPE_5_MOVE_SPD,
    
    E_D_TYPE_6_ARTIFACT,//일반던전 보물. 석판이랑 관계없음.
};

enum class E_Poolable_Object_Type
{
    NONE = 0,
    DF_BULLET,
    DF_EXPLOSION,
    DF_MONSTER,
    DF_EFFECT,
    DF_HPBAR
};

#pragma mark - Enum Defense ============================================================================================================
enum class E_DF_AttackType
{
    NONE = 0,
    TARGETING,
    NONE_TARGETING,
    GUIDED,
};
enum class E_DF_TargetType
{
    NONE = 0,
    HEAD,
    TAIL,
};
enum class E_DF_MonsterType
{
    NONE = 0,
    NORMAL,
    RUNNER,
    TANKER,
    BOSS,
};
enum class E_DF_ExplosionType
{
    NONE = 0,
    EXPLOSION_3X3,
    EXPLOSION_4X4,
};

enum class E_DF_MuzzleEffectType
{
    NONE = 0,
    SMALL_TYPE,
    STLONG_TYPE,
    BIG_STLONG_TYPE,
    IMAGE_NUM_TYPE,
};

enum class E_DF_CartridgeType
{
    NONE = 0,
    BULLET,
};
enum class E_DF_HitEffectType
{
    NONE = 0,
    SMALL,
    MEDIUM,
    LARGE
};
enum class E_DF_PierceType
{
    NONE = 0,
    PIERCING,
};

enum class E_DF_ProjectileType
{
    NONE = 0,
    PROJECTILE,
    AOE,
};

enum class E_DF_TrajectoryType
{
    NONE = 0,
    STRAIGHT_LINE,
    RADIATING
};

enum class E_DF_WeaponType
{
    NONE = 0,
    PISTOL,
    RIFLE,
    SHOT_GUN,
    MACHINE_GUN,
    ROCKET_LAUNCHER,
    LASER,
};

enum class E_DF_Trench_SettingType
{
    NONE = 0,
    WEAPON,
    CHARACTER
};

enum class E_DF_AMP_STONE_Type
{
    NONE = 0,
    BLACK,
    RED,
    YELLO
};

#endif /* ConfigEnum_h */
