//
//  InfoAdventureMember.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/18.
//

#include "InfoAdventureMember.h"

#include "ManagerGame/AdventureManager.h"

USING_NS_CC;

#pragma mark -
InfoAdventureMember::InfoAdventureMember():
_nImageIdx(0)
,_nParty(0)
,_nRequireLv(0)
{
}

InfoAdventureMember::~InfoAdventureMember()
{
}

bool InfoAdventureMember::init()
{
    InfoAdventureLocalUpgradeBase::init();
    return true;
}

const int InfoAdventureMember::getParty() const
{
    return _nParty;
}
void InfoAdventureMember::setParty(const int value)
{
    _nParty = value;
}

const int InfoAdventureMember::getImageIdx() const
{
    return _nImageIdx;
}

void InfoAdventureMember::setImageIdx(const int value)
{
    _nImageIdx = value;
}

const int InfoAdventureMember::getRequireLv() const
{
    return _nRequireLv;
}
void InfoAdventureMember::setRequireLv(const int value)
{
    _nRequireLv = value;
}

const int InfoAdventureMember::getImageGrade() const
{
    int result = 0;
    int nRatio = getMaxLv()/4;
    
    result = _nCurrentLv/nRatio;
    if(result <= 0)
        result++;
    else if(result >= 4)
        result = 4;
    
    return result;
}

const bool InfoAdventureMember::isOpen() const
{
    bool result = false;
    
    int nCurrentLv = AdventureManager::getInstance()->getFurniturePropertyEffect(E_ADVENTURE_PROPERTY_TYPE::PARTY_MEMBER_OPEN);
    
    if(nCurrentLv >= _nRequireLv)
        result = true;
    
    return result;
}
