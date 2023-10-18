//
//  PopupUserLoginGuest.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/23.
//

#ifndef PopupUserLoginGuest_hpp
#define PopupUserLoginGuest_hpp

#include "GameUIPopup/Base/PopupBase.h"

class PopupUserLoginGuest : public PopupBase
{
public:
    CREATE_FUNC(PopupUserLoginGuest);
    
    PopupUserLoginGuest();
    virtual ~PopupUserLoginGuest(void);
    virtual bool init() override;
    virtual void onEnter() override;
 
public:
    // set, get
    void setCallbackResult(const std::function<void(void)>& callback);
    
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
    
    // callback
     
private:
    bool _bInitUI;
    
    //
    std::function<void(void)> _callbackResult;
    
    //
    cocos2d::ui::Layout* _uiContentsInfo;
    cocos2d::ui::Layout* _uiContentsButton;
};

#endif /* PopupUserLoginGuest_hpp */
