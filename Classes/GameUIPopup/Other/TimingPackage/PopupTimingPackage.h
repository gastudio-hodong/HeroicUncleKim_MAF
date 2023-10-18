//
//  PopupTimingPackage.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/12.
//

//
//  PopupTimingPackage.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/07/01.
//

#ifndef PopupTimingPackage_h
#define PopupTimingPackage_h

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/InfoTimingPackage.h"
#include "GameObject/InfoProduct.h"

class PopupTimingPackage : public PopupBase
{
public:
    static PopupTimingPackage* create(InfoTimingPackage* infoTimingPackage, bool isScaedule = true);
    
    PopupTimingPackage(void);
    virtual ~PopupTimingPackage(void);
    virtual bool init(InfoTimingPackage* infoTimingPackage, bool isScaedule);
    
    void scheduleUpdate(float dt);
    void scheduleTimer(float dt);
    
    void callbackInAppPurchasePass(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
    void callbackPurchase(bool bResult);
    
    void setCallbackHide(std::function<void(void)> pCallback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContents();
    void uiReward();
    void uiButton();
    
    void setContentText();
    
    // click
    void onClick(cocos2d::Ref* sender);
    
    void onHide();
private:
    // widget
    float _fHideTimer;
    float _fTimer;
    cocos2d::Layer* _layerContainerReward;
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerButton;
    
    cocos2d::Label* _lbContent;
    
    InfoTimingPackage* _infoTimingPackage;
    
    
    std::function<void(void)> _callbackHide;
};

#endif /* PopupTimingPackage_hpp */

