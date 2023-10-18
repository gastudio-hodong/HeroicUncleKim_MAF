//
//  PopupPrison.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef PopupPrison_h
#define PopupPrison_h

#include "GameUIPopup/Base/PopupBase.h"

#include "ManagerGame/PrisonManager.h"

class UICurrency;
class PopupPrison : public PopupBase
{
public:
    static PopupPrison* create();
    
    PopupPrison(void);
    virtual ~PopupPrison(void);
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void backkey() override;
    
private:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContentsUI();
    void uiContentsUISubMenu();
    void uiContentsUIBattle();
    void uiContentsUIList();
    
    // ui : draw
    void drawUI();
    void drawUIEnter();
    void drawUISubMenu();
    void drawUIBattle();
    void drawUIList();
    
    // ui : set, get
    
    // callback
    void callbackDungeonInfo(bool bResult, bool bAvailable, int nType);
    void callbackDungeonInfoTime();
    void callbackDungeonResultSkip(bool bResult);
    
    void callbackTicketUse();
    void callbackTicketBuy();
    void callbackEnter(int nCount);
    void callbackBeelzebub();
    void callbackAdsInterstitial(int nType);
    void callbackAdsInterstitialResult(bool bResult, int nResult);
    void callbackAdsInterstitialErrorReward(bool bResult, int nResult);
    void callbackBafometzSkip(int nCount);
    void callbackBafometzTicket();
    void callbackJellyHeart();
    void callbackNewRaidBreakTime(bool bResult, int nResult);
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickMinimize(cocos2d::Ref* sender);
    void onClickAchievement(cocos2d::Ref* sender);
    void onClickGrowthPackage(cocos2d::Ref* sender);
    void onClickHelp(cocos2d::Ref* sender);
    void onClickBuff(cocos2d::Ref* sender);
    void onClickMenu(cocos2d::Ref* sender);
    void onClickEnter(cocos2d::Ref* sender);

public:
    // game
    void onDraw();
    void onBattle();
    void onMinimize();
    void onTimingPackage(); // TimingPackage
    void onPopupAchievement();
    void onPopupGrowthPackage();
    void onPopupTicket();
    void onPopupBafometz();
    void onPopupBeelzebub();
    void onPopupJelly();
    void onPopupAdsInterstitial();

private:
    bool _bInitEnter;
    bool _bInitInfo;
    
    PrisonManager::PrisonType _eType;
    
    
    // widget
    cocos2d::ui::Layout* _uiContentsBattleParent;
    cocos2d::ui::Layout* _uiContentsBattle;
    
    cocos2d::ui::Layout* _uiContentsUITop;
    cocos2d::ui::Layout* _uiContentsUISubMenu;
    cocos2d::ui::Layout* _uiContentsUIListParent;
    cocos2d::ui::Layout* _uiContentsUIList;
    
    
    UICurrency* _uiCurrency;
    UICurrency* _uiCurrencyFire;
    MafNode::MafMenuItemSprite* _itemClose;
    MafNode::MafMenuItemSprite* _itemMinimize;
    
    cocos2d::ui::ImageView* _uiEnterInfoBG;
    cocos2d::ui::ImageView* _uiListTitleBG;
    
    cocos2d::ui::ImageView* _uiRaidWarningBG;
 
};
#endif /* PopupPrison_h */
