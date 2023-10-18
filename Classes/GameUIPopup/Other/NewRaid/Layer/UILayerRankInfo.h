//
//  UILayerRankInfo.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/01/27.
//

#ifndef UILayerRankInfo_hpp
#define UILayerRankInfo_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/Raid/InfoRaidBongStat.h"

#include "GameUIPopup/Other/NewRaid/Layer/UILayerRaidParent.h"
#include "GameUIPopup/Other/NewRaid/PopupRaidRankRewardInfo.h"

class UILayerRankInfo : public LayerColor, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static UILayerRankInfo* create(cocos2d::Size size, std::vector<InfoRaidTierInfo*> list);
    
    UILayerRankInfo(void);
    virtual ~UILayerRankInfo(void);
    virtual bool init(cocos2d::Size size, std::vector<InfoRaidTierInfo*> list);
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
public:
    // set, get
    
protected:
    // init
    void initVar();
    void initUi();
    
private:
    std::vector<InfoRaidTierInfo*> _vecInfo;
    
    cocos2d::extension::TableView *_table;
};

#endif /* UILayerRankInfo_hpp */
