//
//  UIBannerPurchase.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/07.
//

#ifndef UIBannerPurchase_hpp
#define UIBannerPurchase_hpp

#include "Common/ConfigDefault.h"

class UIBannerPurchase : public cocos2d::Node
{
public:
    static UIBannerPurchase* create();
    
    UIBannerPurchase();
    virtual ~UIBannerPurchase();
    
    virtual bool init() override;
    
    
    void onClick(cocos2d::Ref* pSender);
    
    void callbackRedraw(bool bResult);
    
    void setCallbackReload(const std::function<void(void)>& pCallback);
    
private:
    
    cocos2d::ui::Button* _button;
    cocos2d::Label* _lbTimer;
    std::function<void(void)> _callbackReload;
    
    
};

#endif /* UIBannerPurchase_hpp */
