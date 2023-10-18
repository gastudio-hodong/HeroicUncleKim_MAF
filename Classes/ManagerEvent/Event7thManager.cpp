//
//  Event7thManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF on 2022/05/12.
//

#include "Event7thManager.h"
#include "Common/ConfigGameNormal.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

Event7thManager::Event7thManager(void):
_isEventActivate(false),
_personalInfo(0),
_personalLevel(0),

_callbackInfo(nullptr),
_callbackUpdate(nullptr),
_callbackReward(nullptr)
{
    
}

Event7thManager::~Event7thManager(void)
{
    
}

bool Event7thManager::init()
{
    // load
    setLoad();
    setLoadPerson();
    EventManager::getInstance()->_onRequestInfo += new MafFunction<void(const int)>(CC_CALLBACK_1(Event7thManager::onEventActivate, this), this);
    return true;
}

#pragma mark - load
void Event7thManager::setLoad()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::EVENT_CO_MISSION);
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    //
    int nPointCountMin = 0;
    int nTypePrev = 0;
    
    //
    _listEvent7th.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        int nType = jsonValue["type"].GetInt();
        int nLevel = jsonValue["level"].GetInt();
        int nPointCountMax = jsonValue["pnt_cnt"].GetInt();
        int nPointCountPersonalLimit = jsonValue["pnt_limit"].GetInt();
        
        int nConditionPersonLevel = jsonValue["req_per_mission"].GetInt();
        
        std::string strNameKey = jsonValue["name_key"].GetString();
        std::string strDescKey = jsonValue["disc_key"].GetString();
        std::string strReward = jsonValue["reward"].GetString();
        
        if ( nTypePrev != nType )
        {
            nPointCountMin= 0;
        }
        
        auto objEvent7th = InfoEvent7th::create();
        objEvent7th->setType(nType);
        objEvent7th->setLevel(nLevel);
        objEvent7th->setPointCountMin(nPointCountMin);
        objEvent7th->setPointCountMax(nPointCountMax);
        objEvent7th->setPointCountPersonalLimit(nPointCountPersonalLimit);
        objEvent7th->setConditionPersonLevel(nConditionPersonLevel);
        objEvent7th->setNameKey(strNameKey);
        objEvent7th->setDescKey(strDescKey);
        objEvent7th->setReward(strReward);
        
        _listEvent7th.pushBack(objEvent7th);
        
        //
        nTypePrev = nType;
        nPointCountMin = nPointCountMax;
    }
}

void Event7thManager::setLoadPerson()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::EVENT_CO_MISSION_PERSON);
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    //
    int nPointCountMin = 0;
    
    //
    _listEvent7thPerson.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        int nLevel = jsonValue["level"].GetInt();
        int nPointCountMax = jsonValue["pnt_cnt"].GetInt();
        
        std::string strReward = jsonValue["reward"].GetString();
        
        auto objEvent7th = InfoEvent7thPerson::create();
        objEvent7th->setLevel(nLevel);
        objEvent7th->setPointCountMin(nPointCountMin);
        objEvent7th->setPointCountMax(nPointCountMax);
        objEvent7th->setReward(strReward);
        
        _listEvent7thPerson.pushBack(objEvent7th);
        
        //
        nPointCountMin = nPointCountMax;
    }
}

#pragma mark - get, set : 협동
cocos2d::Vector<InfoEvent7th*> Event7thManager::getListCoopInfo(int nType)
{
    Vector<InfoEvent7th*> listResult;
 
    for ( auto objEvent7th : _listEvent7th )
    {
        if ( nType == objEvent7th->getType() )
        {
            listResult.pushBack(objEvent7th);
        }
    }
    return listResult;
}

cocos2d::Vector<InfoEvent7th*> Event7thManager::getListCoopInfo()
{
    Vector<InfoEvent7th*> listResult;
    
    for ( auto iter = _listCoopInfo.begin(); iter != _listCoopInfo.end(); ++iter )
    {
        int nType = iter->first;
        
        auto listEvent7th = getListCoopInfo(nType);
        int nLevel = getCoopLevel(nType) + 1;
        if ( nLevel > listEvent7th.size() )
        {
            nLevel = (int)listEvent7th.size();
        }
        
        for ( auto objEvent7th : listEvent7th )
        {
            if ( nType == objEvent7th->getType() && nLevel <= objEvent7th->getLevel() )
            {
                listResult.pushBack(objEvent7th);
                break;
            }
        }
    }
    
    return listResult;
}

int Event7thManager::getCoopCount(int nType)
{
    int nCount = 0;
    
    auto iter = _listCoopInfo.find(nType);
    if ( iter != _listCoopInfo.end() )
    {
        nCount = iter->second;
    }
    
    return nCount;
}

int Event7thManager::getCoopLevel(int nType)
{
    int nLevel = 0;
    
    auto iter = _listCoopLevel.find(nType);
    if ( iter != _listCoopLevel.end() )
    {
        nLevel = iter->second;
    }
    
    return nLevel;
}

int Event7thManager::getCoopPersonalCount(int nType)
{
    int nLevel = 0;
    
    auto iter = _listCoopPersonalCount.find(nType);
    if ( iter != _listCoopPersonalCount.end() )
    {
        nLevel = iter->second;
    }
    
    return nLevel;
}

#pragma mark - get, set : 개인
cocos2d::Vector<InfoEvent7thPerson*> Event7thManager::getPersonalInfo()
{
    return _listEvent7thPerson;
}

InfoEvent7thPerson* Event7thManager::getPersonalInfoCurrent()
{
    InfoEvent7thPerson* objResult = nullptr;
    
    int nLevel = getPersonalLevel() + 1;
    if ( nLevel > _listEvent7thPerson.size() )
    {
        nLevel = (int)_listEvent7thPerson.size();
    }
    
    for ( auto objEvent7thPerson : _listEvent7thPerson )
    {
        if ( nLevel <= objEvent7thPerson->getLevel() )
        {
            objResult = objEvent7thPerson;
            break;
        }
    }
    
    return objResult;
}

int Event7thManager::getPersonalCount()
{
    return _personalInfo.valueInt();
}

int Event7thManager::getPersonalLevel()
{
    return _personalLevel.valueInt();
}

#pragma mark - network
void Event7thManager::requestInfo(const std::function<void(bool, int)>& callback)
{
    _callbackInfo = callback;
    //
    std::string url = "/event/cooperative/info/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(response,dd);
    };
    req->send(cb);
}

void Event7thManager::responseInfo(cocos2d::network::HttpResponse* response,std::string &data)
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
            _callbackInfo(false, 0);
        }
        return;
    }
    
    _listCoopInfo.clear();
    _listCoopLevel.clear();
    _listCoopPersonalCount.clear();
    
    // 0 : 오류, 1 : 성공, 2 : 이벤트 기간이 아님
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        _isEventActivate = true;
        // 개인
        _personalInfo = jsonParser["_total_count"].GetInt();
        
        // 협동 미션 횟수
        for ( int i = 0; i < jsonParser["_mission_info_list"].Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser["_mission_info_list"][i];
     
            int nType = jsonValue["type"].GetInt();
            int nCount = jsonValue["count"].GetInt();
            _listCoopInfo[nType] = nCount;
        }
        
        // 협동 레벨
        for ( int i = 0; i < jsonParser["_reward_info_list"].Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser["_reward_info_list"][i];
     
            int nType = jsonValue["type"].GetInt();
            int nLevel = jsonValue["level"].GetInt();
            
            if ( nType == 101 )
            {
                _personalLevel = nLevel;
            }
            else
            {
                _listCoopLevel[nType] = nLevel;
            }
        }
        
        // 협동 오늘 개인 미셧 횟수
        for ( int i = 0; i < jsonParser["_update_count_list"].Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser["_update_count_list"][i];
     
            int nType = jsonValue["type"].GetInt();
            int nLevel = jsonValue["count"].GetInt();
            
            _listCoopPersonalCount[nType] = nLevel;
        }
        
        
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(true, nResult);
            _callbackInfo = nullptr;
        }
    }
    else
    {
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false, nResult);
            _callbackInfo = nullptr;
        }
    }
}
 

void Event7thManager::requestUpdate(int nType, const std::function<void(bool, int)>& callback)
{
    _callbackUpdate = callback;
    
    //
    std::string url = "/event/cooperative/update";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", nType);
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseUpdate(response,dd);
    };
    req->send(cb);
}

void Event7thManager::responseUpdate(cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackUpdate != nullptr )
        {
            _callbackUpdate(false, 0);
        }
        return;
    }
    
    // 0 : 오류, 1 : 성공, 2 : 이벤트 기간이 아님
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        if ( _callbackUpdate != nullptr )
        {
            _callbackUpdate(true, nResult);
            _callbackUpdate = nullptr;
        }
    }
    else
    {
        if ( _callbackUpdate != nullptr )
        {
            _callbackUpdate(false, nResult);
            _callbackUpdate = nullptr;
        }
    }
}

void Event7thManager::requestReward(int nType, int nLevel, const std::function<void(bool, int, std::string)>& callback)
{
    _callbackReward = callback;
    
    //
    std::string url = "/event/cooperative/reward";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", nType);
    req->setData("_level", nLevel);
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseReward(response,dd);
    };
    req->send(cb);
}

void Event7thManager::responseReward(cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackReward != nullptr )
        {
            _callbackReward(false, 0, "");
        }
        return;
    }
    
    /*
     0 : 오류
     1 : 성공
     2 : 이벤트 기간이 아님
     3 : 이미 보상 획득,
     4 : 현재 받을 수 있는 보상 레벨이 아님,
     5 : 개인 미션 레벨 부족
     */
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        std::string strReward         = jsonParser["_reward"].GetString();
        std::string strItems         = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        if ( _callbackReward != nullptr )
        {
            _callbackReward(true, nResult, strReward);
            _callbackReward = nullptr;
        }
    }
    else
    {
        if ( _callbackReward != nullptr )
        {
            _callbackReward(false, nResult, "");
            _callbackReward = nullptr;
        }
    }
}

void Event7thManager::onEventActivate(const int nIdx)
{
    if(nIdx == 20 && !_isEventActivate)
    {
        PopupLoading::show();
        requestInfo([=](bool, int){
            PopupLoading::hide();
        });
        EventManager::getInstance()->AddActivateRequestCount();
    }
}
