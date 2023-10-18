//
//  EventTreasureHuntManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/24.
//

#ifndef EventTreasureHuntManager_hpp
#define EventTreasureHuntManager_hpp

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoSetting.h"
#include "GameObject/Event/InfoEventTreasureHuntReward.h"
#include "GameObject/Event/InfoEventTreasureHuntMap.h"


class EventTreasureHuntManager : public MafSingleton<EventTreasureHuntManager>
{
public:
    EventTreasureHuntManager(void);
    virtual ~EventTreasureHuntManager(void);
    virtual bool init() override;
    
public:
    // update
    void update(float dt);
    
    // load
    void setLoad();
    void setLoadMap();
    void setLoadReward();
    
    // set, get : info
    cocos2d::Vector<InfoEventTreasureHuntMap*> getListInfoMap();
    InfoEventTreasureHuntMap* getInfoMap(int idx);
    cocos2d::Vector<InfoEventTreasureHuntReward*> getListInfoReward(int type);
    InfoEventTreasureHuntReward* getInfoReward(int type, int level);
    
    // set, get :
    int64_t getEndTime();
    bool isExtraTime();
    
    int getRewardLevel(int type);
    int getRewardValue(int type);
    
    int getADNow();
    int getADMax();
    
    int getCompassNow();
    int getCompassMax();
    bool isCompassUsed();
    
    int getBonusNow();
    int getBonusMax();
    int getBonusRatio();
    bool isBonusEffect();
    bool isBonus(int idx);
    
    bool isBoardOpen(int idx);
    bool isBoardFinish();
    int getBoardPosToIdx(int pos);
    int getBoardIdxToPos(int idx);
    int getBoardLastIdx();
    
    // delegate
    void subDelegateInfo(MafFunction<void(bool, int)>* func);
    void invokeDelegateInfo(bool bResult, int nResult);
    void unSubDelegateInfo(void* obj);
    
    void onEventActivate(int nIdx);

    // network
    void requestInfo(bool bPopupLoading, const std::function<void(bool, int)>& callback = nullptr);
    void requestReset(bool bPopupLoading, const std::function<void(bool, int)>& callback = nullptr);
    void requestCompass(bool bPopupLoading, const std::function<void(bool, int)>& callback = nullptr);
    void requestBoard(bool bPopupLoading, int idx, const std::function<void(bool, int)>& callback = nullptr);
    void requestMissionReward(bool bPopupLoading, int type, int level, const std::function<void(bool, int)>& callback = nullptr);
    void requestAD(bool bPopupLoading, const std::function<void(bool, int)>& callback = nullptr);
    
protected:
    // network
    void responseInfo(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data);
    void responseReset(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data);
    void responseCompass(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data);
    void responseBoard(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data);
    void responseMissionReward(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data);
    void responseAD(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data);
    
    void convertJsonInfo(const rapidjson::Value& json);
    
private:
    //
    bool _isEventActivate;
    TInteger _endTime;
    bool _bExtraTime;
    
    TInteger _rewardSoLevel;
    TInteger _rewardSoValue;
    TInteger _rewardCoLevel;
    TInteger _rewardCoValue;
    
    TInteger _adNow;
    TInteger _compassNow;
    TInteger _compassUsed;
    
    TInteger _boardLastIdx;
    
    std::vector<int> _listBoard;
    std::vector<int> _listBoardReward;
    
    // info
    cocos2d::Vector<InfoEventTreasureHuntMap*> _listInfoMap;
    cocos2d::Vector<InfoEventTreasureHuntReward*> _listInfoReward;
    
    MafDelegate<void(bool, int)> _delegateInfo;
};

#endif /* EventTreasureHuntManager_hpp */
