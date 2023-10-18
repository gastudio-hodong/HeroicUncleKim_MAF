//
//  UILayerRaidStatus.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/01.
//

#ifndef UILayerRaidStatus_hpp
#define UILayerRaidStatus_hpp


#include "UILayerRaidParent.h"

#include "GameObject/Raid/InfoRaidBongStat.h"

class UILayerRaidStatus : public UILayerRaidParent, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static UILayerRaidStatus* create(cocos2d::Size size);
    
    UILayerRaidStatus(void);
    virtual ~UILayerRaidStatus(void);
    virtual bool init(cocos2d::Size size);
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
    virtual void refreshUI() override;
public:
    // set, get
    
    void setCallbackLevelup(const std::function<void(void)>& callback);
    void setCallbackHide(const std::function<void(void)>& callback);
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiTop();
    void uiList();
    
    // ui draw
    void drawList();
    
    // set, get
    void setList();

    // click
    void onClickBongLevelUp(Ref* pSender);
    void onClickStatLevelUp(Ref* pSender);
    
    //action
    void actionBongLevelup(Node* pNode);
    
    void callbackLevelup(bool bResult, int nResult, int idx);
private:
    cocos2d::Vector<InfoRaidBongStat*> _listInfoBongStat;
    std::map<int, bool> _mapLevelupIdx;
    
    cocos2d::Layer* _lyTop;
    cocos2d::Layer* _lyStatus;
    cocos2d::extension::TableView *_table;
    cocos2d::Label* _lbBongLevel;
    std::function<void(void)> _callbackLevelup;
    std::function<void(void)> _callbackHide;
};


#endif /* UILayerRaidStatus_hpp */
