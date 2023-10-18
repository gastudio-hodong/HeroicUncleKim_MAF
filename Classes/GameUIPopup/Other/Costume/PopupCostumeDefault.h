//
//  PopupCostumeDefault.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 07/06/2019.
//

#ifndef PopupCostumeDefault_hpp
#define PopupCostumeDefault_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

class PopupCostumeDefault : public PopupBase, public IRefresh
{
public:
    static PopupCostumeDefault* create();
    
    PopupCostumeDefault(void);
    virtual ~PopupCostumeDefault(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData)  override;
    virtual void hide() override;
public:
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiTop();
    void uiButton();
    void uiContent();
    void uiContentBar();
    void uiContentCurrency();
    
    // ui draw
    void drawHero();
    void drawContent();
    void drawButton();
    void drawAdsSimple();
    void drawRedDotAction(cocos2d::Node *widget);
     
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickCashBanner(cocos2d::Ref* sender);
    void onClickAds(cocos2d::Ref* sender);
    void onClickSkin(cocos2d::Ref* sender);
    void onClickPrison(cocos2d::Ref* sender);
    void onClickRaid(cocos2d::Ref* sender);
    void onClickButton(cocos2d::Ref* sender);
    void onClickButtonStone(cocos2d::Ref* sender);
    void onClickContentBar(cocos2d::Ref* sender);
    

    // callback
    void callbackInfo(bool bResult);
    void callbackAds(ADS_RESULT eResult);
    void callbackAdsResult(bool bResult, int nCurrency);
 
    
private:
    
    std::string _strType;
    cocos2d::Size _sizeContent;
    
    
    // widget
    cocos2d::Layer* _layerContainerParent;
    cocos2d::Layer* _layerContainerTop;
    cocos2d::Layer* _layerContainerContent;
    cocos2d::Layer* _layerContainerContentBar;
    cocos2d::Layer* _layerContainerContentCurrency;
    cocos2d::Layer* _layerContainerContentChild;
    cocos2d::Layer* _layerContainerButton;
    
    cocos2d::Layer* _layerHero;
    
    
    MafNode::MafMenuItemSprite *_itemAdsSimple;
    
    
};
#endif /* PopupCostumeDefault_hpp */
