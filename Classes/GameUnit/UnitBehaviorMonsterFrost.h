//
//  Model.h
//  Tens
//
//  Created by SanHeo on 2014. 10. 11..
//
//

#ifndef UnitBehaviorMonsterFrost_h
#define UnitBehaviorMonsterFrost_h

#include "UnitBehavior.h"

class UnitAnimationMonsterFrost;
class UnitBehaviorMonsterFrost : public UnitBehavior
{
public:
    enum class E_STATE{
        NONE = 0,
        
        IDLE,
        ANGRY,
        WALK,
    };
    
public:
    static UnitBehaviorMonsterFrost* create(E_PLACE ePlace);

    UnitBehaviorMonsterFrost();
    virtual ~UnitBehaviorMonsterFrost();
    virtual bool init() override;
    virtual void onEnter() override;
    
public:
    E_STATE getState();
    
    void setMotion();
    
    // event
    void onIdle();
    void onAngry(const std::function<void(void)>& callbackFinish);
    void onWalk();
    void onDamage(std::string damage, bool critical, int hitMask);
    
    //
    void animationAttackEffect();
    
private:
    
    
private:
    E_STATE _eState;

    UnitAnimationMonsterFrost* _unitMonster;
    
    //
    cocos2d::Map<int, cocos2d::Animate*> _listAnimateAttackEffectFront;
    cocos2d::Map<int, cocos2d::Animate*> _listAnimateAttackEffectBack;
    cocos2d::Map<int, cocos2d::Sprite*> _listSpriteAttackEffectFront;
    cocos2d::Map<int, cocos2d::Sprite*> _listSpriteAttackEffectBack;
};

#endif /* UnitBehaviorMonsterFrost_h */
