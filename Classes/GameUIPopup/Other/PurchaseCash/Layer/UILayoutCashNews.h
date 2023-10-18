//
//  UILayoutCashNews.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef UILayoutCashNews_h
#define UILayoutCashNews_h

#include "GameUIPopup/Base/PopupBase.h"

class UILayoutCashNews : public cocos2d::ui::Layout
{
public:
    static UILayoutCashNews* create();
    
    UILayoutCashNews(void);
    virtual ~UILayoutCashNews(void) {};
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void update(float dt) override;
    
public:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContainer();
    void uiInfo();
    
    // ui draw
    void drawInfo();
    
    // callback
    void callbackInfo(bool bResult);
    
    // click
    

private:
    bool _bInitUI;
    
    //
    std::vector<std::string> _listNews;
    
    double _time;
    bool _bTouch;
    
    // widget
    cocos2d::ui::Layout* _uiContainerInfo;
    
    cocos2d::ui::PageView* _uiList;
};
#endif /* UILayoutCashNews_h */
