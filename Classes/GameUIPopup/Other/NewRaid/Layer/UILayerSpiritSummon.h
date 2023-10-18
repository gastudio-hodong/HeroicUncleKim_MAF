//
//  UILayerSpiritSummon.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/30.
//

#ifndef UILayerSpiritSummon_hpp
#define UILayerSpiritSummon_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/Raid/InfoRaidBongStat.h"

#include "GameUIPopup/Other/NewRaid/PopupRaidShopDefault.h"

class UILayerSpiritSummon : public cocos2d::LayerColor
{
public:
    static UILayerSpiritSummon* create(cocos2d::Size size);
    
    UILayerSpiritSummon(void);
    virtual ~UILayerSpiritSummon(void);
    virtual bool init(cocos2d::Size size);
    
    virtual void refreshUI() ;
public:
    // set, get
    void setCloseCondition(bool value);
    void setCallbackHide(const std::function<void(void)>& callback);
    void setCallbackTransferShop(const std::function<void(PopupRaidShopDefault::E_TYPE)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiTop();
    void uiMiddle();
    void uiBottom();
    
    void onClickTicketBuy(Ref* sender);
    void onClickInfo(Ref* sender);
    void onClickSummon(Ref* sender);
    void onClickSummonAds(Ref* sender);
    void onClickMileageReward(Ref* sender);
    void onClickSpiritList(Ref* sender);
    void onClickAdsDragonBall(Ref* sender);
    
    void callBackResultAds(ADS_RESULT result);
    void callbackSummon(bool bResult, int nResult, Vector<InfoItems*> listReward);
    void callbackProductOk(int nIdx);
    void callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
    void callbackMileageReward(bool bResult, int nResult);
    void callbackAdsDragonBall(ADS_RESULT result);
    void callbackAdsDragonBallBuy(bool bResult, int nResult);
    void callbackTransferShop();
private:
    bool _isCloseCondition;
    cocos2d::Layer* _lyTopUI;
    cocos2d::Layer* _lyMiddleUI;
    cocos2d::Layer* _lyBottomUI;
    
    std::function<void(void)> _callbackHide;
    std::function<void(PopupRaidShopDefault::E_TYPE)> _callbackSetTab;
};

#endif /* UILayerSpiritSummon_hpp */
