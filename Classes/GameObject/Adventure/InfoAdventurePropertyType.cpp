//
//  InfoAdventurePropertyType.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/20.
//

#include "InfoAdventurePropertyType.h"

USING_NS_CC;

#pragma mark -
InfoAdventurePropertyType::InfoAdventurePropertyType():
_isPartyOption(false)
,_isRewardOption(false)
,_nIdx(0)
{
}

InfoAdventurePropertyType::~InfoAdventurePropertyType()
{
}

bool InfoAdventurePropertyType::init()
{
    return true;
}

const int InfoAdventurePropertyType::getIdx() const
{
    return _nIdx;
}
void InfoAdventurePropertyType::setIdx(const int value)
{
    _nIdx = value;
}

const E_ADVENTURE_PROPERTY_TYPE InfoAdventurePropertyType::getType() const
{
    return (E_ADVENTURE_PROPERTY_TYPE)_nIdx;
}

const bool InfoAdventurePropertyType::isPartyOption() const
{
    return _isPartyOption;
}
void InfoAdventurePropertyType::setIsPartyOption(const bool value)
{
    _isPartyOption = value;
}

const bool InfoAdventurePropertyType::isRewardOption() const
{
    return _isRewardOption;
}
void InfoAdventurePropertyType::setIsRewardOption(const bool value)
{
    _isRewardOption = value;
}
