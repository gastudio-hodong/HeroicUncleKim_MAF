//
//  LayerDexLoots.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/06/11.
//

#ifndef LayerDexLoots_hpp
#define LayerDexLoots_hpp

#include "Common/ConfigDefault.h"
#include "Common/Observer/IRefresh.h"

#include "GameObject/InfoLoot.h"

class LayerDexLoots : public cocos2d::LayerColor, public cocos2d::ui::DelegateListView, public IRefresh
{    
public:
    static LayerDexLoots* create(cocos2d::Size size);

    LayerDexLoots(void);
    virtual ~LayerDexLoots(void);
    virtual bool init(cocos2d::Size size);
    virtual void onEnter() override;

    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;
    
    // listview
    virtual void listviewScrollView(cocos2d::ui::CListView* listview, cocos2d::ui::ScrollView::EventType type) override;
    virtual cocos2d::Size listviewCellSize(cocos2d::ui::CListView* listview, ssize_t idx) override;
    virtual void listviewCell(cocos2d::ui::CListView *listview, cocos2d::ui::Widget* childview, ssize_t idx) override;
    virtual int64_t listviewNumberOfCells(cocos2d::ui::CListView *listview) override;

public:
    // set, get, add
    void setCallbackInfo(const std::function<void(bool)>& callback);

    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiInfo();
    void uiSynthetic();
    void uiList();
    void uiEffect();
    
    // draw
    void drawInfo();
    void drawSynthetic();
    void drawList();
    void drawEffect();
    
    
    // click
    void onClickLoot(cocos2d::Ref* sender);
    void onClickLootLevelUp(cocos2d::Ref* sender);
    void onClickLootPieceUp(cocos2d::Ref* sender);
    void onClickLootPieceDown(cocos2d::Ref* sender);
    void onClickTrade(cocos2d::Ref* sender);
    void onClickSynthetic(cocos2d::Ref* sender);
    
    //callback
    void callbackInfo(bool bResult);
    void callbackLevelUp(bool bResult, int nResult);
    void callbackSynthetic(bool bResult, int nResult);
    
private:
    bool _bInitUI;
    
    //
    int _selectLootIdx;
    int _selectLootPieceGrade;
    int _selectLootPieceCount;
    
    cocos2d::Vector<InfoLoot*> _listLoots;
    
    std::function<void(bool)> _callbackInfo;
    
    // widget
    cocos2d::ui::Layout* _layerContainerInfo;
    cocos2d::ui::Layout* _layerContainerList;
    cocos2d::ui::Layout* _layerContainerSynthetic;
    cocos2d::ui::Layout* _layerContainerEffect;
    
    // table
    cocos2d::ui::CListView* _uiList;
    
    
};

#endif /* LayerDexLoots_hpp */
