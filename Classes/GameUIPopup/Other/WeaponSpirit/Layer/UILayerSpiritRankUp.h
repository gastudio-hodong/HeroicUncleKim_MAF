//
//  UILayerSpiritRankUp.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/23.
//

#ifndef UILayerSpiritRankUp_hpp
#define UILayerSpiritRankUp_hpp

#include "UILayerSpiritParent.h"

#include "GameObject/Raid/InfoRaidBongStat.h"
#include "GameObject/InfoSetting.h"

class UILayerSpiritRankUp : public UILayerSpiritParent
{
public:
    static UILayerSpiritRankUp* create(cocos2d::Size size, InfoWeaponSpirit* info);
    
    UILayerSpiritRankUp(void);
    virtual ~UILayerSpiritRankUp(void);
    virtual bool init(cocos2d::Size size, InfoWeaponSpirit* info);
    
    virtual void refreshUI(InfoWeaponSpirit* info) override;
public:
    // set, get
    
    void callbackRankup(bool bResult, int nResult);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiMain();
    
    void onClickRankUp(cocos2d::Ref* sender);
    void onClickFluctuation(cocos2d::Ref* sender);
private:
    
    int _nMakeCount;
    int _nMaxMakeCount;
    
    cocos2d::Layer* _lyUI;
    InfoWeaponSpirit* _infoSpirit;
    InfoWeaponSpirit* _infoNextSpirit;
    
    InfoSetting*  _objRankUpSetting;
    cocos2d::ui::CText* _lbNowCnt;
    cocos2d::ui::CText* _lbNextCnt;
    cocos2d::Sprite* _sprEffect;
    
    cocos2d::Label* _lbRankUp;
};


#endif /* UILayerSpiritRankUp_hpp */
