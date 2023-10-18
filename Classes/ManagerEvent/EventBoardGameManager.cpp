//
//  EventBoardGameManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/15.
//

#include "EventBoardGameManager.h"

#include "Common/ConfigGameNormal.h"
#include "GameObject/InfoQuest.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/AccountManager.h"

USING_NS_CC;

EventBoardGameManager::EventBoardGameManager(void) :
_isEventActivate(false),
_nBoardPosition(0),
_nTotalCandy(0),
_nTotalPoint(0),

_adsNow(0),
_adsMax(0),


_callbackBoardInfo(nullptr),
_callbackBoardReward(nullptr),
_callbackBoardAds(nullptr)
{
}

EventBoardGameManager::~EventBoardGameManager(void)
{
    
}

bool EventBoardGameManager::init()
{
    EventManager::getInstance()->_onRequestInfo += new MafFunction<void(const int)>(CC_CALLBACK_1(EventBoardGameManager::onEventActivate, this), this);
    setLoadQuest();
    setLoadBoard();
    
    return true;
}

#pragma mark - load
void EventBoardGameManager::setLoadQuest()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::EVENT_BOARDGAME_QUEST);
    
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
        
        int idx = jsonValue[("idx")].GetInt();
        int countMax = jsonValue[("quest_count")].GetInt();
        std::string title = jsonValue["name_key"].GetString();
        std::string reward = jsonValue["reward"].GetString();
        
        auto objQuest = InfoQuest::create();
        objQuest->setIdx(idx);
        objQuest->setCountMax(countMax);
        objQuest->setTitleKey(title);
        objQuest->setReward(reward);
        
        //
        _listQuest.pushBack(objQuest);
    }
}

void EventBoardGameManager::setLoadBoard()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::EVENT_BOARDGAME_BOARD);
    
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
        
        int nIdx = jsonValue["block_idx"].GetInt();
        
        std::string strTilePos = jsonValue["block_pos"].GetString();
        
        std::string strReward = jsonValue["reward"].GetString();
        
        auto listReward = ItemsManager::getInstance()->getConvertArray(strReward);
        
        int nItemIdx = 0;
        int nItemTypeParams = 0;
        int nItemCount = 0;
        if(listReward.size() > 0)
        {
            nItemIdx = listReward.at(0)->getIdx();
            nItemTypeParams = listReward.at(0)->getTypeParams();
            nItemCount = std::atoi(listReward.at(0)->getCount().c_str());
        }
        
        auto objBoard = InfoBoard::create();
        objBoard->setIdx(nIdx);
        objBoard->setTilePos(strTilePos);
        objBoard->setItems(nItemIdx, nItemTypeParams, nItemCount);
        
        //
        _listBoard.pushBack(objBoard);
    }
}

 
#pragma mark - set, get : prison
cocos2d::Vector<InfoQuest*> EventBoardGameManager::getListQuest()
{
    return _listQuest;
}

cocos2d::Vector<InfoBoard*> EventBoardGameManager::getListBoard()
{
    return _listBoard;
}

#pragma mark - set, get : other
int EventBoardGameManager::getPointCount()
{
    return (int)_nTotalPoint;
}
void EventBoardGameManager::setPointCount(int nValue)
{
    _nTotalPoint = nValue;
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}
int EventBoardGameManager::getTotalCandy()
{
    return (int)_nTotalCandy;
}
void EventBoardGameManager::setTotalCandy(int nValue)
{
    _nTotalCandy = nValue;
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}
int EventBoardGameManager::getTotalPoint()
{
    return (int)_nTotalPoint;
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}
void EventBoardGameManager::setTotalPoint(int nValue)
{
    _nTotalPoint = nValue;
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}
int EventBoardGameManager::getBoardPosition()
{
    return (int)_nBoardPosition;
}

int EventBoardGameManager::getAdsNow()
{
    return _adsNow.valueInt();
}

int EventBoardGameManager::getAdsMax()
{
    return _adsMax.valueInt();
}

#pragma mark - network : info

void EventBoardGameManager::requestBoardInfo(const std::function<void(bool, int)>& callback)
{
    _callbackBoardInfo = callback;
    
    //
    std::string url = "/event/halloween/board/info/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseBoardInfo(response,dd);
    };
    req->send(cb);
}

void EventBoardGameManager::responseBoardInfo(cocos2d::network::HttpResponse* response,std::string &data)
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
        _isEventActivate = true;
        int nTotalCandy = jsonParser["_total_candy"].GetInt();
        int nTotalPoint = jsonParser["_total_point"].GetInt();
        setTotalCandy(nTotalCandy);
        setTotalPoint(nTotalPoint);
        
        //
        _nBoardPosition = jsonParser["_postion"].GetInt();
        
        std::string strReward         = jsonParser["_reward"].GetString();
        std::string strItems         = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        _adsNow = jsonParser["_today_ads_count"].GetInt();;
        _adsMax = 0;
        std::string strEventOtherText = EventManager::getInstance()->getEventOhterDataByEventIdx(26);
        if ( strEventOtherText.empty() == false )
        {
            rapidjson::Document jsonParser;
            jsonParser.ParseInsitu<0>((char*)strEventOtherText.c_str());
            if( jsonParser.HasParseError() == false )
            {
                _adsMax = jsonParser["ads_max"].GetInt();
            }
        }
        
        for ( int i = 0; i < jsonParser["_quest_list"].Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser["_quest_list"][i];
            
            for (auto objQuest : _listQuest) {
                if ( objQuest->getIdx() == jsonValue["idx"].GetInt() )
                {
                    objQuest->setCountMax(jsonValue["quest_count"].GetInt());
                    objQuest->setCountNow(jsonValue["cureent_quest_count"].GetInt());
                    objQuest->setReceive(jsonValue["is_complete"].GetBool());
                }
            }
        }
        
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

void EventBoardGameManager::requestBoardReward(const std::function<void(bool, int, int, std::string)>& callback)
{
    _callbackBoardReward = callback;
    
    //
    std::string url = "/event/halloween/board/reward";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseBoardReward(response,dd);
    };
    req->send(cb);
}

void EventBoardGameManager::responseBoardReward(cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackBoardReward != nullptr )
        {
            _callbackBoardReward(false, -1, -1, "");
        }
        return;
    }
    
//    _result 0 : 재화 부족, 1 : 성공, 2 : 이벤트 진행 안함
    
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        int nTotalCandy = jsonParser["_total_candy"].GetInt();
        int nTotalPoint = jsonParser["_total_point"].GetInt();
        setTotalCandy(nTotalCandy);
        setTotalPoint(nTotalPoint);
        
        //
        _nBoardPosition = jsonParser["_postion"].GetInt();

        std::string strReward         = jsonParser["_reward"].GetString();
        std::string strItems         = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward, false);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        int nDice = jsonParser["_dice"].GetInt();
        
        if ( _callbackBoardReward != nullptr )
        {
            _callbackBoardReward(true, nResult, nDice, strReward);
        }
    }
    else
    {
        if ( _callbackBoardReward != nullptr )
        {
            _callbackBoardReward(false, nResult, 0, "");
        }
    }
}

void EventBoardGameManager::requestBoardAds(const std::function<void(bool, int)>& callback)
{
    _callbackBoardAds = callback;
    
    //
    std::string url = "/event/halloween/board/ads";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseBoardAds(response,dd);
    };
    req->send(cb);
}

void EventBoardGameManager::responseBoardAds(cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackBoardAds != nullptr )
        {
            _callbackBoardAds(false, -1);
            _callbackBoardAds = nullptr;
        }
        return;
    }
    
    // 1: 성공, 2 : 종료 된 이벤트, 3 : 이벤트 정보가 없습니다. 4 : 오늘 모두 소진
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        _adsNow = jsonParser["_today_ads_count"].GetInt();
        
        std::string strReward         = jsonParser["_reward"].GetString();
        std::string strItems         = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        if ( _callbackBoardAds != nullptr )
        {
            _callbackBoardAds(true, nResult);
            _callbackBoardAds = nullptr;
        }
    }
    else
    {
        if ( _callbackBoardAds != nullptr )
        {
            _callbackBoardAds(false, nResult);
            _callbackBoardAds = nullptr;
        }
    }
}

void EventBoardGameManager::onEventActivate(const int nIdx)
{
    if(nIdx == 26 && !_isEventActivate)
    {
        PopupLoading::show();
        requestBoardInfo([=](bool, int){
            PopupLoading::hide();
        });
        EventManager::getInstance()->AddActivateRequestCount();
    }
}
