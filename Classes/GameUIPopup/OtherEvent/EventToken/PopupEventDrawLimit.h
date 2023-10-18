//
//  PopupEventDrawLimit.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/10/13.
//

#ifndef PopupEventDrawLimit_h
#define PopupEventDrawLimit_h

#include "GameUIPopup/Base/PopupBase.h"

#include "ManagerEvent/EventDrawLimitManager.h"

class PopupEventDrawLimit : public PopupBase, public cocos2d::ui::DelegateListView
{
public:
    CREATE_FUNC(PopupEventDrawLimit);
    
    PopupEventDrawLimit(void);
    virtual ~PopupEventDrawLimit(void);
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void backkey() override;
    
    // listview
    virtual void listviewScrollView(cocos2d::ui::CListView* listview, cocos2d::ui::ScrollView::EventType type) override;
    virtual cocos2d::Size listviewCellSize(cocos2d::ui::CListView* listview, ssize_t idx) override;
    virtual void listviewCell(cocos2d::ui::CListView *listview, cocos2d::ui::Widget* childview, ssize_t idx) override;
    virtual int64_t listviewNumberOfCells(cocos2d::ui::CListView *listview) override;
    
public:
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiDefault();
    void uiBestItems();
    void uiInfo();
    void uiButtons();
    
    // draw
    void drawDefault();
    void drawBestItems();
    void drawInfo();
    void drawButtons();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickHelp(cocos2d::Ref* sender);
    void onClickDraw(cocos2d::Ref* sender);
    void onClickBonus(cocos2d::Ref* sender);
    void onClickTokenPieceExchange(cocos2d::Ref* sender);
    void onClickTokenExchange(cocos2d::Ref* sender);
    
    
    // network
    
    // callback
    void callbackNetworkInfo(bool bResult, int nResult);
    void callbackNetworkDraw(bool bResult, int nResult);
    void callbackNetworkDrawBonus(bool bResult, int nResult);
    void callbackNetworkTokenPieceExchange(bool bResult, int nResult);
    void callbackNetworkTokenExchange(bool bResult, int nResult);
    void callbackDrawAniFinish();
    void callbackDrawAniAgain(int count);
    
private:
    bool _bInitUI;
    
    //
    cocos2d::Vector<InfoEventDraw*> _listInfoDraw;
    
    // widget
    cocos2d::ui::Layout* _uiContentsDefault;
    cocos2d::ui::Layout* _uiContentsBestItems;
    cocos2d::ui::Layout* _uiContentsInfo;
    cocos2d::ui::Layout* _uiContentsButtons;
    
    cocos2d::ui::CListView* _uiList;
};

#endif /* PopupEventDrawLimit_hpp */
