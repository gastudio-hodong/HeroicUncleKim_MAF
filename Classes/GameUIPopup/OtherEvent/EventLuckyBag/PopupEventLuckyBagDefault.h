//
//  PopupEventLuckyBagDefault.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/12/21.
//

#ifndef PopupEventLuckyBagDefault_hpp
#define PopupEventLuckyBagDefault_hpp

#include "GameUIPopup/Base/PopupBase.h"

class PopupEventLuckyBagDefault : public PopupBase
{
public:
    static PopupEventLuckyBagDefault* create();
    
    PopupEventLuckyBagDefault();
    virtual ~PopupEventLuckyBagDefault(void);
    virtual bool init() override;
    virtual void onEnter() override;
protected:
    void initVar();
    void initUI();
    
    //ui
    void uiContainer();
    void uiContainerNetwork();
    void uiTop();
    
    // ui : draw
    void drawTop();
    void drawMyReward();
    void drawMain();
    
    void onClickClose(Ref* sender);
    void onClickHelp(Ref* sender);
    void onClickAds(Ref* sender);
    void onClickToggleAnimation(Ref* sender);
    
    //callback
    void callbackInfo(const bool bResult, const int nResult);
    void callbackProgress(const bool bResult, const int nResult);
    void callbackAds(const bool bResult, const int nResult);
    void callBackResultAds(ADS_RESULT result);
private:
    bool _bInitUI;
    bool _bInitNetworkInfo;
    
    cocos2d::ui::Layout* _uiTop;
    cocos2d::ui::Layout* _uiCurrentLvInfo;
    cocos2d::ui::Layout* _uiMain;
};

#endif /* PopupEventLuckyBagDefault_hpp */
