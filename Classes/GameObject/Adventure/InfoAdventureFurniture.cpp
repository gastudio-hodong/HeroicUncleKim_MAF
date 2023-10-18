//
//  InfoAdventureOFurniture.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/18.
//

#include "InfoAdventureFurniture.h"

#pragma mark -
InfoAdventureFurniture::InfoAdventureFurniture()
{
    _listCondition.clear();
}

InfoAdventureFurniture::~InfoAdventureFurniture()
{
    _listCondition.clear();
}

bool InfoAdventureFurniture::init()
{
    InfoAdventureLocalUpgradeBase::init();
    return true;
}

const int InfoAdventureFurniture::getImageGrade() const
{
    int result = 0;
    int nRatio = getMaxLv()/5;
    
    result = _nCurrentLv/nRatio;
    if(result <= 0)
        result++;
    else if(result >= 5)
        result = 5;
    
    return result;
}

const int InfoAdventureFurniture::getLevelupCondition(const int value) const
{
    int result = -1;
    
    if(_listCondition.size() > value-1)
        result = _listCondition[value-1];
    
    return result;
}


void InfoAdventureFurniture::setLevelupCondition(const std::vector<int> value)
{
    _listCondition = value;
}
