//
//  UILayoutPetSkin.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef UILayoutPetSkin_h
#define UILayoutPetSkin_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoPetSkin;
class UILayoutPetSkin : public cocos2d::ui::Layout, public cocos2d::ui::DelegateListView
{
public:
    static UILayoutPetSkin* create(E_PET ePet);

    UILayoutPetSkin(void);
    virtual ~UILayoutPetSkin(void);
    virtual bool init(E_PET ePet);
    virtual void onEnter() override;


    // listview
    virtual void listviewScrollView(cocos2d::ui::CListView* listview, cocos2d::ui::ScrollView::EventType type) override;
    virtual cocos2d::Size listviewCellSize(cocos2d::ui::CListView* listview, ssize_t idx) override;
    virtual void listviewCell(cocos2d::ui::CListView *listview, cocos2d::ui::Widget* childview, ssize_t idx) override;
    virtual int64_t listviewNumberOfCells(cocos2d::ui::CListView *listview) override;
    
    
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
    void onClickSkin(cocos2d::Ref* sender);
    
    // callback
    void callbackSkinBuy(bool bResult, int nResult);
    
private:
    bool _bInitUI;
    
    //
    E_PLACE _ePlace;
    E_PET _ePet;
    
    cocos2d::Vector<InfoPetSkin*> _listPetSkin;
    
    // widget
    cocos2d::ui::Layout* _uiContentsUIList;
    
    cocos2d::ui::CListView* _uiList;
    cocos2d::ui::ImageView* _uiListDownArrow;
    
};
#endif /* UILayoutPetSkin_h */
