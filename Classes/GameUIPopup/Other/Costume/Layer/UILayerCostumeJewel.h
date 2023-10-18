//
//  UILayerCostumeJewel.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef UILayerCostumeJewel_h
#define UILayerCostumeJewel_h

#include "GameUIPopup/Other/Costume/Layer/UILayerCostumeParent.h"

class InfoCostume;
class UILayerCostumeJewel : public UILayerCostumeParent, cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
private:
    enum class E_DETAIL_TYPE{
        DEFAULT=0,
        NORMAL,
        PRISON,
        EXIST,
        NONE,
        DIBS
    };
    
public:
    static UILayerCostumeJewel* create(std::string strType, cocos2d::Size size);
    
    UILayerCostumeJewel(void);
    virtual ~UILayerCostumeJewel(void);
    virtual bool init(std::string strType, cocos2d::Size size);
    virtual void setContentSize(const cocos2d::Size & var) override;
    virtual void refreshList() override;
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
public:
    // set, get
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiDetailType();
    void uiList();
    
    // ui draw
    void drawDetailType();
    void drawList();
    
    // set, get
    void setList();

    // click
    void onClickDetailType(cocos2d::Ref* sender);
    void onClickReview(cocos2d::Ref* sender);
    void onClickDips(cocos2d::Ref* sender);
    void onClickAction(cocos2d::Ref* sender);
    void onClickActionHonor(cocos2d::Ref* sender);
    void onClickInfo(cocos2d::Ref* sender);

    // callback
    void callbackBuy(bool bResult, int nReslut);
    void callbackBuyHonor(bool bResult);
    void callbackBuyLegend(bool bResult);

private:
    std::string _strType;
    E_DETAIL_TYPE _eDetailType;
    
    cocos2d::Vector<InfoCostume*> _listCostume;
    cocos2d::Vector<InfoCostume*> _listCostumeTemp;
    
    // widget
    cocos2d::Layer* _layerContainerType;
    cocos2d::Layer* _layerContainerList;
    cocos2d::extension::TableView *_table;
};

#endif /* UILayerCostumeJewel_h */
