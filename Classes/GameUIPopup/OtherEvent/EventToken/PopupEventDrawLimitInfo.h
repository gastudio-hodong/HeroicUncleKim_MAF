//
//  PopupEventDrawLimitInfo.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/10/13.
//

#ifndef PopupEventDrawLimitInfo_h
#define PopupEventDrawLimitInfo_h

#include "GameUIPopup/Base/PopupBase.h"

class InfoEventDraw;
class PopupEventDrawLimitInfo : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static PopupEventDrawLimitInfo* create();
    
    PopupEventDrawLimitInfo(void);
    virtual ~PopupEventDrawLimitInfo(void);
    virtual bool init() override;
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
public:
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiTop();
    void uiReward();
    
    
    // draw
    void drawReward();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickDraw(cocos2d::Ref* sender);
    
private:
    cocos2d::Vector<InfoEventDraw*> _listInfoDraw;
    
    // widget
    cocos2d::Layer* _layerContainerTop;
    cocos2d::Layer* _layerContainerReward;
    
    
 
    
    // table
    cocos2d::extension::TableView* _tableList;
};

#endif /* PopupEventDrawLimitInfo_hpp */
