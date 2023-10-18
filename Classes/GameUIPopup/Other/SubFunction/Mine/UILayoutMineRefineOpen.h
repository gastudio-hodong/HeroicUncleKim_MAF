//
//  UILayoutMineRefineOpen.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef UILayoutMineRefineOpen_h
#define UILayoutMineRefineOpen_h

#include "GameUIPopup/Base/PopupBase.h"

class UILayoutMineRefineOpen : public cocos2d::ui::Layout
{
public:
    static UILayoutMineRefineOpen* create();
    
    UILayoutMineRefineOpen(void);
    virtual ~UILayoutMineRefineOpen(void) {};
    virtual bool init() override;
    virtual void onEnter() override;
    
public:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContainer();
    void uiInfo();
    void uiMine();
    void uiButtons();
    
    // ui draw
    void drawInfo();
    void drawInfoPet();
    void drawMine();
    void drawButtons();
    
    // callback
    void callbackPurchaseOk(int nIdx);
    void callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
    
    // click
    void onClickMine(Ref* sender);
    void onClickOpen(Ref* sender);
    

private:
    bool _bInitUI;
    
    //
    E_PLACE _ePlace;
    
    
    // widget
    cocos2d::ui::Layout* _uiContainerInfo;
    cocos2d::ui::Layout* _uiContainerMine;
    cocos2d::ui::Layout* _uiContainerButtons;
    
    cocos2d::ui::ImageView* _uiPet;
    
};
#endif /* UILayoutMineRefineOpen_h */
