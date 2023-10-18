//
//  InfoTownReward.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/07.
//

#include "InfoTownReward.h"
#include "ManagerGame/ItemsManager.h"

InfoTownReward::InfoTownReward() :
_type(0),
_condition(0),

_items(""),
_bReward(false)
{
}

InfoTownReward::~InfoTownReward()
{
}

bool InfoTownReward::init()
{
    return true;
}

int InfoTownReward::getType()
{
    return _type;
}

void InfoTownReward::setType(int value)
{
    _type = value;
}

int InfoTownReward::getCondition()
{
    return _condition;
}

void InfoTownReward::setCondition(int value)
{
    _condition = value;
}
 
InfoItems* InfoTownReward::getItems()
{
    return ItemsManager::getInstance()->getConvert(_items);
}

void InfoTownReward::setItems(std::string value)
{
    _items = value;
}

bool InfoTownReward::isReward()
{
    return _bReward;
}

void InfoTownReward::setReward(bool value)
{
    _bReward = value;
}
