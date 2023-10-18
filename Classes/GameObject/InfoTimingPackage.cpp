//  InfoTimingPackage.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/06/29.
//

#include "InfoTimingPackage.h"

#include "GameObject/InfoItems.h"

USING_NS_CC;

InfoTimingPackage* InfoTimingPackage::create()
{
    InfoTimingPackage *pRet = new(std::nothrow) InfoTimingPackage();
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

InfoTimingPackage::InfoTimingPackage() :
_nIdx(0)
,_eType(E_TIMING_PACKAGE_TYPE::NONE)
,_nCashIdx(0)

,_eStartConditionType(E_TIMING_PACKAGE_CONDITION_TYPE::NONE)
,_eStartConditionTypeParams(0)
,_nStartValue(0)
,_nStartRange(0)

,_eEndConditionType(E_TIMING_PACKAGE_CONDITION_TYPE::NONE)
,_eEndConditionTypeParams(0)
,_nCompleteValue(0)

,_strNameKey("")
,_strDescKey("")
,_strIconPath("")
,_strBannerPath("")

,_nEndTimer(0)
,_isPurchase(false)
,_isOpenPopup(false)
,_isActivate(false)
,_isBtnActive(false)
{
    _listItems.clear();
}

InfoTimingPackage::~InfoTimingPackage()
{
    _listItems.clear();
}

bool InfoTimingPackage::init()
{
    return true;
}

int InfoTimingPackage::getIdx()
{
    return _nIdx;
}
void InfoTimingPackage::setIdx(int value)
{
    _nIdx = value;
}

E_TIMING_PACKAGE_TYPE InfoTimingPackage::getType()
{
    return _eType;
}
void InfoTimingPackage::setType(E_TIMING_PACKAGE_TYPE value)
{
    _eType = value;
}

int InfoTimingPackage::getCashIdx()
{
    return _nCashIdx;
}
void InfoTimingPackage::setCashIdx(int value)
{
    _nCashIdx = value;
}

E_TIMING_PACKAGE_CONDITION_TYPE InfoTimingPackage::getStartConditionType()
{
    return _eStartConditionType;
}
void InfoTimingPackage::setStartConditionType(E_TIMING_PACKAGE_CONDITION_TYPE value)
{
    _eStartConditionType = value;
}

int InfoTimingPackage::getStartConditionTypeParams()
{
    return _eStartConditionTypeParams;
}
void InfoTimingPackage::setStartConditionTypeParams(int value)
{
    _eStartConditionTypeParams = value;
}

int InfoTimingPackage::getStartValue()
{
    return _nStartValue;
}
void InfoTimingPackage::setStartValue(int value)
{
    _nStartValue = value;
}

int InfoTimingPackage::getStartRange()
{
    return _nStartRange;
}
void InfoTimingPackage::setStartRange(int value)
{
    _nStartRange = value;
}

E_TIMING_PACKAGE_CONDITION_TYPE InfoTimingPackage::getEndConditionType()
{
    return _eEndConditionType;
}
void InfoTimingPackage::setEndConditionType(E_TIMING_PACKAGE_CONDITION_TYPE value)
{
    _eEndConditionType = value;
}

int InfoTimingPackage::getEndConditionTypeParams()
{
    return _eEndConditionTypeParams;
}
void InfoTimingPackage::setEndConditionTypeParams(int value)
{
    _eEndConditionTypeParams = value;
}

int InfoTimingPackage::getCompleteValue()
{
    return _nCompleteValue;
}
void InfoTimingPackage::setCompleteValue(int value)
{
    _nCompleteValue = value;
}

std::string InfoTimingPackage::getName()
{
    return GAME_TEXT(_strNameKey);
}
void InfoTimingPackage::setNameKey(std::string value)
{
    _strNameKey = value;
}

std::string InfoTimingPackage::getDescKey()
{
    return _strDescKey;
}
void InfoTimingPackage::setDescKey(std::string value)
{
    _strDescKey = value;
}

std::string InfoTimingPackage::getIconPath()
{
    return _strIconPath;
}
void InfoTimingPackage::setIconPath(std::string value)
{
    _strIconPath = value;
}

std::string InfoTimingPackage::getBannerPath()
{
    return _strBannerPath;
}
void InfoTimingPackage::setBannerPath(std::string value)
{
    _strBannerPath = value;
}

cocos2d::Vector<InfoItems*> InfoTimingPackage::getListItems()
{
    return _listItems;
}
void InfoTimingPackage::setListItems(cocos2d::Vector<InfoItems*> list)
{
    _listItems = list;
}

float InfoTimingPackage::getEndTimer()
{
    return _nEndTimer;
}
void InfoTimingPackage::setEndTimer(float value)
{
    _nEndTimer = value;
}

bool InfoTimingPackage::isPurchase()
{
    return _isPurchase;
}
void InfoTimingPackage::setIsPurchase(bool value)
{
    _isPurchase = value;
}

bool InfoTimingPackage::isActivate()
{
    return _isActivate;
}
void InfoTimingPackage::setIsActivate(bool value)
{
    _isActivate = value;
}

bool InfoTimingPackage::isBtnActive()
{
    return _isBtnActive;
}
void InfoTimingPackage::setIsBtnActive(bool value)
{
    _isBtnActive = value;
}

bool InfoTimingPackage::isOpenPopup()
{
    return _isOpenPopup;
}
void InfoTimingPackage::setIsOpenPopup(bool value)
{
    _isOpenPopup = value;
}

