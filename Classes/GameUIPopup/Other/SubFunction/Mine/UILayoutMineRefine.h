//
//  UILayoutMineRefine.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef UILayoutMineRefine_h
#define UILayoutMineRefine_h

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

#include "GameObject/Mineral/InfoMineralRefineUpgrade.h"

class UILayoutMineRefine : public cocos2d::ui::Layout, public IRefresh, public cocos2d::ui::DelegateListView
{
public:
    static UILayoutMineRefine* create();
    
    UILayoutMineRefine(void);
    virtual ~UILayoutMineRefine(void) {};
    virtual bool init() override;
    virtual void onEnter() override;
    
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;

    // listview
    virtual void listviewScrollView(cocos2d::ui::CListView* listview, cocos2d::ui::ScrollView::EventType type) override;
    virtual cocos2d::Size listviewCellSize(cocos2d::ui::CListView* listview, ssize_t idx) override;
    virtual void listviewCell(cocos2d::ui::CListView *listview, cocos2d::ui::Widget* childview, ssize_t idx) override;
    virtual int64_t listviewNumberOfCells(cocos2d::ui::CListView *listview) override;
    
public:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContainer();
    void uiInfo();
    void uiList();
    
    // ui draw
    void drawInfo();
    void drawInfoPet();
    void drawList();
    
    // callback
    
    // click
    void onClickItemProduction(Ref* sender);
    void onClickItemReward(Ref* sender);
    void onClickUpgradeLevelUp(Ref* sender);
    void onClickShop(Ref* sender);

private:
    bool _bInitUI;
    
    //
    E_PLACE _ePlace;
    
    cocos2d::Vector<InfoMineralRefineUpgrade*> _listInfoRefineUpgrade;
    
    // widget
    cocos2d::ui::Layout* _uiContainerInfo;
    cocos2d::ui::Layout* _uiContainerList;
    
    cocos2d::ui::CListView* _uiList;
    cocos2d::ui::ImageView* _uiPet;
};
#endif /* UILayoutMineRefine_h */
