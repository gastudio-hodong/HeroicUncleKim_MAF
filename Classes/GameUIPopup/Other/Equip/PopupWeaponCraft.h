//
//  PopupWeaponCraft.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 07/10/2019.
//

#ifndef PopupWeaponCraft_h
#define PopupWeaponCraft_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupWeaponCraft : public PopupBase
{
public:
    
    static PopupWeaponCraft* create(int weaponIdx);
    
    PopupWeaponCraft();
    virtual ~PopupWeaponCraft(void);
    virtual bool init(int weaponIdx);
    virtual void onEnter() override;
    
public:
    // set
    
protected:
    
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiInfo();
    void uiMine();
    void uiButtons();
    
    //
    void drawInfo();
    void drawMine();
    
    //click
    void onClickClose(cocos2d::Ref* sender);
    void onClickCraft(cocos2d::Ref* sender);
    void onClickMine(cocos2d::Ref* sender);
    
    // callback
    void callbackPurchaseOk(int nIdx);
    void callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
    
private:
    bool _bInitUI;
    
    //
    int _weaponIdx;
    
    // widget
    cocos2d::ui::Layout* _layerContainerInfo;
    cocos2d::ui::Layout* _layerContainerMine;
    cocos2d::ui::Layout* _layerContainerButtons;
    
    
};
#endif /* PopupWeaponCraft_h */
