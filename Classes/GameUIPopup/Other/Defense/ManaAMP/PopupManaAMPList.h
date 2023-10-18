//
//  PopupManaAMPList.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/09/11.
//

#ifndef PopupManaAMPList_hpp
#define PopupManaAMPList_hpp

#include "GameUIPopup/Base/PopupBase.h"

class PopupManaAMPList : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static PopupManaAMPList* create();
    
    PopupManaAMPList(void);
    virtual ~PopupManaAMPList(void);
    bool init() override;
    void onEnter() override;
    
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
    void uiTopInfo();
    void uiTable();
    void uiBottom();
    
    void uiRefresh();
protected:
    void onClickClose(Ref* sender);
    void onClickAMP(Ref* sender);
    
    void onServerResponse(bool bResult, int nResult);
private:
    Layer* _lyTop;
    Layer* _lyTopInfo;
    Layer* _lyMiddle;
    Layer* _lyBottom;
    cocos2d::extension::TableView* _table;
    
};


#endif /* PopupManaAMPList_hpp */
