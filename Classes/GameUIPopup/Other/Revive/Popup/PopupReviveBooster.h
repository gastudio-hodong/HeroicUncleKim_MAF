//
//  PopupReviveBooster.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 24/06/2019.
//

#ifndef PopupReviveBooster_hpp
#define PopupReviveBooster_hpp

#include "GameUIPopup/Base/PopupBase.h"



class PopupReviveBooster : public PopupBase
{
public:
    static PopupReviveBooster* create();
    
    
    PopupReviveBooster(void);
    virtual ~PopupReviveBooster(void);
    virtual bool init();
    

public:
    void setCallbackResult(const std::function<void(void)>& callback);
    
protected:
    void initUi();
    void onClickBuyBooster(Ref* sender);
    
    
private:
    std::function<void(void)> _onCallbackResult;
};

#endif /* PopupReviveBooster_hpp */
