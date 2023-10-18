//
//  InfoRaidTradingMarketReward.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/02/15.
//

#include "InfoRaidTradingMarketReward.h"

#include "GameObject/InfoItems.h"

USING_NS_CC;

InfoRaidTradingMarketReward* InfoRaidTradingMarketReward::create()
{
    InfoRaidTradingMarketReward *pRet = new(std::nothrow) InfoRaidTradingMarketReward();
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

InfoRaidTradingMarketReward::InfoRaidTradingMarketReward() :
isActivate(false)
,_eType(InfoRaidTradingMarketReward::E_TYPE::FIXED)
,_nIdx(0)
,_nBuyCount(0)
,_nMaxBuyCount(0)
,_nStartTime(0)
,_nEndTime(0)
,_item(nullptr)
,_itemPrice(nullptr)
{
}

InfoRaidTradingMarketReward::~InfoRaidTradingMarketReward()
{
    if(_item != nullptr)
    {
        _item->release();
        _item = nullptr;
    }
    if(_itemPrice != nullptr)
    {
        _itemPrice->release();
        _itemPrice = nullptr;
    }
    
}

bool InfoRaidTradingMarketReward::init()
{
    return true;
}
void InfoRaidTradingMarketReward::setActivate(bool value)
{
    isActivate = value;
}
InfoRaidTradingMarketReward::E_TYPE InfoRaidTradingMarketReward::getType()
{
    return _eType;
}
void InfoRaidTradingMarketReward::setType(InfoRaidTradingMarketReward::E_TYPE value)
{
    _eType = value;
}

int InfoRaidTradingMarketReward::getIdx()
{
    return _nIdx;
}
void InfoRaidTradingMarketReward::setIdx(int value)
{
    _nIdx = value;
}

int InfoRaidTradingMarketReward::getBuyCount()
{
    return _nBuyCount;
}
void InfoRaidTradingMarketReward::setBuyCount(int value)
{
    _nBuyCount = value;
}

int InfoRaidTradingMarketReward::getMaxBuyCount()
{
    return _nMaxBuyCount;
}
void InfoRaidTradingMarketReward::setMaxBuyCount(int value)
{
    _nMaxBuyCount = value;
}

InfoItems* InfoRaidTradingMarketReward::getItem()
{
    return _item;
}
void InfoRaidTradingMarketReward::setItem(InfoItems* value)
{
    _item = value;
    _item->retain();
}

InfoItems* InfoRaidTradingMarketReward::getPrice()
{
    return _itemPrice;
}
void InfoRaidTradingMarketReward::setPrice(InfoItems* value)
{
    _itemPrice = value;
    _itemPrice->retain();
}

int InfoRaidTradingMarketReward::getStartTime()
{
    return _nStartTime;
}
void InfoRaidTradingMarketReward::setStartTime(int value)
{
    _nStartTime = value;
}

int InfoRaidTradingMarketReward::getEndTime()
{
    return _nEndTime;
}
void InfoRaidTradingMarketReward::setEndTime(int value)
{
    _nEndTime = value;
}

bool InfoRaidTradingMarketReward::isSaleCondition()
{
    bool result = false;
    
    if(!isActivate)
        return result;
        
    switch(_eType)
    {
        case InfoRaidTradingMarketReward::E_TYPE::FIXED :
        {
            auto time = UtilsDate::getInstance()->getTime();
            
            if(time > _nStartTime && time < _nEndTime)
                result = true;
        }break;
        default :
        {
            result = true;
        }break;
    }
    
    return result;
}
