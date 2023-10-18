//
//  LayerDexEquip.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef LayerDexEquip_h
#define LayerDexEquip_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/Observer/IRefresh.h"

class InfoWeapon;
class LayerDexEquip : public cocos2d::ui::Layout, public cocos2d::ui::DelegateListView, public IRefresh
{
public:
    CREATE_FUNC(LayerDexEquip);

    LayerDexEquip(void);
    virtual ~LayerDexEquip(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
    // listview
    virtual void listviewScrollView(cocos2d::ui::CListView* listview, cocos2d::ui::ScrollView::EventType type) override;
    virtual cocos2d::Size listviewCellSize(cocos2d::ui::CListView* listview, ssize_t idx) override;
    virtual void listviewCell(cocos2d::ui::CListView *listview, cocos2d::ui::Widget* childview, ssize_t idx) override;
    virtual int64_t listviewNumberOfCells(cocos2d::ui::CListView *listview) override;

    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;
    
public:
    // set, get
    
    // event
    
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiInfo();
    void uiList();
    void uiEffect();
    
    // draw
    void drawInfo();
    void drawList();
    void drawEffect();
    
    // click
    void onClickHelp(cocos2d::Ref* sender);
    void onClickWeapon(cocos2d::Ref* sender);
    void onClickPermanent(cocos2d::Ref* sender);
    void onClickReinforce(cocos2d::Ref* sender);
    
    // callback
    void callbackPermanent(int idx);
     
    
private:
    bool _bInitUI;
    
    //
    int _nWeaponSelectIdx;
    
    cocos2d::Vector<InfoWeapon*> _listWeapon;
    
    // widget
    cocos2d::ui::Layout* _uiContentsInfo;
    cocos2d::ui::Layout* _uiContentsList;
    cocos2d::ui::Layout* _uiContentsEffect;
    
    // table
    cocos2d::ui::CListView* _uiList;
    
};
#endif /* LayerDexEquip_h */
