//
//  FlashmanManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#include "FlashmanManager.h"

#include "Common/ConfigKey.h"
#include "Common/ConfigGameNormal.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

FlashmanManager::FlashmanManager(void) :

//
_season(0),
_timeReset(0),
_todayScoreReset(0),
_ticket(false),

_scoreTotal(0),
_scoreBest(0),
_scoreToday(0),

_scoreAccTotal(0),
_scoreAccIdx(0),

//
_enterCount(0),
_enterBuyCount(0),
_enterBuyAdsCount(0),

_monsterBossHPMin(1),
_monsterBossHPMax(1),

//
_callbackInfo(nullptr),
_callbackResult(nullptr),
_callbackBuyTicket(nullptr),
_callbackMissionReward(nullptr),
_callbackRankReward(nullptr)
{
    
}

FlashmanManager::~FlashmanManager(void)
{
    _delegateResult.Clear();
}

bool FlashmanManager::init()
{
    return true;
}

#pragma mark - update
void FlashmanManager::update(float dt)
{
    
}

#pragma mark - data
void FlashmanManager::saveData()
{
    std::stringstream str;
    
    // score best
    str.str("");
    str << MafUtils::toString(_scoreBest.valueInt());
    UserDefault::getInstance()->setStringForKey(KEY_FLASHMAN_SCORE_BEST, MafAes256::Encrypt(str.str()));
}

void FlashmanManager::loadData()
{
    std::string str = "";
    
    // score best
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_FLASHMAN_SCORE_BEST);
    if ( !str.empty() )
    {
        _scoreBest = std::atoi(MafAes256::Decrypt(str).c_str());
    }
    else
    {
        _scoreBest = 0;
    }
}

#pragma mark - load
void FlashmanManager::setLoad()
{
    setLoadMission();
    setLoadRank();
    setLoadSetting();
}

void FlashmanManager::setLoadMission()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::FLASHMAN_MISSION);
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)(const char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _listInfoMission.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        auto idx = jsonValue["idx"].GetInt();
        auto type = jsonValue["type"].GetInt();
        auto countMax = jsonValue["value"].GetUint64();
        
        auto titleKey = jsonValue["text_key"].GetString();
        auto desc = MafUtils::format("%d", (int)countMax);
        std::string reward = jsonValue["reward"].GetString();
        
        auto obj = InfoQuest::create();
        obj->setIdx(idx);
        obj->setType(type);
        obj->setTitleKey(titleKey);
        obj->setDesc(desc);
        obj->setCountMax(countMax);
        obj->setReward(reward);
        
        //
        _listInfoMission.pushBack(obj);
    }
}

void FlashmanManager::setLoadRank()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::FLASHMAN_RANK);
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)(const char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _listInfoRank.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        int idx = jsonValue["idx"].GetInt();
        int type = jsonValue["rank_type"].GetInt();
        uint64_t condition1 = jsonValue["condition1"].GetUint64();
        uint64_t condition2 = jsonValue["condition2"].GetUint64();
        std::string reward = jsonValue["reward"].GetString();
        
        auto obj = new InfoRank();
        obj->setIdx(idx);
        obj->setType(type);
        obj->setCondition1(condition1);
        obj->setCondition2(condition2);
        obj->setReward(reward);
        
        //
        _listInfoRank.pushBack(obj);
    }
}

void FlashmanManager::setLoadSetting()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::FLASHMAN_SETTING);
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)strText.c_str());
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
    
    // 몬스터 보스 체력
    if ( std::string json = getInfoSetting("boss_monster_hp_value")->getValueToString(); json.length() != 0 )
    {
        rapidjson::Document jsonParser;
        jsonParser.Parse<0>((char*)json.c_str());
        if( jsonParser.HasParseError() == false )
        {
            _monsterBossHPMin = jsonParser["min"].GetInt();
            _monsterBossHPMax = jsonParser["max"].GetInt();
        }
    }
    
    // 몬스터 보스 체력
    if ( std::string json = getInfoSetting("kill_speed_bonus")->getValueToString(); json.length() != 0 )
    {
        rapidjson::Document jsonParser;
        jsonParser.Parse<0>((char*)json.c_str());
        if( jsonParser.HasParseError() == false )
        {
            _listMonsterKillPoint.clear();
            for ( rapidjson::Value::ConstMemberIterator itr = jsonParser.MemberBegin(); itr != jsonParser.MemberEnd(); ++itr )
            {
                auto key = itr->name.GetString();
                auto value = itr->value.GetInt();;
                
                _listMonsterKillPoint[key] = value;
            }
        }
    }
    
    // 누적점수 보너스
    if ( std::string json = getInfoSetting("total_score_reward")->getValueToString(); json.length() != 0 )
    {
        rapidjson::Document jsonParser;
        jsonParser.Parse<0>((char*)json.c_str());
        if( jsonParser.HasParseError() == false )
        {
            _listScoreBonusReward.clear();
            for ( int i = 0; i < jsonParser.Size(); i++ )
            {
                const rapidjson::Value& jsonValue = jsonParser[i];
                
                auto idx = jsonValue["idx"].GetInt();
                auto countMax = jsonValue["score"].GetInt64();
                
                auto rewardIdx = jsonValue["reward"]["idx"].GetInt();
                auto rewardParams = jsonValue["reward"]["type_params"].GetInt();
                auto rewardCount = jsonValue["reward"]["count"].GetInt();
                
                auto obj = InfoQuest::create();
                obj->setIdx(idx);
                obj->setCountMax(countMax);
                obj->addReward(rewardIdx, rewardParams, rewardCount);
                
                _listScoreBonusReward.pushBack(obj);
            }
        }
    }
}

#pragma mark - set, get : prison
cocos2d::Vector<InfoQuest*> FlashmanManager::getListInfoMission()
{
    return _listInfoMission;
}
InfoQuest* FlashmanManager::getInfoMission(int idx)
{
    InfoQuest* objResult = nullptr;
    
    for ( auto& obj : _listInfoMission )
    {
        if ( idx == obj->getIdx() )
        {
            objResult = obj;
            break;
        }
    }
    
    return objResult;
}

cocos2d::Vector<InfoRank*> FlashmanManager::getListInfoRank()
{
    return _listInfoRank;
}
InfoRank* FlashmanManager::getInfoRank(int idx)
{
    InfoRank* objResult = nullptr;
    
    for ( auto& obj : _listInfoRank )
    {
        if ( idx == obj->getIdx() )
        {
            objResult = obj;
            break;
        }
    }
    
    return objResult;
}

InfoSetting* FlashmanManager::getInfoSetting(std::string key)
{
    InfoSetting* objResult = nullptr;
    
    for ( auto& obj : _listInfoSetting )
    {
        if ( obj->getKey().compare(key) == 0 )
        {
            objResult = obj;
            break;
        }
    }
    
    return objResult;
}

#pragma mark - set, get :
int64_t FlashmanManager::getTimeReset()
{
    return _timeReset;
}

int64_t FlashmanManager::getTodayScoreReset()
{
    return _todayScoreReset;
}

bool FlashmanManager::isTicket()
{
    return _ticket;
}

void FlashmanManager::setTicket(bool bTicket)
{
    _ticket = bTicket;
}

#pragma mark - set, get : battle
int FlashmanManager::getEnterCount()
{
    return _enterCount.valueInt();
}

int FlashmanManager::getEnterCountMax()
{
    return getInfoSetting("enter_count")->getValueToInt();
}

int FlashmanManager::getEnterBuyPrice()
{
    int price = getInfoSetting("minigame_devil_price")->getValueToInt();
    int cycle = getInfoSetting("minigame_devil_cycle")->getValueToInt();
    int increase = getInfoSetting("minigame_devil_increase")->getValueToInt();
   
    return price + ((int)(_enterBuyCount / cycle) * increase);
}

int FlashmanManager::getEnterBuyAdsCount()
{
    return _enterBuyAdsCount.valueInt();
}
int FlashmanManager::getEnterBuyAdsCountMax()
{
    return getInfoSetting("max_ads_count")->getValueToInt();
}

int64_t FlashmanManager::getScoreTotal()
{
    return _scoreTotal;
}

int FlashmanManager::getScoreToday()
{
    return _scoreToday.valueInt();
}

int FlashmanManager::getScoreBest()
{
    return _scoreBest.valueInt();
}

int FlashmanManager::getScoreToGrade(int score)
{
    int grade = 1;
    
    // 몬스터 보스 체력
    if ( std::string json = getInfoSetting("end_reward")->getValueToString(); json.length() != 0 )
    {
        rapidjson::Document jsonParser;
        jsonParser.Parse<0>((char*)json.c_str());
        if( jsonParser.HasParseError() == false )
        {
            for ( int i = 0; i < jsonParser.Size(); i++ )
            {
                const rapidjson::Value& jsonValue = jsonParser[i];
                
                if ( score < jsonValue["score"].GetInt() )
                {
                    break;
                }
                
                grade = jsonValue["idx"].GetInt();
            }
        }
    }
    
    return grade;
}

std::string FlashmanManager::getScoreToGradeText(int score)
{
    std::string grade = "C";
    
    // 몬스터 보스 체력
    if ( std::string json = getInfoSetting("end_reward")->getValueToString(); json.length() != 0 )
    {
        rapidjson::Document jsonParser;
        jsonParser.Parse<0>((char*)json.c_str());
        if( jsonParser.HasParseError() == false )
        {
            for ( int i = 0; i < jsonParser.Size(); i++ )
            {
                const rapidjson::Value& jsonValue = jsonParser[i];
                
                if ( score < jsonValue["score"].GetInt() )
                {
                    break;
                }
                
                grade = jsonValue["ranking_text"].GetString();
            }
        }
    }
    
    return grade;
}

int FlashmanManager::getScoreToPoint(int score)
{
    int point = 0;
    
    // 몬스터 보스 체력
    if ( std::string json = getInfoSetting("end_reward")->getValueToString(); json.length() != 0 )
    {
        rapidjson::Document jsonParser;
        jsonParser.Parse<0>((char*)json.c_str());
        if( jsonParser.HasParseError() == false )
        {
            for ( int i = 0; i < jsonParser.Size(); i++ )
            {
                const rapidjson::Value& jsonValue = jsonParser[i];
                
                if ( score < jsonValue["score"].GetInt() )
                {
                    break;
                }
                
                point = jsonValue["ranking_score"].GetInt();
            }
        }
    }
    
    return point;
}

double FlashmanManager::getScoreEffect()
{
    double bonus = getInfoSetting("total_score_bonus")->getValueToFloat();
    return std::floor(_scoreAccTotal * bonus);
}

int FlashmanManager::getScoreAccIdx()
{
    return _scoreAccIdx.valueInt();
}

int FlashmanManager::getScoreAccTotal()
{
    return _scoreAccTotal.valueInt();
}

InfoQuest* FlashmanManager::getScoreBonusReward(int idx)
{
    InfoQuest* objResult = nullptr;
    
    for ( auto& obj : _listScoreBonusReward )
    {
        if ( idx == obj->getIdx() )
        {
            objResult = obj;
            break;
        }
    }
    
    return objResult;
}

int FlashmanManager::getMonsterGenerateCount()
{
    return MAX(30, getMonsterGenerateBonusPos());
}

int FlashmanManager::getMonsterGenerateBossCount()
{
    return getInfoSetting("boss_monster_regen")->getValueToInt();
}

int FlashmanManager::getMonsterGenerateBonusPos()
{
    return getInfoSetting("time_monster_regen")->getValueToInt();
}

int FlashmanManager::getMonsterBossHp()
{
    return random(_monsterBossHPMin.valueInt(), _monsterBossHPMax.valueInt());
}

int FlashmanManager::getMonsterKillPoint(int killPerSec)
{
    int point = 0;
    
    auto iter = _listMonsterKillPoint.find(std::to_string(killPerSec));
    if ( iter != _listMonsterKillPoint.end() )
    {
        point = iter->second.valueInt();
    }
    else
    {
        point = _listMonsterKillPoint["other"].valueInt();
    }
    
    return point;
}

int FlashmanManager::getMonsterKillBonusTime()
{
    return getInfoSetting("time_bonus")->getValueToInt();
}

int FlashmanManager::getFeverCondition()
{
    return getInfoSetting("fever_condition")->getValueToInt();
}

int FlashmanManager::getFeverTime()
{
    return getInfoSetting("fever_time")->getValueToInt();
}

#pragma mark - set, get : mission
int FlashmanManager::getMissionCount(int idx)
{
    int count = 0;
    
    auto iter = _listMissionCount.find(idx);
    if ( iter != _listMissionCount.end() )
    {
        count = _listMissionCount[idx].valueInt();
    }
    
    return count;
}

bool FlashmanManager::getMissionReceive(int idx)
{
    int bReceive = false;
    
    auto iter = _listMissionReceive.find(idx);
    if ( iter != _listMissionReceive.end() )
    {
        bReceive = _listMissionReceive[idx];
    }
    
    return bReceive;
}

bool FlashmanManager::isMissionComplete()
{
    bool bResult = false;
    for ( int i = 0; i < _listInfoMission.size(); i++ )
    {
        auto obj = _listInfoMission.at(i);
   
        bool bReceive = FlashmanManager::getInstance()->getMissionReceive(obj->getIdx());
        if ( bReceive == true )
            continue;
        
        int count = FlashmanManager::getInstance()->getMissionCount(obj->getIdx());
        int countMax = (int)obj->getCountMax();
        if ( count >= countMax )
        {
            bResult = true;
            break;
        }
    }
    
    return bResult;
}

#pragma mark - game

#pragma mark - delegate
void FlashmanManager::delegateResult(MafFunction<void(bool, int, int, bool, std::string)>* func)
{
    _delegateResult += func;
}
void FlashmanManager::undelegateResult(void* obj)
{
    _delegateResult.Clear(obj);
}

#pragma mark - network : info
void FlashmanManager::requestInfo(bool bPopupLoading, const std::function<void(bool, int)>& callback/* = nullptr*/)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackInfo = callback;
    
    //
    std::string url = "/minigame/info/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void FlashmanManager::responseInfo(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
  
    int result = jsonParser["_result"].GetInt();
    if ( result == 1 )
    {
        _timeReset = jsonParser["_end_season_time"].GetInt64();
        _todayScoreReset = jsonParser["_end_time"].GetInt64();
        _season = jsonParser["_season"].GetInt();
        _ticket = jsonParser["_is_ticket"].GetInt();
        
        _scoreTotal = jsonParser["_score"].GetInt64();
        _scoreToday = jsonParser["_today_score"].GetInt64();
        _scoreAccTotal = jsonParser["_acc_score"].GetInt64();
        _scoreAccIdx = jsonParser["_acc_reward_idx"].GetInt();
        
        _enterCount = jsonParser["_ticket"].GetInt();
        _enterBuyCount = jsonParser["_buy_ticket_count"].GetInt();
        _enterBuyAdsCount = jsonParser["_ad_count"].GetInt();
        
         
        //
        if ( jsonParser.HasMember("_mission_list") == true )
        {
            convertInfoMission(jsonParser["_mission_list"]);
        }
        
        //
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(true, result);
            _callbackInfo = nullptr;
        }
    }
    else
    {
        //
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false, result);
            _callbackInfo = nullptr;
        }
    }
}

void FlashmanManager::requestResult(bool bPopupLoading, int score, const std::function<void(bool, int, int, bool, std::string)>& callback/* = nullptr*/)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    int scoreMax = FlashmanManager::getInstance()->getInfoSetting("score_limit")->getValueToInt();
    if ( score > scoreMax )
    {
        score = scoreMax;
    }
    
    //
    _callbackResult = callback;
    
    //
    std::string url = "/minigame/finish";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_score", score);
    
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseResult(bPopupLoading, score, response, dd);
    };
    req->send(cb);
}

void FlashmanManager::requestSkipResult(bool bPopupLoading, int score, const std::function<void(bool, int, int, bool, std::string)>& callback/* = nullptr*/)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackResult = callback;
    
    //
    std::string url = "/minigame/autoclear";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseResult(bPopupLoading, score, response, dd);
    };
    req->send(cb);
}

void FlashmanManager::responseResult(bool bPopupLoading, int score, cocos2d::network::HttpResponse* response,std::string &data)
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
        _delegateResult(false, -1, 0, false, "");
        if ( _callbackResult != nullptr )
        {
            _callbackResult(false, -1, 0, false, "");
            _callbackResult = nullptr;
        }
        return;
    }
  
    int result = jsonParser["_result"].GetInt();
    if ( result == 1 )
    {
        bool bBest = false;
        
        _enterCount = jsonParser["_ticket"].GetInt();
        _scoreTotal = jsonParser["_score"].GetInt64();
        if ( score > _scoreBest )
        {
            _scoreBest = score;
            bBest = true;
        }
        
        //
        std::string strReward = jsonParser["_reward"].GetString();
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward, false);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        //
        if ( jsonParser.HasMember("_mission_list") == true )
        {
            convertInfoMission(jsonParser["_mission_list"]);
        }
        
        //
        saveData();
        
        //
        _delegateResult(true, result, score, bBest, strReward);
        if ( _callbackResult != nullptr )
        {
            _callbackResult(true, result, score, bBest, strReward);
            _callbackResult = nullptr;
        }
    }
    else
    {
        //
        _delegateResult(false, result, 0, false, "");
        if ( _callbackResult != nullptr )
        {
            _callbackResult(false, result, 0, false, "");
            _callbackResult = nullptr;
        }
    }
}

void FlashmanManager::requestBuyTicket(bool bPopupLoading, const std::function<void(bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackBuyTicket = callback;
    
    //
    std::string url = "/minigame/buyTicket";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseBuyTicket(bPopupLoading, response, dd);
    };
    req->send(cb);
}
void FlashmanManager::requestBuyTicketAds(bool bPopupLoading, const std::function<void(bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackBuyTicket = callback;
    
    //
    std::string url = "/ads/finish";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", 4);
    
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseBuyTicket(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void FlashmanManager::responseBuyTicket(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackBuyTicket != nullptr )
        {
            _callbackBuyTicket(false, -1);
            _callbackBuyTicket = nullptr;
        }
        return;
    }
  
    int result = jsonParser["_result"].GetInt();
    if ( result == 1 )
    {
        _enterCount = jsonParser["_ticket"].GetInt();
        
        if ( jsonParser.HasMember("_buy_ticket_count") == true )
        {
            _enterBuyCount = jsonParser["_buy_ticket_count"].GetInt();
        }
        
        if ( jsonParser.HasMember("_count") == true )
        {
            _enterBuyAdsCount = jsonParser["_count"].GetInt();
        }
        
        if ( jsonParser.HasMember("_items") == true )
        {
            std::string strItems = jsonParser["_items"].GetString();
            ItemsManager::getInstance()->setItemsServer(strItems);
        }
        
         
        //
        RefreshManager::getInstance()->refreshUI(E_REFRESH::FLASHMAN_INFO);
        
        //
        if ( _callbackBuyTicket != nullptr )
        {
            _callbackBuyTicket(true, result);
            _callbackBuyTicket = nullptr;
        }
    }
    else
    {
        //
        if ( _callbackBuyTicket != nullptr )
        {
            _callbackBuyTicket(false, result);
            _callbackBuyTicket = nullptr;
        }
    }
}

void FlashmanManager::requestMissionReward(bool bPopupLoading, int idx, const std::function<void(bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackMissionReward = callback;
    
    //
    std::string url = "/minigame/reward";
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_index", idx);
    
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseMissionReward(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void FlashmanManager::requestMissionRewardAll(bool bPopupLoading, const std::function<void(bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackMissionReward = callback;
    
    //
    std::string url = "/minigame/rewardAll";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseMissionReward(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void FlashmanManager::responseMissionReward(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackMissionReward != nullptr )
        {
            _callbackMissionReward(false, -1);
            _callbackMissionReward = nullptr;
        }
        return;
    }
  
    int result = jsonParser["_result"].GetInt();
    if ( result == 1 )
    {
        std::string strReward = jsonParser["_reward"].GetString();
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        //
        if ( _callbackMissionReward != nullptr )
        {
            _callbackMissionReward(true, result);
            _callbackMissionReward = nullptr;
        }
    }
    else
    {
        //
        if ( _callbackMissionReward != nullptr )
        {
            _callbackMissionReward(false, result);
            _callbackMissionReward = nullptr;
        }
    }
}

void FlashmanManager::requestRankReward(bool bPopupLoading, const std::function<void(int, std::string)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackRankReward = callback;
    
    //
    std::string url = "/minigame/ranking/reward";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseRankReward(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void FlashmanManager::responseRankReward(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackRankReward != nullptr )
        {
            _callbackRankReward(0, "");
            _callbackRankReward = nullptr;
        }
        return;
    }
  
    int result = jsonParser["_result"].GetInt();
    if ( result == 1 )
    {
        std::string strReward = jsonParser["_reward"].GetString();
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward, false);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        int rank = 0;
        if ( jsonParser.HasMember("_rank") == true )
        {
            rank = jsonParser["_rank"]["rank"].GetInt();
        }
        
        //
        if ( _callbackRankReward != nullptr )
        {
            _callbackRankReward(rank, strReward);
            _callbackRankReward = nullptr;
        }
    }
    else
    {
        //
        if ( _callbackRankReward != nullptr )
        {
            _callbackRankReward(0, "");
            _callbackRankReward = nullptr;
        }
    }
}

void FlashmanManager::requestAccReward(bool bPopupLoading, const std::function<void(bool, int)>& callback)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    _callbackAccReward = callback;
    
    //
    std::string url = "/minigame/accumulateReward";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseAccReward(bPopupLoading, response, dd);
    };
    req->send(cb);
}

void FlashmanManager::responseAccReward(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data)
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
        if ( _callbackAccReward != nullptr )
        {
            _callbackAccReward(false, -1);
            _callbackAccReward = nullptr;
        }
        return;
    }
  
    int result = jsonParser["_result"].GetInt();
    if ( result == 1 )
    {
        _scoreAccTotal = jsonParser["_acc_score"].GetInt64();
        _scoreAccIdx = jsonParser["_acc_reward_idx"].GetInt();
        
        std::string strReward = jsonParser["_reward"].GetString();
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        //
        if ( _callbackAccReward != nullptr )
        {
            _callbackAccReward(true, result);
            _callbackAccReward = nullptr;
        }
    }
    else
    {
        //
        if ( _callbackAccReward != nullptr )
        {
            _callbackAccReward(false, result);
            _callbackAccReward = nullptr;
        }
    }
}

void FlashmanManager::convertInfoMission(const rapidjson::Value& jsonMission)
{
    _listMissionCount.clear();
    _listMissionReceive.clear();
    for ( int i = 0; i < jsonMission.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonMission[i];
        
        int idx = jsonValue["id"].GetInt();
        _listMissionCount[idx] = jsonValue["current_value"].GetInt64();
        _listMissionReceive[idx] = jsonValue["is_reward"].GetBool();
    }
    
    RefreshReddotManager::getInstance()->setValue(E_REFRESH_REDDOT::FLASHMAN_MISSION_COMPLETE, isMissionComplete());
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::FLASHMAN_MISSION_COMPLETE);
}
