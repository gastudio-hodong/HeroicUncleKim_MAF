//
//  EventReturnAttendManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/06/20.
//
#include "EventReturnAttendManager.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

EventReturnAttendManager::EventReturnAttendManager(void):
_nResult(0),
_nEndTime(0),
_isRewardCondition(false),
_callbackInfo(nullptr),
_callbackReward(nullptr)
{
    _listInfoReward.clear();
}

EventReturnAttendManager::~EventReturnAttendManager(void)
{
    _listInfoReward.clear();
}

bool EventReturnAttendManager::init()
{
    return true;
}

#pragma mark - get set
const int EventReturnAttendManager::getToday() const
{
    int result = 0;
    
    for(auto obj : _listInfoReward)
    {
        if(obj->isReward() && obj->getDay() > result)
        {
            result = obj->getDay();
        }
    }

    if(_isRewardCondition)
        result++;
    
    return result;
}
const int EventReturnAttendManager::getEndTime() const
{
    return _nEndTime;
}
const bool EventReturnAttendManager::isRewardCondition() const
{
    return _isRewardCondition;
}
const bool EventReturnAttendManager::isReturnEventCondition() const
{
    bool result = false;
    
    if(EventManager::getInstance()->isActivatedEvent(32) == true && _nResult == 1)
        result = true;
    
    return result;
}
const Vector<InfoEventReturnAttendReward*> EventReturnAttendManager::getListInfoReward() const
{
    return _listInfoReward;
}

const InfoEventReturnAttendReward* EventReturnAttendManager::getEventReturnAttendReward(const int day) const
{
    InfoEventReturnAttendReward* result = nullptr;
    
    for(auto obj : _listInfoReward)
    {
        if(obj->getDay() == day)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

#pragma mark - network
void EventReturnAttendManager::requestInfo(const std::function<void(bool, int)>& callback)
{
    _callbackInfo = callback;
    //
    std::string url = "/event/returnuserattendance/info/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseInfo(response,dd);
    };
    req->send(cb);
}

void EventReturnAttendManager::responseInfo(cocos2d::network::HttpResponse* const response,const std::string &data)
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
        if(_callbackInfo != nullptr)
            _callbackInfo(false, 0);
        return;
    }
    
    _nResult = jsonParser["_result"].GetInt();
    if ( _nResult != 1 )
    {
        if(_callbackInfo != nullptr)
            _callbackInfo(false, _nResult);
        return;
    }
    
    _nEndTime = jsonParser["_end_time"].GetInt();
    _isRewardCondition = jsonParser["_is_reward"].GetBool();
    
    _listInfoReward.clear();
    for ( int i = 0; i < jsonParser["_reward_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_reward_list"][i];
        
        const int nDay = jsonValue["day"].GetInt();
        const bool isReward = jsonValue["is_reward"].GetBool();
        const int nItemIdx = jsonValue["reward"]["idx"].GetInt();
        const int nItemCount = jsonValue["reward"]["count"].GetInt();
        int nItemTypeParams = 0;
        if(jsonValue["reward"].HasMember("type_params"))
            nItemTypeParams = jsonValue["reward"]["type_params"].GetInt();
        
        Vector<InfoItems*> list;
        list.clear();
        
        auto objItems = ItemsManager::getInstance()->getInfoItems(nItemIdx);
        if(objItems != nullptr)
        {
            objItems->setCount(nItemCount);
            objItems->setTypeParams(nItemTypeParams);
            list.pushBack(objItems);
        }
        
        auto objInfoReward = InfoEventReturnAttendReward::create();
        objInfoReward->setDay(nDay);
        objInfoReward->setIsReward(isReward);
        objInfoReward->setListReward(list);
        
        _listInfoReward.pushBack(objInfoReward);
    }
    
    if(_callbackInfo != nullptr)
        _callbackInfo(true, _nResult);
    
    _callbackInfo = nullptr;
}
 
void EventReturnAttendManager::requestReward(const std::function<void(bool, int)>& callback)
{
    _callbackReward = callback;
    
    //
    std::string url = "/event/returnuserattendance/reward";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseReward(response,dd);
    };
    req->send(cb);
}

void EventReturnAttendManager::responseReward(cocos2d::network::HttpResponse* const response,const std::string &data)
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
            _callbackReward(false, 0);
        }
        return;
    }
    
    const int result = jsonParser["_result"].GetInt();
    if ( result != 1 )
    {
        if(_callbackInfo != nullptr)
            _callbackInfo(false, result);
        return;
    }
    
    _nEndTime = jsonParser["_end_time"].GetInt();
    _isRewardCondition = false;
    
    _listInfoReward.clear();
    for ( int i = 0; i < jsonParser["_reward_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_reward_list"][i];
        
        const int nDay = jsonValue["day"].GetInt();
        const bool isReward = jsonValue["is_reward"].GetBool();
        const int nItemIdx = jsonValue["reward"]["idx"].GetInt();
        const int nItemCount = jsonValue["reward"]["count"].GetInt();
        Vector<InfoItems*> list;
        list.clear();
        
        auto objItems = ItemsManager::getInstance()->getInfoItems(nItemIdx);
        if(objItems != nullptr)
        {
            objItems->setCount(nItemCount);
            list.pushBack(objItems);
        }
        
        auto objInfoReward = InfoEventReturnAttendReward::create();
        objInfoReward->setDay(nDay);
        objInfoReward->setIsReward(isReward);
        objInfoReward->setListReward(list);
        
        _listInfoReward.pushBack(objInfoReward);
    }
    
    if ( _callbackReward != nullptr )
    {
        _callbackReward(true, result);
        _callbackReward = nullptr;
    }
}
