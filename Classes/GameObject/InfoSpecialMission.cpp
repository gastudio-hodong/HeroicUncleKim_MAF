//
//  InfoSpecialMission.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/29.
//

#include "InfoSpecialMission.h"

#include "GameObject/InfoItems.h"

USING_NS_CC;

InfoSpecialMission::InfoSpecialMission():
_isServerReward(false)
,_idx(0)
,_content("")
,_shortcuts(E_SHORTCUTS::SHORTCUT_NONE)
{
    _listMissionReward.clear();
}
InfoSpecialMission::~InfoSpecialMission()
{
    _listMissionReward.clear();
}
bool InfoSpecialMission::init()
{
    return true;
}
const bool InfoSpecialMission::isServerReward()const
{
    return _isServerReward;
}
void InfoSpecialMission::setIsServerReward(const bool value)
{
    _isServerReward = value;
}
const int InfoSpecialMission::getIdx()const
{
    return _idx;
}
void InfoSpecialMission::setIdx(const int value)
{
    _idx = value;
}
const std::string InfoSpecialMission::getContent()const
{
    return GAME_TEXT(_content);
}
void InfoSpecialMission::setContent(const std::string value)
{
    _content = value;
}
const E_SHORTCUTS InfoSpecialMission::getShortcutType()const
{
    return _shortcuts;
}
void InfoSpecialMission::setShortcutType(const E_SHORTCUTS value)
{
    _shortcuts = value;
}

Vector<InfoItems*> InfoSpecialMission::getListMissionReward() const
{
    return _listMissionReward;
}
void InfoSpecialMission::setListMissionReward(const Vector<InfoItems*> list)
{
    _listMissionReward = list;
}
