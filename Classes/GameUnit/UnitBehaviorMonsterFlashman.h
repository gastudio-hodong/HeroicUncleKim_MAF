//
//  Model.h
//  Tens
//
//  Created by SanHeo on 2014. 10. 11..
//
//

#ifndef UnitBehaviorMonsterFlashman_h
#define UnitBehaviorMonsterFlashman_h

#include "UnitBehavior.h"

#include "GameUIPopup/Other/Flashman/EnumFlashman.h"

class UnitBehaviorMonsterFlashman : public cocos2d::Layer
{
public:
    enum class E_STATE{
        NONE = 0,
        
        IDLE,
    };
    
public:
    static UnitBehaviorMonsterFlashman* create();

    UnitBehaviorMonsterFlashman();
    virtual ~UnitBehaviorMonsterFlashman();
    virtual bool init() override;
    virtual void onEnter() override;
    
public:
    
    // set, get
    E_STATE getState();
    E_FLASHMAN_TYPE getType();
    
    void setMotion(E_FLASHMAN_TYPE eType, bool bBoss);
    
    int getHP();
    void setHP(int hp);
    
    // event
    void onIdle();
    
    //
    
protected:
    void drawHP();
    
    
private:
    bool _bInitUI;
    
    //
    E_STATE _eState;
    E_FLASHMAN_TYPE _eType;
    
    bool _bBoss;
    int _hp;

    //
    cocos2d::Sprite* _uiAnimationMonster;
    cocos2d::Animate* _uiAnimateIdle;
    
    cocos2d::ui::ImageView* _uiHPBG;
};

#endif /* UnitBehaviorMonsterFlashman_h */
