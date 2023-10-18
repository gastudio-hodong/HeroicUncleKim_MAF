//
//  DFHPBar.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/17.
//

#ifndef DFHPBar_hpp
#define DFHPBar_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "GameUnit/Defense/DFObject.h"

class DFMonster;
class DFHPBar : public DFObject
{
public:
    static DFHPBar* create();
    
    DFHPBar(void);
    virtual ~DFHPBar(void);
    bool init(void) override;
public:
    void activateObject() override;
    void deactivateObject() override;
    
    void setOwnerMonster(DFMonster* owner);
    void updateHpBar();
    void updatePercentage();
    void movement(float dt);
protected:
    //update
    
    //
    
public://override
    
protected:
    DFMonster* _ownerMonster;
    Sprite* _hpBG;
    Sprite* _icon;
    cocos2d::ProgressTimer* _hpBar;
};

#endif /* DFHPBar_hpp */
