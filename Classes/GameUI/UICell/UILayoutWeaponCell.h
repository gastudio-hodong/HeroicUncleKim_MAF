//
//  UILayoutWeaponCell.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef UILayoutWeaponCell_h
#define UILayoutWeaponCell_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/Observer/IRefresh.h"

class InfoWeapon;
class UILayoutWeaponCell : public cocos2d::ui::Layout, public IRefresh, public cocos2d::ui::DelegateListView
{
public:
    static UILayoutWeaponCell* create(int weaponEquip = E_WEAPON::WEAPON_NONE);

    UILayoutWeaponCell(void);
    virtual ~UILayoutWeaponCell(void);
    virtual bool init(int weaponEquip);
    virtual void onEnter() override;
    
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;

    // listview
    virtual void listviewScrollView(cocos2d::ui::CListView* listview, cocos2d::ui::ScrollView::EventType type) override;
    virtual cocos2d::Size listviewCellSize(cocos2d::ui::CListView* listview, ssize_t idx) override;
    virtual void listviewCell(cocos2d::ui::CListView *listview, cocos2d::ui::Widget* childview, ssize_t idx) override;
    virtual int64_t listviewNumberOfCells(cocos2d::ui::CListView *listview) override;
    
public:
    // set, get, add
    
    void onListMove();
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiList();
    
    // draw
    void drawList();
    
    // click
    void onClickWeapon(cocos2d::Ref* sender);
    void onClickBuyLegend(cocos2d::Ref* sender);
    void onClickBuy(cocos2d::Ref* sender);
    void onClickBuyPermanent(cocos2d::Ref* sender);
    void onClickBuyReinforce(cocos2d::Ref* sender);
    
    // callback
    void callbackBuyLegend(int idx);
    void callbackBuyPermanent(int idx);
    
     
    
private:
    bool _bInitUI;
    
    //
    E_PLACE _ePlace;
    
    int _weaponEquip;
    cocos2d::Vector<InfoWeapon*> _listEquip;
    
    //
    int _tutorialIdx;
    
    // widget
    cocos2d::ui::Layout* _uiContentsUIList;
    
    cocos2d::ui::CListView* _uiList;
    
};
#endif /* UILayoutWeaponCell_h */
