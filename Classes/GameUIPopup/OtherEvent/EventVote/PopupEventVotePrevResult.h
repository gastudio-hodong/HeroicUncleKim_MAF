//
//  PopupEventVotePrevResult.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/29.
//

#ifndef PopupEventVotePrevResult_hpp
#define PopupEventVotePrevResult_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/EventVote/InfoEventVoteCandidate.h"

class PopupEventVotePrevResult : public PopupBase, public cocos2d::ui::DelegateListView
{
public:
    static PopupEventVotePrevResult* create();
    
    PopupEventVotePrevResult();
    virtual ~PopupEventVotePrevResult(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
    static bool sortVoteMinority(InfoEventVoteCandidate* a, InfoEventVoteCandidate* b);
    
    // listview
    virtual void listviewScrollView(cocos2d::ui::CListView* listview, cocos2d::ui::ScrollView::EventType type) override;
    virtual cocos2d::Size listviewCellSize(cocos2d::ui::CListView* listview, ssize_t idx) override;
    virtual void listviewCell(cocos2d::ui::CListView *listview, cocos2d::ui::Widget* childview, ssize_t idx) override;
    virtual int64_t listviewNumberOfCells(cocos2d::ui::CListView *listview) override;
protected:
    void initVar();
    void initUI();
    
    //ui
    void uiContainer();
    
    // ui : draw
    void drawList();
    
    void onClickClose(Ref* sender);
    //utils
    const float getPersent(InfoEventVoteCandidate* candidate);
    const int getPrevVoteAllCount();
    //callback
private:
    bool _bInitUI;
    cocos2d::Vector<InfoEventVoteCandidate*> _listInfoPrevVoteCandidate;
    
    cocos2d::ui::CListView* _uiList;
};

#endif /* PopupEventVotePrevResult_hpp */
