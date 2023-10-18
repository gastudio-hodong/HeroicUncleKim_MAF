//
//  PopupAdsInterstitial.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupAdsInterstitial_h
#define PopupAdsInterstitial_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupAdsInterstitial : public PopupBase
{
public:
    static PopupAdsInterstitial* create(int nType);
    
    PopupAdsInterstitial(void);
    virtual ~PopupAdsInterstitial(void);
    virtual bool init(int nType);
    virtual void onEnter();

    
public:
    // set
    void setCallbackResult(const std::function<void(int)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContents();
    void uiButton();
    
    void drawButton();
    
    // schedule
    void scheduleHide(float dt);
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickAds(cocos2d::Ref* sender);
    
    // callback
    void callbackAdsInterstitial(ADS_RESULT result);
    
private:
    int _nType;
    
    int _nTimeHide;
    int _nTimeHideMax;
    
    std::function<void(int)> _callbackResult;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerButton;
    cocos2d::Label* _labelButtonClose;
};

#endif /* PopupAdsInterstitial_h */
