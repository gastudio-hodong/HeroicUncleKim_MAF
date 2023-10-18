//
//  InfoEvent7thPerson.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/01/11.
//

#include "InfoEvent7thPerson.h"

USING_NS_CC;

InfoEvent7thPerson::InfoEvent7thPerson() :
_nIdx(0)
{
    
}

InfoEvent7thPerson::~InfoEvent7thPerson()
{
    
}

bool InfoEvent7thPerson::init()
{
    return true;
}

int InfoEvent7thPerson::getLevel()
{
    return _nLevel.valueInt();
}

void InfoEvent7thPerson::setLevel(int level)
{
    _nLevel = level;
}

int InfoEvent7thPerson::getPointCountMin()
{
    return _nPointCountMin.valueInt();
}

void InfoEvent7thPerson::setPointCountMin(int count)
{
    _nPointCountMin = count;
}

int InfoEvent7thPerson::getPointCountMax()
{
    return _nPointCountMax.valueInt();
}

void InfoEvent7thPerson::setPointCountMax(int count)
{
    _nPointCountMax = count;
}

std::string InfoEvent7thPerson::getReward()
{
    return _strReward;
}

void InfoEvent7thPerson::setReward(std::string reward)
{
    _strReward = reward;
}
