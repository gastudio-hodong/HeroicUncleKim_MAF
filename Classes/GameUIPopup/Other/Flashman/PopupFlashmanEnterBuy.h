//
//  PopupFlashmanEnterBuy.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef PopupFlashmanEnterBuy_h
#define PopupFlashmanEnterBuy_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupFlashmanEnterBuy : public PopupBase
{
public:
    static PopupFlashmanEnterBuy* create();
    
    PopupFlashmanEnterBuy(void);
    virtual ~PopupFlashmanEnterBuy(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
protected:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContainer();
    void uiDefault();
    void uiButtons();
    
    // ui : draw
    void drawButtons();
    
    // set, get
    
    // callback
    void callbackAds(ADS_RESULT result);
    void callbackBuy(bool bResult, int nResult);
    
    // schedule
    
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickBuyItem(cocos2d::Ref* sender);
    void onClickBuyAds(cocos2d::Ref* sender);
    
private:
    bool _bInitUI;
    
    //
    
    // widget
    cocos2d::ui::Layout* _uiContentsUIDefault;
    cocos2d::ui::Layout* _uiContentsUIButtons;
    
};
#endif /* PopupFlashmanEnterBuy_h */
