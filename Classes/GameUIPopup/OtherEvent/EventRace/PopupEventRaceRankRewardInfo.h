//
//  PopupEventRaceRankRewardInfo.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/08.
//

#ifndef PopupEventRaceRankRewardInfo_hpp
#define PopupEventRaceRankRewardInfo_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/InfoItems.h"

class InfoEventRaceRankReward;
class PopupEventRaceRankRewardInfo : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    enum class E_RANK_TYPE
    {
        RANK = 0,
        RANK_RANGE,
        END
    };
public:
    static PopupEventRaceRankRewardInfo* create();
    
    PopupEventRaceRankRewardInfo();
    virtual ~PopupEventRaceRankRewardInfo(void);
    virtual bool init() override;
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
protected:
    void setLoadRankReward();
    void initVar();
    void initUI();
    
    void uiTitle();
    void uiBanner();
    void uiTable();
    void uiBottom();
    
    void onClickClose(Ref* sender);
    
private:
    cocos2d::Layer* _lyContainerParent;
    cocos2d::Layer* _lyTitle;
    cocos2d::Layer* _lyBanner;
    cocos2d::Layer* _lyTable;
    cocos2d::Layer* _lyBottom;
    cocos2d::extension::TableView *_table;
    
    std::vector<InfoEventRaceRankReward*> _vecInfo;
};

class InfoEventRaceRankReward
{
public:
    InfoEventRaceRankReward();
    ~InfoEventRaceRankReward();
    
    PopupEventRaceRankRewardInfo::E_RANK_TYPE getType();
    int getIdx();
    int getCondition1();
    int getCondition2();
    cocos2d::Vector<InfoItems*> getListReward();
    
    void setType(PopupEventRaceRankRewardInfo::E_RANK_TYPE value);
    void setIdx(int value);
    void setCondition1(int value);
    void setCondition2(int value);
    void setListReward(cocos2d::Vector<InfoItems*> value);
    
private:
    PopupEventRaceRankRewardInfo::E_RANK_TYPE _eType;
    int _nIdx;
    int _nCondition1;
    int _nCondition2;
    cocos2d::Vector<InfoItems*> _listReward;
};


#endif /* PopupEventRaceRankRewardInfo_hpp */
