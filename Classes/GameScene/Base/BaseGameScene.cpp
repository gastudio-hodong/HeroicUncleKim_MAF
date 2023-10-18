//
//  BaseGameScene.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 04/06/2019.
//

#include "BaseGameScene.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Base/PopupBase.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/UserInfoConfigManager.h"
#include "ManagerGame/AttackEffectManager.h"
#include "ManagerGame/RankingManager.h"

USING_NS_CC;

BaseGameScene::BaseGameScene() :
_nTouchTime(0)
{
    
}

BaseGameScene::~BaseGameScene()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool BaseGameScene::init()
{
    if ( !BaseScene::init() )
    {
        return false;
    }
    
    // event key
    auto listenerKey = EventListenerKeyboard::create();
    listenerKey->onKeyReleased = CC_CALLBACK_2(BaseGameScene::onKeyReleaseNew, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerKey, this);
    
    //
    RankingManager::getInstance()->sceneChange();
    AttackEffectManager::getInstance()->stopAllEffect();
    AttackEffectManager::getInstance()->reactivationEffect();
    AdsUtilsManager::getInstance()->adsCooldownHide();
    
    EventManager::getInstance()->_onEventIconShow.Clear();
    return true;
}

void BaseGameScene::onEnter()
{
    BaseScene::onEnter();
    
    // event touch
    auto scene = UtilsScene::getInstance()->getRunningScene();

    auto layerTouch = Layer::create();
    layerTouch->setContentSize(getContentSize());
    scene->removeChildByTag(TAG_SCENE_TOUCH);
    scene->addChild(layerTouch, ZORDER_SCENE_TOUCH, TAG_SCENE_TOUCH);
    
    auto listenerTouch = cocos2d::EventListenerTouchOneByOne::create();
    listenerTouch->setSwallowTouches(true);
    listenerTouch->onTouchBegan = [=](cocos2d::Touch *touch, cocos2d::Event *event)->bool {
        
        _nTouchTime = 0;
        
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerTouch, layerTouch);
    
    //
    this->schedule(schedule_selector(BaseGameScene::scheduleTouch));
}

void BaseGameScene::onKeyReleaseNew(EventKeyboard::KeyCode keyCode, Event* event)
{
    onKeyRelease(keyCode,event);
}

bool BaseGameScene::onKeyRelease(EventKeyboard::KeyCode keyCode, Event* event)
{
    if ( keyCode != EventKeyboard::KeyCode::KEY_BACK )
    {
        return false;
    }
    
    // 로딩 화면 체크
    if ( PopupLoading::isShow() == true )
    {
        return false;
    }
    
    // 로딩 락 화면 체크
    if ( PopupLoadingLock::isShow() == true )
    {
        return false;
    }
    
    // 락 화면 체크
    if ( PopupScreenLock::isShow() == true )
    {
        return false;
    }
    
    // 팝업 레이어 체크
    auto scene = UtilsScene::getInstance()->getRunningScene();
    if ( scene->getChildByTag(TAG_SCENE_POPUP) )
    {
        auto sceneChild = scene->getChildren();
        for ( ssize_t i = sceneChild.size() - 1; i >= 0; i--)
        {
            auto child = dynamic_cast<PopupBase *>(sceneChild.at(i));
            if ( child != nullptr && child->isHideBackKey() )
            {
                child->backkey();
                return false;
            }
            else if(child != nullptr && child->isHideBackKey() == false)
            {
                return false;
            }
        }
    }
    
    return true;
}

bool BaseGameScene::isPopupShow()
{
    if ( PopupLoading::isShow() == true )
    {
        return true;
    }
    
    // 로딩 락 화면 체크
    if ( PopupLoadingLock::isShow() == true )
    {
        return true;
    }
    
    auto scene = UtilsScene::getInstance()->getRunningScene();
    if ( scene->getChildByTag(TAG_SCENE_POPUP) )
    {
        auto sceneChild = scene->getChildren();
        for ( ssize_t i = sceneChild.size() - 1; i >= 0; i--)
        {
            auto child = dynamic_cast<PopupBase *>(sceneChild.at(i));
            if ( child != nullptr )
            {
                return true;
            }
        }
    }
    
    return false;
}

void BaseGameScene::hidePopupAll()
{
    auto scene = UtilsScene::getInstance()->getRunningScene();
    if ( scene->getChildByTag(TAG_SCENE_POPUP) == nullptr )
    {
        return;
    }
    
    auto sceneChild = scene->getChildren();
    for ( ssize_t i = sceneChild.size() - 1; i >= 0; i--)
    {
        auto child = dynamic_cast<PopupBase *>(sceneChild.at(i));
        if ( child != nullptr && child->isHideAll() == true )
        {
            // hide
            child->hide();
            
            // erase
            sceneChild.erase(sceneChild.begin() + i);
        }
    }
}

void BaseGameScene::scheduleTouch(float dt)
{
    bool bBattery = UserInfoConfigManager::getInstance()->isFunction(E_FUNCTION_ACTIVE::BATTERY_AUTO);
    if ( bBattery == false )
    {
        return;
    }
    
    if ( PopupScreenLock::isShow() == true )
    {
        return;
    }
    
    auto scene = UtilsScene::getInstance()->getRunningScene();
    if ( scene != nullptr && scene->getChildByName("Popup_Notic") != nullptr )
    {
        return;
    }
    
    if ( _nTouchTime >= 300 )
    {
        _nTouchTime = 0;
        
        // show
        PopupScreenLock::show();
    }
    else
    {
        _nTouchTime += dt;
    }
}
