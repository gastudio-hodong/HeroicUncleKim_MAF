//
//  PopupBuff.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupBuff_h
#define PopupBuff_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupBuff : public PopupBase
{    
public:
    static PopupBuff* create(cocos2d::ValueVector list);
    
    PopupBuff(void);
    virtual ~PopupBuff(void);
    virtual bool init(cocos2d::ValueVector list);
    virtual void update(float dt) override;
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiDungeonPass();
    cocos2d::Layer* uiContent(E_SHOP eType, cocos2d::Size sizeBox);
    
    void reDrawAdsButton();
    //
    int getPrice(E_SHOP eType);
 
    
    // click
    void onClickBuy(cocos2d::Ref* sender);
    void onClickBuyOk(cocos2d::Ref* sender);
    void onClickDungeonPass(cocos2d::Ref* sender);
    
    
    void onClickAds(cocos2d::Ref* sender);
    void onClickAdsOk();
    void ResultAdsCallBack(ADS_RESULT result);
private:
    E_SHOP _eTypeShop;
    
    E_BUFF_TYPE _eAdsBuffType;
    
    cocos2d::ValueVector _list;
    
    // widget
    cocos2d::Layer* _layerContainerDungeonPass;
    
    cocos2d::Vector<MafNode::MafMenuItemSprite*> _vecAdsItem;
    
    float _nTimer;
    bool _isReward;
};

#endif /* PopupBuff_h */
