//
//  PopupAdsWatchDouble.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupAdsWatchDouble_h
#define PopupAdsWatchDouble_h

#include "GameUIPopup/Base/PopupBase.h"


class PopupAdsWatchDouble : public PopupBase
{
public:
    static PopupAdsWatchDouble* create(E_ADS_WATCH eType);
    
    PopupAdsWatchDouble(void);
    virtual ~PopupAdsWatchDouble(void);
    virtual bool init(E_ADS_WATCH eType);
    
public:
    // set
    void setCallbackResult(const std::function<void(bool)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContents();
    void uiButton();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickOk(cocos2d::Ref* sender);
    void onClickAds(cocos2d::Ref* sender);
    
private:
    //
    E_ADS_WATCH _eType;
    std::function<void(bool)> _callbackResult;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerButton;
};

#endif /* PopupAdsWatchDouble_h */
