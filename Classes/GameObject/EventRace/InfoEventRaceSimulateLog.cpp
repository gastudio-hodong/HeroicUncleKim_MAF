//
//  InfoEventRaceSimulateLog.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/03.
//

#include "InfoEventRaceSimulateLog.h"

#pragma mark -
InfoEventRaceSimulateLog::InfoEventRaceSimulateLog():
_isBoost(false)
,_nTick(0)
,_speed(0)
,_stamina(0)
,_curSection(0)
{
}

InfoEventRaceSimulateLog::~InfoEventRaceSimulateLog()
{
}

bool InfoEventRaceSimulateLog::init()
{
    return true;
}

const bool InfoEventRaceSimulateLog::isBoost() const
{
    return _isBoost;
}
void InfoEventRaceSimulateLog::setIsBoost(const bool value)
{
    _isBoost = value;
}

const int InfoEventRaceSimulateLog::getTick() const
{
    return _nTick;
}
void InfoEventRaceSimulateLog::setTick(const int value)
{
    _nTick = value;
}

const float InfoEventRaceSimulateLog::getSpeed()
{
    return _speed;
}
void InfoEventRaceSimulateLog::setSpeed(const float value)
{
    _speed = value;
}

const float InfoEventRaceSimulateLog::getStamina()
{
    return _stamina;
}
void InfoEventRaceSimulateLog::setStamina(const float value)
{
    _stamina = value;
}

const double InfoEventRaceSimulateLog::getSection()
{
    return _curSection;
}
void InfoEventRaceSimulateLog::setSection(const double value)
{
    _curSection = value;
}
