//
//  InfoDefenseManaAmpSton.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/21.
//

#include "InfoDefenseManaAmpStone.h"

InfoDefenseManaAmpStone* InfoDefenseManaAmpStone::create()
{
    InfoDefenseManaAmpStone *pRet = new(std::nothrow) InfoDefenseManaAmpStone();
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

InfoDefenseManaAmpStone::InfoDefenseManaAmpStone() :
_index(0),
_bonusSuccessRate(0),
_successIncreaseRate(0),
_successDecreaseRate(0),
_failReplaceIdx(0),
_outputQuantity(0)
{
}

InfoDefenseManaAmpStone::~InfoDefenseManaAmpStone()
{
}

bool InfoDefenseManaAmpStone::init()
{
    return true;
}

int InfoDefenseManaAmpStone::getIndex() const
{
    return _index;
}

void InfoDefenseManaAmpStone::setIndex(const int value)
{
    _index = value;
}

E_DF_AMP_STONE_Type InfoDefenseManaAmpStone::getType()
{
    return (E_DF_AMP_STONE_Type)_index;
}

int InfoDefenseManaAmpStone::getBonusSuccessRate() const
{
    return _bonusSuccessRate;
}

void InfoDefenseManaAmpStone::setBonusSuccessRate(const int value)
{
    _bonusSuccessRate = value;
}

int InfoDefenseManaAmpStone::getSuccessIncreaseRate() const
{
    return _failReplaceIdx;
}

void InfoDefenseManaAmpStone::setSuccessIncreaseRate(const int value)
{
    _failReplaceIdx = value;
}

int InfoDefenseManaAmpStone::getSuccessDecreaseRate() const
{
    return _successDecreaseRate;
}

void InfoDefenseManaAmpStone::setSuccessDecreaseRate(const int value)
{
    _successDecreaseRate = value;
}

int InfoDefenseManaAmpStone::getFailReplaceIndex() const
{
    return _successIncreaseRate;
}

void InfoDefenseManaAmpStone::setFailReplaceIndex(const int value)
{
    _successIncreaseRate = value;
}

float InfoDefenseManaAmpStone::getOutputQuantity() const
{
    return _outputQuantity;
}

void InfoDefenseManaAmpStone::setOutputQuantity(const float value)
{
    _outputQuantity = value;
}
