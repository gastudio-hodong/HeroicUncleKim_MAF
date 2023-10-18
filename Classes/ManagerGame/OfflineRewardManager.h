//
//  OfflineRewardManager.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#ifndef OfflineRewardManager_h
#define OfflineRewardManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class OfflineRewardManager : public MafSingleton<OfflineRewardManager>
{
public:
    OfflineRewardManager(void);
    virtual ~OfflineRewardManager(void);
    virtual bool init();
    
public:
    
    // reward
    void rewardCalculation(); 
    void reward(bool bAds);
    
    // set, get
    void setOfflineLastTime();
    time_t getOfflineLastTimeDiff();
    
    bool isOfflineTime();
    int getOfflineTime();
    
    int getRewardTime();
    int getRewardFloor();
    int getRewardGem();
    int getRewardPoint();
    std::string getRewardKey();
    std::string getRewardGold();
    int getRewardMine(E_MINE eType);
    bool isRewardPetEvolution(E_PET ePet);
    bool isRewardStory();
    std::string getRewardDefenseCoin();
    
    void setCallbackCalculation(const std::function<void(void)>& callback);
    
protected:
    
    // set, get
    std::string getMonsterHp(int nFloor, int nFloorCount);
    
    
private:
    std::string _strFloor;
    std::string _strFloorCount;
    std::string _strTime;
    
    std::string _strRewardTime;
    std::string _strRewardFloor;
    std::string _strRewardGem;
    std::string _strRewardPoint;
    std::string _strRewardKey;
    std::string _strRewardGold;
    std::string _strRewardMine1;
    std::string _strRewardMine2;
    std::string _strRewardMine3;
    std::string _strRewardMine4;
    std::string _strRewardMine5;
    std::string _strRewardMine6;
    std::string _strRewardMineSpecial;
    std::string _strRewardMonsterSoulCount;
    std::string _strRewardDefenseCoinCount;
    
    std::function<void(void)> _onCallbackCalculation;
    
   
};

#endif /* OfflineRewardManager_h */
