//
//  InfoEventReturnAttendReward.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/06/20.
//

#include "InfoEventReturnAttendReward.h"

#include "GameObject/InfoItems.h"

USING_NS_CC;

InfoEventReturnAttendReward::InfoEventReturnAttendReward():
_nDay(0)
,_isReward(false)
{
    _listReward.clear();
}

InfoEventReturnAttendReward::~InfoEventReturnAttendReward()
{
    _listReward.clear();
}

bool InfoEventReturnAttendReward::init()
{
    return true;
}

const int InfoEventReturnAttendReward::getDay() const
{
    return _nDay;
}
void InfoEventReturnAttendReward::setDay(const int value)
{
    _nDay = value;
}

const bool InfoEventReturnAttendReward::isReward() const
{
    return _isReward;
}
void InfoEventReturnAttendReward::setIsReward(const bool value)
{
    _isReward = value;
}

const Vector<InfoItems*> InfoEventReturnAttendReward::getListReward() const
{
    return _listReward;
}
void InfoEventReturnAttendReward::setListReward(Vector<InfoItems*> list)
{
    _listReward = list;
}
