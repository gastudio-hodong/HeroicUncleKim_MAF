//
//  UILayerDefenseSummon.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/11.
//

#ifndef UILayerDefenseSummon_hpp
#define UILayerDefenseSummon_hpp


#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/Raid/InfoRaidBongStat.h"

#include "GameUIPopup/Other/NewRaid/PopupRaidShopDefault.h"

class UILayerDefenseSummon : public cocos2d::LayerColor
{
public:
    static UILayerDefenseSummon* create(cocos2d::Size size);
    
    UILayerDefenseSummon(void);
    virtual ~UILayerDefenseSummon(void);
    virtual bool init(cocos2d::Size size);
    
    virtual void refreshUI() ;
public:
    // set, get
    void setCloseCondition(bool value);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiTop();
    void uiBottom();
    
    void onClickInfo(Ref* sender);
    void onClickSummon(Ref* sender);
    void onClickSummonAds(Ref* sender);
    
    void callBackResultAds(ADS_RESULT result);
    void callbackSummon(bool bResult, int nResult, Vector<InfoItems*> listReward);
    void callbackTransferShop();
    void callbackSummonUIRefresh(bool bResult, int nResult);
private:
    bool _isCloseCondition;
    cocos2d::Layer* _lyTopUI;
    cocos2d::Layer* _lyBottomUI;
    
public:
    MafDelegate<void(void)> _onTrensferShop;
    MafDelegate<void(void)> _onHide;
};


#endif /* UILayerDefenseSummon_hpp */
