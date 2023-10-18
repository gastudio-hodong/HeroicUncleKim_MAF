//
//  UILayoutMineTab.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef UILayoutMineTab_h
#define UILayoutMineTab_h

#include "GameUIPopup/Base/PopupBase.h"

#include "ui/CocosGUI.h"

class UILayoutMineTab : public cocos2d::ui::Layout
{
    enum class E_TAB {
        NONE = 0,
        
        MINE, // 광물
        REFINE, // 제련
    };
    
public:
    static UILayoutMineTab* create();
    
    UILayoutMineTab(void);
    virtual ~UILayoutMineTab(void) {};
    virtual bool init() override;
    virtual void onEnter() override;
 
public:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContainer();
    void uiTab();
    void uiContents();
    
    // ui draw
    void drawTab();
    void drawContents();

    // click
    void onClickTab(cocos2d::Ref* sender);
    
private:
    bool _bInitUI;

    //
    E_TAB _eTab;
    
    // widget
    cocos2d::ui::Layout* _uiContainerTab;
    cocos2d::ui::Layout* _uiContainerContents;
 
 
};
#endif /* UILayoutMineTab_h */
