//
//  PopupDefenseShopDefault.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/11.
//

#ifndef PopupDefenseShopDefault_hpp
#define PopupDefenseShopDefault_hpp


#include "GameUIPopup/Base/PopupBase.h"

class PopupDefenseShopDefault : public PopupBase
{
public:
    enum class E_TYPE
    {
        NONE = 0,
        
        SUMMON = 1,
        PURCHASE
    };
    static PopupDefenseShopDefault* create(PopupDefenseShopDefault::E_TYPE eType = PopupDefenseShopDefault::E_TYPE::SUMMON);
    
    PopupDefenseShopDefault(void);
    virtual ~PopupDefenseShopDefault(void);
    bool init(PopupDefenseShopDefault::E_TYPE eType);
    
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
    void callbackSetTab(PopupDefenseShopDefault::E_TYPE value);
private:
    bool _isCloseCondition;
    PopupDefenseShopDefault::E_TYPE _eType;
    Layer* _lyTopUI;
    Layer* _lyContentArea;
    
    Layer* _lyContainerNow;
    Layer* _lyContainerSummon;
    Layer* _lyContainerPurchase;
};



#endif /* PopupDefenseShopDefault_hpp */
