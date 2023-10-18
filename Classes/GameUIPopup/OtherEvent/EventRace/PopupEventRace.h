//
//  PopupEventRace.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/21.
//

#ifndef PopupEventRace_hpp
#define PopupEventRace_hpp

#include "GameUIPopup/Base/PopupBase.h"

class PopupEventRace : public PopupBase
{
public:
    static PopupEventRace* create();
    
    PopupEventRace();
    virtual ~PopupEventRace(void);
    virtual bool init() override;
    virtual void onEnter() override;
protected:
    void initVar();
    void initUI();
    
    void uiBanner();
    void uiTop();
    void uiMainGame();
    void uiChat();
    void uiButton();
    void uiAds();
    
    void onClickClose(Ref* sender);
    void onClickRank(Ref* sender);
    void onClickMission(Ref* sender);
    void onClickDoubleAds(Ref* sender);
    void onClickHiddenAds(Ref* sender);
    void onClickStart(Ref* sender);
    
    //event
    void onEventInvest(const int nIdx);
    void onEventGameOver(const bool isBankrupt);
    void onEventEventOver();
    
    //Ads Callback
    void callBackResultHiddenAds(ADS_RESULT result);
    void callBackResultDoubleAds(ADS_RESULT result);
    
    //callback
    void callbackInfo(bool bResult, int nResult);
    void callbackAds(bool bResult, int nResult, int nType);
    void callbackGameStart(bool bResult, int nResult);
    void callbackGameOver(bool bResult, int nResult);
    void callbackRankingHide();
private:
    cocos2d::Layer* _lyContainerParent;
    cocos2d::Layer* _lyBanner;
    cocos2d::Layer* _lyTop;
    cocos2d::Layer* _lyMainGame;
    cocos2d::Layer* _lyChat;
    cocos2d::Layer* _lyButton;
    cocos2d::Layer* _lyAds;
};


#endif /* PopupEventRace_hpp */
