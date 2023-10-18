//
//  UILayerPurchaseProduct.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef UILayerCostumeSkinParent_h
#define UILayerCostumeSkinParent_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class UILayerCostumeSkinParent : public cocos2d::ui::Layout
{
public:
    UILayerCostumeSkinParent(void);
    virtual ~UILayerCostumeSkinParent(void);
    virtual bool init();
    
public:
    // set, get, add
    virtual void refreshList() = 0;
    
protected:
    // ui


protected:
    
};

#endif /* UILayerPurchaseProduct_h */
