//
//  Model.h
//  Tens
//
//  Created by SanHeo on 2014. 10. 11..
//
//

#ifndef UnitAnimationMonsterFrost_h
#define UnitAnimationMonsterFrost_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class UnitAnimationMonsterFrost : public cocos2d::LayerColor
{
public:
    enum E_TYPE {
        DEFAULT,
    };
    
    enum E_PART{
        BODY = 0,
    };
    
public:
    static UnitAnimationMonsterFrost* create();

    UnitAnimationMonsterFrost();
    virtual ~UnitAnimationMonsterFrost();
    virtual bool init() override;
    
public:
    void setMotion();
    
    // animation
    void animationIdle();
    void animationAngry(cocos2d::CallFunc *callback);
    void animationAngryWalk();

    
private:
    cocos2d::Animate* getMotionAnimate(std::string path, double delayPerUnit);
    
private:
    E_TYPE _eType;
    
    //
    cocos2d::Map<E_PART, cocos2d::Animate*> _listAnimateIdle;
    cocos2d::Map<E_PART, cocos2d::Animate*> _listAnimateAngry;
    cocos2d::Map<E_PART, cocos2d::Animate*> _listAnimateAngryMove;
    
    
};

#endif /* UnitAnimationMonsterFrost_h */
