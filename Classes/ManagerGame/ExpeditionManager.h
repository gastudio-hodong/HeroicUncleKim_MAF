//
//  ExpeditionManager.h
//  FantasyClicker-mobile
//
//  Created by Hyun Jin on 29/03/2020.
//

#ifndef ExpeditionManager_h
#define ExpeditionManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoItems.h"

class ExpeditionManager : public MafSingleton<ExpeditionManager>
{
public:
    ExpeditionManager(void);
    virtual ~ExpeditionManager(void);
    virtual bool init();
    
public:
    //Load data
    
    // set, get : Expedition default

    // set, get : Expedition draw
    cocos2d::Vector<InfoItems*> getDrawRewardAll();
    cocos2d::Vector<InfoItems*> getDrawRewardBonusAll();
    int64_t getDrawEndTime();
    int getDrawSwapFriendPoint();
    bool isDrawBounus();
    
    // network : Expedition draw
    void requestDrawInfo(const std::function<void(bool, int)>& pCallback);
    void responseDrawInfo(cocos2d::network::HttpResponse* response,std::string &data);
    
    void requestDrawUpdate(const std::function<void(bool, int)>& pCallback, std::string strType, int nIdx);
    void responseDrawUpdate(cocos2d::network::HttpResponse* response,std::string &data);
    
    void requestDrawBonus(const std::function<void(bool, int)>& pCallback, int nIdx);
    void responseDrawBonus(cocos2d::network::HttpResponse* response,std::string &data);
    
    
    
private:
    // expedition
    
    //cat

    // draw
    int _nDrawSeason;
    int64_t _nDrawEndTime;
    int _nSwapFriendPoint;
    bool _bDrawBonus;
    cocos2d::Vector<InfoItems*> _listDrawReward;
    cocos2d::Vector<InfoItems*> _listDrawRewardBonus;
     
    //
    std::function<void(bool, int)> _callbackDrawInfo;
    std::function<void(bool, int)> _callbackDraw;
    std::function<void(bool, int)> _callbackDrawBonus;
};
#endif /* ExpeditionManager_h */
