//
//  PopupCostumeSkin.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 07/06/2019.
//

#ifndef PopupCostumeSkin_hpp
#define PopupCostumeSkin_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

class PopupCostumeSkin : public PopupBase, public IRefresh
{
public:
    enum class E_TYPE {
        LIST = 1,
        SHOP
    };
    
public:
    static PopupCostumeSkin* create();
    
    PopupCostumeSkin(void);
    virtual ~PopupCostumeSkin(void);
    virtual bool init() override;
    virtual void onEnter() override;
       
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;
    
public:
    void setTabType(E_TYPE eType);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiInfo();
    void uiTab();
    void uiTabContents();
    void uiStats();
    
    //
    void drawInfo();
    void drawTab();
    void drawTabContents();
    void drawStats();
    
    //
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickTab(cocos2d::Ref* sender);
    void onClickSkinEquip(cocos2d::Ref* sender);
    
    // callback
    void callbackInfo(bool bResult);
 
    
private:
    bool _bInitUI;
    
    //
    E_TYPE _eType;
    
    // widget
    cocos2d::ui::Layout* _uiContainerInfo;
    cocos2d::ui::Layout* _uiContainerInfoUnit;
    cocos2d::ui::Layout* _uiContainerTab;
    cocos2d::ui::Layout* _uiContainerTabContents;
    cocos2d::ui::Layout* _uiContainerStats;
    
    cocos2d::ui::Button* _uiInfoBtnSkinEquip;
    cocos2d::ui::Widget* _uiInfoNameBG;
    cocos2d::ui::Widget* _uiInfoBuffBG;
    
    cocos2d::ui::CText* _uiStats;
};
#endif /* PopupCostumeSkin_hpp */
