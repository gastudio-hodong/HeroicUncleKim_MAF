//
//  Model.h
//  Tens
//
//  Created by SanHeo on 2014. 10. 11..
//
//

#ifndef UnitBehaviorHero_h
#define UnitBehaviorHero_h

#include "UnitBehavior.h"

class UnitHeroShadow;
class UnitBehaviorMonster;
class UnitBehaviorHero : public UnitBehavior
{
public :
    enum E_HERO{
        SHADOW2 = 0,        // 배경 그림자
        SHADOW1,            // 바닥 그림자
        RIDER,
        AURA,
        BODY,
        HEAD,
        HAND,
        FOOT,
        CAPE,
        WEAPON,
    };
    
    enum E_HERO_SKIN{
        DEFAULT = 0,
        DEFAULT_SUMMER,
        PREMIUM,
        HERO,
    };
public:
    static UnitBehaviorHero* create(E_PLACE ePlace);
    
    UnitBehaviorHero();
    virtual ~UnitBehaviorHero();
    virtual bool init() override;
    
    virtual void update(float dt) override;
    
public:
    // set, get
    void setAttackCallBackListener(cocos2d::CallFunc *call);
    void setMoveCallBackListener(cocos2d::CallFunc *call);
    void setUpstairsCallBackListener(cocos2d::CallFunc *call);
    void setAttackCountCallBackListener(cocos2d::CallFunc *call);
    
    void setShowSkinAttackEffect(bool active);
    void setShowWeaponAttackEffect(bool active);
    
    void setTalk(std::string talk);
    void setTalkAuto();
    void setTalkShow(bool value);
    
    void setSpiritBuff(bool value);
    
    void setShowRing();
    void setHideRing();
    
    void setSkinUse(bool bSkinUse);
    
    // character 모양 및 위치
    void setChangeParts();
    void setMotion(int nSkinDummy = 0, int nWeaponDummy = 0);
    void setMotionDevil(const bool isNotRiderMotion = false);
    void setPositionHero(Node *parent, float height);
    
    // pet buff
    void onPrincessBuff(E_PRINCESS_BUFF type,float time);
    void onGatekeeperBuff(E_GATEKEEPER_BUFF type,float time);
    void updatePrincessBuff(float dt);
    void updateGatekeeperBuff(float dt);
    double getPrincessBuffTime();
    double getGatekeeperBuffTime();
    
    // hero action default
    void actionMoveUpstairs();
    void actionMove();
    void actionMoveDungeon(bool first);
    
    void actionAttack(UnitBehaviorMonster &mon);
    
    void actionRun();
    
    void heroStopAllAction();
    
    //2차 크리티컬
    std::string getAttackDeathBlowDamage(std::string &tmpAtk);
    float getDeathBlowCoolDown();
    bool isDeathBlowActive();
    void actionDeathBlowActive();
    
    std::string getAttackSkillFrostDamage(std::string &tmpAtk);
    
    void showItemFeverFx();
    void hideItemFeverFx();
    
    // 재판장의 망치
    void actionAttackGavel();
    
private:
    void initPart();
    void initPartDevil();
    void initShadow();
    
    // schedule
    void scheduleTalk(float dt);
    
    // set, get
    void setPositionDevilHeroChange(bool bMove);//마왕성에서만 사용
    cocos2d::Animate* getMotionDefault(std::string strPath, double delay);
    
    // hero action default
    void actionAttackContinue();
    void actionWeaponEffect(float x);
    void actionSkinEffect();
    
private:
    E_HERO_STATE _eState;
    E_HERO_SKIN _eSkin;
    int _nTalkCount;    // 대화 카운트
    bool _bChangeParts; // 외형 변경여부
    bool _bOnekill;     // 원킬 여부
    bool _bRider;       // 마왕성 라이더 출력 여부
    bool _bDeathBlow;   // 실제 2차 크리티컬 여부
    bool _isDeathBlow;  // 데미지 표기를 위한
    bool _isDeathBlowActive;  //2차 크리티컬 오픈 여부
    
    bool _isFrostSkillCri;  // 서리감옥 스킬 크리티컬 
    
    float _nTimePrincessBuff;
    float _nTimeGatekeeperBuff;
    float _nDeathBlowTime;
    
    bool _bSkinUse;
    
    UnitBehaviorMonster *_enemy;
    UnitHeroShadow *_shadow;
    
    //
    cocos2d::CallFunc *_onAttackCallback;
    cocos2d::CallFunc *_onMoveCallback;
    cocos2d::CallFunc *_onUpstairsCallback;
    cocos2d::CallFunc *_onAttackCountCallback;
    
    //
    cocos2d::Sprite *_spriteTalk;
    cocos2d::Sprite *_spriteRing;
    cocos2d::Sprite *_spriteItemFeverFx;
    cocos2d::Sprite *_spriteSpiritBuffFx;
    cocos2d::ParticleSystemQuad* _fxSpiritBuff;
    
    // 버프 이펙트
    cocos2d::Animate *_animateBuffAttack;
    cocos2d::Animate *_animateBuffSpeed;
    cocos2d::Animate *_animateBuffCritical;
    cocos2d::Animate *_animateBuffAttackSpeed;
    cocos2d::Animate *_animateBuffAll;
    cocos2d::Animate *_animateBuffBack;
    cocos2d::Sprite *_spriteBuff;
    cocos2d::Sprite *_spriteBuffBack;
    
    // 캐릭터
    cocos2d::Animate* _animatePartDefaultMove[10];
    cocos2d::Animate* _animatePartDefaultAttack[10];
    cocos2d::Sprite* _spritePartDefault[10];
    
    cocos2d::Animate* _animatePartSkinMove;
    cocos2d::Animate* _animatePartSkinAttack;
    cocos2d::Animate* _animatePartSkinMoveEffect;
    cocos2d::Animate* _animatePartSkinAttackEffect;
    cocos2d::Sprite* _spritePartSkin;
    cocos2d::Sprite* _spritePartSkinMoveEffect;
    cocos2d::Sprite* _spritePartSkinAttackEffect;
    cocos2d::Sprite* _spriteFrostSkillShape;
    
    cocos2d::Vector<cocos2d::Animate*> _vecSequence;
    
    cocos2d::Layer* _lySpiritBuff;
    
private:
    struct tagAtkEffectDefaultInfo
    {
        cocos2d::Color3B color;
        tagAtkEffectDefaultInfo(cocos2d::Color3B color):
        color(color){}
    };
    
    struct tagAtkEffectAnimtInfo
    {
        cocos2d::Animate* fAnim;
        cocos2d::Animate* bAnim;
        
        cocos2d::Sprite* sprFrontDefault;
        cocos2d::Sprite* sprBackDefault;
        
        tagAtkEffectAnimtInfo(cocos2d::Animate* fAnim, cocos2d::Animate* bAnim, cocos2d::Sprite* sprFDefault, cocos2d::Sprite* sprBDefault):
        fAnim(fAnim),
        bAnim(bAnim),
        sprFrontDefault(sprFDefault),
        sprBackDefault(sprBDefault)
        {}
    };
    std::vector<tagAtkEffectDefaultInfo> _vecAtkEffectInfo;
    
    std::vector<tagAtkEffectAnimtInfo> _vecAtkEffectAnimInfo;
    
    bool _bShowSkinAttackEffect;  // 스킨 이펙트 출력 여부
    bool _bShowWeaponAttackEffect;  // 스킨 이펙트 출력 여부
};
     

#endif /* UnitBehaviorHero_h */
