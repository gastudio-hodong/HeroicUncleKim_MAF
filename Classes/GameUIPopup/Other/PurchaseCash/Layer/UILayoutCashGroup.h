//
//  UILayoutCashGroup.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef UILayoutCashGroup_h
#define UILayoutCashGroup_h

#include "UILayoutCashBase.h"

class UILayoutCashGroup : public UILayoutCashBase
{
public:
    static UILayoutCashGroup* create(std::vector<int> list);
    
    UILayoutCashGroup(void);
    virtual ~UILayoutCashGroup(void) {};
    virtual bool init(std::vector<int> list);
    virtual void onEnter() override;
    
public:
    // game
    void onDraw() override;
    
protected:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContainer();
    void uiInfo();
    cocos2d::ui::Layout* getUiGroup(int group, double width);
    cocos2d::ui::Layout* getUiGroup5(int group, double width);
    cocos2d::ui::Layout* getUiGroup100(int group, double width);
    
    
    // ui draw
    void drawInfo();
    
    // callback
    
    // click
    void onClickProduct(cocos2d::Ref* sender);

    
    
private:
    bool _bInitUI;
    
    //
    std::vector<int> _listGroup;
    
    // widget
    cocos2d::ui::Layout* _uiContainerInfo;
    
};
#endif /* UILayoutCashGroup_h */
