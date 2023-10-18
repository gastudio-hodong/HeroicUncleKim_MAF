//
//  PetNewManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF on 2022/05/12.
//

#ifndef PetNewManager_h
#define PetNewManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoPetSkin.h"
#include "GameObject/InfoPetEquip.h"

class PetNewManager : public MafSingleton<PetNewManager>
{
public:
    PetNewManager(void);
    virtual ~PetNewManager(void);
    virtual bool init();
    
public:
    // update
    void update(float dt);
    
    // data
    void saveData();
    void loadData();
    
    // load
    void setLoad();
    void setLoadPetSkin();
    void setLoadPetEquip();
    void setLoadPetEquipLevel();
    
    // info
    cocos2d::Vector<InfoPetSkin*> getListPetSkinAll();
    cocos2d::Vector<InfoPetSkin*> getListPetSkin(E_PET ePet);
    InfoPetSkin* getInfoPetSkin(E_PET ePet, int idx);
    
    cocos2d::Vector<InfoPetEquip*> getListPetEquipAll();
    cocos2d::Vector<InfoPetEquip*> getListPetEquip(E_PET ePet);
    InfoPetEquip* getInfoPetEquip(int idx);
    
    cocos2d::Vector<InfoPetEquipAmount*> getListPetEquipLevelAll();
    InfoPetEquipAmount* getInfoPetEquipLevel(int level);
    
    
    // get, set : pet
    std::string getPetName(E_PET ePet, bool bEvolution = false); //
    std::string getPetExplain(E_PET ePet); //
    std::string getPetUpgradeExplain(E_PET ePet); //
    std::string getPetImagePath(E_PET ePet);
    
    double getPetCoolTime(E_PET ePet);
    double getPetEffect(E_PET ePet, int level);
    std::string getPetEffectPuriringKey(int nLevel, int nFloor, int nFloorCount); //
    
    // get, set : pet level
    int getPetLevel(E_PET ePet);
    int getPetLevelMax(E_PET ePet);
    void setPetLevel(E_PET ePet, int level);
    
    bool getPet(E_PET ePet);
    int getPetAcquireFloor(E_PET ePet);
    int getPetUpgradePrice(E_PET ePet, int level);
    
    // get, set : pet evolution
    int getPetEvolution(E_PET ePet);
    void setPetEvolution(E_PET ePet, int level);
    
    int getPetEvolutionReward(E_PET ePet);
    void setPetEvolutionReward(E_PET ePet, int level);
    
    std::string getPetEvolutionExplain(E_PET ePet, int nEvol);
    int getPetEvolutionFloor(E_PET ePet, int nEvol);
    double getPetEvolutionEffect1(E_PET ePet, int nEvol);
    double getPetEvolutionEffect2(E_PET ePet, int nEvol);
    
    // get, set : pet reinforce
    int getPetReinforce(E_PET ePet);
    void setPetReinforce(E_PET ePet, int level);
    
    std::string getPetReinforceExplain(E_PET ePet, int nLevel);
    std::vector<int> getPetReinforceUpgradePrice(E_PET ePet, int nLevel);
    double getPetReinforceEffect(E_PET ePet, int nLevel);
    
    // get, set : pet skin
    int getPetSkin(E_PET ePet);
    void setPetSkin(E_PET ePet, int idx);
    
    bool isPetSkinBuy(E_PET ePet, int idx);
    void setPetSkinBuy(E_PET ePet, int idx);
    
    // get, set : pet equip
    int getPetEquipLevel(int idx);
    int getPetEquipLevelMax();
    void setPetEquipLevel(int idx, int level);
    
    int getPetEquipAmount(int idx);
    int getPetEquipAmountNeed(int level);
    void setPetEquipAmount(int idx, int level);
    
    std::string getPetEquipExplain(int idx, int levelAdd = 0);
    double getPetEquipEffect(int idx, int returntype, int levelAdd = 0, bool ignore = false);
    
    int getPetEquipCollectLevelAverage();
    int getPetEquipCollectLevelTotal();
    double getPetEquipCollectEffect();
    double getPetEquipCollectEffectBongsik();
    
    int getPetEquipMaxUseCount();
    
    // event
    int onPetFree(E_PET ePet);
    int onPetLevelUP(E_PET ePet);
    
    int onPetEvolution(E_PET ePet);
    int onPetEvolutionReward(E_PET ePet);
    
    int onPetReinforce(E_PET ePet);
    
    int onPetSkinEquip(E_PET ePet, int idx);
    int onPetSkinBuy(E_PET ePet, int idx, bool bPopupLoading, const std::function<void(bool, int)>& callback);
    
    int onPetEquipLevelUP(int idx, bool bPopupLoading, const std::function<void(bool)>& callback);
    
    // network
    void requestSkinBuy(E_PET ePet, int nSkinIdx, int nItemIdx, std::string strItemCount, bool bPopupLoading, const std::function<void(bool, int)>& callback);
    void requestEquipInfo(bool bPopupLoading, const std::function<void(bool)>& callback);
    void requestEquipLevelUP(int idx, int level, bool bPopupLoading, const std::function<void(bool)>& callback);
    
protected:
    // network
    void responseSkinBuy(cocos2d::network::HttpResponse* response,std::string &data);
    void responseEquipInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void responseEquipLevelUP(int idx, cocos2d::network::HttpResponse* response,std::string &data);
    
private:
    //
    std::mutex _mutexEffect;
    std::mutex _mutexEffectPuriringKey;
    std::mutex _mutexEffectEvolution1;
    std::mutex _mutexEffectEvolution2;
    std::mutex _mutexEffectReinforce;
    std::mutex _mutexEffectEquip;
    
    //
    E_PLACE _ePlace;
    
    // pet level
    TInteger _uPetLevel[C_COUNT_PET+1];
    
    // pet evolution
    TInteger _listPetEvolution[C_COUNT_PET+1];
    TInteger _listPetEvolutionReward[C_COUNT_PET+1];
    
    // pet reinforce
    TInteger _uPetReinforce[C_COUNT_PET+1];
    
    // pet skin
    TInteger _uPetSkin[C_COUNT_PET+1];
    std::vector<std::string> _listPetSkinBuy;
    
    // pet equip
    std::map<int, TInteger> _listPetEquipLevel;
    std::map<int, TInteger> _listPetEquipAmount;
    
    //
    cocos2d::Vector<InfoPetSkin*> _listInfoPetSkin;
    cocos2d::Vector<InfoPetEquip*> _listInfoPetEquip;
    cocos2d::Vector<InfoPetEquipAmount*> _listInfoPetEquipLevel;
    
    // callback
    std::function<void(bool, int)> _callbackSkinBuy;
    std::function<void(bool)> _callbackEquipInfo;
    std::function<void(bool)> _callbackEquipLevelUP;
};


#endif /* PetNewManager_hpp */
