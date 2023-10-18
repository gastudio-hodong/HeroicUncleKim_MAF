//
//  InfoFrostFloor.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/07.
//

#include "InfoFrostFloor.h"

#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

InfoFrostFloor::InfoFrostFloor() :
_idx(0),

_bBoss(false),

_cost(""),
_reward(""),

_hpControl(0)
{
    
}

InfoFrostFloor::~InfoFrostFloor()
{
}

bool InfoFrostFloor::init()
{
    return true;
}
 
int InfoFrostFloor::getIdx()
{
    return _idx;
}

void InfoFrostFloor::setIdx(int value)
{
    _idx = value;
}

bool InfoFrostFloor::isBoss()
{
    return _bBoss;
}

void InfoFrostFloor::setBoss(int value)
{
    _bBoss = value;
}

InfoItems* InfoFrostFloor::getCost()
{
    return ItemsManager::getInstance()->getConvert(_cost);
}

void InfoFrostFloor::setCost(std::string value)
{
    _cost = value;
}

std::string InfoFrostFloor::getReward()
{
    return _reward;
}

void InfoFrostFloor::setReward(std::string value)
{
    _reward = value;
}

std::string InfoFrostFloor::getHP()
{
    std::string hp = std::to_string(std::pow(1.2, _idx));
    
    long plusZero = 0;
    unsigned long index = hp.find(".");
    if ( index != std::string::npos )
    {
        hp.erase(hp.begin() + index);
        
        unsigned long length = hp.length() - index;
        
        plusZero = _hpControl - length;
        if ( plusZero < 0 )
        {
            hp.erase(index);
            plusZero = 0;
        }
    }

    hp = hp + std::string(plusZero, '0');
    return hp;
}

void InfoFrostFloor::setHPControl(int value)
{
    _hpControl = value;
}
