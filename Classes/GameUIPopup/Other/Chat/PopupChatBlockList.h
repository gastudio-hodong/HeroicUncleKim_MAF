//
//  PopupChatBlockList.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef PopupChatBlockList_h
#define PopupChatBlockList_h

#include "GameUIPopup/Base/PopupBase.h"

class InfoChat;
class PopupChatBlockList : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static PopupChatBlockList* create();
    
    PopupChatBlockList(void);
    virtual ~PopupChatBlockList(void);
    virtual bool init() override;

    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
public:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContents();
    
    // ui draw
    void drawTable();
    
    //callback
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickUnBlock(cocos2d::Ref* sender);
    
private:
    std::vector<std::string> _listBlock;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    
    cocos2d::extension::TableView *_table;
    cocos2d::Label* _labelListNothing;
 
};
#endif /* PopupChatBlockList_h */
