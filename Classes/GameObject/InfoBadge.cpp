//
//  InfoBadge.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/01.
//

#include "InfoBadge.h"

#include "ManagerGame/BadgeManager.h"
#include "ManagerGame/AdventureRelicManager.h"

USING_NS_CC;

#pragma mark - InfoBadge
InfoBadge::InfoBadge():
_nIdx(0),
_nIdxSub(0),
_strNameKey(""),
_strDescKey(""),
_strPath(""),
_strGroup(""),
_eBuff(E_BADGE_BUFF::BADGE_NONE),
_nBuffCnt(0)
{
    
}

InfoBadge::~InfoBadge()
{
    
}

bool InfoBadge::init()
{
    return true;
}
 
int InfoBadge::getIdx()
{
    return _nIdx;
}
void InfoBadge::setIdx(int nIdx)
{
    _nIdx = nIdx;
}

int InfoBadge::getIdxSub()
{
    return _nIdxSub;
}
void InfoBadge::setIdxSub(int nIdx)
{
    _nIdxSub = nIdx;
}

std::string InfoBadge::getName()
{
    return GAME_TEXT(_strNameKey);
}
void InfoBadge::setNameKey(std::string strNameKey)
{
    _strNameKey = strNameKey;
}

std::string InfoBadge::getDesc()
{
    return GAME_TEXT(_strDescKey);
}
void InfoBadge::setDescKey(std::string strDescKey)
{
    _strDescKey = strDescKey;
}

std::string InfoBadge::getPath()
{
    return _strPath;
}
void InfoBadge::setPath(std::string strPath)
{
    _strPath = strPath;
}

std::string InfoBadge::getGroup()
{
    return _strGroup;
}
void InfoBadge::setGroup(std::string strGroup)
{
    _strGroup = strGroup;
}

E_BADGE_BUFF InfoBadge::getBuffType()
{
    return _eBuff;
}
void InfoBadge::setBuffType(E_BADGE_BUFF value)
{
    _eBuff = value;
}

float InfoBadge::getBuffCount(const bool isOriginal)
{
    float result = _nBuffCnt;
    
    if(!isOriginal)
    {
        //모험 유물 적용
        double advtRelicBuff = AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::BADGE_ATK_BONUS);
        float relicBonus = result * advtRelicBuff * 0.01;
        
        result += relicBonus;
    }
    
    return result;
}
void InfoBadge::setBuffCount(float value)
{
    _nBuffCnt = value;
}

std::string InfoBadge::getBuffText()
{
    std::string strText = "";
    
    if ( _eBuff == E_BADGE_BUFF::BADGE_ATK )
    {
        double nEffect = _nBuffCnt * 100.0;
        strText = GAME_TEXTFORMAT("t_arti_d2_option_1", nEffect);
    }
    
    return strText;
}


#pragma mark - InfoBadgeHave
InfoBadgeHave::InfoBadgeHave():
_nIdx(0),
_nIdxBadge(0),
_nIdxSeason(0),
_nCreated(0)
{
    
}

InfoBadgeHave::~InfoBadgeHave()
{
    
}

bool InfoBadgeHave::init()
{
    return true;
}

int InfoBadgeHave::getIdx()
{
    return _nIdx;
}
void InfoBadgeHave::setIdx(int nIdx)
{
    _nIdx = nIdx;
}

int InfoBadgeHave::getIdxBadge()
{
    return _nIdxBadge;
}
void InfoBadgeHave::setIdxBadge(int nIdx)
{
    _nIdxBadge = nIdx;
}

int InfoBadgeHave::getIdxSeason()
{
    return _nIdxSeason;
}
void InfoBadgeHave::setIdxSeason(int nIdx)
{
    _nIdxSeason = nIdx;
}

int64_t InfoBadgeHave::getCreated()
{
    return _nCreated;
}
void InfoBadgeHave::setCreated(int64_t nTime)
{
    _nCreated = nTime;
}

E_BADGE_BUFF InfoBadgeHave::getBuffType()
{
    E_BADGE_BUFF result = E_BADGE_BUFF::BADGE_NONE;
    
    auto info = BadgeManager::getInstance()->getBadge(_nIdxBadge);
    
    if(info != nullptr)
    {
        result = info->getBuffType();
    }
    
    return result;
}

float InfoBadgeHave::getBuffCount()
{
    float result = 0;
    
    auto info = BadgeManager::getInstance()->getBadge(_nIdxBadge);
    
    if(info != nullptr)
    {
        result = info->getBuffCount();
    }
    
    return result;
}
