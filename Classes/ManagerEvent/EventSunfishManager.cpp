//
//  EventSunfishManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/24.
//
#include "EventSunfishManager.h"

#include "Common/ConfigKey.h"
#include "Common/ConfigGameNormal.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

EventSunfishManager::EventSunfishManager(void) :
_endTime(0),
_bExtraTime(0),

_gameWeight(0),
_gameLevel(0),

_die(0),
_point(0),

_adventureCount(0),
_adventureTime(0),

_adNow(0),

_feverTime(0)
{
    
}

EventSunfishManager::~EventSunfishManager(void)
{
    onDelegateGameLevelUp.Clear();
    onDelegateGameDie.Clear();
    onDelegateGameWeight.Clear();
    onDelegateInfoUpdate.Clear();
}

bool EventSunfishManager::init()
{
    //
    std::string str = UserDefault::getInstance()->getStringForKey(KEY_EVENT_SUNFISH_FEVER);
    if ( !str.empty() )
    {
        _feverTime = std::atoll(MafAes256::Decrypt(str).c_str());
    }
    
    return true;
}

#pragma mark - update
void EventSunfishManager::update(float dt)
{
    
}

#pragma mark - load
void EventSunfishManager::setLoad()
{
    setLoadSunfish();
}

void EventSunfishManager::setLoadSunfish()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::EVENT_SUNFISH);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)(const char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _listInfoSunfish.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];

        auto type = jsonValue["type"].GetInt();
        auto level = jsonValue["lv"].GetInt();
        
        auto weight = jsonValue["weight"].GetDouble();
        auto survivalRate = jsonValue["survival_rate"].GetInt();
        auto bonusRate = jsonValue["bonus_rate"].GetInt();
        auto bonusWeight = jsonValue["bonus_weight"].GetInt();
        auto cost = jsonValue["cost"].GetInt();
        auto point = jsonValue["point"].GetInt();
        
        auto nameKey = jsonValue["name"].GetString();
        auto dieDialogueKey = jsonValue["die_dialogue"].GetString();
        auto dieTextKey = jsonValue["die_text"].GetString();
        auto illustratedTitleTextKey = jsonValue["illustrated_tite_text"].GetString();
        auto illustratedTextKey = jsonValue["illustrated_text"].GetString();
        auto survivalTextKey = jsonValue["survival_text"].GetString();
        
        const std::string reward = jsonValue["reward"].GetString();
        const std::string rewardDie = jsonValue["die_reward"].GetString();
        
        auto obj = InfoEventSunfish::create();
        obj->setType(type);
        obj->setLevel(level);
        
        obj->setWeight(weight);
        obj->setSurvivalRate(survivalRate);
        obj->setBonusRate(bonusRate);
        obj->setBonusWeight(bonusWeight);
        obj->setCost(cost);
        obj->setPoint(point);
        
        obj->setNameKey(nameKey);
        obj->setDieDialogueKey(dieDialogueKey);
        obj->setDieTextKey(dieTextKey);
        obj->setIllustratedTitleTextKey(illustratedTitleTextKey);
        obj->setIllustratedTextKey(illustratedTextKey);
        obj->setSurvivalTextKey(survivalTextKey);
        
        obj->setReward(reward);
        obj->setRewardDie(rewardDie);
        
        //
        _listInfoSunfish.pushBack(obj);
    }
}

#pragma mark - set, get : info
cocos2d::Vector<InfoEventSunfish*> EventSunfishManager::getListInfoSunfish(E_TYPE eType)
{
    Vector<InfoEventSunfish*> listResult;
    
    for ( auto& obj : _listInfoSunfish )
    {
        if ( obj->getType() == enumToInt(eType) )
        {
            listResult.pushBack(obj);
        }
    }
    
    return listResult;
}
InfoEventSunfish* EventSunfishManager::getInfoSunfishFood(int idx)
{
    auto list = getListInfoSunfish(E_TYPE::FOOD);
    for ( auto& obj : list )
    {
        if ( idx == obj->getLevel() )
            return obj;
    }

    return nullptr;
}
InfoEventSunfish* EventSunfishManager::getInfoSunfishAdventure(int idx)
{
    auto list = getListInfoSunfish(E_TYPE::ADVENTURE);
    for ( auto& obj : list )
    {
        if ( idx == obj->getLevel() )
            return obj;
    }

    return nullptr;
}
InfoEventSunfish* EventSunfishManager::getInfoSunfishLevel(int level)
{
    auto list = getListInfoSunfish(E_TYPE::LEVEL);
    for ( auto& obj : list )
    {
        if ( level == obj->getLevel() )
            return obj;
    }

    return nullptr;
}


#pragma mark - set, get :
int64_t EventSunfishManager::getEndTime()
{
    return _endTime;
}
bool EventSunfishManager::isExtraTime()
{
    return _bExtraTime;
}

#pragma mark - set, get : game
int EventSunfishManager::getGameLevelNow()
{
    return _gameLevel.valueInt();
}
int EventSunfishManager::getGameLevelMax()
{
    auto list = getListInfoSunfish(E_TYPE::LEVEL);
    return (int)list.size();
}

double EventSunfishManager::getGameWeight()
{
    return _gameWeight;
}

void EventSunfishManager::setGameWeight(double value)
{
    _gameWeight = floor(value * 10000) / 10000;
}

int EventSunfishManager::getGameEnemyCountMax()
{
    int count = 6 + (int)((_listReinforceFood.size() - 1) * 2);
    
    return count;
}

int EventSunfishManager::getGamePoint()
{
    return _point.valueInt();
}

#pragma mark - set, get : adventure
int EventSunfishManager::getAdventureCountNow()
{
    int count = _adventureCount.valueInt();
    int countMax = getAdventureCountMax();
    
    if ( count < countMax )
    {
        auto timeDiff = UtilsDate::getInstance()->getTime() - _adventureTime.valueInt();
        auto timeMax = getAdventureTimeMax();
        
        if ( timeDiff > 0 )
        {
            count += timeDiff / timeMax;
            count = MIN(countMax, count);
        }
    }
    
    return count;
}

int EventSunfishManager::getAdventureCountMax()
{
    int countMax = 0;

    std::string data = EventManager::getInstance()->getEventOhterDataByEventIdx(40);
    if ( data.empty() == false )
    {
        rapidjson::Document jsonParser;
        jsonParser.ParseInsitu<0>((char*)data.c_str());
        if( jsonParser.HasParseError() == false )
        {
            countMax = jsonParser["adventure_max_count"].GetInt();
        }
    }

    return countMax;
}

int EventSunfishManager::getAdventureTimeNow()
{
    auto timeNow = _adventureTime.valueInt();
    auto timeMax = getAdventureTimeMax();
    
    int time = timeMax;
    
    auto timeDiff = UtilsDate::getInstance()->getTime() - timeNow;
    if ( timeDiff > 0 )
    {
        if ( timeDiff % timeMax == 0 )
        {
            time = 0;
        }
        else
        {
            time = timeMax - (timeDiff % timeMax);
        }
    }
    
    return time;
}

int EventSunfishManager::getAdventureTimeMax()
{
    int timeMax = 0;
    
    std::string data = EventManager::getInstance()->getEventOhterDataByEventIdx(40);
    if ( data.empty() == false )
    {
        rapidjson::Document jsonParser;
        jsonParser.ParseInsitu<0>((char*)data.c_str());
        if( jsonParser.HasParseError() == false )
        {
            timeMax = jsonParser["adventure_cool_time_m"].GetInt() * 60;
        }
    }
    
    return timeMax;
}

#pragma mark - set, get : reinforce
bool EventSunfishManager::isReinforce(E_TYPE eType, int level)
{
    return isReinforce(enumToInt(eType), level);
}
bool EventSunfishManager::isReinforce(int type, int level)
{
    bool bResult = false;
    
    if ( type == 1 ) // food
    {
        auto iter = std::find(_listReinforceFood.begin(), _listReinforceFood.end(), level);
        if ( iter != _listReinforceFood.end() )
        {
            bResult = true;
        }
    }
    else if ( type == 2 ) // adventure
    {
        auto iter = std::find(_listReinforceAdventure.begin(), _listReinforceAdventure.end(), level);
        if ( iter != _listReinforceAdventure.end() )
        {
            bResult = true;
        }
    }
    else if ( type == 3 ) // evolution
    {
        auto iter = std::find(_listReinforceEvolution.begin(), _listReinforceEvolution.end(), level);
        if ( iter != _listReinforceEvolution.end() )
        {
            bResult = true;
        }
    }
    return bResult;
}

#pragma mark - set, get : die
int EventSunfishManager::getDieTotal()
{
    return _die.valueInt();
}

int EventSunfishManager::getDie(E_TYPE eType, int level)
{
    return getDie(enumToInt(eType), level);
}
int EventSunfishManager::getDie(int type, int idx)
{
    int count = 0;
    
    if ( type == 1 ) // food
    {
        auto iter = _listDieFood.find(idx);
        if ( iter != _listDieFood.end() )
        {
            count = _listDieFood[idx];
        }
    }
    else if ( type == 2 ) // adventure
    {
        auto iter = _listDieAdventure.find(idx);
        if ( iter != _listDieAdventure.end() )
        {
            count = _listDieAdventure[idx];
        }
    }
    else if ( type == 3 ) // evolution
    {
        auto iter = _listDieEvolution.find(idx);
        if ( iter != _listDieEvolution.end() )
        {
            count = _listDieEvolution[idx];
        }
    }
    
    return count;
}

bool EventSunfishManager::isDie(E_TYPE eType, int level)
{
    return isDie(enumToInt(eType), level);
}
bool EventSunfishManager::isDie(int type, int level)
{
    bool bResult = false;
    
    if ( type == 1 ) // food
    {
        auto iter = _listDieFood.find(level);
        if ( iter != _listDieFood.end() && _listDieFood[level] > 0 )
        {
            bResult = true;
        }
    }
    else if ( type == 2 ) // adventure
    {
        auto iter = _listDieAdventure.find(level);
        if ( iter != _listDieAdventure.end() && _listDieAdventure[level] > 0 )
        {
            bResult = true;
        }
    }
    else if ( type == 3 ) // evolution
    {
        auto iter = _listDieEvolution.find(level);
        if ( iter != _listDieEvolution.end() && _listDieEvolution[level] > 0 )
        {
            bResult = true;
        }
    }
    
    return bResult;
}

bool EventSunfishManager::isDieReward(E_TYPE eType, int level)
{
    return isDieReward(enumToInt(eType), level);
}
bool EventSunfishManager::isDieReward(int type, int level)
{
    bool bResult = false;
    
    if ( type == 1 ) // food
    {
        auto iter = std::find(_listDieFoodReward.begin(), _listDieFoodReward.end(), level);
        if ( iter != _listDieFoodReward.end() )
        {
            bResult = true;
        }
    }
    else if ( type == 2 ) // adventure
    {
        auto iter = std::find(_listDieAdventureReward.begin(), _listDieAdventureReward.end(), level);
        if ( iter != _listDieAdventureReward.end() )
        {
            bResult = true;
        }
    }
    else if ( type == 3 ) // evolution
    {
        auto iter = std::find(_listDieEvolutionReward.begin(), _listDieEvolutionReward.end(), level);
        if ( iter != _listDieEvolutionReward.end() )
        {
            bResult = true;
        }
    }
    return bResult;
}

bool EventSunfishManager::isDieRewardComplete(E_TYPE eType)
{
    bool bResult = false;
    
    auto list = getListInfoSunfish(eType);
    for ( auto& obj : list )
    {
        if ( isDie(obj->getType(), obj->getLevel()) == true && isDieReward(obj->getType(), obj->getLevel()) == false )
        {
            bResult = true;
            break;
        }
    }
     
    return bResult;
}

bool EventSunfishManager::isReward(E_TYPE eType, int level)
{
    return isReward(enumToInt(eType), level);
}
bool EventSunfishManager::isReward(int type, int level)
{
    bool bResult = false;
    
    if ( type == 1 ) // food
    {
        auto iter = std::find(_listFoodReward.begin(), _listFoodReward.end(), level);
        if ( iter != _listFoodReward.end() )
        {
            bResult = true;
        }
    }
    else if ( type == 2 ) // adventure
    {
        auto iter = std::find(_listAdventureReward.begin(), _listAdventureReward.end(), level);
        if ( iter != _listAdventureReward.end() )
        {
            bResult = true;
        }
    }
    else if ( type == 3 ) // evolution
    {
        auto iter = std::find(_listEvolutionReward.begin(), _listEvolutionReward.end(), level);
        if ( iter != _listEvolutionReward.end() )
        {
            bResult = true;
        }
    }
    return bResult;
}

#pragma mark - set, get : ad
int EventSunfishManager::getADNow()
{
    return _adNow.valueInt();
}

int EventSunfishManager::getADMax()
{
    int adsMax = 0;
    
    std::string data = EventManager::getInstance()->getEventOhterDataByEventIdx(40);
    if ( data.empty() == false )
    {
        rapidjson::Document jsonParser;
        jsonParser.ParseInsitu<0>((char*)data.c_str());
        if( jsonParser.HasParseError() == false )
        {
            adsMax = jsonParser["ads_max"].GetInt();
        }
    }
    
    return adsMax;
}

#pragma mark - set, get : ad
int EventSunfishManager::getFeverCoolTime()
{
    int time = 0;
    
    std::string data = EventManager::getInstance()->getEventOhterDataByEventIdx(40);
    if ( data.empty() == false )
    {
        rapidjson::Document jsonParser;
        jsonParser.ParseInsitu<0>((char*)data.c_str());
        if( jsonParser.HasParseError() == false )
        {
            time = jsonParser["fever_cool_time_m"].GetInt() * 60;
        }
    }
    
    return time;
}
int EventSunfishManager::getFeverCoolAgainTime()
{
    return 120;
}
int EventSunfishManager::getFeverActiveTime()
{
    int time = 0;
    
    std::string data = EventManager::getInstance()->getEventOhterDataByEventIdx(40);
    if ( data.empty() == false )
    {
        rapidjson::Document jsonParser;
        jsonParser.ParseInsitu<0>((char*)data.c_str());
        if( jsonParser.HasParseError() == false )
        {
            time = jsonParser["fever_time_c"].GetInt();
        }
    }
    
    return time;
}
int64_t EventSunfishManager::getFeverTime()
{
    return _feverTime;
}
void EventSunfishManager::setFeverTime(int64_t time)
{
    _feverTime = time;
    
    std::stringstream str;
    str << MafUtils::toString(_feverTime.value());
    UserDefault::getInstance()->setStringForKey(KEY_EVENT_SUNFISH_FEVER, MafAes256::Encrypt(str.str()));
}
 
#pragma mark - network : info
void EventSunfishManager::requestInfo(bool bPopupLoading, const std::function<void(bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/event/sunfish/info/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(bPopupLoading, callback, response, dd);
    };
    req->send(cb);
}

void EventSunfishManager::responseInfo(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data)
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
        _endTime = jsonParser["_end_event"].GetInt64();
        _bExtraTime = jsonParser["_extra_time"].GetBool();
         
        if ( jsonParser.HasMember("_weight") == true )
        {
            _gameWeight = jsonParser["_weight"].GetDouble();
        }
        
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

void EventSunfishManager::requestWeight(bool bPopupLoading, const std::function<void(bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/event/sunfish/weight";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setDataDouble("_weight", getGameWeight());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseWeight(bPopupLoading, callback, response, dd);
    };
    req->send(cb);
}

void EventSunfishManager::responseWeight(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data)
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

void EventSunfishManager::requestFoodCheck(bool bPopupLoading, int foodIdx, const std::function<void(bool, int, bool)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/event/sunfish/result";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", 1);
    req->setData("_lv", foodIdx);
    req->setDataDouble("_weight", getGameWeight());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseFoodCheck(bPopupLoading, callback, response, dd);
    };
    req->send(cb);
}

void EventSunfishManager::responseFoodCheck(bool bPopupLoading, const std::function<void(bool, int, bool)>& callback, cocos2d::network::HttpResponse* response,std::string &data)
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
            callback(false, -1, false);
        }
        return;
    }
    
    auto result = jsonParser["_result"].GetInt();
    if ( result == 1 )
    {
        bool bSuccess = jsonParser["_is_success"].GetInt();

        //
        convertJsonInfo(jsonParser);
        
        //
        if ( jsonParser.HasMember("_reward") == true )
        {
            std::string strReward = jsonParser["_reward"].GetString();
            ItemsManager::getInstance()->addItems(strReward);
        }
        
        if ( jsonParser.HasMember("_items") == true )
        {
            std::string strItems = jsonParser["_items"].GetString();
            ItemsManager::getInstance()->setItemsServer(strItems);
        }
        
        //
        if ( callback != nullptr )
        {
            callback(true, result, bSuccess);
        }
    }
    else
    {
        if ( callback != nullptr )
        {
            callback(false, result, false);
        }
    }
}

void EventSunfishManager::requestLevelCheck(bool bPopupLoading, int levelIdx, const std::function<void(bool, int, bool)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/event/sunfish/result";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", 3);
    req->setData("_lv", levelIdx);
    req->setDataDouble("_weight", getGameWeight());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseLevelCheck(bPopupLoading, callback, response, dd);
    };
    req->send(cb);
}

void EventSunfishManager::responseLevelCheck(bool bPopupLoading, const std::function<void(bool, int, bool)>& callback, cocos2d::network::HttpResponse* response,std::string &data)
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
            callback(false, -1, false);
        }
        return;
    }
    
    auto result = jsonParser["_result"].GetInt();
    if ( result == 1 )
    {
        bool bSuccess = jsonParser["_is_success"].GetInt();

        //
        convertJsonInfo(jsonParser);
        
        //
        if ( jsonParser.HasMember("_reward") == true )
        {
            std::string strReward = jsonParser["_reward"].GetString();
            ItemsManager::getInstance()->addItems(strReward);
        }
        
        if ( jsonParser.HasMember("_items") == true )
        {
            std::string strItems = jsonParser["_items"].GetString();
            ItemsManager::getInstance()->setItemsServer(strItems);
        }
        
        //
        if ( callback != nullptr )
        {
            callback(true, result, bSuccess);
        }
    }
    else
    {
        if ( callback != nullptr )
        {
            callback(false, result, false);
        }
    }
}

void EventSunfishManager::requestAdventureUse(bool bPopupLoading, const std::function<void(bool, int, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/event/sunfish/useChance";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseAdventureUse(bPopupLoading, callback, response, dd);
    };
    req->send(cb);
}

void EventSunfishManager::responseAdventureUse(bool bPopupLoading, const std::function<void(bool, int, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data)
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
            callback(false, -1, -1);
        }
        return;
    }
    
    auto result = jsonParser["_result"].GetInt();
    if ( result == 1 )
    {
        auto adventureIdx = jsonParser["_choice_adventure_lv"].GetInt();
        
        //
        convertJsonInfo(jsonParser);
        
        //
        if ( callback != nullptr )
        {
            callback(true, result, adventureIdx);
        }
    }
    else
    {
        if ( callback != nullptr )
        {
            callback(false, result, -1);
        }
    }
}

void EventSunfishManager::requestAdventureStart(bool bPopupLoading, int adventureIdx, const std::function<void(bool, int, bool)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/event/sunfish/result";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", 2);
    req->setData("_lv", adventureIdx);
    req->setDataDouble("_weight", getGameWeight());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseAdventureStart(bPopupLoading, callback, response, dd);
    };
    req->send(cb);
}

void EventSunfishManager::responseAdventureStart(bool bPopupLoading, const std::function<void(bool, int, bool)>& callback, cocos2d::network::HttpResponse* response,std::string &data)
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
            callback(false, -1, false);
        }
        return;
    }
    
    auto result = jsonParser["_result"].GetInt();
    if ( result == 1 )
    {
        bool bSuccess = jsonParser["_is_success"].GetInt();
        
        //
        convertJsonInfo(jsonParser);
        
        //
        if ( jsonParser.HasMember("_reward") == true )
        {
            std::string strReward = jsonParser["_reward"].GetString();
            ItemsManager::getInstance()->addItems(strReward);
        }
        
        if ( jsonParser.HasMember("_items") == true )
        {
            std::string strItems = jsonParser["_items"].GetString();
            ItemsManager::getInstance()->setItemsServer(strItems);
        }
        
        //
        if ( callback != nullptr )
        {
            callback(true, result, bSuccess);
        }
    }
    else
    {
        if ( callback != nullptr )
        {
            callback(false, result, false);
        }
    }
}

void EventSunfishManager::requestReinforce(bool bPopupLoading, int type, int level, const std::function<void(bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/event/sunfish/unlock";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", type);
    req->setData("_lv", level);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseReinforce(bPopupLoading, callback, response, dd);
    };
    req->send(cb);
}

void EventSunfishManager::responseReinforce(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data)
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

void EventSunfishManager::requestReward(bool bPopupLoading, int type, int level, const std::function<void(bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/event/sunfish/reward";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", type);
    req->setData("_lv", level);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseReward(bPopupLoading, callback, response, dd);
    };
    req->send(cb);
}

void EventSunfishManager::responseReward(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data)
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

void EventSunfishManager::requestAD(bool bPopupLoading, const std::function<void(bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/ads/finish";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", 6);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseAD(bPopupLoading, callback, response, dd);
    };
    req->send(cb);
}

void EventSunfishManager::responseAD(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data)
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
        _adventureCount = jsonParser["_chance"].GetInt();
        _adventureTime = jsonParser["_updatetime"].GetInt64();
        
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

/*
void EventSunfishManager::requestReinforce(bool bPopupLoading, const std::function<void(bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/event/sunfish/info/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseReinforce(bPopupLoading, callback, response, dd);
    };
    req->send(cb);
}

void EventSunfishManager::responseReinforce(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data)
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
 */

void EventSunfishManager::convertJsonInfo(const rapidjson::Value& json)
{
    if ( json.HasMember("_ad_count") == true )
    {
        _adNow = json["_ad_count"].GetInt();
    }
    
    if ( json.HasMember("_lv") == true )
    {
        _gameLevel = json["_lv"].GetInt();
    }
    
    if ( json.HasMember("_point") == true )
    {
        _point = json["_point"].GetInt();
    }
    
    if ( json.HasMember("_chance_info") == true )
    {
        _adventureCount = json["_chance_info"]["chance"].GetInt();
        _adventureTime = json["_chance_info"]["updatetime"].GetInt();
    }
    
    // 강화
    if ( json.HasMember("_sunfish_open_info") == true )
    {
        _listReinforceFood.clear();
        _listReinforceAdventure.clear();
        _listReinforceEvolution.clear();
        for ( int i = 0; i < json["_sunfish_open_info"]["1"].Size(); i++ )
        {
            _listReinforceFood.push_back(json["_sunfish_open_info"]["1"][i].GetInt());
        }
        
        for ( int i = 0; i < json["_sunfish_open_info"]["2"].Size(); i++ )
        {
            _listReinforceAdventure.push_back(json["_sunfish_open_info"]["2"][i].GetInt());
        }
        
        for ( int i = 0; i < json["_sunfish_open_info"]["3"].Size(); i++ )
        {
            _listReinforceEvolution.push_back(json["_sunfish_open_info"]["3"][i].GetInt());
        }
    }
    
    // 사망
    if ( json.HasMember("_sunfish_info") == true )
    {
        _die = 0;
        _listDieFood.clear();
        _listDieAdventure.clear();
        _listDieEvolution.clear();
        for ( rapidjson::Value::ConstMemberIterator itr = json["_sunfish_info"]["1"].MemberBegin(); itr != json["_sunfish_info"]["1"].MemberEnd(); ++itr )
        {
            auto key = (std::string)(itr->name.GetString());
            auto value = itr->value.GetInt();
            
            _listDieFood[atoi(key.c_str())] = value;
            _die += value;
        }
        
        for ( rapidjson::Value::ConstMemberIterator itr = json["_sunfish_info"]["2"].MemberBegin(); itr != json["_sunfish_info"]["2"].MemberEnd(); ++itr )
        {
            auto key = (std::string)(itr->name.GetString());
            auto value = itr->value.GetInt();
            
            _listDieAdventure[atoi(key.c_str())] = value;
            _die += value;
        }
         
        for ( rapidjson::Value::ConstMemberIterator itr = json["_sunfish_info"]["3"].MemberBegin(); itr != json["_sunfish_info"]["3"].MemberEnd(); ++itr )
        {
            auto key = (std::string)(itr->name.GetString());
            auto value = itr->value.GetInt();
            
            _listDieEvolution[atoi(key.c_str())] = value;
            _die += value;
        }
    }
    
    // 보상
    if ( json.HasMember("_quest_reward_info") == true )
    {
        _listDieFoodReward.clear();
        _listDieAdventureReward.clear();
        _listDieEvolutionReward.clear();
        for ( int i = 0; i < json["_quest_reward_info"]["1"]["collection_reward_info"].Size(); i++ )
        {
            _listDieFoodReward.push_back(json["_quest_reward_info"]["1"]["collection_reward_info"][i].GetInt());
        }
        
        for ( int i = 0; i < json["_quest_reward_info"]["2"]["collection_reward_info"].Size(); i++ )
        {
            _listDieAdventureReward.push_back(json["_quest_reward_info"]["2"]["collection_reward_info"][i].GetInt());
        }
        
        for ( int i = 0; i < json["_quest_reward_info"]["3"]["collection_reward_info"].Size(); i++ )
        {
            _listDieEvolutionReward.push_back(json["_quest_reward_info"]["3"]["collection_reward_info"][i].GetInt());
        }
        
        _listFoodReward.clear();
        _listAdventureReward.clear();
        _listEvolutionReward.clear();
        for ( int i = 0; i < json["_quest_reward_info"]["1"]["reward_info"].Size(); i++ )
        {
            _listFoodReward.push_back(json["_quest_reward_info"]["1"]["reward_info"][i].GetInt());
        }
        
        for ( int i = 0; i < json["_quest_reward_info"]["2"]["reward_info"].Size(); i++ )
        {
            _listAdventureReward.push_back(json["_quest_reward_info"]["2"]["reward_info"][i].GetInt());
        }
        
        for ( int i = 0; i < json["_quest_reward_info"]["3"]["reward_info"].Size(); i++ )
        {
            _listEvolutionReward.push_back(json["_quest_reward_info"]["3"]["reward_info"][i].GetInt());
        }
    }
    
    //
    bool bDieRewardCompleteFood = isDieRewardComplete(E_TYPE::FOOD);
    bool bDieRewardCompleteAdventure = isDieRewardComplete(E_TYPE::ADVENTURE);
    
    RefreshReddotManager::getInstance()->setValue(E_REFRESH_REDDOT::EVENT_SUNFISH_DEX_ALL, bDieRewardCompleteFood || bDieRewardCompleteAdventure);
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::EVENT_SUNFISH_DEX_ALL);
    
    RefreshReddotManager::getInstance()->setValue(E_REFRESH_REDDOT::EVENT_SUNFISH_DEX_FOOD, bDieRewardCompleteFood);
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::EVENT_SUNFISH_DEX_FOOD);
    
    RefreshReddotManager::getInstance()->setValue(E_REFRESH_REDDOT::EVENT_SUNFISH_DEX_ADVENTURE, bDieRewardCompleteAdventure);
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::EVENT_SUNFISH_DEX_ADVENTURE);
}

#pragma mark - event
