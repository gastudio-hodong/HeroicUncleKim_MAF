//
//  PopupBonsikDexLootTradingMarket.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef PopupBonsikDexLootTradingMarket_h
#define PopupBonsikDexLootTradingMarket_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupBonsikDexLootTradingMarket : public PopupBase
{
public:
    static PopupBonsikDexLootTradingMarket* create();
    
    PopupBonsikDexLootTradingMarket(void);
    virtual ~PopupBonsikDexLootTradingMarket(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
protected:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContainer();
    
    // ui : draw
    
    // set, get
    
    // callback
    void callbackInfo(bool bResult);
    
    // schedule
    
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    
    
private:
    bool _bInitUI;
    
    //
    
    // widget
    cocos2d::ui::Layout* _uiContainer;
};
#endif /* PopupBonsikDexLootTradingMarket_h */
