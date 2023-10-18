//
//  WeaponSpiritManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/01.
//

#ifndef WeaponSpiritManager_hpp
#define WeaponSpiritManager_hpp

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoItems.h"
#include "GameObject/Raid/InfoWeaponSpirit.h"
#include "GameObject/Raid/InfoRaidMileageLv.h"
#include "GameObject/Raid/InfoRaidMileageReward.h"
#include "GameObject/Raid/InfoWeaponSpiritBuff.h"
#include "GameObject/Raid/InfoWeaponSpiritGradeSynergy.h"

class WeaponSpiritManager : public MafSingleton<WeaponSpiritManager>
{
public:
    enum class E_EFFECT_TYPE{
        NONE = 0,
        
        DMG = 1,
        GOLD,
        KEY
    };
    enum class E_SPIRIT_BUFF_STATE{
        NONE = 0,
        
        COOL_TIME,
        DURATION_TIME
    };
    
public:
    WeaponSpiritManager(void);
    virtual ~WeaponSpiritManager(void);
    virtual bool init();
    
public:
    //load
    void setLoadSpiritFirstSummonData(int size);
    void setLoadSpirit();
    void setLoadSmeltingCost();
    void setLoadMileageLv();
    void setLoadMileageReward();
    void setLoadWeaponSpiritBuff();
    void setLoadWeaponSpiritSynergy();
    
    //save
    void setSaveSpiritFirstSummonData();
    
    //get
    cocos2d::Vector<InfoWeaponSpirit*> getListWeaponSpirit();
    cocos2d::Vector<InfoWeaponSpirit*> getListWeaponSpirit(int nGrade);
    InfoWeaponSpirit* getInfoWeaponSpirit(int idx);
    InfoWeaponSpirit* getInfoWearSpirit();
    int getSpiritStonCount();
    int getTotalMileage();
    int getSummonLevelEXP();
    int getSummonTicket_1();
    int getSummonTicket_11();
    int getSummonTicket_33();
    int getCurrentMileageLv();
    int getCurrentMileageLvPoint();
    InfoRaidMileageLv* getInfoMileageLv(int value);
    cocos2d::Vector<InfoRaidMileageLv*> getListMileageLv();
    int getCurrentMileageRewardIdx();
    void setCurrentMileageRewardIdx(int value);
    int getCurrentMileageRewardPoint();
    void setContractLv(int value);
    int getContractLv();
    InfoRaidMileageReward* getInfoMileageReward(int value);
    cocos2d::Vector<InfoRaidMileageReward*> getListMileageReward();
    InfoWeaponSpiritBuff* getInfoWeaponSpiritBuff();
    InfoWeaponSpiritBuff* getInfoWeaponSpiritBuff(int nIdx);
    InfoWeaponSpiritGradeSynergy* getInfoWeaponSpiritGradeSynergy(int nGrade);
    void setSpiritStonCount(int value);
    void setTotalMileage(int value);
    void setSummonTicket_1(int value);
    void setSummonTicket_11(int value);
    void setSummonTicket_33(int value);
    
    int getAdsSummonCooldown();
    void setAdsSummonCooldown(int value);
    
    void setSpiritBuff();
    void scheduleSpiritBuff();
    WeaponSpiritManager::E_SPIRIT_BUFF_STATE getBuffState();
    
    int getAdsSummonCount();
    void setAdsSummonCount(int value);
    
    bool isContractPossible(int value);
    
    bool isReddotCondition();
    
    bool isFirstSummon(int value);
    
    void setFirstSummon(int value);
    
    bool isServerDispatch();
    
    int getSmeltingSpiritCost(int nLv);
    std::string getSpiritCalculation(std::string value, WeaponSpiritManager::E_EFFECT_TYPE type, bool isOrginal = false);
    double getSpiritSynergyAll();
    double getSpiritSynergy(int nGrade);
    int getSpiritMaximumAll();
    bool getSpiritMaximum(int idx);
    
    int getSpiritMaxGrade();
    
    //network : request
    void requestInfo(const std::function<void(bool, int)>& callback);
    void requestSpiritCompose(const std::function<void(bool, int)>& callback, int nIdx, int nCount);
    void requestSpiritLevelup(const std::function<void(bool, int)>& callback, int nIdx);
    void requestSpiritSmelting(const std::function<void(bool, int)>& callback, int nIdx);
    void requestSpiritWear(const std::function<void(bool, int)>& callback, int nIdx);
    
    void requestSpiritSummon(const std::function<void(bool, int, cocos2d::Vector<InfoItems*>)>& callback, int nCount, bool isAds = false);
    void requestMileageReward(const std::function<void(bool, int)>& callback);
    void requestContract(const std::function<void(bool, int)>& callback);
    
protected:
    // network : respons
    void responseInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void responseSpiritCompose(cocos2d::network::HttpResponse* response,std::string &data);
    void responseSpiritLevelup(cocos2d::network::HttpResponse* response,std::string &data);
    void responseSpiritSmelting(cocos2d::network::HttpResponse* response,std::string &data);
    void responseSpiritSummon(cocos2d::network::HttpResponse* response,std::string &data);
    void responseSpiritWear(cocos2d::network::HttpResponse* response,std::string &data);
    void responseMileageReward(cocos2d::network::HttpResponse* response,std::string &data);
    void responseContract(cocos2d::network::HttpResponse* response,std::string &data);
    
    //utills
    void resetSpiritInfo();
    
private:
    bool _isServerDispatch;
    int _nSpiritStonCount;
    int _nCuttentMileageRewardIdx;
    int _nTotalMileage;
    int _nSummonTicket_1;
    int _nSummonTicket_11;
    int _nSummonTicket_33;
    int _nAdsSummonCount;
    int _nAdsSummonCooltime;
    int _nSpiritBuffCooltime;
    int _nSpiritBuffDurationTime;
    E_SPIRIT_BUFF_STATE _eSpiritBuffState;
    TInteger _nSpiritContractLv;
    
    InfoWeaponSpirit* _infoWearSpirit;
    cocos2d::Vector<InfoWeaponSpirit*> _listInfoWeaponSpirit;
    cocos2d::Vector<InfoRaidMileageLv*> _listInfoMileageLv;
    cocos2d::Vector<InfoRaidMileageReward*> _listInfoMileageReward;
    cocos2d::Vector<InfoWeaponSpiritBuff*> _listInfoWeaponSpiritBuff;
    cocos2d::Vector<InfoWeaponSpiritGradeSynergy*> _listInfoWeaponSpiritGradeSynergy;
    
    std::vector<bool> _listIsSpiritFirstSummon;
    std::map<int, int> _mapSmeltingCost;
    
    std::function<void(bool, int)> _callbackInfo;
    std::function<void(bool, int)> _callbackRankUp;
    std::function<void(bool, int)> _callbackLevelUp;
    std::function<void(bool, int)> _callbackSmelting;
    std::function<void(bool, int)> _callbackWear;
    std::function<void(bool, int)> _callbackMileageReward;
    std::function<void(bool, int)> _callbackContractLv;
    std::function<void(bool, int, cocos2d::Vector<InfoItems*>)> _callbackSummon;
    
};

#endif /* WeaponSpiritManager_hpp */
