//
//  LayerEventDrawLimitAni.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef LayerEventDrawLimitAni_h
#define LayerEventDrawLimitAni_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class LayerEventDrawLimitAni : public cocos2d::ui::Layout
{
public:
    static LayerEventDrawLimitAni* create(cocos2d::Size size);

    LayerEventDrawLimitAni(void);
    virtual ~LayerEventDrawLimitAni(void);
    virtual bool init(cocos2d::Size size);
    virtual void onEnter() override;
    
public:
    void setCallbackFinish(const std::function<void(void)>& callback);
    
    void onDrawStart();
    void onResult();
    void onHide();
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui : defalut
    void uiContainer();
    void uiDefault();
    void uiButtons();
    
    // ui : draw
    void drawDefault();
    void drawButtons();
    
    // ui : action
    void actionBall();
    
    // ui : click
    void onClickSkip(cocos2d::Ref* sender);
    
    // ui
    
    // callback
    
    // schedule
    
    //

private:
    bool _bInitUI;
    
    //
    int _machineBallNow;
    int _machineBallMax;
    
    std::vector<int> _listDraw;
    
    // callback
    std::function<void(void)> _callbackFinish;
    
    // widget
    cocos2d::ui::Layout* _uiContentsDefault;
    cocos2d::ui::Layout* _uiContentsButtons;
    
    cocos2d::ui::ImageView* _uiMachineBG;
    cocos2d::ui::ImageView* _uiMachine;
};
#endif /* LayerEventDrawLimitAni_h */
