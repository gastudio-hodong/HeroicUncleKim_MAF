//
//  InfoAttend.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 14/02/2020.
//

#include "InfoAttend.h"

#include "GameObject/InfoItems.h"

#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

InfoAttend::InfoAttend() :
_type(0),
_day(0),
_reward(""),

_receive(false)
{
    
}

InfoAttend::~InfoAttend()
{
    
}

bool InfoAttend::init()
{
    return true;
}


int InfoAttend::getType()
{
    return _type;
}

void InfoAttend::setType(int value)
{
    _type = value;
}

int InfoAttend::getDay()
{
    return _day;
}

void InfoAttend::setDay(int value)
{
    _day = value;
}

InfoItems* InfoAttend::getReward()
{
    return ItemsManager::getInstance()->getConvert(_reward);
}

void InfoAttend::setReward(std::string value)
{
    _reward = value;
}

bool InfoAttend::isReceive()
{
    return _receive;
}

void InfoAttend::setReceive(bool value)
{
    _receive = value;
}
