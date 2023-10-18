//
//  TownManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF on 2022/05/12.
//

#ifndef TownManager_h
#define TownManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoTownReward.h"

class TownManager : public MafSingleton<TownManager>
{
public:
    TownManager(void);
    virtual ~TownManager(void);
    virtual bool init();
    
public:
    // update
    void update(float dt);
    
    // data
    void saveData();
    void loadData();
    
    // load
    void setLoad();
    
    // get, set : town
    cocos2d::Vector<InfoTownReward*> getListTownReward();
    InfoTownReward* getObjectTownReward(int type, int condition);
    InfoTownReward* getObjectTownRewardLast(int type);
    
    std::string getTownPathIcon(E_TOWN type);
    std::string getTownName(E_TOWN type);
    std::string getTownExplain(E_TOWN type);
     
    double getTownEffect(E_TOWN type, int level);
    
    int getTownLevelMax(E_TOWN type);
    int getTownLevel(E_TOWN type);
    void setTownLevel(E_TOWN type, int level);
    
    std::string getTownPriceCoin(E_TOWN type, int level);
    int getTownPriceGem(E_TOWN type, int level);
    
    // get, set : building
    std::string getBuildingPathFloorTop(E_TOWN type);
    std::string getBuildingPathFloorBottom(E_TOWN type);
    std::string getBuildingPathFloor(E_TOWN type);
    
    std::string getBuildingProgress(E_TOWN type);
    void setBuildingProgress(E_TOWN type, std::string progress);
    
    int getBuildingUpgradeExpectLevel(E_TOWN type, std::string gold);
    int getBuildingUpgradeExpectPercent(E_TOWN type, std::string gold);
    
    // get, set : satellite : 우주왕복선, 인공위성
    int getSatelliteStep();
    void setSatelliteStep(int step);
    
    int getSatelliteTutorial();
    void setSatelliteTutorial(int tutorial);
    
    // event
    int onTownUpgrade(E_TOWN type, std::string gold); //
    int onTownUpgradeTower(); //봉식타워 구입
    int onTownUpgradeSatellite(); //인공위성 구입
    
    // network : request
    void requestInfo(const std::function<void(bool)>& callback);
    void requestReward(int type, int condition, const std::function<void(bool, int)>& callback);
    
protected:
    // network : respons
    void responseInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void responseReward(cocos2d::network::HttpResponse* response,std::string &data);
    
private:
    E_PLACE _ePlace;
    
    /* building */
    cocos2d::Vector<InfoTownReward*> _listTownReward;
    
    TInteger _listTownLevel[C_COUNT_BUILDING+1];
    std::string _listTownProgress[C_COUNT_BUILDING+1];
    
    /* satellite */
    TInteger _nSatelliteStep;
    TInteger _nSatelliteTutorial;  //
    
    
    // callback
    std::function<void(bool)> _callbackInfo;
    std::function<void(bool, int)> _callbackReward;
    
};


#endif /* TownManager_hpp */
