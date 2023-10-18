//
//  InfoRaidBongStat.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/01.
//

#include "InfoRaidBongStat.h"

#include "ManagerGame/NewRaidManager.h"

USING_NS_CC;

InfoRaidBongStat::InfoRaidBongStat():
_nIdx(0),
_nCurrentLv(0),
_nMaxLv(0),
_strPath(""),
_strTextKey("")
{
    
}

InfoRaidBongStat::~InfoRaidBongStat()
{
    
}

bool InfoRaidBongStat::init()
{
    return true;
}
 
int InfoRaidBongStat::getIdx()
{
    return _nIdx;
}
void InfoRaidBongStat::setIdx(int value)
{
    _nIdx = value;
}

int InfoRaidBongStat::getCurrentLv()
{
    return _nCurrentLv;
}
void InfoRaidBongStat::setCurrentLv(int value)
{
    _nCurrentLv = value;
}

int InfoRaidBongStat::getMaxLv()
{
    return _nMaxLv;
}
void InfoRaidBongStat::setMaxLv(int value)
{
    _nMaxLv = value;
}

double InfoRaidBongStat::getEffect()
{
    double result = 0;
    
    auto obj = NewRaidManager::getInstance()->getRaidStatIncrease(_nCurrentLv);
    
    if(obj != nullptr)
    {
        switch(_nIdx)
        {
            case 1: result = obj->getDmg(); break;
            case 2: result = obj->getCriDmg() * 100; break;
            case 3: result = obj->getCriRate() * 100; break;
            case 4: result = obj->getDoubleRate() * 100; break;
            default:  break;
        }
    }
    
    return result;
}

std::string InfoRaidBongStat::getIconPath()
{
    return _strPath;
}
void InfoRaidBongStat::setIconPath(std::string value)
{
    _strPath = value;
}

std::string InfoRaidBongStat::getText()
{
    return GAME_TEXTFORMAT(_strTextKey, getCurrentLv());
}
void InfoRaidBongStat::setTextKey(std::string value)
{
    _strTextKey = value;
}
