//
//  Model.h
//  Tens
//
//  Created by SanHeo on 2014. 10. 11..
//
//

#ifndef UnitBehaviorHeroAnother_h
#define UnitBehaviorHeroAnother_h

#include "UnitBehavior.h"

class UnitBehaviorHeroAnother : public UnitBehavior
{
public:
    enum E_HERO{
        BODY = 0,
        HEAD,
        HAND,
        FOOT,
        CAPE,
        WEAPON,
    };
    
public:
    static UnitBehaviorHeroAnother* create(E_PLACE ePlace);

    UnitBehaviorHeroAnother();
    virtual ~UnitBehaviorHeroAnother();
    virtual bool init() override;
    
public:
    void setName(const char* name, bool up);
    
    
    // character 모양 및 위치
    void setMotion(int weapon, int p1,int p2,int p3,int p4, int p5);
    void setHeroFlipX(bool value);
    void setImage(int weapon, int p1,int p2,int p3,int p4, int p5);
    void setSkin(int skinIdx);
    
    // hero action default
    void actionMove();
    void actionAttack(bool isAutoMove = false);
    
    
    cocos2d::Animate* getMotionDefault(std::string strPath, double delay);
    
private:
    void initParts();
    
private:
    bool _isAttack;
    float _time;
    bool _isSkin;
    cocos2d::Label* _labName;

    //move
    cocos2d::Sequence *_seqMoveWeapon;
    cocos2d::Sequence *_seqMoveBody;
    cocos2d::Sequence *_seqMoveHead;
    cocos2d::Sequence *_seqMoveHand;
    cocos2d::Sequence *_seqMoveFoot;
    cocos2d::Sequence *_seqMoveCape;
    cocos2d::Sprite *_spritePartDefault[6];
    cocos2d::Sprite* _spritePartSkin;
    cocos2d::Animate* _animatePartSkinMove;
    
    //attack
    cocos2d::Sequence *_seqAttackWeapon;
    cocos2d::Sequence *_seqAttackBody;
    cocos2d::Sequence *_seqAttackHead;
    cocos2d::Sequence *_seqAttackHand;
    cocos2d::Sequence *_seqAttackFoot;
    cocos2d::Sequence *_seqAttackCape;
    cocos2d::Animate* _animatePartSkinAttack;
    
    cocos2d::Vector<cocos2d::Sequence*> _vecSequence;
};

#endif /* UnitBehaviorHeroAnother_h */
