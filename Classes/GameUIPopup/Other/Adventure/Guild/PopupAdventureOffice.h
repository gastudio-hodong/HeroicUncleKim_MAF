//
//  PopupOfficeUpgradeMain.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/19.
//

#ifndef PopupOfficeUpgradeMain_hpp
#define PopupOfficeUpgradeMain_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

#include "GameObject/Raid/InfoWeaponSpirit.h"
#include "GameObject/Adventure/InfoAdventureFurniture.h"

class PopupAdventureOffice : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate, public IRefresh
{
public:
    static PopupAdventureOffice* create();
    
    PopupAdventureOffice(void);
    virtual ~PopupAdventureOffice(void);
    virtual void onEnter() override;
    virtual bool init() override;
    virtual void show() override;
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData)  override;
    
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
public:
    
    void showButtonForceTutorial();
    void showTutorial();
    
    void initVar();
    void initUI();
    
    void uiTop();
    void uiTable();
    
    void onClickClose(Ref* const sender);
    void onClickUpgrade(Ref* const sender);
    
    //
    void callbackUpgrade();
    
    bool isUpgradeCondition(int nIdx);
private:
    cocos2d::Layer* _lyTopUI;
    cocos2d::Layer* _lyFurniture;
    cocos2d::ui::Button* _btnTutorial;
    cocos2d::extension::TableView *_table;
    cocos2d::Vector<InfoAdventureFurniture*> _listFurniture;
};

#endif /* PopupOfficeUpgradeMain_hpp */
