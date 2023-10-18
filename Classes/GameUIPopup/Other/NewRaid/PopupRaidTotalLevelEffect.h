//
//  PopupRaidTotalLevelEffect.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/11.
//

#ifndef PopupRaidTotalLevelEffect_hpp
#define PopupRaidTotalLevelEffect_hpp

#include "GameUIPopup/Base/PopupBase.h"

class PopupRaidTotalLevelEffect : public PopupBase
{
public:
    static PopupRaidTotalLevelEffect* create();
    
    PopupRaidTotalLevelEffect(void);
    virtual ~PopupRaidTotalLevelEffect(void);
    
    virtual bool init() override;
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiTop();
    void uiTotalLevel();
    void uiEffect();
    
    // click
private:
    cocos2d::Layer* _lyTop;
    cocos2d::Layer* _lyTotalLevel;
    cocos2d::Layer* _lyEffect;
};

#endif /* PopupRaidTotalLevelEffect_hpp */
