//
//  LayerEventDrawLimitResult.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef LayerEventDrawLimitResult_h
#define LayerEventDrawLimitResult_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoEventDraw;
class LayerEventDrawLimitResult : public cocos2d::ui::Layout
{
public:
    static LayerEventDrawLimitResult* create(cocos2d::Size size);

    LayerEventDrawLimitResult(void);
    virtual ~LayerEventDrawLimitResult(void);
    virtual bool init(cocos2d::Size size);
    virtual void onEnter() override;
    
public:
    void setCallbackAgain(const std::function<void(int)>& callback);
    
    void onHide();
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui : defalut
    void uiContainer();
    void uiTitle();
    void uiReward();
    void uiButtons();
    
    // ui : draw
    void drawReward();
    void drawButtons();
    
    // ui : action
    void actionBall(cocos2d::ui::ImageView* uiBall);
    
    // ui : click
    void onClickClose(cocos2d::Ref* sender);
    void onClickReward(cocos2d::Ref* sender);
    void onClickDraw(cocos2d::Ref* sender);
    void onClickSkip(cocos2d::Ref* sender);
    
    // ui
    
    // callback
    
    // schedule
    
    //

private:
    bool _bInitUI;
    
    //
    cocos2d::Vector<InfoEventDraw*> _listEventDraw;
    
    // callback
    std::function<void(int)> _callbackAgain;
    
    // widget
    cocos2d::ui::Layout* _uiContentsTitle;
    cocos2d::ui::Layout* _uiContentsReward;
    cocos2d::ui::Layout* _uiContentsButtons;
    
    cocos2d::Vector<cocos2d::ui::ImageView*> _listBall;
};
#endif /* LayerEventDrawLimitResult_h */
