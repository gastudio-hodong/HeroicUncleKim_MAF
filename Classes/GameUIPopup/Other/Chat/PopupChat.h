//
//  PopupChat.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef PopupChat_h
#define PopupChat_h

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

class InfoChat;
class PopupChat : public PopupBase, public IRefresh, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static PopupChat* create(E_CHAT_TYPE eType);
    
    PopupChat(void);
    virtual ~PopupChat(void);
    virtual bool init(E_CHAT_TYPE eType);
    virtual void onEnter() override;
    
    // table TableViewDelegate
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override;
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
    
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;
    
public:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContent();
    void uiInput();
    
    void uiReviewMy();
    
    // ui draw
    void drawContent();
    void drawTable(bool bReload);
    void drawInput();
    
    //callback
    void callbackInfo(bool bResult, int nResult);
    void callbackMessage(bool bResult, int nResult);
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickReload(cocos2d::Ref* sender);
    void onClickSetting(cocos2d::Ref* sender);
    void onClickMessage(cocos2d::Ref* sender);
    
 
 
    
private:
    cocos2d::Vector<InfoChat*> _listChatInfo;
    
    E_CHAT_TYPE _eType;
    
    bool _isRequestChatList;
    
    // widget
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerontainerInput;
    
    cocos2d::extension::TableView *_table;
    cocos2d::ui::EditBox* _edit;
    
    
    cocos2d::Label* _labelListNothing;
    cocos2d::Sprite* _spriteShadow;
    
 
};
#endif /* PopupChat_h */
