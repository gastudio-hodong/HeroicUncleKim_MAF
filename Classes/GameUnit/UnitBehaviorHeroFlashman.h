//
//  Model.h
//  Tens
//
//  Created by SanHeo on 2014. 10. 11..
//
//

#ifndef UnitBehaviorHeroFlashman_h
#define UnitBehaviorHeroFlashman_h

#include "UnitBehavior.h"

class UnitAnimationHero;
class UnitBehaviorMonsterFrost;
class UnitBehaviorHeroFlashman : public UnitBehavior
{
public:
    static UnitBehaviorHeroFlashman* create(E_PLACE ePlace);

    UnitBehaviorHeroFlashman();
    virtual ~UnitBehaviorHeroFlashman();
    virtual bool init() override;
    virtual void onEnter() override;
    
public:
    void setMotion(int skinDummy = 0);
    void setCallbackAttack(const std::function<void(void)>& callbackAttack);
    
    // event
    void onWalk();
    void onAttack();
    
private:
    
private:
    UnitAnimationHero* _unitHero;
    
    std::function<void(void)> _callbackAttack;
    
    bool _isFrostSkillCri;  // 서리감옥 스킬 크리티컬
};

#endif /* UnitBehaviorHeroFlashman_h */
