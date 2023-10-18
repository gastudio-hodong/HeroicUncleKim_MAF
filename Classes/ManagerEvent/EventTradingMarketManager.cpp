//
//  EventTradingMarketManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/01/11.
//

#include "EventTradingMarketManager.h"

#include "Common/ConfigGameNormal.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

EventTradingMarketManager::EventTradingMarketManager(void):
_isEventActivate(false)
,_nTotalCoin(0)
,_nTodayCoin(0)
,_nEndTime(0)
,_callbackInfo(nullptr)
,_callbackBuyItem(nullptr)
{
    _listInfoEventTradingItem.clear();
    _listInfoSetting.clear();
}

EventTradingMarketManager::~EventTradingMarketManager(void)
{
    _listInfoEventTradingItem.clear();
    _listInfoSetting.clear();
}

bool EventTradingMarketManager::init()
{
    setLoadTradingItem();
    setLoadSetting();
    EventManager::getInstance()->_onRequestInfo += new MafFunction<void(const int)>(CC_CALLBACK_1(EventTradingMarketManager::onEventActivate, this), this);
    return true;
}

#pragma mark - Load Data
void EventTradingMarketManager::setLoadTradingItem()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::EVENT_TRADING_MARKET_REWARD);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoEventTradingItem.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        auto objEventTradingItem = InfoEventTradingItem::create();
        int nIdx = jsonValue["idx"].GetInt();
        int nOrder = jsonValue["order"].GetInt();
        std::string strType = jsonValue["type"].GetString();
        int nLimitCount = jsonValue["limit_count"].GetInt();
        std::string strItems = jsonValue["item"].GetString();
        std::string strCost = jsonValue["cost"].GetString();
        
        objEventTradingItem->setIdx(nIdx);
        objEventTradingItem->setType(strType);
        objEventTradingItem->setOrder(nOrder);
        objEventTradingItem->setMaxBuyCount(nLimitCount);

        rapidjson::Document jsonParserReward;
        jsonParserReward.Parse<0>((char*)strItems.c_str());
        if( jsonParserReward.HasParseError() == false )
        {
            auto objItems = ItemsManager::getInstance()->getInfoItems(jsonParserReward["idx"].GetInt());
            if ( objItems == nullptr )
            {
                continue;
            }
            if ( jsonParserReward["count"].IsString() == true )      objItems->setCount(jsonParserReward["count"].GetString());
            else                                                    objItems->setCount(jsonParserReward["count"].GetInt());

            if ( jsonParserReward.HasMember("type_params") == true )
            {
                objItems->setTypeParams(jsonParserReward["type_params"].GetInt());
            }
            objEventTradingItem->setItem(objItems);
        }
        
        rapidjson::Document jsonParserPrice;
        jsonParserPrice.Parse<0>((char*)strCost.c_str());
        if( jsonParserPrice.HasParseError() == false )
        {
            auto objCostItem = ItemsManager::getInstance()->getInfoItems(jsonParserPrice["idx"].GetInt());
            if ( objCostItem == nullptr )
            {
                continue;
            }

            if ( jsonParserPrice["count"].IsString() == true )     objCostItem->setCount(jsonParserPrice["count"].GetString());
            else                                                   objCostItem->setCount(jsonParserPrice["count"].GetInt());

            if ( jsonParserPrice.HasMember("type_params") == true )
            {
                objCostItem->setTypeParams(jsonParserPrice["type_params"].GetInt());
            }
            objEventTradingItem->setPrice(objCostItem);
        }
        
        //
        _listInfoEventTradingItem.pushBack(objEventTradingItem);
    }
}
void EventTradingMarketManager::setLoadSetting()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::EVENT_TRADING_MARKET_SETTING);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoSetting.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        
        auto obj = InfoSetting::create();
        obj->setKey(jsonValue["key"].GetString());
        obj->setType(jsonValue["type"].GetString());
        obj->setValue(jsonValue["value"].GetString());
        _listInfoSetting.pushBack(obj);
    }
}

#pragma mark -get
Vector<InfoEventTradingItem*> EventTradingMarketManager::getListInfoEventTradingItem()
{
    return _listInfoEventTradingItem;
}

InfoEventTradingItem* EventTradingMarketManager::getInfoEventTradingItem(int idx)
{
    InfoEventTradingItem* result = nullptr;
    
    for(auto obj : _listInfoEventTradingItem)
    {
        if(obj->getIdx() == idx)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

InfoSetting* EventTradingMarketManager::getRaidSetting(std::string strKey)
{
    InfoSetting* result = nullptr;
    
    for(auto obj : _listInfoSetting)
    {
        if(obj->getKey() == strKey)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

int EventTradingMarketManager::getTotalCoin()
{
    return _nTotalCoin;
}

void EventTradingMarketManager::setTotalCoin(int value)
{
    _nTotalCoin = value;
}

int EventTradingMarketManager::getTodayCoin()
{
    return _nTodayCoin;
}

void EventTradingMarketManager::setTodayCoin(int value)
{
    _nTodayCoin = value;
}

int64_t EventTradingMarketManager::getEndTime()
{
    return _nEndTime;
}

void EventTradingMarketManager::requestInfo(const std::function<void(bool, int)>& callback)
{
    _callbackInfo = callback;
    //
    std::string url = "/event/tradingmarket/info/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(response,dd);
    };
    req->send(cb);
}

void EventTradingMarketManager::responseInfo(cocos2d::network::HttpResponse* response,std::string &data)
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
    
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false, nResult);
            _callbackInfo = nullptr;
        }
        return;
    }
    _isEventActivate = true;
    _nTotalCoin = jsonParser["_total_count"].GetInt();
    _nTodayCoin = jsonParser["_total_limit_count"].GetInt();
    _nEndTime   = jsonParser["_event_end_time"].GetInt();
    
    for ( int i = 0; i < jsonParser["_buy_shop_info_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_buy_shop_info_list"][i];
        
        for (auto obj : _listInfoEventTradingItem)
        {
            if ( obj->getIdx() == jsonValue["idx"].GetInt() )
            {
                int nCurrentCount = jsonValue["count"].GetInt();
                
                obj->setBuyCount(nCurrentCount);
            }
        }
    }
    
    
    if ( _callbackInfo != nullptr )
    {
        _callbackInfo(true, 1);
    }
    
    _callbackInfo = nullptr;
}

void EventTradingMarketManager::requestBuyItem(const std::function<void(bool, int)>& pCallback, int nIdx)
{
    //
    _callbackBuyItem = pCallback;
    
    //
    std::string url = "/event/tradingmarket/buyTradingMarket";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_index", nIdx);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseBuyItem(response,dd);
    };
    req->send(cb);
}

void EventTradingMarketManager::responseBuyItem(cocos2d::network::HttpResponse* response,std::string &data)
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
    Vector<InfoItems*> listReward;
    listReward.clear();
    if ( bError == true )
    {
        if ( _callbackBuyItem != nullptr )
        {
            _callbackBuyItem(false, 0);
            _callbackBuyItem = nullptr;
        }
        return;
    }
    
   
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackBuyItem != nullptr )
        {
            _callbackBuyItem(false, nResult);
            _callbackBuyItem = nullptr;
        }
        return;
    }
    
    _nTotalCoin = jsonParser["_total_count"].GetInt();
    
    for ( int i = 0; i < jsonParser["_buy_shop_info_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_buy_shop_info_list"][i];
        
        for (auto obj : _listInfoEventTradingItem)
        {
            if ( obj->getIdx() == jsonValue["idx"].GetInt() )
            {
                int nCurrentCount = jsonValue["count"].GetInt();
                
                obj->setBuyCount(nCurrentCount);
            }
        }
    }
    
    // reward
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    ItemsManager::getInstance()->addItems(strReward);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    if ( _callbackBuyItem != nullptr )
    {
        _callbackBuyItem(true, nResult);
        _callbackBuyItem = nullptr;
    }
}

void EventTradingMarketManager::onEventActivate(const int nIdx)
{
    if(nIdx == 28 && !_isEventActivate)
    {
        PopupLoading::show();
        requestInfo([=](bool, int){
            PopupLoading::hide();
        });
        EventManager::getInstance()->AddActivateRequestCount();
    }
}
