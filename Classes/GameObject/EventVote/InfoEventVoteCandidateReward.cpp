//
//  InfoEventVoteCandidateReward.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/12/09.
//
#include "InfoEventVoteCandidateReward.h"

#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

InfoEventVoteCandidateReward::InfoEventVoteCandidateReward() :
_idx(0)
{
    _mapReward.clear();
}

InfoEventVoteCandidateReward::~InfoEventVoteCandidateReward()
{
    _mapReward.clear();
}

bool InfoEventVoteCandidateReward::init()
{
    return true;
}
 
int InfoEventVoteCandidateReward::getIdx()
{
    return _idx;
}

void InfoEventVoteCandidateReward::setIdx(int value)
{
    _idx = value;
}

InfoItems* InfoEventVoteCandidateReward::getReward(const int idx)
{
    if(auto iter = _mapReward.find(idx); iter != _mapReward.end())
    {
        return ItemsManager::getInstance()->getConvert(iter->second);
    }
    return nullptr;
}
void InfoEventVoteCandidateReward::insertReward(const int idx, const std::string value)
{
    if(auto iter = _mapReward.find(idx); iter == _mapReward.end())
    {
        _mapReward.insert(std::make_pair(idx, value));
    }
}
