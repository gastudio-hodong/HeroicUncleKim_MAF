//
//  PopupStone.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/23.
//

#ifndef PopupStone_hpp
#define PopupStone_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

class PopupStone : public PopupBase, public IRefresh, public cocos2d::ui::DelegateListView
{
public:
    enum class E_TYPE{
        Storage = 0,
        
        Default_Attack = 1,
        Default_Gold = 2,
        Default_Key = 3,
        
        Force_Attack = 4,
        Force_Gold = 5,
        Force_Key = 6,
        
        Force_2_Attack = 7,
    };
    
public:
    CREATE_FUNC(PopupStone);
    
    PopupStone();
    virtual ~PopupStone(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;
    
    // listview
    virtual void listviewScrollView(cocos2d::ui::CListView* listview, cocos2d::ui::ScrollView::EventType type) override;
    virtual cocos2d::Size listviewCellSize(cocos2d::ui::CListView* listview, ssize_t idx) override;
    virtual void listviewCell(cocos2d::ui::CListView *listview, cocos2d::ui::Widget* childview, ssize_t idx) override;
    virtual void listviewCellStorage(cocos2d::ui::Layout *uiLayout);
    virtual void listviewCellStone(cocos2d::ui::Layout *uiLayout, E_STONE eStone);
    virtual void listviewCellStoneForce(cocos2d::ui::Layout *uiLayout, E_STONE eStone);
    virtual int64_t listviewNumberOfCells(cocos2d::ui::CListView *listview) override;
    
protected:
    void initVar();
    void initUI();
    
    //ui
    void uiContainer();
    void uiInfo();
    void uiList();
    
    // ui : draw
    void drawInfo();
    void drawList();
    
    //
    
    //
    void onClickClose(Ref* sender);
    void onClickHelp(Ref* sender);
    void onClickBuyStorage(Ref* sender);
    void onClickBuyStone(Ref* sender);
    
    // callback
    void callbackInfo(bool bResult);
     
private:
    bool _bInitUI;
    
    //
    std::vector<E_TYPE> _listStone;
    
    
    //
    cocos2d::ui::Layout* _uiContentsInfo;
    cocos2d::ui::Layout* _uiContentsList;
    
    cocos2d::ui::CListView* _uiList;
    
};

#endif /* PopupStone_hpp */
