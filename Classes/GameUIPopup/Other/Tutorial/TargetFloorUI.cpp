//
//  TargetFloorUI.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/07/07.
//

#include "TargetFloorUI.h"

#include "ManagerGame/UserInfoManager.h"

USING_NS_CC;

TargetFloorUI* TargetFloorUI::create(int floor)
{
    TargetFloorUI* pRet = new(std::nothrow) TargetFloorUI(floor);
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

TargetFloorUI::TargetFloorUI(int floor):
_sprBg(nullptr),
_label(nullptr),
_goalFloor(floor),
_fTimer(0)
{
}

TargetFloorUI::~TargetFloorUI()
{
}

bool TargetFloorUI::init()
{
    setContentSize(Size(296, 54));
    _sprBg = Sprite::create("Assets/ui/mission/mission_rise1_1.png");
    _sprBg->setPosition(0, 0);
    addChild(_sprBg);
    
    _sprEffect = Sprite::create("Assets/ui/mission/mission_rise1_2.png");
    _sprEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _sprEffect->setPosition(0, 0);
    addChild(_sprEffect);
    
    auto fideOut = FadeOut::create(0.5f);
    auto fideIn = FadeIn::create(0.5f);
    Vector<FiniteTimeAction*> actions;
    actions.pushBack(fideOut);
    actions.pushBack(fideIn);
    auto sequence = Sequence::create(actions);
    auto rep = RepeatForever::create(sequence);
    _sprEffect->runAction(rep);
    
    int uFloor = UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL);
    std::string str = GAME_TEXTFORMAT("t_mission_target_floor", _goalFloor - uFloor);
    _label = Label::createWithTTF(str, GAME_FONT, 25);
    
    _label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _label->setPosition(75, getContentSize().height/2);
    _label->setOverflow(Label::Overflow::SHRINK);
    _label->setDimensions(getContentSize().width - getContentSize().width/3, getContentSize().height);
    _label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    _label->setName("text");
    _sprBg->addChild(_label);
    
    this->scheduleUpdate();
    
    return true;
}

void TargetFloorUI::update(float dt)
{
    _fTimer += dt;
    
    int uFloor = UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL);
    
    if(_fTimer >= 2.f)
    {
        _fTimer = 0;
        
        std::string str = GAME_TEXTFORMAT("t_mission_target_floor", _goalFloor - uFloor);
        _label->setString(str);
    }
    
    if(uFloor >= _goalFloor)
    {
        hide();
    }
    
}

void TargetFloorUI::hide()
{
    if ( getParent() != nullptr )
    {
        removeFromParent();
    }
}
