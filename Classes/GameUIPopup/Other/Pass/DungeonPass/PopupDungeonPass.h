//
//  PopupDungeonPass.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/09/15.
//

#ifndef PopupDungeonPass_h
#define PopupDungeonPass_h

#include "GameUIPopup/Base/PopupBase.h"

class PassInfo;
class PassStep;
class PopupDungeonPass : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static PopupDungeonPass* create();
    
    PopupDungeonPass(void);
    virtual ~PopupDungeonPass(void);
    virtual bool init() override;
    virtual void onEnter() override;
     
    // table TableViewDelegate
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) override;
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;

protected:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContents();
    void uiList();
    void uiBadge();
    
    // draw
    void drawPurchase();
    void drawBuffInfo();
    void drawProgress();
    void drawList();
    void drawBadge();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickProgress(cocos2d::Ref* sender);
    void onClickReward(cocos2d::Ref* sender);
    void onClickPurchase(cocos2d::Ref* sender);
    void onClickStepUp(cocos2d::Ref* sender);
    
    // callback
    void callbackPassInfo(bool bResult);
    void callbackPassReward(bool bResult, bool bBadgeFree, bool bBadgePur);
    void callbackPurchase();
    void callbackBadge();
    void callbackStepUp(bool bResult, int nResult);
    
private:
    PassInfo* _objPass;
    cocos2d::Vector<PassStep*> _listPassStep;
    
    bool _bListPosInit;
     
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerList;
    cocos2d::Layer* _layerContainerBadge;
    
    // widget other
    cocos2d::ProgressTimer *_progress;
    cocos2d::Label* _labelProgressTitle;
    cocos2d::Label* _labelProgressLv;
    cocos2d::Label* _labelProgressRate;
    
    MafNode::MafMenuItemSprite* _itemPurchase;
    cocos2d::Label* _labelPurchase;
    cocos2d::Label* _labelTime;
    
    cocos2d::Layer* _layerBuff;
    
    cocos2d::ui::Scale9Sprite* _spriteBadgeFreeIconBG;
    cocos2d::ui::Scale9Sprite* _spriteBadgePurIconBG;
    
    // table
    cocos2d::extension::TableView* _tableList;
    
};

#endif /* DungeonPass_h */
