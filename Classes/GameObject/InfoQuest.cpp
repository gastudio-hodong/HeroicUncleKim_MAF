//
//  InfoQuest.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 14/02/2020.
//

#include "InfoQuest.h"

#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

InfoQuest::InfoQuest() :
_group(0),
_idx(0),
_type(0),

_titleKey(""),
_title(""),
_descKey(""),
_desc(""),

_nCurrentCount(0),
_nCompleteCount(0),
_nCurrentBigCount(0),
_nCompleteBigCount(0),

_bCountShort(false),
_bReceive(false)
{
    
}

InfoQuest::~InfoQuest()
{
    
}

bool InfoQuest::init()
{
    return true;
}
 
int InfoQuest::getGroup()
{
    return _group;
}
void InfoQuest::setGroup(int value)
{
    _group = value;
}

int InfoQuest::getIdx()
{
    return _idx;
}
void InfoQuest::setIdx(int value)
{
    _idx = value;
}

int InfoQuest::getType()
{
    return _type;
}
void InfoQuest::setType(int value)
{
    _type = value;
}

std::string InfoQuest::getTitle()
{
    if ( _titleKey.length() != 0 )
    {
        return GAME_TEXT(_titleKey);
    }
    else
    {
        return _title;
    }
}
void InfoQuest::setTitleKey(std::string value)
{
    _titleKey = value;
}
void InfoQuest::setTitle(std::string value)
{
    _title = value;
}

std::string InfoQuest::getDesc()
{
    if ( _descKey.length() != 0 )
    {
        return GAME_TEXT(_descKey);
    }
    else
    {
        return _desc;
    }
}
void InfoQuest::setDescKey(std::string value)
{
    _descKey = value;
}
void InfoQuest::setDesc(std::string value)
{
    _desc = value;
}

uint64_t InfoQuest::getCountNow()
{
    return _nCurrentCount;
}
void InfoQuest::setCountNow(uint64_t value)
{
    _nCurrentCount = value;
}

uint64_t InfoQuest::getCountMax()
{
    return _nCompleteCount;
}
void InfoQuest::setCountMax(uint64_t value)
{
    _nCompleteCount = value;
}

double InfoQuest::getCountBigNow()
{
    return _nCurrentBigCount;
}
void InfoQuest::setCountBigNow(double value)
{
    _nCurrentBigCount = value;
}

double InfoQuest::getCountBigMax()
{
    return _nCompleteBigCount;
}
void InfoQuest::setCountBigMax(double value)
{
    _nCompleteBigCount = value;
}

bool InfoQuest::isCountShort()
{
    return _bCountShort;
}
void InfoQuest::setCountShort(bool value)
{
    _bCountShort = value;
}

bool InfoQuest::isReceive()
{
    return _bReceive;
}
void InfoQuest::setReceive(bool value)
{
    _bReceive = value;
}

cocos2d::Vector<InfoItems*> InfoQuest::getListReward()
{
    return _listItems;
}
void InfoQuest::setReward(std::string value)
{
    _listItems.clear();
    _listItems = ItemsManager::getInstance()->getConvertArray(value);
}

void InfoQuest::addReward(int idx, int params, int count)
{
    auto objItem = ItemsManager::getInstance()->getInfoItems(idx);
    if ( objItem != nullptr )
    {
        objItem->setTypeParams(params);
        objItem->setCount(count);
        
        _listItems.pushBack(objItem);
    }
}
