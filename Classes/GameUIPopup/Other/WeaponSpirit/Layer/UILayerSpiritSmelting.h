//
//  UILayerSpiritSmelting.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/26.
//

#ifndef UILayerSpiritSmelting_hpp
#define UILayerSpiritSmelting_hpp

#include "UILayerSpiritParent.h"

#include "GameObject/Raid/InfoRaidBongStat.h"

class UILayerSpiritSmelting : public UILayerSpiritParent
{
public:
    static UILayerSpiritSmelting* create(cocos2d::Size size, InfoWeaponSpirit* info);
    
    UILayerSpiritSmelting(void);
    virtual ~UILayerSpiritSmelting(void);
    virtual bool init(cocos2d::Size size, InfoWeaponSpirit* info);
    
    virtual void refreshUI(InfoWeaponSpirit* info) override;
    
public:
    // set, get
    void callbackSmelting(bool bResult, int nResult);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiMain();
    
    void onClickSmelting(Ref* sender);
private:
    
    cocos2d::Layer* _lyUI;
    InfoWeaponSpirit* _infoSpirit;
    
    
    cocos2d::Label* _lbNowCnt;
    cocos2d::Label* _lbNextCnt;
};

#endif /* UILayerSpiritSmelting_hpp */
