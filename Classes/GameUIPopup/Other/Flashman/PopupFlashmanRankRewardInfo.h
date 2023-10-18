//
//  PopupFlashmanRankRewardInfo.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/08.
//

#ifndef PopupFlashmanRankRewardInfo_hpp
#define PopupFlashmanRankRewardInfo_hpp

#include "GameUIPopup/Base/PopupBase.h"

class InfoRank;
class PopupFlashmanRankRewardInfo : public PopupBase, public cocos2d::ui::DelegateListView
{
public:
    static PopupFlashmanRankRewardInfo* create();
    
    PopupFlashmanRankRewardInfo();
    virtual ~PopupFlashmanRankRewardInfo(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
    // listview
    virtual void listviewScrollView(cocos2d::ui::CListView* listview, cocos2d::ui::ScrollView::EventType type) override;
    virtual cocos2d::Size listviewCellSize(cocos2d::ui::CListView* listview, ssize_t idx) override;
    virtual void listviewCell(cocos2d::ui::CListView *listview, cocos2d::ui::Widget* childview, ssize_t idx) override;
    virtual int64_t listviewNumberOfCells(cocos2d::ui::CListView *listview) override;
    
protected:
    void initVar();
    void initUI();
    
    // ui
    void uiContainer();
    void uiDefault();
    void uiList();
    void uiButtons();
    
    // ui : draw
    void drawList();
    
    // set, get
    
    // callback
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    
private:
    bool _bInitUI;
    
    //
    cocos2d::Vector<InfoRank*> _listInfoRank;
    
    // widget
    cocos2d::ui::Layout* _uiContentsUIDefault;
    cocos2d::ui::Layout* _uiContentsUIList;
    cocos2d::ui::Layout* _uiContentsUIButtons;
    
    cocos2d::ui::CListView* _uiList;
};
 

#endif /* PopupFlashmanRankRewardInfo_hpp */
