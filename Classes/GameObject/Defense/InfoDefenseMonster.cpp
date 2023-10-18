//
//  InfoDefenseMonster.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/20.
//

#include "InfoDefenseMonster.h"

USING_NS_CC;

InfoDefenseMonster* InfoDefenseMonster::create()
{
    InfoDefenseMonster *pRet = new(std::nothrow) InfoDefenseMonster();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

InfoDefenseMonster::InfoDefenseMonster() :
_index(0),
_type(E_DF_MonsterType::NONE),
_hpCoefficient(0),
_defensive(0),
_speed(0)
{
}

InfoDefenseMonster::~InfoDefenseMonster()
{
}

bool InfoDefenseMonster::init()
{
    return true;
}

int InfoDefenseMonster::getIndex() const
{
    return _index;
}

void InfoDefenseMonster::setIndex(const int value)
{
    _index = value;
}

E_DF_MonsterType InfoDefenseMonster::getType() const
{
    return _type;
}
void InfoDefenseMonster::setType(const E_DF_MonsterType value)
{
    _type = value;
}

double InfoDefenseMonster::getHpCoefficient() const
{
    return _hpCoefficient;
}
void InfoDefenseMonster::setHpCoefficient(const double value)
{
    _hpCoefficient = value;
}

double InfoDefenseMonster::getDefensive() const
{
    return _defensive;
}

void InfoDefenseMonster::setDefensive(const double value)
{
    _defensive = value;
}

int InfoDefenseMonster::getSpeed() const
{
    return _speed;
}

void InfoDefenseMonster::setSpeed(const int value)
{
    _speed = value;
}
