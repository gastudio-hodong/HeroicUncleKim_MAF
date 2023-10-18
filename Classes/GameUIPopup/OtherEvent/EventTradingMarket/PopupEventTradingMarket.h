//
//  PopupEventTradingMarket.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/01/11.
//

#ifndef PopupEventTradingMarket_h
#define PopupEventTradingMarket_h

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

#include "GameObject/Event/InfoEventTradingItem.h"

class PopupEventTradingMarket : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
    enum class E_BUY_TYPE
    {
        NONE = 0,
        
        POSSIBLE,
        IMPOSSIBLE,
        INFINITY_BUY
    };
public:
    static PopupEventTradingMarket* create();
    
    PopupEventTradingMarket(void);
    virtual ~PopupEventTradingMarket(void);
    virtual void onEnter() override;
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
    
    void uiRefresh();
    void uiTop();
    void uiCurrency();
    void uiMiddle();
    void uiBottom();
    
    void onClickBuyItem(Ref* sender);
    void onClickHelp(Ref* sender);
    void onClickClose(Ref* sender);
    
    
    void callbackInfo(bool bResult, int nResult);
    void callbackBuyItem(bool bResult, int nResult);
    
private:
    Layer* _lyTopUI;
    Layer* _lyCurrencyUI;
    Layer* _lyMiddleUI;
    Layer* _lyBottomUI;
    cocos2d::extension::TableView *_table;
    cocos2d::Vector<InfoEventTradingItem*> _listInfoItems;
};
#endif /* PopupEventTradingMarket_hpp */
