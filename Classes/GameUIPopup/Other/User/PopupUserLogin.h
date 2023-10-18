//
//  PopupUserLogin.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/23.
//

#ifndef PopupUserLogin_hpp
#define PopupUserLogin_hpp

#include "GameUIPopup/Base/PopupBase.h"

class PopupUserLogin : public PopupBase
{
public:
    CREATE_FUNC(PopupUserLogin);
    
    PopupUserLogin();
    virtual ~PopupUserLogin(void);
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
    
    // ui : draw
    void drawInfo();
    
    //
    
    //
    void onClickLogin(Ref* sender);
    void onClickGuest(Ref* sender);
    
    // callback
    void callbackGuest();
     
private:
    bool _bInitUI;
    
    //
    std::function<void(void)> _callbackResult;
    
    //
    cocos2d::ui::Layout* _uiContentsInfo;
};

#endif /* PopupUserLogin_hpp */
