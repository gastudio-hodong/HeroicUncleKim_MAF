//
//  PopupAdventureRelicCraftRateInfo.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/17.
//

#ifndef PopupAdventureRelicCraftRateInfo_hpp
#define PopupAdventureRelicCraftRateInfo_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

#include "GameObject/InfoDrawRate.h"

class PopupAdventureRelicCraftRateInfo : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate, public IRefresh
{
public:
    enum class E_TYPE
    {
        AREA_CORE = 1,
        CASH_CORE = 5
    };
    static PopupAdventureRelicCraftRateInfo* create();
    
    PopupAdventureRelicCraftRateInfo(void);
    virtual ~PopupAdventureRelicCraftRateInfo(void);
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
    void loadData();
    void initVar();
    void initUI();
    
    void uiTab();
    void uiAreaCore();
    void uiCashCore();
    void uiTable();
    
    void onClickClose(Ref* sender);
    void onClickLevel(Ref* sender);
    void onClickTab(Ref* sender);
    
    InfoDrawRate* const getInfoRate(const int nType, const int nLevel);
    const int getAreaCoreMaxLv() const;
private:
    int _nLevel;
    E_TYPE _eType;
    cocos2d::Layer* _lyTabUI;
    cocos2d::Layer* _lyTopUI;
    cocos2d::Layer* _lyTableUI;
    cocos2d::extension::TableView *_table;
    InfoDrawRate* _infoRateNow;
    cocos2d::Vector<InfoDrawRate*> _listRate;
};


#endif /* PopupAdventureRelicCraftRateInfo_hpp */
