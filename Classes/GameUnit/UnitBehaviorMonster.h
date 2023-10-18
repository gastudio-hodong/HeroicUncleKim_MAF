//
//  Model.h
//  Tens
//
//  Created by SanHeo on 2014. 10. 11..
//
//

#ifndef UnitBehaviorMonster_h
#define UnitBehaviorMonster_h

#include "UnitBehavior.h"

class UnitBehaviorMonster : public UnitBehavior
{
public:
    enum E_BOSS{
        NORMAL,
        BOSS,
        BOSS_10,
        BOSS_100,
        BOSS_DRAGON,
        BOSS_HELL,
        BOSS_HEAVEN,
        BOSS_AFTERLIFE1,
        BOSS_AFTERLIFE2,
        BOSS_AFTERLIFE3,
        BOSS_6000,
        BOSS_7000,
        BOSS_8000,
        BOSS_9000,
        PRISON_NORMAL,
        PRISON_TREASURE,
        PRISON_NORMAL_FIRE,
        PRISON_TREASURE_FIRE,
        
        
        DEVIL_BOSS_1,
        DEVIL_BOSS_2,
        DEVIL_BOSS_3,
        DEVIL_BOSS_4,
        DEVIL_BOSS_5,
        DEVIL_BOSS_6,
        DEVIL_BOSS_7,
        DEVIL_BOSS_8,
        DEVIL_BOSS_9,
        DEVIL_BOSS_10
    };
public:
    static UnitBehaviorMonster * create(E_PLACE ePlace);
    
    UnitBehaviorMonster();
    virtual ~UnitBehaviorMonster();
    virtual bool init() override;
    
public:
    // set, get
    bool isDie();
    std::string getMonsterHpMax();
    std::string getMonsterHp();
    
    E_BOSS getMonsterType();
    int getMonsterIdx();
    
    cocos2d::Sprite* getMonsterSprite();
    cocos2d::Sprite* getMonsterHPSprite();
    
    // monster 모양 및 위치
    void setMotion(E_BOSS eBosstype = E_BOSS::NORMAL);
    void setPositionMonster(Node *parent, float height);
    void setMonsterHp(std::string hp,std::string hpMax);
    
    
    // hero action default
    void actionMove();
    void actionHit(E_CHARACTER eCharac, int hitMask, std::string damage, bool critical);
    void actionDie();
    
    bool isBuffRecommend();
    void setBuffRecommend(bool rec);
    
private:
    void initPart();
    
private:
    E_BOSS _eBoss;
    int _nIdx;
    
    bool _bDie;
    bool _bBuffRecommend;
    
    //
    std::string _hp;
    std::string _hpMax;
    
    // widget :캐릭터
    cocos2d::Sprite *_spritePartDefault;
    
    //
    cocos2d::Sprite* _progressHpBG;
    cocos2d::ProgressTimer *_progressHpBar;
    cocos2d::Label* _progressHpLabel;
    
    // damage
    cocos2d::Vector<cocos2d::Label*> _listLabelDamage;
};

#endif /* UnitBehaviorMonster_h */
