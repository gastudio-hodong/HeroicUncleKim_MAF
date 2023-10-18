//
//  PopupUserNickname.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupUserNickname_h
#define PopupUserNickname_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupUserNickname : public PopupBase
{
public:
    static PopupUserNickname* create();
    
    PopupUserNickname(void);
    virtual ~PopupUserNickname(void);
    virtual bool init();
    
public:
    // set
    void setCallbackResult(const std::function<void(void)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContents();
    void uiButton();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickNickChange(cocos2d::Ref* sender);
    
    // callback
    void callbackNick(bool bResult, int nResult);
    
    
private:
    std::function<void(void)> _callbackResult;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerButton;
    
    cocos2d::ui::EditBox* _editBox;
};

#endif /* PopupUserNickname_h */
