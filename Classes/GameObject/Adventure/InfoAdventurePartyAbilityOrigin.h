//
//  InfoAdventurePartyAbility.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/14.
//

#ifndef InfoAdventurePartyAbility_hpp
#define InfoAdventurePartyAbility_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoAdventureMember;
class InfoAdventurePropertyType;
class InfoAdventurePartyAbilityEffect : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoAdventurePartyAbilityEffect);
    InfoAdventurePartyAbilityEffect();
    virtual ~InfoAdventurePartyAbilityEffect();
    virtual bool init();
    
public:
    const int getGrade();
    void setGrade(const int value);
    
    const double getEffectMin();
    void setEffectMin(const double value);
    
    const double getEffectMax();
    void setEffectMax(const double value);
    
private:
    TInteger _nGrade;
    TDouble _dEffectMin;
    TDouble _dEffectMax;
};

class InfoAdventurePartyAbilityOrigin : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoAdventurePartyAbilityOrigin);
    InfoAdventurePartyAbilityOrigin();
    virtual ~InfoAdventurePartyAbilityOrigin();
    virtual bool init();
    
public:
    const int getIdx() const;
    void setIdx(const int value);
    
    const E_ADVENTURE_PROPERTY_TYPE getType() const;
    void setType(const E_ADVENTURE_PROPERTY_TYPE value);
    InfoAdventurePropertyType* const getInfoType() const;
    
    
    const cocos2d::Vector<InfoAdventurePartyAbilityEffect*>& getListEffect() const;
    void setListEffect(const std::string value);
    
    InfoAdventurePartyAbilityEffect* const getInfoEffect(const int nGrade) const;
    const double getEffect(int nLv, const int nGrade) const;
private:
    int _nIdx;
    E_ADVENTURE_PROPERTY_TYPE _eType;
    
    cocos2d::Vector<InfoAdventurePartyAbilityEffect*> _listEffect;
    
};

class InfoAdventurePartyAbility : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoAdventurePartyAbility);
    InfoAdventurePartyAbility();
    virtual ~InfoAdventurePartyAbility();
    virtual bool init();
    
public:
    const int getIdx() const;
    void setIdx(const int value);
    const E_ADVENTURE_PROPERTY_TYPE getType() const;
    const int getGrade() const;
    void setGrade(const int value);
    
    InfoAdventureMember* const getMember() const;
    void setMember(InfoAdventureMember* const value);
    
    InfoAdventurePartyAbilityEffect* const getInfoEffect() const;
    InfoAdventurePartyAbilityOrigin* const getInfoOrigin() const;
    const double getEffect(const int nLv) const;
    const bool isOpen() const;
    const cocos2d::Color3B getColor() const;
    const bool isPartyOption() const;
    const bool isRewardOption() const;
    
private:
    int _nIdx;
    int _nGrade;
    InfoAdventureMember* _infoMember;
    
};

#endif /* InfoAdventurePartyAbility_hpp */
