//
//  Model.h
//  Tens
//
//  Created by SanHeo on 2014. 10. 11..
//
//

#ifndef UnitAnimationHero_h
#define UnitAnimationHero_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class UnitAnimationHero : public cocos2d::LayerColor
{
public:
    enum E_TYPE {
        DEFAULT = 0,
        SKIN,
        HERO,
    };
    
    enum E_PART{
        BODY = 0,
        HEAD,
        HAND,
        FOOT,
        CAPE,
        WEAPON,
        SHADOW,
        AURA,
    };
    
public:
    static UnitAnimationHero* create();

    UnitAnimationHero();
    virtual ~UnitAnimationHero();
    virtual bool init() override;
    
public:
    void setMotionCostume(int weapon, int body, int head, int hand, int foot, int cape);
    void setMotionSkin(int skin);
    void setMotionHero();
    
    void setDelayUnitWalk(double delayUnit);
    void setDelayUnitAttack(double delayUnit);
    void setAura(bool bAura, cocos2d::Color3B color);

    // animation
    void animationWalk();
    void animationAttack(bool bLoop);
    
private:
    cocos2d::Animate* getMotionAnimate(std::string path, double delayPerUnit);
    
private:
    E_TYPE _eType;
    
    double _delayUnitWalk;
    double _delayUnitAttack;
    
    bool _bAura;
    cocos2d::Color3B _colorAura;

    
    //
    cocos2d::Map<E_PART, cocos2d::Animate*> _listAnimateMove;
    cocos2d::Map<E_PART, cocos2d::Animate*> _listAnimateAttack;
    cocos2d::Animate* _animateAura;
    
    
};

#endif /* UnitAnimationHero_h */
