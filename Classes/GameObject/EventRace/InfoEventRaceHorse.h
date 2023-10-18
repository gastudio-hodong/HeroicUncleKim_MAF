//
//  InfoEventRaceHorse.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/22.
//

#ifndef InfoEventRaceHorse_hpp
#define InfoEventRaceHorse_hpp

#include "Common/ConfigDefault.h"

class InfoEventRaceStat;
class InfoEventRaceSimulateLog;
class InfoEventRaceHorse : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoEventRaceHorse);
    InfoEventRaceHorse();
    virtual ~InfoEventRaceHorse();
    virtual bool init();
    
public:
    const int getIdx() const;
    void setIdx(const int value);
    
    const int getSkinIdx() const;
    void setSkinIdx(const int value);
    
    const double getInvest();
    void setInvest(const double value);
    
    cocos2d::Vector<InfoEventRaceStat*> getListBasicStat();
    cocos2d::Vector<InfoEventRaceStat*> getListHiddenStat();
    
    InfoEventRaceStat* getInfoStat(const std::string strType);
    void addInfoStat(InfoEventRaceStat* const obj);
    
    const int getHackRank() const;
    void setHackRank(const int nValue);
    
    //race
    const bool isBoost(const int nTurn, const std::string strAbility = "NONE");
    
    const bool isComplateRace();
    
    const int getCurrentRank() const;
    void setCurrentRank(const int nValue);
    
    const float getCurrentSection();
    void setCurrentSection(const float nValue);
    
    //simulate
    const bool isComplateSimulate();
    
    const int getFinalRank();
    void setFinalRank(const int nValue);
    
    const int getCurrentSimulateRank() const;
    void setCurrentSimulateRank(const int nValue);
    
    const double getCurrentSimulateSection();
    void setCurrentSimulateSection(const double nValue);
    
    void addSimulateSectionLog(InfoEventRaceSimulateLog* const objLog);
    InfoEventRaceSimulateLog* getSimulateSectionLog(const int nTick);
    const int getLastSimulateTick();
protected:
    int _nIdx;
    int _nSkinIdx;
    TDouble _nInvest;
    
    TInteger _nFinalRank;
    int _nSimulateRank;
    int _nHackRank;
    
    //race
    int _nCurrentRank;
    TDouble _currentSection;
    
    //simulate
    float _currentSimulateSection;
    
    cocos2d::Vector<InfoEventRaceSimulateLog*> _listSimulateLog;
    
    cocos2d::Vector<InfoEventRaceStat*> _listStat;
    std::vector<bool> _listBoost;
};

#endif /* InfoEventRaceHorse_hpp */
