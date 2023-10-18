//
//  InfoMail.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 14/02/2020.
//

#include "InfoMail.h"

#include "ManagerGame/ItemsManager.h"


USING_NS_CC;

InfoMail::InfoMail() :
_idx(0),

_title(""),
_deleteTime(0)
{
    
}

InfoMail::~InfoMail()
{
    
}

bool InfoMail::init()
{
    return true;
}

int InfoMail::getIdx()
{
    return _idx;
}
void InfoMail::setIdx(int value)
{
    _idx = value;
}

std::string InfoMail::getTitle()
{
    return _title;
}
void InfoMail::setTitle(std::string value)
{
    _title = value;
}

int64_t InfoMail::getDeleteTime()
{
    return _deleteTime;
}

void InfoMail::setDeleteTime(int64_t value)
{
    _deleteTime = value;
}

cocos2d::Vector<InfoItems*> InfoMail::getListItems()
{
    return _listItems;
}

void InfoMail::setItems(std::string value)
{
    _listItems.clear();
    _listItems = ItemsManager::getInstance()->getConvertArray(value);
}
