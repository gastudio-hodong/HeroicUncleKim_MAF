//
//  InfoAdventurePartyAbilityGradeRate.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/14.
//

#include "InfoAdventurePartyAbilityGradeRate.h"

USING_NS_CC;

InfoAdventurePartyAbilityGradeRate::InfoAdventurePartyAbilityGradeRate():
_nGrade(0)
,_dRate(0)
,_color(Color3B(0,0,0))
{
   
}

InfoAdventurePartyAbilityGradeRate::~InfoAdventurePartyAbilityGradeRate()
{
   
}

bool InfoAdventurePartyAbilityGradeRate::init()
{
  return true;
}

const int InfoAdventurePartyAbilityGradeRate::getGrade()
{
  return _nGrade;
}
void InfoAdventurePartyAbilityGradeRate::setGrade(const int value)
{
  _nGrade = value;
}

const double InfoAdventurePartyAbilityGradeRate::getRate()
{
  return _dRate;
}

void InfoAdventurePartyAbilityGradeRate::setRate(const double value)
{
  _dRate = value;
}

const Color3B InfoAdventurePartyAbilityGradeRate::getColor() const
{
  return _color;
}

void InfoAdventurePartyAbilityGradeRate::setColor(const Color3B value)
{
    _color = value;
}
