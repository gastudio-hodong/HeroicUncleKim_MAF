//
//  DFObject.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/19.
//

#include "DFObject.h"

#include "GameUnit/Defense/DFBattleManager.h"
#include "GameObject/Grid/Grid.h"

#include "ManagerGame/DefenseManager.h"
USING_NS_CC;

DFObject* DFObject::create()
{
    DFObject* pRet = new(std::nothrow) DFObject();
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

DFObject::DFObject() :
_grid(nullptr),
_battleManager(nullptr)
{
    _onRemoveActiveList.Clear();
}

DFObject::~DFObject()
{
    _onRemoveActiveList.Clear();
}

bool DFObject::init()
{
    if ( !MafPoolableObject::init() )
    {
        return false;
    }
    setPosition(Vec2(9999,9999));
    return true;
}
void DFObject::update(float dt)
{
    MafPoolableObject::update(dt);
}
void DFObject::activateObject()
{
    MafPoolableObject::activateObject();
}
void DFObject::deactivateObject()
{
    _onRemoveActiveList.Invoke(this);
    _onRemoveActiveList.Clear();
    
    MafPoolableObject::deactivateObject();
}
#pragma mark -get set
void DFObject::setBattleManager(DFBattleManager* battleManager)
{
    _battleManager = battleManager;
    _grid = _battleManager->getGrid();
}
#pragma mark -update
