//
//  InfoEventVoteAll.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/24.
//

#include "InfoEventVoteAll.h"

#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

InfoEventVoteAll::InfoEventVoteAll() :
_idx(0),
_count(0),
_reward("")
{
}

InfoEventVoteAll::~InfoEventVoteAll()
{
}

bool InfoEventVoteAll::init()
{
    return true;
}
 
const int InfoEventVoteAll::getIdx() const
{
    return _idx;
}

void InfoEventVoteAll::setIdx(int value)
{
    _idx = value;
}

const int InfoEventVoteAll::getCount() const
{
    return _count;
}

void InfoEventVoteAll::setCount(int value)
{
    _count = value;
}
cocos2d::Vector<InfoItems*> InfoEventVoteAll::getReward()
{
    return ItemsManager::getInstance()->getConvertArray(_reward);
}
void InfoEventVoteAll::setReward(const std::string value)
{
    _reward = value;
}
