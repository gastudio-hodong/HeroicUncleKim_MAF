//
//  UILayoutMine.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef UILayoutMine_h
#define UILayoutMine_h

#include "GameUIPopup/Base/PopupBase.h"

class UILayoutMine : public cocos2d::ui::Layout, public cocos2d::ui::DelegateListView
{
public:
    static UILayoutMine* create();
    
    UILayoutMine(void);
    virtual ~UILayoutMine(void) {};
    virtual bool init() override;
    virtual void onEnter() override;
    
    // listview
    virtual void listviewScrollView(cocos2d::ui::CListView* listview, cocos2d::ui::ScrollView::EventType type) override;
    virtual cocos2d::Size listviewCellSize(cocos2d::ui::CListView* listview, ssize_t idx) override;
    virtual void listviewCell(cocos2d::ui::CListView *listview, cocos2d::ui::Widget* childview, ssize_t idx) override;
    virtual int64_t listviewNumberOfCells(cocos2d::ui::CListView *listview) override;
    
public:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContainer();
    void uiMineInfo();
    void uiList();
    
    // ui draw
    void drawMinePet();
    void drawMineInfo();
    void drawList();
    
    //callback
    void callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems);
    
    // click
    void onClickUpgradeArtifactAsk(cocos2d::Ref* sender);
    void onClickUpgradeArtifact(cocos2d::Ref* sender);
    void onClickMineSpecialChange(cocos2d::Ref* sender);
    void onClickMineBuy(cocos2d::Ref* sender);
    void onClickMineBuyPurchase(cocos2d::Ref* sender);
    

private:
    E_PLACE _ePlace;
    E_ARTIFACT _eArtifact;
    E_MINERAL _eMineralSpecial;
    
    std::vector<E_MINERAL> _listMine;
    
    
    // widget
    cocos2d::ui::Layout* _uiContainerMineInfo;
    cocos2d::ui::Layout* _uiContainerList;
    
    cocos2d::ui::CListView* _uiList;
    
    cocos2d::ui::ImageView* _uiImgPet;
    cocos2d::ui::Button* _uiBtnArtifactUpgrade;
    cocos2d::ui::CText* _uiTextArtifactName;
    cocos2d::ui::CText* _uiTextArtifactDesc;
    cocos2d::ui::CText* _uiTextArtifactInfo;
    
    
 
};
#endif /* UILayoutMine_h */
