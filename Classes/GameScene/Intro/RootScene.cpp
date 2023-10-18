//
//  RootScene.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 31/05/2019.
//

#include "GameScene/Intro/RootScene.h"

// lib firebase crash


USING_NS_CC;

RootScene::RootScene() :
_bInitUI(false)
{
    
}

RootScene::~RootScene()
{
    
}

bool RootScene::init()
{
    if ( !BaseScene::init() )
    {
        return false;
    }
     
    
    // event init
    onInit();
    
    // enter callback
    setOnEnterTransitionDidFinishCallback([=](){
        //
        if ( _bInitUI == false )
        {
            // event start
            onStart();
            
            // set init
            _bInitUI = true;
        }
        
        // event refresh
        onRefresh();
    });
     
    return true;
}
 

void RootScene::onInit()
{
    // lib firebase crash
    CrashlyticsSdk::getInstance();
    
    // lib Analytics
    MafAnalyticsManager::InitAnalytics();
    
    // lib RemoteConfig
    MafRemoteConfig::InitRemoteConfigAndLoadDefaultData(REMOTE_CONFIG_DEFAULT_DATA);
    
    // lib ads
    MafGoogleRewardAds::InitAds([=](){
        #if COCOS2D_DEBUG == 0
            MafGoogleRewardAds::InitGoogleRewardAd(ADMOB_ID_REWARD);
            MafGoogleRewardAds::InitGoogleInterstitial(ADMOB_ID_INTERSTITIAL);
            MafGoogleRewardAds::InitGoogleBanner(ADMOB_ID_BANNER);
            MafGoogleRewardAds::InitGoogleTopBanner(ADMOB_ID_BANNER);
        #elif COCOS2D_DEBUG == 1 && (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            MafGoogleRewardAds::InitGoogleRewardAd("6200ff0c2a918d000100000d");
            MafGoogleRewardAds::InitGoogleInterstitial("6200fef52a918d0001000007");
            MafGoogleRewardAds::InitGoogleBanner("6200fee42a918d0001000003");
            MafGoogleRewardAds::InitGoogleTopBanner("6200fee42a918d0001000003");
        #elif COCOS2D_DEBUG == 1 && (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            MafGoogleRewardAds::InitGoogleRewardAd("61ee2e91cb8c67000100002f");
            MafGoogleRewardAds::InitGoogleInterstitial("61ee2e3fcb8c67000100002e");
            MafGoogleRewardAds::InitGoogleBanner("61ee2b7dcb8c67000100002a");
            MafGoogleRewardAds::InitGoogleTopBanner("61ee2b7dcb8c67000100002a");
        #endif
    });
    
}

void RootScene::onStart()
{
    // noti
    auto nodeNotify = Director::getInstance()->getNotificationNode();
    if ( nodeNotify != nullptr )
    {
        
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
        nodeNotify->addChild(DebugLog::create(), ZORDER_SCENE_LOG, TAG_SCENE_LOG);
#endif
    }
}

void RootScene::onRefresh()
{
    UtilsScene::getInstance()->scenePush(UtilsScene::eType::scene_splash);
}
