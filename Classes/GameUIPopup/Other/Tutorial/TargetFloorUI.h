//
//  TargetFloorUI.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/07/07.
//

#ifndef TargetFloorUI_h
#define TargetFloorUI_h

#include "Common/ConfigDefault.h"

class TargetFloorUI : public cocos2d::Node
{
public:
    static TargetFloorUI* create(int floor);
    
    TargetFloorUI(int floor);
    virtual ~TargetFloorUI();
    virtual bool init() override;
    virtual void update(float dt) override;
    
private:
    
    cocos2d::Sprite *_sprBg;
    cocos2d::Sprite *_sprEffect;
    cocos2d::Label *_label;
    
    int _goalFloor;
    float _fTimer;
    
    void hide();
};


#endif /* TargetFloorUI_h */
