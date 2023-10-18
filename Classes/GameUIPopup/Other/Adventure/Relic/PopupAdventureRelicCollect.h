//
//  PopupAdventureRelicCollect.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/12.
//

#ifndef PopupAdventureRelicCollect_hpp
#define PopupAdventureRelicCollect_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

#include "GameObject/Raid/InfoWeaponSpirit.h"
#include "GameObject/Adventure/InfoAdventureMap.h"
#include "GameObject/Adventure/InfoAdventureParty.h"
#include "GameObject/Adventure/InfoAdventureRelicCollect.h"

class PopupAdventureRelicCollect : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate, public IRefresh
{
public:
    static PopupAdventureRelicCollect* create();
    
    PopupAdventureRelicCollect(void);
    virtual ~PopupAdventureRelicCollect(void);
    virtual void onEnter() override;
    virtual bool init();
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
    static bool sortCondition(InfoAdventureRelicCollect* const a, InfoAdventureRelicCollect* const b);
    
    void initVar();
    void initUI();
    
    void uiTop();
    void uiTable();
    
    void onClickClose(Ref* const sender);
    void onClickCollect(Ref* const sender);
    
    
    void callbackInit(const bool bResult, const int nResult);
    void callbackCollect(const bool bResult, const int nResult);
    
private:
    cocos2d::Layer* _lyTopUI;
    cocos2d::Layer* _lyTable;
    cocos2d::extension::TableView *_table;
    cocos2d::Vector<InfoAdventureRelicCollect*> _listInfoCollect;
    
    std::function<void(void)> _callbackRefresh;
    std::function<void(void)> _callbackHide;
};


#endif /* PopupAdventureRelicCollect_hpp */
