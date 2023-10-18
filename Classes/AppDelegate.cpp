#include "AppDelegate.h"

#include "Common/ConfigDefault.h"

#include "GameScene/Intro/RootScene.h"
#include "GameScene/Intro/SplashScene.h"
#include "GameScene/HelloWorldScene.h"

#include "ManagerGame/SaveManager.h"
#include "ManagerGame/UserInfoConfigManager.h"

#define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
//#elif USE_SIMPLE_AUDIO_ENGINE
//#include "audio/include/SimpleAudioEngine.h"
//using namespace CocosDenshion;
#endif

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    #include <jni.h>
//    #include "platform/android/jni/JniHelper.h"
//#endif

USING_NS_CC;

AppDelegate::AppDelegate() :
_bBackGround(false),
_nBackgroundTime(0)
{
    
}

AppDelegate::~AppDelegate()
{

}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    
    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    /// initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }
    
    // turn on display FPS
    //director->setDisplayStats(COCOS2D_DEBUG);
    
    // Set the design resolution
    cocos2d::Size designSize = cocos2d::Size(GAME_WIDTH, GAME_HEIGHT);
    cocos2d::Size deviceSize = director->getVisibleSize();
    double nRatioDesign = designSize.width / designSize.height;
    double nRatioDevice = deviceSize.width / deviceSize.height;
    if ( nRatioDesign > nRatioDevice )
    {
        designSize = cocos2d::Size(designSize.width, designSize.width / nRatioDevice);
    }
    
    double nScaleDesign = (float)deviceSize.height / designSize.height;
    designSize.width = ceilf(deviceSize.width/nScaleDesign);
    
    //
    glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::SHOW_ALL);
    
    // 리소스 경로 추가
    std::string path = FileUtils::getInstance()->getWritablePath();
    FileUtils::getInstance()->addSearchPath(path, true);

    register_all_packages();
    
    // create a scene. it's an autorelease object
    UtilsScene::getInstance()->scenePush(UtilsScene::eType::scene_root);
    director->startAnimation();
    
    // noti
    MafNative::CancelLocalNotification(E_NOTI::NOTI_OFFLINE);
    
    // notification
    auto nodeNotify = Layer::create();
    Director::getInstance()->setNotificationNode(nodeNotify);
    
    // sound
    AudioEngine::preload("", nullptr);
    
    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground()
{
    //
    Director::getInstance()->stopAnimation();
    
#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
//#elif USE_SIMPLE_AUDIO_ENGINE
//    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
//    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
    
    //
    _bBackGround = true;
    _nBackgroundTime = 0;
    
    // Game running scene
    auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
    if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) != nullptr )
    {
        SaveManager::saveAllData();
    }
    
    // Director running scene
    if ( dynamic_cast<SplashScene *>(sceneGameLayer) == nullptr && UtilsDate::getInstance()->isTimeServer() == true )
    {
        _nBackgroundTime = UtilsDate::getInstance()->getTime();
    }
    
    // noti
    if ( UserInfoConfigManager::getInstance()->isFunction(E_FUNCTION_ACTIVE::OFFLINE) == true  )
    {
        MafNative::SendLocalNotification(E_NOTI::NOTI_OFFLINE, 28800, GAME_TEXT("t_ui_push_offlinereward_title").c_str(), GAME_TEXT("t_ui_push_offlinereward_contents").c_str());
    }
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    //
    if ( _bBackGround == false )
    {
        return;
    }
    _bBackGround = false;
    
    //
    Director::getInstance()->startAnimation();
    
#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#endif
    
    // noti
    MafNative::CancelLocalNotification(E_NOTI::NOTI_OFFLINE);
    
    if ( _nBackgroundTime != 0 )
    {
        time_t nTime = (UtilsDate::getInstance()->getTime() - _nBackgroundTime) / 60;
        nTime = std::abs(nTime);
        if ( nTime > 19 )
        {
            auto scene = UtilsScene::getInstance()->getRunningLayer();
            if ( dynamic_cast<BaseGameScene *>(scene) != nullptr )
            {
                dynamic_cast<BaseGameScene *>(scene)->hidePopupAll();
            }
                        
            //
            MafHttpNew::_vecRequest.clear();
            
            //
            auto scheduler = cocos2d::Director::getInstance()->getScheduler();
            scheduler->unscheduleAllWithMinPriority(Scheduler::PRIORITY_NON_SYSTEM_MIN);
            
            MafGoogleRewardAds::HideGoogleBanner();
            Director::getInstance()->pause();
            UtilsScene::getInstance()->scenePopRoot();
            Director::getInstance()->resume();

        }
        
        //
        _nBackgroundTime = 0;
    }
}


void AppDelegate::applicationScreenSizeChanged(int newWidth, int newHeight)
{
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (glview)
    {
        auto size = glview->getFrameSize();
        if (size.equals(Size(newWidth, newHeight)))
            return;

        glview->setFrameSize(newWidth, newHeight);

        // Set the design resolution
        cocos2d::Size designSize = cocos2d::Size(GAME_WIDTH, GAME_HEIGHT);
        cocos2d::Size deviceSize = director->getVisibleSize();
        double nRatioDesign = designSize.width / designSize.height;
        double nRatioDevice = deviceSize.width / deviceSize.height;
        if ( nRatioDesign > nRatioDevice )
        {
            designSize = cocos2d::Size(designSize.width, designSize.width / nRatioDevice);
        }
        
        double nScaleDesign = (float)deviceSize.height / designSize.height;
        designSize.width = ceilf(deviceSize.width/nScaleDesign);
        
        //
        glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::SHOW_ALL);
    }
}


 
