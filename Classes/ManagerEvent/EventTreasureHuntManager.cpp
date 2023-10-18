//
//  EventTreasureHuntManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/24.
//
#include "EventTreasureHuntManager.h"

#include "Common/ConfigGameNormal.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

EventTreasureHuntManager::EventTreasureHuntManager(void) :
_isEventActivate(false),
_endTime(0),
_bExtraTime(0),

_rewardSoLevel(0),
_rewardSoValue(0),
_rewardCoLevel(0),
_rewardCoValue(0),

_adNow(0),
_compassNow(0),
_compassUsed(0),

_boardLastIdx(0)
{
    
}

EventTreasureHuntManager::~EventTreasureHuntManager(void)
{
    _delegateInfo.Clear();
}

bool EventTreasureHuntManager::init()
{
    EventManager::getInstance()->_onRequestInfo += new MafFunction<void(const int)>(CC_CALLBACK_1(EventTreasureHuntManager::onEventActivate, this), this);
    return true;
}

#pragma mark - update
void EventTreasureHuntManager::update(float dt)
{
    
}

#pragma mark - load
void EventTreasureHuntManager::setLoad()
{
    setLoadMap();
    setLoadReward();
}

void EventTreasureHuntManager::setLoadMap()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::EVENT_TREASURE_HUNT_MAP);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)(const char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _listInfoMap.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];

        int idx = jsonValue["idx"].GetInt();
        const std::string reward = jsonValue["reward"].GetString();
        
        auto obj = InfoEventTreasureHuntMap::create();
        obj->setIdx(idx);
        obj->setReward(reward);
        
        //
        _listInfoMap.pushBack(obj);
    }
}

void EventTreasureHuntManager::setLoadReward()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::EVENT_TREASURE_HUNT_REWARD);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)(const char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _listInfoReward.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];

        int type = jsonValue["type"].GetInt();
        int level = jsonValue["level"].GetInt();
        int value = jsonValue["value"].GetInt();
        const std::string reward = jsonValue["reward"].GetString();
        
        auto obj = InfoEventTreasureHuntReward::create();
        obj->setType(type);
        obj->setLevel(level);
        obj->setValue(value);
        obj->setReward(reward);
        
        //
        _listInfoReward.pushBack(obj);
    }
}
 
#pragma mark - set, get : info
cocos2d::Vector<InfoEventTreasureHuntMap*> EventTreasureHuntManager::getListInfoMap()
{
    return _listInfoMap;
}
InfoEventTreasureHuntMap* EventTreasureHuntManager::getInfoMap(int idx)
{
    for ( auto& obj : _listInfoMap )
    {
        if ( idx == obj->getIdx() )
            return obj;
    }
    
    return nullptr;
}
cocos2d::Vector<InfoEventTreasureHuntReward*> EventTreasureHuntManager::getListInfoReward(int type)
{
    return _listInfoReward;
}
InfoEventTreasureHuntReward* EventTreasureHuntManager::getInfoReward(int type, int level)
{
    for ( auto& obj : _listInfoReward )
    {
        if ( type == obj->getType() && level == obj->getLevel() )
            return obj;
    }
    
    return nullptr;
}


#pragma mark - set, get : reward
int64_t EventTreasureHuntManager::getEndTime()
{
    return _endTime;
}

bool EventTreasureHuntManager::isExtraTime()
{
    return _bExtraTime;
}

int EventTreasureHuntManager::getRewardLevel(int type)
{
    int level = 0;
    if ( type == 1 )
        level = _rewardSoLevel.valueInt();
    else if ( type == 2 )
        level = _rewardCoLevel.valueInt();
        
    return level;
}

int EventTreasureHuntManager::getRewardValue(int type)
{
    int value = 0;
    if ( type == 1 )
        value = _rewardSoValue.valueInt();
    else if ( type == 2 )
        value = _rewardCoValue.valueInt();
        
    return value;
}

int EventTreasureHuntManager::getADNow()
{
    return _adNow.valueInt();
}

int EventTreasureHuntManager::getADMax()
{
    int adsMax = 0;
    
    std::string data = EventManager::getInstance()->getEventOhterDataByEventIdx(39);
    if ( data.empty() == false )
    {
        rapidjson::Document jsonParser;
        jsonParser.ParseInsitu<0>((char*)data.c_str());
        if( jsonParser.HasParseError() == false )
        {
            adsMax = jsonParser["ads_max_count"].GetInt();
        }
    }
    
    return adsMax;
}

int EventTreasureHuntManager::getCompassNow()
{
    return _compassNow.valueInt();
}

int EventTreasureHuntManager::getCompassMax()
{
    int compassMax = 0;
    
    std::string data = EventManager::getInstance()->getEventOhterDataByEventIdx(39);
    if ( data.empty() == false )
    {
        rapidjson::Document jsonParser;
        jsonParser.ParseInsitu<0>((char*)data.c_str());
        if( jsonParser.HasParseError() == false )
        {
            compassMax = jsonParser["compass_count"].GetInt();
        }
    }
    
    return compassMax;
}

bool EventTreasureHuntManager::isCompassUsed()
{
    return _compassUsed.valueInt();
}

int EventTreasureHuntManager::getBonusNow()
{
    return (int)_listBoardReward.size();
}

int EventTreasureHuntManager::getBonusMax()
{
    int bonusMax = 0;
    
    std::string data = EventManager::getInstance()->getEventOhterDataByEventIdx(39);
    if ( data.empty() == false )
    {
        rapidjson::Document jsonParser;
        jsonParser.ParseInsitu<0>((char*)data.c_str());
        if( jsonParser.HasParseError() == false )
        {
            bonusMax = jsonParser["reward_count"].GetInt();
        }
    }
    
    return bonusMax;
}

int EventTreasureHuntManager::getBonusRatio()
{
    int bonusRatio = 0;
    
    std::string data = EventManager::getInstance()->getEventOhterDataByEventIdx(39);
    if ( data.empty() == false )
    {
        rapidjson::Document jsonParser;
        jsonParser.ParseInsitu<0>((char*)data.c_str());
        if( jsonParser.HasParseError() == false )
        {
            bonusRatio = jsonParser["reward_up"].GetInt();
        }
    }
    
    return bonusRatio;
}

bool EventTreasureHuntManager::isBonusEffect()
{
    int bonusMax = getBonusMax();
    int bonusNow = getBonusNow() % bonusMax;
    
    bool bBonus = false;
    if ( bonusNow >= bonusMax - 1 )
    {
        bBonus = true;
    }
    
    return bBonus;
}

bool EventTreasureHuntManager::isBonus(int idx)
{
    bool bBonus = false;
    
    auto iter = std::find(_listBoardReward.begin(), _listBoardReward.end(), idx);
    if ( iter != _listBoardReward.end() )
    {
        auto pos = std::distance(_listBoardReward.begin(), iter) + 1;
        if ( pos % getBonusMax() == 0 )
            bBonus = true;
    }
        
    return bBonus;
}

bool EventTreasureHuntManager::isBoardOpen(int idx)
{
    bool bOpen = false;
    
    if ( std::find(_listBoardReward.begin(), _listBoardReward.end(), idx) != _listBoardReward.end() )
    {
        bOpen = true;
    }
    return bOpen;
}

bool EventTreasureHuntManager::isBoardFinish()
{
    bool bFinish = false;
    
    if ( std::find(_listBoardReward.begin(), _listBoardReward.end(), 1) != _listBoardReward.end() )
    {
        bFinish = true;
    }
    
    if ( _listBoardReward.size() == _listBoard.size() )
    {
        bFinish = true;
    }
    
    
    return bFinish;
}

int EventTreasureHuntManager::getBoardPosToIdx(int pos)
{
    int idx = 0;
    if ( _listBoard.size() > pos )
    {
        idx = _listBoard.at(pos);
    }
    return idx;
}

int EventTreasureHuntManager::getBoardIdxToPos(int idx)
{
    auto iter = std::find(_listBoard.begin(), _listBoard.end(), idx);
    auto pos = std::distance(_listBoard.begin(), iter);
    return (int)pos;
}

int EventTreasureHuntManager::getBoardLastIdx()
{
    return _boardLastIdx.valueInt();
}

#pragma mark - game

#pragma mark - delegate
void EventTreasureHuntManager::subDelegateInfo(MafFunction<void(bool, int)>* func)
{
    _delegateInfo += func;
}
void EventTreasureHuntManager::invokeDelegateInfo(bool bResult, int nResult)
{
    _delegateInfo(bResult, nResult);
}
void EventTreasureHuntManager::unSubDelegateInfo(void* obj)
{
    _delegateInfo.Clear(obj);
}
 
#pragma mark - network : info
void EventTreasureHuntManager::requestInfo(bool bPopupLoading, const std::function<void(bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/event/treasurehunt/info/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(bPopupLoading, callback, response, dd);
    };
    req->send(cb);
}

void EventTreasureHuntManager::responseInfo(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data)
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
        invokeDelegateInfo(false, -1);
        if ( callback != nullptr )
        {
            callback(false, -1);
        }
        return;
    }
    
    auto result = jsonParser["_result"].GetInt();
    if ( result == 1 )
    {
        _isEventActivate = true;
        _endTime = jsonParser["_end_event"].GetInt64();
        _bExtraTime = jsonParser["_extra_time"].GetBool();
        
        _adNow = jsonParser["_ad_count"].GetInt();
        
        //
        convertJsonInfo(jsonParser);
        
        //
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        //
        invokeDelegateInfo(true, result);
        if ( callback != nullptr )
        {
            callback(true, result);
        }
    }
    else
    {
        invokeDelegateInfo(false, result);
        if ( callback != nullptr )
        {
            callback(false, result);
        }
    }
}

void EventTreasureHuntManager::requestReset(bool bPopupLoading, const std::function<void(bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/event/treasurehunt/resetBoard";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseReset(bPopupLoading, callback, response, dd);
    };
    req->send(cb);
}

void EventTreasureHuntManager::responseReset(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data)
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
           callback(false, -1);
       }
       return;
   }
   
   auto result = jsonParser["_result"].GetInt();
   if ( result == 1 )
   {
       //
       _listBoardReward.clear();
       convertJsonInfo(jsonParser);
       
       //
       std::string strItems = jsonParser["_items"].GetString();
       ItemsManager::getInstance()->setItemsServer(strItems);
       
       //
       if ( callback != nullptr )
       {
           callback(true, result);
       }
   }
   else
   {
       if ( callback != nullptr )
       {
           callback(false, result);
       }
   }
    
}

void EventTreasureHuntManager::requestCompass(bool bPopupLoading, const std::function<void(bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/event/treasurehunt/useHint";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseCompass(bPopupLoading, callback, response, dd);
    };
    req->send(cb);
}

void EventTreasureHuntManager::responseCompass(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data)
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
            callback(false, -1);
        }
        return;
    }
    
    auto result = jsonParser["_result"].GetInt();
    if ( result == 1 )
    {
        //
        convertJsonInfo(jsonParser);
        
        //
        if ( callback != nullptr )
        {
            callback(true, result);
        }
    }
    else
    {
        if ( callback != nullptr )
        {
            callback(false, result);
        }
    }
}

void EventTreasureHuntManager::requestBoard(bool bPopupLoading, int idx, const std::function<void(bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/event/treasurehunt/reward";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_idx", idx);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseBoard(bPopupLoading, callback, response, dd);
    };
    req->send(cb);
}

void EventTreasureHuntManager::responseBoard(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data)
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
            callback(false, -1);
        }
        return;
    }
    
    auto result = jsonParser["_result"].GetInt();
    if ( result == 1 )
    {
        //
        convertJsonInfo(jsonParser);
        
        //
        std::string strReward = jsonParser["_reward"].GetString();
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        //
        if ( callback != nullptr )
        {
            callback(true, result);
        }
    }
    else
    {
        if ( callback != nullptr )
        {
            callback(false, result);
        }
    }
}

void EventTreasureHuntManager::requestMissionReward(bool bPopupLoading, int type, int level, const std::function<void(bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/event/treasurehunt/questReward";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", type);
    req->setData("_lv", level);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseMissionReward(bPopupLoading, callback, response, dd);
    };
    req->send(cb);
}
void EventTreasureHuntManager::responseMissionReward(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data)
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
    jsonParser.Parse<0>((char*)data.c_str());
    if( data.length() == 0 || jsonParser.HasParseError() )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        if ( callback != nullptr )
        {
            callback(false, -1);
        }
        return;
    }
  
    int result = jsonParser["_result"].GetInt();
    if ( result == 1 )
    {
        //
        convertJsonInfo(jsonParser);
        
        //
        std::string strReward = jsonParser["_reward"].GetString();
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        //
        if ( callback != nullptr )
        {
            callback(true, result);
        }
    }
    else
    {
        if ( callback != nullptr )
        {
            callback(false, result);
        }
    }
}

void EventTreasureHuntManager::requestAD(bool bPopupLoading, const std::function<void(bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/ads/finish";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", 5);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseAD(bPopupLoading, callback, response, dd);
    };
    req->send(cb);
}

void EventTreasureHuntManager::responseAD(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data)
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
    jsonParser.Parse<0>((char*)data.c_str());
    if( data.length() == 0 || jsonParser.HasParseError() )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        if ( callback != nullptr )
        {
            callback(false, -1);
        }
        return;
    }
  
    int result = jsonParser["_result"].GetInt();
    if ( result == 1 )
    {
        _adNow = jsonParser["_count"].GetInt();
        
        //
        std::string strReward = jsonParser["_reward"].GetString();
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);

        //
        if ( callback != nullptr )
        {
            callback(true, result);
        }
    }
    else
    {
        if ( callback != nullptr )
        {
            callback(false, result);
        }
    }
}

void EventTreasureHuntManager::convertJsonInfo(const rapidjson::Value& json)
{
     
    if ( json.HasMember("_hint_info") == true )
    {
        _compassNow = json["_hint_info"]["hint_count"].GetInt();
        _compassUsed = json["_hint_info"]["is_used"].GetInt();
        
        _boardLastIdx = json["_hint_info"]["last_choice_idx"].GetInt();
    }
    
    if ( json.HasMember("_board_info") == true )
    {
        _listBoard.clear();
        for ( int i = 0; i < json["_board_info"].Size(); i++ )
        {
            const rapidjson::Value& jsonValue = json["_board_info"][i];
            
            _listBoard.push_back(jsonValue.GetInt());
        }
    }
    
    if ( json.HasMember("_board_reward_info") == true )
    {
        _listBoardReward.clear();
        for ( int i = 0; i < json["_board_reward_info"].Size(); i++ )
        {
            const rapidjson::Value& jsonValue = json["_board_reward_info"][i];
            
            _listBoardReward.push_back(jsonValue.GetInt());
        }
    }
    
    if ( json.HasMember("_quest_reward_info") == true )
    {
        _rewardSoValue = json["_quest_reward_info"]["1"]["count"].GetInt();
        _rewardSoLevel = json["_quest_reward_info"]["1"]["reward_info"].Size();
        
        _rewardCoValue = json["_quest_reward_info"]["2"]["count"].GetInt();
        _rewardCoLevel = json["_quest_reward_info"]["2"]["reward_info"].Size();
    }
}

void EventTreasureHuntManager::onEventActivate(const int nIdx)
{
    if(nIdx == 39 && !_isEventActivate)
    {
        requestInfo(true);
        EventManager::getInstance()->AddActivateRequestCount();
    }
}
