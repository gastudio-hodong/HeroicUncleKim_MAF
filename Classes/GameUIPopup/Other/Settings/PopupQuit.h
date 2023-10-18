//
//  PopupQuit.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupQuit_h
#define PopupQuit_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupQuit : public PopupBase
{
public:
    static PopupQuit* create();
    
    PopupQuit(void);
    virtual ~PopupQuit(void);
    virtual bool init();
    
public:
    
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContents();
    void uiButton();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickQuit(cocos2d::Ref* sender);
    
private:
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerButton;
    
    
};

#endif /* PopupQuit_h */
