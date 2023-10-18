//
//  LayerEventVoteParticipation.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/23.
//

#ifndef LayerEventVoteParticipation_hpp
#define LayerEventVoteParticipation_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/Observer/IRefresh.h"

#include "GameObject/EventVote/InfoEventVoteAll.h"

class LayerEventVoteParticipation : public cocos2d::ui::Layout, public cocos2d::ui::DelegateListView
{
public:
    static LayerEventVoteParticipation* create(cocos2d::Size size);

    LayerEventVoteParticipation(void);
    virtual ~LayerEventVoteParticipation(void);
    virtual bool init(cocos2d::Size size);
    virtual void onEnter() override;
    
    virtual void onSizeChanged() override;
    
    // listview
    virtual void listviewScrollView(cocos2d::ui::CListView* listview, cocos2d::ui::ScrollView::EventType type) override;
    virtual cocos2d::Size listviewCellSize(cocos2d::ui::CListView* listview, ssize_t idx) override;
    virtual void listviewCell(cocos2d::ui::CListView *listview, cocos2d::ui::Widget* childview, ssize_t idx) override;
    virtual int64_t listviewNumberOfCells(cocos2d::ui::CListView *listview) override;
public:
    
    void onDrawInfo();
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui : defalut
    void uiContainer();
    void uiParticipation();
    
    // ui : draw
    void drawList();
    
    // ui : set, get
    
    // ui : click
    
    // callback
    
    // schedule
    
    //

private:
    bool _bInitUI;
    
    bool _bResult;
    
    
    cocos2d::Vector<InfoEventVoteAll*> _listInfoVoteAll;
    // unit, map
    cocos2d::Vector<cocos2d::Sprite*> _listMap;
    
    // widget
    cocos2d::ui::Layout* _uiContentsLobby;
    cocos2d::ui::CListView* _uiList;
};

#endif /* LayerEventVoteParticipation_hpp */
