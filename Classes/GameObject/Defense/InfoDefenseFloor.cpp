//
//  InfoDefenseFloor.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/20.
//

#include "InfoDefenseFloor.h"

USING_NS_CC;

InfoDefenseFloor* InfoDefenseFloor::create()
{
    InfoDefenseFloor *pRet = new(std::nothrow) InfoDefenseFloor();
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

InfoDefenseFloor::InfoDefenseFloor() :
_index(0),
_mapNumber(0),
_startFloor(0),
_endFloor(0),
_monsterHp(0),
_monsterValue(0),
_10SecMonsterSpawnCount(0),
_normalPer(0),
_runnerIdx(0),
_runnerPer(0),
_tankerIdx(0),
_tankerPer(0),
_bossIdx(0),
_rewardRepeat(""),
_rewardFail("")
{
}

InfoDefenseFloor::~InfoDefenseFloor()
{
}

bool InfoDefenseFloor::init()
{
    return true;
}

int InfoDefenseFloor::getIndex() const
{
    return _index;
}

void InfoDefenseFloor::setIndex(const int value)
{
    _index = value;
}

int InfoDefenseFloor::getMapNumber() const
{
    return _mapNumber;
}
void InfoDefenseFloor::setMapNumber(const int value)
{
    _mapNumber = value;
}

int InfoDefenseFloor::getStartFloor() const
{
    return _startFloor;
}
void InfoDefenseFloor::setStartFloor(const int value)
{
    _startFloor = value;
}

int InfoDefenseFloor::getEndFloor() const
{
    return _endFloor;
}

void InfoDefenseFloor::setEndFloor(const int value)
{
    _endFloor = value;
}

double InfoDefenseFloor::getMonsterHp() const
{
    return _monsterHp;
}

void InfoDefenseFloor::setMonsterHp(const int value)
{
    _monsterHp = value;
}

int InfoDefenseFloor::getMonsterValue() const
{
    return _monsterValue;
}

void InfoDefenseFloor::setMonsterValue(const double value)
{
    _monsterValue = value;
}

int InfoDefenseFloor::get10SecMonsterSpawnCount() const
{
    return _10SecMonsterSpawnCount;
}

void InfoDefenseFloor::set10SecMonsterSpawnCount(const int value)
{
    _10SecMonsterSpawnCount = value;
}

int InfoDefenseFloor::getNormalPer() const
{
    return _normalPer;
}

void InfoDefenseFloor::setNormalPer(const int value)
{
    _normalPer = value;
}

int InfoDefenseFloor::getRunnerIdx() const
{
    return _runnerIdx;
}

void InfoDefenseFloor::setRunnerIdx(const int value)
{
    _runnerIdx = value;
}

int InfoDefenseFloor::getRunnerPer() const
{
    return _runnerPer;
}

void InfoDefenseFloor::setRunnerPer(const int value)
{
    _runnerPer = value;
}

int InfoDefenseFloor::getTankerIdx() const
{
    return _tankerIdx;
}

void InfoDefenseFloor::setTankerIdx(const int value)
{
    _tankerIdx = value;
}

int InfoDefenseFloor::getTankerPer() const
{
    return _tankerPer;
}

void InfoDefenseFloor::setTankerPer(const int value)
{
    _tankerPer = value;
}

int InfoDefenseFloor::getBossIdx() const
{
    return _bossIdx;
}

void InfoDefenseFloor::setBossIdx(const int value)
{
    _bossIdx = value;
}

std::string InfoDefenseFloor::getRewardRepeat() const
{
    return _rewardRepeat;
}

void InfoDefenseFloor::setRewardRepeat(const std::string value)
{
    _rewardRepeat = value;
}

std::string InfoDefenseFloor::getRewardFail() const
{
    return _rewardFail;
}

void InfoDefenseFloor::setRewardFail(const std::string value)
{
    _rewardFail = value;
}

