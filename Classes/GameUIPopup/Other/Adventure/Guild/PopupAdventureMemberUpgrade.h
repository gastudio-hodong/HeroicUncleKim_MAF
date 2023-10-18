//
//  PopupPartyUpgrade.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/22.
//

#ifndef PopupPartyUpgrade_hpp
#define PopupPartyUpgrade_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "Common/Observer/IRefresh.h"

class PopupAdventureMemberUpgrade : public PopupBase, public IRefresh
{
public:
    
    static PopupAdventureMemberUpgrade* create(const int nParty, const int nIdx);
    
    PopupAdventureMemberUpgrade();
    virtual ~PopupAdventureMemberUpgrade(void);
    virtual bool init(const int nparty, const int nIdx);
    virtual void show() override;
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData)  override;
    
    //
    void setCallbackUpgrade(const std::function<void(void)>& callback);
protected:
    
    // init
    void initUi();
    
    // ui
    void uiContents(bool isLevelup = false);
    void uiMine();
    void uiButton();
    
    //
    void drawMine();
    
    //click
    void onClickClose(Ref* const sender);
    void onClickUpgrade(Ref* const sender);
    void onClickMine(Ref* const sender);
    
    bool isUpgradeCondition();
    //
    void callbackUpgrade(const bool bResult, const int nResult);
    void callbackPurchaseOk(const int nIdx);
    void callbackPurchase(const MafGooglePlay::RESULT eResult, const int nResult, const int nIdx, std::string strItems);
    
    void callbackLevelUp(const bool bResult);
    
private:
    int _nParty;
    int _nIdx;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerMine;
    cocos2d::Layer* _layerContainerButton;
    
    
    std::function<void(void)> _callbackUpgrade;
};

#endif /* PopupPartyUpgrade_hpp */
