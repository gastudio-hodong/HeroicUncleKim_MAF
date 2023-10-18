//
//  InfoLoot.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/06/07.
//

#include "InfoLoot.h"

USING_NS_CC;

#pragma mark - InfoLoot
InfoLoot::InfoLoot() :
_idx(0),
_grade(0),

_nameKey(""),
_descKey(""),

_effectDefault(0),
_effectIncreasing(0),

_levelMax(0)
{
    
}

InfoLoot::~InfoLoot()
{
    
}

bool InfoLoot::init()
{
    return true;
}

int InfoLoot::getIdx()
{
    return _idx;
}

void InfoLoot::setIdx(int value)
{
    _idx = value;
}

int InfoLoot::getGrade()
{
    return _grade;
}
void InfoLoot::setGrade(int value)
{
    _grade = value;
}

std::string InfoLoot::getName()
{
    return GAME_TEXT(_nameKey);
}
void InfoLoot::setNameKey(std::string value)
{
    _nameKey = value;
}

std::string InfoLoot::getDesc()
{
    return GAME_TEXT(_descKey);
}
void InfoLoot::setDescKey(std::string value)
{
    _descKey = value;
}

double InfoLoot::getEffectDefault()
{
    return _effectDefault;
}
void InfoLoot::setEffectDefault(double value)
{
    _effectDefault = value;
}

double InfoLoot::getEffectIncreasing()
{
    return _effectIncreasing;
}
void InfoLoot::setEffectIncreasing(double value)
{
    _effectIncreasing = value;
}

int InfoLoot::getLevelMax()
{
    return _levelMax;
}
void InfoLoot::setLevelMax(int value)
{
    _levelMax = value;
}

