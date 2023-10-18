//
//  MonsterManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF on 2022/05/12.
//

#ifndef MonsterManager_h
#define MonsterManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/ConfigGameNormal.h"

class MonsterManager : public MafSingleton<MonsterManager>
{
public:
    MonsterManager(void);
    virtual ~MonsterManager(void);
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
    
    // get, set : soul
    bool isSoulLevelComplete();
    
    int getSoulCountNow(int idx);
    void addSoulCount(int idx, int countAdd);
    void setSoulCount(int idx, int count);
    
    // get, set : dex
    double getDexEffect();
    
    int getDexLevelMax();
    int getDexLevelNow(int idx);
    int getDexLevelCount(int level);
    void addDexLevel(int idx);
    void setDexLevel(int idx, int levelDex);
    
    int getDexInfoNeedCountTotal(int levelDexMax = -1);
    int getDexInfoNeedCount(int levelDex);
    
    // get, set : dex bonus
    double getDexBonusEffect();
    
    int getDexBonusLevelMax();
    int getDexBonusLevelNow();
    
    int getDexBonusInfoNeedCount(int levelDexBonus);
    int getDexBonusInfoNeedDexLevel(int levelDexBonus);
    double getDexBonusInfoEffect(int levelDexBonus);
    
    // event
    
    // network
    
protected:
    // network
    
    
private:
    //
    E_PLACE _ePlace;
    
    // soul, dex
    TInteger _uSoulCount[C_COUNT_MONSTER+1];
    TInteger _uDexLevel[C_COUNT_MONSTER+1];
    
    
    // callback 
};


#endif /* MonsterManager_hpp */
