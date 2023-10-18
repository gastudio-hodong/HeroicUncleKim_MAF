//
//  ExpeditionManager.cpp
//  FantasyClicker-mobile
//
//  Created by Hyun Jin on 29/03/2020.
//

#include "ExpeditionManager.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

ExpeditionManager::ExpeditionManager(void) :
//
_nDrawSeason(0),
_nDrawEndTime(0),
_nSwapFriendPoint(0),
_bDrawBonus(false),

//
_callbackDrawInfo(nullptr),
_callbackDraw(nullptr),
_callbackDrawBonus(nullptr)
{
    
}

ExpeditionManager::~ExpeditionManager(void)
{
    
}

bool ExpeditionManager::init()
{
    return true;
}

#pragma mark - Load Data

#pragma mark - set, get : Expedition default

#pragma mark - set, get : Expedition cat

#pragma mark - set, get : Expedition draw
cocos2d::Vector<InfoItems*> ExpeditionManager::getDrawRewardAll()
{
    return _listDrawReward;
}

cocos2d::Vector<InfoItems*> ExpeditionManager::getDrawRewardBonusAll()
{
    return _listDrawRewardBonus;
}

int64_t ExpeditionManager::getDrawEndTime()
{
    return _nDrawEndTime;
}

int ExpeditionManager::getDrawSwapFriendPoint()
{
    return _nSwapFriendPoint;
}

bool ExpeditionManager::isDrawBounus()
{
    return _bDrawBonus;
}

#pragma mark - network : Expedition default

#pragma mark - network : Expedition Kraken

#pragma mark - network : Expedition Cat

#pragma mark - network : Expedition draw
void ExpeditionManager::requestDrawInfo(const std::function<void(bool, int)>& pCallback)
{
    //
    _callbackDrawInfo = pCallback;
    
    //
    std::string url = "/v1/event/point/draw/read/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());

    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseDrawInfo(response,dd);
    };
    req->send(cb);
}

void ExpeditionManager::responseDrawInfo(cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackDrawInfo != nullptr )
        {
            _callbackDrawInfo(false, -1);
            _callbackDrawInfo = nullptr;
        }
        return;
    }
    
    
    auto nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        _nDrawSeason = jsonParser["_eventseason"].GetInt();
        _nDrawEndTime = jsonParser["_end_time"].GetInt();
        _nSwapFriendPoint = jsonParser["_swap_friend_point"].GetInt();
        _bDrawBonus = jsonParser["_is_event_bonus"].GetBool();
        
        
        int nFriendship = jsonParser["_frindship"].GetInt();
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::FRIEND_POINT, 0, nFriendship);
        
        int nExpedition = jsonParser["_expedition"].GetInt();
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::EXPEDI_POINT, 0, nExpedition);
        
        //
        _listDrawReward.clear();
        for ( int i = 0; i < jsonParser["_board"].Size(); i++ )
        {
            const rapidjson::Value& jsonItems = jsonParser["_board"][i];
            
            InfoItems* objItem = nullptr;
            int nType = 0;
            int nCount = 0;
            int nTypeParams = 0;
            
            if ( jsonItems["type"].IsInt())
            {
                nType = jsonItems["type"].GetInt();
                objItem = ItemsManager::getInstance()->getInfoItems(nType);
            }
            else
            {
                objItem = InfoItems::create(nType);
            }
            
            if ( jsonItems.HasMember("count") == true )
            {
                nCount = jsonItems["count"].GetInt();
            }
            if ( jsonItems.HasMember("type_params") == true )
            {
                nTypeParams = jsonItems["type_params"].GetInt();
            }
            objItem->setCount(nCount);
            objItem->setTypeParams(nTypeParams);
            
            _listDrawReward.pushBack(objItem);
        }
        
        //
        _listDrawRewardBonus.clear();
        if ( _bDrawBonus == true )
        {
            for ( int i = 0; i < jsonParser["_event_bonus"].Size(); i++ )
            {
                const rapidjson::Value& jsonItems = jsonParser["_event_bonus"][i];
                
                InfoItems* objItem = nullptr;
                int nType = 0;
                int nCount = 0;
                int nTypeParams = 0;
                
                if ( jsonItems["type"].IsInt() )
                {
                    nType = jsonItems["type"].GetInt();
                    objItem = ItemsManager::getInstance()->getInfoItems(nType);
                }
                else
                {
                    objItem = InfoItems::create(nType);
                }
                
                if ( jsonItems.HasMember("count") == true )
                {
                    nCount = jsonItems["count"].GetInt();
                }
                if ( jsonItems.HasMember("type_params") == true )
                {
                    nTypeParams = jsonItems["type_params"].GetInt();
                }
                objItem->setCount(nCount);
                objItem->setTypeParams(nTypeParams);
                
                _listDrawRewardBonus.pushBack(objItem);
            }
        }
        
        //
        if ( _callbackDrawInfo != nullptr )
        {
            _callbackDrawInfo(true, nResult);
            _callbackDrawInfo = nullptr;
        }
    }
    else
    {
        if ( _callbackDrawInfo != nullptr )
        {
            _callbackDrawInfo(false, nResult);
            _callbackDrawInfo = nullptr;
        }
    }
}

void ExpeditionManager::requestDrawUpdate(const std::function<void(bool, int)>& pCallback, std::string strType, int nIdx)
{
    //
    _callbackDraw = pCallback;
    
    //
    std::string url = "/v1/event/point/draw/update";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_eventseason", _nDrawSeason);
    req->setData("_type", strType.c_str());
    req->setData("_order", nIdx);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseDrawUpdate(response,dd);
    };
    req->send(cb);
}

void ExpeditionManager::responseDrawUpdate(cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackDraw != nullptr )
        {
            _callbackDraw(false, -1);
            _callbackDraw = nullptr;
        }
        return;
    }
    
    auto nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        const rapidjson::Value& jsonItems = jsonParser["_reward"];
        
        int nType = jsonItems["type"].GetInt();
        int nCount = jsonItems["count"].GetInt();
        int nTypeParams = 0;
        
        if(jsonItems.HasMember("type_params"))
        {
            nTypeParams = jsonItems["type_params"].GetInt();
        }
        
        Vector<InfoItems*> listReward;
        auto objItem = ItemsManager::getInstance()->getInfoItems(nType);
        objItem->setCount(nCount);
        objItem->setTypeParams(nTypeParams);
        listReward.pushBack(objItem);
        
        ItemsManager::getInstance()->addItems(listReward);
        //
        
        if ( _callbackDraw != nullptr )
        {
            _callbackDraw(true, nResult);
            _callbackDraw = nullptr;
        }
    }
    else
    {
        if ( _callbackDraw != nullptr )
        {
            _callbackDraw(false, nResult);
            _callbackDraw = nullptr;
        }
    }
}

void ExpeditionManager::requestDrawBonus(const std::function<void(bool, int)>& pCallback, int nIdx)
{
    //
    _callbackDrawBonus = pCallback;
    
    //
    std::string url = "/v1/event/point/draw/bonusUpdate";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_eventseason", _nDrawSeason);
    req->setData("_order", nIdx);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseDrawBonus(response,dd);
    };
    req->send(cb);
}

void ExpeditionManager::responseDrawBonus(cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackDrawBonus != nullptr )
        {
            _callbackDrawBonus(false, -1);
            _callbackDrawBonus = nullptr;
        }
        return;
    }
    
    // {"_result": 1, "_reward": {"type": "PET_SKIN", "reward": "3_4"}}
    // {"_is_reward": [{"type": "GEM", "reward": 100}], "_result": 4, "_reward": {"type": "PET_SKIN", "reward": "7_4"}}
    auto nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        const rapidjson::Value& jsonItems = jsonParser["_reward"];
        
        int nType = jsonItems["type"].GetInt();
        int nCount = jsonItems["count"].GetInt();
        int nTypeParams = 0;
        
        if(jsonItems.HasMember("type_params"))
        {
            nTypeParams = jsonItems["type_params"].GetInt();
        }
        
        Vector<InfoItems*> listReward;
        auto objItem = ItemsManager::getInstance()->getInfoItems(nType);
        objItem->setCount(nCount);
        objItem->setTypeParams(nTypeParams);
        listReward.pushBack(objItem);
        
        ItemsManager::getInstance()->addItems(listReward);
        
        if ( _callbackDrawBonus != nullptr )
        {
            _callbackDrawBonus(true, nResult);
            _callbackDrawBonus = nullptr;
        }
    }
    else if ( nResult == 4 )
    {
        const rapidjson::Value& jsonItems = jsonParser["_is_reward"];
        
        int nType = jsonItems["type"].GetInt();
        int nCount = jsonItems["count"].GetInt();
        int nTypeParams = 0;
        
        if(jsonItems.HasMember("type_params"))
        {
            nTypeParams = jsonItems["type_params"].GetInt();
        }
        
        Vector<InfoItems*> listReward;
        auto objItem = ItemsManager::getInstance()->getInfoItems(nType);
        objItem->setCount(nCount);
        objItem->setTypeParams(nTypeParams);
        listReward.pushBack(objItem);
        
        ItemsManager::getInstance()->addItems(listReward);
        //
        
        if ( _callbackDrawBonus != nullptr )
        {
            _callbackDrawBonus(true, nResult);
            _callbackDrawBonus = nullptr;
        }
    }
    else
    {
        if ( _callbackDrawBonus != nullptr )
        {
            _callbackDrawBonus(false, nResult);
            _callbackDrawBonus = nullptr;
        }
    }
}
