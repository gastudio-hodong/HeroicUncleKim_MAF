//
//  PopupDefenseTrenchSelect.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/01.
//

#ifndef PopupDefenseTrenchSelect_hpp
#define PopupDefenseTrenchSelect_hpp

#include "GameUIPopup/Base/PopupBase.h"


class PopupDefenseTrenchSelect : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static PopupDefenseTrenchSelect* create(E_DF_Trench_SettingType type, int trenchNumber);
    
    PopupDefenseTrenchSelect(void);
    virtual ~PopupDefenseTrenchSelect(void);
    bool init(E_DF_Trench_SettingType type, int trenchNumber);
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;

public:
    void initVar();
    void initUI();
    
    // ui
    void uiTop();
    void uiTable();
    void uiBottom();
    void uiRefresh();
    
protected:
    void onClickClose(Ref* sender);
    void onClickTab(Ref* sender);
    void onClickSelect(Ref* sencer);
    void onClickChangeTrench(Ref* sender);
    //ui
    void createCell(Node* cell, const int cellNumber);
    
    //callback
    void onServerEquip(bool bResult, int nResult);
private:
    E_DF_Trench_SettingType _type;
    int _trenchNumber;
    Layer* _lyTop;
    Layer* _lyTable;
    Layer* _lyBottom;
    cocos2d::extension::TableView* _table;
    
};


#endif /* PopupDefenseTrenchSelect_hpp */
