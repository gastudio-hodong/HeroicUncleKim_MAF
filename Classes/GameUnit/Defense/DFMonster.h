//
//  DefenseMonster.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/06.
//

#ifndef DFMonster_hpp
#define DFMonster_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "GameUnit/Defense/DFObject.h"

class DFHPBar;
class DFMonster : public DFObject
{
public:
    static DFMonster* create();
    
    DFMonster(void);
    virtual ~DFMonster(void);
    bool init(void) override;
    void update(float dt) override;
public:
    void activateObject() override;
    void deactivateObject() override;
    
    void setMonsterType(E_DF_MonsterType type);
    
    int getPathLeftSize();
    bool isDead();
    
    E_DF_MonsterType getMonsterType();
    double getMaxHP();
    double getCurrentHP();
    Sprite* getMonsterSprite();
    
    
    void onHit(double damage, int hitEffectNumber, Vec2 hitLocation, Vec2 hitSize, bool isExplosion = false);
    void deadMonster();
protected:
    //update
    void updateTramcar();
    void updateMonster();
    void updateMonsterStatus();
    
    //
    void movement(float dt);
    void moveCurrentPos();
    
    void playHitEffect(int hitEffectNumber, Vec2 hitLocation, Vec2 hitSize, bool isExplosion);
    
public://override
    void CollisionBegin(IMafColider* Other) override;
    Rect GetBoundingBox() override;
    
protected:
    E_DF_MonsterType _type;
    bool _isDead;
    float _speed;
    double _hp;
    double _hpMax;
    double _defensivePower;
    IntPoint _currentPos;
    std::vector<IntPoint> _path;
    Vec2 _nextLocation;
    
    DFHPBar* _hpBar;
    Sprite* _tramcar;
    Sprite* _tramcarFront;
    Sprite* _monster;
    Sprite* _deadEffect;
};

#endif /* DFMonster */
