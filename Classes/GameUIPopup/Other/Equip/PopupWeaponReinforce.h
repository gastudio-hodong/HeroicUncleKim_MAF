//
//  PopupWeaponReinforce.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 23/09/2019.
//

#ifndef PopupWeaponReinforce_h
#define PopupWeaponReinforce_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupWeaponReinforce : public PopupBase
{
public:
    CREATE_FUNC(PopupWeaponReinforce);
    
    PopupWeaponReinforce(void);
    virtual ~PopupWeaponReinforce(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
public:
    void setWeaponIdx(int idx);
    
    
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
    void onClickReinforce(cocos2d::Ref* sender);
    
    
private:
    bool _bInitUI;
    
    //
    TInteger _idxWeapon;
    
    // widget
    cocos2d::ui::Layout* _uiContentsInfo;
    cocos2d::ui::Layout* _uiContentsButton;
};


#endif /* PopupWeaponReinforce_hpp */
