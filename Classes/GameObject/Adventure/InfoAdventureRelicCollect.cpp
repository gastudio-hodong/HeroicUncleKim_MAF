//
//  InfoAdventureRelicCollect.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/12.
//

#include "InfoAdventureRelicCollect.h"

#include "GameObject/InfoItems.h"

#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/AdventureRelicManager.h"

USING_NS_CC;

#pragma mark -
InfoAdventureRelicCollect::InfoAdventureRelicCollect():
_isActive(false)
,_nIdx(0)
,_dAvlilityEffect(0)
,_eType(E_ADVENTURE_PROPERTY_TYPE::NONE)
{
    _listCondition.clear();
    _listReward.clear();
}

InfoAdventureRelicCollect::~InfoAdventureRelicCollect()
{
    _listCondition.clear();
    _listReward.clear();
}

bool InfoAdventureRelicCollect::init()
{
    return true;
}

const bool InfoAdventureRelicCollect::isActive() const
{
    return _isActive;
}
void InfoAdventureRelicCollect::setIsActive(const bool value)
{
    _isActive = value;
}
const int InfoAdventureRelicCollect::getIdx() const
{
    return _nIdx;
}
void InfoAdventureRelicCollect::setIdx(const int value)
{
    _nIdx = value;
}

const E_ADVENTURE_PROPERTY_TYPE InfoAdventureRelicCollect::getType() const
{
    return _eType;
}
void InfoAdventureRelicCollect::setType(const E_ADVENTURE_PROPERTY_TYPE value)
{
    _eType = value;
}

InfoAdventurePropertyType* const InfoAdventureRelicCollect::getInfoType() const
{
    return AdventureManager::getInstance()->getPropertyType(_eType);
}

void InfoAdventureRelicCollect::setListCondition(const std::vector<std::pair<int,int>> list)
{
    _listCondition = list;
}
const std::vector<std::pair<int,int>> InfoAdventureRelicCollect::getListCondition() const
{
    return _listCondition;
}
const std::pair<int,int> InfoAdventureRelicCollect::getCondition(int idx) const
{
    std::pair<int,int> result = std::make_pair(1, 1);;
    
    for(auto obj : _listCondition)
    {
        if(obj.first == idx)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

const double InfoAdventureRelicCollect::getAbilityEffect()
{
    return _dAvlilityEffect;
}
void InfoAdventureRelicCollect::setAbilityEffect(const double value)
{
    _dAvlilityEffect = value;
}

const cocos2d::Vector<InfoItems*>& InfoAdventureRelicCollect::getListReward() const
{
    return _listReward;
}
void InfoAdventureRelicCollect::setListReward(const cocos2d::Vector<InfoItems*> list)
{
    _listReward = list;
}

const bool InfoAdventureRelicCollect::isReward() const
{
    bool result = true;
    
    for(int i = 0; i < _listCondition.size(); ++i)
    {
        std::pair<int,int> objCondition = _listCondition[i];
        
        auto objRelic = AdventureRelicManager::getInstance()->getInfoRelic(objCondition.first);
        
        if(objRelic == nullptr || objRelic->getCurrentLv() < objCondition.second)
        {
            result = false;
            break;
        }
    }
    
    return result;
}
