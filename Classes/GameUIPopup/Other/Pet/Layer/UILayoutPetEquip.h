//
//  UILayoutPetEquip.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef UILayoutPetEquip_h
#define UILayoutPetEquip_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/Observer/IRefresh.h"

class InfoPetEquip;
class UILayoutPetEquip : public cocos2d::ui::Layout, public IRefresh, public cocos2d::ui::DelegateListView
{
public:
    static UILayoutPetEquip* create(E_PET ePet);

    UILayoutPetEquip(void);
    virtual ~UILayoutPetEquip(void);
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
    //
    void onPetEquipLevelUPAll();
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiInfo();
    void uiList();
    
    // draw
    void drawInfo();
    void drawList();
    
    // click
    void onClickDetail(cocos2d::Ref* sender);
    void onClickLevelUP(cocos2d::Ref* sender);
    void onClickLevelUPAll(cocos2d::Ref* sender);
    
    // callback
    void callbackEquipLevelUP(bool bResult);
    void callbackEquipLevelUPAll(bool bResult);
    
    
private:
    bool _bInitUI;
    
    //
    E_PLACE _ePlace;
    E_PET _ePet;
    
    cocos2d::Vector<InfoPetEquip*> _listPetEquip;
    
    // widget
    cocos2d::ui::Layout* _uiContentsUIInfo;
    cocos2d::ui::Layout* _uiContentsUIList;
    
    cocos2d::ui::CListView* _uiList;
    
};
#endif /* UILayoutPetEquip_h */
