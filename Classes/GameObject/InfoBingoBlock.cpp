//
//  InfoBingoBlock.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/07/23.
//

#include "InfoBingoBlock.h"

#include "ManagerEvent/EventBingoManager.h"

USING_NS_CC;

InfoBingoBlock* InfoBingoBlock::create()
{
    InfoBingoBlock *pRet = new(std::nothrow) InfoBingoBlock();
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

InfoBingoBlock::InfoBingoBlock() :
_isReward(false)
,_eType(InfoBingoBlock::E_TYPE::NONE)
,_nIdx(0)
,_nNum(0)
,_nQuestCount(0)
,_item(nullptr)
{
}

InfoBingoBlock::~InfoBingoBlock()
{
}

bool InfoBingoBlock::init()
{
    return true;
}

InfoBingoBlock::E_TYPE InfoBingoBlock::getType()
{
    return _eType;
}
void InfoBingoBlock::setType(InfoBingoBlock::E_TYPE value)
{
    _eType = value;
}

int InfoBingoBlock::getIdx()
{
    return _nIdx;
}
void InfoBingoBlock::setIdx(int value)
{
    _nIdx = value;
}

int InfoBingoBlock::getNum()
{
    return _nNum;
}
void InfoBingoBlock::setNum(int value)
{
    _nNum = value;
}

int InfoBingoBlock::getQuestCount()
{
    return _nQuestCount;
}
void InfoBingoBlock::setQuestCount(int value)
{
    _nQuestCount = value;
}

InfoItems* InfoBingoBlock::getItem()
{
    return _item;
}
void InfoBingoBlock::setItem(InfoItems* value)
{
    _item = value;
}

bool InfoBingoBlock::isReward()
{
    return _isReward;
}
void InfoBingoBlock::setIsReward(bool value)
{
    _isReward = value;
}
