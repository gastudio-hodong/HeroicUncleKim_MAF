//
//  PopupFrost.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef PopupFrost_h
#define PopupFrost_h

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

class UICurrency;
class PopupFrost : public PopupBase, public IRefresh
{
public:
    static PopupFrost* create();
    
    PopupFrost(void);
    virtual ~PopupFrost(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;
    
public:
    void onBattle();
    
protected:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContainer();
    void uiDefault();
    void uiFloorInfo();
    
    // ui : draw
    void drawDefault();
    void drawFloorInfo();
    void drawBattle();
    void drawList();
    
    // ui : set, get
    
    // callback
    void callbackInfo(bool bResult, int nResult);
    void callbackSkillLevelUp(bool bResult, int nResult);
    void callbackSkillReset(bool bResult, int nResult);
    void callbackAds(ADS_RESULT result);
    void callbackAdsResult(bool bResult, int nResult);
    void callbackBattleEnter(int count, bool bAnimation);
    void callbackBattleResult(bool bResult, int nResult, std::string reward);
    void callbackBattleResultInfo(bool bResult, int nResult);
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickHelp(cocos2d::Ref* sender);
    void onClickTowerInfo(cocos2d::Ref* sender);
    void onClickAds(cocos2d::Ref* sender);
    void onClickBattle(cocos2d::Ref* sender);
    

private:
    bool _bInitUI;
    
    //
    bool _battleAnimation;
    
    // widget
    cocos2d::ui::Layout* _uiContentsBattle;
    cocos2d::ui::Layout* _uiContentsUIDefault;
    cocos2d::ui::Layout* _uiContentsUIFloorInfo;
    cocos2d::ui::Layout* _uiContentsUIList;
    
    
 
};
#endif /* PopupFrost_h */
