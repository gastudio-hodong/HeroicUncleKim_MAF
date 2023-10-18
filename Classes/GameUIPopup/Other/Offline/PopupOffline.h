//
//  PopupOffline.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupOffline_h
#define PopupOffline_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupOffline : public PopupBase
{
public:
    static PopupOffline* create();
    
    PopupOffline(void);
    virtual ~PopupOffline(void);
    virtual bool init();
    
public:
    // set, get
    void setCallbackResult(const std::function<void(void)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContentsTop();
    void uiContentsBottom();
    void uiButton();
    void uiStep1();
    void uiStep2();
    void uiStep3();
    void uiStep4();
    void uiStep5();

    // set, get
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickAds(cocos2d::Ref* sender);

    // callback
    void callbackAds(ADS_RESULT result);
    

    
private:
    int _nTime;
    
    int _nRewardFloor;
    int _nRewardGem;
    int _nRewardPoint;
    std::string _strRewardKey;
    std::string _strRewardGold;
    int _nRewardMine1;
    int _nRewardMine2;
    int _nRewardMine3;
    int _nRewardMine4;
    int _nRewardMine5;
    int _nRewardMine6;
    int _nRewardMineSpecial;
    bool _bRewardAds;
    std::string _strRewardDefenseCoin;
    
    std::function<void(void)> _onCallbackResult;
    
    // widget
    cocos2d::Layer* _layerContainerContentsTop;
    cocos2d::Layer* _layerContainerContentsBottom;
    cocos2d::Layer* _layerContainerButton;
    cocos2d::Layer* _layerStep1;
    cocos2d::Layer* _layerStep2;
    cocos2d::Layer* _layerStep3;
    cocos2d::Layer* _layerStep4;
    cocos2d::Layer* _layerStep5;
};

#endif /* PopupOffline_h */
