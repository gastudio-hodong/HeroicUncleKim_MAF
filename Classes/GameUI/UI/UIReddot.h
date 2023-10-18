//
//  UIReddot.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/09/09.
//

#ifndef UIReddot_h
#define UIReddot_h

#include "Common/ConfigDefault.h"

enum class E_REDDOT_TYPE : char
{
    NOMAL = 0,
    EVENT = 1
};

class UIReddot : public cocos2d::Node
{
public:
    static UIReddot* create(E_REDDOT_TYPE type, cocos2d::Vec2 anchorPoint = cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);
        
    UIReddot(E_REDDOT_TYPE type);
    virtual ~UIReddot();
    virtual bool init(cocos2d::Vec2 anchorPoint);
    
private:
    E_REDDOT_TYPE _eType;
    cocos2d::Sprite *_sprReddot;
    
    
public:
    void hide();
};


#endif /* UIReddot_h */
