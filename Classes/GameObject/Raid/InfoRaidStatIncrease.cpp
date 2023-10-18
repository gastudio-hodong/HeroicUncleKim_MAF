//
//  InfoRaidStatIncrease.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/10.
//

#include "InfoRaidStatIncrease.h"

USING_NS_CC;

InfoRaidStatIncrease::InfoRaidStatIncrease():
_nLv(0)
,_nDmg(0)
,_fCriDmg(0)
,_fCriRate(0)
,_fDoubleRate(0)
{
    
}

InfoRaidStatIncrease::~InfoRaidStatIncrease()
{
    
}

bool InfoRaidStatIncrease::init()
{
    return true;
}
 
int InfoRaidStatIncrease::getLv()
{
    return _nLv;
}
void InfoRaidStatIncrease::setLv(int value)
{
    _nLv = value;
}

int InfoRaidStatIncrease::getDmg()
{
    return _nDmg;
}
void InfoRaidStatIncrease::setDmg(int value)
{
    _nDmg = value;
}

double InfoRaidStatIncrease::getCriDmg()
{
    return _fCriDmg;
}
void InfoRaidStatIncrease::setCriDmg(double value)
{
    _fCriDmg = value;
}

double InfoRaidStatIncrease::getCriRate()
{
    return _fCriRate;
}
void InfoRaidStatIncrease::setCriRate(double value)
{
    _fCriRate = value;
}

double InfoRaidStatIncrease::getDoubleRate()
{
    return _fDoubleRate;
}
void InfoRaidStatIncrease::setDoubleRate(double value)
{
    _fDoubleRate = value;
}

