//
//  PopupUserLoginGuestAgain.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/23.
//

#ifndef PopupUserLoginGuestAgain_hpp
#define PopupUserLoginGuestAgain_hpp

#include "GameUIPopup/Base/PopupBase.h"

class PopupUserLoginGuestAgain : public PopupBase
{
public:
    CREATE_FUNC(PopupUserLoginGuestAgain);
    
    PopupUserLoginGuestAgain();
    virtual ~PopupUserLoginGuestAgain(void);
    virtual bool init() override;
    virtual void onEnter() override;
 
public:
    // set, get
    
    // event
    
protected:
    void initVar();
    void initUI();
    
    //ui
    void uiContainer();
    void uiInfo();
    void uiButton();
    
    // ui : draw
    void drawInfo();
    void drawButton();
    
    //
    
    //
    void onClickClose(Ref* sender);
    void onClickOk(Ref* sender);
    void onClickHelp(Ref* sender);
    
    // callback
    void callbackNetworkCheck(bool bResult);
    void callbackNetworkLink(bool bResult);
     
private:
    bool _bInitUI;
    
    //
    
    //
    cocos2d::ui::Layout* _uiContentsInfo;
    cocos2d::ui::Layout* _uiContentsButton;
};

#endif /* PopupUserLoginGuestAgain_hpp */
