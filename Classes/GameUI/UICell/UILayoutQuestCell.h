//
//  UILayoutQuestCell.h
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
class UILayoutQuestCell : public cocos2d::ui::Layout, public IRefresh, public cocos2d::ui::DelegateListView
{
public:
    static UILayoutQuestCell* create(E_QUEST eQuest = E_QUEST::QUEST_NONE);

    UILayoutQuestCell(void);
    virtual ~UILayoutQuestCell(void);
    virtual bool init(E_QUEST eQuest);
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
    void setLevelAdd(int levelAdd);
    
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
    void onClickLevelUP(cocos2d::Ref* sender);
    
    // callback
     
    
private:
    bool _bInitUI;
    
    //
    E_PLACE _ePlace;
    
    //
    E_QUEST _eQuest;
    std::vector<E_QUEST> _listQuest;
    
    //
    int _levelAdd;
    int _tutorialIdx;
    
    // widget
    cocos2d::ui::Layout* _uiContentsUIList;
    
    cocos2d::ui::CListView* _uiList;
    
};
#endif /* UILayoutWeaponCell_h */
