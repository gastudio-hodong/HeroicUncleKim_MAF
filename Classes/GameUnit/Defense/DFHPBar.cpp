//
//  DFHPBar.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/17.
//

#include "DFHPBar.h"

#include "Common/ObjectPool/PoolableObjectComponent.h"
#include "GameUnit/Defense/DFBattleManager.h"

#include "GameUnit/Defense/DFMonster.h"

#include "GameObject/Grid/InfoGridTileData.h"
#include "GameObject/Grid/Grid.h"
#include "GameObject/Defense/InfoDefenseMap.h"
#include "GameObject/Defense/InfoDefenseBullet.h"
#include "GameObject/Defense/InfoDefenseMonster.h"
#include "GameObject/Defense/InfoDefenseFloor.h"

#include "ManagerGame/DefenseManager.h"
USING_NS_CC;

DFHPBar* DFHPBar::create()
{
    DFHPBar* pRet = new(std::nothrow) DFHPBar();
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

DFHPBar::DFHPBar() :
_ownerMonster(nullptr),
_hpBG(nullptr),
_icon(nullptr),
_hpBar(nullptr)
{
}

DFHPBar::~DFHPBar()
{
}

bool DFHPBar::init()
{
    if ( !DFObject::init() )
    {
        return false;
    }
    
    return true;
}
void DFHPBar::activateObject()
{
    DFObject::activateObject();
    updateHpBar();
}
void DFHPBar::deactivateObject()
{
    DFObject::deactivateObject();
}
#pragma mark -get set
void DFHPBar::setOwnerMonster(DFMonster* owner)
{
    _ownerMonster = owner;
}
#pragma mark -update
void DFHPBar::updateHpBar()
{
    if(_hpBG == nullptr)
    {
        _hpBG = Sprite::create("Assets/ui/defense/Ui_hp_gauge_bg.png");
        _hpBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        addChild(_hpBG, 100);
        
        setContentSize(_hpBG->getContentSize());
        _hpBG->setPosition(getContentSize()/2);
    }
    if(_hpBar == nullptr)
    {
        _hpBar = ProgressTimer::create(Sprite::create("Assets/ui/defense/Ui_hp_gauge_red.png"));
        _hpBar->setType(ProgressTimer::Type::BAR);
        _hpBar->setPercentage(100);
        _hpBar->setMidpoint(Vec2::ANCHOR_MIDDLE_LEFT);
        _hpBar->setBarChangeRate(Vec2(1,0));
        _hpBar->setPosition(_hpBG->getContentSize().width / 2, _hpBG->getContentSize().height / 2);
        _hpBG->addChild(_hpBar);
    }
    if(_icon == nullptr)
    {
        _icon = Sprite::create("Assets/icon/icon_defense_mon.png");
        _icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        _icon->setPosition(Vec2(15, 0));
        _hpBG->addChild(_icon, 2);
    }
    
    
    if(_ownerMonster == nullptr)
        return;
    switch(_ownerMonster->getMonsterType())
    {
        case E_DF_MonsterType::BOSS:
        {
            _icon->setVisible(true);
        }break;
        default:
        {
            _icon->setVisible(false);
        }break;
    }
    updatePercentage();
}
void DFHPBar::updatePercentage()
{
    _hpBar->setPercentage(_ownerMonster->getCurrentHP()/_ownerMonster->getMaxHP() * 100);
}
#pragma mark -utill
void DFHPBar::movement(float dt)
{
    if(_ownerMonster == nullptr)
        return;
    
    Vec2 location = _ownerMonster->getPosition();
    if(auto monster = _ownerMonster->getMonsterSprite())
    {
        location += Vec2::UP * (monster->getContentSize().height + 50);
    }
    setPosition(location);
}
