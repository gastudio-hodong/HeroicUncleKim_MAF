//
//  TradingMarketManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/06/07.
//

#include "TradingMarketManager.h"

#include "Common/ConfigGameNormal.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

TradingMarketManager::TradingMarketManager(void) :
_hellPointNow(0),
_hellPointMax(0),

//
_callbackInfo(nullptr),
_callbackOpen(nullptr),
_callbackTrade(nullptr)
{
    
}

TradingMarketManager::~TradingMarketManager(void)
{
    
}

bool TradingMarketManager::init()
{
    
    return true;
}

#pragma mark - load
void TradingMarketManager::setLoad()
{
    setLoadTrade();
}

void TradingMarketManager::setLoadTrade()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::PRISON_HELL_TRADING_MARKET);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoTradingMarket.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        int type = jsonValue["type"].GetInt();
        int idx = jsonValue["idx"].GetInt();
        
        int countMax = jsonValue["lmit_count"].GetInt();
        
        std::string itemFrom = jsonValue["cost"].GetString();
        std::string itemTo = jsonValue["item"].GetString();
        
        int openCost = jsonValue["open_cost"].GetInt();;
        int openStep = jsonValue["open_step"].GetInt();;
        
        //
        auto obj = InfoTradingMarket::create();
        obj->setType(type);
        obj->setIdx(idx);
        
        obj->setCountMax(countMax);
        
        obj->setItemFrom(itemFrom);
        obj->setItemTo(itemTo);
        
        obj->setOpenCost(openCost);
        obj->setOpenStep(openStep);
        
        _listInfoTradingMarket.pushBack(obj);
    }
}

#pragma mark - info
Vector<InfoTradingMarket*> TradingMarketManager::getListInfoTradingMarketAll()
{
    return _listInfoTradingMarket;
}

Vector<InfoTradingMarket*> TradingMarketManager::getListInfoTradingMarketFromType(int type)
{
    Vector<InfoTradingMarket*> listResult;
    
    for ( auto& obj : _listInfoTradingMarket )
    {
        if ( obj->getType() == type )
        {
            listResult.pushBack(obj);
        }
    }
    
    return listResult;
}

InfoTradingMarket* TradingMarketManager::getInfoTradingMarket(int type, int idx)
{
    InfoTradingMarket* result = nullptr;
    
    for(auto obj : _listInfoTradingMarket)
    {
        if ( obj->getType() == type && obj->getIdx() == idx)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

#pragma mark - set, get :
bool TradingMarketManager::isOpen(int type, int idx)
{
    bool bResult = false;
    
    auto obj = getInfoTradingMarket(type, idx);
    if ( obj != nullptr )
    {
        if ( obj->getOpenStep() == 0 )
        {
            bResult = true;
        }
        else
        {
            auto iter = _listOpen.find(std::make_pair(type, idx));
            if ( iter != _listOpen.end() )
            {
                bResult = iter->second;
            }
        }
    }
    
    return bResult;
}

int TradingMarketManager::getCount(int type, int idx)
{
    int count = 0;
    
    auto iter = _listCount.find(std::make_pair(type, idx));
    if ( iter != _listCount.end() )
    {
        count = iter->second;
    }
    
    return count;
}

int TradingMarketManager::getHellPointNow()
{
    return _hellPointNow.valueInt();
}
int TradingMarketManager::getHellPointMax()
{
    return _hellPointMax.valueInt();
}

#pragma mark - network
void TradingMarketManager::requestInfo(bool bPopupLoading, int type, const std::function<void(bool)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackInfo = callback;
    
    //
    std::string url = "/v1/exchange/info/%d/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx(), type);
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void TradingMarketManager::responseInfo(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
    
    if ( bError == false && jsonParser.HasMember("_hell_info_list") == false )
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
    
    _listOpen.clear();
    _listCount.clear();
    for ( int i = 0; i < jsonParser["_hell_info_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_hell_info_list"][i];
        
        int type = 0;
        int idx = jsonValue["_hellidx"].GetInt();
        int count = jsonValue["_count"].GetInt();
        
        _listOpen[std::make_pair(type, idx)] = true;
        _listCount[std::make_pair(type, idx)] = count;
    }
    
    if ( jsonParser.HasMember("_hell_point") == true )
    {
        _hellPointNow = jsonParser["_hell_point"].GetInt();
    }
    
    if ( jsonParser.HasMember("_hell_max_point") == true )
    {
        _hellPointMax = jsonParser["_hell_max_point"].GetInt();
    }
    
    if ( jsonParser.HasMember("_helltrium") == true )
    {
        int nHell = jsonParser["_helltrium"].GetInt();
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::HELL, 0, nHell);
    }
     
    //
    if ( _callbackInfo != nullptr )
    {
        _callbackInfo(true);
        _callbackInfo = nullptr;
    }
    
}

void TradingMarketManager::requestOpen(bool bPopupLoading, int type, int idx, const std::function<void(bool)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackOpen = callback;
    
    //
    std::string url = "/exchange/expand";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", type);
    req->setData("_hellidx", idx);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseOpen(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void TradingMarketManager::responseOpen(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackOpen != nullptr )
        {
            _callbackOpen(false);
            _callbackOpen = nullptr;
        }
        return;
    }
    
    bool bResult = jsonParser["_result"].GetBool();
    if ( bResult == true )
    {
        if ( jsonParser.HasMember("_total_helltrium") == true )
        {
            int nHell = jsonParser["_total_helltrium"].GetInt();
            ItemsManager::getInstance()->setItemsServer(E_ITEMS::HELL, 0, nHell);
        }
        
        //
        if ( _callbackOpen != nullptr )
        {
            _callbackOpen(true);
            _callbackOpen = nullptr;
        }
    }
    else
    {
        //
        if ( _callbackOpen != nullptr )
        {
            _callbackOpen(false);
            _callbackOpen = nullptr;
        }
    }
}

void TradingMarketManager::requestTrade(bool bPopupLoading, int type, int idx, int count, const std::function<void(bool)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackTrade = callback;
    
    //
    std::string url = "/v1/exchange/trade";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", type);
    req->setData("_hellidx", idx);
    req->setData("_hell_count", count);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseTrade(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void TradingMarketManager::responseTrade(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackTrade != nullptr )
        {
            _callbackTrade(false);
            _callbackTrade = nullptr;
        }
        return;
    }
    
    bool bResult = jsonParser["_result"].GetBool();
    if ( bResult == true )
    {
        std::string strReward = jsonParser["_reward"].GetString();
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        //
        if ( _callbackTrade != nullptr )
        {
            _callbackTrade(true);
            _callbackTrade = nullptr;
        }
    }
    else
    {
        //
        if ( _callbackTrade != nullptr )
        {
            _callbackTrade(false);
            _callbackTrade = nullptr;
        }
    }
}
 

#pragma mark - event
int TradingMarketManager::onTradeOpen(int type, int idx, const std::function<void(bool)>& callback)
{
    int result = 0;
    /*
     0 : 성공
     1 : 조건 안됨
     2 : 이미 오픈
     3 : 재화 부족
     */
    
    auto obj = getInfoTradingMarket(type, idx);
    if ( obj == nullptr )
    {
        return 1;
    }
    
    bool bOpen = isOpen(type, obj->getIdx());
    if ( bOpen == true )
    {
        return 2;
    }
    
    bool bOpenPrev = isOpen(type, obj->getOpenStep());
    if ( bOpenPrev == false )
    {
        return 1;
    }
    
    
    auto cost = obj->getOpenCost();
    
    auto itemNeed = std::atoi(cost->getCount().c_str());
    auto itemNow = std::atoi(ItemsManager::getInstance()->getItems((E_ITEMS)cost->getIdx()).c_str());
    if ( itemNeed > itemNow )
    {
        return 3;
    }
    
    // network
    requestOpen(true, type, idx, callback);
    
    return result;
}

int TradingMarketManager::onTrade(int type, int idx, int count, const std::function<void(bool)>& callback)
{
    int result = 0;
    /*
     0 : 성공
     1 : 조건 안됨
     2 : 재화 부족
     */
    
    auto obj = getInfoTradingMarket(type, idx);
    if ( obj == nullptr )
    {
        return 1;
    }
    
    if ( obj->getCountMax() != 0 )
    {
        int countMax = obj->getCountMax();
        int countNow = TradingMarketManager::getInstance()->getCount(type, idx);
        if ( countNow + count > countMax )
        {
            return 1;
        }
    }
    
    auto cost = obj->getItemFrom();
    
    auto itemNeed = std::atoi(cost->getCount().c_str()) * count;
    auto itemNow = std::atoi(ItemsManager::getInstance()->getItems((E_ITEMS)cost->getIdx()).c_str());
    if ( itemNeed > itemNow )
    {
        return 2;
    }
    
    // network
    requestTrade(true, type, idx, count, callback);
    
    return result;
}
