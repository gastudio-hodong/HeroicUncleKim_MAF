//
//  InfoEventTreasureHuntReward.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/24.
//

#include "InfoEventTreasureHuntReward.h"

#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

InfoEventTreasureHuntReward::InfoEventTreasureHuntReward() :
_type(0),
_level(0),
_value(0),
_reward("")
{
}

InfoEventTreasureHuntReward::~InfoEventTreasureHuntReward()
{
}

bool InfoEventTreasureHuntReward::init()
{
    return true;
}
 
const int InfoEventTreasureHuntReward::getType()
{
    return _type.valueInt();
}

void InfoEventTreasureHuntReward::setType(int value)
{
    _type = value;
}

const int InfoEventTreasureHuntReward::getLevel()
{
    return _level.valueInt();
}

void InfoEventTreasureHuntReward::setLevel(int value)
{
    _level = value;
}

const int InfoEventTreasureHuntReward::getValue()
{
    return _value.valueInt();
}

void InfoEventTreasureHuntReward::setValue(int value)
{
    _value = value;
}

cocos2d::Vector<InfoItems*> InfoEventTreasureHuntReward::getReward()
{
    return ItemsManager::getInstance()->getConvertArray(_reward);
}
void InfoEventTreasureHuntReward::setReward(const std::string value)
{
    _reward = value;
}
