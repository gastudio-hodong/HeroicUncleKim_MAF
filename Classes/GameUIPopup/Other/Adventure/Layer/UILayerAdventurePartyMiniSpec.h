//
//  UILayerAdventurePartyMiniSpec.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/06.
//

#ifndef UILayerAdventurePartyMiniSpec_hpp
#define UILayerAdventurePartyMiniSpec_hpp

#include "GameUIPopup/Other/Adventure/Layer/UILayerAdventureParent.h"

class UILayerAdventurePartyMiniSpec : public UILayerAdventureParent, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static UILayerAdventurePartyMiniSpec* create(const cocos2d::Size size);
    
    UILayerAdventurePartyMiniSpec(void);
    virtual ~UILayerAdventurePartyMiniSpec(void);
    virtual bool init(const cocos2d::Size size);
    
    virtual void refreshUI() override;
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;

protected:
    // init
    void initVar();
    
    // ui
    void uiTop();
    void uiMiddle();
    void uiTable();
    
    void uiMyTeam();
    void uiFriend();
    
    void setEffectList();
    // click
    void onClickParty(Ref* const pSender);
    
public:
    void initUi();
    //set
    void setIsFriend(const bool isFriend);
    void setIsPopup(const bool isPopup);
    void setAdventureID(const int nAdventureID);
    
private:
    bool _isFriend;
    bool _isPopup;
    int _nCurrentAdventureID;
    
    cocos2d::Layer* _lyTop;
    cocos2d::Layer* _lyMiddle;
    cocos2d::Layer* _lyTable;
    cocos2d::Layer* _lyFriend;
    cocos2d::extension::TableView *_table;
    
    std::vector<E_ADVENTURE_PROPERTY_TYPE> _listEffect;
};

#endif /* UILayerAdventurePartyMiniSpec_hpp */
