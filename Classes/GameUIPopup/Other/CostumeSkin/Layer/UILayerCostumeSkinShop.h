//
//  UILayerCostumeSkinShop.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef UILayerCostumeSkinShop_h
#define UILayerCostumeSkinShop_h

#include "GameUIPopup/Other/CostumeSkin/Layer/UILayerCostumeSkinParent.h"

class InfoCostumeSkinShop;
class UILayerCostumeSkinShop : public UILayerCostumeSkinParent, public cocos2d::ui::DelegateListView
{
public:
    static UILayerCostumeSkinShop* create();
    
    UILayerCostumeSkinShop(void);
    virtual ~UILayerCostumeSkinShop(void);
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void refreshList() override;
    
    // listview
    virtual void listviewScrollView(cocos2d::ui::CListView* listview, cocos2d::ui::ScrollView::EventType type) override;
    virtual cocos2d::Size listviewCellSize(cocos2d::ui::CListView* listview, ssize_t idx) override;
    virtual void listviewCell(cocos2d::ui::CListView *listview, cocos2d::ui::Widget* childview, ssize_t idx) override;
    virtual int64_t listviewNumberOfCells(cocos2d::ui::CListView *listview) override;
    
public:
    // set, get
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiList();
    
    // ui draw
    void drawList();
    
    // set, get
    void setListLoad();

    // click
    void onClickSkin(cocos2d::Ref* sender);
    
    // callback

private:
    cocos2d::Vector<InfoCostumeSkinShop*> _listSkinShop;
    
    // widget
    cocos2d::ui::Layout* _uiContainerList;
    
    cocos2d::ui::CListView* _uiList;
};

#endif /* UILayerCostumeSkinShop_h */
