//
//  InfoEventRaceHorse.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/22.
//

#include "InfoEventRaceHorse.h"

#include "GameObject/EventRace/InfoEventRaceStat.h"
#include "GameObject/EventRace/InfoEventRaceSimulateLog.h"

USING_NS_CC;

InfoEventRaceHorse::InfoEventRaceHorse():
_nIdx(0)
,_nSkinIdx(0)
,_nInvest(0)
,_nCurrentRank(0)
,_nHackRank(0)
,_nFinalRank(0)
,_nSimulateRank(0)
,_currentSection(0)
,_currentSimulateSection(0)
{
    _listStat.clear();
    _listSimulateLog.clear();
    _listBoost.clear();
}

InfoEventRaceHorse::~InfoEventRaceHorse()
{
    _listStat.clear();
    _listSimulateLog.clear();
    _listBoost.clear();
}

bool InfoEventRaceHorse::init()
{
    return true;
}

const int InfoEventRaceHorse::getIdx() const
{
    return _nIdx;
}
void InfoEventRaceHorse::setIdx(const int value)
{
    _nIdx = value;
}

const int InfoEventRaceHorse::getSkinIdx() const
{
    return _nSkinIdx;
}
void InfoEventRaceHorse::setSkinIdx(const int value)
{
    _nSkinIdx = value;
}

const double InfoEventRaceHorse::getInvest()
{
    return _nInvest;
}
void InfoEventRaceHorse::setInvest(const double value)
{
    _nInvest = value;
}
Vector<InfoEventRaceStat*> InfoEventRaceHorse::getListBasicStat()
{
    Vector<InfoEventRaceStat*> result;
    result.clear();
    
    for(auto obj : _listStat)
    {
        if(!obj->isHidden())
        {
            result.pushBack(obj);
        }
    }
    
    return result;
}
Vector<InfoEventRaceStat*> InfoEventRaceHorse::getListHiddenStat()
{
    Vector<InfoEventRaceStat*> result;
    result.clear();
    
    for(auto obj : _listStat)
    {
        if(obj->isHidden())
        {
            result.pushBack(obj);
        }
    }
    
    return result;
}
InfoEventRaceStat* InfoEventRaceHorse::getInfoStat(const std::string strType)
{
    InfoEventRaceStat* result = nullptr;
    
    for(auto obj : _listStat)
    {
        if(obj->getTypeName() == strType)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
void InfoEventRaceHorse::addInfoStat(InfoEventRaceStat* const obj)
{
    _listStat.pushBack(obj);
}

const int InfoEventRaceHorse::getHackRank() const
{
    return _nHackRank;
}
void InfoEventRaceHorse::setHackRank(const int value)
{
    _nHackRank = value;
}

#pragma mark -Race
const bool InfoEventRaceHorse::isBoost(const int nTurn, const std::string strAbility)
{
    bool result = false;
    if(_listBoost.size() >= nTurn)
    {
        result = _listBoost[nTurn-1];
    }
    else
    {
        for(int i = 0; i < 3; ++i)
        {
            bool isBoost = false;
            
            int nRand = random(1, 100);
            int nPer = 0;
            if(auto objStat = getInfoStat("WISDOM"))
            {
                nPer = objStat->getValue();
                
                if(strAbility.compare("SMART_PLAYER") == 0)
                    nPer += 10;
                else if(strAbility.compare("HARD_WORKER") == 0 && nTurn == 3)
                    nPer += 50;
                else if(strAbility.compare("SPEED_CONTROL") == 0 && nTurn == 2 && _nSimulateRank == 6)
                    nPer = 100;
            }
            
            if(nRand <= nPer)
                isBoost = true;
            
            _listBoost.push_back(isBoost);
        }
        
        result = _listBoost[nTurn-1];
    }
    
    return result;
}

const bool InfoEventRaceHorse::isComplateRace()
{
    return _currentSection >= 100;
}

const int InfoEventRaceHorse::getCurrentRank() const
{
    return _nCurrentRank;
}
void InfoEventRaceHorse::setCurrentRank(const int value)
{
    _nCurrentRank = value;
}

const float InfoEventRaceHorse::getCurrentSection()
{
    return _currentSection;
}
void InfoEventRaceHorse::setCurrentSection(const float value)
{
    _currentSection = value;
}
#pragma mark -Simulate
const bool InfoEventRaceHorse::isComplateSimulate()
{
    return _currentSimulateSection >= 100;
}

const int InfoEventRaceHorse::getFinalRank()
{
    return _nFinalRank;
}
void InfoEventRaceHorse::setFinalRank(const int value)
{
    _nFinalRank = value;
}

const int InfoEventRaceHorse::getCurrentSimulateRank() const
{
    return _nSimulateRank;
}
void InfoEventRaceHorse::setCurrentSimulateRank(const int value)
{
    _nSimulateRank = value;
}

const double InfoEventRaceHorse::getCurrentSimulateSection()
{
    return _currentSimulateSection;
}
void InfoEventRaceHorse::setCurrentSimulateSection(const double value)
{
    _currentSimulateSection = value;
}

void InfoEventRaceHorse::addSimulateSectionLog(InfoEventRaceSimulateLog* const objLog)
{
    _listSimulateLog.pushBack(objLog);
}

InfoEventRaceSimulateLog* InfoEventRaceHorse::getSimulateSectionLog(const int nTick)
{
    InfoEventRaceSimulateLog* result = nullptr;
    
    for(auto obj : _listSimulateLog)
        if(obj->getTick() == nTick)
            return obj;
    
    return result;
}

const int InfoEventRaceHorse::getLastSimulateTick()
{
    return (int)_listSimulateLog.size();
}
