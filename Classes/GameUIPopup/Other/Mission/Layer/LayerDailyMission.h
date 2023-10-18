//
//  LayerDailyMission.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/24.
//

#ifndef LayerDailyMission_hpp
#define LayerDailyMission_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoServerMission.h"

#include "GameUI/UICell/ModelCell.h"

class LayerDailyMission : public cocos2d::LayerColor, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static LayerDailyMission* create(const E_MISSION_RESET_TYPE type, const cocos2d::Size size);
    
    LayerDailyMission(void);
    virtual ~LayerDailyMission();
    bool init(const E_MISSION_RESET_TYPE type, const cocos2d::Size size);
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
protected:
    
    void initVar();
    void initUI();
    
    void uiTable();
    
    // click
    void onClickShortCut(cocos2d::Ref* sender);
    void onClickComplete(cocos2d::Ref* sender);
    
    void setServerMissionOder();
    
    void callbackUpdate(bool bResult, int nResult);
    void callbackComplete(bool bResult, int nResult);
    
public:
    void requestUpdate();
    
    void setCallbackHide(const std::function<void()>& pCallback);
    void setCallbackRefresh(const std::function<void()>& pCallback);
private:
    bool _isPlayTimeMissionComplete;
    E_MISSION_RESET_TYPE _type;
    
    cocos2d::Layer* _lyTable;
    cocos2d::extension::TableView* _table;
    std::function<void()> _callbackHide;
    std::function<void()> _callbackRefresh;
    
    cocos2d::Vector<InfoServerMission*> _vecDailyInfoMission;
};


#endif /* LayerDailyMission_hpp */
