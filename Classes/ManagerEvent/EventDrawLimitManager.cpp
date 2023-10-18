//
//  EventDrawLimitManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#include "EventDrawLimitManager.h"

#include "Common/ConfigGameNormal.h"
#include "ManagerGame/UserInfoManager.h"
#include "GameObject/EventDraw/InfoEventDraw.h"
#include "GameObject/EventDraw/InfoEventDrawBonus.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/StoneManager.h"

USING_NS_CC;

EventDrawLimitManager::EventDrawLimitManager(void) :
_isEventActivate(false),
_endTime(0),
_resetTime(0),

_tokenNow(0),
_tokenPieceNow(0),
_bonusIdx(0),
_bonusCount(0),

_bExtraTime(false),

//
_callbackInfo(nullptr),
_callbackDraw(nullptr),
_callbackDrawBonus(nullptr),
_callbackTokenPieceExchange(nullptr)
{
    
}

EventDrawLimitManager::~EventDrawLimitManager(void)
{
    
}

bool EventDrawLimitManager::init()
{
    EventManager::getInstance()->_onRequestInfo += new MafFunction<void(const int)>(CC_CALLBACK_1(EventDrawLimitManager::onEventActivate, this), this);
    return true;
}

#pragma mark - load
void EventDrawLimitManager::setLoad()
{
    setLoadFixed();
}

void EventDrawLimitManager::setLoadFixed()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::EVENT_LIMIT_COIN_FIXED);
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)(const char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _listInfoDrawBonus.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        auto idx = jsonValue["idx"].GetInt();
        auto countMax = jsonValue["count"].GetInt();
        auto rewardIdx = jsonValue["item_idx"].GetInt();
        auto rewardParams = jsonValue["type_params"].GetInt();
        auto rewardCount = jsonValue["item_count"].GetInt();
        
        auto obj = InfoEventDrawBonus::create();
        obj->setIdx(idx);
        obj->setCountMax(countMax);
        obj->setReward(rewardIdx, rewardParams, rewardCount);
        
        _listInfoDrawBonus.pushBack(obj);
    }
}

#pragma mark - set, get : list
Vector<InfoEventDraw*> EventDrawLimitManager::getListInfoItemsAll()
{
    return _listInfoDraw;
}

Vector<InfoEventDraw*> EventDrawLimitManager::getListInfoItemsLimit()
{
    Vector<InfoEventDraw*> listResult;
    
    for ( auto& obj : _listInfoDraw )
    {
        if ( obj->getCountLimit() != -1 )
        {
            listResult.pushBack(obj);
        }
    }
    
    return listResult;
}

InfoEventDraw* EventDrawLimitManager::getInfoItems(int idx)
{
    InfoEventDraw* objResult = nullptr;
    
    for ( auto& obj : _listInfoDraw )
    {
        if ( idx == obj->getIdx() )
        {
            objResult = obj;
            break;
        }
    }
    
    return objResult;
}

Vector<InfoEventDrawBonus*> EventDrawLimitManager::getListInfoBonusAll()
{
    return _listInfoDrawBonus;
}

InfoEventDrawBonus* EventDrawLimitManager::getInfoBonus(int idx)
{
    InfoEventDrawBonus* objResult = nullptr;
    
    for ( auto& obj : _listInfoDrawBonus )
    {
        if ( idx == obj->getIdx() )
        {
            objResult = obj;
            break;
        }
    }
    
    return objResult;
}

#pragma mark - set, get : other
int64_t EventDrawLimitManager::getEndTime()
{
    return _endTime;
}
int64_t EventDrawLimitManager::getResetTime()
{
    return _resetTime;
}

int EventDrawLimitManager::getTokenNow()
{
    return _tokenNow.valueInt();
}

int EventDrawLimitManager::getTokenPieceNow()
{
    return _tokenPieceNow.valueInt();
}

int EventDrawLimitManager::getTokenPieceMax()
{
    int count = 0;
    
    std::string jsonText = EventManager::getInstance()->getEventOhterDataByEventIdx(20);
    if ( jsonText.empty() == false )
    {
        rapidjson::Document jsonParser;
        jsonParser.Parse<0>((char*)jsonText.c_str());
        if( jsonParser.HasParseError() == false )
        {
            count = jsonParser["token_piece_condition"].GetInt();
        }
    }
    
    return count;
}

int EventDrawLimitManager::getBonusIdx()
{
    int idx = (int)(_bonusIdx + 1);
    if ( idx > _listInfoDrawBonus.size() )
    {
        idx = (int)_listInfoDrawBonus.size();
    }
    
    return idx;
}

int EventDrawLimitManager::getBonusCountNow()
{
    int count = 0;
    
    int idx = getBonusIdx();
    if ( idx > _listInfoDrawBonus.size() )
    {
        idx = (int)_listInfoDrawBonus.size();
    }
    
    for ( auto obj : _listInfoDrawBonus )
    {
        if ( obj->getIdx() < idx  )
        {
            count += obj->getCountMax();
        }
    }
    
    count = (int)(_bonusCount - count);
    count = MAX(count, 0);
    return count;
}

int EventDrawLimitManager::getBonusCountMax()
{
    int count = 0;
    
    int idx = getBonusIdx();
    if ( idx > _listInfoDrawBonus.size() )
    {
        idx = (int)_listInfoDrawBonus.size();
    }
    
    auto obj = getInfoBonus(idx);
    if ( obj != nullptr )
    {
        count = obj->getCountMax();
    }
    
    return count;
}

bool EventDrawLimitManager::isBonusFinish()
{
    return getBonusIdx() >= _listInfoDrawBonus.size();
}

bool EventDrawLimitManager::isExtraTime()
{
    return _bExtraTime;
}

std::vector<int> EventDrawLimitManager::getListDraw()
{
    return _listDraw;
}

#pragma mark - network : info
void EventDrawLimitManager::requestInfo(bool bPopupLoading, const std::function<void(bool, int)>& pCallback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackInfo = pCallback;
    
    //
    std::string url = "/event/limit/coin/info";
    
    // '_stones' : [0, 0, 0] # 43, 44, 45
    std::vector<int> listStone;
    listStone.push_back(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::ATTACK));
    listStone.push_back(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::GOLD));
    listStone.push_back(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::KEY));
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_stones", listStone);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response, const char *data){
        std::string dd = data;
        responseInfo(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void EventDrawLimitManager::responseInfo(bool bPopupLoading, network::HttpResponse* response,std::string &data)
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
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false, -1);
            _callbackInfo = nullptr;
        }
        return;
    }
    
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        _isEventActivate = true;
        _endTime = jsonParser["_end_time"].GetInt64();
        _resetTime = jsonParser["_reset_time"].GetInt64();
        _tokenNow = jsonParser["_total_coin"].GetInt();
        _tokenPieceNow = jsonParser["_total_coin_piece"].GetInt();
        
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::EVENT_TOKEN, 0, _tokenNow.valueInt());
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::EVENT_TOKEN_PIECE, 0, _tokenPieceNow.valueInt());
        
        //
        convertInfoDraw(jsonParser["_item_info_list"]);
        
        //
        _bonusIdx = jsonParser["_fixed_reward_list"].Size();
        _bonusCount = jsonParser["_total_count"].GetInt();
        
        if ( jsonParser.HasMember("_extra_time") )
            _bExtraTime = jsonParser["_extra_time"].GetBool();
        
        //
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

void EventDrawLimitManager::requestDraw(bool bPopupLoading, int nCount, const std::function<void(bool, int)>& pCallback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackDraw = pCallback;
    
    int nBoxIdx = ParameterManager::getInstance()->getParameterInteger("draw_limit_boxidx");
    
    //
    std::string url = "/event/limit/coin/summon";
    
    // '_stones' : [0, 0, 0] # 43, 44, 45
    std::vector<int> listStone;
    listStone.push_back(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::ATTACK));
    listStone.push_back(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::GOLD));
    listStone.push_back(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::KEY));
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_stones", listStone);
    req->setData("_count", nCount);
    req->setData("_boxidx", nBoxIdx);
    MafResponseNew cb = [=](void* http, network::HttpResponse* response, const char *data){
        std::string dd = data;
        responseDraw(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void EventDrawLimitManager::responseDraw(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
        // callback
        if ( _callbackDraw != nullptr )
        {
            _callbackDraw(false, -1);
            _callbackDraw = nullptr;
        }
        return;
    }
    
    
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        _resetTime = jsonParser["_reset_time"].GetInt64();
        _tokenNow = jsonParser["_total_coin"].GetInt();
        _tokenPieceNow = jsonParser["_total_coin_piece"].GetInt();
        
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::EVENT_TOKEN, 0, _tokenNow.valueInt());
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::EVENT_TOKEN_PIECE, 0, _tokenPieceNow.valueInt());
        
        //
        convertInfoDraw(jsonParser["_item_info_list"]);
        
        //
        _bonusCount = jsonParser["_total_count"].GetInt();
        
        //
        _listDraw.clear();
        for ( int i = 0; i < jsonParser["_item_info_idx_list"].Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser["_item_info_idx_list"][i];
         
            _listDraw.push_back(jsonValue.GetInt());
        }
        
        //
        std::string strReward = jsonParser["_reward"].GetString();
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward, false);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        // callback
        if ( _callbackDraw != nullptr )
        {
            _callbackDraw(true, nResult);
            _callbackDraw = nullptr;
        }
    }
    else
    {
        // callback
        if ( _callbackDraw != nullptr )
        {
            _callbackDraw(false, nResult);
            _callbackDraw = nullptr;
        }
    }
    
}

void EventDrawLimitManager::requestDrawBonus(bool bPopupLoading, int nCount, const std::function<void(bool, int)>& pCallback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackDrawBonus = pCallback;
    
    //
    std::string url = "/event/limit/coin/reward";
    
    // '_stones' : [0, 0, 0] # 43, 44, 45
    std::vector<int> listStone;
    listStone.push_back(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::ATTACK));
    listStone.push_back(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::GOLD));
    listStone.push_back(StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, E_STONE::KEY));
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_stones", listStone);
    req->setData("_count", nCount);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response, const char *data){
        std::string dd = data;
        responseDrawBonus(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void EventDrawLimitManager::responseDrawBonus(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackDrawBonus != nullptr )
        {
            _callbackDrawBonus(false, -1);
            _callbackDrawBonus = nullptr;
        }
        return;
    }
    
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        _bonusIdx = jsonParser["_fixed_reward_list"].Size();
        
        //
        std::string strReward = jsonParser["_reward"].GetString();
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        // callback
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

void EventDrawLimitManager::requestTokenPieceExchange(bool bPopupLoading, const std::function<void(bool, int)>& pCallback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackTokenPieceExchange = pCallback;
    
    //
    std::string url = "/event/limit/coin/exchange";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response, const char *data){
        std::string dd = data;
        responseTokenPieceExchange(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void EventDrawLimitManager::responseTokenPieceExchange(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackTokenPieceExchange != nullptr )
        {
            _callbackTokenPieceExchange(false, -1);
            _callbackTokenPieceExchange = nullptr;
        }
        return;
    }
    
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        _tokenNow = jsonParser["_total_coin"].GetInt();
        _tokenPieceNow = jsonParser["_total_coin_piece"].GetInt();
        
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::EVENT_TOKEN, 0, _tokenNow.valueInt());
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::EVENT_TOKEN_PIECE, 0, _tokenPieceNow.valueInt());
        
        //
        cocos2d::Vector<InfoItems*> listReward;
        
        auto objItem = ItemsManager::getInstance()->getInfoItems(E_ITEMS::EVENT_TOKEN);
        objItem->setCount(1);
        
        listReward.pushBack(objItem);
        if ( listReward.size() != 0 )
        {
            PopupToastItems::show(listReward);
        }
        
        // callback
        if ( _callbackTokenPieceExchange != nullptr )
        {
            _callbackTokenPieceExchange(true, nResult);
            _callbackTokenPieceExchange = nullptr;
        }
    }
    else
    {
        if ( _callbackTokenPieceExchange != nullptr )
        {
            _callbackTokenPieceExchange(false, nResult);
            _callbackTokenPieceExchange = nullptr;
        }
    }
}

void EventDrawLimitManager::requestTokenExchange(bool bPopupLoading, const std::function<void(bool, int)>& pCallback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackTokenExchange = pCallback;
    
    //
    std::string url = "/event/limit/coin/exchange";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response, const char *data){
        std::string dd = data;
        responseTokenExchange(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void EventDrawLimitManager::responseTokenExchange(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackTokenExchange != nullptr )
        {
            _callbackTokenExchange(false, -1);
            _callbackTokenExchange = nullptr;
        }
        return;
    }
    
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        _tokenNow = jsonParser["_total_coin"].GetInt();
        _tokenPieceNow = jsonParser["_total_coin_piece"].GetInt();
        
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::EVENT_TOKEN, 0, _tokenNow.valueInt());
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::EVENT_TOKEN_PIECE, 0, _tokenPieceNow.valueInt());
        
        //
        std::string strReward = jsonParser["_reward"].GetString();
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        // callback
        if ( _callbackTokenExchange != nullptr )
        {
            _callbackTokenExchange(true, nResult);
            _callbackTokenExchange = nullptr;
        }
    }
    else
    {
        if ( _callbackTokenExchange != nullptr )
        {
            _callbackTokenExchange(false, nResult);
            _callbackTokenExchange = nullptr;
        }
    }
}

void EventDrawLimitManager::convertInfoDraw(const rapidjson::Value& json)
{
    _listInfoDraw.clear();
    for ( int i = 0; i < json.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = json[i];
        
        auto idx = jsonValue["idx"].GetInt();
        auto type = jsonValue["box_type"].GetInt();
        auto countLimit = jsonValue["item_limit"].GetInt();
        auto rate = jsonValue["item_chance"].GetInt();
        
        auto rewardIdx = jsonValue["item_idx"].GetInt();
        auto rewardParams = jsonValue["type_params"].GetInt();
        auto rewardCount = jsonValue["item_count"].GetInt();
        
        auto obj = InfoEventDraw::create();
        obj->setIdx(idx);
        obj->setType(type);
        obj->setCountLimit(countLimit);
        obj->setRate(rate);
        obj->setReward(rewardIdx, rewardParams, rewardCount);

        _listInfoDraw.pushBack(obj);
    }
}

#pragma mark - event
int EventDrawLimitManager::onDraw(int count, const std::function<void(bool, int)>& callback)
{
    int result = 0;
    /*
     0 : 성공
     1 : 조건 안됨
     2 : 재화 부족
     */
    
    //
    int countNow = count;
    int countMax = getTokenNow();
    if ( countNow > countMax )
    {
        return 2;
    }
    
    //
    int64_t timeDiff = getEndTime() - UtilsDate::getInstance()->getTime();
    timeDiff = MAX(timeDiff, 0);
    if ( timeDiff <= 0 )
    {
        return 1;
    }
    
    //
    _listDraw.clear();
    
    // network
    requestDraw(false, count, callback);
    
    return result;
}
int EventDrawLimitManager::onDrawBonus(const std::function<void(bool, int)>& callback)
{
    int result = 0;
    /*
     0 : 성공
     1 : 조건 안됨.
     */
    
    bool bFinish = isBonusFinish();
    if ( bFinish == true )
    {
        return 1;
    }

    int countNow = getBonusCountNow();
    int countMax = getBonusCountMax();
    if ( countNow < countMax )
    {
        return 1;
    }
        
    auto idx = getBonusIdx();
    auto objBonus = getInfoBonus(idx);
    if ( objBonus == nullptr )
    {
        return 1;
    }
    
    // network
    requestDrawBonus(true, objBonus->getCountMax(), callback);
    
    return result;
}

int EventDrawLimitManager::onTokenPieceExchange(const std::function<void(bool, int)>& callback)
{
    int result = 0;
    /*
     0 : 성공
     1 : 조건 안됨.
     2 : 재화 부족
     */
    
    //
    int countNow = getTokenPieceNow();
    int countMax = getTokenPieceMax();
    if ( countNow < countMax || countMax == 0 )
    {
        return 2;
    }
    
    //
    if ( isExtraTime() == true )
    {
        return 1;
    }
    
    //
    int64_t timeDiff = getEndTime() - UtilsDate::getInstance()->getTime();
    timeDiff = MAX(timeDiff, 0);
    if ( timeDiff <= 0 )
    {
        return 1;
    }
    
    // network
    requestTokenPieceExchange(true, callback);
    
    return result;
}

int EventDrawLimitManager::onTokenExchange(const std::function<void(bool, int)>& callback)
{
    int result = 0;
    /*
     0 : 성공
     1 : 조건 안됨.
     */
    
    if ( isExtraTime() == false )
    {
        return 1;
    }
    
    //
    int64_t timeDiff = getEndTime() - UtilsDate::getInstance()->getTime();
    timeDiff = MAX(timeDiff, 0);
    if ( timeDiff <= 0 )
    {
        return 1;
    }
    
    if ( getTokenNow() <= 0 && getTokenPieceNow() <= 0 )
    {
        return 1;
    }
    
    // network
    requestTokenExchange(true, callback);
    
    return result;
}

void EventDrawLimitManager::onEventActivate(const int nIdx)
{
    if(nIdx == 20 && UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= 140 && !_isEventActivate)
    {
        requestInfo(true, nullptr);
        EventManager::getInstance()->AddActivateRequestCount();
    }
}
