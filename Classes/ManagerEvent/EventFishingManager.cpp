//
//  EventFishingManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#include "EventFishingManager.h"

#include "Common/ConfigGameNormal.h"
#include "GameObject/InfoQuest.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/AccountManager.h"

USING_NS_CC;

EventFishingManager::EventFishingManager(void) :
_isEventActivate(false),
_nTotalTicket(0),
_nTotalPoint(0),
_nTotalBait(0),

_nBaitBuyNow(0),
_nBaitBuyMax(0),
_nBaitBuyPrice(0),

_nBoardPosition(0),
_nBoardDicePrice(0),

_callbackInfo(nullptr),
_callbackBuyBait(nullptr),
_callbackFishing(nullptr),

_callbackBoardInfo(nullptr),
_callbackBoardDice(nullptr)
{
    
}

EventFishingManager::~EventFishingManager(void)
{
    
}

bool EventFishingManager::init()
{
    setLoadQuest();
    setLoadBoard();
    
    EventManager::getInstance()->_onRequestInfo += new MafFunction<void(int)>(CC_CALLBACK_1(EventFishingManager::onEventActivate, this), this);
    return true;
}

#pragma mark - load
void EventFishingManager::setLoadQuest()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::EVENT_FISHING_QUEST);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)(const char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _listQuest.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        auto idx = jsonValue[("idx")].GetInt();
        auto countMax = jsonValue[("quest_count")].GetInt();
        auto title = jsonValue["name_key"].GetString();
        
        int rewardIdx = jsonValue[("reward_idx")].GetInt();
        int rewardParams = jsonValue[("reward_type_params")].GetInt();
        int rewardCount = jsonValue[("reward_count")].GetInt();
        
        auto objQuest = InfoQuest::create();
        objQuest->setIdx(idx);
        objQuest->setCountMax(countMax);
        objQuest->setTitleKey(title);
        objQuest->addReward(rewardIdx, rewardParams, rewardCount);
        
        //
        _listQuest.pushBack(objQuest);
    }
}

void EventFishingManager::setLoadBoard()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::EVENT_FISHING_BOARD);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)(const char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _listBoard.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        int nIdx = jsonValue["block_id"].GetInt();
        
        std::string strTilePos = jsonValue["block_pos"].GetString();
        
        int nItemIdx = jsonValue["reward_type"].GetInt();
        int nItemTypeParams = jsonValue["reward_type_params"].GetInt();
        int nItemCount = jsonValue["reward_count"].GetInt();
        
        auto objBoard = InfoBoard::create();
        objBoard->setIdx(nIdx);
        objBoard->setTilePos(strTilePos);
        objBoard->setItems(nItemIdx, nItemTypeParams, nItemCount);
        
        //
        _listBoard.pushBack(objBoard);
    }
}

 
#pragma mark - set, get : prison
cocos2d::Vector<InfoQuest*> EventFishingManager::getListQuest()
{
    return _listQuest;
}

cocos2d::Vector<InfoBoard*> EventFishingManager::getListBoard()
{
    return _listBoard;
}

#pragma mark - set, get : other
int EventFishingManager::getPointCount()
{
    return (int)_nTotalPoint;
}
void EventFishingManager::setPointCount(int nValue)
{
    _nTotalPoint = nValue;
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}
int EventFishingManager::getBaitCount()
{
    return (int)_nTotalBait;
}
void EventFishingManager::setBaitCount(int nValue)
{
    _nTotalBait = nValue;
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}
int EventFishingManager::getPearlCount()
{
    return (int)_nTotalTicket;
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}
void EventFishingManager::setPearlCount(int nValue)
{
    _nTotalTicket = nValue;
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}
int EventFishingManager::getBaitBuyRemaining()
{
    return (int)(_nBaitBuyMax - _nBaitBuyNow);
}
int EventFishingManager::getBaitBuyMax()
{
    return (int)_nBaitBuyMax;
}
int EventFishingManager::getBaitBuyPrice()
{
    return (int)_nBaitBuyPrice;
}
int EventFishingManager::getBoardPosition()
{
    return (int)_nBoardPosition;
}
int EventFishingManager::getBoardDicePrice()
{
    return (int)_nBoardDicePrice;
}

#pragma mark - network : info
void EventFishingManager::requestInfo(const std::function<void(bool, int)>& callback)
{
    _callbackInfo = callback;
    
    //
    std::string url = "/event/fishing/info/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(response,dd);
    };
    req->send(cb);
}

void EventFishingManager::responseInfo(cocos2d::network::HttpResponse* response,std::string &data)
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
            _callbackInfo(false,  -1);
        }
        return;
    }
    
//    1 : 성공
//    2 : 이벤트 진행중아님
//    4 : 테이블 정보 없음
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        _isEventActivate = true;
        int nTotalPoint = jsonParser["_total_point"].GetInt();
        int nTotalBait = jsonParser["_total_bait"].GetInt();
        int nTotalTicket = jsonParser["_total_ticket"].GetInt();
        setPointCount(nTotalPoint);
        setBaitCount(nTotalBait);
        setPearlCount(nTotalTicket);
        
        _nBaitBuyNow = jsonParser["_buy_bait_count"].GetInt();
        _nBaitBuyMax = jsonParser["_max_buy_bait_count"].GetInt();
        _nBaitBuyPrice = jsonParser["_buy_bait_price"].GetInt();
        
        for ( int i = 0; i < jsonParser["_quest"].Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser["_quest"][i];
            
            for (auto objQuest : _listQuest) {
                if ( objQuest->getIdx() == jsonValue["_idx"].GetInt() )
                {
                    objQuest->setCountNow(jsonValue["_quest_count"].GetInt());
                    objQuest->setReceive(jsonValue["_reward"].GetInt());
                }
            }
        }
        
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(true,  nResult);
        }
    }
    else
    {
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false,  nResult);
        }
    }
}

void EventFishingManager::requestBuyBait(int nCount, const std::function<void(bool, int)>& callback)
{
    _callbackBuyBait = callback;
    if ( nCount <= 0 )
    {
        if ( _callbackBuyBait != nullptr )
            _callbackBuyBait(false, -1);
        
        return;
    }
    
    //
    std::string url = "/event/fishing/buyBait";

    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_count", nCount);
    req->setData("_price", _nBaitBuyPrice.valueInt());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseBuyBait(response,dd);
    };
    req->send(cb);
}

void EventFishingManager::responseBuyBait(cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackBuyBait != nullptr )
        {
            _callbackBuyBait(false,  -1);
        }
        return;
    }
    
//    0 : 실패
//    1 : 성공
//    2 : 이벤트 기간 아님
//    3 : 유저정보없음
//    6 : 초과 구매 불가
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        int nTotalBait = jsonParser["_total_bait"].GetInt();
        setBaitCount(nTotalBait);

        int nCurrencyCoin = jsonParser["_total_coin"].GetInt();
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::COSTUME_COIN, 0, nCurrencyCoin);
        
        _nBaitBuyNow = jsonParser["_buy_bait_count"].GetInt();
        _nBaitBuyMax = jsonParser["_max_buy_bait_count"].GetInt();
        _nBaitBuyPrice = jsonParser["_buy_bait_price"].GetInt();
                
        if ( _callbackBuyBait != nullptr )
        {
            _callbackBuyBait(true,  nResult);
        }
    }
    else
    {
        if ( _callbackBuyBait != nullptr )
        {
            _callbackBuyBait(false,  nResult);
        }
    }
}

void EventFishingManager::requestFishing(int nCount, const std::function<void(bool, int, std::string)>& callback)
{
    _callbackFishing = callback;
    
    //
    std::string url = "/event/fishing/reward";

    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_count", nCount);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseFishing(response,dd);
    };
    req->send(cb);
}

void EventFishingManager::responseFishing(cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackFishing != nullptr )
        {
            _callbackFishing(false,  -1, "");
        }
        return;
    }
    
//    0 : 실패
//    1 : 성공
//    2 : 이벤트 기간아님
//    3 : 유저 정보 없음
//    4 : 테이블 정보 없음
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        int nTotalPoint = jsonParser["_total_point"].GetInt();
        int nTotalBait = jsonParser["_total_bait"].GetInt();
        setPointCount(nTotalPoint);
        setBaitCount(nTotalBait);
        
        std::string strFishingReward = jsonParser["_fishing_list"].GetString();
             
        for ( int i = 0; i < jsonParser["_quest"].Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser["_quest"][i];
            
            for (auto objQuest : _listQuest) {
                if ( objQuest->getIdx() == jsonValue["_idx"].GetInt() )
                {
                    objQuest->setCountNow(jsonValue["_quest_count"].GetInt());
                    objQuest->setReceive(jsonValue["_reward"].GetInt());
                }
            }
        }
        
        std::string strReward         = jsonParser["_reward"].GetString();
        std::string strItems         = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        if ( _callbackFishing != nullptr )
        {
            _callbackFishing(true, nResult, strFishingReward);
        }
    }
    else
    {
        if ( _callbackFishing != nullptr )
        {
            _callbackFishing(false, nResult, "");
        }
    }
}

void EventFishingManager::requestBoardInfo(const std::function<void(bool, int)>& callback)
{
    _callbackBoardInfo = callback;
    
    //
    std::string url = "/event/fishing/board/info/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseBoardInfo(response,dd);
    };
    req->send(cb);
}

void EventFishingManager::responseBoardInfo(cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackBoardInfo != nullptr )
        {
            _callbackBoardInfo(false, -1);
        }
        return;
    }
    
//    1 : 성공
//    2 : 이벤트 기간아님
//    3 : 유저 정보 없음
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        int nTotalPoint = jsonParser["_total_point"].GetInt();
        int nTotalTicket = jsonParser["_total_ticket"].GetInt();
        setPointCount(nTotalPoint);
        setPearlCount(nTotalTicket);
        
        //
        _nBoardPosition = jsonParser["_postion"].GetInt();
        _nBoardDicePrice = jsonParser["_board_dice_price"].GetInt();
        
        //
        if ( _callbackBoardInfo != nullptr )
        {
            _callbackBoardInfo(true, nResult);
        }
    }
    else
    {
        if ( _callbackBoardInfo != nullptr )
        {
            _callbackBoardInfo(false, nResult);
        }
    }
}

void EventFishingManager::requestBoardDice(const std::function<void(bool, int, int, std::string)>& callback)
{
    _callbackBoardDice = callback;
    
    //
    std::string url = "/event/fishing/board/reward";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_price", 100);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseBoardDice(response,dd);
    };
    req->send(cb);
}

void EventFishingManager::responseBoardDice(cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackBoardDice != nullptr )
        {
            _callbackBoardDice(false, -1, -1, "");
        }
        return;
    }
    
//    0 : 실패
//    1 : 성공
//    2 : 이벤트 기간아님
//    3 : 유저 정보 없음
//    4 : 테이블 정보 없음
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        int nTotalPoint = jsonParser["_total_point"].GetInt();
        int nTotalTicket = jsonParser["_total_ticket"].GetInt();
        setPointCount(nTotalPoint);
        setPearlCount(nTotalTicket);
        
        //
        _nBoardPosition = jsonParser["_postion"].GetInt();

        std::string strReward         = jsonParser["_reward"].GetString();
        std::string strItems         = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward, false);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        int nDice = jsonParser["_dice"].GetInt();
        
        if ( _callbackBoardDice != nullptr )
        {
            _callbackBoardDice(true, nResult, nDice, strReward);
        }
    }
    else
    {
        if ( _callbackBoardDice != nullptr )
        {
            _callbackBoardDice(false, nResult, -1, "");
        }
    }
}


void EventFishingManager::onEventActivate(const int nIdx)
{
    if(nIdx == 17 && !_isEventActivate)
    {
        PopupLoading::show();
        requestInfo([=](bool, int){
            PopupLoading::hide();
        });
        EventManager::getInstance()->AddActivateRequestCount();
    }
}
