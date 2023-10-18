//
//  InfoWeaponSpirit.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/27.
//

#include "InfoWeaponSpirit.h"

#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/WeaponSpiritManager.h"
#include "ManagerGame/AdventureRelicManager.h"

USING_NS_CC;

InfoWeaponSpirit::InfoWeaponSpirit():
_isHas(false),
_isWear(false),
_nIdx(0),
_nGrade(0),
_nIdxNextComb(0),
_nCurrentCount(0),
_nCurrentLv(0),
_nLvUpCost(0),
_nCurrentSmelting(0),
_nSmeltingBaseCost(0),

_fEffectDmg(0),
_fEffectGold(0),
_fEffectKey(0)
{
    
}

InfoWeaponSpirit::~InfoWeaponSpirit()
{
    
}

bool InfoWeaponSpirit::init()
{
    return true;
}

bool InfoWeaponSpirit::isHas()
{
    return _isHas;
}
void InfoWeaponSpirit::setHas(bool value)
{
    _isHas = value;
}

bool InfoWeaponSpirit::isWear()
{
    return _isWear;
}
void InfoWeaponSpirit::setWear(bool value)
{
    _isWear = value;
}

int InfoWeaponSpirit::getIdx()
{
    return _nIdx;
}
void InfoWeaponSpirit::setIdx(int value)
{
    _nIdx = value;
}
int InfoWeaponSpirit::getIdxNextComb()
{
    return _nIdxNextComb;
}
void InfoWeaponSpirit::setIdxNextComb(int value)
{
    _nIdxNextComb = value;
}
int InfoWeaponSpirit::getGrade()
{
    return _nGrade;
}
void InfoWeaponSpirit::setGrade(int value)
{
    _nGrade = value;
}

int InfoWeaponSpirit::getCurrentLv()
{
    return _nCurrentLv;
}
void InfoWeaponSpirit::setCurrentLv(int value)
{
    _nCurrentLv = value;
}

int InfoWeaponSpirit::getCurrentCount()
{
    return _nCurrentCount;
}
void InfoWeaponSpirit::setCurrentCount(int value)
{
    _nCurrentCount = value;
}

int InfoWeaponSpirit::getLvUpCost()
{
    return _nLvUpCost;
}
void InfoWeaponSpirit::setLvUpCost(int value)
{
    _nLvUpCost = value;
}

int InfoWeaponSpirit::getCurrentSmelting()
{
    return _nCurrentSmelting;
}
void InfoWeaponSpirit::setCurrentSmelting(int value)
{
    _nCurrentSmelting = value;
}

int InfoWeaponSpirit::getSmeltingCost()
{
    int result = _nSmeltingBaseCost;
    
    //초깃값^(1+0.015*(현재 제련등급)
    
    result = std::pow(result, 1 + 0.015 * _nCurrentSmelting);
    
    return result;
}
void InfoWeaponSpirit::setSmeltingCost(int value)
{
    _nSmeltingBaseCost = value;
}

double InfoWeaponSpirit::getEffectDmg(int nLevel, int nSmelting)
{
    if(nLevel == -1)
        nLevel = _nCurrentLv;
    if(nSmelting == -1)
        nSmelting = _nCurrentSmelting;
    
    //기본값*(1.02+(정령등급/650))^(강화레벨+1))+정령등급*(강화레벨)
    
    double result = _fEffectDmg;
    
    if(nLevel >= 1)
    {
        result = (result * std::pow((1.02 + ((float)_nIdx / (float)650)), (nLevel + 1))) + _nIdx * nLevel;
    }
    if(nSmelting >= 1)
    {
        //round(1.25^(제련 등급*(무기 단계/4)),2)+int(무기 단계*제련등급*/4)*1.5
        double tmp = std::round(std::pow(1.25, nSmelting * (_nIdx / 4)) * 100) / 100;
        
        result *= tmp + (int)(_nIdx * nSmelting / 4) * 1.5 + (nSmelting * 0.5f);
    }
    
    if(_isWear)
    {
        auto objSetting = NewRaidManager::getInstance()->getRaidSetting("sprit_equip_increase");
        result *= objSetting->getValueToFloat();
    }
    result = getCalculationContract(result);
    
    //모험 유물 적용
    double advtRelicBuff = 0.f;
    advtRelicBuff += AdventureRelicManager::getInstance()->getRelicEffectBuffCalculator(E_ADVENTURE_PROPERTY_CALCULATE_TYPE::SPIRIT_ATK);
    double relicBonus = result * advtRelicBuff * 0.01;
    
    result += relicBonus;
    
    return result;
}
void InfoWeaponSpirit::setEffectDmg(double value)
{
    _fEffectDmg = value;
}
double InfoWeaponSpirit::getEffectGold(int nLevel, int nSmelting)
{
    if(nLevel == -1)
        nLevel = _nCurrentLv;
    if(nSmelting == -1)
        nSmelting = _nCurrentSmelting;
    
    double result = _fEffectGold;
    
    if(result <= 0)
        return result;
    
    if(nLevel >= 1)
    {
        result = (result * std::pow((1.02 + ((float)_nIdx / (float)650)), (nLevel + 1))) + _nIdx * nLevel;
        
    }
    if(nSmelting >= 1)
    {
        //round(1.25^(제련 등급*(무기 단계/4)),2)+int(무기 단계*제련등급*/4)*1.5
        double tmp = std::round(std::pow(1.25, nSmelting * (_nIdx / 4)) * 100) / 100;
        
        result *= tmp + (int)(_nIdx * nSmelting / 4) * 1.5 + (nSmelting * 0.5f);
    }
    
    if(_isWear)
    {
        auto objSetting = NewRaidManager::getInstance()->getRaidSetting("sprit_equip_increase");
        result *= objSetting->getValueToFloat();
    }
    result = getCalculationContract(result);
    
    return result;
}
void InfoWeaponSpirit::setEffectGold(double value)
{
    _fEffectGold = value;
}
double InfoWeaponSpirit::getEffectKey(int nLevel, int nSmelting)
{
    
    if(nLevel == -1)
        nLevel = _nCurrentLv;
    if(nSmelting == -1)
        nSmelting = _nCurrentSmelting;
    
    double result = _fEffectKey;
    
    if(result <= 0)
        return result;
    
    if(nLevel >= 1)
    {
        result = (result * std::pow((1.02 + ((float)_nIdx / (float)650)), (nLevel + 1))) + _nIdx * nLevel;
        
    }
    if(nSmelting >= 1)
    {
        //round(1.25^(제련 등급*(무기 단계/4)),2)+int(무기 단계*제련등급*/4)*1.5
        double tmp = std::round(std::pow(1.25, nSmelting * (_nIdx / 4)) * 100) / 100;
        
        result *= tmp + (int)(_nIdx * nSmelting / 4) * 1.5 + (nSmelting * 0.5f);
    }
    if(_isWear)
    {
        auto objSetting = NewRaidManager::getInstance()->getRaidSetting("sprit_equip_increase");
        result *= objSetting->getValueToFloat();
    }
    
    result = getCalculationContract(result);
    
    return result;
}
void InfoWeaponSpirit::setEffectKey(double value)
{
    _fEffectKey = value;
}

double InfoWeaponSpirit::getCalculationContract(double value)
{
    double result = value;
    int nContractLv = WeaponSpiritManager::getInstance()->getContractLv();
    
    if(_nGrade >= 2)
    {
        if(nContractLv < 1)
            result /= 2.5f;
    }
    if(_nGrade >= 3)
    {
        if(nContractLv < 2)
            result /= 4;
    }
    if(_nGrade >= 4)
    {
        if(nContractLv < 3)
            result /= 4.5f;
    }
    if(_nGrade >= 5)
    {
        if(nContractLv < 4)
            result /= 5.5f;
    }
    if(_nGrade >= 6)
    {
        if(nContractLv < 5)
            result /= 9.5f;
    }
    
    return result;
}

double InfoWeaponSpirit::getOriginEffectDmg()
{
    return getCalculationContract(_fEffectDmg);
}

double InfoWeaponSpirit::getOriginEffectGold()
{
    return getCalculationContract(_fEffectGold);
}

double InfoWeaponSpirit::getOriginEffectKey()
{
    return getCalculationContract(_fEffectKey);
}

std::string InfoWeaponSpirit::getText()
{
    return GAME_TEXT("t_spirit_title_" + std::to_string(_nIdx));
}
std::string InfoWeaponSpirit::getResourcePath()
{
    return MafUtils::format("Assets/ui/spirit/spirit_%04d.png", _nIdx);
}
std::string InfoWeaponSpirit::getIconPath()
{
    return MafUtils::format("Assets/ui/spirit/icon_spirit_%04d.png", _nIdx);
}
