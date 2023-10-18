//
//  InfoRaidDragon.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/27.
//

#include "InfoRaidDragon.h"

USING_NS_CC;

InfoRaidDragon::InfoRaidDragon():
_nLevel(0),
_fHP(0)
{
    
}

InfoRaidDragon::~InfoRaidDragon()
{
    
}

bool InfoRaidDragon::init()
{
    return true;
}
 
int InfoRaidDragon::getLevel()
{
    return _nLevel;
}
void InfoRaidDragon::setLevel(int value)
{
    _nLevel = value;
}
double InfoRaidDragon::getHP()
{
    return _fHP;
}
void InfoRaidDragon::setHP(double value)
{
    _fHP = value;
}
