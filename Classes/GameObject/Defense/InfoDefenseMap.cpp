//
//  InfoDefenseMap.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/06.
//

#include "InfoDefenseMap.h"

USING_NS_CC;

InfoDefenseMap* InfoDefenseMap::create()
{
    InfoDefenseMap *pRet = new(std::nothrow) InfoDefenseMap();
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

InfoDefenseMap::InfoDefenseMap() :
_index(0)
,_imgFilePath("")
{
    _path.clear();
}

InfoDefenseMap::~InfoDefenseMap()
{
    _path.clear();
}

bool InfoDefenseMap::init()
{
    return true;
}

int InfoDefenseMap::getIndex() const
{
    return _index;
}
void InfoDefenseMap::setIndex(const int index)
{
    _index = index;
}

std::vector<IntPoint> InfoDefenseMap::getPath() const
{
    return _path;
}
void InfoDefenseMap::setPath(const std::vector<IntPoint> path)
{
    _path = path;
}

std::string InfoDefenseMap::getImageFilePath()
{
    return "Assets/ui/defense/" + _imgFilePath;
}
void InfoDefenseMap::setImageFilePath(std::string path)
{
    _imgFilePath = path;
}
