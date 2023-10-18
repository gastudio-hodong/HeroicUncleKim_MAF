//
//  InfoDrawRate.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/17.


#include "InfoDrawRate.h"

#pragma mark - info
InfoDrawRate::InfoDrawRate():
_nLevel(0),
_nType(0)
{
    _listRate.clear();
}

InfoDrawRate::~InfoDrawRate()
{
    _listRate.clear();
}

bool InfoDrawRate::init()
{
    return true;
}

const int InfoDrawRate::getType() const
{
    return _nType;
}
void InfoDrawRate::setType(const int value)
{
    _nType = value;
}

const int InfoDrawRate::getLevel() const
{
    return _nLevel;
}
void InfoDrawRate::setLevel(const int value)
{
    _nLevel = value;
}
const std::vector<std::pair<int,int>> InfoDrawRate::getListRate() const
{
    return _listRate;
}
void InfoDrawRate::setListRate(const std::vector<std::pair<int,int>> value)
{
    _listRate = value;
}
