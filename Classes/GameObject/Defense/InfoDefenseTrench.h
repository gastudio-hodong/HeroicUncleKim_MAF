//
//  InfoDefenseTrench.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/31.
//

#ifndef InfoDefenseTrench_hpp
#define InfoDefenseTrench_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoDefenseWeapon;
class InfoDefenseCharacter;

class InfoDefenseTrench : public cocos2d::Ref
{
public:
    static InfoDefenseTrench* create();
    
    InfoDefenseTrench();
    virtual ~InfoDefenseTrench();
    virtual bool init();
    
public:
    int getNumber() const;
    void setNumber(const int value);
    int getWeaponIdx() const;
    void setWeaponIdx(const int value);
    int getCharacterIdx() const;
    void setCharacterIdx(const int value);
    
    InfoDefenseWeapon* getInfoDefenseWeapon() const;
    InfoDefenseCharacter* getInfoDefenseCharacter() const;
private:
    int _number;
    int _characterIdx;
    int _weaponIdx;
};
#endif /* InfoDefenseTrench_hpp */
