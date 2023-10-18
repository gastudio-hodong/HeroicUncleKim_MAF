//
//  InfoTradeItems.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 15/07/2019.
//

#include "InfoTradeItems.h"

USING_NS_CC;

InfoTradeItems::InfoTradeItems() :
_nIdx(0),

_eTradeForType(E_ITEMS::NONE),
_nTradeForCount(0),
_eTradeToType(E_ITEMS::NONE),
_nTradeToCount(0),

_nTradeLimit(0),
_nTradeCount(0),
_nTradeProceedingCount(1)
{
    
}

InfoTradeItems::~InfoTradeItems()
{
    
}

bool InfoTradeItems::init()
{
    return true;
}

int InfoTradeItems::getIdx()
{
    return _nIdx;
}

void InfoTradeItems::setIdx(int nIdx)
{
    _nIdx = nIdx;
}

#pragma mark - for
E_ITEMS InfoTradeItems::getTradeForType()
{
    return _eTradeForType;
}

void InfoTradeItems::setTradeForType(E_ITEMS eType)
{
    _eTradeForType = eType;
}

int InfoTradeItems::getTradeForCount()
{
    return _nTradeForCount;
}

void InfoTradeItems::setTradeForCount(int nCount)
{
    _nTradeForCount = nCount;
}

#pragma mark - for
E_ITEMS InfoTradeItems::getTradeToType()
{
    return _eTradeToType;
}

void InfoTradeItems::setTradeToType(E_ITEMS eType)
{
    _eTradeToType = eType;
}

int InfoTradeItems::getTradeToCount()
{
    return _nTradeToCount;
}

void InfoTradeItems::setTradeToCount(int nCount)
{
    _nTradeToCount = nCount;
}

#pragma mark - otehr
int InfoTradeItems::getTradeLimit()
{
    return _nTradeLimit;
}

void InfoTradeItems::setTradeLimit(int nCount)
{
    _nTradeLimit = nCount;
}

int InfoTradeItems::getTradeCount()
{
    return _nTradeCount;
}

void InfoTradeItems::setTradeCount(int nLimit)
{
    _nTradeCount = nLimit;
}

int InfoTradeItems::getTradeProceedingCount()
{
    return _nTradeProceedingCount;
}

void InfoTradeItems::setTradeProceedingCount(int nCount)
{
    _nTradeProceedingCount = nCount;
}
