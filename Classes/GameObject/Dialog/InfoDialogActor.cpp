//
//  InfoDialogActor.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/20.
//

#include "InfoDialogActor.h"

USING_NS_CC;

#pragma mark -
InfoDialogActor::InfoDialogActor():
_nID(0)
,_strImagePath("")
{
}

InfoDialogActor::~InfoDialogActor()
{
}

bool InfoDialogActor::init()
{
    return true;
}

const int InfoDialogActor::getID() const
{
    return _nID;
}
void InfoDialogActor::setID(const int value)
{
    _nID = value;
}

const std::string InfoDialogActor::getImagePath() const
{
    return _strImagePath;
}
void InfoDialogActor::setImagePath(const std::string value)
{
    _strImagePath = value;
}
