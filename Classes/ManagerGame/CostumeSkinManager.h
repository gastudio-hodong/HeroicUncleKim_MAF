//
//  CostumeSkinManager.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#ifndef CostumeSkinManager_h
#define CostumeSkinManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoCostumeSkin.h"

class CostumeSkinManager : public MafSingleton<CostumeSkinManager>
{
public:
    CostumeSkinManager(void);
    virtual ~CostumeSkinManager(void);
    virtual bool init();
    
public:
    
    // data
    void saveData();
    void loadData();
    
    // load
    void setLoadSkinList();
    void setLoadSkinShop();
    
    // set, get : list
    cocos2d::Vector<InfoCostumeSkin*> getListCostumeSkin();
    cocos2d::Vector<InfoCostumeSkinShop*> getListCostumeSkinShop();
    InfoCostumeSkin* getInfoSkin(int nIdx);
    InfoCostumeSkinShop* getInfoSkinShop(int nIdx);
    
    // set, get : other
    int getCostumeSkinEquip();
    void setCostumeSkinEquip(int nSkin);
    int getCostumeSkinEquipDummy();
    void setCostumeSkinEquipDummy(int nSkin);
    
    bool isCostumeSkinBuy(int nSkin);
    void setCostumeSkinBuy(int nSkin);
    void addCostumeSkinBuy(int nSkin);
    
    //
    double getEffectOwned();
    double getEffectBuff();
    
    // event
    int onSkinEquip(int skin);
    
    // network
    void requestSkinIsBuy(const std::function<void(bool)>& callback);
    void requestSkinBuy(int nShopIdx, const std::function<void(bool, int, int)>& callback);
    
protected:
    // network
    void responseSkinIsBuy(cocos2d::network::HttpResponse* response,std::string &data);
    void responseSkinBuy(cocos2d::network::HttpResponse* response,std::string &data);

private:
    cocos2d::Vector<InfoCostumeSkin*> _listCostumeSkin;
    cocos2d::Vector<InfoCostumeSkinShop*> _listCostumeSkinShop;
    
    std::vector<TInteger> _listSkinExist;
    TInteger _skinEquip;
    TInteger _skinEquipDummy;
    
    //
    std::function<void(bool)> _callbackSkinIsBuy;
    std::function<void(bool, int, int)> _callbackSkinBuy;
};

#endif /* CostumeSkinManager_h */
