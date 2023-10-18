//
//  PopupEventRaceMission.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#ifndef PopupEventRaceMission_h
#define PopupEventRaceMission_h

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

class InfoQuest;
class PopupEventRaceMission : public PopupBase, public cocos2d::ui::DelegateListView
{
public:
    static PopupEventRaceMission* create();
    
    PopupEventRaceMission(void);
    virtual ~PopupEventRaceMission(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
    // listview
    virtual void listviewScrollView(cocos2d::ui::CListView* listview, cocos2d::ui::ScrollView::EventType type) override;
    virtual cocos2d::Size listviewCellSize(cocos2d::ui::CListView* listview, ssize_t idx) override;
    virtual void listviewCell(cocos2d::ui::CListView *listview, cocos2d::ui::Widget* childview, ssize_t idx) override;
    virtual int64_t listviewNumberOfCells(cocos2d::ui::CListView *listview) override;
    
protected:
    
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiInfo();
    void uiList();
    void uiButtons();
    
    //
    void drawInfo();
    void drawList();
    void drawButtons();
    
    // get, set
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickRewardAll(cocos2d::Ref* sender);
    
    // callback
    void callbackInfo(bool bResult, int nResult);
    void callbackReward(bool bResult, int nResult);
    
private:
    bool _bInitUI;
    
    //

    cocos2d::Vector<InfoQuest*> _listInfoMission;
    
    // widget
    cocos2d::ui::Layout* _layerContainerInfo;
    cocos2d::ui::Layout* _layerContainerList;
    cocos2d::ui::Layout* _layerContainerButtons;

    cocos2d::ui::CListView* _uiList;
    cocos2d::ui::Button* _uiBtnReward;
};

#endif /* PopupEventRaceMission_h */
