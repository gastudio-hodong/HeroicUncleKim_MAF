//
//  PopupRaidShopDefault.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/30.
//

#ifndef PopupRaidShopDefault_hpp
#define PopupRaidShopDefault_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/Raid/InfoWeaponSpirit.h"

class PopupRaidShopDefault : public PopupBase
{
public:
    enum class E_TYPE
    {
        NONE = 0,
        
        SUMMON = 1,
        PURCHASE,
        TRADING
    };
    static PopupRaidShopDefault* create(PopupRaidShopDefault::E_TYPE eType = PopupRaidShopDefault::E_TYPE::SUMMON);
    
    PopupRaidShopDefault(void);
    virtual ~PopupRaidShopDefault(void);
    bool init(PopupRaidShopDefault::E_TYPE eType);
    
public:
    
    void initVar();
    void initUI();
    
    void uiTop();
    void uiMiddle();
    
    void uiReload();
    void uiLoadLayer();
    
    void onClickClose(Ref* sender);
    void onClickTab(Ref* sender);
    
    void setCloseCondition(bool value);
    void callbackHide();
    void callbackSetTab(PopupRaidShopDefault::E_TYPE value);
private:
    bool _isCloseCondition;
    PopupRaidShopDefault::E_TYPE _eType;
    Layer* _lyTopUI;
    Layer* _lyContentArea;
    
    Layer* _lyContainerNow;
    Layer* _lyContainerSummon;
    Layer* _lyContainerPurchase;
    Layer* _lyContainerTrading;
};


#endif /* PopupRaidShopDefault_hpp */
