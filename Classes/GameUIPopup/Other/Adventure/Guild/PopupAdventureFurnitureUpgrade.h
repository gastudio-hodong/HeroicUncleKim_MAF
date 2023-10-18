//
//  PopupFurnitureUpgrade.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/19.
//

#ifndef PopupFurnitureUpgrade_hpp
#define PopupFurnitureUpgrade_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "Common/Observer/IRefresh.h"

class PopupAdventureFurnitureUpgrade : public PopupBase, public IRefresh
{
public:
    
    static PopupAdventureFurnitureUpgrade* create(const int nIdx);
    
    PopupAdventureFurnitureUpgrade();
    virtual ~PopupAdventureFurnitureUpgrade(void);
    virtual bool init(const int nIdx);
    virtual void show() override;
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData)  override;
    
    //
    void setCallbackUpgrade(const std::function<void(void)>& callback);
    void showButtonForceTutorial();
    void showTutorial();
    
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
    
    //
    bool isUpgradeCondition();
    //
    void callbackUpgrade(const bool bResult, const int nResult);
    void callbackPurchaseOk(const int nIdx);
    void callbackPurchase(const MafGooglePlay::RESULT eResult, const int nResult, const int nIdx, const std::string strItems);
    
    void callbackLevelUp(const bool bResult);
    
private:
    int _nIdx;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerMine;
    cocos2d::Layer* _layerContainerButton;
    cocos2d::ui::Button* _btnTutorial;
    
    std::function<void(void)> _callbackUpgrade;
};

#endif /* PopupFurnitureUpgrade_hpp */
