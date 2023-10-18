//
//  LayerNormalWeapon.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef LayerNormalWeapon_h
#define LayerNormalWeapon_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/Observer/IRefresh.h"

class LayerNormalWeapon : public cocos2d::ui::Layout, public IRefresh
{
public:
    static LayerNormalWeapon* create();

    LayerNormalWeapon(void);
    virtual ~LayerNormalWeapon(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;

public:
    void onListMove();
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui : defalut
    void uiContainer();
    void uiInfo();
    
    // ui : draw
    void drawInfo();
    
    // ui : set, get
    
    // ui : click
    void onClickUpgradeMax(cocos2d::Ref* sender);
    void onClickSpirit(cocos2d::Ref* sender);
    void onClickDictionary(cocos2d::Ref* sender);
    
    
    // callback
    void callbackWeaponBuyMax();
    
    // schedule
    void scheduleDPS(float dt);

private:
    bool _bInitUI;
    
    E_PLACE _ePlace;
    
    //
    std::string _dps;
    
    // widget
    cocos2d::ui::Layout* _uiContainerInfo;
    cocos2d::ui::Layout* _uiContainerList;

    cocos2d::ui::Button* _uiDictionary;
    cocos2d::ui::CText* _uiDPS;
    cocos2d::ui::CText* _uiDexBonus;
};
#endif /* LayerNormalWeapon_h */
