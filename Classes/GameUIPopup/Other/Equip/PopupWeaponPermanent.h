//
//  PopupWeaponPermanent.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 23/09/2019.
//

#ifndef PopupWeaponPermanent_h
#define PopupWeaponPermanent_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupWeaponPermanent : public PopupBase
{
public:
    static PopupWeaponPermanent* create(int weaponIdx);
    
    PopupWeaponPermanent(void);
    virtual ~PopupWeaponPermanent(void);
    virtual bool init(int weaponIdx);
    virtual void onEnter() override;
    
public:
    // set
    void setCallbackResult(const std::function<void(int)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiInfo();
    void uiButtons();
    
    //draw
    void drawInfo();
    void drawButtons();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickPermanent(cocos2d::Ref* sender);
    
private:
    bool _bInitUI;
    
    //
    int _weaponIdx;
    
    //
    std::function<void(int)> _callbackResult;
    
    // widget
    cocos2d::ui::Layout* _layerContainerInfo;
    cocos2d::ui::Layout* _layerContainerButtons;
   
    cocos2d::ui::Button* _uiBtnBuy;
};


#endif /* PopupWeaponPermanent_hpp */
