//
//  PopupPetEquipBoxReward.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/05/21.
//

#ifndef PopupPetEquipBoxReward_h
#define PopupPetEquipBoxReward_h

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

#include "GameObject/InfoPetEquip.h"

class InfoPetEquip;
class InfoPetEquipDraw;
class InfoPetEquipDrawGrade;

class PopupPetEquipBoxReward : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static PopupPetEquipBoxReward* create(E_PET_BOX_TYPE eType);
    
    PopupPetEquipBoxReward(void);
    virtual ~PopupPetEquipBoxReward(void);
    
    // table TableViewDelegate
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override;
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
    
    
    virtual bool init(E_PET_BOX_TYPE eType);
    
    void setCallbackRedraw(const std::function<void(void)>& pCallback);
    void setCallbackHide(const std::function<void(void)>& pCallback);
    
    void onUseBox(int nUse, int nType);
    
protected:
    // init
    void initVar();
    void initUi();
    
    void uiContents();
    
    void uiTable();
    void actionBoxOpen(int nUseCount);
    
    // click
    void onClick(cocos2d::Ref* sender);
    void onHide();
    
    //callback
    void callbackUse(bool bResult, int nUse, cocos2d::Vector<InfoPetEquipDraw*> listDraw, cocos2d::Vector<InfoPetEquipDrawGrade*> lisDrawGrade);
    
    void callbackRedraw();
    void callbackPurchase();
    
    void callbackUseBox(int nUse, int nType);
private:
    bool _isEvent;
    bool _isMileage;
    E_PET_BOX_TYPE _eType;
    
    // widget
    cocos2d::Layer* _lyContents;
    
    cocos2d::Layer* _lyRewardTable;
    cocos2d::extension::TableView * _table;
    
    cocos2d::Sprite* _sprBox;
    cocos2d::Sprite* _sprMagnificationSPBox;
    
    //callback
    std::function<void(void)> _callbackRedraw;
    std::function<void(void)> _callbackHide;
    
    cocos2d::Vector<InfoPetEquipDraw*> _listDraw;
    cocos2d::Vector<InfoPetEquipDrawGrade*> _lisDrawGrade;
    cocos2d::Vector<cocos2d::Sprite*> _listBox;
    cocos2d::Vector<cocos2d::Sprite*> _listBoxBonusBadge;
    cocos2d::Vector<MafNode::MafMenuItemSprite*> _listUseButton;
};

#endif /* PopupPetEquipBoxReward_hpp */
