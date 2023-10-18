//
//  PopupRaidBuyAttackCount.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/10.
//

#ifndef PopupRaidBuyAttackCount_hpp
#define PopupRaidBuyAttackCount_hpp

#include "GameUIPopup/Base/PopupBase.h"

class PopupRaidBuyAttackCount : public PopupBase
{
public:
    static PopupRaidBuyAttackCount* create();
    
    PopupRaidBuyAttackCount(void);
    virtual ~PopupRaidBuyAttackCount(void);
    virtual bool init();
    
public:
    // set
    void setCallbackResult(const std::function<void(void)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContents();
    void uiButton();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickBuy(cocos2d::Ref* sender);
    void onClickAds(cocos2d::Ref* sender);
    
    // network
    
    void callBackResultAds(ADS_RESULT result);
    void callbackBuy(bool bResult, int nResult);
private:
    // widget
    Layer* _layerContainerContents;
    Layer* _layerContainerButton;
    
    std::function<void(void)> _callbackResult;
};

#endif /* PopupRaidBuyAttackCount_hpp */
