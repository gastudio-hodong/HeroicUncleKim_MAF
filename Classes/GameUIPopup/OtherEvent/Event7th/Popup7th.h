//
//  Popup7th.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef Popup7th_h
#define Popup7th_h

#include "GameUIPopup/Base/PopupBase.h"

class InfoEvent7th;
class InfoEvent7thPerson;
class Popup7th : public PopupBase, public cocos2d::ui::DelegateListView
{
public:
    static Popup7th* create();
    
    Popup7th(void);
    virtual ~Popup7th(void);
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
    void uiContent();
    void uiMissionCoop();
    void uiMissionPersonal();
    
    // ui draw
    void drawContent();
    void drawList();
    void drawListPersonalReward();
    void drawPersonal();
    void drawCellMissionCoop(cocos2d::ui::Widget* childview,  ssize_t idx);
    void drawCellMissionPersonalReward(cocos2d::ui::Widget* childview,  ssize_t idx);
    
    //callback
    void callbackInfo(bool bResult, int nResult);
    void callbackReward(bool bResult, int nResult, std::string strReward);
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickRewardCoop(cocos2d::Ref* sender);
    void onClickRewardPersonal(cocos2d::Ref* sender);
    
 
 
    
private:
     
    cocos2d::Vector<InfoEvent7th*> _listCoopInfo;
    cocos2d::Vector<InfoEvent7thPerson*> _listPersonalInfo;
    
    // widget
    cocos2d::ui::Layout* _uiContainerContents;
    cocos2d::ui::Layout* _uiContainerMissionCoop;
    cocos2d::ui::Layout* _uiContainerMissionPersonal;
    
    cocos2d::ui::CListView* _uiListMissionCoop;
    cocos2d::ui::CListView* _uiListMissionPersonalReward;
    
    cocos2d::ui::LoadingBar* _uiBarPersonal;
    cocos2d::ui::Button* _uiBtnPersonal;
    cocos2d::ui::CText* _uiTextPersonalName;
    cocos2d::ui::CText* _uiTextPersonalPercent;
 
};
#endif /* Popup7th_h */
