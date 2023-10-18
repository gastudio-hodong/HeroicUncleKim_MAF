//
//  PopupEventSunfishReinforce.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/23.
//

#ifndef PopupEventSunfishReinforce_hpp
#define PopupEventSunfishReinforce_hpp

#include "GameUIPopup/Base/PopupBase.h"

class InfoEventSunfish;
class PopupEventSunfishReinforce : public PopupBase, public cocos2d::ui::DelegateListView
{
public:
    enum class E_TAB
    {
        FOOD = 1,
        ADVENTURE = 2,
    };
    
public:
    static PopupEventSunfishReinforce* create();
    
    PopupEventSunfishReinforce();
    virtual ~PopupEventSunfishReinforce(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
    // listview
    virtual void listviewScrollView(cocos2d::ui::CListView* listview, cocos2d::ui::ScrollView::EventType type) override;
    virtual cocos2d::Size listviewCellSize(cocos2d::ui::CListView* listview, ssize_t idx) override;
    virtual void listviewCell(cocos2d::ui::CListView *listview, cocos2d::ui::Widget* childview, ssize_t idx) override;
    virtual int64_t listviewNumberOfCells(cocos2d::ui::CListView *listview) override;
    
protected:
    void initVar();
    void initUI();
    
    //ui
    void uiContainer();
    void uiInfo();
    void uiTab();
    void uiList();
    
    // ui : draw
    void drawInfo();
    void drawTab();
    void drawList();
    
    //
    
    //
    void onClickClose(Ref* sender);
    void onClickTab(Ref* sender);
    void onClickReinforce(Ref* sender);
    
    //callback
    void callbackReinforce(bool bResult, int nResult);
    
private:
    bool _bInitUI;
    
    //
    E_TAB _eTab;
    
    cocos2d::Vector<InfoEventSunfish*> _listSunfish;
    
    //
    cocos2d::ui::Layout* _uiContentsInfo;
    cocos2d::ui::Layout* _uiContentsTab;
    cocos2d::ui::Layout* _uiContentsList;
    
    cocos2d::ui::CText* _uiPoint;
    cocos2d::ui::CListView* _uiList;
    
};

#endif /* PopupEventSunfishReinforce_hpp */
