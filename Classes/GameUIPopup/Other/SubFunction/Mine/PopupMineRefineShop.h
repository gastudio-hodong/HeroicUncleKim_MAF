//
//  PopupMineRefineShop.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#ifndef PopupMineRefineShop_h
#define PopupMineRefineShop_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupMineRefineShop : public PopupBase
{
public:
    static PopupMineRefineShop* create();
    
    PopupMineRefineShop(void);
    virtual ~PopupMineRefineShop(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
protected:
    
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiInfo();
    void uiMine();
    void uiButtons();
    
    //
    void drawInfo();
    void drawMine();
    void drawButtons();
    
    // get, set
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickBuy(cocos2d::Ref* sender);
    void onClickBuyAd(cocos2d::Ref* sender);
    void onClickMine(cocos2d::Ref* sender);
    
    // callback
    void callbackAds(ADS_RESULT result);
    void callbackPurchaseOk(int nIdx);
    void callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
    
private:
    bool _bInitUI;
    
    //

    // widget
    cocos2d::ui::Layout* _uiContainerInfo;
    cocos2d::ui::Layout* _uiContainerMine;
    cocos2d::ui::Layout* _uiContainerButtons;
};

#endif /* PopupMineRefineShop_h */
