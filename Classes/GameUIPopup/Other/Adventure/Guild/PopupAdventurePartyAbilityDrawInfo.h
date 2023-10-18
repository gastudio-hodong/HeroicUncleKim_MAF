//
//  PopupAdventurePartyAbilityDrawInfo.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/29.
//

#ifndef PopupAdventurePartyAbilityDrawInfo_hpp
#define PopupAdventurePartyAbilityDrawInfo_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

class InfoAdventurePartyAbilityOrigin;
class PopupAdventurePartyAbilityDrawInfo : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate, public IRefresh
{
public:
    static PopupAdventurePartyAbilityDrawInfo* create();
    
    PopupAdventurePartyAbilityDrawInfo(void);
    virtual ~PopupAdventurePartyAbilityDrawInfo(void);
    virtual void onEnter() override;
    virtual bool init() override;
    virtual void show() override;
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData)  override;
    
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
public:
    
    void initVar();
    void initUI();
    
    void uiTop();
    void uiTable();
    void uiBottom();
    
    void onClickClose(Ref* const sender);
    void onClickSwitching(Ref* const sender);
    
private:
    bool _isValueMax;
    cocos2d::Layer* _lyTopUI;
    cocos2d::Layer* _lyTable;
    cocos2d::Layer* _lyBottomUI;
    cocos2d::extension::TableView *_table;
    
    cocos2d::Vector<InfoAdventurePartyAbilityOrigin*> _listAbilityOrigin;
};

#endif /* PopupAdventurePartyAbilityDrawInfo_hpp */
