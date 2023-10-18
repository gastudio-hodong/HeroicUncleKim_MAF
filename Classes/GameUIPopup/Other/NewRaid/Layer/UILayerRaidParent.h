//
//  UILayerRaidParent.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/04.
//

#ifndef UILayerRaidParent_hpp
#define UILayerRaidParent_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class UILayerRaidParent : public cocos2d::LayerColor
{
public:
    UILayerRaidParent(void);
    virtual ~UILayerRaidParent(void);
    virtual bool init();
    
public:
    // set, get, add
    virtual void refreshUI() = 0;
    
protected:
    // ui


protected:
    
};

#endif /* UILayerRaidParent_hpp */
