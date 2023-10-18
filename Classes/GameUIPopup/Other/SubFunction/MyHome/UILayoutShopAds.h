//
//  UILayerPurchaseProduct.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef UILayerPurchaseProduct_h
#define UILayerPurchaseProduct_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class UILayoutShopAds : public cocos2d::ui::Layout
{
public:
    static UILayoutShopAds* create(E_PLACE ePlace);
    
    UILayoutShopAds(void);
    virtual ~UILayoutShopAds(void);
    virtual bool init(E_PLACE ePlace);
    
public:
    
protected:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiShopAds();
    
    // ui draw
    void drawShopAds(float dt = 0);
    
    // callback
    void callbackShopAds(ADS_RESULT result);

    // click
    void onClickShopAds(cocos2d::Ref* sender);
    
    // game
    void reward();
    
    
protected:
    E_PLACE _ePlace;
    
    E_SHOP_ADS _eShopAds;
    std::vector<E_SHOP_ADS> _listShopAds;
    std::vector<std::string> _listShopAdsIcon;
    
    // widget
    cocos2d::ui::Layout* _layerContainer;
};

#endif /* UILayerPurchaseProduct_h */
