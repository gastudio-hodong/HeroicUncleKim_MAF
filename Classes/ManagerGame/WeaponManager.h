//
//  WeaponManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF on 2022/05/12.
//

#ifndef WeaponManager_h
#define WeaponManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoWeapon.h"

class WeaponManager : public MafSingleton<WeaponManager>
{
public:
    WeaponManager(void);
    virtual ~WeaponManager(void);
    virtual bool init();
    
public:
    // update
    void update(float dt);
    
    // data
    void saveData();
    void loadData();
    
    // load
    void setLoad();
    void setLoadWeapon();
    
    // info
    cocos2d::Vector<InfoWeapon*> getListWeaponAll();
    InfoWeapon* getInfoWeapon(int idx);
    
    // get, set :
    std::string getImagePath(int idx);
    std::string getBuyPrice(int idx, int level);
    std::string getBuyPriceoriginal(int idx, int level);
    
    int getCount();
    int getLegendExistCount();
    
    std::string getAttack(int idx, int level, int honor = 0);
    
    // get, set : level
    int getLevel(int idx);
    int getLevelMax();
    void setLevel(int level);
    
    // get, set : equip
    int getEquip();
    void setEquip(int idx);
    
    int getEquipVisual();
    void setEquipVisual(int idx);
    
    int getEquipHigh();
    void setEquipHigh(int idx);
    
    int getEquipReborn();
    void setEquipReborn(int idx);
    
    int getEquipPermanent();
    void setEquipPermanent(int idx);
    
    int getReinforce(int idx);
    void setReinforce(int idx, int level);
    
    double getReinforceEffect();
    double getReinforceEffect(int idx, int reinforce = -1);
    
    
    // get, set : dex
    double getDexEffect();
    
    // get, set : dex bonus
    double getDexBonusEffect();
    
    int getDexBonusLevelMax();
    int getDexBonusLevelNow();
    
    int getDexBonusInfoNeedCount(int levelDexBonus);
    double getDexBonusInfoEffect(int levelDexBonus);
    
    // event
    int onReset(int floor);
    int onWeaponBuy(int idx);
    int onWeaponBuyLegend(int idx);
    int onWeaponBuyPermanent(int idx);
    int onWeaponChange(int idx);
    int onWeaponReinforce(int idx);
    
    void onWeaponBuyMax();
    
    // network
    
protected:
    // network
    
    
private:
    //
    E_PLACE _ePlace;
    
    // level
    TInteger _uLevel;
    
    // equip
    TInteger _uEquip;
    TInteger _uEquipVisual;
    TInteger _uEquipHigh;
    TInteger _uEquipReborn;
    TInteger _uEquipPermanent;
    
    // 무기 공격력 임시 저장 변수 
    TInteger _equipAttackIdx;
    TInteger _equipAttacklevel;
    TInteger _equipAttackHonor;
    std::string _equipAttackValue;
    
    //
    std::map<int, int> _listEquipReinforce;
    
    // info
    cocos2d::Vector<InfoWeapon*> _listInfoWeapon;
    
    struct WeaponPrice
    {
        std::string lv[6];
    };
    std::map<int, WeaponPrice> _listInfoWeaponPrice;
    
    
    
    // callback 
};


#endif /* WeaponManager_hpp */
