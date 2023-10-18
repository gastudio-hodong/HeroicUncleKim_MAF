//
//  InfoEvent7th.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/01/11.
//

#include "InfoEvent7th.h"

USING_NS_CC;

InfoEvent7th::InfoEvent7th() :
_nType(0),
_nLevel(0),
_nPointCountMin(0),
_nPointCountMax(0),
_nPointCountPersonalLimit(0),

_nConditionPersonLevel(0),

_strNameKey(""),
_strDiscKey(""),
_strReward("")
{
    
}

InfoEvent7th::~InfoEvent7th()
{
    
}

bool InfoEvent7th::init()
{
    return true;
}

int InfoEvent7th::getType()
{
    return _nType.valueInt();
}

void InfoEvent7th::setType(int type)
{
    _nType = type;
}

int InfoEvent7th::getLevel()
{
    return _nLevel.valueInt();
}

void InfoEvent7th::setLevel(int level)
{
    _nLevel = level;
}

int InfoEvent7th::getPointCountMin()
{
    return _nPointCountMin.valueInt();
}

void InfoEvent7th::setPointCountMin(int count)
{
    _nPointCountMin = count;
}

int InfoEvent7th::getPointCountMax()
{
    return _nPointCountMax.valueInt();
}

void InfoEvent7th::setPointCountMax(int count)
{
    _nPointCountMax = count;
}

int InfoEvent7th::getPointCountPersonalLimit()
{
    return _nPointCountPersonalLimit.valueInt();
}

void InfoEvent7th::setPointCountPersonalLimit(int count)
{
    _nPointCountPersonalLimit = count;
}

int InfoEvent7th::getConditionPersonLevel()
{
    return _nConditionPersonLevel.valueInt();
}

void InfoEvent7th::setConditionPersonLevel(int level)
{
    _nConditionPersonLevel = level;
}

std::string InfoEvent7th::getName()
{
    return GAME_TEXTFORMAT(_strNameKey, getLevel());
}

void InfoEvent7th::setNameKey(std::string key)
{
    _strNameKey = key;
}

std::string InfoEvent7th::getDesc()
{
    return GAME_TEXT(_strDiscKey);
}

void InfoEvent7th::setDescKey(std::string key)
{
    _strDiscKey = key;
}

std::string InfoEvent7th::getReward()
{
    return _strReward;
}

void InfoEvent7th::setReward(std::string reward)
{
    _strReward = reward;
}

