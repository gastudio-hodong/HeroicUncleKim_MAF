//
//  UILayerPurchaseProduct.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef UILayerCostumeParent_h
#define UILayerCostumeParent_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class UILayerCostumeParent : public cocos2d::LayerColor
{
public:
    UILayerCostumeParent(void);
    virtual ~UILayerCostumeParent(void);
    virtual bool init();
    
public:
    // set, get, add
    virtual void refreshList() = 0;
    
protected:
    // ui


protected:
    
};

#endif /* UILayerPurchaseProduct_h */
