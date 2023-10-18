//
//  PopupFrostResult.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef PopupFrostResult_h
#define PopupFrostResult_h

#include "GameUIPopup/Base/PopupBase.h"

class InfoFrostFloor;
class PopupFrostResult : public PopupBase
{
public:
    static PopupFrostResult* create();
    
    PopupFrostResult(void);
    virtual ~PopupFrostResult(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
public:
    void setResult(bool bResult, std::string reward = "");
    
public:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContainer();
    void uiInfo();
    void uiButtons();
    
    // ui : draw
    void drawInfo();
    void drawButtons();
    
    // ui : set, get
    
    // callback
    
    // click
    void onClickClose(cocos2d::Ref* sender);

private:
    bool _bInitUI;
    
    bool _bResult;
    std::string _reward;
    
    // widget
    cocos2d::ui::Layout* _uiContentsUIInfo;
    cocos2d::ui::Layout* _uiContentsUIButtons;
 
};
#endif /* PopupFrostResult_h */
