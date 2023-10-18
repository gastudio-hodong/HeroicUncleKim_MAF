//
//  BaseGameScene.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 04/06/2019.
//

#ifndef BaseGameScene_h
#define BaseGameScene_h

#include "GameScene/Base/BaseScene.h"

class BaseGameScene : public BaseScene
{
public:
    BaseGameScene();
    virtual ~BaseGameScene();
    
    // override base
    virtual bool init() override;
    virtual void onEnter() override;
        
    // override
    virtual bool onKeyRelease(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    virtual void onKeyReleaseNew(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    
    
public:
    bool isPopupShow();
    void hidePopupAll();
    
    // schedule
    void scheduleTouch(float dt);

protected:
    

private:
    double _nTouchTime;
    
};

#endif /* BaseGameScene_h */
