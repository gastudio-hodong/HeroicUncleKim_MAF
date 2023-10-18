//
//  PopupFlashmanMission.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef PopupFlashmanMission_h
#define PopupFlashmanMission_h

#include "GameUIPopup/Base/PopupBase.h"

class InfoQuest;
class InfoItems;
class PopupFlashmanMission : public PopupBase, public cocos2d::ui::DelegateListView
{
public:
    static PopupFlashmanMission* create();
    
    PopupFlashmanMission(void);
    virtual ~PopupFlashmanMission(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
    // listview
    virtual void listviewScrollView(cocos2d::ui::CListView* listview, cocos2d::ui::ScrollView::EventType type) override;
    virtual cocos2d::Size listviewCellSize(cocos2d::ui::CListView* listview, ssize_t idx) override;
    virtual void listviewCell(cocos2d::ui::CListView *listview, cocos2d::ui::Widget* childview, ssize_t idx) override;
    virtual int64_t listviewNumberOfCells(cocos2d::ui::CListView *listview) override;
    
protected:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContainer();
    void uiDefault();
    void uiList();
    void uiButtons();
    
    // ui : draw
    void drawDefault();
    void drawList();
    void drawButtons();
    
    // set, get
    
    // callback
    void callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
    void callbackInfo(bool bResult, int nResult);
    void callbackReward(bool bResult, int nResult);
    
    // schedule
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickReward(cocos2d::Ref* sender);
    void onClickRewardAll(cocos2d::Ref* sender);
    void onClickTicketBuy(cocos2d::Ref* sender);
private:
    bool _bInitUI;
    
    //
    cocos2d::Vector<InfoQuest*> _listInfoMission;
    cocos2d::Vector<InfoItems*> _listMissionRewardAll;
    
    // widget
    cocos2d::ui::Layout* _uiContentsUIDefault;
    cocos2d::ui::Layout* _uiContentsUIList;
    cocos2d::ui::Layout* _uiContentsUIButtons;
    
    cocos2d::ui::CListView* _uiList;
    
};
#endif /* PopupFlashmanMission_h */
