//
//  InfoItems.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 8. 3..
//

#include "InfoItems.h"

#include "GameObject/InfoArtifact.h"
#include "GameObject/InfoPetSkin.h"
#include "GameObject/InfoBadge.h"
#include "GameObject/Defense/InfoDefenseCharacter.h"
#include "GameObject/Defense/InfoDefenseWeapon.h"

#include "ManagerGame/DefenseManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/CostumeSkinManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/BadgeManager.h"
#include "ManagerGame/WeaponSpiritManager.h"
#include "ManagerGame/InfiniteArtifactManager.h"

USING_NS_CC;

InfoItems* InfoItems::create(E_ITEMS eType)
{
    return create((int)eType);
}

InfoItems* InfoItems::create(int nIdx)
{
    InfoItems *pRet = new(std::nothrow) InfoItems();
    if (pRet && pRet->init(nIdx))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

InfoItems::InfoItems() :
_nIdx(0),

_strTextKey(""),
_strCommentKey(""),
_strPath(""),

_strType(""),
_strTypeDetail(""),
_strTypeName(""),
_nTypeParams(0),

_strCount(""),

_bString(false),
_bServer(false),

_strVersion("1.0.0")
{
    
}

InfoItems::~InfoItems()
{
    
}

bool InfoItems::init(int nIdx)
{
    setIdx(nIdx);
    return true;
}

InfoItems* InfoItems::clone() const
{
    auto objCopy = InfoItems::create(_nIdx);
    objCopy->setTextKey(_strTextKey);
    objCopy->setCommentKey(_strCommentKey);
    objCopy->setPath(_strPath);
    
    objCopy->setType(_strType);
    objCopy->setTypeDetail(_strTypeDetail);
    objCopy->setTypeName(_strTypeName);
    objCopy->setTypeParams(_nTypeParams);
    
    objCopy->setReward(_strReward);
    
    objCopy->setString(_bString);
    objCopy->setServer(_bServer);
    objCopy->setVersion(_strVersion);
    
    return objCopy;
}

#pragma set, get
int InfoItems::getIdx()
{
    return _nIdx;
}
void InfoItems::setIdx(int nIdx)
{
    _nIdx = nIdx;
}

std::string InfoItems::getText()
{
    std::string strResult = GAME_TEXT(_strTextKey);
    
    if ( getType().compare("ARTIFACT") == 0 )
    {
        E_ARTIFACT eType = (E_ARTIFACT)getTypeParams();
        auto objArtifact = DataManager::GetArtifactInfo(eType);
        if ( objArtifact != nullptr )
        {
            strResult = objArtifact->getTitle();
        }
    }
    else if ( getType().compare("ARTIFACT_INFINITE") == 0 )
    {
        INFINITE_ARTIFACT_TYPE eType = (INFINITE_ARTIFACT_TYPE)getTypeParams();
        
        auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(eType);
        if ( objArtifact != nullptr )
        {
            strResult = objArtifact->getName();
        }
    }
    else if ( getType().compare("COSTUME") == 0 )
    {
        if ( getTypeDetail().compare("SKIN") == 0 )
        {
            auto objSkin = CostumeSkinManager::getInstance()->getInfoSkin(getTypeParams());
            if(objSkin != nullptr)
                strResult = objSkin->getName();
        }
        else
        {
            E_COSTUME_IDX eIdx = (E_COSTUME_IDX)getTypeParams();
            E_COSTUME eType = E_COSTUME::HEAD;
            if ( getTypeDetail().compare("HEAD") == 0 )           eType = E_COSTUME::HEAD;
            else if ( getTypeDetail().compare("BODY") == 0 )      eType = E_COSTUME::BODY;
            else if ( getTypeDetail().compare("CAPE") == 0 )      eType = E_COSTUME::CAPE;
            else if ( getTypeDetail().compare("HAND") == 0 )      eType = E_COSTUME::HAND;
            else if ( getTypeDetail().compare("FOOT") == 0 )      eType = E_COSTUME::FOOT;
            else if ( getTypeDetail().compare("JEWEL") == 0 )     eType = E_COSTUME::JEWEL;
            else if ( getTypeDetail().compare("SETITEM") == 0 )   eType = E_COSTUME::SETITEM;
            
            auto objCostume = CostumeManager::getInstance()->getCostume(eType, eIdx);
            if ( objCostume != nullptr )
                strResult = objCostume->getTitle();
        }
    }
    else if(getType().compare("PET_SKIN") == 0)
    {
        E_PET eType;
        if(_nIdx == 14000)
            eType = E_PET::CHICK_1;
        else if(_nIdx == 14001)
            eType = E_PET::HAMSTER_2;
        else if(_nIdx == 14002)
            eType = E_PET::DRAGON_3;
        else if(_nIdx == 14003)
            eType = E_PET::PRINCESS_4;
        else if(_nIdx == 14004)
            eType = E_PET::PURIRING_5;
        else if(_nIdx == 14005)
            eType = E_PET::HONG_6;
        else if(_nIdx == 14006)
            eType = E_PET::MOLE_7;
        else if(_nIdx == 14007)
            eType = E_PET::HANDSOME_8;
        
        auto obj = PetNewManager::getInstance()->getInfoPetSkin(eType, _nTypeParams);
        if ( obj != nullptr )
        {
            strResult = obj->getName() + " " + GAME_TEXT(_strTextKey);
        }
    }
    else if(getType().compare("WEAPON_SPIRIT") == 0)
    {
        strResult = GAME_TEXT(MafUtils::format(_strTextKey.c_str(), getTypeParams()));
    }
    
    
    if(getTypeName().compare("ADVT_CORE") == 0)
    {
        strResult = GAME_TEXT(MafUtils::format(_strTextKey.c_str(), getTypeParams()));
    }
    
    return strResult;
}
void InfoItems::setTextKey(std::string strKey)
{
    _strTextKey = strKey;
}

std::string InfoItems::getComment()
{
    return GAME_TEXT(_strCommentKey);
}
void InfoItems::setCommentKey(std::string strKey)
{
    _strCommentKey = strKey;
}

std::string InfoItems::getPath()
{
    std::string strResult = _strPath;
    
    if ( getType().compare("ARTIFACT") == 0 )
    {
        E_ARTIFACT eType = (E_ARTIFACT)getTypeParams();
        auto objArtifact = DataManager::GetArtifactInfo(eType);
        if ( objArtifact != nullptr )
        {
            strResult = objArtifact->getIconPath();
        }
    }
    else if ( getType().compare("COSTUME") == 0 )
    {
        strResult = MafUtils::format(_strPath.c_str(), getTypeParams());
    }
    else if ( getType().compare("PET_SKIN") == 0 )
    {
        strResult = MafUtils::format(_strPath.c_str(), getTypeParams());
    }
    else if ( getType().compare("PET_EQUIP") == 0 )
    {
        strResult = MafUtils::format(_strPath.c_str(), getTypeParams());
    }
    else if ( getType().compare("BADGE") == 0 )
    {
        auto objBadge = BadgeManager::getInstance()->getBadge(getTypeParams());
        if ( objBadge != nullptr )
        {
            strResult = objBadge->getPath();
        }
    }
    else if ( getType().compare("ARTIFACT_INFINITE") == 0 )
    {
        INFINITE_ARTIFACT_TYPE eType = (INFINITE_ARTIFACT_TYPE)getTypeParams();
        
        auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(eType);
        if ( objArtifact != nullptr )
        {
            strResult = objArtifact->getIconPath();
        }
    }
    else if ( getType().compare("LOOT") == 0 )
    {
        strResult = MafUtils::format(_strPath.c_str(), getTypeParams());
    }
    else if( getType().compare("WEAPON_SPIRIT") == 0)
    {
        auto obj = WeaponSpiritManager::getInstance()->getInfoWeaponSpirit(getTypeParams());
        if(obj != nullptr)
            strResult = obj->getIconPath();
    }
    else if ( getTypeName().compare("ADVT_CORE") == 0 )
    {
        strResult = MafUtils::format(_strPath.c_str(), getTypeParams());
    }
    else if ( getTypeName().compare("ADVENTURE_RELIC") == 0 )
    {
        strResult = MafUtils::format(_strPath.c_str(), getTypeParams());
    }
    else if ( getType().compare("SKILL_FROST") == 0 )
    {
        strResult = MafUtils::format(_strPath.c_str(), getTypeParams());
    }
    else if ( getType().compare("DEFENSE_WEAPON") == 0 )
    {
        int idx = getTypeParams();
        if(idx == 0)
        {
            strResult = "Assets/icon/icon_defense_gun.png";
        }
        else
        {
            if(auto data = DefenseManager::getInstance()->getWeaponData(idx))
                strResult = data->getResourceIconPath();
        }
        
    }
    else if ( getType().compare("DEFENSE_CHARACTER") == 0 )
    {
        int idx = getTypeParams();
        if(idx == 0)
        {
            strResult = "Assets/icon/icon_defense_mole.png";
        }
        else
        {
            if(auto data = DefenseManager::getInstance()->getCharacterData(idx))
                strResult = data->getResourcePath();
        }
    }
    
    return strResult;
}
void InfoItems::setPath(std::string strPath)
{
    _strPath = strPath;
}

std::string InfoItems::getType()
{
    return _strType;
}
void InfoItems::setType(std::string strType)
{
    _strType = strType;
}

std::string InfoItems::getTypeDetail()
{
    return _strTypeDetail;
}
void InfoItems::setTypeDetail(std::string strTypeDetail)
{
    _strTypeDetail = strTypeDetail;
}

std::string InfoItems::getTypeName()
{
    return _strTypeName;
}
void InfoItems::setTypeName(std::string strTypeName)
{
    _strTypeName = strTypeName;
}

int InfoItems::getTypeParams()
{
    return _nTypeParams;
}
void InfoItems::setTypeParams(int nTypeParams)
{
    _nTypeParams = nTypeParams;
}

std::string InfoItems::getReward()
{
    return _strReward;
}
void InfoItems::setReward(std::string strReward)
{
    _strReward = strReward;
}

std::string InfoItems::getCount()
{
    std::string strCount = MafAes256::Decrypt(_strCount);
    return strCount;
}
void InfoItems::setCount(int nCount)
{
    setCount(MafUtils::toString(nCount));
}
void InfoItems::setCount(std::string strCount)
{
    _strCount = MafAes256::Encrypt(strCount);
}

Color3B InfoItems::getColor()
{
    Color3B color = Color3B::WHITE;
    
    switch (_nIdx) {
        case 1:     color = COLOR_COIN;                 break;
        case 2:     color = COLOR_KEY;                  break;
        case 3:     color = COLOR_GEM;                  break;
        case 4:     color = COLOR_COIN;                 break;
        case 5:     color = COLOR_MATERIAL;             break;
        case 6:     color = COLOR_HONOR;                break;
        case 7:     color = COLOR_POINT;                break;
        case 8:     color = Color3B(248,48,100);        break;
        case 9:     color = COLOR_D_KEY;                break;
        case 10:    color = COLOR_D_COIN;               break;
        case 20:    color = COLOR_HELL;                 break;
        case 125:   color = Color3B(180, 100, 255);     break;
            
        default:
            break;
    }

    return color;
}

bool InfoItems::isString()
{
    return _bString;
}
void InfoItems::setString(bool bString)
{
    _bString = bString;
}

bool InfoItems::isServer()
{
    return _bServer;
}
void InfoItems::setServer(bool bServer)
{
    _bServer = bServer;
}

bool InfoItems::isVersion()
{
    bool bVersion = true;
    if ( MafNative::IsLessThanCurrVersion(_strVersion) == false )
    {
        bVersion = false;
    }
    
    return bVersion;
}
void InfoItems::setVersion(std::string strVersion)
{
    _strVersion = strVersion;
}
