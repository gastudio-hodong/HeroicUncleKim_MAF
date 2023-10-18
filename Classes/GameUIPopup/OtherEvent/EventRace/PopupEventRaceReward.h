//
//  PopupEventRaceReward.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/29.
//

#ifndef PopupEventRaceReward_hpp
#define PopupEventRaceReward_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/EventRace/InfoEventRaceHorse.h"

class PopupEventRaceReward : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
    struct InfoGameRankRewardRule
    {
        int nRank = 0;
        int nRewardIdx = 0;
        float nRewardMag = 0;
    };
public:
    static PopupEventRaceReward* create();
    
    PopupEventRaceReward();
    virtual ~PopupEventRaceReward(void);
    virtual bool init() override;
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
protected:
    static bool sortRank(InfoEventRaceHorse* const a, InfoEventRaceHorse* const b);
    
    void initVar();
    void initUI();
    
    void setLoadInfoGameRewardRule();
    
    void uiTop();
    void uiTopLane();
    void uiTopPlatform();
    void uiTable();
    void uiBottom();
    
    
    void onClickClose(Ref* sender);
    InfoEventRaceHorse* getHorseByRank(const int nRank);
    
    bool isBankrupt();
    InfoGameRankRewardRule getInfoGameRewardRule(const int nRank);
private:
    std::vector<InfoGameRankRewardRule> _listInfoGameRewardRule;
    Vector<InfoEventRaceHorse*> _listHorse;
    cocos2d::Layer* _lyTop;
    cocos2d::Layer* _lyTopLane;
    cocos2d::Layer* _lyTopPlatform;
    cocos2d::Layer* _lyTable;
    cocos2d::Layer* _lyBottom;
    cocos2d::extension::TableView *_table;
};


#endif /* PopupEventRaceReward_hpp */
