//
//  PopupExpeditionDraw.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#ifndef PopupExpeditionDraw_h
#define PopupExpeditionDraw_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupExpeditionDraw : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static PopupExpeditionDraw* create();
    
    PopupExpeditionDraw(void);
    virtual ~PopupExpeditionDraw(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
protected:
    void initVar();
    void initUi();
    
    // ui
    void uiDate();
    void uiCurrency();
    void uiList();
    void uiBottom();
    void uiBonus();
    
    // draw
    void drawTable();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickDraw(cocos2d::Ref* sender);
    void onClickDrawBonus(cocos2d::Ref* sender);
    
    // network
    void requestInfo();
    
    //callback
    void callbackInfo(bool bResult, int nResult);
    void callbackDraw(bool bResult, int nResult);
    void callbackDrawBonus(bool bResult, int nResult);
    
    
private:
    //
    std::string _strType;
    int _nSelectIdx;
    
    // widget
    cocos2d::Layer* _layerContainer;
    cocos2d::Layer* _layerContainerDate;
    cocos2d::Layer* _layerContainerCurrency;
    cocos2d::Layer* _layerContainerList;
    cocos2d::Layer* _layerContainerBottom;
    cocos2d::Layer* _layerContainerBonus;
    cocos2d::Sprite* _spriteBonus;
    
    // table
    cocos2d::extension::TableView* _tableList;
};

#endif /* PopupExpeditionDraw_h */
