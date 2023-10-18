//
//  EventVoteManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/24.
//
#include "EventVoteManager.h"

#include "Common/ConfigGameNormal.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

EventVoteManager::EventVoteManager(void) :
_isEventActivate(false)
,_nSeason(0)
,_nResult(0)
,_isBreakTime(false)
,_nCurrentCandidateGroupNumber(0)
,_nVoteAllCount(0)
,_nVoteCoin(0)
,_nEndEventTime(0)
,_nEndVoteTime(0)
,_adsCount(0)
,_delegateInfo(NULL)
,_callbackInfo(nullptr)
,_callbackVote(nullptr)
,_callbackAds(nullptr)
{
    _listInfoVoteAll.clear();
    _listInfoVoteCandidate.clear();
    _listInfoPrevVoteCandidate.clear();
    _listInfoVoteCandidateReward.clear();
}

EventVoteManager::~EventVoteManager(void)
{
    _delegateInfo.Clear();
    _delegateVote.Clear();
    _delegateAds.Clear();
    _listInfoVoteAll.clear();
    _listInfoPrevVoteCandidate.clear();
    _listInfoPrevVoteCandidate.clear();
    _listInfoVoteCandidateReward.clear();
}

bool EventVoteManager::init()
{
    EventManager::getInstance()->_onRequestInfo += new MafFunction<void(const int)>(CC_CALLBACK_1(EventVoteManager::onEventActivate, this), this);
    return true;
}

#pragma mark - update
void EventVoteManager::update(float dt)
{
    
}

#pragma mark - load
void EventVoteManager::setLoad()
{
    setLoadVoteAll();
    setLoadVoteCandidateReward();
}

void EventVoteManager::setLoadVoteAll()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::EVENT_VOTE_ALL);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)(const char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _listInfoVoteAll.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        const int idx = jsonValue["idx"].GetInt();
        const int count = jsonValue["count"].GetInt();
        const std::string reward = jsonValue["reward"].GetString();
        
        auto obj = InfoEventVoteAll::create();
        obj->setIdx(idx);
        obj->setCount(count);
        obj->setReward(reward);
        
        //
        _listInfoVoteAll.pushBack(obj);
    }
}

void EventVoteManager::setLoadVoteCandidateReward()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::EVENT_VOTE_PERSONAL);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)(const char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _listInfoVoteCandidateReward.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        const int idx = jsonValue["idx"].GetInt();
        
        auto obj = InfoEventVoteCandidateReward::create();
        obj->setIdx(idx);
        for(int j = 1; j <= 5; ++j)
        {
            std::string key = "reward_" + std::to_string(j);
            const std::string reward = jsonValue[key.c_str()].GetString();
            obj->insertReward(j, reward);
        }
        //
        _listInfoVoteCandidateReward.pushBack(obj);
    }
}

#pragma mark - set, get : info
cocos2d::Vector<InfoEventVoteAll*> EventVoteManager::getListVoteAll()
{
    return _listInfoVoteAll;
}
cocos2d::Vector<InfoEventVoteCandidate*> EventVoteManager::getListVoteCandidate()
{
    return _listInfoVoteCandidate;
}
cocos2d::Vector<InfoEventVoteCandidate*> EventVoteManager::getListPrevVoteCandidate()
{
    return _listInfoPrevVoteCandidate;
}
InfoEventVoteAll* const EventVoteManager::getInfoVoteAll(const int idx) const
{
    for ( auto& obj : _listInfoVoteAll )
    {
        if ( idx == obj->getIdx())
            return obj;
    }
    
    return nullptr;
}
InfoEventVoteCandidate* const EventVoteManager::getInfoVoteCandidate(const int idx) const
{
    for ( auto& obj : _listInfoVoteCandidate )
    {
        if ( idx == obj->getIdx())
            return obj;
    }
    
    return nullptr;
}
InfoEventVoteCandidateReward* const EventVoteManager::getInfoVoteCandidateReward(const int idx) const
{
    for ( auto& obj : _listInfoVoteCandidateReward )
    {
        if ( idx == obj->getIdx())
            return obj;
    }
    
    return nullptr;
}
#pragma mark - set, get :
const int EventVoteManager::getResultCode()const
{
    return _nResult;
}
const bool EventVoteManager::isBreakTime()const
{
    return _isBreakTime;
}
const int EventVoteManager::getAllVoteCount()
{
    return _nVoteAllCount;
}
const float EventVoteManager::getPercentageOfCurrentVotes(const int idx)
{
    int allCount = 0;
    for(auto obj : _listInfoVoteCandidate)
    {
        allCount += obj->getPersnalVoteAllCount();
    }
    
    return getPercentageOfVotes(getInfoVoteCandidate(idx), allCount);
}

const float EventVoteManager::getPercentageOfVotes(InfoEventVoteCandidate* objCandidate, const int nAllCount)
{
    if(objCandidate == nullptr || nAllCount == 0)
        return 0;
    
    return objCandidate->getPersnalVoteAllCount() / (float)nAllCount * 100;
}

const int EventVoteManager::getVoteCoinCount()
{
    return _nVoteCoin;
}

void EventVoteManager::setVoteCoinCount(const int value)
{
    _nVoteCoin = value;
}

const int64_t EventVoteManager::getEndEventTime()
{
    return _nEndEventTime;
}


const int64_t EventVoteManager::getEndVoteTime()
{
    return _nEndVoteTime;
}

const int EventVoteManager::getAdsCount()
{
    return _adsCount;
}
const int EventVoteManager::getCurrentCandidateGroupNumber()
{
    return _nCurrentCandidateGroupNumber;
}
const int EventVoteManager::getCandidateRank(InfoEventVoteCandidate* candidate)
{
    int result = 1;
    for(auto obj : _listInfoVoteCandidate)
    {
        if(obj == candidate) continue;
        
        if(obj->getPersnalVoteAllCount() < candidate->getPersnalVoteAllCount())
            result++;
        else if(obj->getPersnalVoteAllCount() == candidate->getPersnalVoteAllCount() && candidate->getIdx() < obj->getIdx())
            result++;
            
    }
    
    return result;
}
#pragma mark - set, get :

#pragma mark - game

#pragma mark - delegate
void EventVoteManager::subDelegateInfo(MafFunction<void(bool, int)>* func)
{
    _delegateInfo += func;
}
void EventVoteManager::invokeDelegateInfo(bool bResult, int nResult)
{
    _delegateInfo(bResult, nResult);
}
void EventVoteManager::unSubDelegateInfo(void* obj)
{
    _delegateInfo.Clear(obj);
}

void EventVoteManager::subDelegateVote(MafFunction<void(bool, int)>* func)
{
    _delegateVote += func;
}
void EventVoteManager::invokeDelegateVote(bool bResult, int nResult)
{
    _delegateVote(bResult, nResult);
}
void EventVoteManager::unSubDelegateVote(void* obj)
{
    _delegateVote.Clear(obj);
}

void EventVoteManager::subDelegateAds(MafFunction<void(bool, int)>* func)
{
    _delegateAds += func;
}
void EventVoteManager::invokeDelegateAds(bool bResult, int nResult)
{
    _delegateAds(bResult, nResult);
}
void EventVoteManager::unSubDelegateAds(void* obj)
{
    _delegateAds.Clear(obj);
}
#pragma mark - network : info
void EventVoteManager::requestInfo(bool bPopupLoading, const std::function<void(bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackInfo = callback;
    
    //
    std::string url = "/event/vote/info/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void EventVoteManager::responseInfo(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false, -1);
            _callbackInfo = nullptr;
        }
        return;
    }
    
    _nResult = jsonParser["_result"].GetInt();
    /*
     _RESULT_AD_MAX           = 10004            # 광고 최대치

     _RESULT_ALREADY_REWARD         = 20000     # 이미 보상받음
     _RESULT_EVENT_CANT_VOTE        = 20ㅊ001     # 투표시간이 아님 (정산시간)
     _RESULT_EVENT_TICKET_EMPTY     = 20002     # 티켓이 없음
     _RESULT_EVENT_EMPTY_JOIN       = 20003     # 이벤트 참여한 이력이없음
     _RESULT_EVENT_REWARD_FAIL      = 20004     # 이벤트 조건 충족하지못함
     */
    if ( _nResult == 1 || _nResult == 20001)
    {
        _isEventActivate = true;
        if(_nResult == 20001)
            _isBreakTime = true;
        else
            _isBreakTime = false;
        
        _nSeason = jsonParser["_season"].GetInt();
        _adsCount = jsonParser["_ad_count"].GetInt();
        _nEndEventTime = jsonParser["_end_event"].GetInt();
        _nEndVoteTime = jsonParser["_end_vote"].GetInt();
        _nVoteAllCount = jsonParser["_all_count"].GetInt();
        _nCurrentCandidateGroupNumber = jsonParser["_current_candidate_group_number"].GetInt();
        
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        _listInfoVoteCandidate.clear();
        
        auto listCandidate = jsonParser["_votes"].GetArray();
        for ( int i = 0; i < listCandidate.Size(); i++ )
        {
            const rapidjson::Value& jsonValue = listCandidate[i];
            
            int idx = jsonValue["idx"].GetInt();
            int count = jsonValue["count"].GetInt();
            int allCount = jsonValue["all_count"].GetInt();
            
            auto info = InfoEventVoteCandidate::create();
            info->setIdx(idx);
            info->setVoteCount(count);
            info->setPersnalVoteAllCount(allCount);
            _listInfoVoteCandidate.pushBack(info);
        }
        
        _listInfoPrevVoteCandidate.clear();
        auto listPrevCandidate = jsonParser["_prev_vote"].GetArray();
        for ( int i = 0; i < listPrevCandidate.Size(); i++ )
        {
            const rapidjson::Value& jsonValue = listPrevCandidate[i];
            
            int idx = jsonValue["idx"].GetInt();
            int allCount = jsonValue["all_count"].GetInt();
            
            auto info = InfoEventVoteCandidate::create();
            info->setIdx(idx);
            info->setPersnalVoteAllCount(allCount);
            info->setIsPrevCandidate(true);
            _listInfoPrevVoteCandidate.pushBack(info);
        }
        
        //
        invokeDelegateInfo(true, _nResult);
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(true, _nResult);
            _callbackInfo = nullptr;
        }
    }
    else
    {
        //
        invokeDelegateInfo(false, _nResult);
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false, _nResult);
            _callbackInfo = nullptr;
        }
    }
}
#pragma mark - network : Voting
void EventVoteManager::requestVoting(bool bPopupLoading, const int idx, const int count, const std::function<void(bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackVote = callback;
    
    //
    std::string url = "/event/vote/vote";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_vote_idx", idx);
    req->setData("_count", count);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseVoting(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void EventVoteManager::responseVoting(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
        _delegateVote(false, -1);
        if ( _callbackVote != nullptr )
        {
            _callbackVote(false, -1);
            _callbackVote = nullptr;
        }
        return;
    }
    
    _nResult = jsonParser["_result"].GetInt();
    /*
     _RESULT_AD_MAX           = 10004            # 광고 최대치

     _RESULT_ALREADY_REWARD         = 20000     # 이미 보상받음
     _RESULT_EVENT_CANT_VOTE        = 20001     # 투표시간이 아님 (정산시간)
     _RESULT_EVENT_TICKET_EMPTY     = 20002     # 티켓이 없음
     _RESULT_EVENT_EMPTY_JOIN       = 20003     # 이벤트 참여한 이력이없음
     _RESULT_EVENT_REWARD_FAIL      = 20004     # 이벤트 조건 충족하지못함
     */
    if ( _nResult == 1 || _nResult == 20001)
    {
        if(_nResult == 20001)
            _isBreakTime = true;
        else
            _isBreakTime = false;
        
        _nSeason = jsonParser["_season"].GetInt();
        _nEndEventTime = jsonParser["_end_event"].GetInt();
        _nEndVoteTime = jsonParser["_end_vote"].GetInt();
        _nVoteAllCount = jsonParser["_all_count"].GetInt();
        
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        _listInfoVoteCandidate.clear();
        
        auto listCandidate = jsonParser["_votes"].GetArray();
        for ( int i = 0; i < listCandidate.Size(); i++ )
        {
            const rapidjson::Value& jsonValue = listCandidate[i];
            
            int idx = jsonValue["idx"].GetInt();
            int count = jsonValue["count"].GetInt();
            int allCount = jsonValue["all_count"].GetInt();
            
            auto info = InfoEventVoteCandidate::create();
            info->setIdx(idx);
            info->setVoteCount(count);
            info->setPersnalVoteAllCount(allCount);
            _listInfoVoteCandidate.pushBack(info);
        }
        //
        _delegateVote(true, _nResult);
        if ( _callbackVote != nullptr )
        {
            _callbackVote(true, _nResult);
            _callbackVote = nullptr;
        }
    }
    else
    {
        //
        _delegateVote(false, _nResult);
        if ( _callbackVote != nullptr )
        {
            _callbackVote(false, _nResult);
            _callbackVote = nullptr;
        }
    }
}

#pragma mark - network : Ads
void EventVoteManager::requestAds(bool bPopupLoading, const std::function<void(bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackAds = callback;
    
    //
    std::string url = "/ads/finish";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", 2);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseAds(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void EventVoteManager::responseAds(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
        _delegateAds(false, -1);
        if ( _callbackAds != nullptr )
        {
            _callbackAds(false, -1);
            _callbackAds = nullptr;
        }
        return;
    }
    
    int result = jsonParser["_result"].GetInt();
    /*
     _RESULT_AD_MAX           = 10004            # 광고 최대치

     _RESULT_ALREADY_REWARD         = 20000     # 이미 보상받음
     _RESULT_EVENT_CANT_VOTE        = 20001     # 투표시간이 아님 (정산시간)
     _RESULT_EVENT_TICKET_EMPTY     = 20002     # 티켓이 없음
     _RESULT_EVENT_EMPTY_JOIN       = 20003     # 이벤트 참여한 이력이없음
     _RESULT_EVENT_REWARD_FAIL      = 20004     # 이벤트 조건 충족하지못함
     */
    if ( result == 1 || result == 20001)
    {
        if(result == 20001)
            _isBreakTime = true;
        else
            _isBreakTime = false;
        
        _delegateAds(true, result);
        if ( _callbackAds != nullptr )
        {
            _callbackAds(true, result);
            _callbackAds = nullptr;
        }
    }
    else
    {
        //
        _delegateAds(false, result);
        if ( _callbackAds != nullptr )
        {
            _callbackAds(false, result);
            _callbackAds = nullptr;
        }
    }
}


void EventVoteManager::onEventActivate(const int nIdx)
{
    if(nIdx == 37 && !_isEventActivate)
    {
        requestInfo(true);
        EventManager::getInstance()->AddActivateRequestCount();
    }
}
