//
//  PopupEquipDetail.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 23/09/2019.
//

#ifndef PopupEquipDetail_h
#define PopupEquipDetail_h

#include "GameUIPopup/Base/PopupBase.h"

#include "GameUI/UICell/ModelCellDevil.h"

class PopupEquipDetail : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    
    static PopupEquipDetail* create(ModelCellDevil::CellDevilEquip *cell);
    PopupEquipDetail(ModelCellDevil::CellDevilEquip *cell);
    virtual ~PopupEquipDetail(void);
    virtual bool init();
    
    // table TableViewDelegate
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view){};
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);
    
protected:
    
    // init
    void initUi();
    
    // ui
    void uiContents();
    void uiButton();
    
    //draw
    void redrawContents();
    
    // click
    void onClickOption(Ref* sender);
    void onClickClose(Ref* sender);
    void onEquipReinforce(Ref* sender);
    
    void checkAgree();
    
    void requestCheckCostumeCoin();
    void ResultCheckCostumeCoin(cocos2d::network::HttpResponse* response,std::string &data);
    
    void requestEvolutionCostumeCoin();
    void ResultEvolutionCostumeCoin(cocos2d::network::HttpResponse* response,std::string &data);
    void onSuccessReinforceEvolution();

    // callback
    void callbackSave(bool bResult);
    
    
private:

    // widget
    cocos2d::Layer* _layerContainerParent;
    cocos2d::Layer* _layerContainerContents;
    
    MafNode::MafMenuItemSprite *_itemReinforceEvolution;
    MafNode::MafMenu* _menuBottom;
    cocos2d::Sprite* _sprIcon;
    cocos2d::Label* _lbName;
    cocos2d::Label* _lbRein;
    cocos2d::Label* _lbEffect;
    cocos2d::Label* _lbKeyRein;
    cocos2d::extension::TableView* _table;
    
    std::vector<std::string> _uLink;
    std::vector<bool> _uAgree;
    
    ModelCellDevil::CellDevilEquip *_cell;
    
    
};


#endif /* PopupEquipDetail_hpp */
