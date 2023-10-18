//
//  EventSunfishManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/24.
//

#ifndef EventSunfishManager_hpp
#define EventSunfishManager_hpp

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoSetting.h"
#include "GameObject/Event/InfoEventSunfish.h"

class EventSunfishManager : public MafSingleton<EventSunfishManager>
{
public:
    enum class E_TYPE {
        FOOD = 1,
        ADVENTURE = 2,
        LEVEL = 3,
    };
public:
    EventSunfishManager(void);
    virtual ~EventSunfishManager(void);
    virtual bool init() override;
    
public:
    // update
    void update(float dt);
    
    // load
    void setLoad();
    void setLoadSunfish();
    
    // set, get : info
    cocos2d::Vector<InfoEventSunfish*> getListInfoSunfish(E_TYPE eType);
    InfoEventSunfish* getInfoSunfishFood(int idx);
    InfoEventSunfish* getInfoSunfishAdventure(int idx);
    InfoEventSunfish* getInfoSunfishLevel(int level);
    
    //
    int64_t getEndTime();
    bool isExtraTime();
    
    // set, get : game
    int getGameLevelNow();
    int getGameLevelMax();
    
    double getGameWeight();
    void setGameWeight(double value);

    int getGameEnemyCountMax();
    
    int getGamePoint();
    
    // set, get : adventure
    int getAdventureCountNow();
    int getAdventureCountMax();
    int getAdventureTimeNow();
    int getAdventureTimeMax();
    
    // set, get : reinforce
    bool isReinforce(E_TYPE eType, int level);
    bool isReinforce(int type, int level);
    
    // set, get : die
    int getDieTotal();
    int getDie(E_TYPE eType, int level);
    int getDie(int type, int level);
    
    bool isDie(E_TYPE eType, int level);
    bool isDie(int type, int level);
    
    bool isDieReward(E_TYPE eType, int level);
    bool isDieReward(int type, int level);
    
    bool isDieRewardComplete(E_TYPE eType);
    
    //
    bool isReward(E_TYPE eType, int level);
    bool isReward(int type, int level);
    
    // set, get : ad
    int getADNow();
    int getADMax();
    
    // set, get : fever
    int getFeverCoolTime();
    int getFeverCoolAgainTime();
    int getFeverActiveTime();
    int64_t getFeverTime();
    void setFeverTime(int64_t time);
    
    
    // network
    void requestInfo(bool bPopupLoading, const std::function<void(bool, int)>& callback = nullptr);
    void requestWeight(bool bPopupLoading, const std::function<void(bool, int)>& callback = nullptr);
    void requestFoodCheck(bool bPopupLoading, int foodIdx, const std::function<void(bool, int, bool)>& callback = nullptr);
    void requestLevelCheck(bool bPopupLoading, int levelIdx, const std::function<void(bool, int, bool)>& callback = nullptr);
    void requestAdventureUse(bool bPopupLoading, const std::function<void(bool, int, int)>& callback = nullptr);
    void requestAdventureStart(bool bPopupLoading, int adventureIdx, const std::function<void(bool, int, bool)>& callback = nullptr);
    void requestReinforce(bool bPopupLoading, int type, int level, const std::function<void(bool, int)>& callback = nullptr);
    void requestReward(bool bPopupLoading, int type, int level, const std::function<void(bool, int)>& callback = nullptr);
    void requestAD(bool bPopupLoading, const std::function<void(bool, int)>& callback = nullptr);
    
protected:
    // network
    void responseInfo(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data);
    void responseWeight(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data);
    void responseFoodCheck(bool bPopupLoading, const std::function<void(bool, int, bool)>& callback, cocos2d::network::HttpResponse* response,std::string &data);
    void responseLevelCheck(bool bPopupLoading, const std::function<void(bool, int, bool)>& callback, cocos2d::network::HttpResponse* response,std::string &data);
    void responseAdventureUse(bool bPopupLoading, const std::function<void(bool, int, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data);
    void responseAdventureStart(bool bPopupLoading, const std::function<void(bool, int, bool)>& callback, cocos2d::network::HttpResponse* response,std::string &data);
    void responseReinforce(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data);
    void responseReward(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data);
    void responseAD(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data);
    
    void convertJsonInfo(const rapidjson::Value& json);
    
private:
    //
    TInteger _endTime;
    bool _bExtraTime;
    
    double _gameWeight;
    TInteger _gameLevel;
    
    TInteger _die;
    TInteger _point;
    
    TInteger _adventureCount;
    TInteger _adventureTime;
    
    TInteger _adNow;
    
    TInteger _feverTime;
    
    std::vector<int> _listReinforceFood;
    std::vector<int> _listReinforceAdventure;
    std::vector<int> _listReinforceEvolution;
    
    std::map<int, int> _listDieFood;
    std::map<int, int> _listDieAdventure;
    std::map<int, int> _listDieEvolution;
    
    std::vector<int> _listDieFoodReward;
    std::vector<int> _listDieAdventureReward;
    std::vector<int> _listDieEvolutionReward;
    
    std::vector<int> _listFoodReward;
    std::vector<int> _listAdventureReward;
    std::vector<int> _listEvolutionReward;
    
    // info
    cocos2d::Vector<InfoEventSunfish*> _listInfoSunfish;
    
    
public:
    MafDelegate<void(void)> onDelegateGameLevelUp;
    MafDelegate<void(void)> onDelegateGameDie;
    MafDelegate<void(double)> onDelegateGameWeight;
    MafDelegate<void(void)> onDelegateInfoUpdate;
    
};

#endif /* EventSunfishManager_hpp */
