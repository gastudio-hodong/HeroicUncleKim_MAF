//
//  InfoMoreGames.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/04/12.
//

#include "InfoMoreGame.h"

USING_NS_CC;

#pragma mark - InfoMoreGames
InfoMoreGame::InfoMoreGame():
_nIdx(0),
_strName(""),
_strCountry(""),
_strCountryExcept(""),
_strPlatform(""),
_strIconFileName(""),
_strAdsVideoPath(""),
_strLink("")
{
    
}

InfoMoreGame::~InfoMoreGame()
{
    
}

bool InfoMoreGame::init()
{
    return true;
}
 
int InfoMoreGame::getIdx()
{
    return _nIdx;
}

std::string InfoMoreGame::getName()
{
    return _strName;
}

std::string InfoMoreGame::getCountry()
{
    return _strCountry;
}

std::string InfoMoreGame::getCountryExcept()
{
    return _strCountryExcept;
}

std::string InfoMoreGame::getPlatform()
{
    return _strPlatform;
}

std::string InfoMoreGame::getIconFileName()
{
    return _strIconFileName;
}

std::string InfoMoreGame::getAdsVideoPath()
{
    return _strAdsVideoPath;
}

std::string InfoMoreGame::getLink()
{
    return _strLink;
}

void InfoMoreGame::setIdx(int value)
{
    _nIdx = value;
}

void InfoMoreGame::setName(std::string value)
{
    _strName = value;
}

void InfoMoreGame::setCountry(std::string value)
{
    _strCountry = value;
}

void InfoMoreGame::setCountryExcept(std::string value)
{
    _strCountryExcept = value;
}

void InfoMoreGame::setPlatform(std::string value)
{
    _strPlatform = value;
}

void InfoMoreGame::setIconFileName(std::string value)
{
    _strIconFileName = value;
}

void InfoMoreGame::setAdsVideoPath(std::string value)
{
    _strAdsVideoPath = value;
}

void InfoMoreGame::setLink(std::string value)
{
    _strLink = value;
}
