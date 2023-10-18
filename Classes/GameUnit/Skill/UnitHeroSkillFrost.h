//
//  Model.h
//  Tens
//
//  Created by SanHeo on 2014. 10. 11..
//
//

#ifndef UnitHeroSkillFrost_h
#define UnitHeroSkillFrost_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class IBattleInfo;
class UnitHeroSkillFrost : public cocos2d::LayerColor
{
public:
    static UnitHeroSkillFrost* create();
    
    UnitHeroSkillFrost();
    virtual ~UnitHeroSkillFrost();
    virtual bool init() override;

    // update
    void update(float dt) override;
    
public:
    void setBattleInfo(IBattleInfo* battle);
    
private:
    cocos2d::Animate* getSkillAnimate(std::string path, double delayPerUnit);
    
    void skill50004();
    void skill50005();
    void skill50006();
    void skillFinish();
    
private:
    IBattleInfo* _battle;
    
    bool _skillCasting;
    int _skillIdx;

    //
    cocos2d::Animate* _animatePartDefaultAttack;
    cocos2d::Sprite* _spritePartDefault;
};

#endif /* UnitHeroSkillFrost_h */
