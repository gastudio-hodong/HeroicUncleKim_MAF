//
//  UILayoutShopItem.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef UILayoutShopItem_h
#define UILayoutShopItem_h

#include "GameUIPopup/Base/PopupBase.h"

class UILayoutShopItem : public cocos2d::ui::Layout, public cocos2d::ui::DelegateListView
{
public:
    static UILayoutShopItem* create();
    
    UILayoutShopItem(void);
    virtual ~UILayoutShopItem(void) {};
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
    void uiList();
    
    // ui draw
    void drawList();
    
    //callback
    void callbackItemBuy(E_SHOP eType);
    void callbackItemBuyGold(E_SHOP eType);
    
    // click
    void onClickItemIcon(cocos2d::Ref* sender);
    void onClickItemBuyAsk(cocos2d::Ref* sender);
    void onClickItemBuy(cocos2d::Ref* sender);
    void onClickItemBuyGold(cocos2d::Ref* sender);

private:
    E_PLACE _ePlace;
    E_SHOP _eShopBuy;
    
    std::vector<E_SHOP> _listShop;
    
    
    // widget
    cocos2d::ui::Layout* _uiContainerShopAds;
    cocos2d::ui::Layout* _uiContainerList;
    
    cocos2d::ui::CListView* _uiList;
    
    
 
};
#endif /* UILayoutShopItem_h */
