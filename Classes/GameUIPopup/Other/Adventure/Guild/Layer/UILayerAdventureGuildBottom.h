//
//  UILayerAdventureGuildBottom.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/15.
//

#ifndef UILayerAdventureGuildBottom_hpp
#define UILayerAdventureGuildBottom_hpp

#include "GameUIPopup/Other/Adventure/Layer/UILayerAdventureParent.h"

#include "Common/Observer/IRefresh.h"

#include "GameObject/Raid/InfoRaidBongStat.h"
#include "GameObject/Adventure/InfoAdventureFriend.h"

class UILayerAdventureGuildBottom : public UILayerAdventureParent, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate, public IRefresh
{
public:
    static UILayerAdventureGuildBottom* create(const cocos2d::Size size);
    
    UILayerAdventureGuildBottom(void);
    virtual ~UILayerAdventureGuildBottom(void);
    virtual bool init(const cocos2d::Size size);
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData)  override;
    
    virtual void refreshUI() override;
    void showButtonForceTutorial();
    void showTutorial();
public:
    void setCallbackHide(const std::function<void(void)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    void uiTable();
    void uiBottom();
    
    void uiFriendRunning(InfoAdventureFriend* const infoFriend);
    void uiFriendDefault();
    
    // click
    void onClickLunch(Ref* const pSender);
    void onClickReward(Ref* const pSender);
    void onClickFriend(Ref* const pSender);
    void onClickFriendReward(Ref* const pSender);
    void onClickTimeReduction(Ref* const pSender);
    void onClickPartyMiniSpec(Ref* const pSender);
    
    //callback
    void callbackReward(const bool bResult, const int nResult);
    void callbackTimeReduction(const bool bResult, const int nResult);
private:
    cocos2d::Layer* _lyTable;
    cocos2d::Layer* _lyBottom;
    
    cocos2d::ui::Button* _btnTutorial;
    cocos2d::extension::TableView *_table;
    std::function<void(void)> _callbackHide;
};



#endif /* UILayerAdventureGuildBottom_hpp */
