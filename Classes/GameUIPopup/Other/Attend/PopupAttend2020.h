//
//  PopupAttend2020.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupAttend2020_h
#define PopupAttend2020_h

#include "GameUIPopup/Base/PopupBase.h"

class InfoAttend;
class PopupAttend2020 : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static PopupAttend2020* create();
    
    PopupAttend2020(void);
    virtual ~PopupAttend2020(void);
    virtual bool init();
    virtual void onEnter();
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);
    
public:
    // set
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContents();
    void uiInfo();
    
    // draw
    void drawTitle();
    void drawContetns();
    
    // set, get, other
    void setLoad(int month);
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickReward(cocos2d::Ref* sender);
    
    // callback
    void callbackAds(ADS_RESULT result);
    
    
    // network
    void requestInfo();
    void responseInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void requestReward();
    void responseReward(cocos2d::network::HttpResponse* response,std::string &data);
    
private:
    cocos2d::Vector<InfoAttend*> _listAttendAll;
    cocos2d::Vector<InfoAttend*> _listAttend;
    std::vector<bool> _listAttendReceive;
    
    int _nMonth;
    int _nToday;
    bool _bBonus;
    
    int _nReceiveCount;
    int _nSelectday;
    bool _bAds;
    
    
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerInfo;
    cocos2d::Label* _labelTitle;
    cocos2d::extension::TableView* _table;
  
};

#endif /* PopupAttend2020_h */
