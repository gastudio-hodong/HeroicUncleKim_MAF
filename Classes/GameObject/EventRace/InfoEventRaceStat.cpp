//
//  InfoEventRaceStat.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/22.
//

#include "InfoEventRaceStat.h"

#pragma mark -
InfoEventRaceStat::InfoEventRaceStat():
_isHidden(false)
,_isAbilityActive(false)
,_nIdx(0)
,_nGrade(0)
,_dValue(0)
,_strTypeName("")
,_strTendency("")
,_strTextKey("")
{
}

InfoEventRaceStat::~InfoEventRaceStat()
{
}

bool InfoEventRaceStat::init()
{
    return true;
}

const bool InfoEventRaceStat::isHidden() const
{
    return _isHidden;
}
void InfoEventRaceStat::setIsHidden(const bool isHidden)
{
    _isHidden = isHidden;
}

const bool InfoEventRaceStat::isAbilityActive() const
{
    return _isAbilityActive;
}
void InfoEventRaceStat::setIsAbilityActive(const bool isHidden)
{
    _isAbilityActive = isHidden;
}

const int InfoEventRaceStat::getIdx() const
{
    return _nIdx;
}
void InfoEventRaceStat::setIdx(const int value)
{
    _nIdx = value;
}

const int InfoEventRaceStat::getGrade()
{
    return _nGrade;
}
void InfoEventRaceStat::setGrade(const int value)
{
    _nGrade = value;
}

const double InfoEventRaceStat::getValue()
{
    return _dValue;
}
void InfoEventRaceStat::setValue(const double value)
{
    _dValue = value;
}

const std::string InfoEventRaceStat::getTypeName() const
{
    return MafAes256::Decrypt(_strTypeName);
}
void InfoEventRaceStat::setTypeName(const std::string value)
{
    _strTypeName = MafAes256::Encrypt(value);
}

const std::string InfoEventRaceStat::getTendency() const
{
    return MafAes256::Decrypt(_strTendency);
}
void InfoEventRaceStat::setTendency(const std::string value)
{
    _strTendency = MafAes256::Encrypt(value);
}

const std::string InfoEventRaceStat::getText()
{
    return GAME_TEXT(_strTextKey);
}
void InfoEventRaceStat::setTextKey(const std::string value)
{
    _strTextKey = value;
}
