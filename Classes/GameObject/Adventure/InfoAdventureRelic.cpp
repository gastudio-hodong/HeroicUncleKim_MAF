//
//  InfoAdventureRelic.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/04.
//

#include "InfoAdventureRelic.h"

#include "GameObject/InfoItems.h"

#include "ManagerGame/AdventureRelicManager.h"
#include "ManagerGame/AdventureManager.h"

USING_NS_CC;

#pragma mark -
InfoAdventureRelic::InfoAdventureRelic():
_isActive(false)
,_nIdx(0)
,_nArea(0)
,_nGrade(0)
,_nCount(0)
,_nCurrentLv(0)
,_dAvlilityDefault(0)
,_dAvlilityIncrease(0)
,_eType(E_ADVENTURE_PROPERTY_TYPE::NONE)
{
    _listGrindReward.clear();
}

InfoAdventureRelic::~InfoAdventureRelic()
{
    _listGrindReward.clear();
}

bool InfoAdventureRelic::init()
{
    return true;
}

const bool InfoAdventureRelic::isActive() const
{
    return _isActive;
}
void InfoAdventureRelic::setIsActive(const bool value)
{
    _isActive = value;
}
const int InfoAdventureRelic::getIdx() const
{
    return _nIdx;
}
void InfoAdventureRelic::setIdx(const int value)
{
    _nIdx = value;
}
const int InfoAdventureRelic::getArea() const
{
    return _nArea;
}
void InfoAdventureRelic::setArea(const int value)
{
    _nArea = value;
}
const int InfoAdventureRelic::getGrade() const
{
    return _nGrade;
}
void InfoAdventureRelic::setGrade(const int value)
{
    _nGrade = value;
}
const int InfoAdventureRelic::getCount() const
{
    return _nCount;
}
void InfoAdventureRelic::setCount(const int value)
{
    _nCount = value;
}

const E_ADVENTURE_PROPERTY_TYPE InfoAdventureRelic::getType() const
{
    return _eType;
}
void InfoAdventureRelic::setType(const E_ADVENTURE_PROPERTY_TYPE value)
{
    _eType = value;
}

InfoAdventurePropertyType* const InfoAdventureRelic::getInfoType() const
{
    return AdventureManager::getInstance()->getPropertyType(_eType);
}

const int InfoAdventureRelic::getCurrentLv()
{
    return _nCurrentLv;
}
void InfoAdventureRelic::setCurrentLv(const int value)
{
    _nCurrentLv = value;
}

const double InfoAdventureRelic::getAbilityEffect(int nLv)
{
    double result = _dAvlilityDefault;
    
    if(nLv == -1)
        nLv = _nCurrentLv;
    
    result += _dAvlilityIncrease * nLv;
    
    return result;
}
void InfoAdventureRelic::setAbilityEffectDefault(const double value)
{
    _dAvlilityDefault = value;
}
void InfoAdventureRelic::setAbilityEffectIncrease(const double value)
{
    _dAvlilityIncrease = value;
}

const cocos2d::Vector<InfoItems*>& InfoAdventureRelic::getListGrindReward() const
{
    return _listGrindReward;
}
void InfoAdventureRelic::setListGrindReward(const cocos2d::Vector<InfoItems*> list)
{
    _listGrindReward = list;
}

const bool InfoAdventureRelic::isReddotCondition()
{
    bool isReddotCondition = false;
    
    if(_isActive && AdventureRelicManager::getInstance()->getRelicLevelUpPrice(_nIdx, 1) <= _nCount && _nCurrentLv < AdventureRelicManager::getInstance()->getRelicMaxLv())
        isReddotCondition = true;
    
    return isReddotCondition;
}

