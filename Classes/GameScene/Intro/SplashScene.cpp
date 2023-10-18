//
//  SplashScene.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 31/05/2019.
//

#include "GameScene/Intro/SplashScene.h"

USING_NS_CC;

SplashScene::SplashScene()
{
    _listSplash.push_back("Assets/ui_intro/splash.png");
}

SplashScene::~SplashScene()
{
    
}

#pragma mark - override base
void SplashScene::onEnter()
{
    BaseScene::onEnter();
    
    //
    uiInit();
    uiDraw();
}

void SplashScene::uiInit()
{
    // color
    auto layer = LayerColor::create(Color4B::WHITE);
    addChild(layer);
}

#pragma mark - ui : draw
void SplashScene::uiDraw()
{
    if ( _listSplash.size() <= 0 )
    {
        UtilsScene::getInstance()->sceneReplace(UtilsScene::eType::scene_intro);
        return;
    }
    
    std::string strPath = _listSplash.at(0);
    
    auto iter = _listSplash.begin();
    _listSplash.erase(iter);
    
    
    // splash image
    auto spriteSplash = Sprite::create(strPath);
    spriteSplash->setPosition(getContentSize().width / 2, getContentSize().height / 2);
    spriteSplash->setOpacity(0);
    spriteSplash->setScale(MAX(getContentSize().width / spriteSplash->getContentSize().width, getContentSize().height / spriteSplash->getContentSize().height));
    addChild(spriteSplash);
    
    // splash action
    auto action1 = FadeIn::create(1.5);
    auto action2 = DelayTime::create(0.5);
    auto action3 = FadeOut::create(0.5);
    auto action4 = CallFunc::create(CC_CALLBACK_0(SplashScene::uiDraw, this));
    auto action5 = RemoveSelf::create();
    auto seq = Sequence::create(action1, action2, action3, action4, action5, nullptr);
    spriteSplash->runAction(seq);
}
