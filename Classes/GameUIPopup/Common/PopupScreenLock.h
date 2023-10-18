//
//  PopupScreenLock.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 6. 4..
//

#ifndef PopupScreenLock_h
#define PopupScreenLock_h

#include "Common/ConfigDefault.h"

class PopupScreenLock : public cocos2d::ui::Layout
{
public:
    enum eTag {
        tag_sprite_arrow = 10,
        tag_sprite_arrowtext = 11,
    };
    
public:
    CREATE_FUNC(PopupScreenLock);
    
    PopupScreenLock(void);
    virtual ~PopupScreenLock(void);
    virtual bool init(void) override;
    
    // override : touch
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) override;
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;
    virtual void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event) override;
    
public:
    static void show();
    static void hide();
    static bool isShow();
    
protected:
    // ui
    void uiContents();
    
    void uiDrawTime(float dt = 0);     //
    void uiDrawFloor(float dt = 0);     //
    void uiDrawAnimation(float dt = 0); //
    
    // set, get
    
private:
    cocos2d::Vec2 _posTouch;
    
    int _nAnimationType;
    
    
    //
    cocos2d::Layer* _layerContainer;
    
    cocos2d::Label* _labelTime;
    cocos2d::Label* _labelTimeFormat;
    cocos2d::Label* _labelFloor;
    cocos2d::Sprite* _spriteBonsik;
    
    cocos2d::Animate *_animateBonsikAttack;
    cocos2d::Animate *_animateBonsikWalk;
    
};

#endif /* PopupScreenLock_h */
