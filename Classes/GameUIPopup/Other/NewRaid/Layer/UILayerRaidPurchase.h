//
//  UILayerRaidPurchase.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/30.
//

#ifndef UILayerRaidPurchase_hpp
#define UILayerRaidPurchase_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/Raid/InfoRaidBongStat.h"
#include "GameObject/InfoProduct.h"

class UILayerRaidPurchase : public cocos2d::LayerColor
{
public:
    static UILayerRaidPurchase* create(cocos2d::Size size);
    
    UILayerRaidPurchase(void);
    virtual ~UILayerRaidPurchase(void);
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
    void uiTop();
    void uiList();
    cocos2d::ui::Layout* getUiGroup(int group, double width);
    
    // click
    void onClickProduct(cocos2d::Ref* sender);
    void onClickDoubleTicket(cocos2d::Ref* sender);
    
    //callback
    void callbackInfo(bool bResult);
    void callbackProductOk(int nIdx);
    void callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
private:
    cocos2d::Layer* _lyTopUI;
    cocos2d::Layer* _lyProduct;
    cocos2d::ui::ScrollView* _layerContainerContentsScrollView;
    
    std::function<void(void)> _onCallbackInfo;
    
};


#endif /* UILayerRaidPurchase_hpp */
