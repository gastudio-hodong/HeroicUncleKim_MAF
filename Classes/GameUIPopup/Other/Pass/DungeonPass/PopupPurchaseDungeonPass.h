//
//  PopupPurchaseDungeonPass.h
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/09/17.
//

#ifndef PopupPurchaseDungeonPass_h
#define PopupPurchaseDungeonPass_h

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/InfoProduct.h"

class PassInfo;
class PassStep;
class PopupPurchaseDungeonPass : public PopupBase
{
public:
    static PopupPurchaseDungeonPass* create();
        
    PopupPurchaseDungeonPass();
    virtual ~PopupPurchaseDungeonPass(void);
    virtual bool init() override;
    
public:
    void setCallback(std::function<void(void)> pCallback);
    
protected:
        
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiReward();
    void uiBuff();
    
    //
    void drawTime();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickPurchase(cocos2d::Ref* sender);
    
    // callback
    void callbackInAppPurchasePass(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
    void callbackInfo(bool bResult);
    
private:
    PassInfo* _objPass;
    cocos2d::Vector<PassStep*> _listPassStep;
    
    cocos2d::Vector<InfoItems*> _listRewardAll;
    cocos2d::Vector<InfoItems*> _listRewardNow;
    
    // callback
    std::function<void(void)> _callbackRedraw;
    
    // widget
    cocos2d::Layer* _layerContainerReward;
    cocos2d::Layer* _layerContainerBuff;
    
    cocos2d::Label* _labelTime;
};

#endif /* PopupPurchaseDungeonPass_h */
