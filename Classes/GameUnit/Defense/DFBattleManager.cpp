//
//  DFBattleManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/10.
//

#include "DFBattleManager.h"

#include "Common/ObjectPool/PoolableObjectComponent.h"
#include "Common/ObjectPool/MafPool.h"

#include "GameObject/Grid/Grid.h"
#include "GameObject/Defense/InfoDefenseFloor.h"

#include "GameUnit/Defense/DFObject.h"
#include "GameUnit/Defense/DFMonster.h"
#include "GameUnit/Defense/DFBullet.h"
#include "GameUnit/Defense/DFTrench.h"
#include "GameUnit/Defense/DFEffect.h"
#include "GameUnit/Defense/DFExplosion.h"
#include "GameUnit/Defense/DFHPBar.h"
#include "GameUnit/Defense/DFMonsterManager.h"

#include "ManagerGame/DefenseManager.h"

USING_NS_CC;

DFBattleManager* DFBattleManager::create()
{
    DFBattleManager* pRet = new(std::nothrow) DFBattleManager();
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

DFBattleManager::DFBattleManager(void):
_monsterManager(nullptr),
_grid(nullptr),
_bulletPool(nullptr),
_effectPool(nullptr),
_explosionPool(nullptr),
_HPBarPool(nullptr)
{
    _activateBullet.clear();
    _activateBullet.clear();
    _onKillMonster.Clear();
}

DFBattleManager::~DFBattleManager(void)
{
    _activateBullet.clear();
    _activateBullet.clear();
    _onKillMonster.Clear();
}

bool DFBattleManager::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    DefenseManager::getInstance()->_onUpdateFloor += new MafFunction<void(void)>(CC_CALLBACK_0(DFBattleManager::onUpdateFloor, this), this);
    DefenseManager::getInstance()->_onUpdateFail += new MafFunction<void(void)>(CC_CALLBACK_0(DFBattleManager::onUpdateFail, this), this);
    
    // init
    generateGrid();
    scheduleUpdate();
    
    return true;
}
void DFBattleManager::update(float dt)
{
    checkCollision();
}
#pragma mark - init


#pragma mark - Grid
void DFBattleManager::generateGrid()
{
    _grid = Grid::create();
    _grid->_onGridGenerated_ += new MafFunction<void()>(CC_CALLBACK_0(DFBattleManager::onGenerateGrid, this), this);
    _grid->spawnGrid(Vec2(334, 555), Vec2(84, 84), IntPoint(9, 9));
    addChild(_grid);
}

#pragma mark - Click

#pragma mark - Event
void DFBattleManager::onGenerateGrid()
{
    _monsterManager = DFMonsterManager::create();
    _monsterManager->setBattleManager(this);
    _monsterManager->activateManager();
    addChild(_monsterManager, 9);
    
    _bulletPool = MafPool::create(E_Poolable_Object_Type::DF_BULLET, 200);
    addChild(_bulletPool, 10);
    
    _explosionPool = MafPool::create(E_Poolable_Object_Type::DF_EXPLOSION, 30);
    addChild(_explosionPool, 10);
    
    _effectPool = MafPool::create(E_Poolable_Object_Type::DF_EFFECT, 100);
    addChild(_effectPool, 11);
    
    _HPBarPool = MafPool::create(E_Poolable_Object_Type::DF_HPBAR, 100);
    addChild(_HPBarPool, 15);
    
    Vec2 location = Vec2(-10, 930);
    for(int i = 0; i < 6; ++i)
    {
        auto trench = DFTrench::create();
        trench->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        trench->setPosition(location);
        trench->setNum(i+1);
        trench->setGrid(_grid);
        trench->setBattleManager(this);
        trench->activateObject();
        addChild(trench);
        
        location += Vec2::DOWN * (trench->getContentSize().height/2 + 10);
    }
    
    startWave();
}

#pragma mark - utills
void DFBattleManager::checkCollision()
{
    auto activateMonsters = _monsterManager->getActivateMonsters();
    for(auto bullet : _activateBullet)
    {
        auto bulletRoundingBox = bullet->GetBoundingBox();
        for(auto monster :activateMonsters)
        {
            if(monster->isDead())
                continue;
            
            auto monsterRoundingBox = monster->GetBoundingBox();
            if(monsterRoundingBox.intersectsRect(bulletRoundingBox))
            {
                bullet->Collision(monster);
                monster->Collision(bullet);
            }
            if(!bullet->IsActivate())
                break;
        }
    }
    
    for(auto explosion : _activateExplosition)
    {
        auto explosionRoundingBox = explosion->GetBoundingBox();
        for(auto monster : activateMonsters)
        {
            if(monster->isDead())
                continue;
            
            auto monsterRoundingBox = monster->GetBoundingBox();
            if(explosionRoundingBox.intersectsRect(monsterRoundingBox))
            {
                explosion->Collision(monster);
                monster->Collision(explosion);
            }
            if(!explosion->IsActivate())
                break;
        }
    }
}

void DFBattleManager::addActivateBullet(DFBullet* bullet)
{
    if(_activateBullet.find(bullet) == _activateBullet.end())
        _activateBullet.pushBack(bullet);
}

void DFBattleManager::removeActivateBullet(DFObject* bullet)
{
    if(_activateBullet.find(dynamic_cast<DFBullet*>(bullet)) != _activateBullet.end())
        _activateBullet.eraseObject(dynamic_cast<DFBullet*>(bullet));
}

void DFBattleManager::addActivateExplosion(DFExplosion* explosion)
{
    if(_activateExplosition.find(explosion) == _activateExplosition.end())
        _activateExplosition.pushBack(explosion);
}

void DFBattleManager::removeActivateExplosion(DFObject* explosion)
{
    if(_activateExplosition.find(dynamic_cast<DFExplosion*>(explosion)) != _activateExplosition.end())
        _activateExplosition.eraseObject(dynamic_cast<DFExplosion*>(explosion));
}

Grid* DFBattleManager::getGrid()
{
    return _grid;
}

DFEffect* DFBattleManager::spawnEffectObject()
{
    if(_effectPool == nullptr)
        return nullptr;
    
    return _effectPool->spawnPooledObject<DFEffect*>();
}
DFBullet* DFBattleManager::spawnBullet()
{
    if(_bulletPool == nullptr)
        return nullptr;
    
    return _bulletPool->spawnPooledObject<DFBullet*>();
}
DFExplosion* DFBattleManager::spawnExplosion()
{
    if(_explosionPool == nullptr)
        return nullptr;
    
    return _explosionPool->spawnPooledObject<DFExplosion*>();
}

DFMonster* DFBattleManager::getMonsterByTargetType(E_DF_TargetType type)
{
    DFMonster* result = nullptr;
    
    if(_monsterManager != nullptr)
        result = _monsterManager->getMonsterByTargetType(type);
    
    return result;
}

DFHPBar* DFBattleManager::getHPBar()
{
    if(_HPBarPool == nullptr)
        return nullptr;
    
    return _HPBarPool->spawnPooledObject<DFHPBar*>();
}

int DFBattleManager::getActivateMonsterCount()
{
    return _monsterManager->getActivateMonsterCount();
}

int DFBattleManager::getCurrentStageMonsterLeftCount() const
{
    return _currentFloorMonsterCount - _killCount;
}

void DFBattleManager::startWave()
{
    _killCount = 0;
    if(auto data = DefenseManager::getInstance()->getCurrentFloorData())
        _currentFloorMonsterCount = data->getMonsterValue();
    if(DefenseManager::getInstance()->getCurrentFloor() % 100 == 0)
        _currentFloorMonsterCount++;
    _monsterManager->startWave();
}

void DFBattleManager::onUpdateFloor()
{
    _killCount = 0;
    stopAllActions();
    float waveStartDelayTime = 4.5f;
    if(DefenseManager::getInstance()->isBackground())
        waveStartDelayTime *= 2;
    auto seq = Sequence::create(DelayTime::create(waveStartDelayTime), CallFunc::create(CC_CALLBACK_0(DFBattleManager::startWave, this)), nullptr);
    runAction(seq);
}

void DFBattleManager::onUpdateFail()
{
    _monsterManager->failWave();
}

void DFBattleManager::addMonsterKillCount()
{
    _killCount++;
    if(_killCount >= _currentFloorMonsterCount)
        DefenseManager::getInstance()->clearFloor();
    
    _onKillMonster.Invoke();
}
