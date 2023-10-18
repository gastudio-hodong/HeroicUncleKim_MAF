//
//  InfoTradingMarket.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 15/07/2019.
//

#include "InfoTradingMarket.h"

#include "GameObject/InfoItems.h"

#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

InfoTradingMarket::InfoTradingMarket() :
_type(0),
_idx(0),

_countMax(0),

_itemFrom(nullptr),
_itemTo(nullptr),
_openCost(nullptr),

_openStep(0)
{
    
}

InfoTradingMarket::~InfoTradingMarket()
{
    if ( _itemFrom != nullptr )
    {
        _itemFrom->release();
        _itemFrom = nullptr;
    }
    
    if ( _itemTo != nullptr )
    {
        _itemTo->release();
        _itemTo = nullptr;
    }
    
    if ( _openCost != nullptr )
    {
        _openCost->release();
        _openCost = nullptr;
    }
}

bool InfoTradingMarket::init()
{
    return true;
}

int InfoTradingMarket::getType()
{
    return _type;
}
void InfoTradingMarket::setType(int value)
{
    _type = value;
}

int InfoTradingMarket::getIdx()
{
    return _idx;
}

void InfoTradingMarket::setIdx(int value)
{
    _idx = value;
}

int InfoTradingMarket::getCountMax()
{
    return _countMax;
}

void InfoTradingMarket::setCountMax(int value)
{
    _countMax = value;
}

InfoItems* InfoTradingMarket::getItemFrom()
{
    return _itemFrom;
}
void InfoTradingMarket::setItemFrom(std::string value)
{
    if ( _itemFrom != nullptr )
    {
        _itemFrom->release();
        _itemFrom = nullptr;
    }
    
    _itemFrom = ItemsManager::getInstance()->getConvert(value);
    if ( _itemFrom != nullptr )
        _itemFrom->retain();
}

InfoItems* InfoTradingMarket::getItemTo()
{
    return _itemTo;
}
void InfoTradingMarket::setItemTo(std::string value)
{
    if ( _itemTo != nullptr )
    {
        _itemTo->release();
        _itemTo = nullptr;
    }
    
    _itemTo = ItemsManager::getInstance()->getConvert(value);
    if ( _itemTo != nullptr )
        _itemTo->retain();
}

InfoItems* InfoTradingMarket::getOpenCost()
{
    return _openCost;
}

void InfoTradingMarket::setOpenCost(int value)
{
    if ( _openCost != nullptr )
    {
        _openCost->release();
        _openCost = nullptr;
    }
    
    _openCost = ItemsManager::getInstance()->getInfoItems(E_ITEMS::HELL);
    if ( _openCost != nullptr )
    {
        _openCost->setCount(value);
        _openCost->retain();
    }
}
 
int InfoTradingMarket::getOpenStep()
{
    return _openStep;
}
void InfoTradingMarket::setOpenStep(int value)
{
    _openStep = value;
}
