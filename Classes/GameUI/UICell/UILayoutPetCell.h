//
//  UILayoutPetCell.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef UILayoutPetCell_h
#define UILayoutPetCell_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/Observer/IRefresh.h"

class UILayoutPetCell : public cocos2d::ui::Layout, public IRefresh, public cocos2d::ui::DelegateListView
{
public:
    static UILayoutPetCell* create(E_PET ePet = E_PET::PET_NONE);

    UILayoutPetCell(void);
    virtual ~UILayoutPetCell(void);
    virtual bool init(E_PET ePet);
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
    void setDetailPopup(bool bPopup);
    
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
    void onClickDetail(cocos2d::Ref* sender);
    void onClickPetFree(cocos2d::Ref* sender);
    void onClickPetLevelUP(cocos2d::Ref* sender);
    
    // callback
     
    
private:
    bool _bInitUI;
    
    //
    E_PLACE _ePlace;
    E_PET _ePet;
    
    std::vector<E_PET> _listPet;
    
    bool _bDetailPopup;
    
    // widget
    cocos2d::ui::Layout* _uiContentsUIList;
    
    cocos2d::ui::CListView* _uiList;
    
};
#endif /* UILayoutPetCell_h */
