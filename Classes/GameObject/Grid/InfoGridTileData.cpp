//
//  InfoGridTileData.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/05.
//

#include "InfoGridTileData.h"

USING_NS_CC;

InfoGridTileData* InfoGridTileData::create()
{
    InfoGridTileData *pRet = new(std::nothrow) InfoGridTileData();
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

InfoGridTileData::InfoGridTileData() :
_index(IntPoint(-999))
,_location(Vec2::ZERO)
{
}

InfoGridTileData::~InfoGridTileData()
{
}

bool InfoGridTileData::init()
{
    return true;
}

IntPoint InfoGridTileData::getIndex() const
{
    return _index;
}
void InfoGridTileData::setIndex(const IntPoint index)
{
    _index = index;
}

Vec2 InfoGridTileData::getLocation() const
{
    return _location;
}
void InfoGridTileData::setLocation(const Vec2 location)
{
    _location = location;
}
void InfoGridTileData::resetData()
{
    _location = Vec2::ZERO;
    _index = IntPoint::ZeroValue;
}
