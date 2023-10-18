//
//  LayerFriendInvite.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef LayerFriendInvite_h
#define LayerFriendInvite_h

#include "Common/ConfigDefault.h"

class LayerFriendInvite : public cocos2d::LayerColor, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
private :
    enum eType {
        type_recommend = 1,
        type_wait = 2,
        type_search = 3,
    };
    
public:
    static LayerFriendInvite* create(cocos2d::Size size);

    LayerFriendInvite(void);
    virtual ~LayerFriendInvite(void);
    virtual bool init(cocos2d::Size size);

    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
public:
    // set, get, add
    void setCallbackHide(const std::function<void(void)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiSearch();
    void uiInfo();
    void uiList();
    void uiBottom();
    
    // draw
    void drawInfo();
    void drawTable();
    void drawSearch();
    
    // click
    void onClickSearch(cocos2d::Ref* sender);
    void onClickRecommend(cocos2d::Ref* sender);
    void onClickWait(cocos2d::Ref* sender);
    void onClickRefresh(cocos2d::Ref* sender);
    void onClickIsRecommend(cocos2d::Ref* sender);
    void onClickIsRecommendInfo(cocos2d::Ref* sender);
    
    void onClickRecommendAdd(cocos2d::Ref* sender);
    void onClickSearchAdd(cocos2d::Ref* sender);
    void onClickWaitAccept(cocos2d::Ref* sender);
    void onClickWaitReject(cocos2d::Ref* sender);
    void onClickUserDetail(cocos2d::Ref* sender);
    
    
    // network
    void requestRecommend();
    void requestWait();
    
    // callback
    void callbackInfo(bool bResult);
    void callbackAddRequest(bool bResult, int nResult);
    void callbackAddResult(bool bResult, int nResult);
    void callbackIsRecommend(bool bResult);
    
private:
    //
    LayerFriendInvite::eType _eType;
    
    int64_t _updateTimeRecommend;
    int64_t _updateTimeWait;
    
    // callback
    std::function<void(void)> _onCallbackHide;
    
    // widget
    cocos2d::Layer* _layerContainerSearch;
    cocos2d::Layer* _layerContainerInfo;
    cocos2d::Layer* _layerContainerList;
    cocos2d::Layer* _layerContainerBottom;
    
    // table
    cocos2d::extension::TableView* _tableList;
    
    // other
    cocos2d::Label* _labelInfoName;
    cocos2d::Label* _labelInfoRefresh;
    MafNode::MafMenuItemSprite* _itemInfoRefresh;
    MafNode::MafMenuItemSprite* _itemIsRecommend;
    MafNode::MafMenuItemSprite* _itemIsRecommendInfo;
    cocos2d::Label* _labelIsRecommendText;
    
};
#endif /* LayerFriendInvite_h */
