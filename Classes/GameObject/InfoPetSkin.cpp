//
//  InfoPetSkin.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 8. 3..
//

#include "InfoPetSkin.h"

USING_NS_CC;

#pragma mark - InfoPetSkin
InfoPetSkin::InfoPetSkin() :
_ePet(E_PET::CHICK_1),
_nIdx(0),
_strNameKey(""),

_nItemIdx(0),
_nItemTypeParams(0),
_strItemCount("0"),

//
_nEventSaleTimeStart(0),
_nEventSaleTimeEnd(0)
{
    
}

InfoPetSkin::~InfoPetSkin()
{
    
}

bool InfoPetSkin::init()
{
    return true;
}

E_PET InfoPetSkin::getPetType()
{
    return _ePet;
}
void InfoPetSkin::setPetType(E_PET ePet)
{
    _ePet = ePet;
}

int InfoPetSkin::getIdx()
{
    return _nIdx;
}

void InfoPetSkin::setIdx(int nIdx)
{
    _nIdx = nIdx;
}

std::string InfoPetSkin::getName()
{
    return GAME_TEXT(_strNameKey);
}
void InfoPetSkin::setNameKey(std::string strNameKey)
{
    _strNameKey = strNameKey;
}

int InfoPetSkin::getItemIdx()
{
    return _nItemIdx;
}
void InfoPetSkin::setItemIdx(int nIdx)
{
    _nItemIdx = nIdx;
}

int InfoPetSkin::getItemTypeParams()
{
    return _nItemTypeParams;
}
void InfoPetSkin::setItemTypeParams(int nTypeParams)
{
    _nItemTypeParams = nTypeParams;
}

std::string InfoPetSkin::getItemCount()
{
    return _strItemCount;
}
void InfoPetSkin::setItemCount(int nCount)
{
    _strItemCount = MafUtils::toString(nCount);
}
void InfoPetSkin::setItemCount(std::string strCount)
{
    _strItemCount = strCount;
}
#pragma mark - event sale
bool InfoPetSkin::isEventSale()
{
    bool bResult = false;
    
    int64_t nTimeNow = UtilsDate::getInstance()->getTime();
    if ( _nEventSaleTimeStart < nTimeNow && _nEventSaleTimeEnd > nTimeNow )
    {
        bResult = true;
    }
    
    return bResult;
}
int64_t InfoPetSkin::getEventSaleTimeStart()
{
    return _nEventSaleTimeStart;
}
void InfoPetSkin::setEventSaleTimeStart(int64_t nTime)
{
    _nEventSaleTimeStart = nTime;
}

int64_t InfoPetSkin::getnEventSaleTimeEnd()
{
    return _nEventSaleTimeEnd;
}
void InfoPetSkin::setEventSaleTimeEnd(int64_t nTime)
{
    _nEventSaleTimeEnd = nTime;
}
