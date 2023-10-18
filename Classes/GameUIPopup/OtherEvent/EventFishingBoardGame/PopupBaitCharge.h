//
//  PopupBaitCharge.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/07/13.
//

#ifndef PopupBaitCharge_h
#define PopupBaitCharge_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupBaitCharge : public PopupBase
{
public:
    static PopupBaitCharge* create();
    
    PopupBaitCharge(void);
    virtual ~PopupBaitCharge(void);
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
    void onClickArrow(cocos2d::Ref* sender);
    void onClickCharge(cocos2d::Ref* sender);
    
    // callback
    void callbackBuyBait(bool bResult, int nResult);
    
private:
    int _nCount;
    
    int _nBaitBuyRemaining;
    int _nBaitBuyMax;
    int _nBaitBuyPrice;
    
    
    std::function<void(void)> _callbackResult;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerButton;
    cocos2d::Label* _labelCount1;//1 단위
    cocos2d::Label* _labelCount10;//10 단위
    cocos2d::Label* _labelPrice;
};


#endif /* PopupBaitCharge_h */
