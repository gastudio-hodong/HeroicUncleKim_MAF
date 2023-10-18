//
//  LayerFrostList.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef LayerFrostList_h
#define LayerFrostList_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"


class InfoFrostSkill;
class LayerFrostList : public cocos2d::ui::Layout, public cocos2d::ui::DelegateListView
{
public:
    static LayerFrostList* create(cocos2d::Size size);

    LayerFrostList(void);
    virtual ~LayerFrostList(void);
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
    void uiContainer();
    void uiInfo();
    void uiButton();
    
    // ui : draw
    void drawButton();
    void drawList();
    
    // ui : set, get
    
    // ui : click
    void onClickLevelUPType(cocos2d::Ref* sender);
    void onClickLevelUP(cocos2d::Ref* sender);
    void onClickReset(cocos2d::Ref* sender);
    void onClickResetOk(cocos2d::Ref* sender);
    
    
    // callback
    
    // schedule
    
    //
    
public:
    // game
    void onDrawList();

private:
    bool _bInitUI;
    
    int _levelUPType;
    
    cocos2d::Vector<InfoFrostSkill*> _listSkill;
    
    // widget
    cocos2d::ui::Layout* _uiContentsUIDefault;
    cocos2d::ui::Layout* _uiContentsUIButtons;
    
    cocos2d::ui::CListView* _uiList;

};
#endif /* LayerFrostList_h */
