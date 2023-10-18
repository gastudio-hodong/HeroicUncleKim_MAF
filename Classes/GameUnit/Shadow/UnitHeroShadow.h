//
//  Model.h
//  Tens
//
//  Created by SanHeo on 2014. 10. 11..
//
//

#ifndef UnitHeroShadow_h
#define UnitHeroShadow_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class UnitHeroShadow : public cocos2d::Layer
{
public:
    static UnitHeroShadow* create();
    
    UnitHeroShadow();
    virtual ~UnitHeroShadow();
    virtual bool init() override;

public:
    //
    void setShow();
    void setHide();
    
    //
    void actionAttack();
    
private:
    void initParts();
    
    
private:
    bool _bShow;

    //
    cocos2d::Animate* _animatePartDefaultAttack;
    cocos2d::Sprite* _spritePartDefault;
};

#endif /* UnitHeroShadow_h */
