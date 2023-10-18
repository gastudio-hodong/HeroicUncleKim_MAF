//
//  InfoWeaponSpiritBuff.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/01/24.
//

#include "InfoWeaponSpiritBuff.h"

USING_NS_CC;

InfoWeaponSpiritBuff::InfoWeaponSpiritBuff():
_eType(E_SPIRIT_BUFF_TYPE::NONE)
,_nIdx(0)
,_nIncreases(0)
,_nCoolTime(0)
,_nDurationTime(0)
,_effectColor(Color4B::BLACK)
{
}

InfoWeaponSpiritBuff::~InfoWeaponSpiritBuff()
{
}

bool InfoWeaponSpiritBuff::init()
{
    return true;
}

E_SPIRIT_BUFF_TYPE InfoWeaponSpiritBuff::getType()
{
    return _eType;
}
void InfoWeaponSpiritBuff::setType(E_SPIRIT_BUFF_TYPE value)
{
    _eType = value;
}
int InfoWeaponSpiritBuff::getIdx()
{
    return _nIdx;
}
void InfoWeaponSpiritBuff::setIdx(int value)
{
    _nIdx = value;
}
int InfoWeaponSpiritBuff::getIncreases()
{
    return _nIncreases.valueInt();
}
void InfoWeaponSpiritBuff::setIncreases(int value)
{
    _nIncreases = value;
}
int InfoWeaponSpiritBuff::getCoolTime()
{
    return _nCoolTime.valueInt();
}
void InfoWeaponSpiritBuff::setCoolTime(int value)
{
    _nCoolTime = value;
}
int InfoWeaponSpiritBuff::getDurationTime()
{
    return _nDurationTime.valueInt();
}
void InfoWeaponSpiritBuff::setDurationTime(int value)
{
    _nDurationTime = value;
}
Color4B InfoWeaponSpiritBuff::getEffectColor()
{
    return _effectColor;
}
void InfoWeaponSpiritBuff::setEffectColor(Color4B value)
{
    _effectColor = value;
}
