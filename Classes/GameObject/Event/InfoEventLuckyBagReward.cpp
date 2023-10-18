//
//  InfoEventLuckyBagReward.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/12/21.
//

#include "InfoEventLuckyBagReward.h"
#include "ManagerGame/ItemsManager.h"

InfoEventLuckyBagReward* InfoEventLuckyBagReward::create()
{
    InfoEventLuckyBagReward *pRet = new(std::nothrow) InfoEventLuckyBagReward();
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

InfoEventLuckyBagReward::InfoEventLuckyBagReward() :
_nIdx(0)
,_nLevel(0)
,_nItemIdx(0)
,_nRate(0)
,_nItemCount(0)
,_nItemParams(0)
{
}

InfoEventLuckyBagReward::~InfoEventLuckyBagReward()
{
}

bool InfoEventLuckyBagReward::init()
{
    return true;
}


const int InfoEventLuckyBagReward::getIdx()const
{
    return _nIdx;
}
void InfoEventLuckyBagReward::setIdx(const int value)
{
    _nIdx = value;
}
const int InfoEventLuckyBagReward::getLevel()const
{
    return _nLevel;
}

void InfoEventLuckyBagReward::setLevel(const int value)
{
    _nLevel = value;
}

const int InfoEventLuckyBagReward::getRate()const
{
    return _nRate;
}

void InfoEventLuckyBagReward::setRate(const int value)
{
    _nRate = value;
}

void InfoEventLuckyBagReward::setItemIdx(const int value)
{
    _nItemIdx = value;
}

void InfoEventLuckyBagReward::setItemCount(const int value)
{
    _nItemCount = value;
}

void InfoEventLuckyBagReward::setItemParam(const int value)
{
    _nItemParams = value;
}

InfoItems* InfoEventLuckyBagReward::getItem()
{
    if(auto objItem = ItemsManager::getInstance()->getInfoItems(_nItemIdx); objItem != nullptr)
    {
        objItem->setTypeParams(_nItemParams);
        objItem->setCount(_nItemCount);
        return objItem;
    }
    
    return nullptr;
}
