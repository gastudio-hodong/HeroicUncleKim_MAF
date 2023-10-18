//
//  PopupFrostInfo.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef PopupFrostInfo_h
#define PopupFrostInfo_h

#include "GameUIPopup/Base/PopupBase.h"

class InfoFrostFloor;
class PopupFrostInfo : public PopupBase, public cocos2d::ui::DelegateListView
{
public:
    static PopupFrostInfo* create();
    
    PopupFrostInfo(void);
    virtual ~PopupFrostInfo(void);
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
    void uiList();
    
    // ui : draw
    void drawList();
    
    // ui : set, get
    
    // callback
    
    // click
    void onClickClose(cocos2d::Ref* sender);

private:
    bool _bInitUI;
    
    cocos2d::Vector<InfoFrostFloor*> _listFloor;
    
    // widget
    cocos2d::ui::Layout* _uiContentsUIList;
 
    cocos2d::ui::CListView* _uiList;
};
#endif /* PopupFrostInfo_h */
