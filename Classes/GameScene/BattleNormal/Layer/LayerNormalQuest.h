//
//  LayerNormalQuest.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef LayerNormalQuest_h
#define LayerNormalQuest_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/Observer/IRefresh.h"

class LayerNormalQuest : public cocos2d::ui::Layout, public IRefresh
{
public:
    static LayerNormalQuest* create();

    LayerNormalQuest(void);
    virtual ~LayerNormalQuest(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;

public:
    void onListMove();
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui : defalut
    void uiContainer();
    void uiInfo();
    
    // ui : draw
    void drawInfo();
    
    // ui : set, get
    
    // ui : click
    void onClickLevelAdd(cocos2d::Ref* sender);
    
    
    // callback
    
    // schedule
    void scheduleGoldPS(float dt);

private:
    bool _bInitUI;
    
    E_PLACE _ePlace;
    
    //
    int _levelAdd;
    std::string _goldps;
    
    // widget
    cocos2d::ui::Layout* _uiContainerInfo;
    cocos2d::ui::Layout* _uiContainerList;

    cocos2d::ui::CText* _uiGoldPS;
};
#endif /* LayerNormalQuest_h */
