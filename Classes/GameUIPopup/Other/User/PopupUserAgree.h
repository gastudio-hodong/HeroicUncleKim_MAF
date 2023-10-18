//
//  PopupUserAgree.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 23/09/2019.
//

#ifndef PopupUserAgree_h
#define PopupUserAgree_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupUserAgree : public PopupBase
{
private:
    struct structUserAgree
    {
        std::string strText;
        std::string strLink;
        bool bCheck;
    };
    
public:
    static PopupUserAgree* create();
    
    PopupUserAgree(void);
    virtual ~PopupUserAgree(void);
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
    void onClickOption(cocos2d::Ref* sender);
    void onClickLink(cocos2d::Ref* sender);
    void onClickOk(cocos2d::Ref* sender);
    
private:
    std::vector<structUserAgree> _listUserAgree;
    std::function<void(void)> _onCallbackResult;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerButton;
    
    
};

#endif /* PopupUserAgree_h */
