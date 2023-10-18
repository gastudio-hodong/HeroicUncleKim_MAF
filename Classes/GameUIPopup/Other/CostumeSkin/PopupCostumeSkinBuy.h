//
//  PopupCostumeSkinBuy.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef PopupCostumeSkinBuy_h
#define PopupCostumeSkinBuy_h

#include "GameUIPopup/Base/PopupBase.h"

class InfoCostumeSkinShop;
class InfoCostumeSkin;
class PopupCostumeSkinBuy : public PopupBase
{
public:
    static PopupCostumeSkinBuy* create();
    
    PopupCostumeSkinBuy(void);
    virtual ~PopupCostumeSkinBuy(void);
    virtual void onEnter() override;
    
public:
    void setSkinShopIdx(int idx);
    
protected:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContainer();
    void uiInfo();
    void uiButtons();
    void uiPurchase(cocos2d::ui::Layout* uiParent);
    
    // ui : draw
    
    // set, get
    
    
    // callback
    void callbackBuyPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
    void callbackBuyItem(bool bResult, int nResult, int nSkin);
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickBuyCash(cocos2d::Ref* sender);
    void onClickBuyItem(cocos2d::Ref* sender);
    
    // game
    

private:
    int _skinShopIdx;
    InfoCostumeSkinShop* _objSkinShop;
    InfoCostumeSkin* _objSkin;
    
    // widget
    cocos2d::ui::Layout* _uiContainerInfo;
    cocos2d::ui::Layout* _uiContainerButtons;
    
 
};
#endif /* PopupCostumeSkinBuy_h */
