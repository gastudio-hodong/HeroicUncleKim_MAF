//
//  LayerEventVoteMain.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/28.
//

#ifndef LayerEventVoteMain_hpp
#define LayerEventVoteMain_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/Observer/IRefresh.h"

#include "GameObject/EventVote/InfoEventVoteCandidate.h"

class LayerEventVoteMain : public cocos2d::ui::Layout
{
public:
    static LayerEventVoteMain* create(cocos2d::Size size);

    LayerEventVoteMain(void);
    virtual ~LayerEventVoteMain(void);
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
    void uiVote();
    void uiCandidate();
    
    // ui : draw
    void drawVote();
    
    // ui : set, get
    
    // ui : click
    void onClickVote(Ref* sender);
    
    // callback
    void callbackInfo(const bool bResult, const int nResult);
    
    // schedule
    
    //

private:
    bool _bInitUI;
    
    bool _bResult;
    // unit, map
    cocos2d::Vector<cocos2d::Sprite*> _listMap;
    
    // widget
    cocos2d::ui::Layout* _uiContentsVote;
};


#endif /* LayerEventVoteMain_hpp */
