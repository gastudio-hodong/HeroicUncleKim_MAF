//
//  PopupTimeTravel.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#ifndef PopupTimeTravel_h
#define PopupTimeTravel_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupTimeTravel : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static PopupTimeTravel* create();
    
    PopupTimeTravel(void);
    virtual ~PopupTimeTravel(void);
    virtual bool init() override;
    
    // table TableViewDelegate
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) override {};
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
public:
    void setCallbackTimeTravel(const std::function<void(void)>& callback);
    
protected:
    
    void initVar();
    void initUi();
    
    // ui
    void uiInfo();
    void uiContent();
    void uiButton();
    
    //
    void drawInfo();
    void drawButton();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickItemBuy(cocos2d::Ref* sender);
    void onClickUse(cocos2d::Ref* sender);
    
    
    // callback
    void callbackTimeTravelCalculation();

public:
    // game
    void onTimeTravel();
    
private:
    //
    int _nFloorPlus;
    std::function<void(void)> _onCallbackTimeTravel;

    // widget
    cocos2d::Layer* _layerContainerInfo;
    cocos2d::Layer* _layerContainerButton;
    cocos2d::Layer* _layerContainerContents;
    
    cocos2d::extension::TableView *_table;
    
    cocos2d::Label* _labelTime;
    cocos2d::Label* _labelTimeTravelCount;
    cocos2d::Label* _labelTimeTravelTime;
    
    cocos2d::Label* _labelFloorPlus;
};

#endif /* PopupTimeTravel_h */
