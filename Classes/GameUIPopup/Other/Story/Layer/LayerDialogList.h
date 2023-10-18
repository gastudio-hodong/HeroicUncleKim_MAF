//
//  LayerDialogList.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/09.
//

#ifndef LayerDialogList_hpp
#define LayerDialogList_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/Dialog/InfoDialog.h"

class LayerDialogList : public cocos2d::LayerColor, cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
    enum class E_TYPE
    {
        NONE = 0,
        
        STORY_MAIN,
        STORY_SUB,
        EVENT
    };
public:
    static LayerDialogList* create(const cocos2d::Size size);
    
    LayerDialogList(void);
    virtual ~LayerDialogList(void);
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
    
    void drawContents(bool bRefresh = false);
    
    // click
    void onClickTab(cocos2d::Ref* sender);
    void onClickDialog(cocos2d::Ref* sender);
    
    
private:
    E_TYPE _eType;
    cocos2d::Vector<InfoDialog*> _listDialog;
    
    // widget
    cocos2d::Layer* _layerContainerMenu;
    cocos2d::Layer* _layerContainerContents;
    cocos2d::extension::TableView *_table;
};

#endif /* LayerDialogList_hpp */
