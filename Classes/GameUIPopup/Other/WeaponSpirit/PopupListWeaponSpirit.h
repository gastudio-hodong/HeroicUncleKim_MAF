//
//  PopupListWeaponSpirit.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/19.
//

#ifndef PopupListWeaponSpirit_h
#define PopupListWeaponSpirit_h

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

#include "GameObject/Raid/InfoWeaponSpirit.h"

class PopupListWeaponSpirit : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate, public IRefresh
{
public:
    static PopupListWeaponSpirit* create();
    
    PopupListWeaponSpirit(void);
    virtual ~PopupListWeaponSpirit(void);
    virtual void onEnter() override;
    virtual bool init() override;
    
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData)  override;
    
    virtual void hide() override;
public:
    
    void initVar();
    void initUI();
    
    void uiTop();
    void uiSpirit();
    void uiBottom();
    
    void onClickShop(Ref* sender);
    void onClickContract(Ref* sender);
    void onClickSpirit(Ref* sender);
    void onClickAllRankUP(Ref* sender);
    void onClickSynergyInfo(Ref* sender);
    void onClickClose(Ref* sender);
    void setCloseCondition(bool value);
    
    void requestAllRankUp();
    
    void callbackInfo(bool bResult, int nResult);
    
    void callbackRankUp(bool bResult, int nResult);
    
    
private:
    bool _isCloseCondition;
    int _nCurrentRankUpIdx;
    cocos2d::Layer* _lyTopUI;
    cocos2d::Layer* _lySpirits;
    cocos2d::Layer* _lyBottomUI;
    cocos2d::extension::TableView *_table;
    cocos2d::Vector<InfoWeaponSpirit*> _listInfoSpirit;
};
#endif /* PopupListWeaponSpirit_hpp */
