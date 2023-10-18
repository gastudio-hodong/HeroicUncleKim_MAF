//
//  InfoProduct.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 8. 3..
//

#include "InfoProduct.h"

#include "ManagerGame/CashManager.h"

#include <regex>

USING_NS_CC;

InfoProduct::InfoProduct() :
_nIdx(-1),
_nOrder(-1),
_strNameKey(""),
_strIconName(""),

_nShopGroup(0),
_eShopType(E_PRODUCT::NONE),

_nSaleType(0),
_nSaleTimeStart(0),
_nSaleTimeEnd(0),
_nSaleLimit(0),
_strSaleRatio(""),

_strPrice(""),
_strPriceOriginal(""),
_strPriceCurrencyCode(""),

_strProudctIdReal(""),
_strProductIdWatch(""),

_strVersion("")
{
    
}

InfoProduct::~InfoProduct()
{
    
}

bool InfoProduct::init()
{
    return true;
}


int InfoProduct::getIdx()
{
    return _nIdx;
}

void InfoProduct::setIdx(int idx)
{
    _nIdx = idx;
}

int InfoProduct::getOrder()
{
    return _nOrder;
}

void InfoProduct::setOrder(int value)
{
    _nOrder = value;
}

std::string InfoProduct::getName()
{
    std::string strName = GAME_TEXT(_strNameKey);
    if ( strName.length() == 0 )
    {
        strName = GAME_TEXTFORMAT("t_cash_title_temp", _nIdx);
    }
    return strName;
}

void InfoProduct::setName(std::string name)
{
    _strNameKey = name;
}

std::string InfoProduct::getIconName()
{
    return _strIconName;
}

void InfoProduct::setIconName(std::string name)
{
    _strIconName = name;
}

int InfoProduct::getShopGroup()
{
    return _nShopGroup;
}

void InfoProduct::setShopGroup(int group)
{
    _nShopGroup = group;
}

E_PRODUCT InfoProduct::getShopType()
{
    return _eShopType;
}

void InfoProduct::setShopType(E_PRODUCT eType)
{
    _eShopType = eType;
}

void InfoProduct::setShopType(std::string strType)
{
    E_PRODUCT eType = enumFromString<E_PRODUCT>(strType);
    if ( eType == E_PRODUCT::NONE )
    {
        eType = E_PRODUCT::DEFAULT;
    }
    
    _eShopType = eType;
}

#pragma mark - sale
int InfoProduct::getSaleType()
{
    return _nSaleType;
}

void InfoProduct::setSaleType(int time)
{
    _nSaleType = time;
}

int64_t InfoProduct::getSaleTimeStart()
{
    return _nSaleTimeStart;
}

void InfoProduct::setSaleTimeStart(int64_t time)
{
    _nSaleTimeStart = time;
}

int64_t InfoProduct::getSaleTimeEnd()
{
    return _nSaleTimeEnd;
}

void InfoProduct::setSaleTimeEnd(int64_t time)
{
    _nSaleTimeEnd = time;
}

int InfoProduct::getSaleLimit()
{
    return _nSaleLimit;
}

void InfoProduct::setSaleLimit(int limit)
{
    _nSaleLimit = limit;
}

std::string InfoProduct::getSaleRatio()
{
    return _strSaleRatio;
}

void InfoProduct::setSaleRatio(std::string ratio)
{
    _strSaleRatio = ratio;
}


#pragma mark - price
std::string InfoProduct::getPrice()
{
    return _strPrice;
}

std::string InfoProduct::getPriceOriginal()
{
    return _strPriceOriginal;
}

void InfoProduct::setPrice(std::string strPrice)
{
    _strPrice = strPrice;
    
    std::regex reg("[^0-9.]");
    _strPriceOriginal = std::regex_replace(strPrice, reg, "");
}

std::string InfoProduct::getPriceCurrencyCode()
{
    return _strPriceCurrencyCode;
}

void InfoProduct::setPriceCurrencyCode(std::string strPriceCurrencyCode)
{
    _strPriceCurrencyCode = strPriceCurrencyCode;
}

std::string InfoProduct::getProductIdReal()
{
    return _strProudctIdReal;
}

void InfoProduct::setProductIdReal(std::string strProductId)
{
    _strProudctIdReal = strProductId;
}

std::string InfoProduct::getProductIdWatch()
{
    return _strProductIdWatch;
}

void InfoProduct::setProductIdWatch(std::string strProductIdWatch)
{
    _strProductIdWatch = strProductIdWatch;
}

#pragma mark - other
std::string InfoProduct::getVersion()
{
    return _strVersion;
}

void InfoProduct::setVersion(std::string strVersion)
{
    _strVersion = strVersion;
}

#pragma mark - item
Vector<InfoItems*> InfoProduct::getItems()
{
    return _listItems;
}

void InfoProduct::setItems(std::string strJson)
{
    auto listItem = CashManager::getInstance()->getItemsConvert(strJson);
    
    if ( _listItems.size() != 0 )
    {
        _listItems.clear();
    }
    _listItems.pushBack(listItem);
}

Vector<InfoItems*> InfoProduct::getItemsOther()
{
    return _listItemsOther;
}

void InfoProduct::setItemsOther(std::string strJson)
{
    auto listItem = CashManager::getInstance()->getItemsConvert(strJson);
    
    _listItemsOther.clear();
    _listItemsOther.pushBack(listItem);
}
