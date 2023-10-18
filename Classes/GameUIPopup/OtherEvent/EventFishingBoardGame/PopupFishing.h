//
//  PopupFishing.hpp
//  FantasyClicker-mobile
//
//  Created by TG on 2020/07/08.
//

#ifndef PopupFishing_h
#define PopupFishing_h

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

class InfoQuest;
class PopupFishing : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate, public IRefresh
{
public:
    static PopupFishing* create();
        
    PopupFishing(void);
    virtual ~PopupFishing(void);
    
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
    void uiFishing();
    void uiBait();
    void uiQuest();
    void uiBoard();
    
   
    void initCurrency();
    
    // draw
    void drawFishing();
    void drawBait();
    void drawQuest();

    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickBanner(cocos2d::Ref* sender);
    void onClickFishing(cocos2d::Ref* sender);
    void onClickBuyBait(cocos2d::Ref* sender);
    
    
    // callback
    void callbackInfo(bool bResult, int nResult);
    void callbackFishing(bool bResult, int nResult, std::string strReward);
    void callbackRedraw();
    
private:
    //
    cocos2d::Vector<InfoQuest*> _listQuest;
    
    
    //
    cocos2d::Layer* _layerContainerFishing;
    cocos2d::Layer* _layerContainerBait;
    cocos2d::Layer* _layerContainerQuest;
    cocos2d::Layer* _layerContainerBoard;
    
    cocos2d::Sprite* _spriteFishing;
    cocos2d::Label* _labelBaitCount;          //
    cocos2d::Label* _labelBaitBuy;          //
    cocos2d::Label* _labelPointcount;
    
    //
    cocos2d::extension::TableView *_table;
    
};


#endif /* PopupFishing_h */
