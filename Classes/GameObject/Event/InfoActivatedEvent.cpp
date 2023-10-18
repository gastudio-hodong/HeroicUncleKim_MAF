//
//  InfoActivatedEvent.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/18.
//

#include "InfoActivatedEvent.h"

USING_NS_CC;

InfoActivatedEvent::InfoActivatedEvent() :
_nIdx(-1),

_nEventStart(0),
_nEventEnd(0),
_nEventRewardDay(0)
{
    
}

InfoActivatedEvent::~InfoActivatedEvent()
{
    
}

bool InfoActivatedEvent::init()
{
    return true;
}


int InfoActivatedEvent::getIdx()
{
    return _nIdx;
}

void InfoActivatedEvent::setIdx(int nIdx)
{
    _nIdx = nIdx;
}

int64_t InfoActivatedEvent::getEventStart()
{
    return _nEventStart;
}

void InfoActivatedEvent::setEventStart(int64_t value)
{
    _nEventStart = value;
}

int64_t InfoActivatedEvent::getEventEnd()
{
    return _nEventEnd;
}

void InfoActivatedEvent::setEventEnd(int64_t value)
{
    _nEventEnd = value;
}

int64_t InfoActivatedEvent::getEventRewardDay()
{
    return 86400 * _nEventRewardDay;
}
void InfoActivatedEvent::setEventRewardDay(int64_t value)
{
    _nEventRewardDay = value;
}
