//
//  InfoAdventurePartyAbilityGradeRate.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/14.
//

#ifndef InfoAdventurePartyAbilityGradeRate_hpp
#define InfoAdventurePartyAbilityGradeRate_hpp

#include "Common/ConfigDefault.h"

class InfoAdventurePartyAbilityGradeRate : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoAdventurePartyAbilityGradeRate);
    InfoAdventurePartyAbilityGradeRate();
    virtual ~InfoAdventurePartyAbilityGradeRate();
    virtual bool init();
   
public:
    const int getGrade() ;
    void setGrade(const int value);
   
    const double getRate() ;
    void setRate(const double value);
   
    const cocos2d::Color3B getColor()  const;
    void setColor(const cocos2d::Color3B value);
private:
    TInteger _nGrade;
    TDouble _dRate;
    cocos2d::Color3B _color;
};

#endif /* InfoAdventurePartyAbilityGradeRate_hpp */
