//
//  EventLuckyBagManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/12/21.
//

#ifndef EventLuckyBagManager_hpp
#define EventLuckyBagManager_hpp

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoSetting.h"
#include "GameObject/Event/InfoEventLuckyBagReward.h"

class EventLuckyBagManager : public MafSingleton<EventLuckyBagManager>
{
private:
    EventLuckyBagManager(void);
    virtual ~EventLuckyBagManager(void);
    virtual bool init();
    
    friend class MafSingleton;
public:
    
    // load
    void setLoad();
    void setLoadLuckyBagInfo();
    
    // set, get
    cocos2d::Vector<InfoEventLuckyBagReward*> getListEventLuckyBagRewardAll();
    cocos2d::Vector<InfoEventLuckyBagReward*> getListEventLuckyBagReward(int lv = -1);
    
    const bool isExtraTime() const;
    const bool isEffect() const;
    void setIsEffect(const bool isActive);
    
    const int getCountNow();
    void setCountNow(const int value);
    const int getCountMax();

    const int getCurrentLevel();
    const int getMaxLevel();
    const int getLuckyBagCount();
    const int getCurrentSuccessRate();
    void setLuckyBagCount(const int value);
    const int getNowDrawRewardLv();
    void setNowDrawRewardLv(const int value);
    InfoItems* const getMyRewardByLevel(const int level);
    const int64_t getEndEventTime();
    
    const int getAdsCount();
    const int getAdsMaxCount();
    
    // delegate
    void subDelegateInfo(MafFunction<void(bool, int)>* func);
    void invokeDelegateInfo(bool bResult, int nResult);
    void unSubDelegateInfo(void* obj);
    
    void subDelegateProcess(MafFunction<void(bool, int)>* func);
    void invokeDelegateProcess(bool bResult, int nResult);
    void unSubDelegateProcess(void* obj);
    
    void subDelegateAds(MafFunction<void(bool, int)>* func);
    void invokeDelegateAds(bool bResult, int nResult);
    void unSubDelegateAds(void* obj);
    
     void onEventActivate(int nIdx);
    // network
    void requestInfo(bool bPopupLoading, const std::function<void(bool, int)>& callback = nullptr);
    void requestProceeLuckyBag(bool bPopupLoading, int type, int count, const std::function<void(bool, int)>& callback = nullptr);
    void requestAds(bool bPopupLoading, const std::function<void(bool, int)>& callback = nullptr);
protected:
    // network
    void responseInfo(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseProceeLuckyBag(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseAds(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
private:
    //
    bool _isEventActivate;
    bool _isExtraTime;
    bool _isEffect;
    
    TInteger _nCount;
    TInteger _nLevel;
    TInteger _nLuckyBagCount;
    
    int _nNowDrawRewardLv;
    
    int _nAdsCount;
    int _nAdsMax;
    int64_t _nAdsResetTime;
    
    int64_t _nEndEventTime;
    
    cocos2d::Vector<InfoEventLuckyBagReward*> _listEventLuckyBagReward;
    std::map<int, std::string> _mapMyReward;
    
    MafDelegate<void(bool, int)> _delegateInfo;
    MafDelegate<void(bool, int)> _delegateProcess;
    MafDelegate<void(bool, int)> _delegateAds;
    
    std::function<void(bool, int)> _callbackInfo;
    std::function<void(bool, int)> _callbackProcess;
    std::function<void(bool, int)> _callbackAds;
};


#endif /* EventLuckyBagManager_hpp */
