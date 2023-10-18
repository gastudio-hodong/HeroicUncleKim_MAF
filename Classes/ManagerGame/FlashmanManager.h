//
//  FlashmanManager.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#ifndef FlashmanManager_h
#define FlashmanManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoRank.h"
#include "GameObject/InfoSetting.h"
#include "GameObject/InfoQuest.h"

class FlashmanManager : public MafSingleton<FlashmanManager>
{
public:
    FlashmanManager(void);
    virtual ~FlashmanManager(void);
    virtual bool init();
    
public:
    // update
    void update(float dt);
    
    // data
    void saveData();
    void loadData();
    
    // load
    void setLoad();
    void setLoadMission();
    void setLoadRank();
    void setLoadSetting();
    
    // set, get : prison
    cocos2d::Vector<InfoQuest*> getListInfoMission();
    InfoQuest* getInfoMission(int idx);
    
    cocos2d::Vector<InfoRank*> getListInfoRank();
    InfoRank* getInfoRank(int idx);
    
    InfoSetting* getInfoSetting(std::string key);
    
    // set, get :
    int64_t getTimeReset();
    int64_t getTodayScoreReset();
    
    bool isTicket();
    void setTicket(bool bTicket);
    
    
    // set, get : battle
    int getEnterCount();
    int getEnterCountMax();
    
    int getEnterBuyPrice();
    int getEnterBuyAdsCount();
    int getEnterBuyAdsCountMax();
    
    int64_t getScoreTotal();
    int getScoreToday();
    int getScoreBest();
    int getScoreToGrade(int score);
    std::string getScoreToGradeText(int score);
    int getScoreToPoint(int score);
    double getScoreEffect();
    int getScoreAccIdx();
    int getScoreAccTotal();
    InfoQuest* getScoreBonusReward(int idx);
    
    int getMonsterGenerateCount();
    int getMonsterGenerateBossCount();
    int getMonsterGenerateBonusPos();
    int getMonsterBossHp();
    
    int getMonsterKillPoint(int killPerSec);
    int getMonsterKillBonusTime();
    
    int getFeverCondition();
    int getFeverTime();
    
    // set, get : mission
    int getMissionCount(int idx);
    bool getMissionReceive(int idx);
    bool isMissionComplete();
    
    // delegate
    void delegateInfo(MafFunction<void(bool, int)>* func);
    void undelegateInfo(void* obj);
    void delegateResult(MafFunction<void(bool, int, int, bool, std::string)>* func);
    void undelegateResult(void* obj);
    
    // network
    void requestInfo(bool bPopupLoading, const std::function<void(bool, int)>& callback);
    void requestResult(bool bPopupLoading, int score, const std::function<void(bool, int, int, bool, std::string)>& callback = nullptr);
    void requestSkipResult(bool bPopupLoading, int score, const std::function<void(bool, int, int, bool, std::string)>& callback = nullptr);
    void requestBuyTicket(bool bPopupLoading, const std::function<void(bool, int)>& callback);
    void requestBuyTicketAds(bool bPopupLoading, const std::function<void(bool, int)>& callback);
    void requestMissionReward(bool bPopupLoading, int idx, const std::function<void(bool, int)>& callback);
    void requestMissionRewardAll(bool bPopupLoading, const std::function<void(bool, int)>& callback);
    void requestRankReward(bool bPopupLoading, const std::function<void(int, std::string)>& callback);
    void requestAccReward(bool bPopupLoading, const std::function<void(bool, int)>& callback);
    
protected:
    // network
    void responseInfo(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseResult(bool bPopupLoading, int score, cocos2d::network::HttpResponse* response,std::string &data);
    void responseBuyTicket(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseMissionReward(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseRankReward(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseAccReward(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    
    void convertInfoMission(const rapidjson::Value& jsonMission);
    
private:
    //
    cocos2d::Vector<InfoQuest*> _listInfoMission;
    cocos2d::Vector<InfoRank*> _listInfoRank;
    cocos2d::Vector<InfoSetting*> _listInfoSetting;
    
    //
    TInteger _season;
    TInteger _timeReset;
    TInteger _todayScoreReset;
    bool _ticket;
    
    TInteger _scoreTotal;
    TInteger _scoreBest;
    TInteger _scoreToday;
    cocos2d::Vector<InfoQuest*> _listScoreBonusReward;
    
    TInteger _scoreAccTotal;
    TInteger _scoreAccIdx;
    
    // battle
    TInteger _enterCount;
    TInteger _enterBuyCount;
    TInteger _enterBuyAdsCount;
    
    TInteger _monsterBossHPMin;
    TInteger _monsterBossHPMax;
    std::map<std::string, TInteger> _listMonsterKillPoint;
    
    // mission
    std::map<int, TInteger> _listMissionCount;
    std::map<int, bool> _listMissionReceive;
    
    
    //
    std::function<void(bool, int)> _callbackInfo;
    std::function<void(bool, int, int, bool, std::string)> _callbackResult;
    std::function<void(bool, int)> _callbackBuyTicket;
    std::function<void(bool, int)> _callbackMissionReward;
    std::function<void(int, std::string)> _callbackRankReward;
    std::function<void(bool, int)> _callbackAccReward;
    
    MafDelegate<void(bool, int, int, bool, std::string)> _delegateResult;
    
};

#endif /* FlashmanManager_h */
