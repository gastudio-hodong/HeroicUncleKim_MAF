//
//  DFMonsterManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/20.
//

#include "DFMonsterManager.h"

#include "Common/ObjectPool/PoolableObjectComponent.h"
#include "Common/ObjectPool/MafPool.h"

#include "GameObject/Grid/Grid.h"
#include "GameObject/Defense/InfoDefenseFloor.h"

#include "GameUnit/Defense/DFObject.h"
#include "GameUnit/Defense/DFMonster.h"

#include "ManagerGame/DefenseManager.h"

USING_NS_CC;

static bool isMonsterFrontForZOrder(DFMonster* a, DFMonster* b)
{
    bool result = false;
    
    if((int)a->getPositionX() < (int)b->getPositionX())
        result = true;
    else
        result = (int)a->getPositionY() < (int)b->getPositionY();
    
    return result;
}

static bool isMonsterFront(DFMonster* a, DFMonster* b)
{
    return a->getPathLeftSize() < b->getPathLeftSize();
}

DFMonsterManager* DFMonsterManager::create()
{
    DFMonsterManager* pRet = new(std::nothrow) DFMonsterManager();
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

DFMonsterManager::DFMonsterManager(void):
_grid(nullptr),
_monsterPool(nullptr)
{
    _spwanList.clear();
}

DFMonsterManager::~DFMonsterManager(void)
{
    _spwanList.clear();
}

bool DFMonsterManager::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    return true;
}

void DFMonsterManager::activateManager()
{
    _monsterPool = MafPool::create(E_Poolable_Object_Type::DF_MONSTER);
    addChild(_monsterPool);
    
    schedule(schedule_selector(DFMonsterManager::updateMonsterZOrder), 0.5f, kRepeatForever, 1.f);
}

void DFMonsterManager::deactivateObject()
{
    unscheduleAllCallbacks();
}

#pragma mark - Event

#pragma mark - utills

void DFMonsterManager::addActivateMonster(DFMonster* monster)
{
    if(_activateMonster.find(monster) == _activateMonster.end())
        _activateMonster.pushBack(monster);
}

void DFMonsterManager::removeActivateMonster(DFObject* monster)
{
    if(_activateMonster.find(dynamic_cast<DFMonster*>(monster)) != _activateMonster.end())
        _activateMonster.eraseObject(dynamic_cast<DFMonster*>(monster));
}

void DFMonsterManager::updateMonsterZOrder(float t)
{
    Vector<DFMonster*> tmpList = _activateMonster;
    std::sort(_activateMonster.begin(), _activateMonster.end(), isMonsterFront);
    std::sort(tmpList.begin(), tmpList.end(), isMonsterFrontForZOrder);
    
    for(int i = 0; i < tmpList.size(); ++i)
    {
        int num = (int)tmpList.size() - i;
        tmpList.at(i)->setLocalZOrder(num);
    }
}
void DFMonsterManager::createSpwanList(InfoDefenseFloor* data)
{
    _spwanList.clear();
    
    int count = data->getMonsterValue();
    int normalCount = count * data->getNormalPer() * 0.01;
    int runnerCount = count * data->getRunnerPer() * 0.01;
    int tankerCount = count * data->getTankerPer() * 0.01;
    normalCount += count - (normalCount + runnerCount + tankerCount);
    
    for(int i = 0; i < normalCount; ++i)
    {
        _spwanList.push_back(E_DF_MonsterType::NORMAL);
    }
    for(int i = 0; i < runnerCount; ++i)
    {
        _spwanList.push_back(E_DF_MonsterType::RUNNER);
    }
    for(int i = 0; i < tankerCount; ++i)
    {
        _spwanList.push_back(E_DF_MonsterType::TANKER);
    }
    
    if(DefenseManager::getInstance()->getCurrentFloor() % 100 == 0)
    {
        _spwanList.push_back(E_DF_MonsterType::BOSS);
    }
}

Vector<DFMonster*> DFMonsterManager::getActivateMonsters()
{
    return _activateMonster;
}
int DFMonsterManager::getActivateMonsterCount() const
{
    return _activateMonster.size();
}
void DFMonsterManager::setBattleManager(DFBattleManager* manager)
{
    _battleManager = manager;
}

DFMonster* DFMonsterManager::spawnMonster()
{
    DFMonster* result = nullptr;
    
    if(auto monster = _monsterPool->spawnPooledObject<DFMonster*>())
    {
        monster->setBattleManager(_battleManager);
        monster->setMonsterType(getSpawnType());
        if(auto poolComp = (PoolableObjectComponent*)monster->getComponent("Poolable"))
        {
            poolComp->ActivateObject();
            monster->_onRemoveActiveList += new MafFunction<void(DFObject*)>(CC_CALLBACK_1(DFMonsterManager::removeActivateMonster, this), this);
            addActivateMonster(monster);
            result = monster;
        }
    }
    
    return result;
}

E_DF_MonsterType DFMonsterManager::getSpawnType()
{
    E_DF_MonsterType result = E_DF_MonsterType::NONE;
    
    if(DefenseManager::getInstance()->getCurrentFloor() % 100 == 0)
    {
        if(_spwanList.size() > 1)
        {
            int rand = random(0, (int)_spwanList.size()-2);
            result = _spwanList[rand];
            _spwanList.erase(_spwanList.begin() + rand);
        }
        else if(_spwanList.size() == 1)
        {
            result = E_DF_MonsterType::BOSS;
            _spwanList.clear();
        }
    }
    else
    {
        if(_spwanList.size() > 0)
        {
            int rand = random(0, (int)_spwanList.size()-1);
            result = _spwanList[rand];
            _spwanList.erase(_spwanList.begin() + rand);
        }
    }
    
    
    return result;
}
DFMonster* DFMonsterManager::getMonsterByTargetType(E_DF_TargetType type)
{
    DFMonster* result = nullptr;
    
    if(_activateMonster.size() > 0)
    {
        if(type == E_DF_TargetType::HEAD)
            result = _activateMonster.at(0);
        else if(type == E_DF_TargetType::TAIL)
            result = _activateMonster.at(_activateMonster.size()-1);
    }
    
    return result;
}

void DFMonsterManager::startWave()
{
    stopAllActions();
    
    auto floorData = DefenseManager::getInstance()->getCurrentFloorData();
    createSpwanList(floorData);
    
    
    auto action1 = CallFunc::create([=](){
        
        if(_spwanList.size() <= 0)
        {
            stopAllActions();
            return;
        }
        spawnMonster();
    });
    
    auto seq = Sequence::create(action1, DelayTime::create(10.f / (float)floorData->get10SecMonsterSpawnCount()), nullptr);
    runAction(RepeatForever::create(seq));
    
}

void DFMonsterManager::failWave()
{
    stopAllActions();
    _spwanList.clear();
    
    for(auto monster : _activateMonster)
    {
        monster->deadMonster();
    }
}
