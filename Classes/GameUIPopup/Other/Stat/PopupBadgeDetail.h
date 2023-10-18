//
//  PopupBadgeDetail.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/10/13.
//

#ifndef PopupBadgeDetail_h
#define PopupBadgeDetail_h

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/InfoBadge.h"

class PopupBadgeDetail : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static PopupBadgeDetail* create(int nBadgeIdx);
    
    PopupBadgeDetail(void);
    virtual ~PopupBadgeDetail(void);
    virtual bool init(int nBadgeIdx);
    virtual void onEnter() override;
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    

    
public:
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiBadgeInfo();
    void uiBadgeList();
 
    // draw

    // set, get
    
    
    // click
    void onClickClose(cocos2d::Ref* sender);
     
     
    // network
    
    // callback

    
private:
    int _nBadgeIdx;
    float _nBadgeBuffValue;
    
    InfoBadge* _objBadge;
    cocos2d::Vector<InfoBadgeHave*> _listBadgeHave;
 
    
    // widget
    cocos2d::Layer* _layerContainerBadgeInfo;
    cocos2d::Layer* _layerContainerBadgeList;
 
    
    // table
    cocos2d::extension::TableView* _tableList;
};

#endif /* PopupBadgeDetail_hpp */
