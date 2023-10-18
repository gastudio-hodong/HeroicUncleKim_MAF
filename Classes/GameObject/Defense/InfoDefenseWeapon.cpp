//
//  InfoDefenseWeapon.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/13.
//

#include "InfoDefenseWeapon.h"
#include "GameObject/InfoSetting.h"
#include "ManagerGame/DefenseManager.h"


USING_NS_CC;

InfoDefenseWeapon* InfoDefenseWeapon::create()
{
    InfoDefenseWeapon *pRet = new(std::nothrow) InfoDefenseWeapon();
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

InfoDefenseWeapon::InfoDefenseWeapon() :
_isHave(false),
_index(0),
_grade(0),
_maxLv(0),
_lvDFAttackIncrease(0),
_lvUpCost(0),
_maxSmelting(0),
_smeltingBSAttack(0),
_smeltingCost(0),
_weaponType(E_DF_WeaponType::NONE),
_bulletType(0),
_baseDFAttack(0),
_baseDFfAttackSpeed(0),
_baseBSAttak(0),
_currentLv(0),
_currentCount(0),
_currentSmelting(0)
{
}

InfoDefenseWeapon::~InfoDefenseWeapon()
{
}

bool InfoDefenseWeapon::init()
{
    return true;
}

bool InfoDefenseWeapon::isHave() const
{
    return _isHave;
}

void InfoDefenseWeapon::setIsHave(const bool value)
{
    _isHave = value;
}

int InfoDefenseWeapon::getIndex() const
{
    return _index;
}

void InfoDefenseWeapon::setIndex(const int value)
{
    _index = value;
}

int InfoDefenseWeapon::getGrade() const
{
    return _grade;
}

void InfoDefenseWeapon::setGrade(const int value)
{
    _grade = value;
}

int InfoDefenseWeapon::getMaxLv()
{
    return _maxLv;
}

void InfoDefenseWeapon::setMaxLv(const int value)
{
    _maxLv = value;
}

double InfoDefenseWeapon::getDFAttackIncrease()
{
    return _lvDFAttackIncrease;
}

void InfoDefenseWeapon::setDFAttackIncrease(const double value)
{
    _lvDFAttackIncrease = value;
}

int InfoDefenseWeapon::getLvUpCost() const
{
    return _lvUpCost;
}

void InfoDefenseWeapon::setLvUpCost(const int value)
{
    _lvUpCost = value;
}

int InfoDefenseWeapon::getMaxSmelting() const
{
    return _maxSmelting;
}

void InfoDefenseWeapon::setMaxSmelting(const int value)
{
    _maxSmelting = value;
}

double InfoDefenseWeapon::getSmeltingBSAttack()
{
    return _smeltingBSAttack;
}

void InfoDefenseWeapon::setSmeltingBSAttack(const double value)
{
    _smeltingBSAttack = value;
}
int InfoDefenseWeapon::getSmeltingCost()
{
    return _smeltingCost;
}

void InfoDefenseWeapon::setSmeltingCost(const int value)
{
    _smeltingCost = value;
}

E_DF_WeaponType InfoDefenseWeapon::getWeaponType() const
{
    return _weaponType;
}
void InfoDefenseWeapon::setWeaponType(const E_DF_WeaponType value)
{
    _weaponType = value;
}
int InfoDefenseWeapon::getBulletType() const
{
    return _bulletType;
}
void InfoDefenseWeapon::setBulletType(const int value)
{
    _bulletType = value;
}
double InfoDefenseWeapon::getBaseDFAttack()
{
    return _baseDFAttack;
}
void InfoDefenseWeapon::setBaseDFAttack(const double value)
{
    _baseDFAttack = value;
}
double InfoDefenseWeapon::getBaseDFAttackSpeed()
{
    return _baseDFfAttackSpeed;
}
void InfoDefenseWeapon::setBaseDFAttackSpeed(const double value)
{
    _baseDFfAttackSpeed = value;
}
double InfoDefenseWeapon::getBaseBSAttak()
{
    return _baseBSAttak;
}
void InfoDefenseWeapon::setBaseBSAttak(const double value)
{
    _baseBSAttak = value;
}
double InfoDefenseWeapon::getBSAttack(int nSmelting)
{
    double result = _baseBSAttak;
    
    if(nSmelting == -1)
        nSmelting = _currentSmelting;
    
    result += _smeltingBSAttack* nSmelting;
    
    return result;
}
double InfoDefenseWeapon::getDFAttack(int nLevel)
{
    double result = _baseDFAttack;
    
    if(nLevel == -1)
        nLevel = _currentLv;
    
    result += nLevel * _lvDFAttackIncrease;
    
    return result;
}

int InfoDefenseWeapon::getCurrentCount()
{
    return _currentCount;
}
void InfoDefenseWeapon::setCurrentCount(const int value)
{
    _currentCount = value;
}
int InfoDefenseWeapon::getCurrentLv() 
{
    return _currentLv;
}
void InfoDefenseWeapon::setCurrentLv(const int value)
{
    _currentLv = value;
}
int InfoDefenseWeapon::getCurrentSmelting()
{
    return _currentSmelting;
}
void InfoDefenseWeapon::setCurrentSmelting(const int value)
{
    _currentSmelting = value;
}
std::string InfoDefenseWeapon::getResourceIconPath() const
{
    return MafUtils::format("Assets/ui/defense/Icon_gun_lv%d.png", _index);
}
std::string InfoDefenseWeapon::getName() const
{
    return GAME_TEXT(MafUtils::format("t_ui_defense_gun%d", _index));
}

bool InfoDefenseWeapon::isReddotCondition()
{
    if(!_isHave)
        return false;
    
    bool result = !UserDefault::getInstance()->getBoolForKey(MafUtils::format("df_wp_rd_comp_%d", _index).c_str(), false);
    
    if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_promotion"))
    {
        int value = setting->getValueToInt();
        
        if(_index < DefenseManager::getInstance()->getWeaponDataCount())
        {
            if(_currentCount >= value)
                result = true;
        }
    }
    
    return result;
}
void InfoDefenseWeapon::completeFirstReddotCondition()
{
    if(!_isHave)
        return;
    
    UserDefault::getInstance()->setBoolForKey(MafUtils::format("df_wp_rd_comp_%d", _index).c_str(), true);
}
