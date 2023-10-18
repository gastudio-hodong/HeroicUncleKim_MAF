//
//  PoolManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/07.
//

#include "MafPool.h"
#include "Common/ObjectPool/PoolableObjectComponent.h"
#include "GameUnit/Defense/DFMonster.h"
#include "GameUnit/Defense/DFBullet.h"
#include "GameUnit/Defense/DFEffect.h"
#include "GameUnit/Defense/DFExplosion.h"
#include "GameUnit/Defense/DFHPBar.h"

MafPool* MafPool::create(E_Poolable_Object_Type type, int poolSize)
{
    MafPool* pRet = new(std::nothrow) MafPool();
    if ( pRet && pRet->init(type, poolSize) )
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

// Sets default values
MafPool::MafPool()
{
    _poolingIngameObject.clear();
    _type = E_Poolable_Object_Type::NONE;
    _defaultPoolSize = 100;
}
MafPool::~MafPool()
{
    _poolingIngameObject.clear();
}

// Called when the game starts or when spawned
bool MafPool::init(E_Poolable_Object_Type type, int poolSize)
{
    
    if(!Node::init())
        return false;
    
    
    _type = type;
    _defaultPoolSize = poolSize;
    
    return true;
}

void MafPool::onEnter()
{
    Node::onEnter();
}

void MafPool::onPooledObjectDespawn(Node* poolActor)
{
    if(_poolingIngameObject.size() <= 0)
    {
        createObjectPool(_defaultPoolSize);
    }
    
    _poolingIngameObject.pushBack(poolActor);
}

void MafPool::createObjectPool(int poolSize)
{
    for(int i = 0; i < poolSize; ++i)
    {
        auto object = createPoolableObjectByType();
        if(auto poolComp = (PoolableObjectComponent*)object->getComponent("Poolable"))
        {
            poolComp->SetOwnerType(_type);
            poolComp->SetActiveObject(false);
            poolComp->_onPooledObjectDespawn += new MafFunction<void(Node*)>(CC_CALLBACK_1(MafPool::onPooledObjectDespawn, this), this);
            addChild(object);
            _poolingIngameObject.pushBack(object);
        }
    }
}

Node* MafPool::createPoolableObjectByType()
{
    Node* result = nullptr;
    
    switch(_type)
    {
        case E_Poolable_Object_Type::DF_BULLET :
        {
            result = DFBullet::create();
        }break;
        case E_Poolable_Object_Type::DF_MONSTER :
        {
            result = DFMonster::create();
        }break;
        case E_Poolable_Object_Type::DF_EXPLOSION:
        {
            result = DFExplosion::create();
        }break;
        case E_Poolable_Object_Type::DF_EFFECT :
        {
            result = DFEffect::create();
        }break;
        case E_Poolable_Object_Type::DF_HPBAR :
        {
            result = DFHPBar::create();
        }break;
        default: break;
    }
    
    return result;
}
