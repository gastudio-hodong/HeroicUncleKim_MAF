//
//  InfoProductGrowth.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 8. 3..
//

#include "InfoProductGrowth.h"

#include "ManagerGame/ItemsManager.h"

USING_NS_CC;
 
#pragma mark - InfoProductGrowth
InfoProductGrowth::InfoProductGrowth() :
_type(""),

_level(0),
_idx(0),
_idxCash(0),

_condition(0),

_bRewardNormal(false),
_bRewardPass(false)
{
    
}

InfoProductGrowth::~InfoProductGrowth()
{
    
}

bool InfoProductGrowth::init()
{
    return true;
}

std::string InfoProductGrowth::getType()
{
    return _type;
}
void InfoProductGrowth::setType(std::string value)
{
    _type = value;
}

int InfoProductGrowth::getLevel()
{
    return _level;
}
void InfoProductGrowth::setLevel(int value)
{
    _level = value;
}

int InfoProductGrowth::getIdx()
{
    return _idx;
}
void InfoProductGrowth::setIdx(int value)
{
    _idx = value;
}

int InfoProductGrowth::getIdxCash()
{
    return _idxCash;
}
void InfoProductGrowth::setIdxCash(int value)
{
    _idxCash = value;
}

int InfoProductGrowth::getCondition()
{
    return _condition;
}
void InfoProductGrowth::setCondition(int value)
{
    _condition = value;
}

bool InfoProductGrowth::isRewardFree()
{
    return _bRewardNormal;
}
void InfoProductGrowth::setRewardFree(bool bReward)
{
    _bRewardNormal = bReward;
}

bool InfoProductGrowth::isRewardCash()
{
    return _bRewardPass;
}
void InfoProductGrowth::setRewardCash(bool bReward)
{
    _bRewardPass = bReward;
}

Vector<InfoItems*> InfoProductGrowth::getItemsFree()
{
    return _listItemsFree;
}

void InfoProductGrowth::setItemsFree(std::string strItems)
{
    _listItemsFree.clear();
    _listItemsFree = ItemsManager::getInstance()->getConvertArray(strItems);
}

Vector<InfoItems*> InfoProductGrowth::getItemsCash()
{
    return _listItemsCash;
}

void InfoProductGrowth::setItemsCash(std::string strItems)
{
    _listItemsCash.clear();
    _listItemsCash = ItemsManager::getInstance()->getConvertArray(strItems);
}
