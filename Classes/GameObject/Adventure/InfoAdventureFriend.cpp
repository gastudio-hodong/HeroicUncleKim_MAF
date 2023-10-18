//
//  InfoAdventureFriendAbility.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/16.
//

#include "InfoAdventureFriend.h"

#include "ManagerGame/AdventureManager.h"

USING_NS_CC;

#pragma mark - Ability
InfoAdventureFriendAbility::InfoAdventureFriendAbility():
_nIdx(0)
,_nLv(0)
,_nAdventurePoint(0)
{
    _listEffect.clear();
}

InfoAdventureFriendAbility::~InfoAdventureFriendAbility()
{
    _listEffect.clear();
}

bool InfoAdventureFriendAbility::init()
{
    return true;
}

const int InfoAdventureFriendAbility::getIdx() const
{
    return _nIdx;
}
void InfoAdventureFriendAbility::setIdx(const int value)
{
    _nIdx = value;
}

const int InfoAdventureFriendAbility::getMinLv() const
{
    return _nLv;
}
void InfoAdventureFriendAbility::setMinLv(const int value)
{
    _nLv = value;
}

const int InfoAdventureFriendAbility::getAdventurePoint() const
{
    return _nAdventurePoint;
}
void InfoAdventureFriendAbility::setAdventurePoint(const int value)
{
    _nAdventurePoint = value;
}


InfoAdventurePropertyType* const InfoAdventureFriendAbility::getInfoType(E_ADVENTURE_PROPERTY_TYPE eType) const
{
    return AdventureManager::getInstance()->getPropertyType(eType);
}

void InfoAdventureFriendAbility::setListEffect(const std::vector<std::pair<E_ADVENTURE_PROPERTY_TYPE, TDouble>> list)
{
    _listEffect = list;
}
const std::vector<std::pair<E_ADVENTURE_PROPERTY_TYPE, TDouble>> InfoAdventureFriendAbility::getListEffect() const
{
    return _listEffect;
}

const double InfoAdventureFriendAbility::getAbilityEffect(E_ADVENTURE_PROPERTY_TYPE eType)
{
    double result = 0;
    
    for(auto obj : _listEffect)
    {
        if(obj.first == eType)
        {
            result = obj.second;
            break;
        }
    }
    
    return result;
}

#pragma mark - Friend
InfoAdventureFriend::InfoAdventureFriend():
_isAdventure(false)
,_nUseridx(0)
,_nLevel(0)
,_strPlatform("")
,_strNickName("")
,_infoStage(nullptr)
{
}

InfoAdventureFriend::~InfoAdventureFriend()
{
}

bool InfoAdventureFriend::init()
{
    return true;
}

const bool InfoAdventureFriend::isAdventure() const
{
    return _isAdventure;
}
void InfoAdventureFriend::setIsAdventure(const bool value)
{
    _isAdventure = value;
}

const int InfoAdventureFriend::getUseridx() const
{
    return _nUseridx;
}
void InfoAdventureFriend::setUseridx(const int value)
{
    _nUseridx = value;
}
void InfoAdventureFriend::setUseridxFromPartyIdx(const int value)
{
    _nUseridx = value;
    _strPlatform = "aos";
    
    if(_nUseridx % 100 != 0)
        _strPlatform = "ios";
    
    _nUseridx /= 100;
}
const int InfoAdventureFriend::getPartyIdx() const
{
    int result = _nUseridx * 100;
    
    if(_strPlatform.compare("ios") == 0)
        result++;
    
    return result;
}

const int InfoAdventureFriend::getLevel() const
{
    return _nLevel;
}
void InfoAdventureFriend::setLevel(const int value)
{
    _nLevel = value;
}

const int InfoAdventureFriend::getGrade() const
{
    int result = 0;
    
    int nRatio = AdventureManager::getInstance()->getMaxLevel()/4;
    
    result = _nLevel/nRatio;
    if(result <= 0)
        result++;
    else if(result >= 4)
        result = 4;
    
    return result;
}

const std::string InfoAdventureFriend::getPlatform() const
{
    return _strPlatform;
}

void InfoAdventureFriend::setPlatform(const std::string value)
{
    _strPlatform = value;
}

const std::string InfoAdventureFriend::getNickName() const
{
    return _strNickName;
}

void InfoAdventureFriend::setNickName(const std::string value)
{
    _strNickName = value;
}

InfoAdventureStage* const InfoAdventureFriend::getInfoStage() const
{
    return _infoStage;
}
void InfoAdventureFriend::setInfoStage(InfoAdventureStage* const value)
{
    _infoStage = value;
    _infoStage->setPartyIdx(getPartyIdx());
}
const double InfoAdventureFriend::getAbilityEffect(E_ADVENTURE_PROPERTY_TYPE eType)
{
    double result = 0;
    
    auto objAbility = AdventureManager::getInstance()->getInfoFriendAbilityFromLevel(_nLevel);
    
    if(objAbility != nullptr)
        result = objAbility->getAbilityEffect(eType);
    
    return result;
}

const std::vector<std::pair<E_ADVENTURE_PROPERTY_TYPE, TDouble>> InfoAdventureFriend::getListEffect()const
{
    std::vector<std::pair<E_ADVENTURE_PROPERTY_TYPE, TDouble>> result;
    result.clear();
    
    auto objAbility = AdventureManager::getInstance()->getInfoFriendAbilityFromLevel(_nLevel);
    
    if(objAbility != nullptr)
        result = objAbility->getListEffect();
    
    return result;
}
