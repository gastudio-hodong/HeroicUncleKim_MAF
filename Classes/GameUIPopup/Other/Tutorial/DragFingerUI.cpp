//
//  DragFingerUI.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/07/28.
//

#include "DragFingerUI.h"

#include "GameScene/HelloWorldScene.h"

#include "ManagerGame/UserInfoManager.h"

USING_NS_CC;

DragFingerUI* DragFingerUI::create(E_QUEST quest, int level)
{
    DragFingerUI* pRet = new(std::nothrow) DragFingerUI(quest, level);
    if ( pRet && pRet->init() )
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

DragFingerUI::DragFingerUI(E_QUEST quest, int level):
_sprFinger(nullptr),
_quest(quest),
_level(level),
_fTimer(0),
_maxTimer(0)
{
}

DragFingerUI::~DragFingerUI()
{
}

bool DragFingerUI::init()
{
    setName("DragFingerUI");
    
    _sprFinger = Sprite::create("Assets/ui/tutorial/guide_finger3_1.png");
    _sprFinger->setPosition(0, 0);
    addChild(_sprFinger);
    setContentSize(_sprFinger->getContentSize());
    
    auto hitMove = MoveBy::create(0.5f, Vec2(0,50));
    auto moveSeq = Sequence::create(hitMove, hitMove->reverse(), nullptr);
    
    auto rep = RepeatForever::create(moveSeq);
    
    _sprFinger->runAction(rep);
    
    
    this->scheduleUpdate();
    
    return true;
}

void DragFingerUI::update(float dt)
{
    _fTimer += dt;
    _maxTimer += dt;
    if(_fTimer >= 1.f)
    {
        _fTimer = 0;
        if(UserInfoManager::getInstance()->getQuestLevel(E_PLACE::DG_NORMAL, _quest) >= _level)
        {
            hide();
            return;
        }
    }
    if(_maxTimer >= 10)
    {
        hide();
        return;
    }
    
    auto layer = UtilsScene::getInstance()->getRunningLayer();
    auto scene = dynamic_cast<HelloWorldScene *>(layer);
    if ( scene != nullptr )
    {
        if(scene->_eGameState != HelloWorldScene::E_GAME_STATE::QUEST)
        {
            _sprFinger->setVisible(false);
            return;
        }
        else
        {
            _sprFinger->setVisible(true);
            return;
        }
    }
    
}

void DragFingerUI::hide()
{
    if ( getParent() != nullptr )
    {
        removeFromParent();
    }
}
