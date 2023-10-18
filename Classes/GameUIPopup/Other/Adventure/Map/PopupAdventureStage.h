//
//  PopupAdventureStage.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/26.
//

#ifndef PopupAdventureStage_hpp
#define PopupAdventureStage_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

#include "GameObject/Raid/InfoWeaponSpirit.h"
#include "GameObject/Adventure/InfoAdventureMap.h"
#include "GameObject/Adventure/InfoAdventureParty.h"

class PopupAdventureStage : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate, public IRefresh
{
public:
    static PopupAdventureStage* create(const int nCurrentAdventureID, const int nCurrentArea, bool isFriend = false);
    
    PopupAdventureStage(void);
    virtual ~PopupAdventureStage(void);
    virtual void onEnter() override;
    virtual bool init(const int nCurrentAdventureID, const int nCurrentArea, bool isFriend);
    virtual void show() override;
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData)  override;
    
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
    void setCallbackHide(const std::function<void(void)>& callback);
    void setCallbackRefresh(const std::function<void(void)>& callback);
private:
    
    void initVar();
    void initUI();
    
    void uiTop();
    void uiTable();
    
    void onClickClose(Ref* const sender);
    void onClickTab(Ref* const sender);
    void onClickRunning(Ref* const sender);
    void onClickFurnitureUpgrade(Ref* const sender);
    void onClickReward(Ref* const sender);
    
    void callbackRunning(const bool bResult, const int nResult);
    void callbackRefresh();
    void callbackReward(const bool bResult, const int nResult);
    
private:
    bool _isFriend;
    int _nCurrentAdventureID;
    int _nCurrentArea;
    
    InfoAdventureMap* _infoMap;
    
    std::function<void(void)> _callbackHide;
    std::function<void(void)> _callbackRefresh;
    
    //
    Layer* _lyTopUI;
    Layer* _lyStage;
    cocos2d::extension::TableView *_table;
    
    
};

#endif /* PopupAdventureStage_hpp */
