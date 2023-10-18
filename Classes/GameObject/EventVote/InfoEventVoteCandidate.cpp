//
//  InfoEventVoteCandidate.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/12/05.
//

#include "InfoEventVoteCandidate.h"

#include "ManagerEvent/EventVoteManager.h"

USING_NS_CC;

InfoEventVoteCandidate::InfoEventVoteCandidate() :
_isPrevCandidate(false)
,_idx(0)
,_nPersnalVoteAllCount(0)
,_nVoteCount(0)
{
}

InfoEventVoteCandidate::~InfoEventVoteCandidate()
{
}

bool InfoEventVoteCandidate::init()
{
    return true;
}

const bool InfoEventVoteCandidate::isPrevCandidate()
{
    return _isPrevCandidate;
}

void InfoEventVoteCandidate::setIsPrevCandidate(const bool value)
{
    _isPrevCandidate = value;
}
 
const int InfoEventVoteCandidate::getIdx()
{
    return _idx;
}

void InfoEventVoteCandidate::setIdx(const int value)
{
    _idx = value;
}

const int InfoEventVoteCandidate::getPersnalVoteAllCount()
{
    return _nPersnalVoteAllCount;
}

void InfoEventVoteCandidate::setPersnalVoteAllCount(const int value)
{
    _nPersnalVoteAllCount = value;
}

const int InfoEventVoteCandidate::getVoteCount()
{
    return _nVoteCount;
}

void InfoEventVoteCandidate::setVoteCount(const int value)
{
    _nVoteCount = value;
}

InfoItems* InfoEventVoteCandidate::getReward()
{
    int nCandidateGroupNum = EventVoteManager::getInstance()->getCurrentCandidateGroupNumber();
    if(_isPrevCandidate)
        nCandidateGroupNum--;
    
    if(auto info = EventVoteManager::getInstance()->getInfoVoteCandidateReward(nCandidateGroupNum); info != nullptr)
    {
        return info->getReward(_idx);
    }
    
    return nullptr;
}
