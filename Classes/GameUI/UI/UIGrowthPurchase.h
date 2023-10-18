//
//  UIGrowthPurchase.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/07.
//

#ifndef UIGrowthPurchase_hpp
#define UIGrowthPurchase_hpp

#include "Common/ConfigDefault.h"

#include "GameUI/UI/UIReddot.h"

class UIGrowthPurchase : public cocos2d::Node
{
public:
    static UIGrowthPurchase* create();
    
    UIGrowthPurchase();
    virtual ~UIGrowthPurchase();
    
    virtual bool init() override;
    
    
    void onClick(cocos2d::Ref* pSender);
    
    void setCallbackReload(const std::function<void(void)>& pCallback);
    
private:
    
    cocos2d::ui::Button* _button;
    cocos2d::Label* _lbText;
    std::function<void(void)> _callbackReload;
    
    
};


#endif /* UIGrowthPurchase_hpp */
