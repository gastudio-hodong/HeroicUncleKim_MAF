//
//  MailPopup.hpp
//  FantasyClicker
//
//  Created by KangChangJu on 2015. 11. 4..
//
//

#ifndef MailPopup_hpp
#define MailPopup_hpp

#include "GameUIPopup/Base/PopupBase.h"

class InfoMail;
class InfoItems;
class MailPopup : public PopupBase, cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    CREATE_FUNC(MailPopup);
    
    MailPopup(void);
    virtual ~MailPopup(void);
    virtual bool init();
    virtual void onEnter();

    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);
    

    
private:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiAds();
    void uiMail();
    
    void uiDrawList();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickAds(cocos2d::Ref* sender);
    void onClickMailRecevice(cocos2d::Ref* sender);
    void onClickMailReceviceAll(cocos2d::Ref* sender);
    
    // callback
    
    
    // network
    void requestMailList();
    void responseMailList(cocos2d::network::HttpResponse* response,std::string &data);
    void requestMailReceive();
    void responseMailReceive(cocos2d::network::HttpResponse* response,std::string &data);
    
    

    
private:
    cocos2d::Vector<InfoMail*> _listMail;
    cocos2d::Vector<InfoMail*> _listMailReceive;
    cocos2d::Vector<InfoItems*> _listReward;
    
    int _nMailReceiveSuccess;
    int _nMailReceiveError;
    
    //
    std::string _strAdsPath;
    std::string _strAdsLink;

    //
    cocos2d::Layer* _layerContainerMail;
    
    cocos2d::extension::TableView *_table;
    cocos2d::Label* _labelListNothing;
 
    
    
};

#endif /* MailPopup_hpp */
