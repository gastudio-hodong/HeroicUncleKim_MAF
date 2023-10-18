//
//  PopupRaidMission.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/15.
//

#ifndef PopupRaidMission_hpp
#define PopupRaidMission_hpp

#include "GameUIPopup/Base/PopupBase.h"

class InfoQuest;
class InfoItems;
class PopupRaidMission : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static PopupRaidMission* create();
    
    PopupRaidMission(void);
    virtual ~PopupRaidMission(void);
    virtual bool init() override;
    
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
public:
    
    void initVar();
    void initUI();
    
    void uiTop();
    
    void uiMission();
    
    void uiBottom();
    
    void onClick(cocos2d::Ref* sender);
    void onClickComplete(cocos2d::Ref* sender);
    void onClickAllComplete(cocos2d::Ref* sender);
    void onClickClose(cocos2d::Ref* sender);
    void onClickTicketBuy(cocos2d::Ref* sender);
    
    void sortingTotalReward();
    void addTotalReward(InfoItems* infoItems);
    
    void setCallbackHide(const std::function<void(void)>& callback);
    
    void callbackComplete(bool bResult, int nResult);
    void callbackProductOk(int nIdx);
    void callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
private:
    
    cocos2d::Layer* _lyTopUI;
    cocos2d::Layer* _lyMissionUI;
    cocos2d::Layer* _lyBottomUI;
    cocos2d::extension::TableView *_table;
    cocos2d::Vector<InfoQuest*> _listInfoQuest;
    cocos2d::Vector<InfoItems*> _listTotalReward;
    std::function<void(void)> _callbackHide;
};

#endif /* PopupRaidMission_hpp */
