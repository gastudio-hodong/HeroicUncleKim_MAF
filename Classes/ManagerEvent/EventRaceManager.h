//
//  EventHorseRaceManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/18.
//

#ifndef EventHorseRaceManager_hpp
#define EventHorseRaceManager_hpp

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/EventRace/InfoEventRaceHorse.h"
#include "GameObject/EventRace/InfoEventRaceStat.h"

class InfoQuest;
class EventRaceManager : public MafSingleton<EventRaceManager>
{
private:
    friend class MafSingleton;
    
    EventRaceManager(void);
    virtual ~EventRaceManager(void);

public:
    virtual bool init();
    
    //get
    cocos2d::Vector<InfoEventRaceStat*> getListStatByTypeName(const std::string strTypeName);
    cocos2d::Vector<InfoEventRaceHorse*> getListHorse();
    cocos2d::Vector<InfoQuest*> getListMission();
    InfoEventRaceHorse* getInfoHorse(const int nIdx);
    InfoEventRaceHorse* getInfoHorseBySkinIdx(const int nIdx);
    InfoEventRaceHorse* getInfoHorseByCurrentRank(const int nRank);
    InfoEventRaceStat* getInfoRaceStatByIdx(const int nIdx);
    InfoQuest* getInfoMission(const int nIdx);
    
    const bool isGameRunning() const;
    const bool isHiddenAds() const;
    const float getGameRewardMag() const;
    const double getCoin();
    const int getTicket() const;
    const double getInvestCount() const;
    const int getPressureHorseIdx() const;
    int getGameResetCount();
    bool isCompleteMission();
    
    //load table
    void setLoadAll();
    void setLoadStat();
    void setLoadSetting();
    void setLoadMission();
    
    //utils
    std::vector<int> getListRandomSkinIdx();
    const bool isEventEndSeason() const;
    
    //simulate
    static bool sortRank(std::pair<int, float>a, std::pair<int, float> b);
    void setCurrentRank();
    void setCurrentSimulateRank();
    void requestSimulatingRace();
    const bool isSimulateAllComplete() const;
    
    InfoEventRaceStat* getRandomStat(const std::string strTypeName);
    
    //delegate
    void subRaceReadyEvent(MafFunction<void()>* func);
    void unSubRaceReadyEvent(void* obj);
    void invokeRaceReadyEvent();
    
    void subRaceStartEvent(MafFunction<void()>* func);
    void unSubRaceStartEvent(void* obj);
    void invokeRaceStartEvent();
    
    void subRaceGameOverEvent(MafFunction<void(bool)>* func);
    void unSubRaceGameOverEvent(void* obj);
    void invokeRaceGameOverEvent(bool isBankrupt);
    
    void subStatusEvent(MafFunction<void(int)>* func);
    void unSubStatusEvent(void* obj);
    void invokeStatusEvent(const int nIdx);
    
    void subInvestmentEvent(MafFunction<void(int)>* func);
    void unSubInvestmentEvent(void* obj);
    void invokeInvestmentEvent(const int nIdx);
    
    void subAdsEvent(MafFunction<void(int)>* func);
    void unSubAdsEvent(void* obj);
    void invokeAdsEvent(const int nType);
    
    void subEventOverEvent(MafFunction<void()>* func);
    void unSubEventOverEvent(void* obj);
    void invokeEventOverEvent();
    
     void onEventActivate(int nIdx);
    // network : request
    void requestInfo(const std::function<void(bool, int)>& callback);
    void requestReset(const std::function<void(bool, int)>& callback, bool bPurchase);
    void requestAds(const std::function<void(bool, int, int)>& callback, const int nType);
    void requestMission(const std::function<void(bool, int)>& callback);
    void requestMissionReward(const std::function<void(bool, int)>& callback);
    void requestInvestment(const std::function<void(bool, int)>& callback, const int nIdx, const double nBetCount);
    void requestGameStart(const std::function<void(bool, int)>& callback);
private:
    // network : respons
    void responseInfo(cocos2d::network::HttpResponse* const response, const std::string &data);
    void responseReset(cocos2d::network::HttpResponse* const response, const std::string &data);
    void responseAds(cocos2d::network::HttpResponse* const response, const std::string &data, const int nType);
    void responseMission(cocos2d::network::HttpResponse* const response, const std::string &data);
    void responseMissionReward(cocos2d::network::HttpResponse* const response, const std::string &data);
    void responseInvestment(cocos2d::network::HttpResponse* const response, const std::string &data);
    void responseGameStart(cocos2d::network::HttpResponse* const response, const std::string &data);
    
private:
    bool _isEventActivate;
    bool _isGameRunning;
    bool _isHiddenAds;
    float _gameRewardMag;
    bool _isServerDispatch;
    TDouble _nCoin;
    int _nTicket;
    int _nSecondaryCorrection;
    TInteger _gameResetCount;
    cocos2d::Vector<InfoEventRaceHorse*> _listInfoHorse;
    cocos2d::Vector<InfoEventRaceStat*> _listInfoStat;
    cocos2d::Vector<InfoQuest*> _listInfoMission;
    
    MafDelegate<void()> _eventRaceReady;
    MafDelegate<void()> _eventRaceStart;
    MafDelegate<void(int)> _eventInvestment;
    MafDelegate<void(int)> _eventAds;
    MafDelegate<void(bool)> _eventRaceGameOver;
    MafDelegate<void(int)> _eventStatus;
    MafDelegate<void()> _eventEventOver;
    
    std::function<void(bool, int)> _callbackInfo;
    std::function<void(bool, int)> _callbackReset;
    std::function<void(bool, int, int)> _callbackAds;
    std::function<void(bool, int)> _callbackMission;
    std::function<void(bool, int)> _callbackInvestment;
    std::function<void(bool, int)> _callbackGameStart;
};

#endif /* EventHorseRaceManager_hpp */
