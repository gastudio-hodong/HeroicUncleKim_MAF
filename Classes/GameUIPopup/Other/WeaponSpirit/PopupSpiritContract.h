//
//  PopupSpiritContract.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/12/09.
//

#ifndef PopupSpiritContract_hpp
#define PopupSpiritContract_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/Raid/InfoWeaponSpirit.h"

class PopupSpiritContract : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static PopupSpiritContract* create();
    
    PopupSpiritContract(void);
    virtual ~PopupSpiritContract(void);
    virtual bool init() override;
    
    
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
    void uiMain();
    
    void onClickContract(Ref* sender);
    void onClickClose(Ref* sender);
    
    
    void callbackContract(bool bResult, int nResult);
    
private:
    Layer* _lyTopUI;
    Layer* _lyMainUI;
    cocos2d::extension::TableView *_table;
};

#endif /* PopupSpiritContract_hpp */
