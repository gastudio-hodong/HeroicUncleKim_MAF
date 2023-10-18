//
//  MineralManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF on 2022/05/12.
//

#ifndef MineralManager_h
#define MineralManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoItems.h"
#include "GameObject/InfoSetting.h"
#include "GameObject/Event/InfoEvent7th.h"
#include "GameObject/Event/InfoEvent7thPerson.h"
#include "GameObject/Mineral/InfoMineralRefineUpgrade.h"

class MineralManager : public MafSingleton<MineralManager>
{
public:
    MineralManager(void);
    virtual ~MineralManager(void);
    virtual bool init();
    
public:
    // update
    void update(float dt);
    
    // data
    void saveData();
    void loadData();
    
    // load
    void setLoad();
    void setLoadRefineSetting();
    
    // get, set : info
    InfoSetting* getInfoRefineSetting(std::string key);
    cocos2d::Vector<InfoMineralRefineUpgrade*> getInfoRefineUpgradeAll();
    InfoMineralRefineUpgrade* getInfoRefineUpgrade(int idx);
    
    // get, set : default
    E_MINERAL getSpecialMineral();
    void setSpecialMineral(E_MINERAL eType);
    
    double getPickaxEffect();
    double getPickaxEffect(E_MINERAL eType);
    
    // get, set : time
    double getTimeMax(E_MINERAL eType);
    double getTimeReal(E_MINERAL eType);
    double getTime(E_MINERAL eType);
    void setTime(E_MINERAL eType, double time);
    
    // get, set : count
    int getCountAdd(E_MINERAL eType);
    int getCount(E_MINERAL eType);
    void setCount(E_MINERAL eType, int count);
    
    
    // get, set : buy
    int getBuyAdd(E_MINERAL eType);
    int getBuyPrice(E_MINERAL eType);
    int getBuyCount(E_MINERAL eType);
    int getBuyCountMax();
    void setBuyCount(E_MINERAL eType, int count);
    
    E_CASH getBuyPurchaseType(E_MINERAL eType);
    
    
    // get, set : refine
    int getRefineOpen();
    void setRefineOpen(int open);
    
    int64_t getRefineBuyResetTime();
    void setRefineBuyResetTime(int64_t time);
    int getRefineBuyCount();
    void setRefineBuyCount(int count);
    int64_t getRefineBuyAdsResetTime();
    void setRefineBuyAdsResetTime(int64_t time);
    int getRefineBuyAdsCount();
    int getRefineBuyAdsCountMax();
    
    int getRefineUpgradeLevel(int idx);
    void setRefineUpgradeLevel(int idx, int level);
    
    double getRefineUpgradeEffect(int idx, int level = 0);
    
    int getRefineProductionAdd();
    int getRefineProductionTimeNeed();
    int64_t getRefineProductionTimeStart();
    void setRefineProductionTimeStart(int64_t time);
    int getRefineProductionCount();
    void setRefineProductionCount(int count);
    int getRefineProductionCountMax();
    int getRefineProductionItem();
    void setRefineProductionItem(int count);
    int getRefineProductionItemMax();
    
    
    
    InfoItems* getRefineOpenCost();
    cocos2d::Vector<InfoItems*> getRefineOpenCostMine();
    
    InfoItems* getRefineBuyCost();
    cocos2d::Vector<InfoItems*> getRefineBuyCostMine();
    
    InfoItems* getRefineUpgradeCost(int idx);
    cocos2d::Vector<InfoItems*> getRefineUpgradeCostMine(int idx);
    
    InfoItems* getRefineProductionCost();
    cocos2d::Vector<InfoItems*> getRefineProductionCostMine();
    
    // event
    int onBuyInit();
    int onBuy(E_MINERAL eType);
    int onRefineOpen();
    int onRefineBuyInit();
    int onRefineBuy();
    int onRefineBuyAdsInit();
    int onRefineBuyAds();
    int onRefineUpgrade(int idx);
    int onRefineProduction();
    int onRefineProductionUpdate();
    int onRefineProductionReward();
    
    
    // network : request
    
protected:
    // network : respons
    
private:
    E_PLACE _ePlace;
    
    //
    E_MINERAL _eMineralSpecial;
    
    TInteger _uMineBuyMineral[C_COUNT_MINE_QUEST+1];
    TInteger _uMineMineralCount[C_COUNT_MINE_MINERAL+1];
    double _uMineTime[C_COUNT_MINE_QUEST+1];
    
    //
    TInteger _uMineRefineOpen;
    TInteger _uMineRefineBuyResetTime;
    TInteger _uMineRefineBuyCount;
    TInteger _uMineRefineBuyAdsResetTime;
    TInteger _uMineRefineBuyAdsCount;
    TInteger _uMineRefineProductionCount;
    TInteger _uMineRefineProductionStartTime;
    TInteger _uMineRefineProductionItem;
    
    std::map<int, int> _listMineRefineUpgradeLevel;
    
    
    // 정보
    std::vector<E_MINERAL> _listMine;
    
    // 정보 : 제련
    cocos2d::Vector<InfoSetting*> _listInfoRefineSetting;
    
    cocos2d::Vector<InfoItems*> _listInfoRefineOpenCost;
    cocos2d::Vector<InfoItems*> _listInfoRefineOpenCostMine;
    
    cocos2d::Vector<InfoItems*> _listInfoRefineBuyCost;
    cocos2d::Vector<InfoItems*> _listInfoRefineBuyCostMine;
    
    cocos2d::Vector<InfoItems*> _listInfoRefineProductionCost;
    cocos2d::Vector<InfoItems*> _listInfoRefineProductionCostMine;
    
    cocos2d::Vector<InfoMineralRefineUpgrade*> _listInfoRefineUpgrade;
    
    
    // callback
    
    // mutex
    std::mutex _mutexMine;
    
    
};


#endif /* MineralManager_hpp */
