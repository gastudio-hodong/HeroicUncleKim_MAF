//
//  UILayerRaidMain.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/03.
//

#ifndef UILayerRaidMain_hpp
#define UILayerRaidMain_hpp

#include "UILayerRaidParent.h"

#include "GameObject/Raid/InfoRaidBongStat.h"

class UnitBehaviorHeroAnother;
class UILayerRaidMain : public UILayerRaidParent
{
public:
    static UILayerRaidMain* create(cocos2d::Size size);
    
    UILayerRaidMain(void);
    virtual ~UILayerRaidMain(void);
    virtual bool init(cocos2d::Size size);
    
    virtual void refreshUI() override;
public:
    // set, get
    
    void callbackLevelup();
    void setCallbackHide(const std::function<void(void)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiBattle();
    void uiIcon();
    
    
    // click
    void onClickMission(cocos2d::Ref* pSender);
    void onClickHelp(cocos2d::Ref* pSender);
    void onClickChat(cocos2d::Ref* pSender);
    void onClickRank(cocos2d::Ref* pSender);
    void onClickWeekReward(cocos2d::Ref* pSender);
    
    void onClickAttack(cocos2d::Ref* pSender);
    void onClickTicketShop(cocos2d::Ref* pSender);
    
    //action
    void actionAttackStart(bool isCritical, bool isDoubleAttack);
    void actionAttackMiddle(bool isCritical, bool isDoubleAttack);
    void actionDamage(bool isCritical, bool isDoubleAttack);
    void actionAttackEnd(bool isCritical, bool isDoubleAttack);
    
    void actionLevelup();
    
    //callback
    void callbackAttack(int nResult, bool isCritical, bool isDoubleAttack, cocos2d::Vector<InfoItems*> listReward);
    
    void callbackProductOk(int nIdx);
    void callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
    void callbackHide();
    
private:
    
    cocos2d::Layer* _lyUI;
    cocos2d::Layer* _lyBattle;
    
    cocos2d::Sprite* _sprDragonHead;
    cocos2d::Sprite* _sprAttackFx;
    
    cocos2d::Label* _lbSeasonTimer;
    
    cocos2d::ProgressTimer* _progressHP;
    cocos2d::ProgressTimer* _progressCO_OP;
    
    UnitBehaviorHeroAnother* _hero;
    cocos2d::Vector<InfoItems*> _listReward;
    
    std::function<void(void)> _callbackHide;
};


#endif /* UILayerRaidMain_hpp */
