//
//  IncludeDefault.h
//

#ifndef Default_h
#define Default_h

/** cocos2d */
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

#include <external/json/document.h>
#include <external/json/writer.h>
#include <external/json/stringbuffer.h>

/** cocos2d - custom action */
#include "Common/Cocos2d/Action/NumberFromTo.h"
#include "Common/Cocos2d/Action/Shake.h"
#include "Common/Cocos2d/Action/TextTyping.h"
#include "Common/Cocos2d/Action/TintBlink.h"

/** cocos2d - custom */
#include "Common/Cocos2d/CListView.h"
#include "Common/Cocos2d/CText.h"

/** types */
#include "Common/Types/TInteger.h"
#include "Common/Types/TDouble.h"
#include "Common/IntPoint.h"
#include "Common/MafColider.h"

/** utils */
#include "Utils/UtilsDate.h"
#include "Utils/UtilsRect.h"
#include "Utils/UtilsScene.h"
#include "Utils/UtilsString.h"
#include "Utils/UtilsWidget.h"

/** libs */
#include "Libs/coroutine/Coroutine.h"
#include "Libs/csv/CSVParse.h"
#include "Libs/crashlytics/CrashlyticsSdk.h"

/** libs maf */
#include "Libs/maf/MafNative.h"
#include "Libs/maf/MafUtils.h"
#include "Libs/maf/MafHttpNew.h"
#include "Libs/maf/MafAes256.h"
#include "Libs/maf/MafNode.h"
#include "Libs/maf/MafMath.h"
#include "Libs/maf/MafConfig.h"
#include "Libs/maf/cpplinq.hpp"
#include "Libs/maf/MafShareSNS.h"
#include "Libs/maf/MafGridView.h"
#include "Libs/maf/Ads/MafGoogleRewardAds.h"
#include "Libs/maf/Ads/MafGooglePlay.h"
#include "Libs/maf/Analytics/MafAnalyticsManager.h"
#include "Libs/maf/Analytics/MafRemoteConfig.h"
#include "Libs/maf/Function/MafDelegate.hpp"
#include "Libs/maf/Analytics/MafAppsFlyer.h"

/** debug */
#include "Common/Support/DebugLog.h"

/** system manager */
#include "ManagerSystem/AdsUtilsManager.h"
#include "ManagerSystem/MafRewardAdsManager.h"
#include "ManagerSystem/ParameterManager.h"
#include "ManagerSystem/RefreshManager.h"
#include "ManagerSystem/RefreshReddotManager.h"
#include "ManagerSystem/SoundManager.h"
#include "ManagerSystem/TableManager.h"
#include "ManagerSystem/TextManager.h"

#define GAME_WIDTH  750.0f
#define GAME_HEIGHT 1334.0f

#define DUNGEON_SERVER_URL_TEST1        "http://10.10.13.144:5000/ios_game"
#define DUNGEON_SERVER_URL_TEST2        "http://10.10.13.165:5000/ios_game"
#define DUNGEON_SERVER_URL_DEV          "https://moneyhero-test-server.appspot.com/ios_game"
#define DUNGEON_SERVER_URL_LIVE         "https://moneyhero.mafserver.com/ios_game"

#define REMOTE_CONFIG_DEFAULT_DATA "remote_config_data.txt"

#define GAME_FONT           "Fonts/NotoSansCJKjp-Medium_small.otf"
#define GAME_FONT_AL        "Fonts/visitor1.ttf"
#define GAME_BM_FONT_AL     "Fonts/font_visitor.fnt"
#define SYSTEM_FONT         "Helvetica"

#define DATA_FILE_D2_EQUIP              "Data/info_d2_equip.txt"
#define DATA_FILE_D2_ARTIFACT           "Data/info_d2_artifact.txt"
#define DATA_FILE_ADS "Maf/ads/ads.txt"

#define PIXEL "Maf/10x10.png"

#define C_COUNT_PLACE 3

#define COLOR_COIN      Color3B(255,216,0)
#define COLOR_KEY       Color3B(231, 164, 48)
#define COLOR_GEM       Color3B(241,71,71)
#define COLOR_POINT       Color3B::GRAY
#define COLOR_COSTUME_COIN  Color3B(255,223,28)

#define COLOR_TITLE     Color3B(255,210,28)
#define COLOR_MATERIAL  Color3B(92, 203, 255)
#define COLOR_HELL      Color3B(239, 0, 144)
#define COLOR_HONOR     Color3B(113, 255, 61)

#define COLOR_UPGRADE_TOP Color3B(255,255,255)
#define COLOR_TALK      Color3B(22, 60, 63)

#define COLOR_D_COIN      Color3B(255,216,0)
#define COLOR_D_KEY       Color3B(78,231,48)

// noti zorder
#define ZORDER_NOTI_LOADING 1002
#define ZORDER_NOTI_LOADINGLOCK 1003
#define ZORDER_NOTI_LOCK 1004

// noti tag
#define TAG_NOTI_LOADING 1002
#define TAG_NOTI_LOADINGLOCK 1003
#define TAG_NOTI_LOCK 1004
#define TAG_NOTI_LOADING_SPIRIT_SUMMON 1005

// scene zorder
#define ZORDER_SCENE_PRISON 999
#define ZORDER_SCENE_POPUP 1000
#define ZORDER_SCENE_TOAST 1001
#define ZORDER_SCENE_TOAST_REWARD 1002
#define ZORDER_SCENE_TOAST_BUFF 1003
#define ZORDER_SCENE_POPUP_STORY 1004
#define ZORDER_SCENE_POPUP_NOTIC 1005
#define ZORDER_SCENE_TOAST_PET 1006
#define ZORDER_SCENE_LOG 1007
#define ZORDER_SCENE_TOUCH 1008
#define ZORDER_SCENE_FORCE_TUTORIAL 1001

// scene tag
#define TAG_SCENE_PRISON 999
#define TAG_SCENE_POPUP 1000
#define TAG_SCENE_TOAST 1001
#define TAG_SCENE_TOAST_REWARD 1002
#define TAG_SCENE_TOAST_BUFF 1003
#define TAG_SCENE_TOAST_PET 1004
#define TAG_SCENE_LOG 1006
#define TAG_SCENE_TOUCH 1007

//
static const cocos2d::Texture2D::TexParams texParams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };

//
#define LOG(format, ...)      DebugLog::log(format, ##__VA_ARGS__)


#endif /* Default_h */
