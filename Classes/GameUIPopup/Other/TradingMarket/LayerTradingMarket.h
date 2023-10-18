//
//  LayerTradingMarket.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/06/11.
//

#ifndef LayerTradingMarket_hpp
#define LayerTradingMarket_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoTradingMarket;
class LayerTradingMarket : public cocos2d::ui::Layout, public cocos2d::ui::DelegateListView
{
public:
    static LayerTradingMarket* create(int type);

    LayerTradingMarket(void);
    virtual ~LayerTradingMarket(void);
    virtual bool init(int type);
    virtual void onEnter() override;

    // listview
    virtual void listviewScrollView(cocos2d::ui::CListView* listview, cocos2d::ui::ScrollView::EventType type) override;
    virtual cocos2d::Size listviewCellSize(cocos2d::ui::CListView* listview, ssize_t idx) override;
    virtual void listviewCell(cocos2d::ui::CListView *listview, cocos2d::ui::Widget* childview, ssize_t idx) override;
    virtual int64_t listviewNumberOfCells(cocos2d::ui::CListView *listview) override;

public:
    // set, get, add
    void setCallbackInfo(const std::function<void(bool)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiList();
    
    // draw
    void drawList();
    
    // set, get
    
    // callback
    void callbackInfo(bool bResult);
    void callbackOpen(bool bResult);
    void callbackTrade(bool bResult);
    
    // schedule
    
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickTradeOpen(cocos2d::Ref* sender);
    void onClickTrade(cocos2d::Ref* sender);
    void onClickTradeArrowL(cocos2d::Ref* sender);
    void onClickTradeArrowR(cocos2d::Ref* sender);
    
private:
    bool _bInitUI;
    
    //
    int _type;
    std::map<int, int> _listCountAdd;
    
    cocos2d::Vector<InfoTradingMarket*> _listTradingMarket;
    
    // callback
    std::function<void(bool)> _callbackInfo;
    
    // widget
    cocos2d::ui::Layout* _layerContainerList;
    
    // table
    cocos2d::ui::CListView* _uiList;
    
};

#endif /* LayerTradingMarket_hpp */
