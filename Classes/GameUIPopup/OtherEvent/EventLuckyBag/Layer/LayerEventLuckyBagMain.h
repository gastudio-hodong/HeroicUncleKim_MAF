//
//  LayerEventLuckyBagMyReward.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/12/21.
//

#ifndef LayerEventLuckyBagMyReward_hpp
#define LayerEventLuckyBagMyReward_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/Observer/IRefresh.h"

class LayerEventLuckyBagMain : public cocos2d::ui::Layout
{
public:
    static LayerEventLuckyBagMain* create(cocos2d::Size size);

    LayerEventLuckyBagMain(void);
    virtual ~LayerEventLuckyBagMain(void);
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
    void uiNotics();
    void uiMyReward();
    void uiButtton();
    
    // ui : draw
    void drawMyReward();
    void drawButtton();
    
    void drawReward(cocos2d::ui::Layout* layout, int nStartIdx, int nLastIdx, cocos2d::Vec2 anchor);
    
    // ui : Click
    void onClickStop(Ref* const sender);
    void onClickGo(Ref* const sender);
    void onClickCount(Ref* const sender);
    
    // schedule
    
    //
    void callbackProgress(const bool bResult, const int nResult);

private:
    bool _bInitUI;
    
    bool _bResult;
    int _nNowDrawRewardLv;
    // unit, map
    cocos2d::Vector<cocos2d::Sprite*> _listMap;
    
    // widget
    cocos2d::ui::Layout* _uiContentsNotics;
    cocos2d::ui::Layout* _uiContentsMyReward;
    cocos2d::ui::Layout* _uiContentsButton;
    
};

#endif /* LayerEventLuckyBagMyReward_hpp */
