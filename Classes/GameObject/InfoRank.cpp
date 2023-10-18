//
//  InfoRank.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 14/02/2020.
//

#include "InfoRank.h"

#include "ManagerGame/ItemsManager.h"


USING_NS_CC;

InfoRank::InfoRank() :
_nIdx(0),
_nType(0),

_nCondition1(0),
_nCondition2(0)
{
    
}

InfoRank::~InfoRank()
{
    
}

bool InfoRank::init()
{
    return true;
}

int InfoRank::getIdx()
{
    return _nIdx;
}
void InfoRank::setIdx(int idx)
{
    _nIdx = idx;
}

int InfoRank::getType()
{
    return _nType;
}
void InfoRank::setType(int type)
{
    _nType = type;
}

uint64_t InfoRank::getCondition1()
{
    return _nCondition1;
}
void InfoRank::setCondition1(uint64_t condition)
{
    _nCondition1 = condition;
}

uint64_t InfoRank::getCondition2()
{
    return _nCondition2;
}
void InfoRank::setCondition2(uint64_t condition)
{
    _nCondition2 = condition;
}

cocos2d::Vector<InfoItems*> InfoRank::getListReward()
{
    return _listReward;
}
void InfoRank::setReward(std::string reward)
{
    _listReward.clear();
    _listReward = ItemsManager::getInstance()->getConvertArray(reward);
}
