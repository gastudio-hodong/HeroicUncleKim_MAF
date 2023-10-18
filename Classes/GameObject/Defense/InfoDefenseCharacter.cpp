//
//  InfoDefenseCharacter.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/31.
//

#include "InfoDefenseCharacter.h"
#include "GameObject/InfoSetting.h"
#include "ManagerGame/DefenseManager.h"
USING_NS_CC;

InfoDefenseCharacter* InfoDefenseCharacter::create()
{
    InfoDefenseCharacter *pRet = new(std::nothrow) InfoDefenseCharacter();
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

InfoDefenseCharacter::InfoDefenseCharacter() :
_isHave(false),
_index(0),
_grade(0),
_maxLv(0),
_lvDFAttackIncrease(0),
_lvUpCost(0),
_maxSmelting(0),
_smeltingBSAttack(0),
_smeltingCost(0),
_baseDFAttack(0),
_baseDFfAttackSpeed(0),
_baseBSAttak(0),
_currentCount(0),
_currentLv(0),
_currentSmelting(0)
{
}

InfoDefenseCharacter::~InfoDefenseCharacter()
{
}

bool InfoDefenseCharacter::init()
{
    return true;
}

bool InfoDefenseCharacter::isHave() const
{
    return _isHave;
}

void InfoDefenseCharacter::setIsHave(const bool value)
{
    _isHave = value;
}

int InfoDefenseCharacter::getIndex() const
{
    return _index;
}

void InfoDefenseCharacter::setIndex(const int value)
{
    _index = value;
}

int InfoDefenseCharacter::getGrade() const
{
    return _grade;
}

void InfoDefenseCharacter::setGrade(const int value)
{
    _grade = value;
}

int InfoDefenseCharacter::getMaxLv()
{
    return _maxLv;
}

void InfoDefenseCharacter::setMaxLv(const int value)
{
    _maxLv = value;
}

double InfoDefenseCharacter::getDFAttackIncrease()
{
    return _lvDFAttackIncrease;
}

void InfoDefenseCharacter::setDFAttackIncrease(const double value)
{
    _lvDFAttackIncrease = value;
}

int InfoDefenseCharacter::getLvUpCost() const
{
    return _lvUpCost;
}

void InfoDefenseCharacter::setLvUpCost(const int value)
{
    _lvUpCost = value;
}

int InfoDefenseCharacter::getMaxSmelting() const
{
    return _maxSmelting;
}

void InfoDefenseCharacter::setMaxSmelting(const int value)
{
    _maxSmelting = value;
}

double InfoDefenseCharacter::getSmeltingBSAttack()
{
    return _smeltingBSAttack;
}

void InfoDefenseCharacter::setSmeltingBSAttack(const double value)
{
    _smeltingBSAttack = value;
}

int InfoDefenseCharacter::getSmeltingCost()
{
    return _smeltingCost;
}

void InfoDefenseCharacter::setSmeltingCost(const int value)
{
    _smeltingCost = value;
}

double InfoDefenseCharacter::getBaseDFAttack()
{
    return _baseDFAttack;
}
void InfoDefenseCharacter::setBaseDFAttack(const double value)
{
    _baseDFAttack = value;
}
double InfoDefenseCharacter::getBaseDFAttackSpeed()
{
    return _baseDFfAttackSpeed;
}
void InfoDefenseCharacter::setBaseDFAttackSpeed(const double value)
{
    _baseDFfAttackSpeed = value;
}
double InfoDefenseCharacter::getBaseBSAttak()
{
    return _baseBSAttak;
}
void InfoDefenseCharacter::setBaseBSAttak(const double value)
{
    _baseBSAttak = value;
}
double InfoDefenseCharacter::getBSAttack(int nSmelting)
{
    double result = _baseBSAttak;
    
    if(nSmelting == -1)
        nSmelting = _currentSmelting;
    
    result += _smeltingBSAttack* nSmelting;
    
    return result;
}
double InfoDefenseCharacter::getDFAttack(int nLevel)
{
    double result = _baseDFAttack;
    
    if(nLevel == -1)
        nLevel = _currentLv;
    
    result += nLevel * _lvDFAttackIncrease;
    
    return result;
}
int InfoDefenseCharacter::getCurrentCount()
{
    return _currentCount;
}
void InfoDefenseCharacter::setCurrentCount(const int value)
{
    _currentCount = value;
}
int InfoDefenseCharacter::getCurrentLv()
{
    return _currentLv;
}
void InfoDefenseCharacter::setCurrentLv(const int value)
{
    _currentLv = value;
}
int InfoDefenseCharacter::getCurrentSmelting()
{
    return _currentSmelting;
}
void InfoDefenseCharacter::setCurrentSmelting(const int value)
{
    _currentSmelting = value;
}
std::string InfoDefenseCharacter::getResourcePath() const
{
    return MafUtils::format("Assets/ui/defense/Defense_mole_lv%d_00.png", _index);
}
std::string InfoDefenseCharacter::getName() const
{
    return GAME_TEXT(MafUtils::format("t_ui_defense_character%d", _index));
}

bool InfoDefenseCharacter::isReddotCondition()
{
    if(!_isHave)
        return false;
    
    bool result = !UserDefault::getInstance()->getBoolForKey(MafUtils::format("df_ch_rd_comp_%d", _index).c_str(), false);
    
    if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_promotion"))
    {
        int value = setting->getValueToInt();
        
        if(_index < DefenseManager::getInstance()->getCharacterDataCount())
        {
            if(_currentCount >= value)
                result = true;
        }
    }
    
    return result;
}
void InfoDefenseCharacter::completeFirstReddotCondition()
{
    if(!_isHave)
        return;
    
    UserDefault::getInstance()->setBoolForKey(MafUtils::format("df_ch_rd_comp_%d", _index).c_str(), true);
}
