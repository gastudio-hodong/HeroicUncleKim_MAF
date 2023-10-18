//
//  UILayerAdventureParent.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/15.
//

#ifndef UILayerAdventureParent_hpp
#define UILayerAdventureParent_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class UILayerAdventureParent : public cocos2d::LayerColor
{
public:
    UILayerAdventureParent(void);
    virtual ~UILayerAdventureParent(void);
    virtual bool init();
    
public:
    // set, get, add
    virtual void refreshUI() = 0;
    
protected:
    // ui


protected:
    
};

#endif /* UILayerAdventureParent_hpp */
