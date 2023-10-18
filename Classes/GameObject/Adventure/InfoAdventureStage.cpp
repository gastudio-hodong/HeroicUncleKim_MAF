//
//  InfoAdventureStage.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/25.
//

#include "InfoAdventureStage.h"

#include "GameObject/InfoItems.h"

#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

#pragma mark -
InfoAdventureStage::InfoAdventureStage():
_isRunning(false)
,_isComplete(false)
,_nIdx(0)
,_nArea(0)
,_nRequireLv(0)
,_nTime(0)
,_nEndTime(0)
,_nExp(0)
,_nPartyIdx(0)
{
    
}

InfoAdventureStage::~InfoAdventureStage()
{
    
}

bool InfoAdventureStage::init()
{
    return true;
}

const int InfoAdventureStage::getArea() const
{
    return _nArea;
}
void InfoAdventureStage::setArea(const int value)
{
    _nArea = value;
}

const int InfoAdventureStage::getIdx() const
{
    return _nIdx;
}
void InfoAdventureStage::setIdx(const int value)
{
    _nIdx = value;
}

const int InfoAdventureStage::getRequireLv() const
{
    return _nRequireLv;
}
void InfoAdventureStage::setRequireLv(const int value)
{
    _nRequireLv = value;
}

const int InfoAdventureStage::getTime() const
{
    return _nTime;
}
void InfoAdventureStage::setTime(const int value)
{
    _nTime = value;
}

const int InfoAdventureStage::getEndTime() const
{
    return _nEndTime;
}
void InfoAdventureStage::setEndTime(const int value)
{
    _nEndTime = value;
}

const int InfoAdventureStage::getExp() const
{
    return _nExp;
}
void InfoAdventureStage::setExp(const int value)
{
    _nExp = value;
}
const int InfoAdventureStage::getPartyIdx() const
{
    return _nPartyIdx;
}
void InfoAdventureStage::setPartyIdx(const int value)
{
    _nPartyIdx = value;
}
const bool InfoAdventureStage::isRunning() const
{
    return _isRunning;
}
void InfoAdventureStage::setIsRunning(const bool value)
{
    _isRunning = value;
}

const bool InfoAdventureStage::isComplete() const
{
    return _isComplete;
}
void InfoAdventureStage::setIsComplete(const bool value)
{
    _isComplete = value;
}

cocos2d::Vector<InfoItems*> InfoAdventureStage::getListReward()
{
    return ItemsManager::getInstance()->getConvertArray(_reward);
}

void InfoAdventureStage::setReward(std::string reward)
{
    _reward = reward;
}

cocos2d::Vector<InfoItems*> InfoAdventureStage::getListRewardBossPoint()
{
    return ItemsManager::getInstance()->getConvertArray(_rewardBossPoint);
}

void InfoAdventureStage::setRewardBossPoint(std::string reward)
{
    _rewardBossPoint = reward;
}

const bool InfoAdventureStage::isOpen() const
{
    bool result = false;
    
    int nCurrentLv = AdventureManager::getInstance()->getFurniturePropertyEffect(E_ADVENTURE_PROPERTY_TYPE::AREA_STAGE_OPEN);
    
    if(nCurrentLv >= _nRequireLv)
        result = true;
    
    return result;
}
