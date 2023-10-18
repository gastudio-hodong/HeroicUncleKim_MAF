//
//  BaseScene.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 04/06/2019.
//

#include "BaseScene.h"

#include "GameScene/Intro/RootScene.h"
#include "GameScene/Intro/SplashScene.h"
#include "GameScene/Intro/IntroScene.h"

USING_NS_CC;

BaseScene::BaseScene()
{
    
}

BaseScene::~BaseScene()
{
}

bool BaseScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void BaseScene::onEnter()
{
    Layer::onEnter();
}
 
void BaseScene::onExit()
{
    Layer::onExit();
}
