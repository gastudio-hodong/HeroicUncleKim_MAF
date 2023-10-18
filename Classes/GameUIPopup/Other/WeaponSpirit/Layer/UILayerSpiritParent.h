//
//  UILayerSpiritParent.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/23.
//

#ifndef UILayerSpiritParent_hpp
#define UILayerSpiritParent_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "GameObject/Raid/InfoWeaponSpirit.h"

class UILayerSpiritParent : public cocos2d::LayerColor
{
public:
    UILayerSpiritParent(void);
    virtual ~UILayerSpiritParent(void);
    virtual bool init();
    
public:
    // set, get, add
    virtual void refreshUI(InfoWeaponSpirit* info) = 0;
    
    virtual void setCallbackRedraw(const std::function<void(void)>& callback);
protected:
    // callback
    std::function<void(void)> _callbackRedraw;
    
protected:
    
};

#endif /* UILayerSpiritParent_hpp */
