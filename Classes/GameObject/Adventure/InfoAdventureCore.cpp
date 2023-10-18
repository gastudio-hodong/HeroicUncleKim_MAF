//
//  InfoAdventureCore.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/09.
//

#include "InfoAdventureCore.h"

USING_NS_CC;

#pragma mark -
InfoAdventureCore::InfoAdventureCore():
_isActive(false)
,_nIdx(0)
,_nCount(0)
,_nCurrentLv(0)
,_nExp(0)
{
}

InfoAdventureCore::~InfoAdventureCore()
{
}

bool InfoAdventureCore::init()
{
    return true;
}
const bool InfoAdventureCore::isActive() const
{
    return _isActive;
}
void InfoAdventureCore::setIsActice(const bool value)
{
    _isActive = value;
}
const int InfoAdventureCore::getIdx() const
{
    return _nIdx;
}
void InfoAdventureCore::setIdx(const int value)
{
    _nIdx = value;
}
const int InfoAdventureCore::getCurrentLv()
{
    return _nCurrentLv.valueInt();
}
void InfoAdventureCore::setCurrentLv(const int value)
{
    _nCurrentLv = value;
}
const int InfoAdventureCore::getExp()
{
    return _nExp.valueInt();
}
void InfoAdventureCore::setExp(const int value)
{
    _nExp = value;
}
const int InfoAdventureCore::getCount()
{
    return _nCount.valueInt();
}
void InfoAdventureCore::setCount(const int value)
{
    _nCount = value;
}
