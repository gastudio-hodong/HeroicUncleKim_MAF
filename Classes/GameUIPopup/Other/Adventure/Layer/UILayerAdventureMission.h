//
//  UILayerAdventureMission.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/08.
//

#ifndef UILayerAdventureMission_hpp
#define UILayerAdventureMission_hpp

#include "UILayerAdventureParent.h"

class InfoQuest;
class UILayerAdventureMission : public UILayerAdventureParent, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    
    static UILayerAdventureMission* create(const cocos2d::Size size, const E_ADVENTURE_MISSION_GROUP type);
    
    UILayerAdventureMission(void);
    virtual ~UILayerAdventureMission(void);
    virtual bool init(const cocos2d::Size size, const E_ADVENTURE_MISSION_GROUP type);
    
    virtual void refreshUI() override;
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;

protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiTop();
    void uiTable();
    
    void onClickMissionComplete(Ref* const sender);
    
    void callbackMissionComplete(bool bResult, int nResult);
private:
    E_ADVENTURE_MISSION_GROUP _eType;
    
    cocos2d::Layer* _lyTop;
    cocos2d::Layer* _lyTable;
    cocos2d::extension::TableView *_table;
    
    cocos2d::Vector<InfoQuest*> _listMission;
};


#endif /* UILayerAdventureMission_hpp */
