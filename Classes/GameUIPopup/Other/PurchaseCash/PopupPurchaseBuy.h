//
//  PopupPurchaseBuy.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupPurchaseBuy_h
#define PopupPurchaseBuy_h

#include "GameUIPopup/Base/PopupBase.h"

class InfoProduct;
class PopupPurchaseBuy : public PopupBase
{
public:
    static PopupPurchaseBuy* create(int nIdx);
    
    PopupPurchaseBuy(void);
    virtual ~PopupPurchaseBuy(void);
    virtual bool init(int nIdx);
    
public:
    // set
    void setCallbackResult(const std::function<void(int)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContents();
    void uiButton();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickBuy(cocos2d::Ref* sender);
    
private:
    InfoProduct* _objProduct;
    std::function<void(int)> _callbackResult;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerButton;
     
};

#endif /* PopupPurchaseBuy_h */
