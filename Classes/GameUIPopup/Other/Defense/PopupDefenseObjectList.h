//
//  PopupDefenseObjectList.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/03.
//

#ifndef PopupDefenseObjectList_hpp
#define PopupDefenseObjectList_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/ConfigEnum.h"

class InfoDefenseWeapon;
class InfoDefenseCharacter;

class PopupDefenseObjectList : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static PopupDefenseObjectList* create(E_DF_Trench_SettingType type);
    
    PopupDefenseObjectList(void);
    virtual ~PopupDefenseObjectList(void);
    virtual void onEnter() override;
    virtual bool init(E_DF_Trench_SettingType type);
    
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
    void createCell(Node* cell, const int cellNumber);
    
    virtual void hide() override;
public:
    
    void initVar();
    void initUI();
    
    void uiTop();
    void uiTable();
    void uiBottom();
    
    void onClickTab(Ref* sender);
    void onClickObject(Ref* sender);
    void onClickAllRankUP(Ref* sender);
    void onClickClose(Ref* sender);
    
    void requestAllRankUp();
    
    void callbackServer(bool bResult, int nResult);
    
    void callbackAllRankUp(bool bResult, int nResult);
    
    
private:
    E_DF_Trench_SettingType _type;
    bool _isAllRankup;
    int _nCurrentRankUpIdx;
    cocos2d::Layer* _lyTop;
    cocos2d::Layer* _lyTable;
    cocos2d::Layer* _lyBottomUI;
    cocos2d::extension::TableView *_table;
    cocos2d::Vector<InfoDefenseWeapon*> _listInfoWeapon;
    cocos2d::Vector<InfoDefenseCharacter*> _listInfoCharacter;
};

#endif /* PopupDefenseObjectList_hpp */
