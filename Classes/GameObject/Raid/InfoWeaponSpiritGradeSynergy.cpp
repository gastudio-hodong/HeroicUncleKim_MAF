//
//  InfoWeaponSpiritGradeSynergy.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/01/25.
//

#include "InfoWeaponSpiritGradeSynergy.h"

USING_NS_CC;

InfoWeaponSpiritGradeSynergy::InfoWeaponSpiritGradeSynergy():
_nGrade(0)
,_nIncreasesLv(0)
,_nIncreasesSmelting(0)
{
}

InfoWeaponSpiritGradeSynergy::~InfoWeaponSpiritGradeSynergy()
{
}

bool InfoWeaponSpiritGradeSynergy::init()
{
    return true;
}
int InfoWeaponSpiritGradeSynergy::getGrade()
{
    return _nGrade;
}
void InfoWeaponSpiritGradeSynergy::setGrade(int value)
{
    _nGrade = value;
}
double InfoWeaponSpiritGradeSynergy::getIncreasesLv()
{
    return _nIncreasesLv;
}
void InfoWeaponSpiritGradeSynergy::setIncreasesLv(double value)
{
    _nIncreasesLv = value;
}
double InfoWeaponSpiritGradeSynergy::getIncreasesSmelting()
{
    return _nIncreasesSmelting;
}
void InfoWeaponSpiritGradeSynergy::setIncreasesSmelting(double value)
{
    _nIncreasesSmelting = value;
}
