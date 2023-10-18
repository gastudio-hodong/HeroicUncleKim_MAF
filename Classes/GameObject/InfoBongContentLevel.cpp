//
//  InfoRaidBongLevel.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/27.
//

#include "InfoBongContentLevel.h"

USING_NS_CC;

InfoBongContentLevel::InfoBongContentLevel():
_nLevel(0),
_fTotalEXP(0)
{
    
}

InfoBongContentLevel::~InfoBongContentLevel()
{
    
}

bool InfoBongContentLevel::init()
{
    return true;
}
 
int InfoBongContentLevel::getLevel()
{
    return _nLevel;
}
void InfoBongContentLevel::setLevel(int value)
{
    _nLevel = value;
}
double InfoBongContentLevel::getTotalEXP()
{
    return _fTotalEXP;
}
void InfoBongContentLevel::setTotalEXP(double value)
{
    _fTotalEXP = value;
}
