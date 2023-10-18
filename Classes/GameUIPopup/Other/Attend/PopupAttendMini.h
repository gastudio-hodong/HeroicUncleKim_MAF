//
//  PopupAttendMini.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 05/11/2019.
//

#ifndef PopupAttendMini_h
#define PopupAttendMini_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupAttendMini : public PopupBase
{
public:
    static PopupAttendMini* create();
    
    PopupAttendMini();
    virtual ~PopupAttendMini();
    virtual bool init();
    virtual void onEnter();
protected:

    //void
    void initVar();
    void initUi();
    
    //ui
    void uiContents();
    
    //draw
    void drawButton();
    
    // schedule
    void scheduleHide(float dt);
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickAttend(cocos2d::Ref* sender);
    
private:
    
    int _nTimeHide;
    int _nTimeHideMax;
    bool _bHide;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Label* _labelButtonClose;
};

#endif /* PopupAttendMini_h */
