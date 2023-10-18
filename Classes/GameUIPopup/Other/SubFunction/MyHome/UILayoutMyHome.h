//
//  UILayoutMyHome.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef UILayoutMyHome_h
#define UILayoutMyHome_h

#include "GameUIPopup/Base/PopupBase.h"

class UILayoutBoost;
class UILayoutMyHome : public cocos2d::ui::Layout, public cocos2d::ui::DelegateListView
{
public:
    static UILayoutMyHome* create();
    
    UILayoutMyHome(void);
    virtual ~UILayoutMyHome(void) {};
    virtual bool init() override;
    virtual void onEnter() override;
    
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
    void uiShopAds();
    void uiBoost();
    void uiList();
    
    // ui draw
    void drawBoost();
    void drawList();
    
    // set, get
    int getAdsWatchMax(int floor);
    
    // callback
    void callbackAds(ADS_RESULT result);
    
    // click
    void onClickItem(cocos2d::Ref* sender);
    void onClickAdsAsk(cocos2d::Ref* sender);
    void onClickUpgrade(cocos2d::Ref* sender);
    
    // game
    void reward();
    void upgrade();

private:
    E_PLACE _ePlace;
    E_MYHOME _eMyhome;
    std::vector<E_MYHOME> _listMyHome;
    
    // widget
    cocos2d::ui::Layout* _uiContainerShopAds;
    cocos2d::ui::Layout* _uiContainerBoost;
    cocos2d::ui::Layout* _uiContainerList;
    
    cocos2d::ui::CListView* _uiList;
    
    UILayoutBoost* _uiLayoutBoost;
 
};
#endif /* UILayoutMyHome_h */
