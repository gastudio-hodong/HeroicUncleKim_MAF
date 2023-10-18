//
//  EventLuckyBagManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/12/21.
//

#include "EventLuckyBagManager.h"

#include "Common/ConfigGameNormal.h"

#include "GameUIPopup/OtherEvent/EventLuckyBag/PopupLuckyBagDrawEffect.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

EventLuckyBagManager::EventLuckyBagManager(void) :
_isEventActivate(false)
,_isExtraTime(false)
,_isEffect(true)
,_nCount(0)
,_nLevel(0)
,_nLuckyBagCount(0)

,_nNowDrawRewardLv(0)

,_nAdsCount(0)
,_nAdsMax(0)
,_nAdsResetTime(0)

,_nEndEventTime(0)

,_callbackInfo(nullptr)
,_callbackProcess(nullptr)
,_callbackAds(nullptr)
{
    _mapMyReward.clear();
    _delegateInfo.Clear();
    _delegateProcess.Clear();
    _delegateAds.Clear();
}

EventLuckyBagManager::~EventLuckyBagManager(void)
{
    _mapMyReward.clear();
    _delegateInfo.Clear();
    _delegateProcess.Clear();
    _delegateAds.Clear();
}

bool EventLuckyBagManager::init()
{
    EventManager::getInstance()->_onRequestInfo += new MafFunction<void(const int)>(CC_CALLBACK_1(EventLuckyBagManager::onEventActivate, this), this);
    return true;
}

#pragma mark - update

#pragma mark - load
void EventLuckyBagManager::setLoad()
{
    setLoadLuckyBagInfo();
}

void EventLuckyBagManager::setLoadLuckyBagInfo()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::EVENT_LUCKYBAG_REWARD);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)(const char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }

    _listEventLuckyBagReward.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        if(const int isRewardSuccess = jsonValue["is_reward_success"].GetInt(); isRewardSuccess == 0)
            continue;
        
        const int idx = jsonValue["idx"].GetInt();
        const int lv = jsonValue["lv"].GetInt();
        const int rate = jsonValue["rate"].GetInt();
        const int itemIdx = jsonValue["item_idx"].GetInt();
        const int itemCount = jsonValue["item_count"].GetInt();
        const int itemTypeParams = jsonValue["type_params"].GetInt();

        auto obj = InfoEventLuckyBagReward::create();
        obj->setIdx(idx);
        obj->setLevel(lv);
        obj->setRate(rate);
        obj->setItemIdx(itemIdx);
        obj->setItemCount(itemCount);
        obj->setItemParam(itemTypeParams);

        //
        _listEventLuckyBagReward.pushBack(obj);
    }
    
    std::string strEventOtherText = EventManager::getInstance()->getEventOhterDataByEventIdx(38);
    if ( strEventOtherText.empty() == false )
    {
        rapidjson::Document jsonOtherDataParser;
        jsonOtherDataParser.ParseInsitu<0>((char*)strEventOtherText.c_str());
        if( jsonOtherDataParser.HasParseError() == false )
        {
            _nAdsMax = jsonOtherDataParser["ads_max_count"].GetInt();
        }
    }
}

#pragma mark - set, get :
cocos2d::Vector<InfoEventLuckyBagReward*> EventLuckyBagManager::getListEventLuckyBagRewardAll()
{
    return _listEventLuckyBagReward;
}
cocos2d::Vector<InfoEventLuckyBagReward*> EventLuckyBagManager::getListEventLuckyBagReward(int lv)
{
    if(lv == -1)
        lv = _nLevel + 1;
    cocos2d::Vector<InfoEventLuckyBagReward*> result;
    
    for(auto obj : _listEventLuckyBagReward)
    {
        if(obj->getLevel() == lv)
            result.pushBack(obj);
    }
    
    return result;
}
const bool EventLuckyBagManager::isExtraTime()const
{
    return _isExtraTime;
}
const bool EventLuckyBagManager::isEffect()const
{
    return _isEffect;
}

void EventLuckyBagManager::setIsEffect(const bool isActive)
{
    _isEffect = isActive;
}

const int EventLuckyBagManager::getCountNow()
{
    return _nCount.valueInt();
}
void EventLuckyBagManager::setCountNow(const int value)
{
    _nCount = value;
}
const int EventLuckyBagManager::getCountMax()
{
    int countMax = 0;
    
    std::string data = EventManager::getInstance()->getEventOhterDataByEventIdx(38);
    if ( data.empty() == false )
    {
        rapidjson::Document jsonParser;
        jsonParser.ParseInsitu<0>((char*)data.c_str());
        if( jsonParser.HasParseError() == false )
        {
            countMax = jsonParser["bundle_draw"].GetInt();
        }
    }
    
    return countMax;
}

const int EventLuckyBagManager::getCurrentLevel()
{
    return _nLevel.valueInt();
}
const int EventLuckyBagManager::getMaxLevel()
{
    int result = 0;
    for(auto obj : _listEventLuckyBagReward)
    {
        if(result < obj->getLevel())
            result = obj->getLevel();
    }
    return result;
}
const int EventLuckyBagManager::getLuckyBagCount()
{
    return _nLuckyBagCount.valueInt();
}
const int EventLuckyBagManager::getCurrentSuccessRate()
{
    auto list = getListEventLuckyBagReward();
    int result = 0;
    for(auto obj : list)
    {
        result += obj->getRate();
    }
    
    return result / 10000;
}
void EventLuckyBagManager::setLuckyBagCount(const int value)
{
    _nLuckyBagCount = value;
}

const int EventLuckyBagManager::getNowDrawRewardLv()
{
    return _nNowDrawRewardLv;
}
void EventLuckyBagManager::setNowDrawRewardLv(const int value)
{
    _nNowDrawRewardLv = value;
}
InfoItems* const EventLuckyBagManager::getMyRewardByLevel(const int level)
{
    auto iter = _mapMyReward.find(level);
    
    if(iter == _mapMyReward.end())
        return nullptr;
    
    return ItemsManager::getInstance()->getConvert(iter->second);
}

const int64_t EventLuckyBagManager::getEndEventTime()
{
    return _nEndEventTime;
}
const int EventLuckyBagManager::getAdsCount()
{
    return _nAdsCount;
}
const int EventLuckyBagManager::getAdsMaxCount()
{
    return _nAdsMax;
}
#pragma mark - set, get :

#pragma mark - game

#pragma mark - delegate
void EventLuckyBagManager::subDelegateInfo(MafFunction<void(bool, int)>* func)
{
    _delegateInfo += func;
}
void EventLuckyBagManager::invokeDelegateInfo(bool bResult, int nResult)
{
    _delegateInfo(bResult, nResult);
}
void EventLuckyBagManager::unSubDelegateInfo(void* obj)
{
    _delegateInfo.Clear(obj);
}

void EventLuckyBagManager::subDelegateProcess(MafFunction<void(bool, int)>* func)
{
    _delegateProcess += func;
}
void EventLuckyBagManager::invokeDelegateProcess(bool bResult, int nResult)
{
    _delegateProcess(bResult, nResult);
}
void EventLuckyBagManager::unSubDelegateProcess(void* obj)
{
    _delegateProcess.Clear(obj);
}

void EventLuckyBagManager::subDelegateAds(MafFunction<void(bool, int)>* func)
{
    _delegateAds += func;
}
void EventLuckyBagManager::invokeDelegateAds(bool bResult, int nResult)
{
    _delegateAds(bResult, nResult);
}
void EventLuckyBagManager::unSubDelegateAds(void* obj)
{
    _delegateAds.Clear(obj);
}
#pragma mark - network : info
void EventLuckyBagManager::requestInfo(bool bPopupLoading, const std::function<void(bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackInfo = callback;
    
    //
    std::string url = "/v1/event/luckybag/info/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void EventLuckyBagManager::responseInfo(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
    
    int result = jsonParser["_result"].GetInt();
    if ( result == 1)
    {
        _isEventActivate = true;
        _nCount = jsonParser["_count"].GetInt();
        _nLevel = jsonParser["_is_level"].GetInt();
        _nEndEventTime = jsonParser["_end_event"].GetInt();
        _nAdsCount = jsonParser["_ad_count"].GetInt();
        
        if(jsonParser.HasMember("_extra_time"))
            _isExtraTime = jsonParser["_extra_time"].GetBool();
        
        auto listMyReward = jsonParser["_possi_items"].GetArray();
        _mapMyReward.clear();
        for ( int i = 0; i < listMyReward.Size(); i++ )
        {
            const rapidjson::Value& jsonValue = listMyReward[i];
            
            int lv = jsonValue["lv"].GetInt();
            std::string strItemInfo = jsonValue["iteminfo"].GetString();
            
            _mapMyReward.insert(std::make_pair(lv, strItemInfo));
        }
        
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        //
        invokeDelegateInfo(true, result);
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(true, result);
            _callbackInfo = nullptr;
        }
    }
    else
    {
        //
        invokeDelegateInfo(false, result);
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false, result);
            _callbackInfo = nullptr;
        }
    }
}

#pragma mark - network : Voting
void EventLuckyBagManager::requestProceeLuckyBag(bool bPopupLoading, int type, int count, const std::function<void(bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackProcess = callback;
    
    //
    std::string url = "/v1/event/luckybag";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_status", type);
    req->setData("_count", count);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseProceeLuckyBag(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void EventLuckyBagManager::responseProceeLuckyBag(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
        _delegateProcess(false, -1);
        if ( _callbackProcess != nullptr )
        {
            _callbackProcess(false, -1);
            _callbackProcess = nullptr;
        }
        return;
    }
    
    int result = jsonParser["_result"].GetInt();
    int status = jsonParser["_status"].GetInt();
    if ( result == 1)
    {
        _nNowDrawRewardLv = jsonParser["_lv"].GetInt();
        
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        if(_nNowDrawRewardLv == getMaxLevel() && status == 1)
        {
            auto popup = PopupLuckyBagDrawEffect::create(PopupLuckyBagDrawEffect::E_TYPE::ENDING);
            popup->show();
        }
        
        if(jsonParser.HasMember("_recive_rewards"))
        {
            std::string strReward = jsonParser["_recive_rewards"].GetString();
            if(status == 1 && _isEffect)
            {
                auto list = ItemsManager::getInstance()->getConvertArray(strReward);
                if(list.size() > 0)
                {
                    ItemsManager::getInstance()->addItems(strReward, false);
                    auto popup = PopupLuckyBagDrawEffect::create(PopupLuckyBagDrawEffect::E_TYPE::FAIL, strReward);
                    popup->show();
                }
            }
            else
            {
                ItemsManager::getInstance()->addItems(strReward);
            }
        }
        //
        _delegateProcess(true, result);
        if ( _callbackProcess != nullptr )
        {
            _callbackProcess(true, result);
            _callbackProcess = nullptr;
        }
    }
    else
    {
        //
        _delegateProcess(false, result);
        if ( _callbackProcess != nullptr )
        {
            _callbackProcess(false, result);
            _callbackProcess = nullptr;
        }
    }
}


#pragma mark - network : Ads
void EventLuckyBagManager::requestAds(bool bPopupLoading, const std::function<void(bool, int)>& callback)
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
    req->setData("_type", 3);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseAds(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void EventLuckyBagManager::responseAds(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
    if ( result == 1)
    {
        _nAdsCount = jsonParser["_count"].GetInt();
        _nAdsResetTime = jsonParser["_cool_time"].GetInt();
        
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        std::string strReward = jsonParser["_reward"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        
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

void EventLuckyBagManager::onEventActivate(const int nIdx)
{
    if(nIdx == 38 && !_isEventActivate)
    {
        requestInfo(true);
        EventManager::getInstance()->AddActivateRequestCount();
    }
}
