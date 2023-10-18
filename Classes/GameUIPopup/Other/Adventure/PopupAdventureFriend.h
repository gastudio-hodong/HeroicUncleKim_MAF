//
//  PopupAdventureFriend.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/16.
//

#ifndef PopupAdventureFriend_hpp
#define PopupAdventureFriend_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

#include "GameObject/Adventure/InfoAdventureFriend.h"

class PopupAdventureFriend : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate, public IRefresh
{
public:
    static PopupAdventureFriend* create();
    
    PopupAdventureFriend(void);
    virtual ~PopupAdventureFriend(void);
    virtual void onEnter() override;
    virtual bool init();
    virtual void show() override;
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData)  override;
    
    
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
    void uiTable();
    void uiBottom();
    
    void onClickClose(Ref* const sender);
    void onClickRunning(Ref* const sender);
    void onClickAddFriend(Ref* const sender);
    
    void callbackHide();
private:
    Layer* _lyTopUI;
    Layer* _lyFriends;
    Layer* _lyBottomUI;
    cocos2d::extension::TableView *_table;
    cocos2d::Vector<InfoAdventureFriend*> _listFriend;
};


#endif /* PopupAdventureFriend_hpp */
