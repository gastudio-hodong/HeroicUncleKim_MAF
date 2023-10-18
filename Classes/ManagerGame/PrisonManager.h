//
//  PrisonManager.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#ifndef PrisonManager_h
#define PrisonManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoQuest.h"

class PrisonManager : public MafSingleton<PrisonManager>
{
public:
    enum class PrisonType
    {
        DEFAULT = 1,
        FIRE = 2
    };
    
public:
    PrisonManager(void);
    virtual ~PrisonManager(void);
    virtual bool init();
    
public:
    // load
    void setLoadQuestDefault();
    void setLoadQuestHell();
    
    // set, get : prison
    cocos2d::Vector<InfoQuest*> getListQuestDefault();
    cocos2d::Vector<InfoQuest*> getListQuestHell();
    
    PrisonType getPrisonType();
    void setPrisonType(PrisonType type);
    
    int getResetTime();
    double getEvent();
    
    int getHeart();
    void setHeart(int nCount);
    int getHeartMax();
    int64_t getHeartTime();
    
    int getEnterAutoCount();
    void setEnterAutoCount(int nAuto);
    int getEnterAutoCountMax();
    void setEnterAutoCountMax(int nAuto);
    bool isEnterAuto();
    void setEnterAuto(bool bEnable);
    
    bool isDungeonPlay();
    void setDungeonPlay(bool bPlay);
    
    int getDungeonCount();
    int getDungeonKill();
    int getDungeonKillAdd();
    void setDungeonKillAdd(int dungeonKill);
    
    std::string getDungeonHp();
    void setDungeonHp(std::string dungeonHp);
    
    std::string getDungeonEarnKey();
    void setDungeonEarnKey(std::string dungeonEarnKey);
    
    double getDungeonTimeDefault();
    double getDungeonTimeNow();
    void setDungeonTimeNow(double nTime);
    
    int getAuthorityCount();
    
    
    // set, get : ticket
    int getTicketCount();
    void setTicketCount(int ticket);
    double getTicketUseMax();
    double getTicketUseRatio();
    int getTicketUseIdx();
    
    // set, get : bafometz
    int getBafometzCount();
    
    // set, get : ads
    bool isAds();
    
    // set, get : jelly
    bool isJelly();
    bool isGummy();
    void setGummy(int nGummy);
    int getJellyPrice();
    int getJellyLvTime();
    int getJellyLvDouble();
    int getJellyLvExpand();
    bool isJellyRewardMax();
    int getJellyReward();
    int getJellyRewardMax();
    int64_t getJellyRewardTime();
    
    // game
    
    // network
    void subEventInfo(MafFunction<void(bool, bool, int)>* func);
    void invokeEventInfo(bool bResult, bool bAvailable, int nType);
    void unSubEventInfo(void* obj);
    
    void subEventInfoTime(MafFunction<void()>* func);
    void invokeEventInfoTime();
    void unSubEventInfoTime(void* obj);
    
    // network : info
    void requestInfo(bool bPopupLoading, int nType, const std::function<void(bool, bool, int)>& callback);
    void requestInfoTime(int nType);
    
    // network : result
    void requestDefaultResult(bool bPopupLoading, const std::function<void(bool)>& callback);
    void requestDefaultResultSkip(bool bPopupLoading, int nSkip, const std::function<void(bool)>& callback);
    void requestFireResult(bool bPopupLoading, const std::function<void(bool)>& callback);
    
    // network : other
    void requestAds(const std::function<void(bool)>& callback);
    void requestAdsInterstitial(const std::function<void(bool, int)>& callback);
    void requestTicketUse(const std::function<void(bool, int)>& callback);
    void requestJellyInfo(const std::function<void(bool)>& callback);
    
protected:
    // network : prison info
    void responseInfoDefault(cocos2d::network::HttpResponse* response,std::string &data);
    void responseInfoFire(cocos2d::network::HttpResponse* response,std::string &data);
    void responseInfoTime(cocos2d::network::HttpResponse* response,std::string &data);
    
    // network : prison result
    void responseResult(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    
    // network : prison other
    void responseAds(cocos2d::network::HttpResponse* response,std::string &data);
    void responseAdsInterstitial(cocos2d::network::HttpResponse* response,std::string &data);
    void responseTicketUse(cocos2d::network::HttpResponse* response,std::string &data);
    void responseJellyInfo(cocos2d::network::HttpResponse* response,std::string &data);

private:
    PrisonType _ePrisonType;
    
    int64_t _nResetTime;
    double _nEvent;
    
    int _nHeart;
    int _nHeartMax;
    int64_t _nHeartTime;
    int _nEnterAuto;
    int _nEnterAutoMax;
    bool _bEnterAuto;
    
    bool _dungeonPlay;
    int _dungeonKill;
    int _dungeonKillAdd;
    int _dungeonCount;
    double _dungeonTime;
    std::string _dungeonHp;
    std::string _dungeonEarnKey;
    
    // 티켓
    int _nTicketCount;
    double _nTicketUseRatio;
    
    // 바포메트
    int _nBafometzCount;
    
    // 광고
    int _nAdsCount;
    int _nAdsMax;
    
    // 젤리
    int _nJelly;
    int _nGummy;
    int _nJellyPrice;
    int _nJellyLvTime;
    int _nJellyLvDouble;
    int _nJellyLvExpand;
    int _nJellyReward;
    int64_t _nJellyRewardTime;
    
    
    //
    cocos2d::Vector<InfoQuest*> _listQuestDefault;
    cocos2d::Vector<InfoQuest*> _listQuestHell;
    
    //
    MafDelegate<void(bool, bool, int)> _eventInfo;
    MafDelegate<void()> _eventInfoTime;
    
    // bool bResult, bool bAvailable, int nType
    std::function<void(bool, bool, int)> _callbackDungeonInfo;
    std::function<void(bool)> _callbackDungeonResult;
    
    std::function<void(bool)> _callbackAds;
    std::function<void(bool, int)> _callbackAdsInterstitial;
    std::function<void(bool, int)> _callbackTickeetUse;
    std::function<void(bool)> _callbackJellyInfo;
    
    
};

#endif /* PrisonManager_h */
