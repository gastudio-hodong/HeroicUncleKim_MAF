//
//  NewRaidManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/09/29.
//

#ifndef NewRaidManager_h
#define NewRaidManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoQuest.h"
#include "GameObject/InfoBongContentLevel.h"
#include "GameObject/InfoSetting.h"
#include "GameObject/Raid/InfoRaidDragon.h"
#include "GameObject/Raid/InfoRaidBongStat.h"
#include "GameObject/Raid/InfoRaidStatIncrease.h"
#include "GameObject/Raid/InfoRaidTradingMarketReward.h"

class NewRaidManager : public MafSingleton<NewRaidManager>
{
private:
    NewRaidManager(void);
    virtual ~NewRaidManager(void);
    friend class MafSingleton;
public:
    
    virtual bool init();
    
    //load
    void setLoad();
    void setLoadQuest();
    void setLoadDragon();
    void setLoadBongLevel();
    void setLoadBongStat();
    void setLoadBongSetting();
    void setLoadStatIncrease();
    void setLoadTradingMarket();
    
    //get
    cocos2d::Vector<InfoRaidDragon*> getInfoDragonList();
    cocos2d::Vector<InfoBongContentLevel*> getInfoBongLevelList();
    cocos2d::Vector<InfoRaidBongStat*> getInfoBongStatList();
    cocos2d::Vector<InfoQuest*> getQuestList();
    cocos2d::Vector<InfoRaidTradingMarketReward*> getListActivateTradingMarketReward();
    cocos2d::Vector<InfoRaidTradingMarketReward*> getListTradingMarketReward();
    
    InfoBongContentLevel* getInfoBongLevel(int nLv);
    InfoRaidDragon* getNowDragon();
    InfoSetting* getRaidSetting(std::string strKey);
    InfoRaidStatIncrease* getRaidStatIncrease(int nLv);
    InfoRaidBongStat* getStat(int nIdx);
    InfoRaidTradingMarketReward* getRaidTradingMarketReward(int nIdx);
    
    int getSeason();
    double getTotalDamage();
    int getTotalPoint();
    int getAttackCount();
    int getTotalMaxAttackCount();
    int getAdsBuyCount();
    int getBuyCount();
    int getLevel();
    int getMaxLevel();
    int getEXP();
    int getAbilityPoint();
    int getAdsDragonBallCount();
    void setAdsDragonBallCount(int value);
    
    int getDragonBall();
    void setDragonBall(int value);
    
    int getTotalAbilityPoint();
    int getNextAttackTime();
    int getEndSeasonTime();
    int getSeasonBreakTime();
    bool isTicket();
    bool isAttack();
    bool isServerDispatch();
    void setIsAttack(bool value);
    
    bool isMissionComplete();
    bool isLevelUpPossible();
    bool isRaidEnterCondition();
    
    double getActualDamage();
    double getNowDamage();
    
    double getAccrueEffect(E_RAID_ACCRUE_EFFECT_TYPE eType);
    double getAccrueEffectNowSeason(E_RAID_ACCRUE_EFFECT_TYPE eType);
    
    cocos2d::Vector<InfoItems*> getListIntroReward();
    void clearListIntroReward();
    
    int getMarketRefreshCount();
    
    void clearListTradingMarketReward();
    
    //network : request
    void requestInfo(const std::function<void(bool, int)>& callback, bool isIntro = false);
    void requestAttack(const std::function<void(int, bool, bool, cocos2d::Vector<InfoItems*>)>& callback, int count);
    void requestBuyAttackCount(const std::function<void(bool, int)>& callback, int type);
    void requestLevelup(const std::function<void(bool, int, int)>& callback, int type);
    void requestMissionComplete(const std::function<void(bool, int)>& callback, int nIdx);
    void requestMissionAllComplete(const std::function<void(bool, int)>& callback);
    
    void requestBuyDragonBall(const std::function<void(bool, int)>& callback, bool isAds);
    
    void requestTradingMarketInfo(const std::function<void(bool, int)>& callback, bool isRefresh = false);
    void requestTradingMarketBuy(const std::function<void(bool, int)>& callback, int nIdx);
protected:
    // network : respons
    void responseInfo(cocos2d::network::HttpResponse* response,std::string &data, bool isIntro);
    void responseAttack(cocos2d::network::HttpResponse* response,std::string &data);
    void responseBuyAttackCount(cocos2d::network::HttpResponse* response,std::string &data);
    void responseLevelup(cocos2d::network::HttpResponse* response,std::string &data, int nIdx);
    void responseMissionComplete(cocos2d::network::HttpResponse* response,std::string &data);
    void responseMissionAllComplete(cocos2d::network::HttpResponse* response,std::string &data);
    void responseBuyDragonBall(cocos2d::network::HttpResponse* response,std::string &data);
    void responseTradingMarketInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void responseTradingMarketBuy(cocos2d::network::HttpResponse* response,std::string &data);
private:
    
    bool _isAttack;
    bool _isServerDispatch;
    bool _isTicket;
    
    int _nSeason;
    int _nTotalPoint;
    int _nAttackCount;
    int _nAdsBuyCount;
    int _nBuyCount;
    int _nLevel;
    int _nExp;
    int _nAbilityPoint;
    int _nTotalAbilityPoint;
    int _nDragonBall;
    int _nAdsDragonBallCount;
    int _nMarketRefreshCount;
    
    int _nNextAttackTime;
    int _nEndSeasonTime;
    int _nSeasonBreakTime;
    
    double _nTotalDamage;
    double _nNowDamage;
    
    cocos2d::Vector<InfoRaidDragon*> _listInfoDragon;
    cocos2d::Vector<InfoBongContentLevel*> _listInfoBongLevel;
    cocos2d::Vector<InfoRaidBongStat*> _listInfoStat;
    cocos2d::Vector<InfoSetting*> _listInfoSetting;
    cocos2d::Vector<InfoRaidStatIncrease*> _listInfoStatIncrease;
    cocos2d::Vector<InfoRaidTradingMarketReward*> _listInfoTradingMarketReward;
    cocos2d::Vector<InfoQuest*> _listInfoQuest;
    
    cocos2d::Vector<InfoItems*> _listIntroReward;
    
    std::function<void(bool, int)> _callbackInfo;
    std::function<void(bool, int)> _callbackBuyAttackCount;
    std::function<void(bool, int, int)> _callbackLevelup;
    std::function<void(bool, int)> _callbackMission;
    std::function<void(bool, int)> _callbackBuyDragonBall;
    std::function<void(int, bool, bool, cocos2d::Vector<InfoItems*>)> _callbackAttack;
    std::function<void(bool, int)> _callbackTradingMarketInfo;
    std::function<void(bool, int)> _callbackTradingMarketBuy;
    
};

#endif /* NewRaidManager_h */
