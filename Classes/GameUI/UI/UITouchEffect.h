//
//  UITouchEffect.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef UITouchEffect_h
#define UITouchEffect_h

#include "Common/ConfigDefault.h"

class UITouchEffect : public cocos2d::LayerColor
{
public:
    static UITouchEffect* create();
    
    UITouchEffect(void);
    virtual ~UITouchEffect(void);
    virtual bool init();
    
protected:
    // init
    void initVar();
    void initUi();

private:
    
    
    // widget
    cocos2d::Layer* _layerContainerContent;
    
    
};

#endif /* UITouchEffect_h */
