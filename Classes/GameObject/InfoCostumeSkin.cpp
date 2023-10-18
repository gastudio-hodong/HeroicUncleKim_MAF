//
//  InfoCostumeSkin.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 8. 3..
//

#include "InfoCostumeSkin.h"

USING_NS_CC;

#pragma mark - InfoCostumeSkin
InfoCostumeSkin::InfoCostumeSkin() :
_idx(-1),
_sort(0),

_nameKey(""),

_buffType(""),
_buffStrength(0)
{
    
}

InfoCostumeSkin::~InfoCostumeSkin()
{
    
}

bool InfoCostumeSkin::init()
{
    return true;
}


int InfoCostumeSkin::getIdx()
{
    return _idx;
}

void InfoCostumeSkin::setIdx(int value)
{
    _idx = value;
}

int InfoCostumeSkin::getSort()
{
    return _sort;
}
void InfoCostumeSkin::setSort(int value)
{
    _sort = value;
}
 
std::string InfoCostumeSkin::getName()
{
    
    return GAME_TEXT(_nameKey);
}

void InfoCostumeSkin::setNameKey(std::string value)
{
    _nameKey = value;
}

std::string InfoCostumeSkin::getIconPath()
{
    return UtilsString::getInstance()->format("Assets/icon_hero/skin_%d.png", _idx);
}
    
std::string InfoCostumeSkin::getBuffType()
{
    return _buffType;
}

void InfoCostumeSkin::setBuffType(std::string value)
{
    _buffType = value;
}

double InfoCostumeSkin::getBuffStrength()
{
    return _buffStrength;
}

void InfoCostumeSkin::setBuffStrength(double value)
{
    _buffStrength = value;
}

#pragma mark - InfoCostumeSkin
InfoCostumeSkinShop::InfoCostumeSkinShop() :
_idx(-1),
_sort(0),

_commentKey(""),

_skinIdx(0),

_label(0),
_rare(0),

_buyType(""),
_buyCashIdx(0),
_buyItemIdx(0),
_buyItemParams(0),
_buyItemCount(0),

_nEventSaleTimeStart(0),
_nEventSaleTimeEnd(0)
{
    
}

InfoCostumeSkinShop::~InfoCostumeSkinShop()
{
    
}

bool InfoCostumeSkinShop::init()
{
    return true;
}


int InfoCostumeSkinShop::getIdx()
{
    return _idx;
}

void InfoCostumeSkinShop::setIdx(int value)
{
    _idx = value;
}

int InfoCostumeSkinShop::getSort()
{
    return _sort;
}
void InfoCostumeSkinShop::setSort(int value)
{
    _sort = value;
}
 
std::string InfoCostumeSkinShop::getComment()
{
    return GAME_TEXT(_commentKey);
}

void InfoCostumeSkinShop::setCommentKey(std::string value)
{
    _commentKey = value;
}

int InfoCostumeSkinShop::getSkinIdx()
{
    return _skinIdx;
}
void InfoCostumeSkinShop::setSkinIdx(int value)
{
    _skinIdx = value;
}

int InfoCostumeSkinShop::getLabel()
{
    return _label;
}
void InfoCostumeSkinShop::setLabel(int value)
{
    _label = value;
}

int InfoCostumeSkinShop::getRare()
{
    return _rare;
}
void InfoCostumeSkinShop::setRare(int value)
{
    _rare = value;
}

std::string InfoCostumeSkinShop::getBuyType()
{
    return _buyType;
}
void InfoCostumeSkinShop::setBuyType(std::string value)
{
    _buyType = value;
}

int InfoCostumeSkinShop::getBuyCashIdx()
{
    return _buyCashIdx;
}
void InfoCostumeSkinShop::setBuyCashIdx(int value)
{
    _buyCashIdx = value;
}

int InfoCostumeSkinShop::getBuyItemIdx()
{
    return _buyItemIdx;
}
void InfoCostumeSkinShop::setBuyItemIdx(int value)
{
    _buyItemIdx = value;
}

int InfoCostumeSkinShop::getBuyItemParams()
{
    return _buyItemParams;
}
void InfoCostumeSkinShop::setBuyItemParams(int value)
{
    _buyItemParams = value;
}

int InfoCostumeSkinShop::getBuyItemCount()
{
    return _buyItemCount;
}
void InfoCostumeSkinShop::setBuyItemCount(int value)
{
    _buyItemCount = value;
}

bool InfoCostumeSkinShop::isBuySaleTime()
{
    bool bResult = false;
    
    int64_t nTimeNow = UtilsDate::getInstance()->getTime();
    if ( _nEventSaleTimeStart < nTimeNow && _nEventSaleTimeEnd > nTimeNow )
    {
        bResult = true;
    }
    
    return bResult;
}

int64_t InfoCostumeSkinShop::getBuySaleTimeStart()
{
    return _nEventSaleTimeStart;
}
void InfoCostumeSkinShop::setBuySaleTimeStart(int64_t time)
{
    _nEventSaleTimeStart = time;
}

int64_t InfoCostumeSkinShop::getBuySaleTimeEnd()
{
    return _nEventSaleTimeEnd;
}
void InfoCostumeSkinShop::setBuySaleTimeEnd(int64_t time)
{
    _nEventSaleTimeEnd = time;
}
