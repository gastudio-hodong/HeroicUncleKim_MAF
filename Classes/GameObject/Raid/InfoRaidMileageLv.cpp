//
//  InfoRaidMileageLv.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/27.
//

#include "InfoRaidMileageLv.h"

USING_NS_CC;

InfoRaidMileageLv::InfoRaidMileageLv():
_nILv(0),
_nValue(0)
{
    
}

InfoRaidMileageLv::~InfoRaidMileageLv()
{
    
}

bool InfoRaidMileageLv::init()
{
    return true;
}
 
int InfoRaidMileageLv::getLv()
{
    return _nILv;
}
void InfoRaidMileageLv::setLv(int value)
{
    _nILv = value;
}
int InfoRaidMileageLv::getValue()
{
    return _nValue;
}
void InfoRaidMileageLv::setValue(int value)
{
    _nValue = value;
}
