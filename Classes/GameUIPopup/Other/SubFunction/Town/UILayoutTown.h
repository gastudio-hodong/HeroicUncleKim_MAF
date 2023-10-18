//
//  UILayoutTown.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef UILayoutTown_h
#define UILayoutTown_h

#include "GameUIPopup/Base/PopupBase.h"

class UILayoutTown : public cocos2d::ui::Layout
{
public:
    static UILayoutTown* create();
    
    UILayoutTown(void);
    virtual ~UILayoutTown(void) {};
    virtual bool init() override;
    virtual void onEnter() override;
    
public:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContainer();
    void uiTown();
    void uiUpgrade();
    
    // ui draw
    void drawTownBG();
    void drawTownGround();
    void drawBuilding();
    bool drawBuildingReward(double posX, double posY);
    void drawSatelliteLaunch();
    void drawSatellite();
    bool drawSatelliteReward(double posX, double posY);
    void drawTownAniBack();
    void drawTownAniFront();
    void drawTown();
    void drawUpgrade();
    
    void drawCellUpgrade(cocos2d::ui::Widget* childview,  E_TOWN eType);
    void drawCellUpgradeTower(cocos2d::ui::Widget* childview,  E_TOWN eType);
    void drawCellUpgradeSatellite(cocos2d::ui::Widget* childview,  E_TOWN eType);
    
    // callback
    void callbackInfo(bool bResult);
    void callbackRewardBuilding(bool bResult, int nResult);
    void callbackRewardSatellite(bool bResult, int nResult);
    
    // click
    void onClickTownArrow(cocos2d::Ref* sender);
    void onClickBuildingUpgradeAsk(cocos2d::Ref* sender);
    void onClickBuildingUpgrade(cocos2d::Ref* sender);
    void onClickBuildingReward(cocos2d::Ref* sender);
    void onClickSatelliteOpen(cocos2d::Ref* sender);
    void onClickSatelliteReward(cocos2d::Ref* sender);
    
    //
    void onMoveBuilding();
    void onMoveSatellite();

private:
    E_PLACE _ePlace;
    E_TOWN _eBuildingUpgrade;
    
    bool _bTownPos;
    
    // widget
    cocos2d::ui::Layout* _uiContainerTown;
    cocos2d::ui::Layout* _uiContainerUpgrade;
    
    cocos2d::ui::ScrollView* _uiScrollTown;
    
    cocos2d::ui::ImageView* _uiSatelliteLast;
    
 
};
#endif /* UILayoutTown_h */
