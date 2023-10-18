//
//  NewRaidManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/09/29.
//

#include "NewRaidManager.h"

#include "GameObject/InfoQuest.h"
#include "ManagerGame/UserInfoManager.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/WeaponSpiritManager.h"
#include "ManagerGame/ServerMissionManager.h"
#include "ManagerGame/AchievementManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/SpecialMissionManager.h"

USING_NS_CC;

NewRaidManager::NewRaidManager(void):
_isTicket(false)
,_isServerDispatch(false)
,_isAttack(false)
,_nSeason(0)
,_nTotalDamage(0)
,_nNowDamage(0)
,_nTotalPoint(0)
,_nAttackCount(0)
,_nAdsBuyCount(0)
,_nBuyCount(0)
,_nLevel(0)
,_nExp(0)
,_nAbilityPoint(0)
,_nTotalAbilityPoint(0)
,_nDragonBall(0)
,_nMarketRefreshCount(0)

,_nNextAttackTime(0)
,_nEndSeasonTime(0)
,_nSeasonBreakTime(0)

,_callbackInfo(nullptr)
,_callbackAttack(nullptr)
,_callbackLevelup(nullptr)
,_callbackMission(nullptr)
,_callbackBuyDragonBall(nullptr)
,_callbackTradingMarketInfo(nullptr)
,_callbackTradingMarketBuy(nullptr)
{
    _listInfoDragon.clear();
    _listInfoBongLevel.clear();
    _listInfoStat.clear();
    _listInfoSetting.clear();
    _listInfoQuest.clear();
    _listInfoStatIncrease.clear();
    _listIntroReward.clear();
}

NewRaidManager::~NewRaidManager(void)
{
    _listInfoDragon.clear();
    _listInfoBongLevel.clear();
    _listInfoStat.clear();
    _listInfoSetting.clear();
    _listInfoQuest.clear();
    _listInfoStatIncrease.clear();
    _listIntroReward.clear();
}

bool NewRaidManager::init()
{
    return true;
}

#pragma mark - Load Data
void NewRaidManager::setLoad()
{
    setLoadQuest();
    setLoadDragon();
    setLoadBongLevel();
    setLoadBongStat();
    setLoadBongSetting();
    setLoadStatIncrease();
    setLoadTradingMarket();
}

void NewRaidManager::setLoadQuest()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::RAID_NEW_QUEST);
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoQuest.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        auto idx = jsonValue["idx"].GetInt();
        auto type = jsonValue["type"].GetInt();
        auto countMax = jsonValue["value"].GetFloat();
        auto titleKey = jsonValue["text_key"].GetString();
        auto desc = MafUtils::format("%d", (int)countMax);
        auto reward = jsonValue["reward"].GetString();
        
        auto objQuest = InfoQuest::create();
        objQuest->setIdx(idx);
        objQuest->setType(type);
        objQuest->setCountMax(countMax);
        objQuest->setTitleKey(titleKey);
        objQuest->setDesc(desc);
        objQuest->setReward(reward);
        
        //
        _listInfoQuest.pushBack(objQuest);
    }
}

void NewRaidManager::setLoadDragon()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::RAID_DRAGON);
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoDragon.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        auto obj = InfoRaidDragon::create();
        obj->setLevel(jsonValue["lv"].GetInt());
        obj->setHP(jsonValue["dragon_hp"].GetFloat());
        
        _listInfoDragon.pushBack(obj);
    }
}

void NewRaidManager::setLoadBongLevel()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::RAID_BONG_LEVEL);
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoBongLevel.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        auto obj = InfoBongContentLevel::create();
        obj->setLevel(jsonValue["lv"].GetInt());
        obj->setTotalEXP(jsonValue["exp_total"].GetFloat());
        
        _listInfoBongLevel.pushBack(obj);
    }
}

void NewRaidManager::setLoadBongStat()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::RAID_BONG_STAT);
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoStat.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        auto obj = InfoRaidBongStat::create();
        obj->setIdx(jsonValue["idx"].GetInt());
        obj->setMaxLv(jsonValue["max_lv"].GetInt());
        obj->setIconPath(jsonValue["icon"].GetString());
        obj->setTextKey(jsonValue["text_key"].GetString());
        
        _listInfoStat.pushBack(obj);
    }
}

void NewRaidManager::setLoadBongSetting()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::RAID_BONG_SETTING);
    
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
}

void NewRaidManager::setLoadStatIncrease()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::RAID_STAT_INCREASE);
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoStatIncrease.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        auto obj = InfoRaidStatIncrease::create();
        obj->setLv(jsonValue["lv"].GetInt());
        obj->setDmg(jsonValue["dmg"].GetInt());
        obj->setCriDmg(jsonValue["cri_dmg"].GetFloat());
        obj->setCriRate(jsonValue["cri_rate"].GetFloat());
        obj->setDoubleRate(jsonValue["double_rate"].GetFloat());
        _listInfoStatIncrease.pushBack(obj);
    }
}

void NewRaidManager::setLoadTradingMarket()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::RAID_TRADING_MARKET);
    
    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoTradingMarketReward.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        int nIdx = jsonValue["idx"].GetInt();
        int nType = jsonValue["limit_type"].GetInt();
        int nMaxCount = jsonValue["limit_count"].GetInt();
        auto listReward = ItemsManager::getInstance()->getConvertArray(jsonValue["item"].GetString());
        auto listCost = ItemsManager::getInstance()->getConvertArray(jsonValue["cost"].GetString());
        InfoItems* objReward = nullptr;
        InfoItems* objCost = nullptr;
        if ( listReward.size() > 0)
        {
            objReward = listReward.at(0);
        }
        if(listCost.size() > 0)
        {
            objCost = listCost.at(0);
        }
        
        auto obj = InfoRaidTradingMarketReward::create();
        obj->setIdx(nIdx);
        obj->setType((InfoRaidTradingMarketReward::E_TYPE)nType);
        obj->setMaxBuyCount(nMaxCount);
        obj->setItem(objReward);
        obj->setPrice(objCost);
        
        _listInfoTradingMarketReward.pushBack(obj);
    }
}

#pragma mark - set, get
Vector<InfoQuest*> NewRaidManager::getQuestList()
{
    return _listInfoQuest;
}

Vector<InfoBongContentLevel*> NewRaidManager::getInfoBongLevelList()
{
    return _listInfoBongLevel;
}

Vector<InfoRaidBongStat*> NewRaidManager::getInfoBongStatList()
{
    return _listInfoStat;
}

Vector<InfoRaidDragon*> NewRaidManager::getInfoDragonList()
{
    return _listInfoDragon;
}

Vector<InfoRaidTradingMarketReward*> NewRaidManager::getListActivateTradingMarketReward()
{
    Vector<InfoRaidTradingMarketReward*> result;
    
    for(auto obj : _listInfoTradingMarketReward)
    {
        if(obj->isSaleCondition())
            result.pushBack(obj);
    }
    
    return result;
}
Vector<InfoRaidTradingMarketReward*> NewRaidManager::getListTradingMarketReward()
{
    return _listInfoTradingMarketReward;
}

InfoBongContentLevel* NewRaidManager::getInfoBongLevel(int nLv)
{
    InfoBongContentLevel* result = nullptr;
    
    for(auto obj : _listInfoBongLevel)
    {
        if(obj->getLevel() == nLv)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

InfoRaidDragon* NewRaidManager::getNowDragon()
{
    InfoRaidDragon* result = nullptr;
    
    double tmpDamage = _nTotalDamage;
    for(auto obj : _listInfoDragon)
    {
        if(tmpDamage - obj->getHP() >= 0)
        {
            tmpDamage -= obj->getHP();
        }
        else
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

InfoSetting* NewRaidManager::getRaidSetting(std::string strKey)
{
    InfoSetting* result = nullptr;
    
    for(auto obj : _listInfoSetting)
    {
        if(obj->getKey() == strKey)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

InfoRaidStatIncrease* NewRaidManager::getRaidStatIncrease(int nLv)
{
    InfoRaidStatIncrease* result = nullptr;
    
    for(auto obj : _listInfoStatIncrease)
    {
        if(obj->getLv() == nLv)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
InfoRaidBongStat* NewRaidManager::getStat(int nIdx)
{
    InfoRaidBongStat* result = nullptr;
    
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

InfoRaidTradingMarketReward* NewRaidManager::getRaidTradingMarketReward(int nIdx)
{
    InfoRaidTradingMarketReward* result = nullptr;
    
    for(auto obj : _listInfoTradingMarketReward)
    {
        if(obj->getIdx() == nIdx)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

int NewRaidManager::getSeason()
{
    return _nSeason;
}

double NewRaidManager::getTotalDamage()
{
    return _nTotalDamage;
}

int NewRaidManager::getTotalPoint()
{
    return _nTotalPoint;
}

int NewRaidManager::getAttackCount()
{
    return _nAttackCount;
}

int NewRaidManager::getTotalMaxAttackCount()
{
    int result = 0;
    auto objSetting = getRaidSetting("attack_count");
    
    if(objSetting != nullptr)
        result += objSetting->getValueToInt();
    
    result += _nAdsBuyCount;
    result += _nBuyCount;
    
    return result;
}

int NewRaidManager::getAdsBuyCount()
{
    return _nAdsBuyCount;
}

int NewRaidManager::getBuyCount()
{
    return _nBuyCount;
}

int NewRaidManager::getLevel()
{
    return _nLevel;
}

int NewRaidManager::getMaxLevel()
{
    return (int)_listInfoBongLevel.size();;
}

int NewRaidManager::getEXP()
{
    return _nExp;
}

int NewRaidManager::getAbilityPoint()
{
    return _nAbilityPoint;
}

int NewRaidManager::getAdsDragonBallCount()
{
    return _nAdsDragonBallCount;
}

void NewRaidManager::setAdsDragonBallCount(int value)
{
    _nAdsDragonBallCount = value;
}

int NewRaidManager::getDragonBall()
{
    return _nDragonBall;
}

void NewRaidManager::setDragonBall(int value)
{
    _nDragonBall = value;
}

int NewRaidManager::getTotalAbilityPoint()
{
    return _nTotalAbilityPoint;
}

int NewRaidManager::getNextAttackTime()
{
    return _nNextAttackTime;
}

int NewRaidManager::getEndSeasonTime()
{
    return _nEndSeasonTime;
}

int NewRaidManager::getSeasonBreakTime()
{
    return _nSeasonBreakTime;
}

bool NewRaidManager::isTicket()
{
    return _isTicket;
}

bool NewRaidManager::isAttack()
{
    return _isAttack;
}

bool NewRaidManager::isServerDispatch()
{
    return _isServerDispatch;
}

void NewRaidManager::setIsAttack(bool value)
{
    _isAttack = value;
}

bool NewRaidManager::isMissionComplete()
{
    bool result = false;
    
    for(auto obj : _listInfoQuest)
    {
        if(!obj->isReceive() && (obj->getCountNow() >= obj->getCountMax()))
            result = true;
    }
    
    return result;
}

bool NewRaidManager::isLevelUpPossible()
{
    bool result = false;
    
    auto infoLv = getInfoBongLevel(_nLevel + 1);
    int nTotalEXP = 0;
    int nPercent = 0;
    
    if(infoLv != nullptr)
    {
        nTotalEXP = infoLv->getTotalEXP();
    }
    nPercent = (double)_nExp / nTotalEXP * 100;
    if ( nPercent > 100 )
        nPercent = 100;
    
    if(_nLevel >= getMaxLevel())
    {
        nPercent = 0;
    }
    
    return result;
}
bool NewRaidManager::isRaidEnterCondition()
{
    bool result = false;
    
    int nRevival = UserInfoManager::getInstance()->getRevival(E_PLACE::DG_NORMAL);
    if ( nRevival > 1 )
    {
        result = true;
    }
    
    if ( UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL) >= 1000 || UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL) >= 1000 )
    {
        result = true;
    }
    
    return result;
}
double NewRaidManager::getActualDamage()
{
    double result = _nTotalDamage;
    
    for(auto obj : _listInfoDragon)
    {
        if(result - obj->getHP() >= 0)
        {
            result -= obj->getHP();
        }
        else
        {
            break;
        }
    }
    
    return result;
}

double NewRaidManager::getNowDamage()
{
    return _nNowDamage;
}

double NewRaidManager::getAccrueEffect(E_RAID_ACCRUE_EFFECT_TYPE eType)
{
    double result = 0;
    
    std::string strKey = "";
    switch(eType)
    {
        case E_RAID_ACCRUE_EFFECT_TYPE::DAMAGE:
        {
            strKey = "accrue_coefficient_dmg";
        }break;
        case E_RAID_ACCRUE_EFFECT_TYPE::GOLD:
        {
            strKey = "accrue_coefficient_gold";
        }break;
        case E_RAID_ACCRUE_EFFECT_TYPE::KEY:
        {
            strKey = "accrue_coefficient_key";
        }break;
    }
    
    auto obj = getRaidSetting(strKey);
    if(obj == nullptr)
        return result;
    
    int fCoefficient = obj->getValueToInt();
    
    result = (_nTotalAbilityPoint / 3) * fCoefficient;
    
    if(eType == E_RAID_ACCRUE_EFFECT_TYPE::DAMAGE)
    {
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::BODY,E_COSTUME_IDX::IDX_31))
        {
            result *= 2;
        }
    }

    return result;
}

double NewRaidManager::getAccrueEffectNowSeason(E_RAID_ACCRUE_EFFECT_TYPE eType)
{
    double result = 0;
    
    std::string strKey = "";
    switch(eType)
    {
        case E_RAID_ACCRUE_EFFECT_TYPE::DAMAGE:
        {
            strKey = "accrue_coefficient_dmg";
        }break;
        case E_RAID_ACCRUE_EFFECT_TYPE::GOLD:
        {
            strKey = "accrue_coefficient_gold";
        }break;
        case E_RAID_ACCRUE_EFFECT_TYPE::KEY:
        {
            strKey = "accrue_coefficient_key";
        }break;
    }
    
    auto obj = getRaidSetting(strKey);
    if(obj == nullptr)
        return result;
    
    int fCoefficient = obj->getValueToInt();
    
    int nTotalLv = 0;
    
    for(auto obj : _listInfoStat)
    {
        nTotalLv += obj->getCurrentLv();
    }
    
    result = (nTotalLv / 3) * fCoefficient;
    
    return result;
}
Vector<InfoItems*> NewRaidManager::getListIntroReward()
{
    return _listIntroReward;
}
void  NewRaidManager::clearListIntroReward()
{
    _listIntroReward.clear();
}

void  NewRaidManager::clearListTradingMarketReward()
{
    for(auto obj : _listInfoTradingMarketReward)
    {
        obj->setActivate(false);
        obj->setBuyCount(0);
        obj->setEndTime(0);
        obj->setStartTime(0);
    }
}

int NewRaidManager::getMarketRefreshCount()
{
    return _nMarketRefreshCount;
}
#pragma mark - network
void NewRaidManager::requestInfo(const std::function<void(bool, int)>& callback, bool isIntro)
{
    _callbackInfo = callback;
    //
    std::string url = "/new/raid/info/%d";
    
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&, isIntro](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(response, dd, isIntro);
    };
    req->send(cb);
}

void NewRaidManager::responseInfo(cocos2d::network::HttpResponse* response,std::string &data, bool isIntro)
{
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
            _callbackInfo(false, 0);
        }
        return;
    }
    
    int nResult = jsonParser["_result"].GetInt();
    if(nResult == 1 || nResult == 8 || nResult == 9)
    {
        _nTotalAbilityPoint   = jsonParser["_total_ability_count"].GetInt();
        _nSeasonBreakTime = jsonParser["_break_time"].GetInt();
    }
    if ( nResult != 1 )
    {
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false, nResult);
            _callbackInfo = nullptr;
        }
        return;
    }
    
    _nSeason         = jsonParser["_season"].GetInt();
    _nTotalDamage    = jsonParser["_total_damage"].GetInt();
    if(!jsonParser["_total_point"].IsNull())
        _nTotalPoint     = jsonParser["_total_point"].GetInt();
    _nAttackCount    = jsonParser["_count"].GetInt();
    _nNextAttackTime   = jsonParser["_next_attack_time"].GetInt();
    _nAdsBuyCount    = jsonParser["_ads_count"].GetInt();
    _nBuyCount       = jsonParser["_buy_count"].GetInt();
    _isTicket         = jsonParser["_is_ticket"].GetBool();
    _nLevel          = jsonParser["_level"].GetInt();
    _nExp            = jsonParser["_exp"].GetInt();
    _nAbilityPoint   = jsonParser["_ability_point"].GetInt();
    _nEndSeasonTime   = jsonParser["_end_season_time"].GetInt();
    _nAdsDragonBallCount   = jsonParser["_dragon_ball_ads_count"].GetInt();
    
    
    int nTotalMileage   = jsonParser["_mileage"].GetInt();
    int nSummonTicket_1   = jsonParser["_draw_ticket_1"].GetInt();
    int nSummonTicket_11   = jsonParser["_draw_ticket_11"].GetInt();
    int nSummonTicket_33   = jsonParser["_draw_ticket_33"].GetInt();
    
    int nAdsSummonCount   = jsonParser["_spirit_draw_ads_count"].GetInt();
    int nAdsSummonCooltime   = jsonParser["_spirit_draw_ads_cooltime"].GetInt();
    
    WeaponSpiritManager::getInstance()->setTotalMileage(nTotalMileage);
    WeaponSpiritManager::getInstance()->setSummonTicket_1(nSummonTicket_1);
    WeaponSpiritManager::getInstance()->setSummonTicket_11(nSummonTicket_11);
    WeaponSpiritManager::getInstance()->setSummonTicket_33(nSummonTicket_33);
    WeaponSpiritManager::getInstance()->setAdsSummonCooldown(nAdsSummonCooltime);
    WeaponSpiritManager::getInstance()->setAdsSummonCount(nAdsSummonCount);
    
    //업적
    AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_NEW_RAID_ACCRUE_STAT, _nTotalAbilityPoint);
    
    int nAchiveLv = std::atoi(MafAes256::Decrypt(UserDefault::getInstance()->getStringForKey(KEY_R_S_MAX_LV, "0")).c_str());
    int nAchiveDamage = std::atoi(MafAes256::Decrypt(UserDefault::getInstance()->getStringForKey(KEY_R_S_MAX_DAMAGE, "0")).c_str());
    
    if(nAchiveLv < _nLevel)
    {
        AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_NEW_RAID_SEASON_LEVEL, _nLevel);
        UserDefault::getInstance()->setStringForKey(KEY_R_S_MAX_LV, MafAes256::Encrypt(std::to_string(_nLevel)));
    }
    if(nAchiveDamage < _nTotalDamage)
    {
        AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_NEW_RAID_SEASON_DAMAGE, _nTotalDamage);
        UserDefault::getInstance()->setStringForKey(KEY_R_S_MAX_DAMAGE, MafAes256::Encrypt(std::to_string((int)_nTotalDamage)));
    }

    
    int nMileageRewardIdx = jsonParser["_mileage_reward"].Size() + 1;
    
    WeaponSpiritManager::getInstance()->setCurrentMileageRewardIdx(nMileageRewardIdx);
    
    for ( int i = 0; i < jsonParser["_quest_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_quest_list"][i];
        
        for (auto objQuest : _listInfoQuest)
        {
            if ( objQuest->getIdx() == jsonValue["id"].GetInt() )
            {
                objQuest->setType(jsonValue["task_type"].GetInt());
                if ( jsonValue["task_count"].IsDouble() )
                    objQuest->setCountMax(jsonValue["task_count"].GetDouble());
                else
                    objQuest->setCountMax(jsonValue["task_count"].GetInt());
                objQuest->setCountNow(jsonValue["cureent_task_count"].GetInt());
                objQuest->setReceive(jsonValue["is_complete"].GetBool());
            }
        }
    }
    
    for ( int i = 0; i < jsonParser["_ability_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_ability_list"][i];
        
        for (auto obj : _listInfoStat)
        {
            if ( obj->getIdx() == jsonValue["idx"].GetInt() )
            {
                obj->setCurrentLv(jsonValue["level"].GetInt());
            }
        }
    }
    
    // reward
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    
    ItemsManager::getInstance()->addItems(strReward, !isIntro);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    if(isIntro)
    {
        _listIntroReward = ItemsManager::getInstance()->getConvertArray(strReward);
    }
    
    if(jsonParser.HasMember("_items"))
    {
        auto listItems = ItemsManager::getInstance()->getConvertArray(strItems);
        for(auto& obj : listItems)
        {
            if( obj->getIdx() == (int)E_ITEMS::NEW_RAID_DRAGON_BALL )
            {
                _nDragonBall = std::atoi(obj->getCount().c_str());
            }
        }
    }
    
    if(!jsonParser["_dragon_ball"].IsNull())
        _nDragonBall = jsonParser["_dragon_ball"].GetInt();
    
    if ( _callbackInfo != nullptr )
    {
        _callbackInfo(true, 1);
    }
    
    _callbackInfo = nullptr;
}

void NewRaidManager::requestAttack(const std::function<void(int, bool, bool, Vector<InfoItems*>)>& pCallback, int count)
{
    //
    _callbackAttack = pCallback;
    
    bool isCostume = false;
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::BODY, E_COSTUME_IDX::IDX_38))
        isCostume = true;
    
    //
    std::string url = "/new/raid/AttackRaid";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_season", _nSeason);
    req->setData("_count", count);
    req->setData("_costume", (int)isCostume);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseAttack(response,dd);
    };
    req->send(cb);
}

void NewRaidManager::responseAttack(cocos2d::network::HttpResponse* response,std::string &data)
{
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
    Vector<InfoItems*> listReward;
    listReward.clear();
    if ( bError == true )
    {
        if ( _callbackAttack != nullptr )
        {
            _callbackAttack(0, false, false, listReward);
            _callbackAttack = nullptr;
        }
        return;
    }
    
   
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackAttack != nullptr )
        {
            _callbackAttack(nResult, false, false, listReward);
            _callbackAttack = nullptr;
        }
        return;
    }
    
    //미션
    ServerMissionManager::getInstance()->setServerMission(E_DAILY_QUEST::RAID_ATT, 1);
    SpecialMissionManager::getInstance()->clearSpecialMission(91);
    
    _nTotalDamage    = jsonParser["_total_damage"].GetInt();
    _nAttackCount    = jsonParser["_count"].GetInt();
    _nTotalPoint     = jsonParser["_total_point"].GetInt();
    _nExp            = jsonParser["_exp"].GetInt();
    _nNowDamage       = jsonParser["_damage"].GetInt();
    bool isCritical = jsonParser["_is_critical"].GetBool();
    bool isDoubleAttack = jsonParser["_is_double_attack"].GetBool();
    
    //업적
    int nAchiveDamage = std::atoi(MafAes256::Decrypt(UserDefault::getInstance()->getStringForKey(KEY_R_S_MAX_DAMAGE, "0")).c_str());
    if(nAchiveDamage < _nTotalDamage)
    {
        AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_NEW_RAID_SEASON_DAMAGE, _nTotalDamage);
        UserDefault::getInstance()->setStringForKey(KEY_R_S_MAX_DAMAGE, MafAes256::Encrypt(std::to_string((int)_nTotalDamage)));
    }
    
    for ( int i = 0; i < jsonParser["_quest_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_quest_list"][i];
        
        for (auto objQuest : _listInfoQuest)
        {
            if ( objQuest->getIdx() == jsonValue["id"].GetInt() )
            {
                objQuest->setType(jsonValue["task_type"].GetInt());
                objQuest->setCountMax(jsonValue["task_count"].GetInt());
                objQuest->setCountNow(jsonValue["cureent_task_count"].GetInt());
                objQuest->setReceive(jsonValue["is_complete"].GetBool());
            }
        }
    }
    
    // reward
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    
    ItemsManager::getInstance()->addItems(strReward, false);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    if(jsonParser.HasMember("_items"))
    {
        auto listItems = ItemsManager::getInstance()->getConvertArray(strItems);
        for(auto& obj : listItems)
        {
            if( obj->getIdx() == (int)E_ITEMS::NEW_RAID_DRAGON_BALL )
            {
                _nDragonBall = std::atoi(obj->getCount().c_str());
            }
        }
    }
    
    
    listReward = ItemsManager::getInstance()->getConvertArray(strReward);
    
    
    //
    if ( _callbackAttack != nullptr )
    {
        _callbackAttack(nResult, isCritical, isDoubleAttack, listReward);
        _callbackAttack = nullptr;
    }
    
}

void NewRaidManager::requestBuyAttackCount(const std::function<void(bool, int)>& pCallback, int type)
{
    //
    _callbackBuyAttackCount = pCallback;
    
    //
    std::string url = "/new/raid/BuyRaidAttackCount";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_season", _nSeason);
    req->setData("_type", type);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseBuyAttackCount(response,dd);
    };
    req->send(cb);
}

void NewRaidManager::responseBuyAttackCount(cocos2d::network::HttpResponse* response,std::string &data)
{
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
    Vector<InfoItems*> listReward;
    listReward.clear();
    if ( bError == true )
    {
        if ( _callbackBuyAttackCount != nullptr )
        {
            _callbackBuyAttackCount(false, 0);
            _callbackBuyAttackCount = nullptr;
        }
        return;
    }
    
   
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackBuyAttackCount != nullptr )
        {
            _callbackBuyAttackCount(false, nResult);
            _callbackBuyAttackCount = nullptr;
        }
        return;
    }
    
    
    _nAttackCount    = jsonParser["_count"].GetInt();
    _nAdsBuyCount    = jsonParser["_ads_count"].GetInt();
    _nBuyCount       = jsonParser["_buy_count"].GetInt();
    
    // reward
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    ItemsManager::getInstance()->addItems(strReward);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    if(jsonParser.HasMember("_items"))
    {
        auto listItems = ItemsManager::getInstance()->getConvertArray(strItems);
        for(auto& obj : listItems)
        {
            if( obj->getIdx() == (int)E_ITEMS::NEW_RAID_DRAGON_BALL )
            {
                _nDragonBall = std::atoi(obj->getCount().c_str());
            }
        }
    }
    
    
    //
    if ( _callbackBuyAttackCount != nullptr )
    {
        _callbackBuyAttackCount(true, nResult);
        _callbackBuyAttackCount = nullptr;
    }
    
}

void NewRaidManager::requestLevelup(const std::function<void(bool, int, int)>& pCallback, int type)
{
    //
    _callbackLevelup = pCallback;
    _isServerDispatch = true;
    
    bool isCostume = false;
    if(type == 0)
    {
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE, E_COSTUME_IDX::IDX_31))
            isCostume = true;
    }
    
    
    //
    std::string url = "/new/raid/UpdateRaidAbility";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_season", _nSeason);
    req->setData("_type", type);
    req->setData("_costume", (int)isCostume);
    MafResponseNew cb = [&, type](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseLevelup(response,dd, type);
    };
    req->send(cb);
}

void NewRaidManager::responseLevelup(cocos2d::network::HttpResponse* response,std::string &data, int nIdx)
{
    bool bError = false;
    _isServerDispatch = false;
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
    Vector<InfoItems*> listReward;
    listReward.clear();
    if ( bError == true )
    {
        if ( _callbackLevelup != nullptr )
        {
            _callbackLevelup(false, 0, -1);
            _callbackLevelup = nullptr;
        }
        return;
    }
    
   
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackLevelup != nullptr )
        {
            _callbackLevelup(false, nResult, -1);
            _callbackLevelup = nullptr;
        }
        return;
    }
    
    
    _nAbilityPoint   = jsonParser["_ability_point"].GetInt();
    _nLevel          = jsonParser["_level"].GetInt();
    _nExp            = jsonParser["_exp"].GetInt();
    
    //업적
    int nAchiveLv = std::atoi(MafAes256::Decrypt(UserDefault::getInstance()->getStringForKey(KEY_R_S_MAX_LV, "0")).c_str());
    if(nAchiveLv < _nLevel)
    {
        AchievementManager::getInstance()->setAchieveNormalMission(E_NORMAL_ACHIEVE_MISSION::ACHIEVE_NEW_RAID_SEASON_LEVEL, _nLevel);
        UserDefault::getInstance()->setStringForKey(KEY_R_S_MAX_LV, MafAes256::Encrypt(std::to_string(_nLevel)));
    }
    
    for ( int i = 0; i < jsonParser["_ability_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_ability_list"][i];
        
        for (auto obj : _listInfoStat)
        {
            if ( obj->getIdx() == jsonValue["idx"].GetInt() )
            {
                obj->setCurrentLv(jsonValue["level"].GetInt());
            }
        }
    }
    
    // reward
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    ItemsManager::getInstance()->addItems(strReward);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    if(jsonParser.HasMember("_items"))
    {
        auto listItems = ItemsManager::getInstance()->getConvertArray(strItems);
        for(auto& obj : listItems)
        {
            if( obj->getIdx() == (int)E_ITEMS::NEW_RAID_DRAGON_BALL )
            {
                _nDragonBall = std::atoi(obj->getCount().c_str());
            }
        }
    }
    
    if ( _callbackLevelup != nullptr )
    {
        _callbackLevelup(true, nResult, nIdx);
        _callbackLevelup = nullptr;
    }
    
}

void NewRaidManager::requestMissionComplete(const std::function<void(bool, int)>& pCallback, int nIdx)
{
    //
    _callbackMission = pCallback;
    _isServerDispatch = true;
    
    //
    std::string url = "/new/raid/CompleteRaidQuest";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_season", _nSeason);
    req->setData("_index", nIdx);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseMissionComplete(response,dd);
    };
    req->send(cb);
}

void NewRaidManager::responseMissionComplete(cocos2d::network::HttpResponse* response,std::string &data)
{
    bool bError = false;
    _isServerDispatch = false;
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
    Vector<InfoItems*> listReward;
    listReward.clear();
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
    
    for ( int i = 0; i < jsonParser["_quest_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_quest_list"][i];
        
        for (auto objQuest : _listInfoQuest)
        {
            if ( objQuest->getIdx() == jsonValue["id"].GetInt() )
            {
                objQuest->setType(jsonValue["task_type"].GetInt());
                objQuest->setCountMax(jsonValue["task_count"].GetInt());
                objQuest->setCountNow(jsonValue["cureent_task_count"].GetInt());
                objQuest->setReceive(jsonValue["is_complete"].GetBool());
            }
        }
    }
    
    // reward
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    ItemsManager::getInstance()->addItems(strReward);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    
    if(jsonParser.HasMember("_items"))
    {
        auto listItems = ItemsManager::getInstance()->getConvertArray(strItems);
        for(auto& obj : listItems)
        {
            if( obj->getIdx() == (int)E_ITEMS::NEW_RAID_DRAGON_BALL )
            {
                _nDragonBall = std::atoi(obj->getCount().c_str());
            }
        }
    }
    
    
    if ( _callbackMission != nullptr )
    {
        _callbackMission(true, nResult);
        _callbackMission = nullptr;
    }
    
}

void NewRaidManager::requestMissionAllComplete(const std::function<void(bool, int)>& pCallback)
{
    //
    _callbackMission = pCallback;
    
    //
    std::string url = "/new/raid/CompleteAllRaidQuest";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_season", _nSeason);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseMissionAllComplete(response,dd);
    };
    req->send(cb);
}

void NewRaidManager::responseMissionAllComplete(cocos2d::network::HttpResponse* response,std::string &data)
{
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
    Vector<InfoItems*> listReward;
    listReward.clear();
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
    
    for ( int i = 0; i < jsonParser["_quest_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_quest_list"][i];
        
        for (auto objQuest : _listInfoQuest)
        {
            if ( objQuest->getIdx() == jsonValue["id"].GetInt() )
            {
                objQuest->setType(jsonValue["task_type"].GetInt());
                objQuest->setCountMax(jsonValue["task_count"].GetInt());
                objQuest->setCountNow(jsonValue["cureent_task_count"].GetInt());
                objQuest->setReceive(jsonValue["is_complete"].GetBool());
            }
        }
    }
    
    // reward
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    
    ItemsManager::getInstance()->addItems(strReward);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    if(jsonParser.HasMember("_items"))
    {
        auto listItems = ItemsManager::getInstance()->getConvertArray(strItems);
        for(auto& obj : listItems)
        {
            if( obj->getIdx() == (int)E_ITEMS::NEW_RAID_DRAGON_BALL )
            {
                _nDragonBall = std::atoi(obj->getCount().c_str());
            }
        }
        
    }
    
    if ( _callbackMission != nullptr )
    {
        _callbackMission(true, nResult);
        _callbackMission = nullptr;
    }
    
}

void NewRaidManager::requestBuyDragonBall(const std::function<void(bool, int)>& pCallback, bool isAds)
{
    //
    _callbackBuyDragonBall = pCallback;
    
    bool isCostume = false;
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND, E_COSTUME_IDX::IDX_31))
            isCostume = true;
    
    
    //
    std::string url = "/new/raid/BuyDragonBall";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_isADS", isAds);
    req->setData("_costume", (int)isCostume);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseBuyDragonBall(response,dd);
    };
    req->send(cb);
}

void NewRaidManager::responseBuyDragonBall(cocos2d::network::HttpResponse* response,std::string &data)
{
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
    Vector<InfoItems*> listReward;
    listReward.clear();
    if ( bError == true )
    {
        if ( _callbackBuyDragonBall != nullptr )
        {
            _callbackBuyDragonBall(false, 0);
            _callbackBuyDragonBall = nullptr;
        }
        return;
    }
    
   
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackBuyDragonBall != nullptr )
        {
            _callbackBuyDragonBall(false, nResult);
            _callbackBuyDragonBall = nullptr;
        }
        return;
    }
    
    _nAdsDragonBallCount   = jsonParser["_dragon_ball_ads_count"].GetInt();
    
    // reward
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    
    ItemsManager::getInstance()->addItems(strReward);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    if(jsonParser.HasMember("_items"))
    {
        auto listItems = ItemsManager::getInstance()->getConvertArray(strItems);
        for(auto& obj : listItems)
        {
            if( obj->getIdx() == (int)E_ITEMS::NEW_RAID_DRAGON_BALL )
            {
                _nDragonBall = std::atoi(obj->getCount().c_str());
            }
        }
        
    }
    
    if ( _callbackBuyDragonBall != nullptr )
    {
        _callbackBuyDragonBall(true, nResult);
        _callbackBuyDragonBall = nullptr;
    }
    
}

void NewRaidManager::requestTradingMarketInfo(const std::function<void(bool, int)>& callback, bool isRefresh)
{
    _callbackTradingMarketInfo = callback;
    //
    std::string url = "/new/raid/market/info";
    
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_season", _nSeason);
    req->setData("_isNew", (int)isRefresh);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseTradingMarketInfo(response,dd);
    };
    req->send(cb);
}

void NewRaidManager::responseTradingMarketInfo(cocos2d::network::HttpResponse* response,std::string &data)
{
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
    Vector<InfoItems*> listReward;
    listReward.clear();
    if ( bError == true )
    {
        if ( _callbackTradingMarketInfo != nullptr )
        {
            _callbackTradingMarketInfo(false, 0);
            _callbackTradingMarketInfo = nullptr;
        }
        return;
    }
    
   
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackTradingMarketInfo != nullptr )
        {
            _callbackTradingMarketInfo(false, nResult);
            _callbackTradingMarketInfo = nullptr;
        }
        return;
    }
    
    _nMarketRefreshCount   = jsonParser["_dragon_market_count"].GetInt();
    
    clearListTradingMarketReward();
    for(int i = 0; i < jsonParser["_dragon_market_list"].GetArray().Size(); ++i)
    {
        const rapidjson::Value& jsonValue = jsonParser["_dragon_market_list"][i];
        
        int idx = jsonValue["idx"].GetInt();
        int maxBuyCount = jsonValue["limit_count"].GetInt();
        int currentCount = jsonValue["cureent_limit_count"].GetInt();
        int startTime = jsonValue["start_timestamp"].GetInt();
        int endTime = jsonValue["end_timestamp"].GetInt();
        
        auto obj = getRaidTradingMarketReward(idx);
        
        if(obj == nullptr)
            continue;
        
        obj->setActivate(true);
        obj->setMaxBuyCount(maxBuyCount);
        obj->setBuyCount(currentCount);
        obj->setStartTime(startTime);
        obj->setEndTime(endTime);
    }
    
    if(jsonParser.HasMember("_items"))
    {
        std::string strItems         = jsonParser["_items"].GetString();
        auto listItems = ItemsManager::getInstance()->getConvertArray(strItems);
        ItemsManager::getInstance()->setItemsServer(strItems);
        for(auto& obj : listItems)
        {
            if( obj->getIdx() == (int)E_ITEMS::NEW_RAID_DRAGON_BALL )
            {
                _nDragonBall = std::atoi(obj->getCount().c_str());
            }
        }
    }
    
    if ( _callbackTradingMarketInfo != nullptr )
    {
        _callbackTradingMarketInfo(true, nResult);
        _callbackTradingMarketInfo = nullptr;
    }
    
}

void NewRaidManager::requestTradingMarketBuy(const std::function<void(bool, int)>& callback, int nIdx)
{
    _callbackTradingMarketBuy = callback;
    //
    std::string url = "/new/raid/BuyDragonMarket";
    
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_idx", nIdx);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseTradingMarketBuy(response,dd);
    };
    req->send(cb);
}

void NewRaidManager::responseTradingMarketBuy(cocos2d::network::HttpResponse* response,std::string &data)
{
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
    Vector<InfoItems*> listReward;
    listReward.clear();
    if ( bError == true )
    {
        if ( _callbackTradingMarketBuy != nullptr )
        {
            _callbackTradingMarketBuy(false, 0);
            _callbackTradingMarketBuy = nullptr;
        }
        return;
    }
    
   
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackTradingMarketBuy != nullptr )
        {
            _callbackTradingMarketBuy(false, nResult);
            _callbackTradingMarketBuy = nullptr;
        }
        return;
    }
    
    clearListTradingMarketReward();
    for(int i = 0; i < jsonParser["_dragon_market_list"].GetArray().Size(); ++i)
    {
        const rapidjson::Value& jsonValue = jsonParser["_dragon_market_list"][i];
        
        int idx = jsonValue["idx"].GetInt();
        int maxBuyCount = jsonValue["limit_count"].GetInt();
        int currentCount = jsonValue["cureent_limit_count"].GetInt();
        int startTime = jsonValue["start_timestamp"].GetInt();
        int endTime = jsonValue["end_timestamp"].GetInt();
        
        auto obj = getRaidTradingMarketReward(idx);
        
        if(obj == nullptr)
            continue;
        
        obj->setActivate(true);
        obj->setMaxBuyCount(maxBuyCount);
        obj->setBuyCount(currentCount);
        obj->setStartTime(startTime);
        obj->setEndTime(endTime);
    }
    
    // reward
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    
    ItemsManager::getInstance()->addItems(strReward);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    if(jsonParser.HasMember("_items"))
    {
        auto listItems = ItemsManager::getInstance()->getConvertArray(strItems);
        for(auto& obj : listItems)
        {
            if( obj->getIdx() == (int)E_ITEMS::NEW_RAID_DRAGON_BALL )
            {
                _nDragonBall = std::atoi(obj->getCount().c_str());
            }
        }
        
    }
    
    if ( _callbackTradingMarketBuy != nullptr )
    {
        _callbackTradingMarketBuy(true, nResult);
        _callbackTradingMarketBuy = nullptr;
    }
    
}
