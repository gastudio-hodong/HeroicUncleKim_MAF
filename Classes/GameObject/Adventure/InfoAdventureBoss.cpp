//
//  InfoAdventureBoss.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/16.
//

#include "InfoAdventureBoss.h"
 
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

InfoAdventureBoss::InfoAdventureBoss() :
_idx(0),

_nameKey(""),
_iconBannerPath(""),
_iconBattlePath(""),
_iconBattleBGPath(""),

_hpNow(0),
_hpMax(0),

_weakness1(0),
_weakness1CountNow(0),
_weakness1CountMax(0),
_weakness2(0),
_weakness2CountNow(0),
_weakness2CountMax(0),

_reward(""),
_rewardBonus("")
{
    
}

bool InfoAdventureBoss::init()
{
    return true;
}
 

int InfoAdventureBoss::getIdx()
{
    return _idx.valueInt();
}

void InfoAdventureBoss::setIdx(int value)
{
    _idx = value;
}

bool InfoAdventureBoss::isHard()
{
    return _bHard.valueInt();
}

void InfoAdventureBoss::setHard(int value)
{
    _bHard = value;
}

std::string InfoAdventureBoss::getName()
{
    return GAME_TEXT(_nameKey);
}

void InfoAdventureBoss::setNameKey(std::string value)
{
    _nameKey = value;
}

std::string InfoAdventureBoss::getIconBannerPath()
{
    return _iconBannerPath;
}

void InfoAdventureBoss::setIconBannerPath(std::string value)
{
    _iconBannerPath = "Assets/ui/adventure/boss/" + value;
}

std::string InfoAdventureBoss::getIconBattlePath()
{
    return _iconBattlePath;
}

void InfoAdventureBoss::setIconBattlePath(std::string value)
{
    _iconBattlePath = "Assets/ui/adventure/boss/" + value;
}

std::string InfoAdventureBoss::getIconBattleBGPath()
{
    return _iconBattleBGPath;
}
void InfoAdventureBoss::setIconBattleBGPath(std::string value)
{
    _iconBattleBGPath = "Assets/ui/adventure/boss/" + value;
}

int InfoAdventureBoss::getHpNow()
{
    return _hpNow.valueInt();
}

void InfoAdventureBoss::setHpNow(int value)
{
    _hpNow = value;
}

int InfoAdventureBoss::getHpMax()
{
    return _hpMax.valueInt();
}

void InfoAdventureBoss::setHpMax(int value)
{
    _hpMax = value;
}

int InfoAdventureBoss::getWeakness1()
{
    return _weakness1.valueInt();
}

void InfoAdventureBoss::setWeakness1(int value)
{
    _weakness1 = value;
}

int InfoAdventureBoss::getWeakness1CountNow()
{
    return _weakness1CountNow.valueInt();
}

void InfoAdventureBoss::setWeakness1CountNow(int value)
{
    _weakness1CountNow = value;
}

int InfoAdventureBoss::getWeakness1CountMax()
{
    return _weakness1CountMax.valueInt();
}

void InfoAdventureBoss::setWeakness1CountMax(int value)
{
    _weakness1CountMax = value;
}

int InfoAdventureBoss::getWeakness2()
{
    return _weakness2.valueInt();
}

void InfoAdventureBoss::setWeakness2(int value)
{
    _weakness2 = value;
}

int InfoAdventureBoss::getWeakness2CountNow()
{
    return _weakness2CountNow.valueInt();
}

void InfoAdventureBoss::setWeakness2CountNow(int value)
{
    _weakness2CountNow = value;
}

int InfoAdventureBoss::getWeakness2CountMax()
{
    return _weakness2CountMax.valueInt();
}

void InfoAdventureBoss::setWeakness2CountMax(int value)
{
    _weakness2CountMax = value;
}

cocos2d::Vector<InfoItems*> InfoAdventureBoss::getListReward()
{
    return ItemsManager::getInstance()->getConvertArray(_reward);
}

void InfoAdventureBoss::setReward(std::string value)
{
    _reward = value;
}

cocos2d::Vector<InfoItems*> InfoAdventureBoss::getListRewardBonus()
{
    return ItemsManager::getInstance()->getConvertArray(_rewardBonus);
}

void InfoAdventureBoss::setRewardBonus(std::string value)
{
    _rewardBonus = value;
}

