//
//  IBattleInfo.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 05/06/2019.
//

#ifndef IBattleInfo_h
#define IBattleInfo_h

#include "Common/ConfigDefault.h"

#include "GameUnit/UnitBehaviorHero.h"
#include "GameUnit/UnitBehaviorMonster.h"
#include "GameUnit/Pet.h"
#include "GameUnit/PetDevil.h"

class IBattleInfo
{
public:
    
    virtual UnitBehaviorHero* getUnitHero() = 0;
    virtual UnitBehaviorMonster* getUnitMonster() = 0;
    virtual cocos2d::Vector<Pet*> getUnitPets() = 0;
};

#endif /* IBattleInfo_h */
