//
//  UILayerAdventurePurchase.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/17.
//

#ifndef UILayerAdventurePurchase_hpp
#define UILayerAdventurePurchase_hpp

#include "GameUIPopup/Other/Adventure/Layer/UILayerAdventureParent.h"

#include "GameObject/Raid/InfoRaidBongStat.h"
#include "GameObject/InfoProduct.h"

class UILayerAdventurePurchase : public UILayerAdventureParent
{
public:
    static UILayerAdventurePurchase* create(cocos2d::Size size);
    
    UILayerAdventurePurchase(void);
    virtual ~UILayerAdventurePurchase(void);
    virtual bool init(cocos2d::Size size);
    
    virtual void refreshUI() override;
    
    
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

#endif /* UILayerAdventurePurchase_hpp */
