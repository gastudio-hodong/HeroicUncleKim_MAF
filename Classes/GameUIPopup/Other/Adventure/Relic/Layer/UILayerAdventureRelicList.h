//
//  UILayerAdventureRelicList.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/04.
//

#ifndef UILayerAdventureRelicList_hpp
#define UILayerAdventureRelicList_hpp

#include "GameUIPopup/Other/Adventure/Layer/UILayerAdventureParent.h"

#include "Common/Observer/IRefresh.h"

#include "GameObject/Adventure/InfoAdventureRelic.h"

#include "GameUIPopup/Other/Adventure/Relic/PopupAdventureRelicDefault.h"

class UILayerAdventureRelicList : public UILayerAdventureParent, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate, public IRefresh
{
public:
    enum class E_TYPE
    {
        FOREST = 1,
        DESERT,
        ICE,
        VOLCANO
    };
    static UILayerAdventureRelicList* create(cocos2d::Size size);
    
    UILayerAdventureRelicList(void);
    virtual ~UILayerAdventureRelicList(void);
    virtual bool init(cocos2d::Size size);
    virtual void onEnter()override;
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
    // refresh
    virtual void refreshUI() override;
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData)  override;
public:
    // set, get
    void setCloseCondition(bool value);
    void setCallbackHide(const std::function<void(void)>& callback);
    void showTutorial();
    
private:
    static bool sortRelic(InfoAdventureRelic* const a, InfoAdventureRelic* const b);
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiTop();
    void uiTable();
    void uiBottom();
    
    void onClickLevelup(Ref* sender);
    void onClickGrind(Ref* sender);
    void onClickCollection(Ref* sender);
    void onClickTab(Ref* sender);
    void onClickRelicTotalSpec(Ref* sender);
    void onClickAllLevelUp(Ref* sender);
    
    void requestAllLevelUp();
    
    void callbackInfo(bool bResult, int nResult);
    void callbackLevelUP(bool bResult, int nResult);
    void callbackAllLevelUp(bool bResult, int nResult);
private:
    bool _isCloseCondition;
    int _nUpgradeRelicIdx;
    int _nCurrentLevelUpIdx;
    UILayerAdventureRelicList::E_TYPE _eType;
    cocos2d::Layer* _lyTopUI;
    cocos2d::Layer* _lyTableUI;
    cocos2d::Layer* _lyBottomUI;
    cocos2d::extension::TableView *_table;
    
    std::function<void(void)> _callbackHide;
    
    cocos2d::Vector<InfoAdventureRelic*> _listRelic;
};

#endif /* UILayerAdventureRelicList_hpp */
