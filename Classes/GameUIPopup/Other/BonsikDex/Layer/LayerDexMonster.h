//
//  LayerDexMonster.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef LayerDexMonster_h
#define LayerDexMonster_h

#include "Common/ConfigDefault.h"

class LayerDexMonster : public cocos2d::ui::Layout, public cocos2d::ui::DelegateListView
{
public:
    CREATE_FUNC(LayerDexMonster);

    LayerDexMonster(void);
    virtual ~LayerDexMonster(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
    // listview
    virtual void listviewScrollView(cocos2d::ui::CListView* listview, cocos2d::ui::ScrollView::EventType type) override;
    virtual cocos2d::Size listviewCellSize(cocos2d::ui::CListView* listview, ssize_t idx) override;
    virtual void listviewCell(cocos2d::ui::CListView *listview, cocos2d::ui::Widget* childview, ssize_t idx) override;
    virtual int64_t listviewNumberOfCells(cocos2d::ui::CListView *listview) override;

public:
    // set, get
    int getFloorIdx(std::string strFloor);
    void setFloor();
    
    std::string getMonsterToFloor(int idx);
    void setMonster();
    
    int getEffectValue(int level);
    
    // event

    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiInfo();
    void uiList();
    void uiEffect();
    
    // draw 
    void drawInfo();
    void drawList();
    void drawEffect();
    
    // click
    void onClickHelp(cocos2d::Ref* sender);
    void onClickMonster(cocos2d::Ref* sender);
    void onClickLevelUp(cocos2d::Ref* sender);
    void onClickSoulChange(cocos2d::Ref* sender);
    
    // callback
    void callbackFloorSelect(std::string strFloor);
    void callbackSoulChange();
     
    
private:
    bool _bInitUI;
    
    //
    int _nFloorSelectIdx;
    int _nMonsterSelectIdx;
    
    std::vector<std::string> _listFloor;
    std::vector<int> _listMonster;
    
    //
    cocos2d::ui::Layout* _uiContentsInfo;
    cocos2d::ui::Layout* _uiContentsList;
    cocos2d::ui::Layout* _uiContentsEffect;
    
    cocos2d::ui::CListView* _uiList;
    
};
#endif /* LayerDexMonster_h */
