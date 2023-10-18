//
//  PopupAdventureParty.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/18.
//

#ifndef PopupAdventureParty_hpp
#define PopupAdventureParty_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

#include "GameObject/Raid/InfoWeaponSpirit.h"
#include "GameObject/Adventure/InfoAdventureParty.h"
#include "GameObject/Adventure/InfoAdventurePartyAbilityPreset.h"
#include "GameObject/Adventure/InfoAdventureMember.h"

class PopupAdventureParty : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate, public IRefresh
{
public:
    static PopupAdventureParty* create(int nParty = 1);
    
    PopupAdventureParty(void);
    virtual ~PopupAdventureParty(void);
    virtual void onEnter() override;
    virtual bool init(int nParty);
    virtual void update(float dt) override;
    virtual void show() override;
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData)  override;
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
    void showButtonForceTutorial();
    void showTutorial();
    void setCallbackRefresh(const std::function<void(void)>& callback);
public:
    void initVar();
    void initUI();
    
    void uiTop();
    void uiMiddle();
    void uiTable();
    void uiBottom();
    
    void onClickClose(Ref* const sender);
    void onClickHelp(Ref* const sender);
    void onClickTab(Ref* const sender);
    void onClickPreset(Ref* const sender);
    void onClickUpgrade(Ref* const sender);
    void onClickDraw(Ref* const sender);
    void onClickSeal(Ref* const sender);
    void onClickFilter(Ref* const sender);
    void onClickMember(Ref* const sender);
    void onClickFurnitureUpgrade(Ref* const sender);
    
    bool isUpgradeCondition() const;
    void callbackUpgrade();
    void callbackDraw(bool bResult, int nResult);
    void callbackSaveHide(bool bResult);
    void callbackFurnitureUpgrade();
    
private:
    bool _bUpgrade;
    bool _isFilterArcana;
    bool _isDrawStop;
    
    int _nDrawCount;
    
    int _nCurrentParty;
    int _nCurrentPresetIdx;
    
    float _fDrawTimer;
    float _fDrawTimerMax;
    
    InfoAdventureParty* _infoParty;
    InfoAdventureMember* _infoCurrentMember;
    InfoAdventurePartyAbilityPreset* _infoPreset;
    
    cocos2d::Sprite* _sprContainerMember;
    cocos2d::Sprite* _sprContainerAbility;
    
    Layer* _lyTopUI;
    Layer* _lyMiddleUI;
    Layer* _lyAbility;
    Layer* _lyBottomUI;
    cocos2d::extension::TableView *_table;
    
    cocos2d::ui::Button* _btnFilter;
    cocos2d::Label* _lbTextFilter;
    MafNode::MafMenuItemSprite* _btnDraw;
    
    cocos2d::ui::Button* _btnTutorial;
    std::function<void(void)> callbackRefresh;
};

#endif /* PopupAdventureParty_hpp */
