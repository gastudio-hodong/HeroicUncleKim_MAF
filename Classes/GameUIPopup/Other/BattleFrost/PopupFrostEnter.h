//
//  PopupFrostEnter.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef PopupFrostEnter_h
#define PopupFrostEnter_h

#include "GameUIPopup/Base/PopupBase.h"

class InfoFrostFloor;
class PopupFrostEnter : public PopupBase
{
public:
    static PopupFrostEnter* create();
    
    PopupFrostEnter(void);
    virtual ~PopupFrostEnter(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
public:
    void setCallback(const std::function<void(int, bool)>& callback);
    
public:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContainer();
    void uiInfo();
    void uiCount();
    void uiButtons();
    
    // ui : draw
    void drawCount();
    void drawButtons();
    
    // ui : set, get
    
    // callback
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickArrow(cocos2d::Ref* sender);
    void onClickEnter(cocos2d::Ref* sender);
    void onClickToggleAnimation(cocos2d::Ref* sender);

private:
    bool _bInitUI;
    
    int _countNow;
    int _countMax;
    bool _bToggleAnimation;
    
    std::function<void(int, bool)> _callbackResult;
    
    // widget
    cocos2d::ui::Layout* _uiContentsUIInfo;
    cocos2d::ui::Layout* _uiContentsUICount;
    cocos2d::ui::Layout* _uiContentsUIButtons;
 
};
#endif /* PopupFrostEnter_h */
