//
//  UIAdsCooldown.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/04/01.
//

#ifndef UIAdsCooldown_hpp
#define UIAdsCooldown_hpp

#include "Common/ConfigDefault.h"

class UIAdsCooldown : public cocos2d::Node
{
public:
    static UIAdsCooldown* create();
    
    UIAdsCooldown();
    virtual ~UIAdsCooldown();
    virtual bool init() override;
    virtual void update(float dt) override;
    void hide();
    void show();
    
    bool isActiveUI();
    
private:
    
    bool _isUIActive;
    float _fTimer;
    cocos2d::Label* _lbMsg;
};

#endif /* UIAdsCooldown_hpp */
