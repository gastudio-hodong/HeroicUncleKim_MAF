//
//  InfoServerMission.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/22.
//

#include "InfoServerMission.h"

#include "GameObject/InfoItems.h"

USING_NS_CC;

InfoServerMission::InfoServerMission() :
_isActivate(false)
,_isServerComplete(false)
,_hasCondition(false)
,_isEventMission(false)
,_nSeason(0)
,_nIdx(0)
,_eResetType(E_MISSION_RESET_TYPE::NONE)
,_eShortcut(E_SHORTCUTS::SHORTCUT_NONE)
,_nCurrentCount(0)
,_nCompleteCount(0)
,_nPassPoint(0)
,_strMissionType("")
,_strTitle("")
,_nStartTime(0)
,_nEndTime(0)
{
    _listMissionReward.clear();
    _listEventReward.clear();
}

InfoServerMission::~InfoServerMission()
{
    _listMissionReward.clear();
    _listEventReward.clear();
}

bool InfoServerMission::init()
{
    return true;
}

const bool InfoServerMission::isActivate() const
{
    return _isActivate;
}
void InfoServerMission::setIsActivate(const bool value)
{
    _isActivate = value;
}

const bool InfoServerMission::isComplete() const
{
    return _nCurrentCount >= _nCompleteCount;
}
const bool InfoServerMission::isServerComplete() const
{
    return _isServerComplete;
}
void InfoServerMission::setIsServerComplete(const bool value)
{
    _isServerComplete = value;
}

const bool InfoServerMission::hasCondition() const
{
    return _hasCondition;
}
void InfoServerMission::setHasCondition(const bool value)
{
    _hasCondition = value;
}

const bool InfoServerMission::isEventMission() const
{
    return _isEventMission;
}
void InfoServerMission::setIsEventMission(const bool value)
{
    _isEventMission = value;
}

const int InfoServerMission::getSeason() const
{
    return _nSeason;
}
void InfoServerMission::setSeason(const int value)
{
    _nSeason = value;
}

const int InfoServerMission::getIdx()const
{
    return _nIdx;
}

void InfoServerMission::setIdx(const int nIdx)
{
    _nIdx = nIdx;
}

const int64_t InfoServerMission::getStartTime() const
{
    return _nStartTime;
}

void InfoServerMission::setStartTime(const int64_t nTime)
{
    _nStartTime = nTime;
}

const int64_t InfoServerMission::getEndTime()const
{
    return _nEndTime;
}

void InfoServerMission::setEndTime(const int64_t nTime)
{
    _nEndTime = nTime;
}

const E_MISSION_RESET_TYPE InfoServerMission::getResetType() const
{
    return _eResetType;
}
void InfoServerMission::setResetType(const E_MISSION_RESET_TYPE value)
{
    _eResetType = value;
}

const E_SHORTCUTS InfoServerMission::getShortcut() const
{
    return _eShortcut;
}

void InfoServerMission::setShortcut(const E_SHORTCUTS value)
{
    _eShortcut = value;
}

const int InfoServerMission::getCurrentCount() const
{
    return _nCurrentCount;
}

void InfoServerMission::setCurrentCount(const int value)
{
    _nCurrentCount = value;
}

const int InfoServerMission::getCompleteCount() const
{
    return _nCompleteCount;
}

void InfoServerMission::setCompleteCount(const int value)
{
    _nCompleteCount =value;
}

const std::string InfoServerMission::getMissionTypeByString() const
{
    return _strMissionType;
}

void InfoServerMission::setMissionType(const std::string value)
{
    _strMissionType = value;
}

const E_DAILY_QUEST InfoServerMission::getMissionType() const
{
    return enumFromString<E_DAILY_QUEST>(_strMissionType);
}

const std::string InfoServerMission::getTitle() const
{
    return GAME_TEXT(_strTitle);
}

void InfoServerMission::setTitle(const std::string value)
{
    _strTitle = value;
}

const int InfoServerMission::getPassPoint() const
{
    return _nPassPoint;
}

void InfoServerMission::setPassPoint(const int value)
{
    _nPassPoint = value;
}

Vector<InfoItems*> InfoServerMission::getListMissionReward() const
{
    return _listMissionReward;
}

void InfoServerMission::setListMissionReward(const Vector<InfoItems*> list)
{
    _listMissionReward = list;
}

Vector<InfoItems*> InfoServerMission::getListEventReward() const
{
    
    return _listEventReward;
}

void InfoServerMission::setListEventReward(const Vector<InfoItems*> list)
{
    _listEventReward = list;
}


void InfoServerMission::resetData()
{
    _isActivate = false;
    _isServerComplete = false;
    _nSeason = 0;
    _nCurrentCount = 0;
}
