//
//  InfoDefenseTrench.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/31.
//

#include "InfoDefenseTrench.h"
#include "ManagerGame/DefenseManager.h"
USING_NS_CC;

InfoDefenseTrench* InfoDefenseTrench::create()
{
    InfoDefenseTrench *pRet = new(std::nothrow) InfoDefenseTrench();
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

InfoDefenseTrench::InfoDefenseTrench() :
_number(0),
_characterIdx(0),
_weaponIdx(0)
{
}

InfoDefenseTrench::~InfoDefenseTrench()
{
}

bool InfoDefenseTrench::init()
{
    return true;
}

int InfoDefenseTrench::getNumber() const
{
    return _number;
}

void InfoDefenseTrench::setNumber(const int value)
{
    _number = value;
}

int InfoDefenseTrench::getWeaponIdx() const
{
    return _weaponIdx;
}

void InfoDefenseTrench::setWeaponIdx(const int value)
{
    _weaponIdx = value;
}

int InfoDefenseTrench::getCharacterIdx() const
{
    return _characterIdx;
}

void InfoDefenseTrench::setCharacterIdx(const int value)
{
    _characterIdx = value;
}
InfoDefenseWeapon* InfoDefenseTrench::getInfoDefenseWeapon() const
{
    return DefenseManager::getInstance()->getWeaponData(_weaponIdx);
}
InfoDefenseCharacter* InfoDefenseTrench::getInfoDefenseCharacter() const
{
    return DefenseManager::getInstance()->getCharacterData(_characterIdx);
}
