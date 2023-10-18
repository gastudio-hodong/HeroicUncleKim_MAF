//
//  PopupRaidDefault.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/28.
//

#ifndef PopupRaidDefault_hpp
#define PopupRaidDefault_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

class PopupRaidDefault : public PopupBase, public IRefresh
{
public:
    static PopupRaidDefault* create();
    
    PopupRaidDefault(void);
    virtual ~PopupRaidDefault(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData)  override;
    virtual void hide() override;
public:
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiMain();
    void uiStatus();
    void uiButton();
    
    // ui draw
    
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickShop(cocos2d::Ref* sender);
    void onClickSpirit(cocos2d::Ref* sender);

    // callback
    void callbackInfo(bool bResult, int nResult);
    void callbackHide();
 
    
private:
    
    // widget
    cocos2d::Layer* _lyContainerParent;
    cocos2d::Layer* _lyContainerTop;
    cocos2d::Layer* _lyContainerCurrency;
    cocos2d::Layer* _lyContainerContent;
    cocos2d::Layer* _lyContainerBottom;
};

#endif /* PopupRaidDefault_hpp */
