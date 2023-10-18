//
//  PopupSpiritSummonDrawInfo.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/12/07.
//

#ifndef PopupSpiritSummonDrawInfo_hpp
#define PopupSpiritSummonDrawInfo_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/InfoDrawRate.h"

class PopupSpiritSummonDrawInfo : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static PopupSpiritSummonDrawInfo* create();
    
    PopupSpiritSummonDrawInfo(void);
    virtual ~PopupSpiritSummonDrawInfo(void);
    virtual bool init() override;
    
    
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
    
    void uiTop();
    
    void uiMiddle();
    
    void uiBottom();
    
    
    void onClickClose(Ref* sender);
    void onClickLevel(Ref* sender);
    
    InfoDrawRate* getInfoRate(int value);
private:
    int _nLevel;
    cocos2d::Layer* _lyTopUI;
    cocos2d::Layer* _lyMiddleUI;
    cocos2d::Layer* _lyBottomUI;
    cocos2d::extension::TableView *_table;
    InfoDrawRate* _infoRateNow;
    cocos2d::Vector<InfoDrawRate*> _listRate;
};

#endif /* PopupSpiritSummonDrawInfo_hpp */
