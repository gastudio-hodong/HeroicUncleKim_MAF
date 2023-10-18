//
//  ServerMissionManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/22.
//

#include "ServerMissionManager.h"

#include "Common/ConfigKey.h"
#include "Common/ConfigGameNormal.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/SpecialMissionManager.h"
#include "ManagerGame/WeaponManager.h"

USING_NS_CC;

ServerMissionManager::ServerMissionManager():
_dailySeason(0),
_weeklySeason(0),
_dailyResetTime(0),
_weeklyResetTime(0),
_callbackInfoDaily(nullptr),
_callbackInfoWeekly(nullptr),
_callbackReward(nullptr)
{
    _vecServerMissionInfo.clear();
}
ServerMissionManager::~ServerMissionManager(void)
{
    _vecServerMissionInfo.clear();
}

bool ServerMissionManager::init()
{
    setLoadMission();
    _dailySeason = std::atoi(UserDefault::getInstance()->getStringForKey(KEY_SERVER_MISSION_SEASON_DAILY, "0").c_str());
    _weeklySeason = std::atoi(UserDefault::getInstance()->getStringForKey(KEY_SERVER_MISSION_SEASON_WEEKLY, "0").c_str());
    return true;
}

#pragma mark- Load
void ServerMissionManager::setLoadMission()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::DAILY_WEEKLY_MISSION);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _vecServerMissionInfo.clear();
    std::string strCurType = "";
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        const int nIdx = jsonValue["idx"].GetInt();
        const int nResetType = jsonValue["reset_type"].GetInt();
        const int isEventMission = jsonValue["is_event_mission"].GetInt();
        const int hasOpenCondition = jsonValue["has_open_condition"].GetInt();
        const int nPassPoint = jsonValue["pass_point"].GetInt();
        const std::string strTextKey = jsonValue["text"].GetString();
        const std::string strMissionType = jsonValue["mission_type"].GetString();
        const int nMissionCompleteCount = jsonValue["mission_complete_count"].GetInt();
        const int nShortCut = jsonValue["redirection"].GetInt();
        const std::string strReward = jsonValue["reward"].GetString();
        
        auto obj = InfoServerMission::create();
        obj->setIdx(nIdx);
        obj->setResetType((E_MISSION_RESET_TYPE)nResetType);
        obj->setIsEventMission(isEventMission);
        obj->setHasCondition(hasOpenCondition);
        obj->setPassPoint(nPassPoint);
        obj->setTitle(strTextKey);
        obj->setMissionType(strMissionType);
        obj->setShortcut((E_SHORTCUTS)nShortCut);
        obj->setCompleteCount(nMissionCompleteCount);
        obj->setListMissionReward(ItemsManager::getInstance()->getConvertArray(strReward));
        
        _vecServerMissionInfo.pushBack(obj);
    }
}
#pragma mark -Reset
void ServerMissionManager::resetServerMission(E_MISSION_RESET_TYPE type)
{
    for(auto obj : _vecServerMissionInfo)
    {
        if(obj->getResetType() == type)
            obj->resetData();
    }

}

#pragma mark -Mission
void ServerMissionManager::setServerMission(E_DAILY_QUEST mission, int plus)
{
    bool isServerUpdate = false;
    for(auto obj : _vecServerMissionInfo)
    {
        if(obj->isComplete() || obj->getMissionType() != mission || checkMissionCondition(obj->getMissionType()) == false)
            continue;
        
        obj->setCurrentCount(obj->getCurrentCount() + plus);
        if (obj->getCurrentCount() >= obj->getCompleteCount())
        {
            obj->setCurrentCount(obj->getCompleteCount());
        }
        
        //플레이 타임 미션이 아닐 경우 바로 서버에 업데이트
        if(mission != E_DAILY_QUEST::PLAYTIME_S)
            isServerUpdate = true;
    }
    saveServerMissionAll();
    
    if(isServerUpdate)
    {
        requestUpdateMission(false, E_MISSION_RESET_TYPE::DAILY, nullptr);
        requestUpdateMission(false, E_MISSION_RESET_TYPE::WEEK, nullptr);
    }
}

bool ServerMissionManager::checkServerMission()
{
    if(!ServerMissionManager::isServerMissionOpenCondition())
    {
        return false;
    }

    bool result = false;
    for(auto obj : _vecServerMissionInfo)
    {
        if(!obj->isComplete())
        {
            if(obj->getCurrentCount() >= obj->getCompleteCount())
            {
                obj->setCurrentCount(obj->getCompleteCount());
            }
        }
        if(obj->isComplete() && !obj->isServerComplete())
            result = true;
    }
    
    return result;
}

void ServerMissionManager::checkMidnightReset()
{
    //초기화 시간이 지나면 초기화
    int64_t nowTime = UtilsDate::getInstance()->getTime();
    if(_dailyResetTime != 0 && _dailyResetTime - nowTime <= 0 )
    {
        resetServerMissionData(E_MISSION_RESET_TYPE::DAILY);
        resetServerMission(E_MISSION_RESET_TYPE::DAILY);
        requestInfoMission(nullptr, E_MISSION_RESET_TYPE::DAILY);
    }
    if(_weeklyResetTime != 0 && _weeklyResetTime - nowTime <= 0)
    {
        resetServerMissionData(E_MISSION_RESET_TYPE::WEEK);
        resetServerMission(E_MISSION_RESET_TYPE::WEEK);
        requestInfoMission(nullptr, E_MISSION_RESET_TYPE::WEEK);
    }
}

bool ServerMissionManager::checkMissionCondition(E_DAILY_QUEST mission)
{
    bool result = false;

    //미션 컨디션
    switch (mission)
    {
        case E_DAILY_QUEST::RAID_ATT:
        {
            result = true;
        }
        case E_DAILY_QUEST::HELLTRIUM_GET:
        {
            if ( SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_PRISON_METER) >= 8000 )
                result = true;
        }
            break;
        case E_DAILY_QUEST::MINERAL_BUY:
        {
            if(WeaponManager::getInstance()->getEquipHigh() >= E_WEAPON::WEAPON_76_MINE)
                result = true;
        }
            break;
        default:
            result = true;
            break;
    }
    return result;
}

void ServerMissionManager::resetServerMissionData(E_MISSION_RESET_TYPE type)
{
    std::string key = "";
    if(type == E_MISSION_RESET_TYPE::DAILY)
        key = KEY_SERVER_MISSION_DAILY;
    else if(type == E_MISSION_RESET_TYPE::WEEK)
        key = KEY_SERVER_MISSION_WEEKLY;
    
    std::string strData = UserDefault::getInstance()->getStringForKey(key.c_str(), "");
    strData = MafAes256::Decrypt(strData);
    if(strData.compare("null") == 0)
    {
        strData = "";
    }

    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*) strData.c_str());
    std::string strDataKey = "";
    if(type == E_MISSION_RESET_TYPE::DAILY)
        strDataKey = "DAILY";
    else if (type == E_MISSION_RESET_TYPE::WEEK)
        strDataKey = "WEEKLY";

    if( jsonParser.HasParseError() == false )
    {
        if ( jsonParser.HasMember(strDataKey.c_str()) == true )
        {
            jsonParser[strDataKey.c_str()].SetArray();
        }
    }

    rapidjson::StringBuffer jsonBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonBuffer);
    jsonParser.Accept(jsonWriter);

    strData = jsonBuffer.GetString();
    strData = MafAes256::Encrypt(strData);
    
    UserDefault::getInstance()->setStringForKey(key.c_str(), strData);
    //구버전 키 제거
    if(UserDefault::getInstance()->getStringForKey(KEY_SERVER_QUEST, "") != "")
        UserDefault::getInstance()->deleteValueForKey(KEY_SERVER_QUEST);
    if(UserDefault::getInstance()->getStringForKey(KEY_QUEST_SEASON_DAILY, "") != "")
        UserDefault::getInstance()->deleteValueForKey(KEY_QUEST_SEASON_DAILY);
    if(UserDefault::getInstance()->getStringForKey(KEY_QUEST_SEASON_WEEKLY, "") != "")
        UserDefault::getInstance()->deleteValueForKey(KEY_QUEST_SEASON_WEEKLY);
}

#pragma mark -get
Vector<InfoServerMission*> ServerMissionManager::getListActivateServerMissionInfo()
{
    Vector<InfoServerMission*> vecResult;

    for(auto info : _vecServerMissionInfo)
    {
        if(info->isActivate())
            vecResult.pushBack(info);
    }

    return vecResult;
}

Vector<InfoServerMission*> ServerMissionManager::getListActivateServerMissionInfo(E_MISSION_RESET_TYPE type)
{
    Vector<InfoServerMission*> vecResult;

    for(auto info : _vecServerMissionInfo)
    {
        if(info->getResetType() == type && info->isActivate())
            vecResult.pushBack(info);
    }

    return vecResult;
}

InfoServerMission* ServerMissionManager::getActivateServerMissionInfo(std::string strType, E_MISSION_RESET_TYPE resetType, int completeCount)
{
    InfoServerMission* resultInfo = nullptr;

    for(auto info : _vecServerMissionInfo)
    {
        if(info->getMissionTypeByString() == strType &&
           info->getResetType() == resetType &&
           info->getCompleteCount() == completeCount &&
           info->isActivate())
        {
            resultInfo = info;
            break;
        }
    }

    return resultInfo;
}

Vector<InfoServerMission*> ServerMissionManager::getListServerMissionInfo()
{
    return _vecServerMissionInfo;
}

Vector<InfoServerMission*> ServerMissionManager::getListServerMissionInfo(E_MISSION_RESET_TYPE type)
{
    Vector<InfoServerMission*> vecResult;

    for(auto info : _vecServerMissionInfo)
    {
        if(info->getResetType() == type)
            vecResult.pushBack(info);
    }

    return vecResult;
}
InfoServerMission* ServerMissionManager::getServerMissionInfo(const int nIdx)
{
    InfoServerMission* resultInfo = nullptr;

    for(auto info : _vecServerMissionInfo)
    {
        if(info->getIdx() == nIdx)
        {
            resultInfo = info;
            break;
        }
    }

    return resultInfo;
}
InfoServerMission* ServerMissionManager::getServerMissionInfo(std::string strType, E_MISSION_RESET_TYPE resetType, int completeCount)
{
    InfoServerMission* resultInfo = nullptr;

    for(auto info : _vecServerMissionInfo)
    {
        if(info->getMissionTypeByString() == strType &&
           info->getResetType() == resetType &&
           info->getCompleteCount() == completeCount)
        {
            resultInfo = info;
            break;
        }
    }

    return resultInfo;
}

int ServerMissionManager::getDailySeason()
{
    return _dailySeason;
}

int ServerMissionManager::getWeeklySeason()
{
    return _weeklySeason;
}

int64_t ServerMissionManager::getResetTime(E_MISSION_RESET_TYPE type)
{
    switch (type) {
        case E_MISSION_RESET_TYPE::DAILY: return _dailyResetTime;

        case E_MISSION_RESET_TYPE::WEEK: return _weeklyResetTime;

        default: return 0;
    }
}

#pragma mark -get : is
bool ServerMissionManager::isServerMissionCompleteDaily()
{
    bool result = false;

    for(auto info : getListActivateServerMissionInfo())
    {
        if(info->getResetType() == E_MISSION_RESET_TYPE::DAILY &&
           info->isComplete() &&
           !info->isServerComplete())
        {
            result = true;
            break;
        }
    }

    return result;
}

bool ServerMissionManager::isServerMissionCompleteWeekly()
{
    bool result = false;

    for(auto info : getListActivateServerMissionInfo())
    {
        if(info->getResetType() == E_MISSION_RESET_TYPE::WEEK &&
           info->isComplete() &&
           !info->isServerComplete())
        {
            result = true;
            break;
        }
    }

    return result;
}

bool ServerMissionManager::isServerMissionCompleteAll()
{
    bool result = false;

    for(auto info : getListActivateServerMissionInfo())
    {
        if(info->isComplete() && !info->isServerComplete())
        {
            result = true;
            break;
        }
    }

    return result;
}

bool ServerMissionManager::isServerMissionComplete(E_DAILY_QUEST eType, E_MISSION_RESET_TYPE eResetType)
{
    bool result = false;

    auto list = getListActivateServerMissionInfo();

    if(eResetType != E_MISSION_RESET_TYPE::NONE)
        list = getListActivateServerMissionInfo(eResetType);

    for(auto obj : list)
    {
        if(obj->isServerComplete())
            return true;
    }

    return result;
}
bool ServerMissionManager::isCompleteDoneMission(E_MISSION_RESET_TYPE eType)
{
    bool result = false;

    switch (eType) {
        case E_MISSION_RESET_TYPE::DAILY:
        {
            bool daily = isServerMissionComplete(E_DAILY_QUEST::QUEST_DAILY_DONE, E_MISSION_RESET_TYPE::DAILY);
            bool weekly = isServerMissionComplete(E_DAILY_QUEST::QUEST_DAILY_DONE, E_MISSION_RESET_TYPE::WEEK);

            if(daily && weekly)
                result = true;

        }
            break;
        case E_MISSION_RESET_TYPE::WEEK:
        {
            result = isServerMissionComplete(E_DAILY_QUEST::QUEST_WEEKLY_DONE, E_MISSION_RESET_TYPE::WEEK);
        }
            break;
    }

    return result;
}

bool ServerMissionManager::isServerMissionOpenCondition()
{
    bool bResult = false;

    if(SpecialMissionManager::getInstance()->getSpecialMissionClear() < 38)
    {
        if(UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= 120)
        {
            bResult = true;
        }
    }
    else
        bResult = true;

    return bResult;
}

#pragma mark -save & load
void ServerMissionManager::loadServerMissionData(E_MISSION_RESET_TYPE type)
{
    std::string key = "";
    if(type == E_MISSION_RESET_TYPE::DAILY)
        key = KEY_SERVER_MISSION_DAILY;
    else if(type == E_MISSION_RESET_TYPE::WEEK)
        key = KEY_SERVER_MISSION_WEEKLY;
    
    std::string strData = UserDefault::getInstance()->getStringForKey(key.c_str(), "");
    strData = MafAes256::Decrypt(strData);
    if(strData.compare("null") == 0)
    {
        strData = "";
    }

    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*) strData.c_str());
    std::string strKey = "";
    
    if(type == E_MISSION_RESET_TYPE::DAILY)
        strKey = "DAILY";
    else if (type == E_MISSION_RESET_TYPE::WEEK)
        strKey = "WEEKLY";
    
    if( jsonParser.HasParseError())
        return;

    if ( jsonParser.HasMember(strKey.c_str()) == false )
        return;
    
    auto missionList = jsonParser[strKey.c_str()].GetArray();
    for(int i = 0; i < missionList.Size(); ++i)
    {
        auto missionInfo = missionList[i].GetObject();

        int idx = missionInfo["id"].GetInt();
        E_DAILY_QUEST missionType = (E_DAILY_QUEST)missionInfo["mission_type"].GetInt();
        int completeCount = missionInfo["complete_count"].GetInt();
        E_MISSION_RESET_TYPE resetType = (E_MISSION_RESET_TYPE)missionInfo["reset_type"].GetInt();

        auto objMission = getActivateServerMissionInfo(enumToString<E_DAILY_QUEST>(missionType), resetType, completeCount);
        if(objMission == nullptr)
            continue;

        int localCurrentCount = missionInfo["cureent_count"].GetInt();
        if(objMission->getCurrentCount() < localCurrentCount)
            objMission->setCurrentCount(localCurrentCount);
    }

    saveServerMissionByResetType(type);
}

void ServerMissionManager::saveServerMissionAll()
{
    writeJsonServerMission(E_MISSION_RESET_TYPE::DAILY);
    writeJsonServerMission(E_MISSION_RESET_TYPE::WEEK);
    UserDefault::getInstance()->setStringForKey(KEY_SERVER_MISSION_SEASON_DAILY, MafUtils::toString(_dailySeason));
    UserDefault::getInstance()->setStringForKey(KEY_SERVER_MISSION_SEASON_WEEKLY, MafUtils::toString(_weeklySeason));
}
void ServerMissionManager::saveServerMissionByResetType(E_MISSION_RESET_TYPE type)
{
    writeJsonServerMission(type);
    UserDefault::getInstance()->setStringForKey(KEY_SERVER_MISSION_SEASON_DAILY, MafUtils::toString(_dailySeason));
    UserDefault::getInstance()->setStringForKey(KEY_SERVER_MISSION_SEASON_WEEKLY, MafUtils::toString(_weeklySeason));
}
void ServerMissionManager::writeJsonServerMission(E_MISSION_RESET_TYPE type)
{
    std::string key = "";
    if(type == E_MISSION_RESET_TYPE::DAILY)
        key = KEY_SERVER_MISSION_DAILY;
    else if(type == E_MISSION_RESET_TYPE::WEEK)
        key = KEY_SERVER_MISSION_WEEKLY;
    
    std::string strData = "";
    Vector<InfoServerMission*> _listMission = getListActivateServerMissionInfo(type);
    
    std::string strDatakey = "";
    if(type == E_MISSION_RESET_TYPE::DAILY)
        strDatakey = "DAILY";
    else if (type == E_MISSION_RESET_TYPE::WEEK)
        strDatakey = "WEEKLY";
    
    rapidjson::Document jsonParser;
    jsonParser.SetObject();
    rapidjson::Value jsonKey(strDatakey.c_str(), jsonParser.GetAllocator());
    rapidjson::Value MissionData;
    MissionData.SetArray();
    
    
    for(auto info : _listMission)
    {
        if(!info->isActivate())
            continue;
            
        rapidjson::Value missionObj;
        missionObj.SetObject();
        
        rapidjson::Value vType;
        vType.SetInt((int)info->getIdx());
        missionObj.AddMember("id", vType, jsonParser.GetAllocator());
        
        rapidjson::Value vReset_type;
        vReset_type.SetInt((int)info->getResetType());
        missionObj.AddMember("reset_type", vReset_type, jsonParser.GetAllocator());
        
        rapidjson::Value vMission_type;
        vMission_type.SetInt((int)info->getMissionType());
        missionObj.AddMember("mission_type", vMission_type, jsonParser.GetAllocator());
        
        rapidjson::Value vTask_count;
        vTask_count.SetInt(info->getCompleteCount());
        missionObj.AddMember("complete_count", vTask_count, jsonParser.GetAllocator());
        
        rapidjson::Value vProgress;
        vProgress.SetInt(info->getCurrentCount());
        missionObj.AddMember("cureent_count", vProgress, jsonParser.GetAllocator());
        
        MissionData.PushBack(missionObj, jsonParser.GetAllocator());
    }
    
    jsonParser.AddMember(jsonKey, MissionData, jsonParser.GetAllocator());
    rapidjson::StringBuffer jsonBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonBuffer);
    jsonParser.Accept(jsonWriter);
    
    
    strData = jsonBuffer.GetString();
    
    strData = MafAes256::Encrypt(strData);
    UserDefault::getInstance()->setStringForKey(key.c_str(), strData);
}

std::string ServerMissionManager::getServerMissionData(E_MISSION_RESET_TYPE type)
{
    std::string key = "";
    if(type == E_MISSION_RESET_TYPE::DAILY)
        key = KEY_SERVER_MISSION_DAILY;
    else if(type == E_MISSION_RESET_TYPE::WEEK)
        key = KEY_SERVER_MISSION_WEEKLY;
    
    std::string strResult = "";
    std::string strData = UserDefault::getInstance()->getStringForKey(key.c_str(), "");
    strData = MafAes256::Decrypt(strData);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strData.c_str());
    if( jsonParser.HasParseError() == false )
    {
        rapidjson::StringBuffer jsonBuffer;
        rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonBuffer);
        jsonParser.Accept(jsonWriter);
        strResult = jsonBuffer.GetString();
    }
    else
    {
        strResult = "";
    }
    UserDefault::getInstance()->setStringForKey(key.c_str(), strResult);
    
    return strResult;
}

#pragma mark -utils
//void ServerMissionManager::removeCallback(E_MISSION_CALLBACK_TYPE eType)
//{
//    switch(eType)
//    {
//        case E_MISSION_CALLBACK_TYPE::DAILY: _callbackInfoDaily = nullptr; break;
//        case E_MISSION_CALLBACK_TYPE::WEEK: _callbackInfoWeekly = nullptr; break;
//        case E_MISSION_CALLBACK_TYPE::CHANGE: _callbackChange = nullptr; break;
//        case E_MISSION_CALLBACK_TYPE::REWARD: _callbackReward = nullptr; break;
//    }
//}

bool ServerMissionManager::checkMissionReset(const E_MISSION_RESET_TYPE type, const int season)
{
    bool result = false;
    
    std::string strKey = "";
    int *nowSeason = nullptr;
    if(type == E_MISSION_RESET_TYPE::DAILY)
    {
        strKey = KEY_SERVER_MISSION_SEASON_DAILY;
        nowSeason = &_dailySeason;
    }
    else if(type == E_MISSION_RESET_TYPE::WEEK)
    {
        strKey = KEY_SERVER_MISSION_SEASON_WEEKLY;
        nowSeason = &_weeklySeason;
    }
    
    if(*nowSeason != season)
    {
        resetServerMissionData(type);
        resetServerMission(type);
        result = true;
        *nowSeason = season;
    }
    
     UserDefault::getInstance()->setStringForKey(strKey.c_str(), MafUtils::toString(*nowSeason));
    
    return result;
}

#pragma mark - network
void ServerMissionManager::requestInfoMission(const std::function<void(bool, int)>& pCallback, E_MISSION_RESET_TYPE type, bool isLoadData)
{
    
    if(type == E_MISSION_RESET_TYPE::DAILY)
        _callbackInfoDaily = pCallback;
    else if(type == E_MISSION_RESET_TYPE::WEEK)
        _callbackInfoWeekly = pCallback;
    
    std::string url = "/v1/mission/info/list/%d/%d";
    if(AccountManager::getInstance()->getUserIdx() == 0)
    {
        if(type == E_MISSION_RESET_TYPE::DAILY)
        {
            if ( _callbackInfoDaily != nullptr )
            {
                _callbackInfoDaily(10, false);
                _callbackInfoDaily = nullptr;
            }
        }
        else if(type == E_MISSION_RESET_TYPE::WEEK)
        {
            if ( _callbackInfoWeekly != nullptr )
            {
                _callbackInfoWeekly(10, false);
                _callbackInfoWeekly = nullptr;
            }
        }
        return;
    }
    //데일리 리셋 타입 : 0
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx(), (int) type);

    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&, type, isLoadData](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfoMission(response,dd,type, isLoadData);
    };
    req->send(cb);
}

void ServerMissionManager::responseInfoMission(cocos2d::network::HttpResponse* response,std::string &data, E_MISSION_RESET_TYPE type, bool isLoadData)
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
        if(type == E_MISSION_RESET_TYPE::DAILY)
        {
            if ( _callbackInfoDaily != nullptr )
            {
                _callbackInfoDaily(false, 0);
                _callbackInfoDaily = nullptr;
            }
            
        }
        else if(type == E_MISSION_RESET_TYPE::WEEK)
        {
            if ( _callbackInfoWeekly != nullptr )
            {
                _callbackInfoWeekly(false, 0);
                _callbackInfoWeekly = nullptr;
            }
        }
        return;
    }
    auto questinfo = jsonParser["_mission_list"].GetArray();
    int tmpSeason = jsonParser["_season"].GetInt();
    
    if(type == E_MISSION_RESET_TYPE::DAILY)
        _dailyResetTime = jsonParser["_reset_time"].GetInt();
    else if(type == E_MISSION_RESET_TYPE::WEEK)
        _weeklyResetTime = jsonParser["_reset_time"].GetInt();
    
    checkMissionReset(type, tmpSeason);
    
    for (int i=0; i < questinfo.Size(); i++)
    {
        int idx = questinfo[i]["idx"].GetInt();
        int currentCount = questinfo[i]["cureent_complete_count"].GetInt();
        bool isServerComplete = questinfo[i]["is_complete"].GetBool();
        
        auto objMission = getServerMissionInfo(idx);
        if(objMission == nullptr)
            continue;
        objMission->setIsActivate(true);
        if(objMission->getCurrentCount() <= currentCount)
            objMission->setCurrentCount(currentCount);
        objMission->setIsServerComplete(isServerComplete);
    }

    if(isLoadData)
    {
        loadServerMissionData(type);
    }
    
    checkServerMission();
    saveServerMissionByResetType(type);
    
    if(type == E_MISSION_RESET_TYPE::DAILY)
    {
        if ( _callbackInfoDaily != nullptr )
        {
            _callbackInfoDaily(true, 1);
            _callbackInfoDaily = nullptr;
        }
    }
    else if(type == E_MISSION_RESET_TYPE::WEEK)
    {
        if ( _callbackInfoWeekly != nullptr )
        {
            _callbackInfoWeekly(true, 1);
            _callbackInfoWeekly = nullptr;
        }
    }
}

void ServerMissionManager::requestUpdateMission(bool bPopupLoading, E_MISSION_RESET_TYPE type, const std::function<void(bool, int)>& callback)
{
    if(AccountManager::getInstance()->getUserIdx() == 0)
    {
        if ( callback != nullptr )
        {
            callback(10, false);
        }
        return;
    }
    
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }

    //
    std::string url = "/v1/mission/UpdatemissionCount";
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_reset_type", (int)type); //데일리 리셋 타입 : 0
    
    Vector<InfoServerMission*> vecMissionData = getListActivateServerMissionInfo(type);
    std::vector<MafHttpModelNew*> ArrayObjList;
    for(auto info : vecMissionData)
    {
        std::vector<MafHttpModelNew*> ObjList;
        MafHttpModelNew *val1 = new MafHttpModelNew();
        MafHttpModelNew *val2 = new MafHttpModelNew();
        val1->setData("idx", info->getIdx());
        val2->setData("count", info->getCurrentCount());
        ObjList.push_back(val1);
        ObjList.push_back(val2);

        MafHttpModelNew *valObj = new MafHttpModelNew();

        valObj->setData("_mission_list", ObjList, true);
        ArrayObjList.push_back(valObj);
    }
    
    req->setDataObjecArray("_mission_list", ArrayObjList);
    
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseUpdateMission(bPopupLoading, type, callback, response, dd);
    };
    req->send(cb);
}

void ServerMissionManager::responseUpdateMission(bool bPopupLoading, E_MISSION_RESET_TYPE type, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response, std::string &data)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::hide();
    }
    
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
        if ( callback != nullptr )
        {
            callback(false, 0);
        }
        return;
    }
    
    auto questinfo = jsonParser["_mission_list"].GetArray();
    int tmpSeason = jsonParser["_season"].GetInt();
    if(type == E_MISSION_RESET_TYPE::DAILY)
        _dailyResetTime = jsonParser["_reset_time"].GetInt();
    else if(type == E_MISSION_RESET_TYPE::WEEK)
        _weeklyResetTime = jsonParser["_reset_time"].GetInt();
    
    checkMissionReset(type, tmpSeason);
    
    for (int i=0; i < questinfo.Size(); i++)
    {
        int idx = questinfo[i]["idx"].GetInt();
        int currentCount = questinfo[i]["cureent_complete_count"].GetInt();
        bool isServerComplete = questinfo[i]["is_complete"].GetBool();
        
        auto objMission = getServerMissionInfo(idx);
        if(objMission == nullptr)
            continue;
        objMission->setIsActivate(true);
        if(objMission->getCurrentCount() <= currentCount)
            objMission->setCurrentCount(currentCount);
        objMission->setIsServerComplete(isServerComplete);
    }

    if(jsonParser.HasMember("_event_list"))
    {
        auto eventList = jsonParser["_event_list"].GetArray();
        
        for(int i = 0; i < eventList.Size(); ++i)
        {
            EventManager::getInstance()->_onRequestInfo.Invoke(eventList[i].GetInt());
        }
        
        if(EventManager::getInstance()->getActivateRequestCount() > 0)
            EventManager::getInstance()->requestInfo(nullptr);
    }
    //
    saveServerMissionAll();
    
    //
    if ( callback != nullptr )
    {
        callback(true, 1);
    }
}

void ServerMissionManager::requestMissionReward(InfoServerMission* info, std::function<void(bool, int)> pCallback)
{
    std::string url = "/v1/mission/CompleteMission";
    _callbackReward = pCallback;
    if(AccountManager::getInstance()->getUserIdx() == 0)
    {
        if ( _callbackReward != nullptr )
        {
            _callbackReward(10, false);
            _callbackReward = nullptr;
        }
        return;
    }
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_reset_type", (int)info->getResetType());
    req->setData("_idx", (int)info->getIdx());
    
    if(info->getResetType() == E_MISSION_RESET_TYPE::DAILY)
        req->setData("_season", _dailySeason);
    else if(info->getResetType() == E_MISSION_RESET_TYPE::WEEK)
        req->setData("_season", _weeklySeason);
    
    MafResponseNew cb = [&, info](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseMissionReward(response, dd, info->getResetType());
    };
    req->send(cb);
}

void ServerMissionManager::responseMissionReward(cocos2d::network::HttpResponse* response,std::string &data, E_MISSION_RESET_TYPE type)
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
        if(_callbackReward != nullptr)
        {
            _callbackReward(false, 0);

            _callbackReward = nullptr;
        }
        return;
    }
    
    auto questinfo = jsonParser["_mission_list"].GetArray();
    int tmpSeason = jsonParser["_season"].GetInt();
    if(type == E_MISSION_RESET_TYPE::DAILY)
        _dailyResetTime = jsonParser["_reset_time"].GetInt();
    else if(type == E_MISSION_RESET_TYPE::WEEK)
        _weeklyResetTime = jsonParser["_reset_time"].GetInt();
    
    checkMissionReset(type, tmpSeason);
    
    for (int i=0; i < questinfo.Size(); i++)
    {
        int idx = questinfo[i]["idx"].GetInt();
        int currentCount = questinfo[i]["cureent_complete_count"].GetInt();
        bool isServerComplete = questinfo[i]["is_complete"].GetBool();
        
        auto objMission = getServerMissionInfo(idx);
        if(objMission == nullptr)
            continue;
        objMission->setIsActivate(true);
        if(objMission->getCurrentCount() <= currentCount)
            objMission->setCurrentCount(currentCount);
        objMission->setIsServerComplete(isServerComplete);
    }
    
    if(type == E_MISSION_RESET_TYPE::DAILY)
        setServerMission(E_DAILY_QUEST::QUEST_DAILY_DONE, 1);
    else if(type == E_MISSION_RESET_TYPE::WEEK)
        setServerMission(E_DAILY_QUEST::QUEST_WEEKLY_DONE, 1);
    
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    ItemsManager::getInstance()->addItems(strReward);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    saveServerMissionAll();
    
    if(_callbackReward != nullptr)
    {
        _callbackReward(true, 1);

        _callbackReward = nullptr;
    }
}
