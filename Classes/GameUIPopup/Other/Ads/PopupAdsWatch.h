//
//  PopupAdsWatch.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupAdsWatch_h
#define PopupAdsWatch_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupAdsWatch : public PopupBase
{
public:
    static PopupAdsWatch* create(E_ADS_WATCH eType, int nCount);
    static PopupAdsWatch* create(E_ADS_WATCH eType, std::string strCount);
    
    PopupAdsWatch(void);
    virtual ~PopupAdsWatch(void);
    virtual bool init(E_ADS_WATCH eType, std::string strCount);
    
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
    void onClickAds(cocos2d::Ref* sender);
    
private:
    //
    E_ADS_WATCH _eType;
    std::string _strCount;
    std::string _strPath;
    
    std::function<void(void)> _callbackResult;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerButton;
};

#endif /* PopupAdsWatch_h */
