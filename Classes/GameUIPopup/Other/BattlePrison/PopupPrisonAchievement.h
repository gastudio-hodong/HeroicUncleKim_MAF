//
//  PopupPrisonAchievement.h
//  FantasyClicker-mobile
//
//  Created by TK on 2020/06/03.
//

#ifndef PopupPrisonAchievement_h
#define PopupPrisonAchievement_h

#include "GameUIPopup/Base/PopupBase.h"

class ObjAchievement;
class PopupPrisonAchievement : public PopupBase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static PopupPrisonAchievement* create();
    
    PopupPrisonAchievement(void);
    virtual ~PopupPrisonAchievement(void);
    virtual bool init() override;
    
    // table TableViewDelegate
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) override;
    
    // table TableViewDataSource
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;
    
    
public:
    // set
    void setCallbackReward(const std::function<void(void)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiProgress();
    void uiList();
    
    // draw
    void drawProgress();
    void drawList();
    
    //
    bool setSortList(cocos2d::Ref *a, cocos2d::Ref *b);
    
    // callback
    void callbackReward(bool bResult, int nResult);
    void callbackComplete(bool bResult, int nResult);
    
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickReward(cocos2d::Ref* sender);
    void onClickComplete(cocos2d::Ref* sender);
    
private:
    cocos2d::Vector<ObjAchievement*> _listAchievement;
    std::function<void(void)> _onCallbackReward;
    
    // widget
    cocos2d::Layer* _layerContainerProgress;
    cocos2d::Layer* _layerContainerList;
    
    cocos2d::extension::TableView *_table;
};


#endif /* PopupPrisonAchievement_h */
