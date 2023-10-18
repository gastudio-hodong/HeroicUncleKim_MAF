//
//  PopupWithdrawal.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/04.
//

#ifndef PopupWithdrawal_hpp
#define PopupWithdrawal_hpp

#include "GameUIPopup/Base/PopupBase.h"

class PopupWithdrawal : public PopupBase
{
public:
    static PopupWithdrawal* create();
    
    PopupWithdrawal(void);
    virtual ~PopupWithdrawal(void);
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
    void onClickWithdrawal(cocos2d::Ref* sender);
    
    // callback
    void callbackWithdrawal(int nResult);
    
    
private:
    std::function<void(void)> _callbackResult;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerButton;
    
    cocos2d::ui::EditBox* _editBox;
};
#endif /* PopupWithdrawal_hpp */
