//
//  UILayerDefensePurchase.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/11.
//

#ifndef UILayerDefensePurchase_hpp
#define UILayerDefensePurchase_hpp


#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/Raid/InfoRaidBongStat.h"
#include "GameObject/InfoProduct.h"

class UILayerDefensePurchase : public cocos2d::LayerColor
{
public:
    static UILayerDefensePurchase* create(cocos2d::Size size);
    
    UILayerDefensePurchase(void);
    virtual ~UILayerDefensePurchase(void);
    virtual bool init(cocos2d::Size size);
    
    virtual void refreshUI();
    
public:
    // set, get
    void setCallbackLevelup(const std::function<void(void)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiList();
    cocos2d::ui::Layout* getUiGroup(int group, double width);
    
    // click
    void onClickProduct(cocos2d::Ref* sender);
    
    //callback
    void callbackInfo(bool bResult);
    void callbackProductOk(int nIdx);
    void callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
private:
    cocos2d::Layer* _lyProduct;
    cocos2d::ui::ScrollView* _layerContainerContentsScrollView;
    
    std::function<void(void)> _onCallbackInfo;
    
};

#endif /* UILayerDefensePurchase_hpp */
