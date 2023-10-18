//
//  AchievementManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#include "AchievementManager.h"

#include "ManagerGame/UserInfoManager.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/CostumeSkinManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/TownManager.h"

USING_NS_CC;

#pragma mark - ObjTable
ObjAchievement::ObjAchievement() :
_nIdx(0),
_nType(0),
_eNormalType(E_ACHIEVE_NORMAR_TYPE::ACHIEVE_NONE),
_nPrecedeIdx(0),

_eRewardType(E_ITEMS::NONE),
_nTypeParams(0),
_nRewardCount(0),

_eShortcuts(E_SHORTCUTS::SHORTCUT_NONE),

_strTextKey(""),

_nCurrentCount(0),
_nClearCount(0),
_nCountMax(0),
_nGrade(1),

_nRewardIdx(0),
_strRewardCount("0"),
_nRewardTypeParams(0),
_bComplete(false),
_bCurrent(false),
_bRewardCondition(false),

_eArtiType(E_ARTIFACT_TYPE::ARTI_TYPE_NONE),
_nArtiGrade(0),
_eArtifact(E_ARTIFACT::E_ARTIFACT_999_NONE)
{

}

ObjAchievement::~ObjAchievement()
{
    
}

bool ObjAchievement::init()
{
    return true;
}

#pragma mark - ObjAchievement : set, get

int ObjAchievement::getIdx()
{
    return _nIdx;
}

void ObjAchievement::setIdx(int value)
{
    _nIdx = value;
}

int ObjAchievement::getType()
{
    return _nType;
}

void ObjAchievement::setType(int value)
{
    _nType = value;
}

E_ACHIEVE_NORMAR_TYPE ObjAchievement::getNormalType()
{
    return _eNormalType;
}

void ObjAchievement::setNormalType(E_ACHIEVE_NORMAR_TYPE value)
{
    _eNormalType = value;
}

E_NORMAL_ACHIEVE_MISSION ObjAchievement::getNormalMission()
{
    return _eNormalMission;
}

void ObjAchievement::setNormalMission(E_NORMAL_ACHIEVE_MISSION value)
{
    _eNormalMission = value;
}

int ObjAchievement::getPrecedeIdx()
{
    return _nPrecedeIdx;
}

void ObjAchievement::setPrecedeIdx(int value)
{
    _nPrecedeIdx = value;
}

InfoItems* ObjAchievement::getReward()
{
    InfoItems* objItems = ItemsManager::getInstance()->getInfoItems(_nRewardIdx);
    if ( objItems == nullptr )
    {
        return objItems;
    }
    
    objItems->setCount(_strRewardCount);
    objItems->setTypeParams(_nRewardTypeParams);
    
    return objItems;
}

void ObjAchievement::setRewardIdx(int nIdx)
{
    _nRewardIdx = nIdx;
}

void ObjAchievement::setRewardCount(std::string strCount)
{
    _strRewardCount = strCount;
}

void ObjAchievement::setRewardTypeParams(int nTypeParams)
{
    _nRewardTypeParams = nTypeParams;
}

InfoItems* ObjAchievement::getItem()
{
    auto item = ItemsManager::getInstance()->getInfoItems(_eRewardType);
        
    item->setCount(_nRewardCount);
    item->setTypeParams(_nTypeParams);
    
    return item;
}

void ObjAchievement::setRewardType(int value)
{
    _eRewardType = (E_ITEMS)value;
}

void ObjAchievement::setTypeParams(int value)
{
    _nTypeParams = value;
}

void ObjAchievement::setRewardCount(int value)
{
    _nRewardCount = value;
}

E_SHORTCUTS ObjAchievement::getShortcuts()
{
    return _eShortcuts;
}

void ObjAchievement::setShortcuts(E_SHORTCUTS value)
{
    _eShortcuts = value;
}

std::string ObjAchievement::getText()
{
    return GAME_TEXT(_strTextKey);
}

std::string ObjAchievement::getTextKey()
{
    return _strTextKey;
}

void ObjAchievement::setTextKey(std::string value)
{
    _strTextKey = value;
}

int ObjAchievement::getCount()
{
    return _nCurrentCount;
}

void ObjAchievement::setCount(int value)
{
    _nCurrentCount = value;
}

int ObjAchievement::getClearCount()
{
    return _nClearCount;
}

void ObjAchievement::setClearCount(int value)
{
    _nClearCount = value;
}

int ObjAchievement::getGradeClearCount()
{
    return _nGrade * _nClearCount;
}

int ObjAchievement::getGradeCurrentCount()
{
    int result = _nCurrentCount;
    
    if(result >= getGradeClearCount())
        result = getGradeClearCount();
    
    return result;
}

int ObjAchievement::getCountMax()
{
    return _nCountMax;
}

void ObjAchievement::setCountMax(int value)
{
    _nCountMax = value;
}

int ObjAchievement::getGrade()
{
    return _nGrade;
}

void ObjAchievement::setGrade(int value)
{
    _nGrade = value;
    
    if(_nGrade >= getMaxGrade())
        _nGrade = getMaxGrade();
    
}

void ObjAchievement::addGrade()
{
    _nGrade++;
}

int ObjAchievement::getMaxGrade()
{
    return _nCountMax / _nClearCount;
}

bool ObjAchievement::isComplete()
{
    return _bComplete;
}

void ObjAchievement::setComplete(bool value)
{
    _bComplete = value;
}

bool ObjAchievement::isCurrent()
{
    return _bCurrent;
}

void ObjAchievement::setCurrent(bool value)
{
    _bCurrent = value;
}

bool ObjAchievement::isRewardCondition()
{
    return _bRewardCondition;
}

void ObjAchievement::setRewardCondition(bool value)
{
    _bRewardCondition = value;
}

E_ARTIFACT_TYPE ObjAchievement::getArtifactType()
{
    return _eArtiType;
}

void ObjAchievement::setArtifactType(E_ARTIFACT_TYPE value)
{
    _eArtiType = value;
}

int ObjAchievement::getArtifactGrade()
{
    return _nArtiGrade;
}

void ObjAchievement::setArtifactGrade(int value)
{
    _nArtiGrade = value;
}

E_ARTIFACT ObjAchievement::getArtifactIdx()
{
    return _eArtifact;
}

void ObjAchievement::setArtifactIdx(E_ARTIFACT value)
{
    _eArtifact = value;
}

#pragma mark - AchievementManager
AchievementManager::AchievementManager(void) :
_callbackInfo(nullptr),
_callbackReward(nullptr),
_callbackComplete(nullptr),
_callbackRedraw(nullptr)
{
    
}

AchievementManager::~AchievementManager(void)
{
    
}

bool AchievementManager::init()
{
    setLoadPrison();
    setLoadNormal();
    
    return true;
}

void AchievementManager::setLoadPrison()
{
    _listAchievement.clear();
    
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::ACHIEVEMENTS);
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == false )
    {
        for ( int i = 0; i < jsonParser.Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser[i];
            
            int nType = jsonValue["type"].GetInt();
            int nIdx = jsonValue["idx"].GetInt();
            std::string strTextKey = jsonValue["text_key"].GetString();
            int nCountMax = jsonValue["count"].GetInt();
            
            
            int nRewardIdx = jsonValue["reward_type"].GetInt();
            std::string strRewardCount = "0";
            if ( jsonValue["reward_count"].IsString() == true )
            {
                strRewardCount = jsonValue["reward_count"].GetString();
            }
            else
            {
                int64_t nCount = jsonValue["reward_count"].GetInt64();
                strRewardCount = MafUtils::toString(nCount);
            }
            
            int nRewardTypeParams = 0;
            if ( jsonValue.HasMember("reward_type_params") == true )
            {
                nRewardTypeParams = jsonValue["reward_type_params"].GetInt();
            }
            
            
            
            auto obj = ObjAchievement::create();
            obj->setType(nType);
            obj->setIdx(nIdx);
            obj->setTextKey(strTextKey);
            
            obj->setCount(0);
            obj->setCountMax(nCountMax);
            
            obj->setRewardIdx(nRewardIdx);
            obj->setRewardCount(strRewardCount);
            obj->setRewardTypeParams(nRewardTypeParams);
            
            obj->setComplete(false);
            
            
            //
            _listAchievement.pushBack(obj);
        }
    }
}

void AchievementManager::setLoadNormal()
{
    _vecNormalAchieveItemAll.clear();
    _listAchievementNormal.clear();
    
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::ACHIEVEMENTS_NORMAL);
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)strText.c_str());
    
    int artiGrade = 0;
    if( jsonParser.HasParseError() == false )
    {
        for ( int i = 0; i < jsonParser.Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser[i];
            
            auto idx = jsonValue["idx"].GetInt();
            auto achieve_type = jsonValue["achieve_type"].GetInt();
            auto precede_idx = jsonValue["precede_idx"].GetInt();
            auto reward_idx = jsonValue["reward_idx"].GetInt();
            auto type_params = jsonValue["type_params"].GetInt();
            auto reward_count = jsonValue["reward_count"].GetInt();
            auto shortcuts = jsonValue["shortcuts"].GetInt();
            auto content = jsonValue["content"].GetString();
            auto count_clear = jsonValue["count_clear"].GetInt();
            auto count_max = jsonValue["count_max"].GetInt();
            auto missionType = (E_NORMAL_ACHIEVE_MISSION) idx;
            
            
            auto obj = ObjAchievement::create();
            
            obj->setIdx(idx);
            obj->setNormalType((E_ACHIEVE_NORMAR_TYPE)achieve_type);
            obj->setPrecedeIdx(precede_idx);
            obj->setRewardType(reward_idx);
            obj->setTypeParams(type_params);
            obj->setRewardCount(reward_count);
            obj->setShortcuts((E_SHORTCUTS)shortcuts);
            obj->setTextKey(content);
            obj->setClearCount(count_clear);
            obj->setCountMax(count_max);
            obj->setNormalMission(missionType);
            
            if(missionType >= E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_ATK_1 && missionType <= E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_KILL_15)
            {
                
                if(missionType >= E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_ATK_1 && missionType <= E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_ATK_15)
                {
                    obj->setArtifactType(E_ARTIFACT_TYPE::ARTI_TYPE_ATK);
                }
                else if(missionType >= E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_CRI_1 && missionType <= E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_CRI_15)
                {
                    obj->setArtifactType(E_ARTIFACT_TYPE::ARTI_TYPE_CRI);
                }
                else if(missionType >= E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_QUEST_1 && missionType <= E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_QUEST_15)
                {
                    obj->setArtifactType(E_ARTIFACT_TYPE::ARTI_TYPE_QUEST);
                }
                else if(missionType >= E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_KILL_1 && missionType <= E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_KILL_15)
                {
                    obj->setArtifactType(E_ARTIFACT_TYPE::ARTI_TYPE_KILL);
                }
                
                if(missionType == E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_ATK_1 ||
                   missionType == E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_CRI_1 ||
                   missionType == E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_QUEST_1 ||
                   missionType == E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_KILL_1)
                {
                    artiGrade = 1;
                }
                
                obj->setArtifactGrade(artiGrade);
                artiGrade++;
                
                auto artiInfo = DataManager::GetArtifactInfo(obj->getArtifactType(), obj->getArtifactGrade());
                obj->setArtifactIdx(artiInfo->getIdx());
                
            }
            
            
            obj->setCurrent(false);
            obj->setComplete(false);
            
            //
            _listAchievementNormal.pushBack(obj);
        }
    }
    
    loadNormalAchieveData();
    initNormalAchieveCount();
}

void AchievementManager::saveNormalAchieveData()
{
    std::stringstream str_count;
    std::stringstream str_complete;
    std::stringstream str_grade;
    
    str_count.str("");
    str_complete.str("");
    str_grade.str("");
    for(auto info : _listAchievementNormal)
    {
        str_count << info->getCount() <<",";
        str_complete << (int)info->isComplete() <<",";
        str_grade << info->getGrade() <<",";
    }
    UserDefault::getInstance()->setStringForKey(KEY_ACHIEVE_NORMAL_COUNT, MafAes256::Encrypt(str_count.str()));
    
    UserDefault::getInstance()->setStringForKey(KEY_ACHIEVE_NORMAL_COMPLETE, MafAes256::Encrypt(str_complete.str()));
    
    UserDefault::getInstance()->setStringForKey(KEY_ACHIEVE_NORMAL_GRADE, MafAes256::Encrypt(str_grade.str()));
}

void AchievementManager::loadNormalAchieveData()
{
    std::string str = "";
    
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_ACHIEVE_NORMAL_COUNT);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        
        for(auto info : _listAchievementNormal)
        {
            auto offset = str.find(",");
            if(offset != std::string::npos)
            {
                auto result = str.substr(0,offset);
                int ret = std::atoi(result.c_str());
                info->setCount(ret);
                str = str.substr(offset+1);
            }
            else
            {
                int ret = std::atoi(str.c_str());
                info->setCount(ret);
                break;
            }
        }
    }
    
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_ACHIEVE_NORMAL_COMPLETE);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        
        for(auto info : _listAchievementNormal)
        {
            auto offset = str.find(",");
            if(offset != std::string::npos)
            {
                auto result = str.substr(0,offset);
                int ret = std::atoi(result.c_str());
                info->setComplete((bool)ret);
                str = str.substr(offset+1);
            }
            else
            {
                int ret = std::atoi(str.c_str());
                info->setComplete((bool)ret);
                break;
            }
        }
    }
    
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_ACHIEVE_NORMAL_GRADE);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        
        for(auto info : _listAchievementNormal)
        {
            auto offset = str.find(",");
            if(offset != std::string::npos)
            {
                auto result = str.substr(0,offset);
                int ret = std::atoi(result.c_str());
                info->setGrade(ret);
                str = str.substr(offset+1);
            }
            else
            {
                int ret = std::atoi(str.c_str());
                info->setGrade(1);
                break;
            }
            if(info->getGrade() <= 0)
            {
                info->setGrade(1);
            }
        }
    }

    updateNormalAchieveCurrent();
    updateNormalAchieveIsReward();
    saveNormalAchieveData();
}

void AchievementManager::updateNormalAchieveCurrent()
{
    auto vecArtiAchieve = getAchievementNormalListFromType(E_ACHIEVE_NORMAR_TYPE::ACHIEVE_ARTIFACT, true);
    
    for(auto obj : vecArtiAchieve)
    {
        obj->setCurrent(false);
    }
    
    for(auto obj : vecArtiAchieve)
    {
        if(obj->getPrecedeIdx() == 0)
        {
            if(!obj->isComplete())
            {
                obj->setCurrent(true);
                continue;
            }
        }
        
        auto preObj = getObjAchieveNormal(obj->getPrecedeIdx());
        if(preObj == nullptr)
            continue;
        
        if(obj->isComplete())
        {
            auto nextObj = getObjAchieveNormal(obj->getIdx()+1);
            if(nextObj == nullptr)
                continue;
            
            if(nextObj->getPrecedeIdx() == 0)
            {
                obj->setCurrent(true);
            }
            
            continue;
        }
        
        if(preObj->isComplete())
            obj->setCurrent(true);
    }
    
}

void AchievementManager::updateNormalAchieveIsReward()
{
    for(auto obj : _listAchievementNormal)
    {
        if(obj->getGradeCurrentCount() >= obj->getGradeClearCount())
            obj->setRewardCondition(true);
    }
}

void AchievementManager::updateNormalAchieveIsReward(ObjAchievement* obj)
{
    if(obj->getGradeCurrentCount() >= obj->getGradeClearCount())
        obj->setRewardCondition(true);
}

#pragma mark - set, get, add : Prison
cocos2d::Vector<ObjAchievement*> AchievementManager::getList(int nType)
{
    cocos2d::Vector<ObjAchievement*> listResult;
    for ( auto& obj : _listAchievement )
    {
        if ( obj->getType() == nType )
        {
            listResult.pushBack(obj);
        }
    }
        
    return listResult;
}

void AchievementManager::setListConvert(int nType, std::string strList)
{
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strList.c_str());
    if(jsonParser.HasParseError())
    {
        return;
    }
    
    cocos2d::Vector<ObjAchievement*> list;
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        int nIdx = jsonValue["idx"].GetInt();
        int nCount = jsonValue["count"].GetInt();
        int nCountMax = jsonValue["max_count"].GetInt();
        bool bComplete = jsonValue["is_complete"].GetBool();
        
        //
        for ( auto& obj : _listAchievement )
        {
            if ( obj->getType() == nType && obj->getIdx() == nIdx )
            {
                obj->setCount(nCount);
                obj->setCountMax(nCountMax);
                obj->setComplete(bComplete);
            }
        }
    }
}


int AchievementManager::getCompleteRate(int nType)
{
    int nResult = 0;
    
    int nTotal = 0;
    int nComplete = 0;
    for ( auto& obj : _listAchievement )
    {
        if ( obj->getType() == nType )
        {
            nTotal++;
            if ( obj->isComplete() == true )
            {
                nComplete++;
            }
        }
    }
    
    nResult = (double)nComplete / nTotal * 100;
    if ( nResult > 100 )
    {
        nResult = 100;
    }
    
    return nResult;
}

bool AchievementManager::isReward(int nType)
{
    int nResult = 0;
    for ( auto& obj : _listAchievement )
    {
        if ( obj->getType() == nType )
        {
            if ( obj->getCount() >= obj->getCountMax() && obj->isComplete() == false )
            {
                nResult++;
            }
        }
    }
    
    return nResult;
}

bool AchievementManager::isCompleteReward(int nType)
{
    bool bResult = false;
    
    auto iter = _listAchievementComplete.find(nType);
    if ( iter != _listAchievementComplete.end() )
    {
        bResult = iter->second;
    }
    
    return bResult;
}

#pragma mark - get, set, add : Normal
void AchievementManager::initNormalAchieveCount()
{
    for(auto obj : _listAchievementNormal)
    {
        initNormalAchieveCount(obj);
    }
    updateNormalAchieveIsReward();
}

void AchievementManager::initNormalAchieveCount(ObjAchievement* obj)
{
    if(obj == nullptr)
        return;
    
    int value = 0;
    
    auto userInfo = UserInfoManager::getInstance();
    auto costumeMgr = CostumeManager::getInstance();
    
    switch (obj->getNormalMission())
    {
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_PET_GET:
        {
            for(int i = 1; i<E_PET::D_1_PENGUIN; ++i)
            {
                if(PetNewManager::getInstance()->getPet((E_PET)i))
                    value++;
            }
        }break;
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_PET_LEVELUP:
        {
            for(int i = 1; i<E_PET::D_1_PENGUIN; ++i)
            {
                value += PetNewManager::getInstance()->getPetLevel((E_PET)i);
            }
        }break;
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_PET_EVOLUTION:
        {
            for(int i = 1; i<E_PET::D_1_PENGUIN; ++i)
            {
                value += PetNewManager::getInstance()->getPetEvolution((E_PET)i);
            }
        }break;
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_PET_ENHANCE:
        {
            for(int i = 1; i<E_PET::D_1_PENGUIN; ++i)
            {
                value += PetNewManager::getInstance()->getPetReinforce((E_PET)i);
            }
        }break;
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_PET_EQUIP_COLLECT_LEVEL:
        {
            value += PetNewManager::getInstance()->getPetEquipCollectLevelAverage();
        }break;
            
            //Costume
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_COSTUME_BUY_HEAD:
        {
            value += costumeMgr->getExistTypeCount(E_COSTUME::HEAD);
        }break;
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_COSTUME_BUY_BODY:
        {
            value += costumeMgr->getExistTypeCount(E_COSTUME::BODY);
        }break;
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_COSTUME_BUY_CAPE:
        {
            value += costumeMgr->getExistTypeCount(E_COSTUME::CAPE);
        }break;
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_COSTUME_BUY_HAND:
        {
            value += costumeMgr->getExistTypeCount(E_COSTUME::HAND);
        }break;
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_COSTUME_BUY_FOOT:
        {
            value += costumeMgr->getExistTypeCount(E_COSTUME::FOOT);
        }break;
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_COSTUME_BUY_SETITEM:
        {
            value += costumeMgr->getExistTypeCount(E_COSTUME::SETITEM);
        }break;
            
            //Artifact
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_ATK_1:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_ATK_2:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_ATK_3:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_ATK_4:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_ATK_5:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_ATK_6:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_ATK_7:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_ATK_8:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_ATK_9:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_ATK_10:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_ATK_11:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_ATK_12:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_ATK_13:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_ATK_14:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_ATK_15:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_CRI_1:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_CRI_2:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_CRI_3:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_CRI_4:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_CRI_5:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_CRI_6:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_CRI_7:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_CRI_8:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_CRI_9:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_CRI_10:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_CRI_11:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_CRI_12:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_CRI_13:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_CRI_14:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_CRI_15:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_QUEST_1:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_QUEST_2:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_QUEST_3:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_QUEST_4:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_QUEST_5:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_QUEST_6:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_QUEST_7:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_QUEST_8:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_QUEST_9:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_QUEST_10:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_QUEST_11:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_QUEST_12:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_QUEST_13:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_QUEST_14:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_QUEST_15:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_KILL_1:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_KILL_2:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_KILL_3:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_KILL_4:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_KILL_5:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_KILL_6:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_KILL_7:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_KILL_8:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_KILL_9:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_KILL_10:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_KILL_11:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_KILL_12:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_KILL_13:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_KILL_14:
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_ARTIFACT_KILL_15:
        {
            auto info = DataManager::GetArtifactInfo(obj->getArtifactIdx());
            
            value += userInfo->getArtifactLevel(E_PLACE::DG_NORMAL, info->getIdx());
        }break;
            
            //Building
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_TOWN_LEVEL_1:
        {
            value += TownManager::getInstance()->getTownLevel(E_TOWN::BUILDING_1);
        }break;
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_TOWN_LEVEL_2:
        {
            value += TownManager::getInstance()->getTownLevel(E_TOWN::BUILDING_2);
        }break;
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_TOWN_LEVEL_3:
        {
            value += TownManager::getInstance()->getTownLevel(E_TOWN::BUILDING_3);
        }break;
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_TOWN_LEVEL_4:
        {
            value += TownManager::getInstance()->getTownLevel(E_TOWN::BUILDING_4);
        }break;
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_TOWN_LEVEL_5:
        {
            value += TownManager::getInstance()->getTownLevel(E_TOWN::BUILDING_5);
        }break;
        
            //Myhome
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_MYHOME_LEVEL_1:
        {
            value += userInfo->getMyhomeLevel(1);
        }break;
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_MYHOME_LEVEL_2:
        {
            value += userInfo->getMyhomeLevel(2);
        }break;
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_MYHOME_LEVEL_3:
        {
            value += userInfo->getMyhomeLevel(3);
        }break;
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_MYHOME_LEVEL_4:
        {
            value += userInfo->getMyhomeLevel(4);
        }break;
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_MYHOME_LEVEL_5:
        {
            value += userInfo->getMyhomeLevel(5);
        }break;
            
            //Raid
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_NEW_RAID_ACCRUE_STAT:
        {
            value = NewRaidManager::getInstance()->getTotalAbilityPoint();
        }break;
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_NEW_RAID_SEASON_LEVEL:
        {
            value = std::atoi(MafAes256::Decrypt(UserDefault::getInstance()->getStringForKey(KEY_R_S_MAX_LV, "0")).c_str());
        }break;
        case E_NORMAL_ACHIEVE_MISSION::ACHIEVE_NEW_RAID_SEASON_DAMAGE:
        {
            value = std::atoi(MafAes256::Decrypt(UserDefault::getInstance()->getStringForKey(KEY_R_S_MAX_DAMAGE, "0")).c_str());
        }break;
    }
    
    obj->setCount(value);
    
}

cocos2d::Vector<ObjAchievement*> AchievementManager::getAchievementNormalListAll()
{
    return _listAchievementNormal;
}

cocos2d::Vector<ObjAchievement*> AchievementManager::getAchievementNormalListFromType(E_ACHIEVE_NORMAR_TYPE value, bool isCleanUp)
{
    cocos2d::Vector<ObjAchievement*> vec;
    
    if(!isCleanUp)
        updateNormalAchieveCurrent();
    
    for(auto info : _listAchievementNormal)
    {
        if(info->getNormalType() == value)
        {
            if(isCleanUp)
            {
                vec.pushBack(info);
            }
            else
            {
                if(value == E_ACHIEVE_NORMAR_TYPE::ACHIEVE_ARTIFACT)
                {
                    if(info->isCurrent())
                        vec.pushBack(info);
                }
                else
                    vec.pushBack(info);
            }
        }
    }
    
    return vec;
}

int AchievementManager::getAchieveNormalSize()
{
    return (int)_listAchievementNormal.size();
}

bool AchievementManager::isRewardCondition()
{
    bool result = false;
    
    for(auto obj : _listAchievementNormal)
    {
        if(obj->isRewardCondition() && !obj->isComplete())
        {
            result = true;
            break;
        }
    }
    
    return result;
}

bool AchievementManager::isRewardCondition(E_ACHIEVE_NORMAR_TYPE type)
{
    bool result = false;
    
    auto achieveList = getAchievementNormalListFromType(type);
    
    for(auto obj : achieveList)
    {
        if(obj->isRewardCondition() && !obj->isComplete())
        {
            result = true;
            break;
        }
    }
    
    return result;
}

void AchievementManager::setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION mission, int value, bool isPlus)
{
    
    for(auto obj : _listAchievementNormal)
    {
        if(obj->isComplete())
            continue;
        
        if(obj->getNormalMission() == mission)
        {
            if(isPlus)
            {
                obj->setCount(obj->getCount() + value);
            }
            else
            {
                obj->setCount(value);
            }
        }
    }
    
    saveNormalAchieveData();
    updateNormalAchieveCurrent();
    updateNormalAchieveIsReward();
}

ObjAchievement* AchievementManager::getObjAchieveNormal(int idx)
{
    ObjAchievement* result = nullptr;
    
    for(auto obj : _listAchievementNormal)
    {
        if(obj->getIdx() == idx)
        {
            result = obj;
        }
    }
    
    return result;
}

ObjAchievement* AchievementManager::getObjAchieveNormal(E_NORMAL_ACHIEVE_MISSION mission)
{
    ObjAchievement* result = nullptr;
    
    for(auto obj : _listAchievementNormal)
    {
        if(obj->getNormalMission() == mission)
        {
            result = obj;
        }
    }
    
    return result;
}

ObjAchievement* AchievementManager::getObjAchieveNormal(E_ARTIFACT arti)
{
    ObjAchievement* result = nullptr;
    
    for(auto obj : _listAchievementNormal)
    {
        if(obj->getArtifactIdx() == arti)
        {
            result = obj;
        }
    }
    
    return result;
}

void AchievementManager::achievementNormalReward(ObjAchievement* achieve)
{
    if(achieve->isComplete() || !achieve->isRewardCondition())
        return;
    
    
    ItemsManager::getInstance()->addItem(achieve->getItem());
    
    achieve->addGrade();
    
    if(achieve->getGrade() >= achieve->getMaxGrade())
    {
        achieve->setComplete(true);
        achieve->setGrade(achieve->getMaxGrade());
    }
    
    achieve->setRewardCondition(false);
    
    updateNormalAchieveCurrent();
    updateNormalAchieveIsReward();
    saveNormalAchieveData();
    
    if(_callbackRedraw != nullptr)
        _callbackRedraw();
    
    _callbackRedraw = nullptr;
}

void AchievementManager::achievementNormalRewardAll(E_ACHIEVE_NORMAR_TYPE type)
{
    auto vecAchieve = getAchievementNormalListFromType(type);
    
    Vector<InfoItems*> vecReward;
    vecReward.clear();
    
    for(auto obj : vecAchieve)
    {
        if(obj->isComplete() || !obj->isRewardCondition())
            continue;
        
        for(int i = obj->getGrade(); i <= obj->getMaxGrade(); ++i)
        {
            if(obj->isComplete() || !obj->isRewardCondition())
            break;
            
            vecReward.pushBack(obj->getItem());
            obj->addGrade();
            
            if(obj->getGrade() > obj->getMaxGrade())
            {
                obj->setComplete(true);
                obj->setGrade(obj->getMaxGrade());
            }
            
            obj->setRewardCondition(false);
            updateNormalAchieveIsReward(obj);
            
            
        }
    }
    
    achieveNormalRewardAllCleanUp(vecReward);
    
    ItemsManager::getInstance()->addItems(_vecNormalAchieveItemAll);
    _vecNormalAchieveItemAll.clear();
    
    updateNormalAchieveCurrent();
    updateNormalAchieveIsReward();
    saveNormalAchieveData();
    
    if(_callbackRedraw != nullptr)
        _callbackRedraw();
    
    _callbackRedraw = nullptr;
}

void AchievementManager::setCallbackRedraw(const std::function<void(void)>& pCallback)
{
    _callbackRedraw = pCallback;
}

#pragma mark - network : info
void AchievementManager::requestInfo(int nType, const std::function<void(bool)>& pCallback)
{
    //
    _callbackInfo = pCallback;
    
    //
    std::string url = "/achievements/info/list/%d/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx(), nType);
    
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(response,dd);
    };
    req->send(cb);
}

void AchievementManager::responseInfo(network::HttpResponse* response,std::string &data)
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
        // callback
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false);
        }
        return;
    }
    
    
    int nType = jsonParser["_type"].GetInt();
    bool bComplete = jsonParser["_is_finish"].GetBool();
    
    //
    _listAchievementComplete[nType] = bComplete;
    
    //
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer( sb );
    jsonParser["_achievements_list"].Accept( writer );

    std::string strList = sb.GetString();
    setListConvert(nType, strList);
    
    
    //
    if ( _callbackInfo != nullptr )
    {
        _callbackInfo(true);
    }
}

void AchievementManager::requestUpdate(int nType, int nKind, int nCount)
{
    //
    std::string url = "/achievements/UpdateAchievementsCount";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", nType);
    req->setData("_kind", nKind);
    req->setData("_count", nCount);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
    };
    req->send(cb);
}

void AchievementManager::requestReward(int nType, int nIdx, const std::function<void(bool, int)>& pCallback)
{
    //
    _callbackReward = pCallback;
    
    //
    std::string url = "/v1/achievements/CompleteAchievements";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", nType);
    req->setData("_idx", nIdx);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseReward(response,dd);
    };
    req->send(cb);
}

void AchievementManager::responseReward(cocos2d::network::HttpResponse* response,std::string &data)
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
        // callback
        if ( _callbackReward != nullptr )
        {
            _callbackReward(false, -1);
        }
        return;
    }
    
    int nResult = jsonParser["_result"].GetInt();
    int nType = jsonParser["_type"].GetInt();
    
    
    //
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer( sb );
    jsonParser["_achievements_list"].Accept( writer );

    std::string strList = sb.GetString();
    setListConvert(nType, strList);
    
    //
    std::string strReward = jsonParser["_reward"].GetString();
    std::string strItems = jsonParser["_items"].GetString();
    ItemsManager::getInstance()->addItems(strReward);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    //
    if ( _callbackReward != nullptr )
    {
        _callbackReward(true, nResult);
    }
}

void AchievementManager::requestComplete(int nType, const std::function<void(bool, int)>& pCallback)
{
    //
    _callbackComplete = pCallback;
    
    //
    std::string url = "/achievements/FinishAchievements";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", nType);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseComplete(response,dd);
    };
    req->send(cb);
}

void AchievementManager::responseComplete(cocos2d::network::HttpResponse* response,std::string &data)
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
        // callback
        if ( _callbackComplete != nullptr )
        {
            _callbackComplete(false, -1);
        }
        return;
    }
    
    // {"_result": 1, "_type": 0}
    int nResult = jsonParser["_result"].GetInt();
    int nType = jsonParser["_type"].GetInt();
    
    if ( nResult == 1 )
    {
        //
        CostumeSkinManager::getInstance()->addCostumeSkinBuy(12);
        
        //
        cocos2d::Vector<InfoItems*> listReward;
        
        auto objItem = ItemsManager::getInstance()->getInfoItems(E_ITEMS::COSTUME_SKIN);
        objItem->setTypeParams(12);
        objItem->setCount(1);
        
        listReward.pushBack(objItem);
        if ( listReward.size() != 0 )
        {
            PopupToastItems::show(listReward);
        }
        
        //
        _listAchievementComplete[nType] = true;
    }
    
    
    //
    if ( _callbackComplete != nullptr )
    {
        _callbackComplete(true, nResult);
    }
}

#pragma mark -normal achieve Utills

void AchievementManager::achieveNormalRewardAllCleanUp(Vector<InfoItems*> rewardList)
{
    for(auto obj : rewardList)
    {
        auto item = getItemToIdxFromNormalRewardAll(obj->getIdx());
        
        if(item != nullptr)
        {
            item->setCount(std::atoi(item->getCount().c_str()) + std::atoi(obj->getCount().c_str()));
        }
        else
        {
            _vecNormalAchieveItemAll.pushBack(obj);
        }
    }
    
}

InfoItems* AchievementManager::getItemToIdxFromNormalRewardAll(int idx)
{
    InfoItems* result = nullptr;
    
    for(auto item : _vecNormalAchieveItemAll)
    {
        if(item->getIdx() == idx)
        {
            result = item;
            break;
        }
    }
    
    return result;
}
