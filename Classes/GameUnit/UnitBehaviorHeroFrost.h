//
//  Model.h
//  Tens
//
//  Created by SanHeo on 2014. 10. 11..
//
//

#ifndef UnitBehaviorHeroFrost_h
#define UnitBehaviorHeroFrost_h

#include "UnitBehavior.h"

class UnitAnimationHero;
class UnitBehaviorMonsterFrost;
class UnitBehaviorHeroFrost : public UnitBehavior
{
public:
    static UnitBehaviorHeroFrost* create(E_PLACE ePlace);

    UnitBehaviorHeroFrost();
    virtual ~UnitBehaviorHeroFrost();
    virtual bool init() override;
    virtual void onEnter() override;
    
public:
    void setMotion();
    void setMonster(UnitBehaviorMonsterFrost* unitMonster);
    
    // event
    void onWalk();
    void onAttack(const std::function<void(std::string)>& callbackDamage);
    
private:
    
    std::string getAttackSkillFrostDamage(std::string &tmpAtk);
    
    
private:
    UnitAnimationHero* _unitHero;
    UnitBehaviorMonsterFrost* _unitMonster;
    
    std::function<void(std::string)> _callbackDamage;
    
    bool _isFrostSkillCri;  // 서리감옥 스킬 크리티컬
};

#endif /* UnitBehaviorHeroFrost_h */
