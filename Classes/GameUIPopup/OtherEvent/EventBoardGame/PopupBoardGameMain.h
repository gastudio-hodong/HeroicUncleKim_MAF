//
//  PopupBoardGameMain.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/15.
//

#ifndef PopupBoardGameMain_hpp
#define PopupBoardGameMain_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

class InfoQuest;
class PopupBoardGameMain : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate, public IRefresh
{

public:
    static PopupBoardGameMain* create();
        
    PopupBoardGameMain(void);
    virtual ~PopupBoardGameMain(void);
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    // table TableViewDelegate
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) override;
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;
    
private:
        // init
    void initVar();
    void initUI();
    
    // ui
    void uiTop();
    void uiInfo();
    void uiQuest();
    void uiBoard();
    
    
    // draw
    void drawQuest();
    void drawInfoAds();

    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickBanner(cocos2d::Ref* sender);
    void onClickSkin(cocos2d::Ref* sender);
    void onClickRaidEnter(cocos2d::Ref* sender);
    void onClickAds(cocos2d::Ref* sender);
    
    // callback
    void callbackInfo(bool bResult, int nResult);
    void callbackRedraw();
    void callbackAds(ADS_RESULT result);
    void callbackAdsReward(bool bResult, int nResult);
    
private:
    //
    cocos2d::Vector<InfoQuest*> _listQuest;
    
    //
    cocos2d::Layer* _layerContainerTop;
    cocos2d::Layer* _layerContainerInfo;
    cocos2d::Layer* _layerContainerQuest;
    cocos2d::Layer* _layerContainerBoard;
    
    cocos2d::Sprite* _spriteFishing;
    cocos2d::ui::CText* _uiTextAdsCount;
    cocos2d::ui::Button* _uiBtnAds;
    
    //
    cocos2d::extension::TableView *_table;
    
};

#endif /* PopupBoardGameMain_hpp */
