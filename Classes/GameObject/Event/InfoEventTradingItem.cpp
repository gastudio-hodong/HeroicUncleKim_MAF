//
//  InfoEventTradingItem.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/01/11.
//

#include "InfoEventTradingItem.h"

#include "GameObject/InfoItems.h"

USING_NS_CC;

InfoEventTradingItem* InfoEventTradingItem::create()
{
    InfoEventTradingItem *pRet = new(std::nothrow) InfoEventTradingItem();
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

InfoEventTradingItem::InfoEventTradingItem() :
_nOrder(0)
,_nIdx(0)
,_nBuyCount(0)
,_nMaxBuyCount(0)
,_strType("")
,_item(nullptr)
,_itemPrice(nullptr)
{
}

InfoEventTradingItem::~InfoEventTradingItem()
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

bool InfoEventTradingItem::init()
{
    return true;
}

int InfoEventTradingItem::getOrder()
{
    return _nOrder;
}
void InfoEventTradingItem::setOrder(int value)
{
    _nOrder = value;
}

int InfoEventTradingItem::getIdx()
{
    return _nIdx;
}
void InfoEventTradingItem::setIdx(int value)
{
    _nIdx = value;
}

int InfoEventTradingItem::getBuyCount()
{
    return _nBuyCount;
}
void InfoEventTradingItem::setBuyCount(int value)
{
    _nBuyCount = value;
}

int InfoEventTradingItem::getMaxBuyCount()
{
    return _nMaxBuyCount;
}
void InfoEventTradingItem::setMaxBuyCount(int value)
{
    _nMaxBuyCount = value;
}

InfoItems* InfoEventTradingItem::getItem()
{
    return _item;
}
void InfoEventTradingItem::setItem(InfoItems* value)
{
    _item = value;
    _item->retain();
}

InfoItems* InfoEventTradingItem::getPrice()
{
    return _itemPrice;
}
void InfoEventTradingItem::setPrice(InfoItems* value)
{
    _itemPrice = value;
    _itemPrice->retain();
}

std::string InfoEventTradingItem::getType()
{
    return _strType;
}
void InfoEventTradingItem::setType(std::string value)
{
    _strType = value;
}
