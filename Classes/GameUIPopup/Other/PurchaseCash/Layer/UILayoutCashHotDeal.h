//
//  UILayoutCashHotDeal.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef UILayoutCashHotDeal_h
#define UILayoutCashHotDeal_h

#include "GameUIPopup/Base/PopupBase.h"

class UILayoutCashHotDeal : public cocos2d::ui::Layout
{
public:
    static UILayoutCashHotDeal* create();
    
    UILayoutCashHotDeal(void);
    virtual ~UILayoutCashHotDeal(void) {};
    virtual bool init() override;
    virtual void onEnter() override;
    
public:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContainer();
    void uiInfo();
    cocos2d::ui::Layout* getUiGrowthNormal();
    cocos2d::ui::Layout* getUiGrowthPrison();
    cocos2d::ui::Layout* getUiWeekBanner();
    cocos2d::ui::Layout* getUiFirstBonus();
    
    // ui draw
    void drawInfo();
    
    // callback
    
    // click
    

private:
    bool _bInitUI;
    
    //
    
    
    // widget
    cocos2d::ui::Layout* _uiContainerInfo;
    
};
#endif /* UILayoutCashHotDeal_h */
