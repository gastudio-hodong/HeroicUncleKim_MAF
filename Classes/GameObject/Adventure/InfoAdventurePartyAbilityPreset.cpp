//
//  InfoAdventurePartyAbilityPreset.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/23.
//

#include "InfoAdventurePartyAbilityPreset.h"

#include "GameObject/Adventure/InfoAdventurePartyAbilityOrigin.h"

USING_NS_CC;

#pragma mark -
InfoAdventurePartyAbilityPreset::InfoAdventurePartyAbilityPreset():
_nIdx(0)
{
    _listAbility.clear();
    _listSeal.clear();
}

InfoAdventurePartyAbilityPreset::~InfoAdventurePartyAbilityPreset()
{
    _listAbility.clear();
    _listSeal.clear();
}

bool InfoAdventurePartyAbilityPreset::init()
{
    return true;
}
const int InfoAdventurePartyAbilityPreset::getIdx() const
{
    return _nIdx;
}
void InfoAdventurePartyAbilityPreset::setIdx(const int value)
{
    _nIdx = value;
}

const cocos2d::Vector<InfoAdventurePartyAbility*>& InfoAdventurePartyAbilityPreset::getListAblity() const
{
    return _listAbility;
}
void InfoAdventurePartyAbilityPreset::setListAblity(const cocos2d::Vector<InfoAdventurePartyAbility*> value)
{
    _listAbility = value;
}

const std::vector<bool> InfoAdventurePartyAbilityPreset::getListSeal() const
{
    return _listSeal;
}
void InfoAdventurePartyAbilityPreset::setListSeal(const std::vector<bool> value)
{
    _listSeal = value;
}

const int InfoAdventurePartyAbilityPreset::getAbilityOpenCount() const
{
    int result = 0;
    
    for(auto obj : _listAbility)
    {
        if(obj->isOpen())
            result++;
    }
    
    return result;
}
const int InfoAdventurePartyAbilityPreset::getSealCount() const
{
    int result = 0;
    
    for(auto obj : _listSeal)
    {
        if(obj)
            result++;
    }
    
    return result;
}

void InfoAdventurePartyAbilityPreset::checkAvilityError()
{
    for(int i = 0; i < _listAbility.size(); ++i)
    {
        auto obj = _listAbility.at(i);
        
        if(obj->getType() == E_ADVENTURE_PROPERTY_TYPE::NONE)
        {
            if(obj->getGrade() != 0)
            {
                obj->setGrade(0);
            }
            if(_listSeal[i] == true)
                _listSeal[i] = false;
        }
        if(obj->getGrade() > 5 || obj->getGrade() == 0)
        {
            if(obj->getType() != E_ADVENTURE_PROPERTY_TYPE::NONE)
            {
                obj->setIdx((int)E_ADVENTURE_PROPERTY_TYPE::NONE);
            }
            if(_listSeal[i] == true)
                _listSeal[i] = false;
        }
    }
    
}
