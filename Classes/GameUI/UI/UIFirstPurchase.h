//
//  UIFirstPurchase.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/07.
//

#ifndef UIFirstPurchase_hpp
#define UIFirstPurchase_hpp

#include "Common/ConfigDefault.h"

class UIFirstPurchase : public cocos2d::Node
{
public:
    static UIFirstPurchase* create();
    
    UIFirstPurchase();
    virtual ~UIFirstPurchase();
    
    virtual bool init() override;
    
    
    void onClick(cocos2d::Ref* pSender);
    
    
    void callbackPurchaseFirstBonus(int nType);
    void setCallbackReload(const std::function<void(void)>& pCallback);
    
private:
    
    cocos2d::ui::Button* _button;
    cocos2d::Label* _lbText;
    std::function<void(void)> _callbackReload;
    
    
};

#endif /* UIFirstPurchase_hpp */
