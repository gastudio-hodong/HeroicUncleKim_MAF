//
//  InfoAdventureParty.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/18.
//

#include "InfoAdventureParty.h"

#include "GameObject/InfoItems.h"
#include "GameObject/Adventure/InfoAdventureMember.h"
#include "GameObject/Adventure/InfoAdventureStage.h"
#include "GameObject/Adventure/InfoAdventurePartyAbilityPreset.h"
#include "GameObject/Adventure/InfoAdventurePartyAbilityOrigin.h"

USING_NS_CC;

InfoAdventureParty::InfoAdventureParty():
_nIdx(0)
,_nPresetIdx(1)
,_infoStage(nullptr)
{
    _listMember.clear();
    _listAbilityPreset.clear();
}

InfoAdventureParty::~InfoAdventureParty()
{
    _listMember.clear();
    _listAbilityPreset.clear();
}

bool InfoAdventureParty::init()
{
    
    return true;
}
const int InfoAdventureParty::getIdx() const
{
    return _nIdx;
}
void InfoAdventureParty::setIdx(const int value)
{
    _nIdx = value;
}
const int InfoAdventureParty::getPresetIdx() const
{
    return _nPresetIdx;
}
void InfoAdventureParty::setPresetIdx(const int value)
{
    _nPresetIdx = value;
}

InfoAdventureStage* const InfoAdventureParty::getInfoStage() const
{
    return _infoStage;
}
void InfoAdventureParty::setInfoStage(InfoAdventureStage* const value)
{
    _infoStage = value;
    _infoStage->setPartyIdx(_nIdx);
}

void InfoAdventureParty::resetStageData()
{
    if(_infoStage == nullptr)
        return;
    _infoStage = nullptr;
}

const int InfoAdventureParty::getArea() const
{
    int result = 0;
    if(_infoStage != nullptr)
        result = _infoStage->getArea();
    
    return result;
}

const bool InfoAdventureParty::isRunning() const
{
    bool result = false;
    
    if(_infoStage != nullptr)
        result = _infoStage->isRunning();
    
    return result;
}

const int InfoAdventureParty::getStage() const
{
    int result = 0;
    if(_infoStage != nullptr)
        result = _infoStage->getIdx();
    
    return result;
}

const int InfoAdventureParty::getEndTime() const
{
    int result = 0;
    if(_infoStage != nullptr)
        result = _infoStage->getEndTime();
    
    return result;
}
const int InfoAdventureParty::getExp() const
{
    int result = 0;
    if(_infoStage != nullptr)
        result = _infoStage->getExp();
    
    return result;
}
const cocos2d::Vector<InfoAdventureMember*>& InfoAdventureParty::getListMemeber() const
{
    return _listMember;
}
InfoAdventureMember* const InfoAdventureParty::getMember(const int nIdx) const
{
    InfoAdventureMember* result = nullptr;
    
    
    for(auto obj : _listMember)
    {
        if(obj->getIdx() == nIdx)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
void InfoAdventureParty::setMember(const cocos2d::Vector<InfoAdventureMember*> value)
{
    _listMember = value;
}
void InfoAdventureParty::pushBackMember(InfoAdventureMember* const value)
{
    _listMember.pushBack(value);
}

const cocos2d::Vector<InfoItems*> InfoAdventureParty::getReward() const
{
    cocos2d::Vector<InfoItems*> listReward;
    listReward.clear();
    
    if(_infoStage != nullptr)
        listReward = _infoStage->getListReward();
    
    return listReward;
}

const cocos2d::Vector<InfoItems*> InfoAdventureParty::getRewardBoss() const
{
    cocos2d::Vector<InfoItems*> listReward;
    listReward.clear();
    
    if(_infoStage != nullptr)
        listReward = _infoStage->getListRewardBossPoint();
    
    return listReward;
}

InfoAdventurePartyAbilityPreset* const InfoAdventureParty::getAblityPreset(int nPresetIdx) const
{
    InfoAdventurePartyAbilityPreset* result = nullptr;
    
    if(nPresetIdx == -1)
        nPresetIdx = _nPresetIdx;
    
    for(const auto obj : _listAbilityPreset)
    {
        if(obj->getIdx() == nPresetIdx)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
void InfoAdventureParty::setListAblityPreset(const cocos2d::Vector<InfoAdventurePartyAbilityPreset*> value)
{
    _listAbilityPreset.clear();
    _listAbilityPreset = value;
}

const cocos2d::Vector<InfoAdventurePartyAbilityPreset*>& InfoAdventureParty::getListAblityPreset() const
{
    return _listAbilityPreset;
}
const cocos2d::Vector<InfoAdventurePartyAbility*> InfoAdventureParty::getListAblity(int nPresetIdx) const
{
    cocos2d::Vector<InfoAdventurePartyAbility*> result;
    result.clear();
    
    if(nPresetIdx == -1)
        nPresetIdx = _nPresetIdx;
    
    auto objPreset = getAblityPreset(nPresetIdx);
    
    if(objPreset != nullptr)
        result = objPreset->getListAblity();
    
    return result;
}

const std::vector<bool> InfoAdventureParty::getListSeal(int nPresetIdx) const
{
    std::vector<bool> result;
    result.clear();
    
    if(nPresetIdx == -1)
        nPresetIdx = _nPresetIdx;
    
    auto objPreset = getAblityPreset(nPresetIdx);
    
    if(objPreset != nullptr)
        result = objPreset->getListSeal();
    
    return result;
}

const bool InfoAdventureParty::isOpen() const
{
    auto objMember = getMember(1);
    
    if(objMember == nullptr)
        return false;
    
    return objMember->isOpen();
}

const int InfoAdventureParty::getAbilityOpenCount(int nPresetIdx) const
{
    int result = 0;
    
    if(nPresetIdx == -1)
        nPresetIdx = _nPresetIdx;
    
    auto objPreset = getAblityPreset(nPresetIdx);
    
    if(objPreset != nullptr)
        result = objPreset->getAbilityOpenCount();
    
    return result;
}
const int InfoAdventureParty::getSealCount(int nPresetIdx) const
{
    int result = 0;
    
    if(nPresetIdx == -1)
        nPresetIdx = _nPresetIdx;
    
    auto objPreset = getAblityPreset(nPresetIdx);
    
    if(objPreset != nullptr)
        result = objPreset->getSealCount();
    
    return result;
}

void InfoAdventureParty::checkPresetError()
{
    for(auto obj : _listAbilityPreset)
    {
        obj->checkAvilityError();
    }
}

void InfoAdventureParty::checkLocalCompleteUpgrade()
{
    for(auto obj : _listMember)
    {
        obj->completeUpgrade();
    }
}
