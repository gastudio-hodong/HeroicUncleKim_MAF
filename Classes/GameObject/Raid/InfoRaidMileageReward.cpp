//
//  InfoRaidMileageReward.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/27.
//

#include "InfoRaidMileageReward.h"

#include "GameObject/InfoItems.h"

USING_NS_CC;

InfoRaidMileageReward::InfoRaidMileageReward():
_nIdx(0),
_nValue(0)
{
    _listReward.clear();
}

InfoRaidMileageReward::~InfoRaidMileageReward()
{
    _listReward.clear();
}

bool InfoRaidMileageReward::init()
{
    return true;
}
 
int InfoRaidMileageReward::getIdx()
{
    return _nIdx;
}
void InfoRaidMileageReward::setIdx(int value)
{
    _nIdx = value;
}
int InfoRaidMileageReward::getValue()
{
    return _nValue;
}
void InfoRaidMileageReward::setValue(int value)
{
    _nValue = value;
}

cocos2d::Vector<InfoItems*> InfoRaidMileageReward::getListReward()
{
    return _listReward;
}
void InfoRaidMileageReward::setListReward(cocos2d::Vector<InfoItems*> list)
{
    _listReward = list;
}
