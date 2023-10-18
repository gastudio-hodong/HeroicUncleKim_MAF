//
//  PopupDungeonPassBadge.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/10/13.
//

#ifndef PopupDungeonPassBadge_h
#define PopupDungeonPassBadge_h

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/InfoBadge.h"

class PassInfo;
class PopupDungeonPassBadge : public PopupBase
{
public:
    static PopupDungeonPassBadge* create(bool bBadgeFree, bool bBadgePur);
    
    PopupDungeonPassBadge(void);
    virtual ~PopupDungeonPassBadge(void);
    virtual bool init(bool bBadgeFree, bool bBadgePur);
    virtual void onEnter() override;
    
public:
    void setCallbackPurchase(std::function<void(void)> callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContents();
    void uiBadge();
    void uiButton();
    
    // draw
    
    // set, get
    
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickPurchase(cocos2d::Ref* sender);
    
    // network
    
    // callback
    

    
private:
    PassInfo* _objPass;
    bool _bBadgeFree;
    bool _bBadgePur;
    
    std::function<void(void)> _callbackPurchase;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerBadge;
    cocos2d::Layer* _layerContainerButton;
    
};

#endif /* PopupDungeonPassBadge_hpp */
