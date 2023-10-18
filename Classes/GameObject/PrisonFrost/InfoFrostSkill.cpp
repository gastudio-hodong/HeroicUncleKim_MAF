//
//  InfoFrostSkill.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/07.
//

#include "InfoFrostSkill.h"

USING_NS_CC;

InfoFrostSkill::InfoFrostSkill() :
_idx(0),
_step(0),

_type(""),
_nameKey(""),
_contentsKey(""),
_iconPath(""),

_levelMax(0),
_valueBasic(0),
_valueLevelPer(0),

_coolTime(0),

_duration(0)
{
    
}

InfoFrostSkill::~InfoFrostSkill()
{
}

bool InfoFrostSkill::init()
{
    return true;
}
 
int InfoFrostSkill::getIdx()
{
    return _idx;
}

void InfoFrostSkill::setIdx(int value)
{
    _idx = value;
}
 
int InfoFrostSkill::getStep()
{
    return _step;
}

void InfoFrostSkill::setStep(int value)
{
    _step = value;
}

std::string InfoFrostSkill::getType()
{
    return _type;
}
void InfoFrostSkill::setType(std::string value)
{
    _type = value;
}

std::string InfoFrostSkill::getName()
{
    return GAME_TEXT(_nameKey);
}
void InfoFrostSkill::setNameKey(std::string value)
{
    _nameKey = value;
}

std::string InfoFrostSkill::getContents()
{
    return GAME_TEXT(_contentsKey);
}
void InfoFrostSkill::setContentsKey(std::string value)
{
    _contentsKey = value;
}

std::string InfoFrostSkill::getIconPath()
{
    std::string path = "Assets/icon_skill_frost/";
    path.append(_iconPath);
    
    return path;
}
void InfoFrostSkill::setIconPath(std::string value)
{
    _iconPath = value;
}

int InfoFrostSkill::getLevelMax()
{
    return _levelMax;
}
void InfoFrostSkill::setLevelMax(int value)
{
    _levelMax = value;
}

double InfoFrostSkill::getValueBasic()
{
    return _valueBasic;
}
void InfoFrostSkill::setValueBasic(double value)
{
    _valueBasic = value;
}

double InfoFrostSkill::getValueLevelPer()
{
    return _valueLevelPer;
}
void InfoFrostSkill::setValueLevelPer(double value)
{
    _valueLevelPer = value;
}

int InfoFrostSkill::getCoolTime()
{
    return _coolTime;
}
void InfoFrostSkill::setCoolTime(int value)
{
    _coolTime = value;
}

int InfoFrostSkill::getDuration()
{
    return _duration;
}
void InfoFrostSkill::setDuration(int value)
{
    _duration = value;
}
