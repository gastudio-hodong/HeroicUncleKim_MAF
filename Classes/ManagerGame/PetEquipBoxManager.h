//
//  PetEquipBoxManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 28/06/2019.
//

#ifndef PetEquipBoxManager_hpp
#define PetEquipBoxManager_hpp

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoPetEquip.h"
#include "GameObject/InfoPetSkin.h"

class PetEquipBoxManager : public MafSingleton<PetEquipBoxManager>
{
public:
    PetEquipBoxManager(void);
    virtual ~PetEquipBoxManager(void);
    virtual bool init();
    
public:
    
    // load
    void setLoad();
    void setLoadPetBoxShop();
    
    // pet equip : box
    int getEquipBoxCount();
    void setEquipBoxCount(int nCount);
    
    int getEquipBoxEventCount();
    
    int getEquipBoxSpecialCount();
    void setEquipBoxSpecialCount(int nCount);
    
    int getEquipBoxMileage();
    
    int getEquipBoxOpenHonor();
    int getEquipBoxOpenBong();
    int getEquipBoxOpenDevil();
    int getEquipBoxOpenFree();
    int getEquipBoxOpenCash();
    
    int getPetEquipFreeBoxCount();
    int getEquipBoxOpenDevilMax();
    
    int getCost(E_PET_EQUIP_BUY eType, bool isEvent);
    int getCostData(E_PET_EQUIP_BUY eType, bool isEvent);
    
    // network
    void requestEquipBoxInfo(const std::function<void(bool)>& callback);
    void requestEquipBoxBuy(int nType, const std::function<void(bool, int, int)>& callback);
    void requestEquipBoxUse(int nUse, int nType, const std::function<void(bool, int, cocos2d::Vector<InfoPetEquipDraw*>, cocos2d::Vector<InfoPetEquipDrawGrade*>)>& callback);
    
    
protected:
    // network
    void responseEquipBoxInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void responseEquipBoxBuy(cocos2d::network::HttpResponse* response,std::string &data, int nType);
    void responseEquipBoxUse(cocos2d::network::HttpResponse* response,std::string &data, int nUse);
    
private:
    int _nEquipBoxCount;
    int _nEquipBoxEventCount;
    int _nEquipBoxSpecialCount;
    
    int _nEquipBoxOpenHonor;
    int _nEquipBoxOpenBong;
    int _nEquipBoxOpenDevil;
    int _nEquipBoxOpenFree;
    int _nEquipBoxOpenCash;
    
    int _nEquipBoxMileage;
    
    int _nCostDevil;
    int _nCostHonor;
    int _nCostBong;
    
    int _nCostEventDevil;
    int _nCostEventHonor;
    int _nCostEventBong;
    
    // callback
    std::function<void(bool)> _callbackEquipBoxInfo;
    std::function<void(bool, int, int)> _callbackEquipBoxBuy;
    std::function<void(bool, int, cocos2d::Vector<InfoPetEquipDraw*>, cocos2d::Vector<InfoPetEquipDrawGrade*>)> _callbackEquipBoxUse;
    
    
    
};
#endif /* PetEquipBoxManager_hpp */
