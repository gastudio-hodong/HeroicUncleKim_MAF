//
//  InfoWeaponSpirit.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/27.
//

#ifndef InfoWeaponSpirit_hpp
#define InfoWeaponSpirit_hpp

#include "Common/ConfigDefault.h"

class InfoWeaponSpirit : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoWeaponSpirit);
    
    InfoWeaponSpirit();
    virtual ~InfoWeaponSpirit();
    virtual bool init();
    
    
public:
    bool isHas();
    void setHas(bool value);
    
    bool isWear();
    void setWear(bool value);
    
    int getIdx();
    void setIdx(int value);
    
    int getIdxNextComb();
    void setIdxNextComb(int value);
    
    int getGrade();
    void setGrade(int value);
    
    int getCurrentCount();
    void setCurrentCount(int value);
    
    int getCurrentLv();
    void setCurrentLv(int value);
    
    int getLvUpCost();
    void setLvUpCost(int value);
    
    int getCurrentSmelting();
    void setCurrentSmelting(int value);
    
    int getSmeltingCost();
    void setSmeltingCost(int value);
    
    double getEffectDmg(int nLevel = -1, int nSmelting = -1);
    void setEffectDmg(double value);
    double getEffectGold(int nLevel = -1, int nSmelting = -1);
    void setEffectGold(double value);
    double getEffectKey(int nLevel = -1, int nSmelting = -1);
    void setEffectKey(double value);
    
    double getCalculationContract(double value);
    
    double getOriginEffectDmg();
    double getOriginEffectGold();
    double getOriginEffectKey();
    
    std::string getText();
    std::string getResourcePath();
    std::string getIconPath();
    
private:
    bool _isHas;
    bool _isWear;
    int _nIdx;
    int _nIdxNextComb;
    TInteger _nGrade;
    TInteger _nCurrentSmelting;
    int _nSmeltingBaseCost;
    
    TInteger _nCurrentCount;
    
    TInteger _nCurrentLv;
    int _nLvUpCost;
    
    TDouble _fEffectDmg;
    TDouble _fEffectGold;
    TDouble _fEffectKey;
};


#endif /* InfoWeaponSpirit_hpp */
