//
//  TimeTravelManager.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#ifndef TimeTravelManager_h
#define TimeTravelManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class TimeTravelManager : public MafSingleton<TimeTravelManager>
{
public:
    TimeTravelManager(void);
    virtual ~TimeTravelManager(void);
    virtual bool init();
    
public:
    void saveData();
    void loadData();
    
    
    
    // set, get item
    double getItemEffectSub(E_TIME_TRAVEL type, int level);
    double getItemEffect(E_TIME_TRAVEL type, int level);
    int getItemPrice(E_TIME_TRAVEL type, int level);
    int getItemMaxLevelCondition();
    int getItemMaxLevel(E_TIME_TRAVEL type);
    int getItemLevel(E_TIME_TRAVEL type);
    void setItemLevel(E_TIME_TRAVEL type, int level);
    
    std::string getItemNameKey(E_TIME_TRAVEL type);
    std::string getItemOptionKey(E_TIME_TRAVEL type);
    
    // set, get Time Travel
    int getTimeTravelChangeTime();
    void setTimeTravelChangeTime(int64_t time);
    
    int getTimeTravelCount();
    int getTimeTravelCountMax();
    void setTimeTravelCount(int64_t count);
    void addTimeTravelCount();
    void subTimeTravelCount();
    
    // set, get other
    void setCallbackCalculation(const std::function<void(void)>& callback);
    
    // event
    bool OnItemLevel(E_TIME_TRAVEL type);
    bool OnTimeTravel();
    
    void OnRewardCalculation();
    void OnReward();
    
    // reward
    int getRewardFloor();
    int getRewardGem();
    int getRewardPoint();
    std::string getRewardKey();
    std::string getRewardGold();
    
protected:
    std::string getMonsterHp(int nFloor, int nFloorCount);
     
    
private:
    //
    TInteger _listItemLevel[3];
    TInteger _nTimeTravelCount;
    TInteger _nTimeTravelTime;
    
    // reward
    std::string _strFloor;
    std::string _strFloorCount;
    
    std::string _strRewardTime;
    std::string _strRewardFloor;
    std::string _strRewardGem;
    std::string _strRewardPoint;
    std::string _strRewardKey;
    std::string _strRewardGold;
    std::string _strRewardMonsterSoulCount;
    
    std::function<void(void)> _onCallbackCalculation;
    
   
};

#endif /* TimeTravelManager_h */
