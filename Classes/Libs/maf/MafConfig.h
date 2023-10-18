//
//  MafConfig.h
//  TapRed
//
//  Created by SanHeo on 2014. 8. 1..
//
//

#ifndef TapRed_MafConfig_h
#define TapRed_MafConfig_h

#define EMAIL_HELP      "support@midagedknight.zendesk.com"

#define AES_KEY "abcdefghijklmnopqrstu123456vwxyz"
enum ADS_RESULT //InAppPurhcase
{   
    SUCCESS =0,
    FAIL,
    BACKGROUND_SKIP,
    LOAD
};

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#define URL_THISGAME      "https://itunes.apple.com/app/id987072755"

//#define ADMOB_ID        "ca-app-pub-9587898460919430~5176058508"
//#define ADMOB_ID_REWARD	"ca-app-pub-9587898460919430/8029179740"
//#define ADMOB_ID_INTERSTITIAL "ca-app-pub-9587898460919430/7688183216"
//#define ADMOB_ID_BANNER "ca-app-pub-9587898460919430/3178383138"
//#define APPLOVINMAX_ID_REWARD    "bce58dc388ade248"
//#define FACEBOOK_PLACEMENT_ID    "1054442607919851_2821603797870381"
#define ADMOB_ID_REWARD    "62a83d216da6b100010002b1"
#define ADMOB_ID_INTERSTITIAL "62a83d116da6b100010002a5"
#define ADMOB_ID_BANNER "62a83cf36da6b10001000299"

#define APPS_FLYER_DEV "eR8sp5swMY94vfjrfQgd47"
#define APPS_FLYER_APPID "987072755"

#define LEADERBOARD_HIGH    "leaderboard_high"
#define LEADERBOARD_TOTAL   "leaderboard_total"
#define LEADERBOARD_ARTIFACT1  "leaderboard_power"
#define LEADERBOARD_ARTIFACT4  "leaderboard_eagle"
#define LEADERBOARD_REVIVE      "leaderboard_revive"

#define ACHIEVE_50_WOOD         "ach_01"
#define ACHIEVE_300_MOUSE       "ach_02"
#define ACHIEVE_100_REVIVE      "ach_03"
#define ACHIEVE_100_NOQUEST     "ach_04"
#define ACHIEVE_1_ALLQUEST      "ach_05"
#define ACHIEVE_300_FLOOR       "ach_06"
#define ACHIEVE_500_FLOOR       "ach_07"
#define ACHIEVE_700_FLOOR       "ach_08"
#define ACHIEVE_1000_FLOOR      "ach_09"
#define ACHIEVE_200_NOREVIVE    "ach_10"
#define ACHIEVE_10_REVIVE       "ach_11"
#define ACHIEVE_20_REVIVE       "ach_12"
#define ACHIEVE_50_REVIVE       "ach_13"
#define ACHIEVE_100_DRAGON      "ach_14"
#define ACHIEVE_ALLWEAPON       "ach_15"
#define ACHIEVE_1200_FLOOR      "ach_16"
#define ACHIEVE_1400_FLOOR      "ach_17"
#define ACHIEVE_1600_FLOOR      "ach_18"
#define ACHIEVE_1800_FLOOR      "ach_19"
#define ACHIEVE_2000_FLOOR      "ach_20"
#define ACHIEVE_30000_ARTIFACT1 "ach_21"
#define ACHIEVE_1000_DRAGON     "ach_22"
#define ACHIEVE_20000_ARTIFACT4 "ach_23"
#define ACHIEVE_10_QUEST25      "ach_24"
#define ACHIEVE_1000_REVIVE     "ach_25"
#define ACHIEVE_2000_DUNGEON    "ach_26"

#define ACHIEVE_2000_ONEKILL    "ach_27"
#define ACHIEVE_GOD_1000KILL    "ach_28"
#define ACHIEVE_5000_DUNGEON    "ach_29"
#define ACHIEVE_ATTEND_20       "ach_30"
#define ACHIEVE_COSTUME_90      "ach_31"
#define ACHIEVE_500_QUEST25     "ach_32"
#define ACHIEVE_1000_WOOD       "ach_33"
#define ACHIEVE_5000_REVIVE     "ach_34"
#define ACHIEVE_2500_FLOOR      "ach_35"
#define ACHIEVE_3000_FLOOR      "ach_36"

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#define URL_THISGAME      "market://details?id=com.maf.moneyhero"

//#define ADMOB_ID        "ca-app-pub-9587898460919430~8414216501"
//#define ADMOB_ID_REWARD	"ca-app-pub-9587898460919430/5343306659"
//#define ADMOB_ID_REWARD_INTERSTITIAL "ca-app-pub-9587898460919430/5806953117"
//#define ADMOB_ID_INTERSTITIAL "ca-app-pub-9587898460919430/9404174236"
//#define ADMOB_ID_BANNER "ca-app-pub-9587898460919430/9430459549"
//#define APPLOVINMAX_ID_REWARD    "cc6075844df01f27"
//#define FACEBOOK_PLACEMENT_ID    "1054442607919851_2834304686600292"
#define ADMOB_ID_REWARD    "62a836f36da6b10001000231"
#define ADMOB_ID_INTERSTITIAL "62a836ec6da6b10001000226"
#define ADMOB_ID_BANNER "62a836e36da6b1000100021b"

#define APPS_FLYER_DEV "eR8sp5swMY94vfjrfQgd47"
#define APPS_FLYER_APPID "don't use android"


#define LEADERBOARD_HIGH        "CgkI0M-a3KcXEAIQKw"
#define LEADERBOARD_TOTAL       "CgkI0M-a3KcXEAIQAg"
#define LEADERBOARD_ARTIFACT1  "CgkI0M-a3KcXEAIQHg"
#define LEADERBOARD_ARTIFACT4  "CgkI0M-a3KcXEAIQHw"
#define LEADERBOARD_REVIVE      "CgkI0M-a3KcXEAIQIA"

#define ACHIEVE_50_WOOD         "CgkI0M-a3KcXEAIQAw"
#define ACHIEVE_300_MOUSE       "CgkI0M-a3KcXEAIQBA"
#define ACHIEVE_100_REVIVE      "CgkI0M-a3KcXEAIQBQ"
#define ACHIEVE_100_NOQUEST     "CgkI0M-a3KcXEAIQBg"
#define ACHIEVE_1_ALLQUEST      "CgkI0M-a3KcXEAIQBw"
#define ACHIEVE_300_FLOOR       "CgkI0M-a3KcXEAIQCA"
#define ACHIEVE_500_FLOOR       "CgkI0M-a3KcXEAIQCQ"
#define ACHIEVE_700_FLOOR       "CgkI0M-a3KcXEAIQCg"
#define ACHIEVE_1000_FLOOR      "CgkI0M-a3KcXEAIQCw"
#define ACHIEVE_200_NOREVIVE    "CgkI0M-a3KcXEAIQDA"
#define ACHIEVE_10_REVIVE       "CgkI0M-a3KcXEAIQDQ"
#define ACHIEVE_20_REVIVE       "CgkI0M-a3KcXEAIQDg"
#define ACHIEVE_50_REVIVE       "CgkI0M-a3KcXEAIQDw"
#define ACHIEVE_100_DRAGON      "CgkI0M-a3KcXEAIQEA"
#define ACHIEVE_ALLWEAPON       "CgkI0M-a3KcXEAIQEQ"
#define ACHIEVE_1200_FLOOR      "CgkI0M-a3KcXEAIQEw"
#define ACHIEVE_1400_FLOOR      "CgkI0M-a3KcXEAIQFA"
#define ACHIEVE_1600_FLOOR      "CgkI0M-a3KcXEAIQFQ"
#define ACHIEVE_1800_FLOOR      "CgkI0M-a3KcXEAIQFg"
#define ACHIEVE_2000_FLOOR      "CgkI0M-a3KcXEAIQFw"
#define ACHIEVE_30000_ARTIFACT1 "CgkI0M-a3KcXEAIQGA"
#define ACHIEVE_1000_DRAGON     "CgkI0M-a3KcXEAIQGQ"
#define ACHIEVE_20000_ARTIFACT4 "CgkI0M-a3KcXEAIQGg"
#define ACHIEVE_10_QUEST25      "CgkI0M-a3KcXEAIQGw"
#define ACHIEVE_1000_REVIVE     "CgkI0M-a3KcXEAIQHA"
#define ACHIEVE_2000_DUNGEON    "CgkI0M-a3KcXEAIQHQ"

#define ACHIEVE_2000_ONEKILL    "CgkI0M-a3KcXEAIQIQ"
#define ACHIEVE_GOD_1000KILL    "CgkI0M-a3KcXEAIQIg"
#define ACHIEVE_5000_DUNGEON    "CgkI0M-a3KcXEAIQIw"
#define ACHIEVE_ATTEND_20       "CgkI0M-a3KcXEAIQJA"
#define ACHIEVE_COSTUME_90      "CgkI0M-a3KcXEAIQJQ"
#define ACHIEVE_500_QUEST25     "CgkI0M-a3KcXEAIQJg"
#define ACHIEVE_1000_WOOD       "CgkI0M-a3KcXEAIQJw"
#define ACHIEVE_5000_REVIVE     "CgkI0M-a3KcXEAIQKA"
#define ACHIEVE_2500_FLOOR      "CgkI0M-a3KcXEAIQKQ"
#define ACHIEVE_3000_FLOOR      "CgkI0M-a3KcXEAIQKg"

#endif

#define FACEBOOK_PAGE_ID "555595311245535"
#define FACEBOOK_PAGE_ID_BILLYBOB "106221889741592"


#endif
