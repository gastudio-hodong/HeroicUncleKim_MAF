//
//  UIReturnPurchase.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/07.
//

#ifndef UIReturnPurchase_hpp
#define UIReturnPurchase_hpp

#include "Common/ConfigDefault.h"

class UIReturnPurchase : public cocos2d::Node
{
public:
    static UIReturnPurchase* create();
    
    UIReturnPurchase();
    virtual ~UIReturnPurchase();
    
    virtual bool init() override;
    
    
    void onClick(cocos2d::Ref* pSender);
    
    void setCallbackReload(const std::function<void(void)>& pCallback);
    
private:
    
    cocos2d::ui::Button* _button;
    cocos2d::Label* _lbText;
    std::function<void(void)> _callbackReload;
    
    
};


#endif /* UIReturnPurchase_hpp */
