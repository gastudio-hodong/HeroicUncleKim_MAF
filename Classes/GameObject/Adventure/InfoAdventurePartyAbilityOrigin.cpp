//
//  InfoAdventurePartyAbility.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/14.
//

#include "InfoAdventurePartyAbilityOrigin.h"

#include "ManagerGame/AdventureManager.h"

USING_NS_CC;

#pragma mark -
InfoAdventurePartyAbilityEffect::InfoAdventurePartyAbilityEffect():
_nGrade(0)
,_dEffectMin(0)
,_dEffectMax(0)
{
    
}

InfoAdventurePartyAbilityEffect::~InfoAdventurePartyAbilityEffect()
{
    
}

bool InfoAdventurePartyAbilityEffect::init()
{
    return true;
}

const int InfoAdventurePartyAbilityEffect::getGrade()
{
    return _nGrade.valueInt();
}
void InfoAdventurePartyAbilityEffect::setGrade(const int value)
{
    _nGrade = value;
}

const double InfoAdventurePartyAbilityEffect::getEffectMin()
{
    return _dEffectMin;
}
void InfoAdventurePartyAbilityEffect::setEffectMin(const double value)
{
    _dEffectMin = value;
}

const double InfoAdventurePartyAbilityEffect::getEffectMax()
{
    return _dEffectMax;
}
void InfoAdventurePartyAbilityEffect::setEffectMax(const double value)
{
    _dEffectMax = value;
}

#pragma mark -
InfoAdventurePartyAbilityOrigin::InfoAdventurePartyAbilityOrigin():
_nIdx(0)
,_eType(E_ADVENTURE_PROPERTY_TYPE::NONE)
{
    
}

InfoAdventurePartyAbilityOrigin::~InfoAdventurePartyAbilityOrigin()
{
    
}

bool InfoAdventurePartyAbilityOrigin::init()
{
    return true;
}
 
const int InfoAdventurePartyAbilityOrigin::getIdx() const
{
    return _nIdx;
}
void InfoAdventurePartyAbilityOrigin::setIdx(const int value)
{
    _nIdx = value;
}

const E_ADVENTURE_PROPERTY_TYPE InfoAdventurePartyAbilityOrigin::getType() const
{
    return _eType;
}
void InfoAdventurePartyAbilityOrigin::setType(const E_ADVENTURE_PROPERTY_TYPE value)
{
    _eType = value;
}

InfoAdventurePropertyType* const InfoAdventurePartyAbilityOrigin::getInfoType() const
{
    return AdventureManager::getInstance()->getPropertyType(_eType);
}

const Vector<InfoAdventurePartyAbilityEffect*>& InfoAdventurePartyAbilityOrigin::getListEffect() const
{
    return _listEffect;
}
void InfoAdventurePartyAbilityOrigin::setListEffect(const std::string value)
{
    if ( value.length() == 0 )
    {
        return;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)value.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    if ( jsonParser.Size() == 0 )
    {
        return;
    }
    
    //
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        auto obj = InfoAdventurePartyAbilityEffect::create();

        if ( jsonValue.HasMember("grade") == true )
        {
            obj->setGrade(jsonValue["grade"].GetInt());
        }
        
        if ( jsonValue.HasMember("min") == true )
        {
            obj->setEffectMin(jsonValue["min"].GetDouble());
        }
        
        if ( jsonValue.HasMember("max") == true )
        {
            obj->setEffectMax(jsonValue["max"].GetDouble());
        }
        
        //
        _listEffect.pushBack(obj);
    }
}

InfoAdventurePartyAbilityEffect* const InfoAdventurePartyAbilityOrigin::getInfoEffect(const int nGrade) const
{
    InfoAdventurePartyAbilityEffect* result = nullptr;
    
    for(auto obj : _listEffect)
    {
        if(obj->getGrade() == nGrade)
        {
            result = obj;
            break;
        }
    }
    return result;
}

const double InfoAdventurePartyAbilityOrigin::getEffect(int nLv, const int nGrade) const
{
    double result = 0;
    auto obj = getInfoEffect(nGrade);
    if(obj == nullptr)
        return result;
    
    if(nLv <= 1)
        nLv = 1;
    
    double min = obj->getEffectMin();
    double max = obj->getEffectMax();
    int nLvGrade = nLv / (40/5);
    
    double increase = (max - min) / 5;
    
    result = min + (increase * nLvGrade);
    
    return result;
}

#pragma mark -
InfoAdventurePartyAbility::InfoAdventurePartyAbility():
_nIdx(0)
,_nGrade(0)
,_infoMember(nullptr)
{
    
}

InfoAdventurePartyAbility::~InfoAdventurePartyAbility()
{
    
}

bool InfoAdventurePartyAbility::init()
{
    return true;
}

const int InfoAdventurePartyAbility::getIdx() const
{
    return _nIdx;
}
void InfoAdventurePartyAbility::setIdx(const int value)
{
    _nIdx = value;
}

const E_ADVENTURE_PROPERTY_TYPE InfoAdventurePartyAbility::getType() const
{
    return (E_ADVENTURE_PROPERTY_TYPE)_nIdx;
}

const int InfoAdventurePartyAbility::getGrade() const
{
    return _nGrade;
}
void InfoAdventurePartyAbility::setGrade(const int value)
{
    _nGrade = value;
}
InfoAdventureMember* const InfoAdventurePartyAbility::getMember() const
{
    return _infoMember;
}
void InfoAdventurePartyAbility::setMember(InfoAdventureMember* const value)
{
    _infoMember = value;
}

InfoAdventurePartyAbilityEffect* const InfoAdventurePartyAbility::getInfoEffect() const
{
    InfoAdventurePartyAbilityEffect* result = nullptr;
    auto infoOrigin = AdventureManager::getInstance()->getPartyAbility(_nIdx);
    
    if(infoOrigin == nullptr)
        return result;
    
    result = infoOrigin->getInfoEffect(_nGrade);
    
    return result;
}
InfoAdventurePartyAbilityOrigin* const InfoAdventurePartyAbility::getInfoOrigin() const
{
    return AdventureManager::getInstance()->getPartyAbility(_nIdx);
}
const double InfoAdventurePartyAbility::getEffect(const int nLv) const
{
    double result = 0;
    
    auto infoOrigin = AdventureManager::getInstance()->getPartyAbility(_nIdx);
    
    if(infoOrigin == nullptr)
        return result;
    
    return infoOrigin->getEffect(nLv, _nGrade);
}

const bool InfoAdventurePartyAbility::isOpen() const
{
    bool result = false;
    if(_infoMember == nullptr)
        return result;
    
    result = _infoMember->isOpen();
    
    return result;
}

const Color3B InfoAdventurePartyAbility::getColor() const
{
    Color3B result = Color3B(128,128,128);
    auto objRate = AdventureManager::getInstance()->getInfoPartyAbilityGradeRate(_nGrade);
    
    if(objRate != nullptr)
        result = objRate->getColor();
    
    return result;
}

const bool InfoAdventurePartyAbility::isPartyOption() const
{
    bool result = false;
    
    auto obj = AdventureManager::getInstance()->getPropertyType((E_ADVENTURE_PROPERTY_TYPE)_nIdx);
    if(obj != nullptr)
        result = obj->isPartyOption();
    
    return result;
}

const bool InfoAdventurePartyAbility::isRewardOption() const
{
    bool result = false;
    
    auto obj = AdventureManager::getInstance()->getPropertyType((E_ADVENTURE_PROPERTY_TYPE)_nIdx);
    if(obj != nullptr)
        result = obj->isRewardOption();
    
    return result;
}
