//
//  StoneManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF on 2022/05/12.
//

#ifndef StoneManager_h
#define StoneManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/ConfigGameNormal.h"

class StoneManager : public MafSingleton<StoneManager>
{
public:
    StoneManager(void);
    virtual ~StoneManager(void);
    virtual bool init();
    
public:
    // update
    void update(float dt);
    
    // data
    void saveData();
    void loadData();
    
    // load
    void setLoad();
    
    // info
    
    // get, set : stone
    std::string getPath(E_STONE stone);
    std::string getName(E_STONE stone);
    std::string getEffectName(E_STONE stone);
    int getPrice(E_STONE stone, int count);
    cocos2d::Vector<InfoItems*> getCost(E_STONE stone, int count);
    
    double getEffect(E_STONE stone, int count = -1);
    
    int getCountAll();
    int getCountMax(E_STONE stone);
    int getCountNow(E_STONE_TYPE type, E_STONE stone);
    void setCount(E_STONE_TYPE type, E_STONE stone, int count);
    
    // get, set : storage
    std::string getStoragePath();
    std::string getStorageName();
    int getStoragePrice();
    cocos2d::Vector<InfoItems*> getStorageCost(int count);
    
    int getStorageCountNow();
    void setStorageCount(int count);
    
    // get, set : dex
 
    
    // event
    int onBuy(E_STONE stone, int count, const std::function<void(bool, int)>& callback);
    int onStorageBuy(int count);
    
    
    // network
    void requestInfo(bool bPopupLoading, const std::function<void(bool)>& callback = nullptr);
    void requestBuy(bool bPopupLoading, E_STONE eType, int nCount, const std::function<void(bool, int)>& callback = nullptr);
    
protected:
    // network
    void responseInfo(bool bPopupLoading, const std::function<void(bool)>& callback, cocos2d::network::HttpResponse* response, std::string &data);
    void responseBuy(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response, std::string &data);
    
    
private:
    //
    E_PLACE _ePlace;
    
    // soul, dex
    TInteger _uStone[C_COUNT_ENCHANT_STONE+1];
    TInteger _uStoneLocal[C_COUNT_ENCHANT_STONE+1];
    TInteger _uStoneStorage;
    
    
    // callback 
};


#endif /* StoneManager_hpp */
