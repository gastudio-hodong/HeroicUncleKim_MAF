//
//  EventBingoManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/07/23.
//

#include "EventBingoManager.h"
#include "Common/ConfigGameNormal.h"
#include "ManagerEvent/EventManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/AccountManager.h"


USING_NS_CC;

EventBingoManager::EventBingoManager(void):
_isEventActivate(false)
,_nBingoCompleteCount(0)
,_nBingoTicketCount(0)
,_nEventIdx(0)
,_nEventStartTime(0)
,_nEventEndTime(0)
,_nTotalEndTime(0)
,_callbackInfo(nullptr)
,_callbackUseTicket(nullptr)
,_callbackReward(nullptr)
,_callbackQuestReward(nullptr)
,_callbackNextBoard(nullptr)
{
    _listBoardBlock.clear();
    _listLineRewardBlock.clear();
    _listBingoQuestBlock.clear();
}

EventBingoManager::~EventBingoManager(void)
{
    _listBoardBlock.clear();
    _listLineRewardBlock.clear();
    _listBingoQuestBlock.clear();
}

bool EventBingoManager::init()
{
    EventManager::getInstance()->_onRequestInfo += new MafFunction<void(const int)>(CC_CALLBACK_1(EventBingoManager::onEventActivate, this), this);
    return true;
}
#pragma mark - get

cocos2d::Vector<InfoBingoBlock*> EventBingoManager::getListBoardBlock()
{
    return _listBoardBlock;
}

cocos2d::Vector<InfoBingoBlock*> EventBingoManager::getListLineRewardBlock()
{
    return _listLineRewardBlock;
}

cocos2d::Vector<InfoBingoBlock*> EventBingoManager::getListBingoQuestBlock()
{
    return _listBingoQuestBlock;
}

InfoBingoBlock* EventBingoManager::getBoardBlock(int nNum)
{
    InfoBingoBlock* result = nullptr;
    
    for(auto info : _listBoardBlock)
    {
        if(info->getIdx() == nNum)
        {
            result = info;
            break;
        }
    }
    
    return result;
}

InfoBingoBlock* EventBingoManager::getLineRewardBlock(int nNum)
{
    InfoBingoBlock* result = nullptr;
    
    for(auto info : _listLineRewardBlock)
    {
        if(info->getNum() == nNum)
        {
            result = info;
            break;
        }
    }
    
    return result;
}

InfoBingoBlock* EventBingoManager::getQuestRewardBlock(int nNum)
{
    InfoBingoBlock* result = nullptr;
    
    for(auto info : _listBingoQuestBlock)
    {
        if(info->getNum() == nNum)
        {
            result = info;
            break;
        }
    }
    
    return result;
}

cocos2d::Vector<InfoBingoBlock*> EventBingoManager::getListBoardBlockLine(std::vector<int> vecIdx)
{
    Vector<InfoBingoBlock*> result;
    result.clear();
    
    for(auto idx : vecIdx)
    {
        result.pushBack(getBoardBlock(idx));
    }
    
    return result;
}

bool EventBingoManager::isRewardPossible(int idx)
{
    bool result = false;
    
    auto vecLine1 = std::vector<int>{1,2,3,4,5};
    auto vecLine2 = std::vector<int>{6,7,8,9,10};
    auto vecLine3 = std::vector<int>{11,12,13,14,15};
    auto vecLine4 = std::vector<int>{16,17,18,19,20};
    auto vecLine5 = std::vector<int>{21,22,23,24,25};
    
    auto vecRow1 = std::vector<int>{1,6,11,16,21};
    auto vecRow2 = std::vector<int>{2,7,12,17,22};
    auto vecRow3 = std::vector<int>{3,8,13,18,23};
    auto vecRow4 = std::vector<int>{4,9,14,19,24};
    auto vecRow5 = std::vector<int>{5,10,15,20,25};
    
    auto vecdiagonal1 = std::vector<int>{1,7,13,19,25};
    auto vecdiagonal2 = std::vector<int>{5,9,13,17,21};
    
    int count = 0;
    
    cocos2d::Vector<InfoBingoBlock*> list;
    //row
    if(idx == 26)
    {
        list = getListBoardBlockLine(vecRow1);
    }
    else if(idx == 27)
    {
        list = getListBoardBlockLine(vecRow2);
    }
    else if(idx == 28)
    {
        list = getListBoardBlockLine(vecRow3);
    }
    else if(idx == 29)
    {
        list = getListBoardBlockLine(vecRow4);
        
    }
    else if(idx == 30)
    {
        list = getListBoardBlockLine(vecRow5);
    }
    //line
    else if(idx == 31)
    {
        list = getListBoardBlockLine(vecLine1);
    }
    else if(idx == 32)
    {
        list = getListBoardBlockLine(vecLine2);
    }
    else if(idx == 33)
    {
        list = getListBoardBlockLine(vecLine3);
    }
    else if(idx == 34)
    {
        list = getListBoardBlockLine(vecLine4);
    }
    else if(idx == 35)
    {
        list = getListBoardBlockLine(vecLine5);
    }
    //diagonal
    else if(idx == 36)
    {
        list = getListBoardBlockLine(vecdiagonal1);
    }
    else if(idx == 37)
    {
        list = getListBoardBlockLine(vecdiagonal2);
    }
    
    for(auto obj : list)
    {
        if(obj->isReward()) count++;
    }
    
    if(count >= 5)
        result = true;
    
    return result;
}

bool EventBingoManager::isQuestRewardPossible(int idx)
{
    bool result = false;
    
    if(_nBingoCompleteCount >= idx)
        result = true;
    
    return result;
}

bool EventBingoManager::isRoundEnd()
{
    
    bool result = true;
    for(auto info : _listLineRewardBlock)
    {
        if(info->isReward() == false)
        {
            result = false;
            break;
        }
    }
    
    return result;
}

int EventBingoManager::getBingoCount()
{
    int result = 0;
    
    for(auto info : _listLineRewardBlock)
    {
        if(isRewardPossible(info->getNum()))
            result++;
    }
    
    return result;
}
int EventBingoManager::getBingoCompleteCount()
{
    return _nBingoCompleteCount;
}
int EventBingoManager::getBingoTicketCount()
{
    return _nBingoTicketCount;
}

void EventBingoManager::setBingoTicketCount(int nValue)
{
    _nBingoTicketCount = nValue;
}

int EventBingoManager::getEventIdx()
{
    return _nEventIdx;
}

int EventBingoManager::getEventStartTime()
{
    return _nEventStartTime;
}

int EventBingoManager::getEventEndTime()
{
    return _nEventEndTime;
}
int EventBingoManager::getEventTotalEndTime()
{
    return _nTotalEndTime;
}

#pragma mark - network
void EventBingoManager::requestInfo(const std::function<void(bool, int)>& callback)
{
    _callbackInfo = callback;
    
    //
    std::string url = "/event/bingo/info/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(response,dd);
    };
    req->send(cb);
}

void EventBingoManager::responseInfo(cocos2d::network::HttpResponse* response,std::string &data)
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
    if ( nResult != 1 )
    {
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false,  nResult);
        }
        return;
    }
    _isEventActivate = true;
    
    _listBoardBlock.clear();
    _listLineRewardBlock.clear();
    _listBingoQuestBlock.clear();
    
    _nBingoCompleteCount = jsonParser["_bingo_complete"].GetInt();
    
    if(jsonParser.HasMember("_items"))
    {
        std::string strItems = jsonParser["_items"].GetString();
        auto listItems = ItemsManager::getInstance()->getConvertArray(strItems);
        
        for(int i = 0; i < listItems.size(); ++i)
        {
            auto objItem = listItems.at(i);
            if(objItem == nullptr)
                continue;
            
            if(objItem->getIdx() == 81)
                _nBingoTicketCount = std::atoi(objItem->getCount().c_str());
        }
        
        ItemsManager::getInstance()->setItemsServer(strItems);
    }
    
    auto listBlockData = jsonParser["_board"].GetArray();
    
    for(int i = 0; i < listBlockData.Size(); ++i)
    {
        const rapidjson::Value& data = listBlockData[i];
        
        int num = data["num"].GetInt();
        int isReward = data["reward"].GetBool();
        
        auto info = InfoBingoBlock::create();
        info->setType(InfoBingoBlock::E_TYPE::BOARD_BLOCK);
        info->setIdx(i + 1);
        info->setNum(num);
        info->setIsReward(isReward);
        _listBoardBlock.pushBack(info);
        
        if(data.HasMember("reward_item") == true)
        {
            auto item = data["reward_item"].GetObject();
            
            auto objItem = ItemsManager::getInstance()->getInfoItems(item["idx"].GetInt());
            objItem->setTypeParams(item["type_params"].GetInt());
            objItem->setCount(item["count"].GetInt());
            info->setItem(objItem);
        }
    }
    
    listBlockData = jsonParser["_bingo_reward"].GetArray();
    
    for(int i = 0; i < listBlockData.Size(); ++i)
    {
        const rapidjson::Value& data = listBlockData[i];
        
        int num = data["num"].GetInt();
        int isReward = data["reward"].GetBool();
        auto item = data["reward_item"].GetObject();
        
        auto objItem = ItemsManager::getInstance()->getInfoItems(item["idx"].GetInt());
        objItem->setTypeParams(item["type_params"].GetInt());
        objItem->setCount(item["count"].GetInt());
        
        auto info = InfoBingoBlock::create();
        info->setType(InfoBingoBlock::E_TYPE::LINE_REWARD_BLOCK);
        info->setIdx(i + 1);
        info->setNum(num);
        info->setIsReward(isReward);
        info->setItem(objItem);
        
        _listLineRewardBlock.pushBack(info);
    }
    
    listBlockData = jsonParser["_quest"].GetArray();
    
    for(int i = 0; i < listBlockData.Size(); ++i)
    {
        const rapidjson::Value& data = listBlockData[i];
        
        int num = data["idx"].GetInt();
        int questCount = data["quest_count"].GetInt();
        int isReward = data["reward"].GetBool();
        auto item = data["reward_item"].GetObject();
        
        auto objItem = ItemsManager::getInstance()->getInfoItems(item["idx"].GetInt());
        objItem->setTypeParams(item["type_params"].GetInt());
        objItem->setCount(item["count"].GetInt());
        
        auto info = InfoBingoBlock::create();
        info->setType(InfoBingoBlock::E_TYPE::LINE_REWARD_BLOCK);
        info->setIdx(i + 1);
        info->setQuestCount(questCount);
        info->setNum(num);
        info->setIsReward(isReward);
        info->setItem(objItem);
        
        _listBingoQuestBlock.pushBack(info);
    }
    
    auto eventData = jsonParser["_event_info"].GetObject();
    
    _nEventIdx = eventData["_idx"].GetInt();
    _nEventStartTime = eventData["_event_start"].GetInt();
    _nEventEndTime = eventData["_event_end"].GetInt();
    if(_nEventIdx == 24)
        _nTotalEndTime = jsonParser["_end_time"].GetInt();
    
    if ( _callbackInfo != nullptr )
    {
        _callbackInfo(true,  nResult);
    }
}

void EventBingoManager::requestUseTicket(const std::function<void(bool, int, int,  Vector<InfoItems*>)>& callback)
{
    _callbackUseTicket = callback;
    
    //
    std::string url = "/event/bingo/useTicket/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseUseTicket(response,dd);
    };
    req->send(cb);
}

void EventBingoManager::responseUseTicket(cocos2d::network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    Vector<InfoItems*> listReward;
    listReward.clear();
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
        if ( _callbackUseTicket != nullptr )
        {
            _callbackUseTicket(false,  -1, 0, listReward);
        }
        return;
    }
    
//    1 : 성공
//    2 : 이벤트 진행중아님
//    4 : 테이블 정보 없음
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackUseTicket != nullptr )
        {
            _callbackUseTicket(false,  nResult, 0, listReward);
        }
        return;
    }
    
    _listBoardBlock.clear();
    _listLineRewardBlock.clear();
    _listBingoQuestBlock.clear();
    
    _nBingoCompleteCount = jsonParser["_bingo_complete"].GetInt();
    
    if(jsonParser.HasMember("_reward"))
    {
        std::string strReward = jsonParser["_reward"].GetString();
        listReward = ItemsManager::getInstance()->getConvertArray(strReward);
        ItemsManager::getInstance()->addItems(listReward, false);
    }
    
    if(jsonParser.HasMember("_items"))
    {
        std::string strItems = jsonParser["_items"].GetString();
        auto listItems = ItemsManager::getInstance()->getConvertArray(strItems);
        
        for(int i = 0; i < listItems.size(); ++i)
        {
            auto objItem = listItems.at(i);
            if(objItem == nullptr)
                continue;
            
            if(objItem->getIdx() == 81)
                _nBingoTicketCount = std::atoi(objItem->getCount().c_str());
        }
        
        ItemsManager::getInstance()->setItemsServer(strItems);
    }
    
    auto listBlockData = jsonParser["_board"].GetArray();
    
    for(int i = 0; i < listBlockData.Size(); ++i)
    {
        const rapidjson::Value& data = listBlockData[i];
        
        int num = data["num"].GetInt();
        int isReward = data["reward"].GetBool();
        
        auto info = InfoBingoBlock::create();
        info->setType(InfoBingoBlock::E_TYPE::BOARD_BLOCK);
        info->setIdx(i + 1);
        info->setNum(num);
        info->setIsReward(isReward);
        _listBoardBlock.pushBack(info);
        
        if(data.HasMember("reward_item") == true)
        {
            auto item = data["reward_item"].GetObject();
            
            auto objItem = ItemsManager::getInstance()->getInfoItems(item["idx"].GetInt());
            objItem->setTypeParams(item["type_params"].GetInt());
            objItem->setCount(item["count"].GetInt());
            info->setItem(objItem);
        }
    }
    
    listBlockData = jsonParser["_bingo_reward"].GetArray();
    
    for(int i = 0; i < listBlockData.Size(); ++i)
    {
        const rapidjson::Value& data = listBlockData[i];
        
        int num = data["num"].GetInt();
        int isReward = data["reward"].GetBool();
        auto item = data["reward_item"].GetObject();
        
        auto objItem = ItemsManager::getInstance()->getInfoItems(item["idx"].GetInt());
        objItem->setTypeParams(item["type_params"].GetInt());
        objItem->setCount(item["count"].GetInt());
        
        auto info = InfoBingoBlock::create();
        info->setType(InfoBingoBlock::E_TYPE::LINE_REWARD_BLOCK);
        info->setIdx(i + 1);
        info->setNum(num);
        info->setIsReward(isReward);
        info->setItem(objItem);
        
        _listLineRewardBlock.pushBack(info);
    }
    
    listBlockData = jsonParser["_quest"].GetArray();
    
    for(int i = 0; i < listBlockData.Size(); ++i)
    {
        const rapidjson::Value& data = listBlockData[i];
        
        int num = data["idx"].GetInt();
        int questCount = data["quest_count"].GetInt();
        int isReward = data["reward"].GetBool();
        auto item = data["reward_item"].GetObject();
        
        auto objItem = ItemsManager::getInstance()->getInfoItems(item["idx"].GetInt());
        objItem->setTypeParams(item["type_params"].GetInt());
        objItem->setCount(item["count"].GetInt());
        
        auto info = InfoBingoBlock::create();
        info->setType(InfoBingoBlock::E_TYPE::LINE_REWARD_BLOCK);
        info->setIdx(i + 1);
        info->setQuestCount(questCount);
        info->setNum(num);
        info->setIsReward(isReward);
        info->setItem(objItem);
        
        _listBingoQuestBlock.pushBack(info);
    }
    
    int nBingoNum = jsonParser["_bingo_number"].GetInt();
    
    
    auto eventData = jsonParser["_event_info"].GetObject();
    
    _nEventIdx = eventData["_idx"].GetInt();
    _nEventStartTime = eventData["_event_start"].GetInt();
    _nEventEndTime = eventData["_event_end"].GetInt();
    if(_nEventIdx == 24)
        _nTotalEndTime = jsonParser["_end_time"].GetInt();
    
    if ( _callbackUseTicket != nullptr )
    {
        _callbackUseTicket(true,  nResult, nBingoNum, listReward);
    }
}


void EventBingoManager::requestReward(const std::function<void(bool, int)>& callback, int nNum)
{
    _callbackReward = callback;
    
    //
    std::string url = "/event/bingo/reward";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_num", nNum);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseReward(response,dd);
    };
    req->send(cb);
}

void EventBingoManager::responseReward(cocos2d::network::HttpResponse* response,std::string &data)
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
            _callbackReward(false,  -1);
        }
        return;
    }
    
//    0 : 유저 정보 없음
//    1 : 성공
//    2 : 이벤트 가능 시간이 아님
//    3 : 해당하는 보상이 없음
//    4 : 빙고가 아님
//    5 : 보상을 이미 받음
//    8 : 테이블 정보없음
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackReward != nullptr )
        {
            _callbackReward(false,  nResult);
        }
        return;
    }
    
    _listBoardBlock.clear();
    _listLineRewardBlock.clear();
    _listBingoQuestBlock.clear();
    
    _nBingoCompleteCount = jsonParser["_bingo_complete"].GetInt();
    
    if(jsonParser.HasMember("_reward"))
    {
        std::string strReward = jsonParser["_reward"].GetString();
        auto listReward = ItemsManager::getInstance()->getConvertArray(strReward);
        ItemsManager::getInstance()->addItems(listReward);
    }
    
    if(jsonParser.HasMember("_items"))
    {
        std::string strItems = jsonParser["_items"].GetString();
        auto listItems = ItemsManager::getInstance()->getConvertArray(strItems);
        
        for(int i = 0; i < listItems.size(); ++i)
        {
            auto objItem = listItems.at(i);
            if(objItem == nullptr)
                continue;
            
            if(objItem->getIdx() == 81)
                _nBingoTicketCount = std::atoi(objItem->getCount().c_str());
        }
        
        ItemsManager::getInstance()->setItemsServer(strItems);
    }
    
    auto listBlockData = jsonParser["_board"].GetArray();
    
    for(int i = 0; i < listBlockData.Size(); ++i)
    {
        const rapidjson::Value& data = listBlockData[i];
        
        int num = data["num"].GetInt();
        int isReward = data["reward"].GetBool();
        
        auto info = InfoBingoBlock::create();
        info->setType(InfoBingoBlock::E_TYPE::BOARD_BLOCK);
        info->setIdx(i + 1);
        info->setNum(num);
        info->setIsReward(isReward);
        _listBoardBlock.pushBack(info);
        
        if(data.HasMember("reward_item") == true)
        {
            auto item = data["reward_item"].GetObject();
            
            auto objItem = ItemsManager::getInstance()->getInfoItems(item["idx"].GetInt());
            objItem->setTypeParams(item["type_params"].GetInt());
            objItem->setCount(item["count"].GetInt());
            info->setItem(objItem);
        }
    }
    
    listBlockData = jsonParser["_bingo_reward"].GetArray();
    
    for(int i = 0; i < listBlockData.Size(); ++i)
    {
        const rapidjson::Value& data = listBlockData[i];
        
        int num = data["num"].GetInt();
        int isReward = data["reward"].GetBool();
        auto item = data["reward_item"].GetObject();
        
        auto objItem = ItemsManager::getInstance()->getInfoItems(item["idx"].GetInt());
        objItem->setTypeParams(item["type_params"].GetInt());
        objItem->setCount(item["count"].GetInt());
        
        auto info = InfoBingoBlock::create();
        info->setType(InfoBingoBlock::E_TYPE::LINE_REWARD_BLOCK);
        info->setIdx(i + 1);
        info->setNum(num);
        info->setIsReward(isReward);
        info->setItem(objItem);
        
        _listLineRewardBlock.pushBack(info);
    }
    
    listBlockData = jsonParser["_quest"].GetArray();
    
    for(int i = 0; i < listBlockData.Size(); ++i)
    {
        const rapidjson::Value& data = listBlockData[i];
        
        int num = data["idx"].GetInt();
        int questCount = data["quest_count"].GetInt();
        int isReward = data["reward"].GetBool();
        auto item = data["reward_item"].GetObject();
        
        auto objItem = ItemsManager::getInstance()->getInfoItems(item["idx"].GetInt());
        objItem->setTypeParams(item["type_params"].GetInt());
        objItem->setCount(item["count"].GetInt());
        
        auto info = InfoBingoBlock::create();
        info->setType(InfoBingoBlock::E_TYPE::LINE_REWARD_BLOCK);
        info->setIdx(i + 1);
        info->setQuestCount(questCount);
        info->setNum(num);
        info->setIsReward(isReward);
        info->setItem(objItem);
        
        _listBingoQuestBlock.pushBack(info);
    }
    
    
    auto eventData = jsonParser["_event_info"].GetObject();
    
    _nEventIdx = eventData["_idx"].GetInt();
    _nEventStartTime = eventData["_event_start"].GetInt();
    _nEventEndTime = eventData["_event_end"].GetInt();
    if(_nEventIdx == 24)
        _nTotalEndTime = jsonParser["_end_time"].GetInt();
    
    if ( _callbackReward != nullptr )
    {
        _callbackReward(true,  nResult);
    }
}

void EventBingoManager::requestQuestReward(const std::function<void(bool, int)>& callback, int nNum)
{
    _callbackQuestReward = callback;
    
    //
    std::string url = "/event/bingo/questReward";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_idx", nNum);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseQuestReward(response,dd);
    };
    req->send(cb);
}

void EventBingoManager::responseQuestReward(cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackQuestReward != nullptr )
        {
            _callbackQuestReward(false,  -1);
        }
        return;
    }
    
//    0 : 유저 정보 없음
//    1 : 성공
//    2 : 이벤트 가능 시간이 아님
//    3 : 해당하는 보상이 없음
//    4 : 빙고가 아님
//    5 : 보상을 이미 받음
//    8 : 테이블 정보없음
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackQuestReward != nullptr )
        {
            _callbackQuestReward(false,  nResult);
        }
        return;
    }
    
    _listBoardBlock.clear();
    _listLineRewardBlock.clear();
    _listBingoQuestBlock.clear();
    
    _nBingoCompleteCount = jsonParser["_bingo_complete"].GetInt();
    
    if(jsonParser.HasMember("_reward"))
    {
        std::string strReward = jsonParser["_reward"].GetString();
        auto listReward = ItemsManager::getInstance()->getConvertArray(strReward);
        ItemsManager::getInstance()->addItems(listReward);
    }
    
    if(jsonParser.HasMember("_items"))
    {
        std::string strItems = jsonParser["_items"].GetString();
        auto listItems = ItemsManager::getInstance()->getConvertArray(strItems);
        
        for(int i = 0; i < listItems.size(); ++i)
        {
            auto objItem = listItems.at(i);
            if(objItem == nullptr)
                continue;
            
            if(objItem->getIdx() == 81)
                _nBingoTicketCount = std::atoi(objItem->getCount().c_str());
        }
        
        ItemsManager::getInstance()->setItemsServer(strItems);
    }
    
    auto listBlockData = jsonParser["_board"].GetArray();
    
    for(int i = 0; i < listBlockData.Size(); ++i)
    {
        const rapidjson::Value& data = listBlockData[i];
        
        int num = data["num"].GetInt();
        int isReward = data["reward"].GetBool();
        
        auto info = InfoBingoBlock::create();
        info->setType(InfoBingoBlock::E_TYPE::BOARD_BLOCK);
        info->setIdx(i + 1);
        info->setNum(num);
        info->setIsReward(isReward);
        _listBoardBlock.pushBack(info);
        
        if(data.HasMember("reward_item") == true)
        {
            auto item = data["reward_item"].GetObject();
            
            auto objItem = ItemsManager::getInstance()->getInfoItems(item["idx"].GetInt());
            objItem->setTypeParams(item["type_params"].GetInt());
            objItem->setCount(item["count"].GetInt());
            info->setItem(objItem);
        }
    }
    
    listBlockData = jsonParser["_bingo_reward"].GetArray();
    
    for(int i = 0; i < listBlockData.Size(); ++i)
    {
        const rapidjson::Value& data = listBlockData[i];
        
        int num = data["num"].GetInt();
        int isReward = data["reward"].GetBool();
        auto item = data["reward_item"].GetObject();
        
        auto objItem = ItemsManager::getInstance()->getInfoItems(item["idx"].GetInt());
        objItem->setTypeParams(item["type_params"].GetInt());
        objItem->setCount(item["count"].GetInt());
        
        auto info = InfoBingoBlock::create();
        info->setType(InfoBingoBlock::E_TYPE::LINE_REWARD_BLOCK);
        info->setIdx(i + 1);
        info->setNum(num);
        info->setIsReward(isReward);
        info->setItem(objItem);
        
        _listLineRewardBlock.pushBack(info);
    }
    
    listBlockData = jsonParser["_quest"].GetArray();
    
    for(int i = 0; i < listBlockData.Size(); ++i)
    {
        const rapidjson::Value& data = listBlockData[i];
        
        int num = data["idx"].GetInt();
        int questCount = data["quest_count"].GetInt();
        int isReward = data["reward"].GetBool();
        auto item = data["reward_item"].GetObject();
        
        auto objItem = ItemsManager::getInstance()->getInfoItems(item["idx"].GetInt());
        objItem->setTypeParams(item["type_params"].GetInt());
        objItem->setCount(item["count"].GetInt());
        
        auto info = InfoBingoBlock::create();
        info->setType(InfoBingoBlock::E_TYPE::LINE_REWARD_BLOCK);
        info->setIdx(i + 1);
        info->setQuestCount(questCount);
        info->setNum(num);
        info->setIsReward(isReward);
        info->setItem(objItem);
        
        _listBingoQuestBlock.pushBack(info);
    }
    
    
    auto eventData = jsonParser["_event_info"].GetObject();
    
    _nEventIdx = eventData["_idx"].GetInt();
    _nEventStartTime = eventData["_event_start"].GetInt();
    _nEventEndTime = eventData["_event_end"].GetInt();
    if(_nEventIdx == 24)
        _nTotalEndTime = jsonParser["_end_time"].GetInt();
    
    if ( _callbackQuestReward != nullptr )
    {
        _callbackQuestReward(true,  nResult);
    }
}

void EventBingoManager::requestNextBoard(const std::function<void(bool, int)>& callback)
{
    _callbackNextBoard = callback;
    
    //
    std::string url = "/event/bingo/nextBoard/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseNextBoard(response,dd);
    };
    req->send(cb);
}

void EventBingoManager::responseNextBoard(cocos2d::network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    Vector<InfoItems*> listReward;
    listReward.clear();
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
        if ( _callbackNextBoard != nullptr )
        {
            _callbackNextBoard(false,  -1);
        }
        return;
    }
    
//    1 : 성공
//    2 : 이벤트 진행중아님
//    4 : 테이블 정보 없음
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackNextBoard != nullptr )
        {
            _callbackNextBoard(false,  nResult);
        }
        return;
    }
    
    _listBoardBlock.clear();
    _listLineRewardBlock.clear();
    _listBingoQuestBlock.clear();
    
    _nBingoCompleteCount = jsonParser["_bingo_complete"].GetInt();
    
    if(jsonParser.HasMember("_reward"))
    {
        std::string strReward = jsonParser["_reward"].GetString();
        listReward = ItemsManager::getInstance()->getConvertArray(strReward);
        ItemsManager::getInstance()->addItems(listReward, false);
    }
    
    if(jsonParser.HasMember("_items"))
    {
        std::string strItems = jsonParser["_items"].GetString();
        auto listItems = ItemsManager::getInstance()->getConvertArray(strItems);
        
        for(int i = 0; i < listItems.size(); ++i)
        {
            auto objItem = listItems.at(i);
            if(objItem == nullptr)
                continue;
            
            if(objItem->getIdx() == 81)
                _nBingoTicketCount = std::atoi(objItem->getCount().c_str());
        }
        
        ItemsManager::getInstance()->setItemsServer(strItems);
    }
    
    auto listBlockData = jsonParser["_board"].GetArray();
    
    for(int i = 0; i < listBlockData.Size(); ++i)
    {
        const rapidjson::Value& data = listBlockData[i];
        
        int num = data["num"].GetInt();
        int isReward = data["reward"].GetBool();
        
        auto info = InfoBingoBlock::create();
        info->setType(InfoBingoBlock::E_TYPE::BOARD_BLOCK);
        info->setIdx(i + 1);
        info->setNum(num);
        info->setIsReward(isReward);
        _listBoardBlock.pushBack(info);
        
        if(data.HasMember("reward_item") == true)
        {
            auto item = data["reward_item"].GetObject();
            
            auto objItem = ItemsManager::getInstance()->getInfoItems(item["idx"].GetInt());
            objItem->setTypeParams(item["type_params"].GetInt());
            objItem->setCount(item["count"].GetInt());
            info->setItem(objItem);
        }
    }
    
    listBlockData = jsonParser["_bingo_reward"].GetArray();
    
    for(int i = 0; i < listBlockData.Size(); ++i)
    {
        const rapidjson::Value& data = listBlockData[i];
        
        int num = data["num"].GetInt();
        int isReward = data["reward"].GetBool();
        auto item = data["reward_item"].GetObject();
        
        auto objItem = ItemsManager::getInstance()->getInfoItems(item["idx"].GetInt());
        objItem->setTypeParams(item["type_params"].GetInt());
        objItem->setCount(item["count"].GetInt());
        
        auto info = InfoBingoBlock::create();
        info->setType(InfoBingoBlock::E_TYPE::LINE_REWARD_BLOCK);
        info->setIdx(i + 1);
        info->setNum(num);
        info->setIsReward(isReward);
        info->setItem(objItem);
        
        _listLineRewardBlock.pushBack(info);
    }
    
    listBlockData = jsonParser["_quest"].GetArray();
    
    for(int i = 0; i < listBlockData.Size(); ++i)
    {
        const rapidjson::Value& data = listBlockData[i];
        
        int num = data["idx"].GetInt();
        int questCount = data["quest_count"].GetInt();
        int isReward = data["reward"].GetBool();
        auto item = data["reward_item"].GetObject();
        
        auto objItem = ItemsManager::getInstance()->getInfoItems(item["idx"].GetInt());
        objItem->setTypeParams(item["type_params"].GetInt());
        objItem->setCount(item["count"].GetInt());
        
        auto info = InfoBingoBlock::create();
        info->setType(InfoBingoBlock::E_TYPE::LINE_REWARD_BLOCK);
        info->setIdx(i + 1);
        info->setQuestCount(questCount);
        info->setNum(num);
        info->setIsReward(isReward);
        info->setItem(objItem);
        
        _listBingoQuestBlock.pushBack(info);
    }
    
    auto eventData = jsonParser["_event_info"].GetObject();
    
    _nEventIdx = eventData["_idx"].GetInt();
    _nEventStartTime = eventData["_event_start"].GetInt();
    _nEventEndTime = eventData["_event_end"].GetInt();
    
    if(_nEventIdx == 24)
        _nTotalEndTime = jsonParser["_end_time"].GetInt();
    
    if ( _callbackNextBoard != nullptr )
    {
        _callbackNextBoard(true,  nResult);
    }
}

void EventBingoManager::onEventActivate(const int nIdx)
{
    if(nIdx == 24 && !_isEventActivate)
    {
        PopupLoading::show();
        requestInfo([=](bool, int){
            PopupLoading::hide();
        });
        EventManager::getInstance()->AddActivateRequestCount();
    }
}
