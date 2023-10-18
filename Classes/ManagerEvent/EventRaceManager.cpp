//
//  EventHorseRaceManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/18.
//
#include "EventRaceManager.h"

#include "Common/ConfigGameNormal.h"
#include "GameObject/InfoQuest.h"
#include "GameObject/EventRace/InfoEventRaceSimulateLog.h"
#include "GameObject/EventRace/InfoEventRaceStat.h"

#include "GameUIPopup/OtherEvent/EventRace/PopupEventRaceRankReward.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/CostumeSkinManager.h"

USING_NS_CC;

bool EventRaceManager::sortRank(std::pair<int, float>a, std::pair<int, float>b)
{
    if(a.second == b.second)
        return a.first < b.first;
    return a.second > b.second;
}
EventRaceManager::EventRaceManager(void):
_isEventActivate(false)
,_isGameRunning(false)
,_isHiddenAds(false)
,_isServerDispatch(false)
,_gameRewardMag(0)
,_nCoin(0)
,_nSecondaryCorrection(0)
,_nTicket(0)
,_callbackInfo(nullptr)
,_callbackAds(nullptr)
,_callbackInvestment(nullptr)
,_callbackGameStart(nullptr)
{
    _listInfoHorse.clear();
    _listInfoStat.clear();
    _eventRaceReady.Clear();
    _eventRaceStart.Clear();
    _eventRaceGameOver.Clear();
    _eventStatus.Clear();
    _eventInvestment.Clear();
    _eventAds.Clear();
    _eventEventOver.Clear();
}

EventRaceManager::~EventRaceManager(void)
{
    _listInfoHorse.clear();
    _listInfoStat.clear();
    _eventRaceReady.Clear();
    _eventRaceStart.Clear();
    _eventRaceGameOver.Clear();
    _eventStatus.Clear();
    _eventInvestment.Clear();
    _eventAds.Clear();
    _eventEventOver.Clear();
}

bool EventRaceManager::init()
{
    setLoadAll();
    EventManager::getInstance()->_onRequestInfo += new MafFunction<void(const int)>(CC_CALLBACK_1(EventRaceManager::onEventActivate, this), this);
    return true;
}
#pragma mark - get
Vector<InfoEventRaceStat*> EventRaceManager::getListStatByTypeName(const std::string strTypeName)
{
    Vector<InfoEventRaceStat*> result;
    result.clear();
    
    for(auto obj : _listInfoStat)
    {
        std::string tmp = obj->getTypeName();
        if(obj->getTypeName() == strTypeName)
            result.pushBack(obj);
    }
    
    return result;
}

Vector<InfoEventRaceHorse*> EventRaceManager::getListHorse()
{
    return _listInfoHorse;
}

Vector<InfoQuest*> EventRaceManager::getListMission()
{
    return _listInfoMission;
}

InfoEventRaceHorse* EventRaceManager::getInfoHorse(const int nIdx)
{
    InfoEventRaceHorse* result = nullptr;
    
    for(auto obj : _listInfoHorse)
    {
        if(obj->getIdx() == nIdx)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
InfoEventRaceHorse* EventRaceManager::getInfoHorseBySkinIdx(const int nIdx)
{
    InfoEventRaceHorse* result = nullptr;
    
    for(auto obj : _listInfoHorse)
    {
        if(obj->getSkinIdx() == nIdx)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
InfoEventRaceHorse* EventRaceManager::getInfoHorseByCurrentRank(const int nRank)
{
    InfoEventRaceHorse* result = nullptr;
    
    for(auto obj : _listInfoHorse)
    {
        if(obj->getCurrentRank() == nRank)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
InfoEventRaceStat* EventRaceManager::getInfoRaceStatByIdx(const int nIdx)
{
    InfoEventRaceStat* result = nullptr;
    
    for(auto obj : _listInfoStat)
    {
        if(obj->getIdx() == nIdx)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
InfoQuest* EventRaceManager::getInfoMission(const int nIdx)
{
    InfoQuest* result = nullptr;
    
    for(auto obj : _listInfoMission)
    {
        if(obj->getIdx() == nIdx)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

const bool EventRaceManager::isGameRunning() const
{
    return _isGameRunning;
}
const bool EventRaceManager::isHiddenAds() const
{
    return _isHiddenAds;
}
const float EventRaceManager::getGameRewardMag() const
{
    return _gameRewardMag;
}
const double EventRaceManager::getCoin()
{
    return _nCoin;
}
const int EventRaceManager::getTicket() const
{
    return _nTicket;
}

const double EventRaceManager::getInvestCount() const
{
    double result = 0;
    
    for(auto obj : _listInfoHorse)
        result += obj->getInvest();
    
    return result;
}
const int EventRaceManager::getPressureHorseIdx() const
{
    int result = 0;
    
    int count = 0;
    double maxInvest = 0;
    
    for(auto obj : _listInfoHorse)
    {
        if(maxInvest < obj->getInvest())
        {
            maxInvest = obj->getInvest();
            count = 0;
            result = obj->getIdx();
        }
        
        double intervalValue = maxInvest - obj->getInvest();
        double largerValue = std::fmax(maxInvest + 0.001f, obj->getInvest());
        if(fabs(intervalValue) <= FLT_EPSILON * largerValue)
            count++;
    }
    
    if(count > 1)
        result = 0;
    
    return result;
}

int EventRaceManager::getGameResetCount()
{
    return _gameResetCount.valueInt();
}

bool EventRaceManager::isCompleteMission()
{
    bool bResult = false;
    
    for ( auto& obj : _listInfoMission )
    {
        if ( obj->isReceive() == true )
        {
            continue;
        }
            
        if ( obj->isCountShort() == false && obj->getCountNow() >= obj->getCountMax() )
        {
            bResult = true;
            break;
        }
        else if ( obj->isCountShort() == true && obj->getCountBigNow() >= obj->getCountBigMax() )
        {
            bResult = true;
            break;
        }
    }
    
    return bResult;
}

#pragma mark - Load Table Data
void EventRaceManager::setLoadAll()
{
    setLoadStat();
    setLoadSetting();
    setLoadMission();
}

void EventRaceManager::setLoadStat()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::EVENT_RACE_STAT);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoStat.clear();
    std::string strCurType = "";
    int nCurGrade = 1;
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        //Scene
        const int nIdx = jsonValue["idx"].GetInt();
        const std::string strTypeName = jsonValue["type_name"].GetString();
        const bool isHidden = (bool)jsonValue["is_hidden"].GetInt();
        const std::string strTendency = jsonValue["tendency"].GetString();
        const double nSceneIdx = jsonValue["value"].GetDouble();
        const std::string strTextKey = jsonValue["text_key"].GetString();
        
        if(strCurType != strTypeName)
        {
            strCurType = strTypeName;
            nCurGrade = 1;
        }
        else
        {
            if(strCurType != "ABILITY")
                nCurGrade++;
        }
        
        InfoEventRaceStat* obj = InfoEventRaceStat::create();
        obj->setIdx(nIdx);
        obj->setGrade(nCurGrade);
        obj->setTypeName(strTypeName);
        obj->setIsHidden(isHidden);
        obj->setTendency(strTendency);
        obj->setValue(nSceneIdx);
        obj->setTextKey(strTextKey);
        
        _listInfoStat.pushBack(obj);
    }
}
void EventRaceManager::setLoadSetting()
{
    std::string strText = EventManager::getInstance()->getEventOhterDataByEventIdx(33);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _nSecondaryCorrection = jsonParser["secondary_correction"].GetInt();
    
}

void EventRaceManager::setLoadMission()
{
    _listInfoMission.clear();
    
    //
    std::string txt = TableManager::getInstance()->getTableData(E_TABLE::EVENT_RACE_MISSION);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)(const char*)txt.c_str()).HasParseError();
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        auto idx = jsonValue["idx"].GetInt();
        auto type = jsonValue["mission_type"].GetInt();
        
        auto title = jsonValue["text_key"].GetString();
        
        bool countShort = jsonValue["value_short"].GetInt();
        
        auto objQuest = InfoQuest::create();
        objQuest->setIdx(idx);
        objQuest->setType(type);
        objQuest->setTitleKey(title);
        objQuest->setCountShort(countShort);
        
        if ( countShort == false )
        {
            objQuest->setCountMax(jsonValue["value"].GetUint64());
        }
        else
        {
            objQuest->setCountBigMax(jsonValue["value"].GetDouble());
        }
        
        {
            int rewardIdx = jsonValue["item_idx_1"].GetInt();
            int rewardParams = jsonValue["item_type_params_1"].GetInt();
            int rewardCount = jsonValue["item_count_1"].GetInt();
            objQuest->addReward(rewardIdx, rewardParams, rewardCount);   
        }
        
        {
            int rewardIdx = jsonValue["item_idx_2"].GetInt();
            int rewardParams = jsonValue["item_type_params_2"].GetInt();
            int rewardCount = jsonValue["item_count_2"].GetInt();
            objQuest->addReward(rewardIdx, rewardParams, rewardCount);
        }
         
        _listInfoMission.pushBack(objQuest);
    }
}
#pragma mark - Utils
std::vector<int> EventRaceManager::getListRandomSkinIdx()
{
    std::vector<int> result;
    
    auto listSkin = CostumeSkinManager::getInstance()->getListCostumeSkin();
    for(int i = 0; i < 6;)
    {
        int nRnd = random(0, (int)listSkin.size()-1);
        auto objSkin = listSkin.at(nRnd);
        int nIdx = objSkin->getIdx();
        
        auto iter = std::find(result.begin(), result.end(), nIdx);
        
        if(iter == result.end())
        {
            result.push_back(nIdx);
            i++;
        }
    }
    
    return result;
}

const bool EventRaceManager::isEventEndSeason()const
{
    bool result = false;
    
    auto objEvent = EventManager::getInstance()->getActivatedEvent(33);
    if(objEvent != nullptr)
    {
        if(objEvent->getEventEnd() - UtilsDate::getInstance()->getTime() < 0)
            return true;
    }
    
    if(EventManager::getInstance()->isActivatedEvent(34))
    {
        if(objEvent != nullptr)
        {
            if(objEvent->getEventEnd() - UtilsDate::getInstance()->getTime() > 0)
                return false;
        }
        
        return true;
    }
    
    return result;
}
#pragma mark - Simulate
void EventRaceManager::setCurrentRank()
{
    int nCompleteCount = 0;
    
    std::vector<std::pair<int, float>> listSection;
    listSection.clear();
    for(auto obj : _listInfoHorse)
    {
        if(obj->isComplateRace())
        {
            obj->setCurrentRank(obj->getFinalRank());
            nCompleteCount++;
            continue;
        }
        listSection.push_back(std::make_pair(obj->getIdx(), obj->getCurrentSection()));
    }
    
    std::sort(listSection.begin(), listSection.end(), EventRaceManager::sortRank);
    
    for(int i = 0; i < listSection.size(); ++i)
    {
        auto obj = getInfoHorse(listSection[i].first);
        if(obj != nullptr)
            obj->setCurrentRank(i+1 + nCompleteCount);
        
    }
}
void EventRaceManager::setCurrentSimulateRank()
{
    int nCompleteCount = 0;
    
    std::vector<std::pair<int, float>> listSection;
    listSection.clear();
    for(auto obj : _listInfoHorse)
    {
        if(obj->isComplateSimulate())
        {
            nCompleteCount++;
            continue;
        }
        listSection.push_back(std::make_pair(obj->getIdx(), obj->getCurrentSimulateSection()));
    }
    
    std::sort(listSection.begin(), listSection.end(), EventRaceManager::sortRank);
    
    for(int i = 0; i < listSection.size(); ++i)
    {
        auto obj = getInfoHorse(listSection[i].first);
        if(obj != nullptr)
            obj->setCurrentSimulateRank(i+1 + nCompleteCount);
        
    }
}
void EventRaceManager::requestSimulatingRace()
{
    int currnetRank = 1;
    int tick = 1;
    int subTick = 1;
    int presssureHorseIdx = getPressureHorseIdx();
    while(isSimulateAllComplete() == false)
    {
        for(auto obj : _listInfoHorse)
        {
            if(obj->getFinalRank() != 0)
                continue;
            
            if(currnetRank == 6)
            {
                obj->setFinalRank(currnetRank);
                break;
            }
            if(obj->isComplateSimulate())
            {
                obj->setFinalRank(currnetRank);
                currnetRank++;
            }
            
            bool isAbilityActive = false;
            int nBoostTurn = 0;
            float curSpeed = 4;
            float maxSpeed = 0;
            float curStamina = 0;
            float power = 0;
            float guts = 0;
            float condition = 0;
            float curSection = obj->getCurrentSimulateSection();
            std::string strAbilityType = "";
            
            InfoEventRaceStat* objStatAbility = obj->getInfoStat("ABILITY");
            InfoEventRaceStat* objStatSpeed = obj->getInfoStat("SPEED");
            InfoEventRaceStat* objStatStamina = obj->getInfoStat("STAMINA");
            InfoEventRaceStat* objStatPower = obj->getInfoStat("POWER");
            InfoEventRaceStat* objStatGuts = obj->getInfoStat("GUTS");
            InfoEventRaceStat* objStatCondition = obj->getInfoStat("CONDITION");
            
            //성향
            if(objStatAbility != nullptr)
            {
                strAbilityType = objStatAbility->getTendency();
                isAbilityActive = objStatAbility->isAbilityActive();
                
                auto objFrontHorse = getInfoHorse(obj->getIdx() - 1);
                auto objBackHorse = getInfoHorse(obj->getIdx() + 1);
                
                if(objFrontHorse != nullptr)
                {
                    auto objStatAbility = objFrontHorse->getInfoStat("ABILITY");
                    if(objStatAbility->getTendency().compare("DISRUPTER"))
                        strAbilityType = "";
                }
                if(objBackHorse != nullptr)
                {
                    auto objStatAbility = objBackHorse->getInfoStat("ABILITY");
                    if(objStatAbility->getTendency().compare("DISRUPTER"))
                        strAbilityType = "";
                }
                
                if(strAbilityType.compare("COPPYCAT") == 0)
                {
                    int nFrontIdx = obj->getIdx() - 1;
                    if(nFrontIdx <= 0)
                        nFrontIdx = (int)_listInfoHorse.size();
                    
                    auto objFrontHorse = getInfoHorse(nFrontIdx);
                    if(objFrontHorse != nullptr)
                    {
                        if(auto objCoppyAbility = objFrontHorse->getInfoStat("ABILITY"))
                            strAbilityType = objCoppyAbility->getTendency();
                    }
                }
            }
            //컨디션
            if(objStatCondition != nullptr)
            {
                condition = objStatCondition->getValue();
                //부담감
                if(presssureHorseIdx == obj->getIdx())
                {
                    if(strAbilityType.compare("PRESSURE_IMMUNITY") != 0)
                    {
                        condition -= 0.5;
                    }
                }
                if(_nSecondaryCorrection > 0)
                {
                    
                    if(getInvestCount() + _nCoin >= 100000000000000)
                    {
                        if(obj->getInvest() >= 1)
                            condition -= 0.5;
                    }
                }
            }
            //스피드
            if(objStatSpeed != nullptr)
            {
                maxSpeed = objStatSpeed->getValue() + condition;
            }
            //파워
            if(objStatPower != nullptr)
            {
                power = objStatPower->getValue() + condition;
            }
            //근성
            if(objStatGuts != nullptr)
            {
                guts = objStatGuts->getValue() + condition;
                
                if(strAbilityType.compare("SUPER_ GRIT") == 0)
                    guts += 1;
            }
            //부스터 턴
            if(curSection >= 0 && curSection < 33)
            {
                nBoostTurn = 1;
            }
            else if(curSection >= 33 && curSection < 66)
            {
                nBoostTurn = 2;
            }
            else if(curSection >= 66)
            {
                nBoostTurn = 3;
            }
            
            curSpeed += power * tick;
            
            if(curSpeed >= maxSpeed)
            {
                curSpeed = maxSpeed;
                if(strAbilityType.compare("HIGH_SPEED") == 0 && !isAbilityActive)
                {
                    objStatAbility->setIsAbilityActive(true);
                    objStatSpeed->setValue(objStatSpeed->getValue() + 1);
                }
            }
            
            //스태미너
            if(objStatStamina != nullptr)
            {
                float defaultStamina = objStatStamina->getValue() + condition;
                
                if(strAbilityType.compare("SUDDEN_START") == 0)
                {
                    defaultStamina -= 2;
                    curSpeed += 4;
                }
                if(strAbilityType.compare("PHYSICAL_CONTROL") == 0)
                {
                    if(curSpeed >= maxSpeed)
                    {
                        curStamina = defaultStamina - subTick;
                        subTick++;
                    }
                }
                else
                    curStamina = defaultStamina - tick;
                
                if(curStamina <= 0)
                    curStamina = 0;
            }
            
            if(curStamina <= 0)
                curSpeed = maxSpeed + guts;
            
            if(curSpeed <= 4)
                curSpeed = 4;
            
            if(obj->isBoost(nBoostTurn, strAbilityType))
            {
                curSpeed += 2;
            }
            
            obj->setCurrentSimulateSection(curSection + curSpeed);
            
            auto objLog = InfoEventRaceSimulateLog::create();
            objLog->setTick(tick);
            objLog->setSpeed(curSpeed);
            objLog->setStamina(curStamina);
            objLog->setSection(curSection + curSpeed);
            objLog->setIsBoost(obj->isBoost(nBoostTurn));
            
            obj->addSimulateSectionLog(objLog);
        }
        setCurrentSimulateRank();
        tick++;
    }
}
const bool EventRaceManager::isSimulateAllComplete() const
{
    int nComplate = 0;
    
    for(auto obj : _listInfoHorse)
    {
        if(obj->getFinalRank() != 0)
        {
            nComplate++;
        }
    }
    
    return nComplate >= _listInfoHorse.size();
}

#pragma mark - delegate
void EventRaceManager::subRaceReadyEvent(MafFunction<void()>* func)
{
    _eventRaceReady += func;
}
void EventRaceManager::unSubRaceReadyEvent(void* obj)
{
    _eventRaceReady.Clear(obj);
}
void EventRaceManager::invokeRaceReadyEvent()
{
    _eventRaceReady();
}

void EventRaceManager::subRaceStartEvent(MafFunction<void()>* func)
{
    _eventRaceStart += func;
}
void EventRaceManager::unSubRaceStartEvent(void* obj)
{
    _eventRaceStart.Clear(obj);
}
void EventRaceManager::invokeRaceStartEvent()
{
    _eventRaceStart();
}

void EventRaceManager::subRaceGameOverEvent(MafFunction<void(bool)>* func)
{
    _eventRaceGameOver += func;
}
void EventRaceManager::unSubRaceGameOverEvent(void* obj)
{
    _eventRaceGameOver.Clear(obj);
}
void EventRaceManager::invokeRaceGameOverEvent(bool isBankrupt)
{
    _isGameRunning = false;
    _eventRaceGameOver(isBankrupt);
}

void EventRaceManager::subStatusEvent(MafFunction<void(int)>* func)
{
    _eventStatus += func;
}
void EventRaceManager::unSubStatusEvent(void* obj)
{
    _eventStatus.Clear(obj);
}
void EventRaceManager::invokeStatusEvent(const int nIdx)
{
    _eventStatus(nIdx);
}

void EventRaceManager::subInvestmentEvent(MafFunction<void(int)>* func)
{
    _eventInvestment += func;
}
void EventRaceManager::unSubInvestmentEvent(void* obj)
{
    _eventInvestment.Clear(obj);
}
void EventRaceManager::invokeInvestmentEvent(const int nIdx)
{
    _eventInvestment(nIdx);
}

void EventRaceManager::subAdsEvent(MafFunction<void(int)>* func)
{
    _eventAds += func;
}
void EventRaceManager::unSubAdsEvent(void* obj)
{
    _eventAds.Clear(obj);
}
void EventRaceManager::invokeAdsEvent(const int nType)
{
    _eventAds(nType);
}

void EventRaceManager::subEventOverEvent(MafFunction<void()>* func)
{
    _eventEventOver += func;
}
void EventRaceManager::unSubEventOverEvent(void* obj)
{
    _eventEventOver.Clear(obj);
}
void EventRaceManager::invokeEventOverEvent()
{
    _eventEventOver();
}

#pragma mark -network
void EventRaceManager::requestInfo(const std::function<void(bool, int)>& callback)
{
    //
    _callbackInfo = callback;
    const std::string url = "/event/race/info";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_costume_list", getListRandomSkinIdx());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseInfo(response,dd);
    };
    req->send(cb);
}



void EventRaceManager::responseInfo(cocos2d::network::HttpResponse* const response, const std::string &data)
{
    _isServerDispatch = false;
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
            _callbackInfo = nullptr;
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
    _nCoin = jsonParser["_event_coin"].GetDouble();
    _nTicket = jsonParser["_event_ticket"].GetDouble();
    
    _isHiddenAds = jsonParser["_is_ads"].GetBool();
    _gameRewardMag = jsonParser["_scale"].GetDouble();
    _gameResetCount = jsonParser.HasMember("_reset_count") == true ? jsonParser["_reset_count"].GetInt() : 0;
    
    _listInfoHorse.clear();
    for ( int i = 0; i < jsonParser["_player_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_player_list"][i];
        
        const int nSkinIdx = jsonValue["idx"].GetInt();
        auto arrayStat = jsonValue["state"].GetArray();
        const double nInvest = jsonValue["investment"].GetDouble();
        
        InfoEventRaceHorse* objHorse = InfoEventRaceHorse::create();
        objHorse->setIdx(i+1);
        objHorse->setSkinIdx(nSkinIdx);
        objHorse->setInvest(nInvest);
        for(int j = 0; j < arrayStat.Size(); ++j)
        {
            const int nStatIdx = arrayStat[j].GetInt();
            auto objStat = getInfoRaceStatByIdx(nStatIdx);
            if(objStat != nullptr)
                objHorse->addInfoStat(objStat);
        }
        _listInfoHorse.pushBack(objHorse);
    }
    
    bool isEventEndReward = false;
    Vector<InfoItems*> listReward;
    if(jsonParser.HasMember("_reward"))
    {
        // reward
        std::string strReward         = jsonParser["_reward"].GetString();
        ItemsManager::getInstance()->addItems(strReward, false);
        listReward = ItemsManager::getInstance()->getConvertArray(strReward);
        
        if(listReward.size() > 0)
            isEventEndReward = true;
    }
    if(jsonParser.HasMember("_items"))
    {
        std::string strItems         = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->setItemsServer(strItems);
    }
    
    if(isEventEndReward)
    {
        if(listReward.size() > 0)
        {
            auto popup = PopupEventRaceRankReward::create(listReward);
            popup->show();
        }
        
        if(!isEventEndSeason())
        {
            EventManager::getInstance()->requestInfo([=](bool){
                
                if ( _callbackInfo != nullptr )
                {
                    _callbackInfo(true, 1);
                    _callbackInfo = nullptr;
                }
            });
            return;
        }
    }
    
    if ( _callbackInfo != nullptr )
    {
        _callbackInfo(true, 1);
        _callbackInfo = nullptr;
    }
}

void EventRaceManager::requestReset(const std::function<void(bool, int)>& callback, bool bPurchase)
{
    //
    _callbackReset = callback;
    
    const std::string url = "/event/race/reset";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_isPurchase", bPurchase);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseReset(response,dd);
    };
    req->send(cb);
}

void EventRaceManager::responseReset(cocos2d::network::HttpResponse* const response, const std::string &data)
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
        if ( _callbackReset != nullptr )
        {
            _callbackReset(false, 0);
            _callbackReset = nullptr;
        }
        
        return;
    }
    
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackReset != nullptr )
        {
            _callbackReset(false, nResult);
            _callbackReset = nullptr;
        }
        return;
    }
    
    _nCoin = jsonParser["_event_coin"].GetDouble();
    _nTicket = jsonParser["_event_ticket"].GetDouble();
    
    _gameResetCount = jsonParser.HasMember("_reset_count") == true ? jsonParser["_reset_count"].GetInt() : 0;
    
    _listInfoHorse.clear();
    for ( int i = 0; i < jsonParser["_player_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_player_list"][i];
        
        const int nSkinIdx = jsonValue["idx"].GetInt();
        auto arrayStat = jsonValue["state"].GetArray();
        const double nInvest = jsonValue["investment"].GetDouble();
        
        InfoEventRaceHorse* objHorse = InfoEventRaceHorse::create();
        objHorse->setIdx(i+1);
        objHorse->setSkinIdx(nSkinIdx);
        objHorse->setInvest(nInvest);
        for(int j = 0; j < arrayStat.Size(); ++j)
        {
            const int nStatIdx = arrayStat[j].GetInt();
            auto objStat = getInfoRaceStatByIdx(nStatIdx);
            if(objStat != nullptr)
                objHorse->addInfoStat(objStat);
        }
        _listInfoHorse.pushBack(objHorse);
    }
    
    if ( _callbackReset != nullptr )
    {
        _callbackReset(true, 1);
        _callbackReset = nullptr;
    }
}

void EventRaceManager::requestAds(const std::function<void(bool, int, int)>& callback, const int nType)
{
    //
    _callbackAds = callback;
    const std::string url = "/event/race/update/ads";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", nType);
    MafResponseNew cb = [&, nType](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseAds(response, dd, nType);
    };
    req->send(cb);
}

void EventRaceManager::responseAds(cocos2d::network::HttpResponse* const response, const std::string &data, const int nType)
{
    _isServerDispatch = false;
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
        if ( _callbackAds != nullptr )
        {
            _callbackAds(false, 0, nType);
            _callbackAds = nullptr;
        }
        
        return;
    }
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackAds != nullptr )
        {
            _callbackAds(false, nResult, nType);
            _callbackAds = nullptr;
        }
        return;
    }
    
    _isHiddenAds = jsonParser["_is_ads"].GetBool();
    _gameRewardMag = jsonParser["_scale"].GetDouble();
    
    if ( _callbackAds != nullptr )
    {
        _callbackAds(true, 1, nType);
        _callbackAds = nullptr;
    }
}

void EventRaceManager::requestMission(const std::function<void(bool, int)>& callback)
{
    //
    _callbackMission = callback;
    
    const std::string url = "/event/race/mission/info";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseMission(response, dd);
    };
    req->send(cb);
}

void EventRaceManager::responseMission(cocos2d::network::HttpResponse* const response, const std::string &data)
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
        if ( _callbackMission != nullptr )
        {
            _callbackMission(false, 0);
            _callbackMission = nullptr;
        }
        
        return;
    }
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackMission != nullptr )
        {
            _callbackMission(false, nResult);
            _callbackMission = nullptr;
        }
        return;
    }
     
    for ( int i = 0; i < jsonParser["_mission"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_mission"][i];
        
        auto objMission = getInfoMission(jsonValue["idx"].GetInt());
        if ( objMission != nullptr )
        {
            if ( objMission->isCountShort() == true )
            {
                objMission->setCountBigNow(jsonValue["value"].GetDouble());
            }
            else
            {
                objMission->setCountNow(jsonValue["value"].GetUint64());
            }
            objMission->setReceive(jsonValue["is_reward"].GetInt());
        }
            
    }
    
    //
    RefreshReddotManager::getInstance()->setValue(E_REFRESH_REDDOT::EVENT_RACE_MISSION, isCompleteMission());
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::EVENT_RACE_MISSION);
    
    //
    if ( _callbackMission != nullptr )
    {
        _callbackMission(true, 1);
        _callbackMission = nullptr;
    }
}

void EventRaceManager::requestMissionReward(const std::function<void(bool, int)>& callback)
{
    //
    _callbackMission = callback;
    
    const std::string url = "/event/race/mission/reward";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseMissionReward(response, dd);
    };
    req->send(cb);
}

void EventRaceManager::responseMissionReward(cocos2d::network::HttpResponse* const response, const std::string &data)
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
        if ( _callbackMission != nullptr )
        {
            _callbackMission(false, 0);
            _callbackMission = nullptr;
        }
        
        return;
    }
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackMission != nullptr )
        {
            _callbackMission(false, nResult);
            _callbackMission = nullptr;
        }
        return;
    }
     
    for ( int i = 0; i < jsonParser["_mission"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_mission"][i];
        
        auto objMission = getInfoMission(jsonValue["idx"].GetInt());
        if ( objMission != nullptr )
        {
            if ( objMission->isCountShort() == true )
            {
                objMission->setCountBigNow(jsonValue["value"].GetDouble());
            }
            else
            {
                objMission->setCountNow(jsonValue["value"].GetUint64());
            }
            objMission->setReceive(jsonValue["is_reward"].GetInt());
        }
    }
    
    //
    std::string strReward = jsonParser["_reward"].GetString();
    std::string strItems = jsonParser["_items"].GetString();
    ItemsManager::getInstance()->addItems(strReward);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    //
    RefreshReddotManager::getInstance()->setValue(E_REFRESH_REDDOT::EVENT_RACE_MISSION, isCompleteMission());
    RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::EVENT_RACE_MISSION);
    
    //
    if ( _callbackMission != nullptr )
    {
        _callbackMission(true, 1);
        _callbackMission = nullptr;
    }
}

void EventRaceManager::requestInvestment(const std::function<void(bool, int)>& callback, const int nIdx, const double nBetCount)
{
    //
    _callbackInvestment = callback;
    const std::string url = "/event/race/update/investment";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_idx", nIdx);
    req->setDataDouble("_count", nBetCount);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseInvestment(response,dd);
    };
    req->send(cb);
}

void EventRaceManager::responseInvestment(cocos2d::network::HttpResponse* const response, const std::string &data)
{
    _isServerDispatch = false;
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
        if ( _callbackInvestment != nullptr )
        {
            _callbackInvestment(false, 0);
            _callbackInvestment = nullptr;
        }
        
        return;
    }
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackInvestment != nullptr )
        {
            _callbackInvestment(false, nResult);
            _callbackInvestment = nullptr;
        }
        return;
    }
    
    _nCoin = jsonParser["_event_coin"].GetDouble();
    _listInfoHorse.clear();
    for ( int i = 0; i < jsonParser["_player_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_player_list"][i];
        
        const int nSkinIdx = jsonValue["idx"].GetInt();
        auto arrayStat = jsonValue["state"].GetArray();
        const double nInvest = jsonValue["investment"].GetDouble();
        
        InfoEventRaceHorse* objHorse = InfoEventRaceHorse::create();
        objHorse->setIdx(i+1);
        objHorse->setSkinIdx(nSkinIdx);
        objHorse->setInvest(nInvest);
        for(int j = 0; j < arrayStat.Size(); ++j)
        {
            const int nStatIdx = arrayStat[j].GetInt();
            auto objStat = getInfoRaceStatByIdx(nStatIdx);
            if(objStat != nullptr)
                objHorse->addInfoStat(objStat);
        }
        _listInfoHorse.pushBack(objHorse);
    }
    
    if ( _callbackInvestment != nullptr )
    {
        _callbackInvestment(true, 1);
        _callbackInvestment = nullptr;
    }
}


void EventRaceManager::requestGameStart(const std::function<void(bool, int)>& callback)
{
    requestSimulatingRace();
    //
    _callbackGameStart = callback;
    const std::string url = "/event/race/reward";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_costume_list", getListRandomSkinIdx());
    
    
    std::vector<MafHttpModelNew*> ArrayRank;
    for(const auto obj : _listInfoHorse)
    {
        std::vector<MafHttpModelNew*> ObjList;
        MafHttpModelNew *val1 = new MafHttpModelNew();
        MafHttpModelNew *val2 = new MafHttpModelNew();
        val1->setData("rank", obj->getFinalRank());
        val2->setDataDouble("idx", obj->getSkinIdx());
        ObjList.push_back(val1);
        ObjList.push_back(val2);
        MafHttpModelNew *valObj = new MafHttpModelNew();
        valObj->setData("_rank_list", ObjList, true);
        ArrayRank.push_back(valObj);
    }
    req->setDataObjecArray("_rank_list", ArrayRank);

    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseGameStart(response,dd);
    };
    req->send(cb);
}

void EventRaceManager::responseGameStart(cocos2d::network::HttpResponse* const response, const std::string &data)
{
    _isServerDispatch = false;
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
        if ( _callbackGameStart != nullptr )
        {
            _callbackGameStart(false, 0);
            _callbackGameStart = nullptr;
        }
        
        return;
    }
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackGameStart != nullptr )
        {
            _callbackGameStart(false, nResult);
            _callbackGameStart = nullptr;
        }
        return;
    }
    
    _isGameRunning = true;
    _nTicket = jsonParser["_event_ticket"].GetDouble();
    
    bool isHacking = false;
    bool isNoneHorseHack = false;
    std::vector<int> listHackIdx;
    for ( int i = 0; i < jsonParser["_rank_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_rank_list"][i];

        const int nSkinIdx = jsonValue["idx"].GetFloat();
        const int nRank = jsonValue["rank"].GetInt();

        auto objHorse = getInfoHorseBySkinIdx(nSkinIdx);
        if(objHorse == nullptr || objHorse->getFinalRank() != nRank)
        {
            isHacking = true;
            if(objHorse == nullptr)
            {
                listHackIdx.push_back(nSkinIdx);
                isNoneHorseHack = true;
            }
            else
                objHorse->setHackRank(nRank);
        }
    }
    
    if(isHacking)
    {
        std::string strMassage = "레이스 이벤트 패킷 변조 감지\n";
        
        if(isNoneHorseHack)
        {
            strMassage += "없는 말 만듦 : ";
            for(auto idx : listHackIdx)
            {
                strMassage += MafUtils::format("%d, ", idx);
            }
        }
        AccountManager::getInstance()->requestReport(false, E_REPORT_TYPE::HACK, strMassage, AccountManager::getInstance()->getUserIdx(), AccountManager::getInstance()->getUserPlatform(), nullptr);
    }
    // reward
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    ItemsManager::getInstance()->addItems(strReward, false);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    if ( _callbackGameStart != nullptr )
    {
        _callbackGameStart(true, 1);
        _callbackGameStart = nullptr;
    }
}


void EventRaceManager::onEventActivate(const int nIdx)
{
    if(nIdx == 33 && !_isEventActivate)
    {
        PopupLoading::show();
        requestInfo([=](bool, int){
            PopupLoading::hide();
        });
        EventManager::getInstance()->AddActivateRequestCount();
    }
}
