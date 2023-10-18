//
//  UILayerSpiritLevelup.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/24.
//

#ifndef UILayerSpiritLevelup_hpp
#define UILayerSpiritLevelup_hpp

#include "UILayerSpiritParent.h"

#include "GameObject/Raid/InfoRaidBongStat.h"

class UILayerSpiritLevelup : public UILayerSpiritParent
{
public:
    static UILayerSpiritLevelup* create(cocos2d::Size size, InfoWeaponSpirit* info);
    
    UILayerSpiritLevelup(void);
    virtual ~UILayerSpiritLevelup(void);
    virtual bool init(cocos2d::Size size, InfoWeaponSpirit* info);
    
    virtual void refreshUI(InfoWeaponSpirit* info) override;
public:
    // set, get
    
    void callbackLevelup(bool bResult, int nResult);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiMain();
    
    void onClickLevelUp(Ref* sender);
private:
    
    cocos2d::Layer* _lyUI;
    InfoWeaponSpirit* _infoSpirit;
    
    
    cocos2d::Label* _lbNowCnt;
    cocos2d::Label* _lbNextCnt;
};

#endif /* UILayerSpiritLevelup_hpp */
