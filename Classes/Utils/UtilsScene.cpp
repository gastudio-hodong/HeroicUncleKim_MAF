//
//  UtilsScene.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 10/10/2019.
//

#include "UtilsScene.h"

#include "GameObject/InfoScene.h"

#include "GameScene/Intro/RootScene.h"
#include "GameScene/Intro/SplashScene.h"
#include "GameScene/Intro/IntroScene.h"
#include "GameScene/HelloWorldScene.h"
#include "GameScene/DevilDomScene.h"

#include "GameUI/UI/UITouchEffect.h"

#include "ManagerGame/UserInfoManager.h"

USING_NS_CC;

static UtilsScene *s_SharedUtilsScene = nullptr;

UtilsScene* UtilsScene::getInstance()
{
    if (!s_SharedUtilsScene)
    {
        s_SharedUtilsScene = new (std::nothrow) UtilsScene();
        s_SharedUtilsScene->init();
    }
    
    return s_SharedUtilsScene;
}

UtilsScene::UtilsScene(void)
{
    
}

UtilsScene::~UtilsScene(void)
{
    
}

bool UtilsScene::init(void)
{
    return true;
}

Scene* UtilsScene::getRunningScene()
{
    return Director::getInstance()->getRunningScene();
}

Layer* UtilsScene::getRunningLayer()
{
    return getRunningScene()->getChildByTag<Layer*>(10);
}

void UtilsScene::scenePop()
{
    Director::getInstance()->popScene();
}

void UtilsScene::scenePopRoot()
{
    Director::getInstance()->popToRootScene();
}

void UtilsScene::scenePush(UtilsScene::eType type, InfoScene* obj)
{
    // scene create
    Layer* layer = nullptr;
    switch (type) {
        case UtilsScene::scene_root:
        {
            layer = RootScene::create();
            break;
        }
        case UtilsScene::scene_splash:
        {
            layer = SplashScene::create();
            break;
        }
        default:
            break;
    }
    
    //
    layer->setTag(10);
    
    //
    auto scene = Scene::create();
    scene->addChild(layer);
    
    //
    Director::getInstance()->pushScene(scene);
}

cocos2d::Scene* UtilsScene::sceneReplace(UtilsScene::eType type, InfoScene* obj)
{
    //
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    
    //
    UserInfoManager::getInstance()->_onModifyFloor = nullptr;
    UserInfoManager::getInstance()->_onModifyEquip = nullptr;
    
    if ( type == eType::scene_dungeon )
    {
        if( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_DEVIL_DOM )
        {
            type = eType::scene_dungeon_devil;
        }
        else
        {
            type = eType::scene_dungeon_normal;
        }
    }
    
    // scene create
    Layer* layer = nullptr;
    switch (type) {
        case UtilsScene::scene_intro:
        {
            layer = IntroScene::create();
            break;
        }
        case UtilsScene::scene_dungeon_normal:
        {
            layer = HelloWorldScene::create();
            break;
        }
        case UtilsScene::scene_dungeon_devil:
        {
            layer = DevilDomScene::create();
            break;
        }
        default:
            break;
    }

    //
    layer->setTag(10);
    
    //
    auto scene = Scene::create();
    scene->setTag(type);
    scene->addChild(layer);
    
    // stop schedule
    if ( Scene* scenePrev = getRunningScene(); scenePrev != nullptr )
    {
        scenePrev->stopAllActions();
        scenePrev->unscheduleAllCallbacks();
    }
    
    if ( Layer* scenePrevLayer = getRunningLayer(); scenePrevLayer != nullptr )
    {
        scenePrevLayer->stopAllActions();
        scenePrevLayer->unscheduleAllCallbacks();
    }
    
    //
    Director::getInstance()->replaceScene(scene);
    
    
    // 유저들이 말들이 많아서 삭제
    // scene->addChild(UITouchEffect::create(), ZORDER_SCENE_TOUCH, TAG_SCENE_TOUCH);
    
    return scene;
}

