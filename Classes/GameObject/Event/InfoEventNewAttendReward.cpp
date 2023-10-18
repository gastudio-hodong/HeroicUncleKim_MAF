//
//  InfoEventNewAttendReward.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/03.
//

#include "InfoEventNewAttendReward.h"

#include "GameObject/InfoItems.h"

USING_NS_CC;

InfoEventNewAttendReward::InfoEventNewAttendReward():
_nDay(0)
,_isReward(false)
{
    _listReward.clear();
}

InfoEventNewAttendReward::~InfoEventNewAttendReward()
{
    _listReward.clear();
}

bool InfoEventNewAttendReward::init()
{
    return true;
}

const int InfoEventNewAttendReward::getDay() const
{
    return _nDay;
}
void InfoEventNewAttendReward::setDay(const int value)
{
    _nDay = value;
}

const bool InfoEventNewAttendReward::isReward() const
{
    return _isReward;
}
void InfoEventNewAttendReward::setIsReward(const bool value)
{
    _isReward = value;
}

const Vector<InfoItems*> InfoEventNewAttendReward::getListReward() const
{
    return _listReward;
}
void InfoEventNewAttendReward::setListReward(Vector<InfoItems*> list)
{
    _listReward = list;
}
