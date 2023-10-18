//
//  PopupAdventureBossSelect.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/10/13.
//

#ifndef PopupAdventureBossSelect_h
#define PopupAdventureBossSelect_h

#include "GameUIPopup/Base/PopupBase.h"

class InfoAdventureBoss;
class PopupAdventureBossSelect : public PopupBase, public cocos2d::ui::DelegateListView
{
public:
    CREATE_FUNC(PopupAdventureBossSelect);
    
    PopupAdventureBossSelect(void);
    virtual ~PopupAdventureBossSelect(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
    // listview
    virtual void listviewScrollView(cocos2d::ui::CListView* listview, cocos2d::ui::ScrollView::EventType type) override;
    virtual cocos2d::Size listviewCellSize(cocos2d::ui::CListView* listview, ssize_t idx) override;
    virtual void listviewCell(cocos2d::ui::CListView *listview, cocos2d::ui::Widget* childview, ssize_t idx) override;
    virtual int64_t listviewNumberOfCells(cocos2d::ui::CListView *listview) override;
    
public:
    // set
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiTab();
    void uiList();
    
    // draw
    void drawTab();
    void drawList();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickTab(cocos2d::Ref* sender);
    void onClickSelect(cocos2d::Ref* sender);
    
    // network
    
    // callback
    void callbackSelect(bool bResult, int nResult);
    
private:
    bool _bInitUI;
    
    //
    cocos2d::Vector<InfoAdventureBoss*> _listInfoBoss;
    
    int _tab;
    
    // widget
    cocos2d::ui::Layout* _uiContentsTab;
    cocos2d::ui::Layout* _uiContentsList;
    
    cocos2d::ui::CListView* _uiList;
};

#endif /* PopupAdventureBossSelect_hpp */
