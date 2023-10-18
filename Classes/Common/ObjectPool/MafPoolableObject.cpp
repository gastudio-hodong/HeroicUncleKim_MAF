//
//  MafPoolableObject.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/07.
//

#include "MafPoolableObject.h"

#include "Common/ObjectPool/PoolableObjectComponent.h"
#include "GameObject/Grid/InfoGridTileData.h"
#include "GameObject/Grid/Grid.h"
#include "GameObject/Defense/InfoDefenseMap.h"
#include "ManagerGame/DefenseManager.h"
USING_NS_CC;

MafPoolableObject* MafPoolableObject::create()
{
    MafPoolableObject* pRet = new(std::nothrow) MafPoolableObject();
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

MafPoolableObject::MafPoolableObject() :
_poolComp(nullptr)
{
    _listCollisionColider.clear();
}

MafPoolableObject::~MafPoolableObject()
{
    _listCollisionColider.clear();
}

bool MafPoolableObject::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    _poolComp = PoolableObjectComponent::create();
    _poolComp->SetOwnerType(E_Poolable_Object_Type::DF_MONSTER);
    _poolComp->_onPooledObjectActivate += new MafFunction<void()>(CC_CALLBACK_0(MafPoolableObject::activateObject, this), this);
    addComponent(_poolComp);
    
    setIgnoreAnchorPointForPosition(false);
    unscheduleUpdate();
    
    _listCollisionColider.clear();
    return true;
}

void MafPoolableObject::update(float)
{
    CleanupColiderList();
}

void MafPoolableObject::activateObject()
{
    scheduleUpdate();
    _listCollisionColider.clear();
}
void MafPoolableObject::deactivateObject()
{
    unscheduleUpdate();
    stopAllActions();
    
    _poolComp->DeactivateObject();
    _listCollisionColider.clear();
}

#pragma mark -override
Rect MafPoolableObject::GetBoundingBox()
{
    return Rect();
}

bool MafPoolableObject::IsActivate()
{
    return _poolComp->IsActive();
}
