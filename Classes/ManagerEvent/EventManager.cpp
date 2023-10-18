//
//  EventManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#include "EventManager.h"

#include "ManagerGame/AccountManager.h"

USING_NS_CC;

EventManager::EventManager(void) :
_callbackInfo(nullptr),
_nRequestCount(0)
{
    _onRequestInfo.Clear();
    _onEventIconShow.Clear();
}

EventManager::~EventManager(void)
{
    _onRequestInfo.Clear();
}

bool EventManager::init()
{
    setLoadOriginData();
    return true;
}

void EventManager::setLoadOriginData()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::EVENT_DATE);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listEventOriginData.clear();
    std::string strCurType = "";
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];

        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        //Scene
        const int nIdx = jsonValue["idx"].GetInt();
        const int nEventIdx = jsonValue["event_idx"].GetInt();
        const int nEventType = jsonValue["event_type"].GetInt();
        const std::string strEventStartTime = jsonValue["date_start"].GetString();
        const std::string strEventEndTime = jsonValue["date_end"].GetString();
        const std::string strOtherData = jsonValue["other"].GetString();

        InfoEventOriginData* obj = InfoEventOriginData::create();
        obj->setIdx(nIdx);
        obj->setEventIdx(nEventIdx);
        obj->setEventType(nEventType);
        obj->setEventStartTime(strEventStartTime);
        obj->setEventEndTime(strEventEndTime);
        obj->setOtherData(strOtherData);

        _listEventOriginData.pushBack(obj);
    }
    _listEventOriginData.reverse();
}

#pragma mark - set, get, add
void EventManager::addActivatedEvent(InfoActivatedEvent* obj)
{
    _listActivatedEvent.pushBack(obj);
}

InfoActivatedEvent* EventManager::getActivatedEvent(int nIdx)
{
    InfoActivatedEvent *pResult = nullptr;
    
    for (auto &obj : _listActivatedEvent)
    {
        if ( obj->getIdx() == nIdx )
        {
            pResult = obj;
        }
    }
    
    return pResult;
}

bool EventManager::isActivatedEvent(int nIdx)
{
    bool bResult = false;
    
    auto obj = getActivatedEvent(nIdx);
    if ( obj != nullptr )
    {
        int64_t nTime = UtilsDate::getInstance()->getTime();
        if ( obj->getEventStart() < nTime && obj->getEventEnd() > nTime )
        {
            bResult = true;
        }
        
        if(obj->getIdx() == 8 || obj->getIdx() == 5)
            bResult = true;
    }
    
    return bResult;
}
InfoEventOriginData* EventManager::getEventOriginDataByEventIdx(int nIdx)
{
    InfoEventOriginData* result = nullptr;
    
    int64_t nTime = UtilsDate::getInstance()->getTime();
    for ( auto& obj : _listEventOriginData )
    {
        if ( obj->getEventIdx() == nIdx && obj->getEventStartTime() < nTime && obj->getEventEndTime() > nTime )
        {
            result = obj;
            break;
        }
    }
    
    if ( result == nullptr )
    {
        for ( auto& obj : _listEventOriginData )
        {
            if ( obj->getEventIdx() == nIdx )
            {
                result = obj;
                break;
            }
        }
    }
    
    return result;
}
std::string EventManager::getEventOhterDataByEventIdx(int nIdx)
{
    std::string result = "";
    
    auto objData = getEventOriginDataByEventIdx(nIdx);
    
    if(objData != nullptr)
        return objData->getOtherData();
    
    return result;
}
#pragma mark - network : info
void EventManager::requestInfo(const std::function<void(bool)>& pCallback)
{
    //
    _callbackInfo = pCallback;
    
    //
    std::string url = "/v1/event/info";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_userid", AccountManager::getInstance()->getUserID().c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(response,dd);
    };
    req->send(cb);
}

void EventManager::responseInfo(network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    _nRequestCount = 0;
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == false && jsonParser.HasMember("_event") == false )
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
    
    _listActivatedEvent.clear();
    for ( int i = 0; i < jsonParser["_event"].Size(); i++ )
    {
        const rapidjson::Value& jsonEvent = jsonParser["_event"][i];
        
        int nIdx = jsonEvent["_idx"].GetInt();
        int64_t nEventStart = jsonEvent["_event_start"].GetInt64();
        int64_t nEventEnd = jsonEvent["_event_end"].GetInt64();
        int64_t nEventRewardDay = 0;
        if ( jsonEvent.HasMember("_end_reward_day") == true )
        {
            nEventRewardDay = jsonEvent["_end_reward_day"].GetInt64();
        }
        
        auto obj = InfoActivatedEvent::create();
        obj->setIdx(nIdx);
        obj->setEventStart(nEventStart);
        obj->setEventEnd(nEventEnd);
        obj->setEventRewardDay(nEventRewardDay);
        
        addActivatedEvent(obj);
    }
    
    _onEventIconShow.Invoke();
    
    // callback
    if ( _callbackInfo != nullptr )
    {
        _callbackInfo(true);
    }
}

int EventManager::getActivateRequestCount() const
{
    return _nRequestCount;
}
void EventManager::AddActivateRequestCount()
{
    _nRequestCount++;
}
