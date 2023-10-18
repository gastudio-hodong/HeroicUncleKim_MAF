//
//  InfoAdventureLocalUpgradeBase.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/06/10.
//

#include "InfoAdventureLocalUpgradeBase.h"

#include "GameObject/Adventure/InfoAdventureUpgradeAbility.h"
#include "GameObject/Adventure/InfoAdventurePropertyType.h"

USING_NS_CC;

#pragma mark -
InfoAdventureLocalUpgradeBase::InfoAdventureLocalUpgradeBase():
_isDispatch(false)
,_nIdx(0)
,_nCurrentLv(0)
,_nPropertyType(E_ADVENTURE_PROPERTY_TYPE::NONE)
{
    _listProperty.clear();
}

InfoAdventureLocalUpgradeBase::~InfoAdventureLocalUpgradeBase()
{
    _listProperty.clear();
}

bool InfoAdventureLocalUpgradeBase::init()
{
    return true;
}

const int InfoAdventureLocalUpgradeBase::getIdx() const
{
    return _nIdx;
}
void InfoAdventureLocalUpgradeBase::setIdx(const int value)
{
    _nIdx = value;
}

const E_ADVENTURE_PROPERTY_TYPE InfoAdventureLocalUpgradeBase::getType() const
{
    return _nPropertyType;
}
void InfoAdventureLocalUpgradeBase::setType(const E_ADVENTURE_PROPERTY_TYPE value)
{
    _nPropertyType = value;
}

const int InfoAdventureLocalUpgradeBase::getCurrentLv() const
{
    return _nCurrentLv;
}
void InfoAdventureLocalUpgradeBase::setCurrentLv(const int value)
{
    _nCurrentLv = value;
}
const int InfoAdventureLocalUpgradeBase::getMaxLv() const
{
    int result = 0;
    
    for(auto obj : _listProperty)
    {
        if(obj->getLv() > result)
            result = obj->getLv();
    }
    
    return result;
}
const int InfoAdventureLocalUpgradeBase::getImageGrade() const
{
    //override
    return 0;
}

InfoAdventureUpgradeAbility* const InfoAdventureLocalUpgradeBase::getInfoUpgradeAbility(int nLv) const
{
    InfoAdventureUpgradeAbility* result = nullptr;
    
    if(nLv == -1)
        nLv = _nCurrentLv;
    
    for(auto obj : _listProperty)
    {
        if(obj->getLv() == nLv)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
void InfoAdventureLocalUpgradeBase::setInfoUpgradeAbility(const cocos2d::Vector<InfoAdventureUpgradeAbility*> value)
{
    _listProperty = value;
}

InfoAdventurePropertyType* const InfoAdventureLocalUpgradeBase::getInfoType(int nLv) const
{
    InfoAdventurePropertyType* result = nullptr;
    
    const auto objProperty = getInfoUpgradeAbility(nLv);
    
    if(objProperty != nullptr)
        result = objProperty->getInfoType();
    
    return result;
}

const int InfoAdventureLocalUpgradeBase::requestUpgrade()
{
    int result = 0;
    
    const auto objProperty = getInfoUpgradeAbility(_nCurrentLv + 1);
    
    if(objProperty == nullptr)
    {
        //Max 레벨
        result = 3;
        return result;
    }
    
    if(_isDispatch)
    {
        //이미 레벨업 시도중
        result = 2;
        return result;
    }
    
    if(!objProperty->isUpgradeCondition())
    {
        //재화 부족
        result = 5;
        return result;
    }
    
    //레벨업
    objProperty->tryUpgradePayment();
    result = 1;
    setCurrentLv(_nCurrentLv + 1);
    _isDispatch = true;
    
    return result;
}
void InfoAdventureLocalUpgradeBase::completeUpgrade()
{
    if(_isDispatch)
        _isDispatch = false;
}
