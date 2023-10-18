//
//  PopupPurchaseDailyReward.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupPurchaseDailyReward_h
#define PopupPurchaseDailyReward_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupPurchaseDailyReward : public PopupBase
{    
public:
    static PopupPurchaseDailyReward* create(std::string strMsg);
    
    PopupPurchaseDailyReward(void);
    virtual ~PopupPurchaseDailyReward(void);
    virtual bool init(std::string strMsg);
    
public:
    
    // set, get
    void setCallbackYes(const std::function<void(bool)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContent();
    
    // click
    
    
private:
    std::string _strMsg;
    
    
    // widget
    cocos2d::Layer* _layerContainerContent;
 
};

#endif /* PopupPurchaseDailyReward_h */
