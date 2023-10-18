//
//  Pet.h
//  FantasyClicker
//
//  Created by SanHeo on 2015. 5. 14..
//
//

#ifndef Pet_h
#define Pet_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class UnitBehaviorHero;
class UnitBehaviorMonster;
class Pet : public cocos2d::Sprite
{
protected:
    Pet();
    virtual ~Pet();
    int _nZorder;

public:
    E_PET _ePet;
    E_HERO_STATE _eState;
    E_PLACE _ePlace;
    double _coolTime;
    double _coolTimeMax;
    
    cocos2d::Animate *_animateIdle;
    cocos2d::Animate *_animateMove;
    cocos2d::Animate *_animateAttack;

    cocos2d::Sprite* _sprRing;
    UnitBehaviorHero* _hero;
    UnitBehaviorMonster* _enemy;
    bool _bNoIdle;
    bool _bSkinUse;

    virtual void update(float dt);
    
    void idle();
    virtual void move();
    virtual void attack(UnitBehaviorMonster& mon);
    
    void changeState(E_HERO_STATE state);
    
    void setHero(UnitBehaviorHero *hero);
    void setCoolTime(double time,bool bReset = false);
    
    
    virtual void setPlace(E_PLACE ePlace);
    virtual void setSkinUse(bool bSkinUse);
    
    virtual void setEvolution() = 0;
    virtual void setEvolutionDummy(int nIdx = 0) = 0;
    void showRing();
    
    void hideRing();
    
    void setZorderNum(int value);
    int getZorderNum();

};

class Chick : public Pet
{
    Chick();
    virtual ~Chick();
public:
    static Chick* create();
    
    virtual void update(float dt);
//    std::string getAttackDamage(std::string &atk);
    
    cocos2d::Animate *_aniEvolutionSKill;
    cocos2d::Sprite *_sprEvolutionSkill;
    virtual void setEvolution();
    virtual void setEvolutionDummy(int nIdx = 0);
    
    int _attackCount;
};

class Hamster : public Pet
{
    Hamster();
    virtual ~Hamster();
public:
    static Hamster* create();
    
    virtual void update(float dt);
    virtual void setEvolution();
    virtual void setEvolutionDummy(int nIdx = 0);
    
    static int getAbilityMagnification();
};

class Dragon : public Pet
{
    Dragon();
    virtual ~Dragon();
public:
    static Dragon* create();
    virtual void attack(UnitBehaviorMonster& mon);
    virtual void setEvolution();
    virtual void setEvolutionDummy(int nIdx = 0);
};

class Princess : public Pet
{
    Princess();
    virtual ~Princess();
public:
    static Princess* create();
    
    int _nBuffAllCount;
    int _nAttackCount;
    std::function<void(E_PRINCESS_BUFF,float time)> _onBuffEffect;
    
    virtual void update(float dt);
    virtual void setPlace(E_PLACE ePlace);
    virtual void setEvolution();
    virtual void setEvolutionDummy(int nIdx = 0);
};

//puriring
class Puriring : public Pet
{
    Puriring();
    virtual ~Puriring();
public:
    static Puriring* create();
    void getKey();
    virtual void setEvolution();
    virtual void setEvolutionDummy(int nIdx = 0);
};


class Hong : public Pet
{
    Hong();
    virtual ~Hong();
public:
    static Hong* create();
    
    virtual void update(float dt);
    
    virtual void setEvolution();
    virtual void setEvolutionDummy(int nIdx = 0);
};

class Mole : public Pet
{
    Mole();
    virtual ~Mole();
public:
    static Mole* create();
    
    virtual void setEvolution();
    virtual void setEvolutionDummy(int nIdx = 0);
    void attackMine();
};

class Handsome : public Pet
{
    Handsome();
    virtual ~Handsome();
public:
    static Handsome* create();
    
    virtual void attack(UnitBehaviorMonster& mon);
    virtual void setEvolution();
    virtual void setEvolutionDummy(int nIdx = 0);
};



#endif /* defined(Pet_h) */
