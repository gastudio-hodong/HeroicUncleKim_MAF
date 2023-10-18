//
//  LayerSpecialMission.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/24.
//

#ifndef LayerSpecialMission_hpp
#define LayerSpecialMission_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/InfoSpecialMission.h"


class LayerSpecialMission : public cocos2d::LayerColor, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static LayerSpecialMission* create(const cocos2d::Size size);

    LayerSpecialMission(void);
    virtual ~LayerSpecialMission();
    bool init(const cocos2d::Size size);

    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;

    void uiReload();
protected:
    
    void initVar();
    void initUI();

    void uiTop();
    void uiTable();

    // click
    void onClickShortCut(cocos2d::Ref* sender);
    void onClickComplete(cocos2d::Ref* sender);

    void callbackComplete(bool bResult, int nResult);
    void setMissionOder();
public:

    void setCallbackHide(const std::function<void()>& pCallback);
    void setCallbackRefresh(const std::function<void()>& pCallback);
private:
    cocos2d::Layer* _lyTop;
    cocos2d::Layer* _lyTable;
    cocos2d::extension::TableView* _table;
    std::function<void()> _callbackHide;
    std::function<void()> _callbackRefresh;

    cocos2d::Vector<InfoSpecialMission*> _listInfoSpecial;
};

#endif /* LayerSpecialMission_hpp */
