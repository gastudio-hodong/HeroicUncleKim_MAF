//
//  LayerNormalDungeon.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/09/07.
//

#ifndef LayerNormalDungeon_hpp
#define LayerNormalDungeon_hpp


#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class LayerNormalDungeon : public cocos2d::ui::Layout, public cocos2d::ui::DelegateListView
{
public:
    static LayerNormalDungeon* create(E_PLACE ePlace);

    LayerNormalDungeon(void);
    virtual ~LayerNormalDungeon(void);
    virtual bool init(E_PLACE ePlace);
    virtual void onEnter() override;

    // listview
    virtual void listviewScrollView(cocos2d::ui::CListView* listview, cocos2d::ui::ScrollView::EventType type) override;
    virtual cocos2d::Size listviewCellSize(cocos2d::ui::CListView* listview, ssize_t idx) override;
    virtual void listviewCell(cocos2d::ui::CListView *listview, cocos2d::ui::Widget* childview, ssize_t idx) override;
    virtual int64_t listviewNumberOfCells(cocos2d::ui::CListView *listview) override;
    
public:
    void updateDungeonList();
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiList();
    
    // draw
    void drawList();
    
    //click
    void onClickDungeon(Ref* sender);
    
private:
    bool _isSceneMove;
    bool _bInitUI;
    //
    E_PLACE _ePlace;
    
    std::vector<E_DUNGEON> _dungeonList;
    
    cocos2d::ui::Layout* _uiContentsUIList;
    
    cocos2d::ui::CListView* _uiList;
};

#endif /* LayerNormalDungeon_hpp */
