//
//  TimingPackageManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/06/29.
//

#include "TimingPackageManager.h"

#include "GameObject/InfoItems.h"

#include "GameUIPopup/Other/TimingPackage/PopupTimingPackage.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/PrisonManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/InfiniteArtifactManager.h"


USING_NS_CC;

#pragma mark - Manager
TimingPackageManager::TimingPackageManager():

//
_callbackInfo(nullptr),
_callbackReward(nullptr)
{
    _listTimingPackgeInfo.clear();
    _listOfflineRewardPackageInfo.clear();
    _eventTimingPackage.Clear();
}

TimingPackageManager::~TimingPackageManager(void)
{
    _listTimingPackgeInfo.clear();
    _listOfflineRewardPackageInfo.clear();
    _eventTimingPackage.Clear();
}

bool TimingPackageManager::init()
{
    setLoad();
    return true;
}

#pragma mark -Load & Save
void TimingPackageManager::setLoad()
{
    std::string txt = TableManager::getInstance()->getTableData(E_TABLE::TIMING_PACKAGE);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)(const char*)txt.c_str()).HasParseError();
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    _listTimingPackgeInfo.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        int nIdx = jsonValue["idx"].GetInt();
        int nType = jsonValue["package_type"].GetInt();
        int nCashIdx = jsonValue["cashidx"].GetInt();
        
        int nStartConditionType = jsonValue["start_type"].GetInt();
        int nStartConditionTypeParams = jsonValue.HasMember("start_type_params") == true ? jsonValue["start_type_params"].GetInt() : 0;
        int nStartValue = jsonValue["start_value"].GetInt();
        int nStartRange = jsonValue["start_range"].GetInt();
        
        int nEndConditionType = jsonValue["end_type"].GetInt();
        int nEndConditionTypeParams = jsonValue.HasMember("end_type_params") == true ? jsonValue["end_type_params"].GetInt() : 0;
        int nEndValue = jsonValue["end_value"].GetInt();
        
        std::string strNameKey = jsonValue["name_key"].GetString();
        std::string strDescKey = jsonValue["desc_key"].GetString();
        std::string strIconPath = jsonValue["icon"].GetString();
        std::string strBannerPath = jsonValue.HasMember("banner") == true ? jsonValue["banner"].GetString() : "";
        std::string strReward = jsonValue["reward"].GetString();
        
        Vector<InfoItems*> listItems = ItemsManager::getInstance()->getConvertArray(strReward);
        
        auto info = InfoTimingPackage::create();
        info->setIdx(nIdx);
        info->setType((E_TIMING_PACKAGE_TYPE)nType);
        info->setCashIdx(nCashIdx);
        info->setStartConditionType((E_TIMING_PACKAGE_CONDITION_TYPE)nStartConditionType);
        info->setStartConditionTypeParams(nStartConditionTypeParams);
        info->setStartValue(nStartValue);
        info->setStartRange(nStartRange);
        info->setEndConditionType((E_TIMING_PACKAGE_CONDITION_TYPE)nEndConditionType);
        info->setEndConditionTypeParams(nEndConditionTypeParams);
        info->setCompleteValue(nEndValue);
        info->setNameKey(strNameKey);
        info->setDescKey(strDescKey);
        info->setIconPath(strIconPath);
        info->setBannerPath(strBannerPath);
        info->setListItems(listItems);
        
        _listTimingPackgeInfo.pushBack(info);
    }
    loadTimingPackageFreeData();
}

void TimingPackageManager::saveTimingPackageFreeData()
{
    std::stringstream str_complete;
    
    str_complete.str("");
    auto list = getListInfoTimingPackage(E_TIMING_PACKAGE_TYPE::FREE);
    
    for(auto info : list)
    {
        str_complete << (int)info->isPurchase() <<",";
    }
    
    UserDefault::getInstance()->setStringForKey(KEY_TIMING_PACKAGE_FREE_COMPLETE, MafAes256::Encrypt(str_complete.str()));
}

void TimingPackageManager::loadTimingPackageFreeData()
{
    std::string str = "";
    auto list = getListInfoTimingPackage(E_TIMING_PACKAGE_TYPE::FREE);
    
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_TIMING_PACKAGE_FREE_COMPLETE);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        
        for(auto info : list)
        {
            auto offset = str.find(",");
            if(offset != std::string::npos)
            {
                auto result = str.substr(0,offset);
                int ret = std::atoi(result.c_str());
                info->setIsPurchase((bool)ret);
                str = str.substr(offset+1);
            }
            else
            {
                int ret = std::atoi(str.c_str());
                info->setIsPurchase((bool)ret);
                break;
            }
        }
    }
    
    saveTimingPackageFreeData();
}

#pragma mark -Manager : GET
cocos2d::Vector<InfoTimingPackage*> TimingPackageManager::getListInfoTimingPackageAll()
{
    return _listTimingPackgeInfo;
}

cocos2d::Vector<InfoTimingPackage*> TimingPackageManager::getListInfoTimingPackage(E_TIMING_PACKAGE_CONDITION_TYPE eType)
{
    Vector<InfoTimingPackage*> result;
    result.clear();
    
    for(auto info : _listTimingPackgeInfo)
    {
        if(info->getStartConditionType() == eType)
        {
            result.pushBack(info);
        }
    }
    
    return result;
}

cocos2d::Vector<InfoTimingPackage*> TimingPackageManager::getListInfoTimingPackage(E_TIMING_PACKAGE_TYPE eType)
{
    Vector<InfoTimingPackage*> result;
    result.clear();
    
    for(auto info : _listTimingPackgeInfo)
    {
        if(info->getType() == eType)
        {
            result.pushBack(info);
        }
    }
    
    return result;
}

InfoTimingPackage* TimingPackageManager::getInfoTimingPackage(E_TIMING_PACKAGE_CONDITION_TYPE eType, int conditionValue, bool isEndValue)
{
    InfoTimingPackage* result = nullptr;
    
    for(auto info : _listTimingPackgeInfo)
    {
        int infoConditionValue = info->getStartValue();
        
        if(isEndValue)
            infoConditionValue = info->getCompleteValue();
        
        if(info->getStartConditionType() == eType && conditionValue == infoConditionValue)
        {
            result = info;
            
            break;
        }
    }
    
    return result;
}

InfoTimingPackage* TimingPackageManager::getInfoTimingPackage(int nIdx)
{
    InfoTimingPackage* result = nullptr;
    
    for(auto info : _listTimingPackgeInfo)
    {
        
        if(info->getIdx() == nIdx)
        {
            result = info;
            
            break;
        }
    }
    
    return result;
}

cocos2d::Vector<InfoTimingPackage*> TimingPackageManager::getListInfoTimingPackageStartAll()
{
    cocos2d::Vector<InfoTimingPackage*> result;
    
    for(auto info : _listTimingPackgeInfo)
    {
        if(!info->isPurchase())
        {
            if(isStartTiming(info))
                result.pushBack(info);
        }
    }
    
    return result;
}

cocos2d::Vector<InfoTimingPackage*> TimingPackageManager::getListInfoTimingPackageStart(E_TIMING_PACKAGE_CONDITION_TYPE eType)
{
    cocos2d::Vector<InfoTimingPackage*> result;
    
    for(auto info : _listTimingPackgeInfo)
    {
        if(!info->isPurchase() && info->getStartConditionType() == eType)
        {
            if(isStartTiming(info))
                result.pushBack(info);
        }
    }
    
    return result;
}

cocos2d::Vector<InfoTimingPackage*> TimingPackageManager::getListInfoTimingPackageEnd(E_TIMING_PACKAGE_CONDITION_TYPE eType)
{
    cocos2d::Vector<InfoTimingPackage*> result;
    
    for(auto info : _listTimingPackgeInfo)
    {
        if(!info->isPurchase() && info->getStartConditionType() == eType)
        {
            if(isEndTiming(info))
                result.pushBack(info);
        }
    }
    
    return result;
}

cocos2d::Vector<InfoTimingPackage*> TimingPackageManager::getListInfoTimingPackageRewardAll()
{
    cocos2d::Vector<InfoTimingPackage*> result;
    
    for(auto info : _listTimingPackgeInfo)
    {
        if(!info->isPurchase())
        {
            if(isRewardTiming(info))
                result.pushBack(info);
        }
    }
    
    return result;
}

cocos2d::Vector<InfoTimingPackage*> TimingPackageManager::getListInfoTimingPackageReward(E_TIMING_PACKAGE_CONDITION_TYPE eType)
{
    cocos2d::Vector<InfoTimingPackage*> result;
    
    for(auto info : _listTimingPackgeInfo)
    {
        if(!info->isPurchase() && info->getStartConditionType() == eType)
        {
            if(isRewardTiming(info))
                result.pushBack(info);
        }
    }
    
    return result;
}

#pragma mark -Offline
cocos2d::Vector<InfoTimingPackage*> TimingPackageManager::getOfflineRewardPackageInfo()
{
    return _listOfflineRewardPackageInfo;
}

void TimingPackageManager::checkOfflineRewardInfo(int nFloor)
{
    //HghFloor
    int nHighFloor = UserInfoManager::getInstance()->getHighFloor(DG_NORMAL);
    auto list = getListInfoTimingPackage(E_TIMING_PACKAGE_CONDITION_TYPE::HIGH_FLOOR);
    for(int i = nHighFloor; i <= nFloor; ++i)
    {
        for(auto obj : list)
        {
            if(isRewardTiming(obj, i) && obj->getType() != E_TIMING_PACKAGE_TYPE::PURCHASE && obj->isPurchase() == false)
            {
                auto iter = _listOfflineRewardPackageInfo.find(obj);
                if(iter == _listOfflineRewardPackageInfo.end())
                {
                    _listOfflineRewardPackageInfo.pushBack(obj);
                }
            }
        }
    }
    
    _listOfflineRewardPackageInfo.reverse();
}

void TimingPackageManager::clearOfflineRewardList()
{
    _listOfflineRewardPackageInfo.clear();
}

cocos2d::Vector<InfoTimingPackage*> TimingPackageManager::getOfflineStartPackageInfo()
{
    return _listOfflineStartPackageInfo;
}

void TimingPackageManager::checkOfflineStartInfo(int nFloor)
{
    //HghFloor
    int nHighFloor = UserInfoManager::getInstance()->getHighFloor(DG_NORMAL);
    auto list = getListInfoTimingPackage(E_TIMING_PACKAGE_CONDITION_TYPE::HIGH_FLOOR);
    for(int i = nHighFloor; i <= nFloor; ++i)
    {
        for(auto obj : list)
        {
            if(isStartTiming(obj, i) && obj->getType() == E_TIMING_PACKAGE_TYPE::PURCHASE && obj->isPurchase() == false)
            {
                auto iter = _listOfflineStartPackageInfo.find(obj);
                if(iter == _listOfflineStartPackageInfo.end())
                {
                    _listOfflineStartPackageInfo.pushBack(obj);
                }
            }
        }
    }
    
    _listOfflineStartPackageInfo.reverse();
}

void TimingPackageManager::clearOfflineStartList()
{
    _listOfflineStartPackageInfo.clear();
}

#pragma mark -Utills
bool TimingPackageManager::isStartTiming(InfoTimingPackage* info)
{
    bool result = false;
    
    switch (info->getStartConditionType())
    {
        case E_TIMING_PACKAGE_CONDITION_TYPE::HIGH_FLOOR:
        {
            result = isStartTiming(info, UserInfoManager::getInstance()->getHighFloor(DG_NORMAL));
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::PRISON_ENTER:
        {
            result = isStartTiming(info, PrisonManager::getInstance()->getDungeonCount());
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::PRISON_FLOOR:
        {
            result = isStartTiming(info, SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_PRISON_METER));
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_ATK_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_ATK, info->getStartConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                result = isStartTiming(info, UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CRI_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_CRI, info->getStartConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                result = isStartTiming(info, UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_QUEST_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_QUEST, info->getStartConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                result = isStartTiming(info, UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_KILL_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_KILL, info->getStartConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                result = isStartTiming(info, UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CROSS_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_CROSS, info->getStartConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                result = isStartTiming(info, UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_ATK_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_ATK);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getStartConditionTypeParams() )
            {
                result = isStartTiming(info, objArtifact->getCurLv());
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CRI_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CRI);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getStartConditionTypeParams() )
            {
                result = isStartTiming(info, objArtifact->getCurLv());
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_QUEST_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_QUEST);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getStartConditionTypeParams() )
            {
                result = isStartTiming(info, objArtifact->getCurLv());
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_KILL_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_KILL);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getStartConditionTypeParams() )
            {
                result = isStartTiming(info, objArtifact->getCurLv());
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CROSS_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getStartConditionTypeParams() )
            {
                result = isStartTiming(info, objArtifact->getCurLv());
            }
            break;
        }
        
        default:
            break;
    }
    
    return result;
}

bool TimingPackageManager::isStartTiming(InfoTimingPackage* info, int nValue)
{
    bool result = false;
    
    switch (info->getStartConditionType())
    {
        case E_TIMING_PACKAGE_CONDITION_TYPE::HIGH_FLOOR:
        case E_TIMING_PACKAGE_CONDITION_TYPE::PRISON_FLOOR:
        
        {
            if(nValue >= info->getStartValue() &&
               nValue < info->getStartRange())
            {
                result = true;
            }
            
            if(info->getType() == E_TIMING_PACKAGE_TYPE::PURCHASE && info->getEndTimer() > 0)
            {
                if(nValue >= info->getStartValue())
                    result = true;
            }
            break;
        }
            
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_ATK_EVOL:
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CRI_EVOL:
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_KILL_EVOL:
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_QUEST_EVOL:
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CROSS_EVOL:
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_ATK_EVOL_BEYOND:
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CRI_EVOL_BEYOND:
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_QUEST_EVOL_BEYOND:
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_KILL_EVOL_BEYOND:
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CROSS_EVOL_BEYOND:
        {
            if ( info->getType() == E_TIMING_PACKAGE_TYPE::PURCHASE )
            {
                if ( nValue >= info->getStartValue() )
                {
                    result = true;
                }
            }
            else
            {
                if ( nValue >= info->getStartValue() && nValue < info->getStartRange() )
                {
                    result = true;
                }
            }
            
            if ( info->getCompleteValue() != 0 && nValue >= info->getCompleteValue() )
            {
                result = false;
            }
            
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::PRISON_ENTER:
        {
            if(nValue == info->getStartValue())
            {
                result = true;
            }
            
            if(info->getType() == E_TIMING_PACKAGE_TYPE::PURCHASE && info->getEndTimer() > 0)
            {
                if(nValue >= info->getStartValue())
                    result = true;
            }
            break;
        }
            
            
        default:
            break;
    }
    
    if(info->isPurchase() || info->isBtnActive())
        result = false;
    
    if(info->getType() == E_TIMING_PACKAGE_TYPE::PURCHASE && info->isActivate())
    {
        if(info->getEndTimer() <= 0)
            result = false;
    }
    
    return result;
}

bool TimingPackageManager::isEndTiming(InfoTimingPackage* info)
{
    bool result = false;
    switch (info->getEndConditionType())
    {
        case E_TIMING_PACKAGE_CONDITION_TYPE::HIGH_FLOOR:
        {
            result = isEndTiming(info, UserInfoManager::getInstance()->getHighFloor(DG_NORMAL));
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::PRISON_ENTER:
        {
            result = isEndTiming(info, PrisonManager::getInstance()->getDungeonCount());
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::PRISON_FLOOR:
        {
            result = isEndTiming(info, SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_PRISON_METER));
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_ATK_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_ATK, info->getEndConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                result = isEndTiming(info, UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CRI_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_CRI, info->getEndConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                result = isEndTiming(info, UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_QUEST_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_QUEST, info->getEndConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                result = isEndTiming(info, UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_KILL_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_KILL, info->getEndConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                result = isEndTiming(info, UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CROSS_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_CROSS, info->getEndConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                result = isEndTiming(info, UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_ATK_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_ATK);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getEndConditionTypeParams() )
            {
                result = isEndTiming(info, objArtifact->getCurLv());
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CRI_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CRI);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getEndConditionTypeParams() )
            {
                result = isEndTiming(info, objArtifact->getCurLv());
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_QUEST_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_QUEST);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getEndConditionTypeParams() )
            {
                result = isEndTiming(info, objArtifact->getCurLv());
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_KILL_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_KILL);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getEndConditionTypeParams() )
            {
                result = isEndTiming(info, objArtifact->getCurLv());
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CROSS_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getEndConditionTypeParams() )
            {
                result = isEndTiming(info, objArtifact->getCurLv());
            }
            break;
        }
            
        default:
            break;
    }
    return result;
}

bool TimingPackageManager::isEndTiming(InfoTimingPackage* info, int nValue)
{
    bool result = false;
 
    switch (info->getType())
    {
        case E_TIMING_PACKAGE_TYPE::FREE:
        case E_TIMING_PACKAGE_TYPE::TERMS_FREE:
        {
            if(nValue >= info->getCompleteValue())
            {
                result = true;
            }
        }
            break;
        case E_TIMING_PACKAGE_TYPE::PURCHASE:
        {
            if (info->getEndConditionType() == E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_ATK_EVOL ||
                info->getEndConditionType() == E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CRI_EVOL ||
                info->getEndConditionType() == E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_KILL_EVOL ||
                info->getEndConditionType() == E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_QUEST_EVOL ||
                info->getEndConditionType() == E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CROSS_EVOL ||
                info->getEndConditionType() == E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_ATK_EVOL_BEYOND ||
                info->getEndConditionType() == E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CRI_EVOL_BEYOND ||
                info->getEndConditionType() == E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_QUEST_EVOL_BEYOND ||
                info->getEndConditionType() == E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_KILL_EVOL_BEYOND ||
                info->getEndConditionType() == E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CROSS_EVOL_BEYOND
                )
            {
                if ( info->getEndTimer() <= 0 )
                {
                    result = true;
                }
                
                if ( info->getCompleteValue() != 0 && nValue >= info->getCompleteValue() )
                {
                    result = true;
                }
            }
            else
            {
                if ( info->getEndTimer() <= 0 )
                {
                    result = true;
                }
            }
            break;
        }
            
        default:
            break;
    }
    
    if(info->isPurchase())
        result = true;
    
    return result;
}

bool TimingPackageManager::isRewardTiming(InfoTimingPackage* info)
{
    bool result = false;
    switch (info->getEndConditionType())
    {
        case E_TIMING_PACKAGE_CONDITION_TYPE::HIGH_FLOOR:
        {
            result = isRewardTiming(info, UserInfoManager::getInstance()->getHighFloor(DG_NORMAL));
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::PRISON_ENTER:
        {
            result = isRewardTiming(info, PrisonManager::getInstance()->getDungeonCount());
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::PRISON_FLOOR:
        {
            result = isRewardTiming(info, SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_PRISON_METER));
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_ATK_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_ATK, info->getEndConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                result = isRewardTiming(info, UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CRI_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_CRI, info->getEndConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                result = isRewardTiming(info, UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_QUEST_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_QUEST, info->getEndConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                result = isRewardTiming(info, UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_KILL_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_KILL, info->getEndConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                result = isRewardTiming(info, UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CROSS_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_CROSS, info->getEndConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                result = isRewardTiming(info, UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_ATK_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_ATK);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getEndConditionTypeParams() )
            {
                result = isRewardTiming(info, objArtifact->getCurLv());
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CRI_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CRI);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getEndConditionTypeParams() )
            {
                result = isRewardTiming(info, objArtifact->getCurLv());
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_QUEST_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_QUEST);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getEndConditionTypeParams() )
            {
                result = isRewardTiming(info, objArtifact->getCurLv());
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_KILL_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_KILL);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getEndConditionTypeParams() )
            {
                result = isRewardTiming(info, objArtifact->getCurLv());
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CROSS_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getEndConditionTypeParams() )
            {
                result = isRewardTiming(info, objArtifact->getCurLv());
            }
            break;
        }
            
        default:
            break;
    }
    return result;
}

bool TimingPackageManager::isRewardTiming(InfoTimingPackage* info, int nValue)
{
    bool result = false;
    
 
    switch (info->getType())
    {
        case E_TIMING_PACKAGE_TYPE::FREE:
        case E_TIMING_PACKAGE_TYPE::TERMS_FREE:
        {
            if(info->isActivate() && nValue >= info->getCompleteValue())
            {
                result = true;
            }
        }
            break;
        default:
            break;
    }
    
    if(info->isPurchase())
        result = false;
    
    return result;
}
#pragma mark -Manager : Utills

void TimingPackageManager::requestReward(InfoTimingPackage* info, const std::function<void(bool, int)>& pCallback, bool isPopupScaedule)
{
    _callbackReward = pCallback;
    
    switch (info->getType()) {
        case E_TIMING_PACKAGE_TYPE::FREE:
        {
            info->setIsPurchase(true);
            ItemsManager::getInstance()->addItems(info->getListItems());
            if(_callbackReward != nullptr)
            {
                _callbackReward(true, 1);
                _callbackReward = nullptr;
            }
            MafAnalyticsManager::LogEvent(MafUtils::format(kAnalTimingPackageWelcome, info->getIdx()).c_str(), kRepeatTrue);
            
        }
            break;
        case E_TIMING_PACKAGE_TYPE::TERMS_FREE:
        {
            if ( AccountManager::getInstance()->getUserIdx() != 0 )
            {
                PopupLoading::show();
                requestTimingPackageReward(pCallback, info, isPopupScaedule);
                return;
            }
        }
        case E_TIMING_PACKAGE_TYPE::PURCHASE:
        {
            
        }
        default:
            break;
    }
    
    auto popup = PopupTimingPackage::create(info, isPopupScaedule);
    popup->show();
    
    saveTimingPackageFreeData();
}

void TimingPackageManager::setArrangementInfo()
{
    for(auto info : _listTimingPackgeInfo)
    {
        info->setIsBtnActive(false);
    }
}
void TimingPackageManager::showTimingPackagePopup(InfoTimingPackage* info) const
{
    auto popup = PopupTimingPackage::create(info);
    popup->show();
}
#pragma mark -Manager : Network

void TimingPackageManager::requestTimingPackageInfo(const std::function<void(bool)>& pCallback)
{
    //
    _callbackInfo = pCallback;
    
    //
    std::string url = "/shop/timingPackage/list/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseTimingPackageInfo(response,dd);
    };
    req->send(cb);
}

void TimingPackageManager::responseTimingPackageInfo(cocos2d::network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
       
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
       
    if ( bError == true )
    {
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false);
            _callbackInfo = nullptr;
        }
        return;
    }
    
    auto lootData = jsonParser["_timingPackageList"].GetArray();
    
    for(int i = 0; i < lootData.Size(); ++i)
    {
        const rapidjson::Value& data = lootData[i];
        
        int idx = data["idx"].GetInt();
        int endTimer = data["end_time"].GetInt();
        bool isPurchase = data["is_purchase"].GetBool();
        
        auto info = getInfoTimingPackage(idx);
        
        if(info == nullptr)
            continue;
        
        info->setEndTimer((float)endTimer);
        info->setIsPurchase(isPurchase);
        if(info->isPurchase() == false)
            info->setIsActivate(true);
    }
    
    //
    if ( _callbackInfo != nullptr )
    {
        _callbackInfo(true);
        _callbackInfo = nullptr;
    }
    
}

void TimingPackageManager::requestTimingPackageActivate(const std::function<void(bool,int)>& pCallback, InfoTimingPackage* info)
{
    //
    std::string url = "/shop/timingPackage/activate";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_idx", info->getIdx());
    req->setData("_type", (int)info->getStartConditionType());
    req->setData("_cashidx", info->getCashIdx());
    switch (info->getStartConditionType())
    {
        case E_TIMING_PACKAGE_CONDITION_TYPE::HIGH_FLOOR:
        {
            req->setData("_value", UserInfoManager::getInstance()->getHighFloor(DG_NORMAL));
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::PRISON_ENTER:
        {
            req->setData("_value", PrisonManager::getInstance()->getDungeonCount());
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::PRISON_FLOOR:
        {
            req->setData("_value", SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_PRISON_METER));
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_ATK_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_ATK, info->getStartConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                req->setData("_value", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CRI_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_CRI, info->getStartConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                req->setData("_value", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_QUEST_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_QUEST, info->getStartConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                req->setData("_value", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_KILL_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_KILL, info->getStartConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                req->setData("_value", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CROSS_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_CROSS, info->getStartConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                req->setData("_value", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_ATK_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_ATK);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getStartConditionTypeParams() )
            {
                req->setData("_value", objArtifact->getCurLv());
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CRI_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CRI);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getStartConditionTypeParams() )
            {
                req->setData("_value", objArtifact->getCurLv());
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_QUEST_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_QUEST);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getStartConditionTypeParams() )
            {
                req->setData("_value", objArtifact->getCurLv());
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_KILL_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_KILL);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getStartConditionTypeParams() )
            {
                req->setData("_value", objArtifact->getCurLv());
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CROSS_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getStartConditionTypeParams() )
            {
                req->setData("_value", objArtifact->getCurLv());
            }
            break;
        }
            
        default:
            break;
    }
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseTimingPackageActivate(response, dd, pCallback, info);
    };
    req->send(cb);
}

void TimingPackageManager::responseTimingPackageActivate(cocos2d::network::HttpResponse* response,std::string &data, const std::function<void(bool,int)>& pCallback, InfoTimingPackage* info)
{
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
       
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
       
    if ( bError == true )
    {
        if ( pCallback != nullptr )
        {
            pCallback(false, -1);
        }
        return;
    }
    
    /*
     0 # 재화 부족
     1 # 성공
     3 # 레벨 정보 다름
     4 # max 레벨이거나 레벨정보가 없음
    */
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( pCallback != nullptr )
        {
            pCallback(false, nResult);
        }
        return;
    }
    
    MafAnalyticsManager::LogEvent(kAnalTimingPackageActivate, kAnalEventTimingPackageParameterIdx, std::to_string(info->getCashIdx()).c_str(), kRepeatTrue);
    
    auto lootData = jsonParser["_timingPackageList"].GetArray();
    
    for(int i = 0; i < lootData.Size(); ++i)
    {
        const rapidjson::Value& data = lootData[i];
        
        int idx = data["idx"].GetInt();
        int endTimer = data["end_time"].GetInt();
        bool isPurchase = data["is_purchase"].GetBool();
        
        auto info = getInfoTimingPackage(idx);
        
        if(info == nullptr)
            continue;
        
        info->setEndTimer(endTimer);
        info->setIsPurchase(isPurchase);
        if(info->isPurchase() == false)
            info->setIsActivate(true);
    }
    
    //
    if ( pCallback != nullptr )
    {
        pCallback(true, nResult);
    }
    
}

void TimingPackageManager::requestTimingPackageReward(const std::function<void(bool,int)>& pCallback, InfoTimingPackage* info, bool isPopupScaedule)
{
    //
    _callbackReward = pCallback;
    
    //
    std::string url = "/shop/timingPackage/free/reward";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_idx", info->getIdx());
    req->setData("_type", (int)info->getEndConditionType());
    req->setData("_cashidx", info->getCashIdx());
    switch (info->getEndConditionType())
    {
        case E_TIMING_PACKAGE_CONDITION_TYPE::HIGH_FLOOR:
        {
            req->setData("_value", UserInfoManager::getInstance()->getHighFloor(DG_NORMAL));
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::PRISON_ENTER:
        {
            req->setData("_value", PrisonManager::getInstance()->getDungeonCount());
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::PRISON_FLOOR:
        {
            req->setData("_value", SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_PRISON_METER));
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_ATK_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_ATK, info->getEndConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                req->setData("_value", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CRI_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_CRI, info->getEndConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                req->setData("_value", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_QUEST_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_QUEST, info->getEndConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                req->setData("_value", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_KILL_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_KILL, info->getEndConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                req->setData("_value", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CROSS_EVOL:
        {
            auto objArtifact = DataManager::GetArtifactInfo(E_ARTIFACT_TYPE::ARTI_TYPE_CROSS, info->getEndConditionTypeParams());
            if ( objArtifact != nullptr )
            {
                req->setData("_value", UserInfoManager::getInstance()->getArtifactLevel(E_PLACE::DG_NORMAL, objArtifact->getIdx()));
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_ATK_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_ATK);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getEndConditionTypeParams() )
            {
                req->setData("_value", objArtifact->getCurLv());
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CRI_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CRI);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getEndConditionTypeParams() )
            {
                req->setData("_value", objArtifact->getCurLv());
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_QUEST_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_QUEST);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getEndConditionTypeParams() )
            {
                req->setData("_value", objArtifact->getCurLv());
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_KILL_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_KILL);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getEndConditionTypeParams() )
            {
                req->setData("_value", objArtifact->getCurLv());
            }
            break;
        }
        case E_TIMING_PACKAGE_CONDITION_TYPE::ARTI_CROSS_EVOL_BEYOND:
        {
            auto objArtifact = InfiniteArtifactManager::getInstance()->getCurrentInfiniteArtiInfoFromType(INFINITE_ARTIFACT_TYPE::INFINITE_ARTI_TYPE_CROSS);
            if ( objArtifact != nullptr && objArtifact->getGrade() == info->getEndConditionTypeParams() )
            {
                req->setData("_value", objArtifact->getCurLv());
            }
            break;
        }
            
        default:
            break;
    }
    MafResponseNew cb = [&, info, isPopupScaedule](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseTimingPackageReward(response, dd, info, isPopupScaedule);
    };
    req->send(cb);
}

void TimingPackageManager::responseTimingPackageReward(cocos2d::network::HttpResponse* response,std::string &data, InfoTimingPackage* info, bool isPopupScaedule)
{
    PopupLoading::hide();
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
       
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
       
    if ( bError == true )
    {
        if ( _callbackReward != nullptr )
        {
            _callbackReward(false, -1);
            _callbackReward = nullptr;
        }
        return;
    }
    
    /*
     0 # 재화 부족
     1 # 성공
     3 # 레벨 정보 다름
     4 # max 레벨이거나 레벨정보가 없음
    */
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackReward != nullptr )
        {
            _callbackReward(false, nResult);
            _callbackReward = nullptr;
        }
        return;
    }
    
    auto lootData = jsonParser["_timingPackageList"].GetArray();
    
    for(int i = 0; i < lootData.Size(); ++i)
    {
        const rapidjson::Value& data = lootData[i];
        
        int idx = data["idx"].GetInt();
        int endTimer = data["end_time"].GetInt();
        bool isPurchase = data["is_purchase"].GetBool();
        
        auto info = getInfoTimingPackage(idx);
        
        if(info == nullptr)
            continue;
        
        info->setEndTimer(endTimer);
        info->setIsPurchase(isPurchase);
        if(info->isPurchase() == false)
            info->setIsActivate(true);
    }
    
    if(jsonParser.HasMember("_reward"))
    {
        std::string strReward = jsonParser["_reward"].GetString();
        auto listReward = ItemsManager::getInstance()->getConvertArray(strReward);
        ItemsManager::getInstance()->addItems(listReward);
    }
    
    if(jsonParser.HasMember("_items"))
    {
        std::string strItems = jsonParser["_items"].GetString();
        auto listItems = ItemsManager::getInstance()->getConvertArray(strItems);
        
        ItemsManager::getInstance()->setItemsServer(strItems);
    }
    
    auto popup = PopupTimingPackage::create(info, isPopupScaedule);
    popup->show();
    
    //
    if ( _callbackReward != nullptr )
    {
        _callbackReward(true, nResult);
        _callbackReward = nullptr;
    }
    
}

#pragma mark - Manager : delegate
void TimingPackageManager::subEventTimingPackage(MafFunction<void(cocos2d::ValueMap )>* func)
{
    _eventTimingPackage += func;
}
void TimingPackageManager::invokeEventTimingPackage(const cocos2d::ValueMap listData)
{
    _eventTimingPackage(listData);
}
void TimingPackageManager::unSubEventTimingPackage(void* obj)
{
    _eventTimingPackage.Clear(obj);
}
