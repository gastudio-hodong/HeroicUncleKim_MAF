//
//  PopupPurchaseReturn.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupPurchaseReturn_h
#define PopupPurchaseReturn_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupPurchaseReturn : public PopupBase
{
public:
    static PopupPurchaseReturn* create();
    
    PopupPurchaseReturn(void);
    virtual ~PopupPurchaseReturn(void);
    virtual bool init();
    virtual void onEnter();
    
public:
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContentsTop();
    void uiContentsMiddle();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickPurchase(cocos2d::Ref* sender);
    
    // callback
    void callbackPurchaseRemaining(bool bResult);
    void callbackPurchaseRemainingInitiate(bool bResult);
    
    void callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
    void callbackCashPension(bool bResult);
    
private:
    //
    int _nItemIdx;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerContentsTop;
    cocos2d::Layer* _layerContainerContentsMiddle;
};

#endif /* PopupPurchaseFirst_h */
