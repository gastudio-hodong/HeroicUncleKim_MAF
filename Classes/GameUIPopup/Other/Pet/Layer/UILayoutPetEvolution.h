//
//  UILayoutPetEvolution.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef UILayoutPetEvolution_h
#define UILayoutPetEvolution_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class UILayoutPetEvolution : public cocos2d::ui::Layout, public cocos2d::ui::DelegateListView
{
public:
    static UILayoutPetEvolution* create(E_PET ePet);

    UILayoutPetEvolution(void);
    virtual ~UILayoutPetEvolution(void);
    virtual bool init(E_PET ePet);
    virtual void onEnter() override;

    // listview
    virtual void listviewScrollView(cocos2d::ui::CListView* listview, cocos2d::ui::ScrollView::EventType type) override;
    virtual cocos2d::Size listviewCellSize(cocos2d::ui::CListView* listview, ssize_t idx) override;
    virtual void listviewCell(cocos2d::ui::CListView *listview, cocos2d::ui::Widget* childview, ssize_t idx) override;
    virtual int64_t listviewNumberOfCells(cocos2d::ui::CListView *listview) override;
    
public:
    // set, get, add
    
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiList();
    
    // draw
    void drawList();
    
    // click
    void onClickReward(cocos2d::Ref* sender);
    
    // callback
     
    
private:
    bool _bInitUI;
    
    //
    E_PLACE _ePlace;
    E_PET _ePet;
    
    std::vector<int> _listEvolution;
    
    // widget
    cocos2d::ui::Layout* _uiContentsUIList;
    
    cocos2d::ui::CListView* _uiList;
    
};
#endif /* UILayoutPetEvolution_h */
