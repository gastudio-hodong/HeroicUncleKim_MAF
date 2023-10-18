//
//  InfoFriend.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 14/02/2020.
//

#include "InfoFriend.h"

USING_NS_CC;

InfoFriend::InfoFriend() :
_nUseridx(0),
_strPlatform(""),
_strNick(""),
_bReceive(false),
_bSend(false),
_nPet(E_PET::PET_NONE),
_nPetEvolution(0),
_nLoginTime(0)
{
    _vecCostume.clear();
}

InfoFriend::~InfoFriend()
{
    
}

bool InfoFriend::init()
{
    return true;
}
void InfoFriend::setUseridx(int idx)
{
    _nUseridx = idx;
}
int InfoFriend::getUseridx()
{
    return _nUseridx;
}

void InfoFriend::setCostume(std::vector<int> list)
{
    
    while ( list.size() > 6)
    {
        list.pop_back();
    }
    
    _vecCostume = list;
}
std::vector<int> InfoFriend::getCostume()
{
    return _vecCostume;
}

void InfoFriend::setPlatform(std::string platform)
{
    _strPlatform = platform;
}
std::string InfoFriend::getPlatform()
{
    return _strPlatform;
}

void InfoFriend::setNickname(std::string nickname)
{
    _strNick = nickname;
}
std::string InfoFriend::getNickname()
{
    return _strNick;
}

void InfoFriend::setPointReceiveAvailable(bool point)
{
    _bReceive = point;
}
bool InfoFriend::getPointReceiveAvailable()
{
    return _bReceive;
}

void InfoFriend::setPointSendAvailable(bool point)
{
    _bSend = point;
}
bool InfoFriend::getPointSendAvailable()
{
    return _bSend;
}
void InfoFriend::setDibson(bool dib)
{
    std::string strKey = MafUtils::format("dibs_friend_%d", getUseridx());
    UserDefault::getInstance()->setBoolForKey(strKey.c_str(), dib);
}
bool InfoFriend::getDibson()
{
    std::string strKey = MafUtils::format("dibs_friend_%d", getUseridx());
    bool bDib = UserDefault::getInstance()->getBoolForKey(strKey.c_str(), false);
    return bDib;
}
void InfoFriend::setUserPet(E_PET pet)
{
    _nPet = pet;
}
E_PET InfoFriend::getUserPet()
{
    return _nPet;
}

void InfoFriend::setUserPetEvolution(int nEvolution)
{
    _nPetEvolution = nEvolution;
}

int InfoFriend::getUserPetEvolution()
{
    return _nPetEvolution;
}

void InfoFriend::setLoginTime(int64_t nTime)
{
    _nLoginTime = nTime;
}

int InfoFriend::getLoginDay()
{
    int nResult = -1;
    
    if ( _nLoginTime != 0 )
    {
        int nTime = (int)(UtilsDate::getInstance()->getTime() - _nLoginTime);
        
        nResult = nTime / 86400;
        if ( nResult > 7 )
            nResult = 7;
    }
    
    return nResult;
}
