//
//  LayerStatFirstPage.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 07/01/2020.
//

#ifndef LayerStatFirstPage_h
#define LayerStatFirstPage_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/Observer/IRefresh.h"

class StatFirstPage : public cocos2d::LayerColor, public IRefresh, cocos2d::extension::MafGridViewDelegate, cocos2d::extension::MafGridViewDataSource
{
public:
    static StatFirstPage* create(E_PLACE ePlace);
    
    StatFirstPage(void);
    virtual ~StatFirstPage(void);
    virtual bool init(E_PLACE ePlace);
    virtual void onEnter() override;
    
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;
    
    
public:
    void redrawLayer();
    
protected:
    void initUI();
    void initVar();
    void initHistory();
    void initMine();
    void initBadge();
    
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::MafGridView *table, ssize_t idx) override;
    virtual cocos2d::Size cellSizeForTable(cocos2d::extension::MafGridView *table) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::MafGridView *table) override;
    
    virtual void tableCellTouched(cocos2d::extension::MafGridView* table, cocos2d::extension::TableViewCell* cell) override {};
    virtual void tableCellHighlight(cocos2d::extension::MafGridView* table, cocos2d::extension::TableViewCell* cell) override {};
    virtual void tableCellUnhighlight(cocos2d::extension::MafGridView* table, cocos2d::extension::TableViewCell* cell) override {};
    
    // click
    void onClickBadge(cocos2d::Ref* sender);
    
private:
    cocos2d::Vector<cocos2d::Label*> _lbCurrentValue;
    E_PLACE _ePlace;
    
    //
    Layer* _layerContainerStat;
    Layer* _layerContainerBadge;
};
#endif /* LayerStatNormal_hpp */
