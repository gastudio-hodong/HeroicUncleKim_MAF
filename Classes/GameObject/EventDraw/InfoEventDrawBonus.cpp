//
//  InfoEventDrawBonus.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 14/02/2020.
//

#include "InfoEventDrawBonus.h"

#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

InfoEventDrawBonus::InfoEventDrawBonus() :
_idx(0),
_countMax(0),

_reward(nullptr)
{
    
}

InfoEventDrawBonus::~InfoEventDrawBonus()
{
    CC_SAFE_RELEASE_NULL(_reward);
}

bool InfoEventDrawBonus::init()
{
    return true;
}

int InfoEventDrawBonus::getIdx()
{
    return _idx.valueInt();
}
void InfoEventDrawBonus::setIdx(int value)
{
    _idx = value;
}

int InfoEventDrawBonus::getCountMax()
{
    return _countMax.valueInt();
}
void InfoEventDrawBonus::setCountMax(int value)
{
    _countMax = value;
}

InfoItems* InfoEventDrawBonus::getReward()
{
    return _reward;
}
void InfoEventDrawBonus::setReward(int idx, int params, int count)
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
