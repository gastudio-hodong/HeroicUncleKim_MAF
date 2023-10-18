//
//  PopupDefenseTrackSetting.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/31.
//

#ifndef PopupDefenseTrackSetting_hpp
#define PopupDefenseTrackSetting_hpp

#include "GameUIPopup/Base/PopupBase.h"

class InfoDefenseBullet;

class PopupDefenseTrenchSetting : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static PopupDefenseTrenchSetting* create();
    
    PopupDefenseTrenchSetting(void);
    virtual ~PopupDefenseTrenchSetting(void);
    bool init() override;
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;

public:
    
    void initVar();
    void initUI();
    
    // ui
    void uiTop();
    void uiTable();
    void uiBottom();
    
    void uiRefresh();
protected:
    void onClickClose(Ref* sender);
    void onClickWeaponChange(Ref* sender);
    void onClickCharacterChange(Ref* sender);
    void onClickAutoSetting(Ref* sender);
    void playMuzzleEffect(InfoDefenseBullet* bulletData, Node* parent, Vec2 location);
    
    void onServerEquip(bool bResult, int nResult);
private:
    Layer* _lyTop;
    Layer* _lyMiddle;
    Layer* _lyBottom;
    cocos2d::extension::TableView* _table;
    
};


#endif /* PopupDefenseTrackSetting_hpp */
