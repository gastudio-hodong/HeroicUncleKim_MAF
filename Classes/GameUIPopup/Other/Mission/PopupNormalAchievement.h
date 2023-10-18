//
//  PopupNormalAchievement.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/29.
//

#ifndef PopupNormalAchievement_h
#define PopupNormalAchievement_h


#include "GameUIPopup/Base/PopupBase.h"

class ObjAchievement;
class PopupNormalAchievement : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static PopupNormalAchievement* create();

    PopupNormalAchievement(void);
    virtual ~PopupNormalAchievement(void);

    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;

    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;

    virtual bool init() override;
    
    void redrawPopupUI();
    
    //callback
    void callbackReward();
    
protected:
    // init
    void initVar();
    void initUI();
    
    void initTopUI();
    void initBottomUI();
    
    // ui
    void uiContents();
    
    
    // click
    void onClick(cocos2d::Ref* sender);
    
    void onClickReward(int idx);
    void onClickRewardAll();
    void onClickTab(int idx);
    void onClickBack();
    void goThroughShortcut(int idx);
    MafNode::MafMenuItemSprite* getCheckBox(E_ACHIEVE_NORMAR_TYPE type);
    
private:
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerTopUI;

    cocos2d::extension::TableView *_table;
    
    cocos2d::Vector<ObjAchievement*> _vecAchievement;
    cocos2d::Vector<MafNode::MafMenuItemSprite*> _vecAchieveTab;
    E_ACHIEVE_NORMAR_TYPE _eType;
    MafNode::MafMenuItemSprite* _itemRewardAll;
};

#endif /* PopupNormalAchievement_hpp */
