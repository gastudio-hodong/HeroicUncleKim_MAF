//
//  PopupAccountTransferAutoCheck.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/09/28.
//

#ifndef PopupAccountTransferAutoCheck_h
#define PopupAccountTransferAutoCheck_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupAccountTransferAutoCheck : public PopupBase
{
public:
    static PopupAccountTransferAutoCheck* create(std::string strID);
    
    PopupAccountTransferAutoCheck(void);
    virtual ~PopupAccountTransferAutoCheck(void);
    virtual bool init(std::string strID);
    
public:
    void setCallback(const std::function<void(void)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickConfirm(cocos2d::Ref* sender);
    
private:
    std::string _strUUID;
    std::function<void(void)> _onCallbackReward;
 
};

#endif /* PopupAccountTransferAutoCheck_hpp */
