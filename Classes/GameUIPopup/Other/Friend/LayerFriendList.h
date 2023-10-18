//
//  LayerFriendList.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef LayerFriendList_h
#define LayerFriendList_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class LayerFriendList : public cocos2d::LayerColor, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static LayerFriendList* create(cocos2d::Size size, E_PLACE ePlace);

    LayerFriendList(void);
    virtual ~LayerFriendList(void);
    virtual bool init(cocos2d::Size size, E_PLACE ePlace);

    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;

public:
    // set, get, add
    void setCallbackHide(const std::function<void(void)>& callback);
    
    // network
    void requestInfo();
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiInfo();
    void uiList();
    void uiBottom();
    
    // draw
    void drawInfo();
    void drawTable();
    void showPetSelect(double nPosX, double nPosY);
    void hidePetSelect();
    
    // click
    void onClickDibs(cocos2d::Ref* sender);
    void onClickDeepLink(cocos2d::Ref* sender);
    void onClickDelete(cocos2d::Ref* sender);
    void onClickPointReceive(cocos2d::Ref* sender);
    void onClickPointSend(cocos2d::Ref* sender);
    void onClickPointAll(cocos2d::Ref* sender);
    void onClickPetSelect(cocos2d::Ref* sender);
    void onClickPetChange(cocos2d::Ref* sender);
    void onClickDraw(cocos2d::Ref* sender);
    
    void onClickUserDetail(cocos2d::Ref* sender);
    
    // network
    
    
    // callback
    void callbackList(bool bResult);
    void callbackDel(bool bResult, int nResult);
    void callbackPet(bool bResult);
    void callbackPointReceive(bool bResult, int nResult);
    void callbackPointSend(bool bResult, int nResult);
    void callbackPointAll(bool bResult, int nResult);
    
private:
    E_PLACE _ePlace;
    
    // callback
    std::function<void(void)> _onCallbackHide;
    
    // widget
    cocos2d::Layer* _layerContainerInfo;
    cocos2d::Layer* _layerContainerList;
    cocos2d::Layer* _layerContainerBottom;
    cocos2d::Layer* _layerContainerPetSelect;
    
    // table
    cocos2d::extension::TableView* _tableList;
    
    // other
    cocos2d::Sprite* _spritePetIcon;
    
};
#endif /* LayerFriendList_h */
