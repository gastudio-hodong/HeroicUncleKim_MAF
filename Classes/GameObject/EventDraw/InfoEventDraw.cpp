//
//  InfoEventDraw.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 14/02/2020.
//

#include "InfoEventDraw.h"

#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

InfoEventDraw::InfoEventDraw() :
_idx(0),
_type(0),
_countLimit(0),
_rate(0),

_reward(nullptr)
{
    
}

InfoEventDraw::~InfoEventDraw()
{
    CC_SAFE_RELEASE_NULL(_reward);
}

bool InfoEventDraw::init()
{
    return true;
}

int InfoEventDraw::getIdx()
{
    return _idx;
}

void InfoEventDraw::setIdx(int value)
{
    _idx = value;
}

int InfoEventDraw::getType()
{
    return _type;
}
void InfoEventDraw::setType(int value)
{
    _type = value;
}

int InfoEventDraw::getCountLimit()
{
    return _countLimit.valueInt();
}
void InfoEventDraw::setCountLimit(int value)
{
    _countLimit = value;
}

int InfoEventDraw::getRate()
{
    return _rate.valueInt();
}
void InfoEventDraw::setRate(int value)
{
    _rate = value;
}

InfoItems* InfoEventDraw::getReward()
{
    return _reward;
}
void InfoEventDraw::setReward(int idx, int params, int count)
{
    CC_SAFE_RELEASE_NULL(_reward);
    
    _reward = ItemsManager::getInstance()->getInfoItems(idx);
    if ( _reward != nullptr )
    {
        _reward->setTypeParams(params);
        _reward->setCount(count);
        _reward->retain();
    }
}
