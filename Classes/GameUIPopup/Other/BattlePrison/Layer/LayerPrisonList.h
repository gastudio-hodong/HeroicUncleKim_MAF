//
//  LayerPrisonList.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef LayerPrisonList_h
#define LayerPrisonList_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoQuest;
class LayerPrisonList : public cocos2d::ui::Layout, public cocos2d::ui::DelegateListView
{
public:
    static LayerPrisonList* create(cocos2d::Size size);

    LayerPrisonList(void);
    virtual ~LayerPrisonList(void);
    virtual bool init(cocos2d::Size size);
    virtual void onEnter() override;
    virtual void onSizeChanged() override;
    
    // listview
    virtual void listviewScrollView(cocos2d::ui::CListView* listview, cocos2d::ui::ScrollView::EventType type) override;
    virtual cocos2d::Size listviewCellSize(cocos2d::ui::CListView* listview, ssize_t idx) override;
    virtual void listviewCell(cocos2d::ui::CListView *listview, cocos2d::ui::Widget* childview, ssize_t idx) override;
    virtual int64_t listviewNumberOfCells(cocos2d::ui::CListView *listview) override;
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui : defalut
    void uiInfo();
    
    // ui : draw
    void drawList();
    
    // ui : set, get
    
    // ui : click
    
    // callback
    
    // schedule
    
    //
    
public:
    // game
    void onDrawList();

private:
    bool _bInitUI;
    
    cocos2d::Vector<InfoQuest*> _listQuest;
    
    // widget
    cocos2d::ui::CListView* _uiList;

};
#endif /* LayerPrisonList_h */
