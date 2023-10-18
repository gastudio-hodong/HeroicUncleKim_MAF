//
//  UILayerRaidTrading.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/02/03.
//

#ifndef UILayerRaidTrading_hpp
#define UILayerRaidTrading_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/Raid/InfoRaidBongStat.h"
#include "GameObject/Raid/InfoRaidTradingMarketReward.h"

class UILayerRaidTrading : public cocos2d::LayerColor, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static UILayerRaidTrading* create(cocos2d::Size size);
    
    UILayerRaidTrading(void);
    virtual ~UILayerRaidTrading(void);
    virtual bool init(cocos2d::Size size);
    virtual void onEnter() override;
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
    virtual void refreshUI() ;
public:
    // set, get
    void setCloseCondition(bool value);
    void setCallbackHide(const std::function<void(void)>& callback);
    
    void requestInfo();
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiTop();
    void uiMiddle();
    void uiTable();
    void uiBottom();
    
    void onClickTicketBuy(cocos2d::Ref* sender);
    void onClickBuyItem(cocos2d::Ref* sender);
    void onClickRefresh(cocos2d::Ref* sender);
    
    void callbackProductOk(int nIdx);
    void callbackInfo(bool bResult, int nResult);
    void callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
    void callbackBuyItem(bool bResult, int nResult);
private:
    bool _isCloseCondition;
    cocos2d::Layer* _lyTopUI;
    cocos2d::Layer* _lyMiddleUI;
    cocos2d::Layer* _lyTableUI;
    cocos2d::Layer* _lyBottomUI;
    
    cocos2d::extension::TableView* _table;
    
    std::function<void(void)> _callbackHide;
    Vector<InfoRaidTradingMarketReward*> _listInfoItems;
};
#endif /* UILayerRaidTrading_hpp */
