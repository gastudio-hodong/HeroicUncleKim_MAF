
//
// UILayerAdventureRelicCrafting.hpp
// FantasyClicker-mobile
//
// Created by MAF_TG on 2022/05/09.
//

#ifndef UILayerAdventureRelicCrafting_hpp
#define UILayerAdventureRelicCrafting_hpp


#include "GameUIPopup/Other/Adventure/Layer/UILayerAdventureParent.h"

#include "Common/Observer/IRefresh.h"

#include "GameObject/Adventure/InfoAdventureCore.h"
#include "GameObject/InfoItems.h"

#include "GameUIPopup/Other/Adventure/Relic/PopupAdventureRelicDefault.h"

class UILayerAdventureRelicCrafting : public UILayerAdventureParent, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate, public IRefresh
{
public:
    static UILayerAdventureRelicCrafting* create(cocos2d::Size size);
   
    UILayerAdventureRelicCrafting(void);
    virtual ~UILayerAdventureRelicCrafting(void);
    virtual bool init(cocos2d::Size size);
    virtual void onEnter() override;
   
   
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
    void setCallbackMoveShop(const std::function<void(PopupAdventureRelicDefault::E_TYPE)>& callback);
    
    void showButtonForceTutorial();
    void showTutorial();
    void requestInfo();
protected:
  // init
    void initVar();
    void initUi();
    
  // ui
    void uiTopBG();
    void uiTop();
    void uiMiddle();
    void uiBottom();
    
    void onClickHelp(Ref* sender);
    void onClickCore(Ref* sender);
    void onClickCraft(Ref* sender);
    void onClickFriendCraft(Ref* sender);
    
    InfoAdventureCore* getCore(int nIdx);
    
    void setCoreList();
    
    void callbackInit(bool bResult, int nResult);
    void callbackCraft(bool bResult, int nResult, cocos2d::Vector<InfoItems*> list);
    void callbackFriendCraft(bool bResult, int nResult, cocos2d::Vector<InfoItems*> list);
    void callbackTutorial();
    void callbackTransferShop();
    
    static bool sortCore(InfoAdventureCore* a, InfoAdventureCore* b);
private:
    bool _isCloseCondition;
    cocos2d::Layer* _lyTopUI;
    cocos2d::Layer* _lyMiddleUI;
    cocos2d::Layer* _lyBottomUI;
    cocos2d::extension::TableView *_table;
    cocos2d::ParticleSystem* _particleDust;
    cocos2d::ParticleSystem* _particleFire;
   
    InfoAdventureCore* _currentInfoCore;
    std::function<void(void)> _callbackHide;
    std::function<void(PopupAdventureRelicDefault::E_TYPE)> _callbackMoveShop;
    
    cocos2d::ui::Button* _btnTutorial;
    
    cocos2d::Vector<InfoAdventureCore*> _listCore;
};

#endif /* UILayerAdventureRelicCrafting_hpp */
