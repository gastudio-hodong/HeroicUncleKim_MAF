//
//  PopupPurchaseFirst.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupPurchaseFirst_h
#define PopupPurchaseFirst_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupPurchaseFirst : public PopupBase
{
public:
    static PopupPurchaseFirst* create();
    
    PopupPurchaseFirst(void);
    virtual ~PopupPurchaseFirst(void);
    virtual bool init() override;
    virtual void onEnter() override;
public:
    // set
    void setCallbackResult(const std::function<void(int)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContents();
    void uiContentsMiddle();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickGet(cocos2d::Ref* sender);
    
    //
    void callbackInfo(bool bResult);
    void callbackReward(bool bResult, std::string strReward);
    void callbackCashPension(bool bResult);
    void callbackSave(bool bResult);
    
private:
    //
    std::function<void(int)> _callbackResult;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerContentsMiddle;
};

#endif /* PopupPurchaseFirst_h */
