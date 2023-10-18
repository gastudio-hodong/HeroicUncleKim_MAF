//
//  InfoChat.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/07.
//

#include "InfoChat.h"

USING_NS_CC;

InfoChat::InfoChat() :
_nIdx(0),

_strUserID(""),
_strUserNick(""),
_strMessage(""),

_nRank(0),

_nSkin(0),
_strCostume("")
{
}

InfoChat::~InfoChat()
{
}

bool InfoChat::init()
{
    return true;
}

int InfoChat::getIdx()
{
    return _nIdx;
}

void InfoChat::setIdx(int value)
{
    _nIdx = value;
}

int InfoChat::getUserIdx()
{
    return _nUserIdx;
}

void InfoChat::setUserIdx(int value)
{
    _nUserIdx = value;
}

std::string InfoChat::getUserId()
{
    return _strUserID;
}

void InfoChat::setUserId(std::string value)
{
    _strUserID = value;
}

std::string InfoChat::getUserNick()
{
    return _strUserNick;
}

void InfoChat::setUserNick(std::string value)
{
    _strUserNick = value;
}

std::string InfoChat::getUserPlatform()
{
    return _strUserPlatform;
}

void InfoChat::setUserPlatform(std::string value)
{
    _strUserPlatform = value;
}

std::string InfoChat::getMessage()
{
    return _strMessage;
}

void InfoChat::setMessage(std::string value)
{
    _strMessage = value;
}

int InfoChat::getRank()
{
    return _nRank;
}

void InfoChat::setRank(int value)
{
    _nRank = value;
}

int InfoChat::getSkin()
{
    return _nSkin;
}

void InfoChat::setSkin(int value)
{
    _nSkin = value;
}

int InfoChat::getCostume()
{
    int nCostume = 0;
    std::vector<std::string> listCostume = UtilsString::getInstance()->split(_strCostume, ',');
    if ( listCostume.size() > 0 )
    {
        nCostume = std::atoi(listCostume.at(0).c_str());
    }
    
    return nCostume;
}

void InfoChat::setCostume(std::string value)
{
    _strCostume = value;
}
  
