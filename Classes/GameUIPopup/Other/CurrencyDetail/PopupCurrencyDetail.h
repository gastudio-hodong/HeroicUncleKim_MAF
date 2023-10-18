//
//  PopupCurrencyDetail.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupCurrencyDetail_h
#define PopupCurrencyDetail_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupCurrencyDetail : public PopupBase
{
public:
    enum E_TYPE {
        NONE,
        QUEST,
        CURRENCY,
    };
    
public:
    static PopupCurrencyDetail* create(E_ITEMS eItems, E_TYPE eType = E_TYPE::NONE, int nTypeParams = 0);
    
    PopupCurrencyDetail(void);
    virtual ~PopupCurrencyDetail(void);
    virtual bool init(E_ITEMS eItems, E_TYPE eType, int nTypeParams);
    virtual void onEnter() override;
    virtual void show() override;
    
    void setCallbackTransferShop(const std::function<void(void)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui : default
    void uiDefaultContentsUse();
    void uiDefaultContentsGet();
    void uiDefaultWarning();
    
    // ui : quest
    void uiQuestAds();
    
    // ui
    
    
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickMove(cocos2d::Ref* sender);
    void onClickShop(cocos2d::Ref* sender);
    void onClickQuestAds(cocos2d::Ref* sender);
    
    // callback
    void callbackAdsInterstitial(ADS_RESULT result);
    
private:
    E_ITEMS _eItems;
    E_TYPE _eType;
    int _nTypeParams;
    
    int _nAdsTimeHide;
    int _nAdsTimeHideMax;
    
    cocos2d::Size _sizeContainer;
    
    std::vector<E_AREA_MOVE> _listGetArea;
    std::vector<E_AREA_MOVE> _listUseArea;
    
    // widget
    cocos2d::Layer* _layerContainerContentsGet;
    cocos2d::Layer* _layerContainerContentsUse;
    cocos2d::Layer* _layerContainerAds;
    
    cocos2d::Label* _labelAdsTime;
    
    std::function<void(void)> _callbackTransferShop;
};

#endif /* PopupTicket_h */
