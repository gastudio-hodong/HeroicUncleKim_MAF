//
//  PopupMineRefineUpgrade.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#ifndef PopupMineRefineUpgrade_h
#define PopupMineRefineUpgrade_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupMineRefineUpgrade : public PopupBase
{
public:
    static PopupMineRefineUpgrade* create();
    
    PopupMineRefineUpgrade(void);
    virtual ~PopupMineRefineUpgrade(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
public:
    void setUpgradeIdx(int idx);
    
protected:
    
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiInfo();
    void uiMine();
    void uiButtons();
    
    //
    void drawInfo();
    void drawMine();
    void drawButtons();
    
    // get, set
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickLevelUp(cocos2d::Ref* sender);
    void onClickMine(cocos2d::Ref* sender);
    
    // callback
    void callbackPurchaseOk(int nIdx);
    void callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
    
private:
    bool _bInitUI;
    
    //
    int _idx;

    // widget
    cocos2d::ui::Layout* _uiContainerInfo;
    cocos2d::ui::Layout* _uiContainerMine;
    cocos2d::ui::Layout* _uiContainerButtons;
};

#endif /* PopupMineRefineUpgrade_h */
