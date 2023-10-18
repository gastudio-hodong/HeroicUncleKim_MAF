//
//  InfoCostume.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 8. 3..
//

#include "InfoCostume.h"

USING_NS_CC;

InfoCostume::InfoCostume() :
_nIdx(-1),
_nSort(0),

_eType(E_COSTUME::HEAD),
_nTypeNormal(0),
_nTypePrison(0),

_strTitle(""),
_strDesc(""),

_nPrice(0)
{
    
}

InfoCostume::~InfoCostume()
{
    
}

bool InfoCostume::init()
{
    return true;
}

E_COSTUME_IDX InfoCostume::getIdx()
{
    return (E_COSTUME_IDX)_nIdx;
}

void InfoCostume::setIdx(int nIdx)
{
    _nIdx = nIdx;
}

int InfoCostume::getSort()
{
    return _nSort;
}

void InfoCostume::setSort(int nSort)
{
    _nSort = nSort;
}

E_COSTUME InfoCostume::getType()
{
    return _eType;
}
void InfoCostume::setType(E_COSTUME eType)
{
    _eType = eType;
}
void InfoCostume::setType(std::string strType)
{
    if ( strType.compare("body") == 0 )             _eType = E_COSTUME::BODY;
    else if ( strType.compare("head") == 0 )        _eType = E_COSTUME::HEAD;
    else if ( strType.compare("hand") == 0 )        _eType = E_COSTUME::HAND;
    else if ( strType.compare("foot") == 0 )        _eType = E_COSTUME::FOOT;
    else if ( strType.compare("cape") == 0 )        _eType = E_COSTUME::CAPE;
    else if ( strType.compare("jewel") == 0 )       _eType = E_COSTUME::JEWEL;
    else if ( strType.compare("set") == 0 )         _eType = E_COSTUME::SETITEM;
}

bool InfoCostume::isTypeNormal()
{
    bool bResult = false;
    if ( _nTypeNormal == 1 )
    {
        bResult = true;
    }
    
    return bResult;
}
void InfoCostume::setTypeNormal(int nData)
{
    _nTypeNormal = nData;
}

bool InfoCostume::isTypePrison()
{
    bool bResult = false;
    if ( _nTypePrison == 1 )
    {
        bResult = true;
    }
    
    return bResult;
}
void InfoCostume::setTypePrison(int nData)
{
    _nTypePrison = nData;
}

std::string InfoCostume::getTitle()
{
    return GAME_TEXT(_strTitle);
}

void InfoCostume::setTitle(std::string strTitle)
{
    _strTitle = strTitle;
}

std::string InfoCostume::getDesc()
{
    return GAME_TEXT(_strDesc);
}

void InfoCostume::setDesc(std::string strDesc)
{
    _strDesc = strDesc;
}
 
int InfoCostume::getPrice()
{
    return _nPrice;
}
int InfoCostume::getPriceRent()
{
    return _nPrice / 10;
}
void InfoCostume::setPrice(int nPrice)
{
    _nPrice = nPrice;
}

bool InfoCostume::isDips()
{
    std::string strKey = MafUtils::format("dibs_%d_%d", _eType, _nIdx);
    bool bResult = UserDefault::getInstance()->getBoolForKey(strKey.c_str(), false);
    
    return bResult;
}
void InfoCostume::setDips(bool bDips)
{
    std::string strKey = MafUtils::format("dibs_%d_%d", _eType, _nIdx);
    UserDefault::getInstance()->setBoolForKey(strKey.c_str(), bDips);
    
    // log
    if ( bDips == true )
    {
        MafAnalyticsManager::LogEvent(kAnalEventCostumeDips, kRepeatTrue);
    }
}
