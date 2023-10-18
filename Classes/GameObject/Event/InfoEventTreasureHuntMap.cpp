//
//  InfoEventTreasureHuntMap.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/24.
//

#include "InfoEventTreasureHuntMap.h"

#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

InfoEventTreasureHuntMap::InfoEventTreasureHuntMap() :
_idx(0),
_reward("")
{
}

InfoEventTreasureHuntMap::~InfoEventTreasureHuntMap()
{
}

bool InfoEventTreasureHuntMap::init()
{
    return true;
}
 
const int InfoEventTreasureHuntMap::getIdx()
{
    return _idx.valueInt();
}

void InfoEventTreasureHuntMap::setIdx(int value)
{
    _idx = value;
}

cocos2d::Vector<InfoItems*> InfoEventTreasureHuntMap::getReward()
{
    return ItemsManager::getInstance()->getConvertArray(_reward);
}
void InfoEventTreasureHuntMap::setReward(const std::string value)
{
    _reward = value;
}
