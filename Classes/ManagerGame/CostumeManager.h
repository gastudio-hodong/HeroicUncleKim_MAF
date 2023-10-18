//
//  CostumeManager.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#ifndef CostumeManager_h
#define CostumeManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoCostume.h"

class CostumeManager : public MafSingleton<CostumeManager>
{
public:
    CostumeManager(void);
    virtual ~CostumeManager(void);
    virtual bool init();
    
public:
    // load
    void setLoad();
    
    // set, get : list
    cocos2d::Vector<InfoCostume*> getCostumeAll();
    cocos2d::Vector<InfoCostume*> getCostumeType(std::string strType);
    cocos2d::Vector<InfoCostume*> getCostumeType(E_COSTUME eType);
    InfoCostume* getCostume(E_COSTUME eType, E_COSTUME_IDX eIdx);
    
    // set, get : info
    bool isExist(E_COSTUME eType, E_COSTUME_IDX eIdx);
    int getExist(E_COSTUME eType, E_COSTUME_IDX eIdx);
    void setExist(E_COSTUME eType, E_COSTUME_IDX eIdx, int nValue);
    
    bool isEquip(E_COSTUME eType, E_COSTUME_IDX eIdx);
    int getEquip(E_COSTUME eType);
    void setEquip(E_COSTUME eType, E_COSTUME_IDX eIdx);
    
    bool isRent(E_COSTUME eType, E_COSTUME_IDX eIdx);
    int getRent(E_COSTUME eType);
    void setRent(E_COSTUME eType, E_COSTUME_IDX eIdx, bool bSave=true);
    int getRentTime(E_COSTUME eType);
    void setRentTime(E_COSTUME eType, int64_t nTime, bool bSave=true);
    
    int getHonorLevel(E_HONOR eHonor);
    void setHonorLevel(E_HONOR eHonor, int nLevel, bool bSave = true);
    
    // set, get : other
    int getAdsCount();
    std::string getIconPath(E_COSTUME eType, E_COSTUME_IDX eIdx);
   
    int getExistTypeCount(E_COSTUME eType);
    int getExistCount();
    int getExistAuraCount();
    int getExistHonorCount();
    int getRentCount(E_COSTUME eType, E_COSTUME_IDX eIdx);
    int getLegendLevel(E_COSTUME eType);
    int getLegendEnableLevel(E_COSTUME eType);
    int getLegendCount();
    bool isExistSet(E_COSTUME_IDX eIdx);
    bool isExistAll();
   
    // callback
    
    // network
    void requestInfo(const std::function<void(bool)>& callback);
    void requestAds(const std::function<void(bool, int)>& callback);
    void requestRent(int nType, int nIdx, int nPrice, const std::function<void(bool)>& callback);
    void requestBuy(int nType, int nIdx, const std::function<void(bool, int)>& callback);
    void requestAuraBuy(int nType, int nIdx, const std::function<void(bool, int)>& callback);
    void requestHonorBuy(int nType, int nPrice, const std::function<void(bool)>& callback);
    void requestLegendBuy(int nType, int nIdx, int nPrice, int nPriceBong, const std::function<void(bool)>& callback );
    
protected:
    // network
    void responseInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void responseAds(cocos2d::network::HttpResponse* response,std::string &data);
    void responseRent(cocos2d::network::HttpResponse* response,std::string &data);
    void responseBuy(cocos2d::network::HttpResponse* response,std::string &data);
    void responseAuraBuy(cocos2d::network::HttpResponse* response,std::string &data);
    void responseHonorBuy(cocos2d::network::HttpResponse* response,std::string &data);
    void responseLegendBuy(cocos2d::network::HttpResponse* response,std::string &data);
    
private:
    cocos2d::Vector<InfoCostume*> _listCostume;
    
    int _nAdsCount;
    int _infoExist[C_COUNT_COSTUME+1][C_COUNT_COSTUME_TYPE+1];
    int _infoExistAura[C_COUNT_COSTUME+1];
    int _infoEquip[C_COUNT_COSTUME_TYPE+1];
    int _infoEquipAura;
    int _infoRent[C_COUNT_COSTUME_TYPE+1];
    int _infoRentTime[C_COUNT_COSTUME_TYPE+1];
    int _infoRentCount[C_COUNT_COSTUME_TYPE+1][C_COUNT_COSTUME+1];
    int _infoHonor[4];
    
    //
    std::function<void(bool)> _callbackInfo;
    std::function<void(bool, int)> _callbackAds;
    
    std::function<void(bool)> _callbackRent;
    std::function<void(bool, int)> _callbackBuy;
    std::function<void(bool, int)> _callbackAuraBuy;
    std::function<void(bool)> _callbackHonorBuy;
    std::function<void(bool)> _callbackLegendBuy;
};

#endif /* CostumeManager_h */
