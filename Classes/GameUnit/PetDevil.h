//
//  Pet.h
//  FantasyClicker
//
//  Created by SanHeo on 2015. 5. 14..
//
//

#ifndef PetDevil_h
#define PetDevil_h

#include "GameUnit/Pet.h"

class Penguin : public Pet
{
    Penguin();
    virtual ~Penguin();
    virtual void setEvolution(){};
    virtual void setEvolutionDummy(int nIdx = 0){};
public:
    static Penguin* create();
    
    virtual void update(float dt);
    int _nAttackCount;
};

class Hedgehog : public Pet
{
    Hedgehog();
    virtual ~Hedgehog();
    virtual void setEvolution(){};
    virtual void setEvolutionDummy(int nIdx = 0){};
public:
    static Hedgehog* create();
    virtual void update(float dt);
    virtual void move();
    
    int _nAttackCount;
    cocos2d::MoveTo *_move;
    cocos2d::JumpTo *_moveBack;
};

class Gatekeeper : public Pet
{
    Gatekeeper();
    virtual ~Gatekeeper();
    
public:
    static Gatekeeper* create();
    
    virtual void update(float dt);
    virtual void setEvolution(){};
    virtual void setEvolutionDummy(int nIdx = 0){};
    
    int _nAttackCount;
    std::function<void(E_GATEKEEPER_BUFF,float time)> _onBuffEffect;
};

class Flash : public Pet
{
    Flash();
    virtual ~Flash();
    
public:
    static Flash* create();
    
    virtual void attack(UnitBehaviorMonster& mon);
    virtual void setEvolution(){};
    virtual void setEvolutionDummy(int nIdx = 0){};
    
    cocos2d::Animate *_aniAttackEffect;
    cocos2d::Sprite *_sprAttackEffect;
};


#endif /* defined(PetDevil_h) */
