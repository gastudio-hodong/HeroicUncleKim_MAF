//
//  DragFingerUI.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/07/28.
//

#ifndef DragFingerUI_h
#define DragFingerUI_h

#include "Common/ConfigDefault.h"

class DragFingerUI : public cocos2d::Node
{
public:
    static DragFingerUI* create(E_QUEST quest, int level);
    
    DragFingerUI(E_QUEST quest, int level);
    virtual ~DragFingerUI();
    virtual bool init() override;
    virtual void update(float dt) override;
    
private:
    
    cocos2d::Sprite *_sprFinger;
    
    E_QUEST _quest;
    int _level;
    float _fTimer;
    float _maxTimer;
    void hide();
};


#endif /* DragFingerUI_h */
