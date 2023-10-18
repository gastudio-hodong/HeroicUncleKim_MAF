//
//  DFTrench.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/07.
//

#include "DFTrench.h"

#include "GameUnit/Defense/DFCharacter.h"
#include "GameUnit/Defense/DFWeapon.h"

#include "GameObject/Grid/InfoGridTileData.h"
#include "GameObject/Grid/Grid.h"
#include "GameObject/Defense/InfoDefenseMap.h"
#include "ManagerGame/DefenseManager.h"
USING_NS_CC;

DFTrench* DFTrench::create()
{
    DFTrench* pRet = new(std::nothrow) DFTrench();
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

DFTrench::DFTrench() :
_num(0),
_grid(nullptr),
_trench(nullptr),
_trenchFront(nullptr),
_trenchSandBag(nullptr),
_character(nullptr),
_battleManager(nullptr),
_weapon(nullptr)
{
    
}

DFTrench::~DFTrench()
{
    
}

bool DFTrench::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    setIgnoreAnchorPointForPosition(false);
    setContentSize(Size(177, 177));
    updateTrench();
    
    return true;
}
void DFTrench::update(float dt)
{
}
void DFTrench::activateObject()
{
    updateCharacter();
    updateWeapon();
}
#pragma mark -get set
void DFTrench::setGrid(Grid* grid)
{
    _grid = grid;
}
void DFTrench::setBattleManager(DFBattleManager* battleManager)
{
    _battleManager = battleManager;
}
void DFTrench::setNum(int num)
{
    _num = num;
}
#pragma mark -create object
void DFTrench::updateCharacter()
{
    if(_character == nullptr)
    {
        _character = DFCharacter::create();
        _character->setPosition(getContentSize()/2);
        _character->setNum(_num);
        addChild(_character, 1);
    }
    _character->activateObject();
}

void DFTrench::updateWeapon()
{
    if(_weapon == nullptr)
    {
        _weapon = DFWeapon::create();
        _weapon->setPosition(getContentSize()/2);
        _weapon->setNum(_num);
        _weapon->setGrid(_grid);
        _weapon->setBattleManager(_battleManager);
        addChild(_weapon, 4);
    }
    _weapon->activateObject();
}
#pragma mark -update
void DFTrench::updateTrench()
{
    if(_trench == nullptr)
    {
        _trench = Sprite::create();
        _trench->setPosition(getContentSize()/2);
        addChild(_trench, 0);
    }
    if(_trenchFront == nullptr)
    {
        _trenchFront = Sprite::create();
        _trenchFront->setPosition(getContentSize()/2);
        addChild(_trenchFront, 2);
    }
    if(_trenchSandBag == nullptr)
    {
        _trenchSandBag = Sprite::create();
        _trenchSandBag->setPosition(getContentSize()/2);
        addChild(_trenchSandBag, 3);
    }
    
    _trench->setTexture("Assets/ui/defense/Defense_trenches_01_lv1.png");
    _trenchFront->setTexture("Assets/ui/defense/Defense_trenches_02_lv1.png");
    _trenchSandBag->setTexture("Assets/ui/defense/Defense_trenches_03_lv1.png");
}

