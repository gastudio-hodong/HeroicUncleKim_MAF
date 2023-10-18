//
//  LayerEventLuckyBagCurrentLvInfo.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/12/21.
//

#ifndef LayerEventLuckyBagCurrentLvInfo_hpp
#define LayerEventLuckyBagCurrentLvInfo_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/Observer/IRefresh.h"

#include "GameObject/Event/InfoEventLuckyBagReward.h"

class LayerEventLuckyBagCurrentLvInfo : public cocos2d::ui::Layout
{
public:
    static LayerEventLuckyBagCurrentLvInfo* create(cocos2d::Size size);

    LayerEventLuckyBagCurrentLvInfo(void);
    virtual ~LayerEventLuckyBagCurrentLvInfo(void);
    virtual bool init(cocos2d::Size size);
    virtual void onEnter() override;
    
    virtual void onSizeChanged() override;
public:
    
    void onDrawInfo();
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui : defalut
    void uiContainer();
    void uiReward();
    
    // ui : draw
    void drawReward();
    
    // ui : set, get
    
    // schedule
    
    //

private:
    bool _bInitUI;
    
    bool _bResult;
    // unit, map
    
    // widget
    cocos2d::ui::Layout* _uiContentsReward;
};

#endif /* LayerEventLuckyBagCurrentLvInfo_hpp */
