//
//  LayerStoryList.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/09.
//

#ifndef LayerStoryList_hpp
#define LayerStoryList_hpp

#include "GameUIPopup/Base/PopupBase.h"

class LayerStoryList : public cocos2d::LayerColor, cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static LayerStoryList* create(const cocos2d::Size size);
    
    LayerStoryList(void);
    virtual ~LayerStoryList(void);
    virtual bool init(const cocos2d::Size size);
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);
    
public:
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiMenu();
    void uiContents();
    
    void drawMenu();
    void drawContents(bool bRefresh = false);
    
    // click
    void onClickTab(cocos2d::Ref* sender);
    void onClickStory(cocos2d::Ref* sender);
    void onClickStoryDownload(cocos2d::Ref* sender);
    
    // callback
    void callbackDownload(int nIdx);
    
private:
    int _nIdxMenu;
    std::vector<int> listStoryNormal;
    std::vector<int> listStoryDevil;
    std::vector<int> listStoryEvent;
    std::vector<int> listStoryTable;
    
    // widget
    cocos2d::Layer* _layerContainerMenu;
    cocos2d::Layer* _layerContainerContents;
    cocos2d::extension::TableView *_table;
};

#endif /* LayerStoryList_hpp */
