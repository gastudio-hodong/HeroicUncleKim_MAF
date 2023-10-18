//
//  AdventureRelicManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/04.
//

#include "AdventureRelicManager.h"

#include "GameObject/InfoSetting.h"
#include "ManagerGame/UserInfoManager.h"

#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/InfiniteArtifactManager.h"

USING_NS_CC;

AdventureRelicManager::AdventureRelicManager(void):
_callbackInfoCore(nullptr)
,_callbackInfoRelic(nullptr)
,_callbackCraftRelic(nullptr)
,_callbackInfoRelicCollect(nullptr)
,_callbackRewardRelicCollect(nullptr)
,_callbackRewardRelicLevelUp(nullptr)
,_callbackRewardRelicGrind(nullptr)
{
    _listInfoRelic.clear();
    _listInfoCore.clear();
    _listInfoCoreLevelOrigin.clear();
    _listInfoCollect.clear();
}

AdventureRelicManager::~AdventureRelicManager(void)
{
    _listInfoRelic.clear();
    _listInfoCore.clear();
    _listInfoCoreLevelOrigin.clear();
    _listInfoCollect.clear();
}

bool AdventureRelicManager::init()
{
    return true;
}
#pragma mark - info get

const Vector<InfoAdventureRelic*>& AdventureRelicManager::getListInfoRelic() const
{
    return _listInfoRelic;
}
const Vector<InfoAdventureRelic*> AdventureRelicManager::getListInfoRelicFromArea(const int nArea) const
{
    Vector<InfoAdventureRelic*> result;
    result.clear();
    
    for(const auto obj : _listInfoRelic)
    {
        if(obj->getArea() == nArea)
            result.pushBack(obj);
    }
    
    return result;
}
InfoAdventureRelic* const AdventureRelicManager::getInfoRelic(const int nIdx) const
{
    InfoAdventureRelic* result = nullptr;
    for(const auto obj : _listInfoRelic)
    {
        if(obj->getIdx() == nIdx)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

const Vector<InfoAdventureCore*>& AdventureRelicManager::getListInfoCore() const
{
    return _listInfoCore;
}

InfoAdventureCore* const AdventureRelicManager::getInfoCore(const int nIdx) const
{
    InfoAdventureCore* result = nullptr;
    for(const auto obj : _listInfoCore)
    {
        if(obj->getIdx() == nIdx)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
InfoBongContentLevel* const AdventureRelicManager::getInfoCoreLevelOrigin(const int nLv) const
{
    InfoBongContentLevel* result = nullptr;
    for(const auto obj : _listInfoCoreLevelOrigin)
    {
        if(obj->getLevel() == nLv)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
InfoBongContentLevel* const AdventureRelicManager::getInfoRelicLevel(const int nLv) const
{
    InfoBongContentLevel* result = nullptr;
    for(const auto obj : _listInfoRelicLevel)
    {
        if(obj->getLevel() == nLv)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
const Vector<InfoAdventureRelicCollect*> AdventureRelicManager::getListCollect() const
{
    return _listInfoCollect;
}

InfoAdventureRelicCollect* const AdventureRelicManager::getInfoCollect(const int nIdx) const
{
    InfoAdventureRelicCollect* result = nullptr;
    for(const auto obj : _listInfoCollect)
    {
        if(obj->getIdx() == nIdx)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

const int AdventureRelicManager::getCoreMaxLv()
{
    return _listInfoCoreLevelOrigin.size();
}
const int AdventureRelicManager::getRelicMaxLv()
{
    return _listInfoRelicLevel.size();
}
const int AdventureRelicManager::getCraftPrice(const int nIdx, const int nCount)
{
    int result = 0;
    
    int nDefaultPrice = 0;
    std::string strSetting = "relic_cost";
    
    if(nIdx == 6)
        strSetting = "relic_cost_bonus";
    else if(nIdx == 7 )
        strSetting = "relic_cost_friend";
    
    InfoSetting* objPrice = AdventureManager::getInstance()->getSetting(strSetting);
    if(objPrice != nullptr)
        nDefaultPrice = objPrice->getValueToInt();
    
    result = nDefaultPrice * nCount;
    
    return result;
}
const int AdventureRelicManager::getRelicLevelUpPrice(const int nIdx, const int nCount)
{
    int result = 0;
    
    auto objRelic = getInfoRelic(nIdx);
    if(objRelic == nullptr)
        return result;
    
    for(int i = 0; i < nCount; ++i)
    {
        auto obj = getInfoRelicLevel(objRelic->getCurrentLv() + i);
        if(obj == nullptr)
            continue;
        
        result += obj->getTotalEXP();
    }
    
    return result;
}
const int AdventureRelicManager::getRelicLevelupMax(const int nIdx)
{
    int result = 0;
    
    auto objRelic = getInfoRelic(nIdx);
    if(objRelic == nullptr)
        return result;
    
    for(int i = objRelic->getCurrentLv(); i < getRelicMaxLv(); ++i)
    {
        if(objRelic->getCount() < getRelicLevelUpPrice(nIdx, result+1))
           break;
        
        result++;
    }
    
    return result;
}
const double AdventureRelicManager::getRelicCollectEffect(const E_ADVENTURE_PROPERTY_TYPE eType)
{
    double result = 0;
    
    for(auto obj : _listInfoCollect)
    {
        if(obj->getType() == eType && obj->isActive())
            result += obj->getAbilityEffect();
    }
    
    return result;
}
const int AdventureRelicManager::getTotalRelicLvFromArea(const int nArea) const
{
    int result = 0;
    
    for(const auto obj : _listInfoRelic)
    {
        if(!obj->isActive() || obj->getArea() != nArea)
            continue;
        
        result += obj->getCurrentLv();
    }
    
    return result;
}
const int AdventureRelicManager::getTotalRelicLv() const
{
    int result = 0;
    
    for(const auto obj : _listInfoRelic)
    {
        if(!obj->isActive())
            continue;
        
        result += obj->getCurrentLv();
    }
    
    return result;
}
const double AdventureRelicManager::getRelicTotalEffectCoefficient(const E_ADVENTURE_PROPERTY_TYPE eType) const
{
    double result = 1;
    switch(eType)
    {
        case E_ADVENTURE_PROPERTY_TYPE::AREA_GRASS_RELIC_LEVEL_KEY:
        case E_ADVENTURE_PROPERTY_TYPE::AREA_GRASS_RELIC_LEVEL_GOLD:
        case E_ADVENTURE_PROPERTY_TYPE::AREA_GRASS_RELIC_LEVEL_ATK:
        {
            result = getTotalRelicLvFromArea(1);
        }break;
        case E_ADVENTURE_PROPERTY_TYPE::AREA_DESERT_RELIC_LEVEL_KEY:
        case E_ADVENTURE_PROPERTY_TYPE::AREA_DESERT_RELIC_LEVEL_GOLD:
        case E_ADVENTURE_PROPERTY_TYPE::AREA_DESERT_RELIC_LEVEL_ATK:
        {
            result = getTotalRelicLvFromArea(2);
        }break;
        case E_ADVENTURE_PROPERTY_TYPE::AREA_SNOW_RELIC_LEVEL_KEY:
        case E_ADVENTURE_PROPERTY_TYPE::AREA_SNOW_RELIC_LEVEL_GOLD:
        case E_ADVENTURE_PROPERTY_TYPE::AREA_SNOW_RELIC_LEVEL_ATK:
        {
            result = getTotalRelicLvFromArea(3);
        }break;
        case E_ADVENTURE_PROPERTY_TYPE::AREA_VOLCANO_RELIC_LEVEL_KEY:
        case E_ADVENTURE_PROPERTY_TYPE::AREA_VOLCANO_RELIC_LEVEL_GOLD:
        case E_ADVENTURE_PROPERTY_TYPE::AREA_VOLCANO_RELIC_LEVEL_ATK:
        {
            result = getTotalRelicLvFromArea(4);
        }break;
        case E_ADVENTURE_PROPERTY_TYPE::ALL_RELIC_LEVEL_DUNGEON_ATK:
        case E_ADVENTURE_PROPERTY_TYPE::ALL_RELIC_LEVEL_CRITICAL_ATK:
        case E_ADVENTURE_PROPERTY_TYPE::ALL_RELIC_LEVEL_SPIRIT_ATK:
        case E_ADVENTURE_PROPERTY_TYPE::ALL_RELIC_LEVEL_SPIRIT_COLLECT_ATK:
        {
            result = getTotalRelicLv();
        }break;
        case E_ADVENTURE_PROPERTY_TYPE::ARTIFACT_GRADE_BONUS_ATK:
        {
            result = UserInfoManager::getInstance()->getArtifactCurrentGrade(E_ARTIFACT_TYPE::ARTI_TYPE_ATK);
            result += InfiniteArtifactManager::getInstance()->getCurGrade();
        }break;
        case E_ADVENTURE_PROPERTY_TYPE::ARTIFACT_GRADE_BONUS_CRI:
        {
            result = UserInfoManager::getInstance()->getArtifactCurrentGrade(E_ARTIFACT_TYPE::ARTI_TYPE_CRI);
            result += InfiniteArtifactManager::getInstance()->getCurGrade();
        }break;
        case E_ADVENTURE_PROPERTY_TYPE::ARTIFACT_GRADE_BONUS_QUEST_GOLD:
        {
            result = UserInfoManager::getInstance()->getArtifactCurrentGrade(E_ARTIFACT_TYPE::ARTI_TYPE_QUEST);
            result += InfiniteArtifactManager::getInstance()->getCurGrade();
        }break;
        case E_ADVENTURE_PROPERTY_TYPE::ARTIFACT_GRADE_BONUS_KILL_GOLD:
        {
            result = UserInfoManager::getInstance()->getArtifactCurrentGrade(E_ARTIFACT_TYPE::ARTI_TYPE_KILL);
            result += InfiniteArtifactManager::getInstance()->getCurGrade();
        }break;
        case E_ADVENTURE_PROPERTY_TYPE::COIN_BANK_GRADE_BONUS_ATK:
        {
            result = UserInfoManager::getInstance()->getArtifactCurrentGrade(E_ARTIFACT_TYPE::ARTI_TYPE_DISCOUNT);
        }break;
        case E_ADVENTURE_PROPERTY_TYPE::ADVENTURE_LEVEL_ATK:
        case E_ADVENTURE_PROPERTY_TYPE::ADVENTURE_LEVEL_CRI:
        case E_ADVENTURE_PROPERTY_TYPE::ADVENTURE_LEVEL_SPIRIT_ATK:
        case E_ADVENTURE_PROPERTY_TYPE::ADVENTURE_LEVEL_SPIRIT_COLLECT_ATK:
        {
            result = AdventureManager::getInstance()->getLevel();
        }break;
        case E_ADVENTURE_PROPERTY_TYPE::CORE_LEVEL_GRASS_ATK:
        {
            auto objCore = getInfoCore(1);
            if(objCore != nullptr && objCore->isActive())
                result = objCore->getCurrentLv();
        }break;
        case E_ADVENTURE_PROPERTY_TYPE::CORE_LEVEL_DESERT_ATK:
        {
            auto objCore = getInfoCore(2);
            if(objCore != nullptr && objCore->isActive())
                result = objCore->getCurrentLv();
        }break;
        case E_ADVENTURE_PROPERTY_TYPE::CORE_LEVEL_SNOW_ATK:
        {
            auto objCore = getInfoCore(3);
            if(objCore != nullptr && objCore->isActive())
                result = objCore->getCurrentLv();
        }break;
        case E_ADVENTURE_PROPERTY_TYPE::CORE_LEVEL_VOLCANO_ATK:
        {
            auto objCore = getInfoCore(4);
            if(objCore != nullptr && objCore->isActive())
                result = objCore->getCurrentLv();
        }break;
        case E_ADVENTURE_PROPERTY_TYPE::FURNITURE_LEVEL_ATK:
        case E_ADVENTURE_PROPERTY_TYPE::FURNITURE_LEVEL_CRI:
        case E_ADVENTURE_PROPERTY_TYPE::FURNITURE_LEVEL_SPIRIT_ATK:
        case E_ADVENTURE_PROPERTY_TYPE::FURNITURE_LEVEL_SPIRIT_COLLECT_ATK:
        {
            result = AdventureManager::getInstance()->getFurnitureTotalLv();
        }break;
        case E_ADVENTURE_PROPERTY_TYPE::ADVENTURE_EXPLORING_ATK:
        case E_ADVENTURE_PROPERTY_TYPE::ADVENTURE_EXPLORING_CRI:
        case E_ADVENTURE_PROPERTY_TYPE::ADVENTURE_EXPLORING_SPIRIT_ATK:
        case E_ADVENTURE_PROPERTY_TYPE::ADVENTURE_EXPLORING_SPIRIT_COLLECT_ATK:
        {
            result = AdventureManager::getInstance()->getCurrentRunningParty();
        }break;
        default: result = 1; break;
    }
    
    return result;
}

const double AdventureRelicManager::getRelicTotalEffect(const E_ADVENTURE_PROPERTY_TYPE eType) const
{
    double result = 0;
    
    double dCoef = getRelicTotalEffectCoefficient(eType);
    
    for(auto obj : _listInfoRelic)
    {
        if(!obj->isActive() || obj->getType() != eType)
            continue;
        
        result += obj->getAbilityEffect() * dCoef;
    }
    
    return result;
}

const double AdventureRelicManager::getRelicEffectBuffCalculator(const E_ADVENTURE_PROPERTY_CALCULATE_TYPE eType) const
{
    double result = 0;
    
    switch(eType)
    {
        case E_ADVENTURE_PROPERTY_CALCULATE_TYPE::DUNGEON_ATK:
        {
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::DUNGEON_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::ARTIFACT_GRADE_BONUS_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::COIN_BANK_GRADE_BONUS_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::AREA_GRASS_RELIC_LEVEL_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::AREA_DESERT_RELIC_LEVEL_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::AREA_SNOW_RELIC_LEVEL_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::AREA_VOLCANO_RELIC_LEVEL_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::ALL_RELIC_LEVEL_DUNGEON_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::ADVENTURE_LEVEL_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::CORE_LEVEL_GRASS_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::CORE_LEVEL_DESERT_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::CORE_LEVEL_SNOW_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::CORE_LEVEL_VOLCANO_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::FURNITURE_LEVEL_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::ADVENTURE_EXPLORING_ATK);
        }break;
        case E_ADVENTURE_PROPERTY_CALCULATE_TYPE::DUNGEON_CRI:
        {
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::CRITICAL_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::ARTIFACT_GRADE_BONUS_CRI);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::ALL_RELIC_LEVEL_CRITICAL_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::ADVENTURE_PARTY_LEVEL_BONUS_CRI);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::ADVENTURE_LEVEL_CRI);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::FURNITURE_LEVEL_CRI);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::ADVENTURE_EXPLORING_CRI);
        }break;
        case E_ADVENTURE_PROPERTY_CALCULATE_TYPE::DUNGEON_GOLD:
        {
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::AREA_GRASS_RELIC_LEVEL_GOLD);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::AREA_DESERT_RELIC_LEVEL_GOLD);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::AREA_SNOW_RELIC_LEVEL_GOLD);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::AREA_VOLCANO_RELIC_LEVEL_GOLD);
        }break;
        case E_ADVENTURE_PROPERTY_CALCULATE_TYPE::SPIRIT_ATK:
        {
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::SPIRIT_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::ALL_RELIC_LEVEL_SPIRIT_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::ADVENTURE_LEVEL_SPIRIT_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::FURNITURE_LEVEL_SPIRIT_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::ADVENTURE_EXPLORING_SPIRIT_ATK);
        }break;
        case E_ADVENTURE_PROPERTY_CALCULATE_TYPE::SPIRIT_COLLECT_ATK:
        {
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::SPIRIT_COLLECT_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::ALL_RELIC_LEVEL_SPIRIT_COLLECT_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::ADVENTURE_LEVEL_SPIRIT_COLLECT_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::FURNITURE_LEVEL_SPIRIT_COLLECT_ATK);
            result += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::ADVENTURE_EXPLORING_SPIRIT_COLLECT_ATK);
        }break;
    }
    
    return result;
}

const bool AdventureRelicManager::isRelicReddotCondition() const
{
    bool result = false;
    
    for(auto obj : _listInfoRelic)
    {
        if(obj->isReddotCondition())
        {
            result = true;
            break;
        }
    }
    
    return result;
}
const bool AdventureRelicManager::isRelicCollectReddotCondition() const
{
    bool result = false;
    
    for(auto obj : _listInfoCollect)
    {
        if(!obj->isActive() && obj->isReward())
        {
            result = true;
            break;
        }
    }
    
    return result;
}
const bool AdventureRelicManager::isRelicReddotConditionFromArea(const int nArea) const
{
    bool result = false;
    
    auto list = getListInfoRelicFromArea(nArea);
    
    for(auto obj : list)
    {
        if(obj->isReddotCondition())
        {
            result = true;
            break;
        }
    }
    
    return result;
}

const int AdventureRelicManager::getCountActiveAreaCore() const
{
    int result = 0;
    
    for(auto obj : _listInfoCore)
    {
        if(obj->getIdx() == 5 && obj->getIdx() == 6 && obj->getIdx() == 7)
            continue;
        
        if(obj->isActive())
            result++;
    }
    
    return result;
}
const bool AdventureRelicManager::isActivateRelicEffect(const E_ADVENTURE_PROPERTY_TYPE eType) const
{
    bool result = false;
    
    for(const auto obj : _listInfoRelic)
    {
        if(obj->getType() == eType && obj->isActive())
        {
            result = true;
            break;
        }
    }
    
    return result;
}
#pragma mark - Load Table Data
void AdventureRelicManager::setLoadAll()
{
    setInitCore();
    setLoadAdventureRelic();
    setLoadCoreLevelOrigin();
    setLoadRelicLevel();
    setLoadRelicCollect();
}
void AdventureRelicManager::setLoadAdventureRelic()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::ADVENTURE_RELIC);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoRelic.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        const int nIdx = jsonValue["idx"].GetInt();
        const int nArea = jsonValue["area"].GetInt();
        const int nGrade = jsonValue["grade"].GetInt();
        const E_ADVENTURE_PROPERTY_TYPE eType = (E_ADVENTURE_PROPERTY_TYPE)jsonValue["ability_type"].GetInt();
        const double nValue = jsonValue["ability_value"].GetDouble();
        const double nIncrease = jsonValue["ability_increase"].GetDouble();
        const std::string strGrindReward = jsonValue["grind_reward"].GetString();
        
        const auto list = ItemsManager::getInstance()->getConvertArray(strGrindReward);
        
        InfoAdventureRelic* obj = InfoAdventureRelic::create();
        obj->setIdx(nIdx);
        obj->setArea(nArea);
        obj->setGrade(nGrade);
        obj->setType(eType);
        obj->setAbilityEffectDefault(nValue);
        obj->setAbilityEffectIncrease(nIncrease);
        obj->setListGrindReward(list);
        
        _listInfoRelic.pushBack(obj);
    }
}

void AdventureRelicManager::setLoadCoreLevelOrigin()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::ADVENTURE_CORE_LEVEL);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoCoreLevelOrigin.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        auto obj = InfoBongContentLevel::create();
        obj->setLevel(jsonValue["craft_lv"].GetInt());
        obj->setTotalEXP(jsonValue["require_cnt"].GetFloat());
        
        _listInfoCoreLevelOrigin.pushBack(obj);
    }
}

void AdventureRelicManager::setLoadRelicLevel()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::ADVENTURE_RELIC_LEVEL);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoRelicLevel.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        auto obj = InfoBongContentLevel::create();
        obj->setLevel(jsonValue["lv"].GetInt());
        obj->setTotalEXP(jsonValue["exp"].GetFloat());
        
        _listInfoRelicLevel.pushBack(obj);
    }
}


void AdventureRelicManager::setLoadRelicCollect()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::ADVENTURE_RELIC_COLLECT);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoCollect.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        int nIdx = jsonValue["idx"].GetInt();
        std::string strCondition = jsonValue["condition"].GetString();
        std::string strProperty = jsonValue["property"].GetString();
        std::string strReward = jsonValue["reward"].GetString();
        
        std::vector<std::pair<int,int>> listCondition;
        listCondition.clear();
        
        auto obj = InfoAdventureRelicCollect::create();
        
        rapidjson::Document jsonParserTemp;
        jsonParserTemp.ParseInsitu<0>((char*)strCondition.c_str());
        if(!jsonParserTemp.HasParseError())
        {
            for ( int j = 0; j < jsonParserTemp.Size(); j++ )
            {
                const rapidjson::Value& jsonConditionValue = jsonParserTemp[j];
                
                int nConditionIdx = jsonConditionValue["idx"].GetInt();
                int nLv = jsonConditionValue["lv"].GetInt();
                listCondition.push_back(std::make_pair(nConditionIdx, nLv));
            }
        }
        
        jsonParserTemp.ParseInsitu<0>((char*)strProperty.c_str());
        if(!jsonParserTemp.HasParseError())
        {
            for ( int j = 0; j < jsonParserTemp.Size(); j++ )
            {
                const rapidjson::Value& jsonPropertyValue = jsonParserTemp[j];
                
                int nType = jsonPropertyValue["type"].GetInt();
                double nValue = jsonPropertyValue["value"].GetDouble();
                
                obj->setType((E_ADVENTURE_PROPERTY_TYPE) nType);
                obj->setAbilityEffect(nValue);
            }
        }
        auto listReward = ItemsManager::getInstance()->getConvertArray(strReward);
        
        obj->setIdx(nIdx);
        obj->setListCondition(listCondition);
        obj->setListReward(listReward);
        
        _listInfoCollect.pushBack(obj);
    }
}

#pragma mark - network
void AdventureRelicManager::requestInfoCore(const std::function<void(bool, int)>& callback)
{
    _callbackInfoCore = callback;
    //
    std::string url = "/adventure/core/list/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseInfoCore(response,dd);
    };
    req->send(cb);
}

void AdventureRelicManager::responseInfoCore(cocos2d::network::HttpResponse* const response, const std::string &data)
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
        if ( _callbackInfoCore != nullptr )
        {
            _callbackInfoCore(false, 0);
        }
        _callbackInfoCore = nullptr;
        return;
    }
    
    
    for ( int i = 0; i < jsonParser["_core_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_core_list"][i];
        
        const int nIdx = jsonValue["idx"].GetInt();
        const int nLevel = jsonValue["level"].GetInt();
        const int nExp = jsonValue["exp"].GetInt();
        const int nCount = jsonValue["count"].GetInt();
        
        auto objCore = getInfoCore(nIdx);
        
        if(objCore == nullptr)
        {
            objCore = InfoAdventureCore::create();
            objCore->setIdx(nIdx);
            _listInfoCore.pushBack(objCore);
        }
        objCore->setIsActice(true);
        objCore->setExp(nExp);
        objCore->setCurrentLv(nLevel);
        objCore->setCount(nCount);
        
    }
    
    
    if ( _callbackInfoCore != nullptr )
    {
        _callbackInfoCore(true, 1);
    }
    
    _callbackInfoCore = nullptr;
}

void AdventureRelicManager::requestInfoRelic(const std::function<void(bool, int)>& callback)
{
    _callbackInfoRelic = callback;
    //
    std::string url = "/adventure/relic/list/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseInfoRelic(response,dd);
    };
    req->send(cb);
}

void AdventureRelicManager::responseInfoRelic(cocos2d::network::HttpResponse* const response, const std::string &data)
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
        if ( _callbackInfoRelic != nullptr )
        {
            _callbackInfoRelic(false, 0);
        }
        _callbackInfoRelic = nullptr;
        return;
    }
    
    
    for ( int i = 0; i < jsonParser["_relic_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_relic_list"][i];
        
        const int nIdx = jsonValue["idx"].GetInt();
        const int nLevel = jsonValue["level"].GetInt();
        const int nCount = jsonValue["count"].GetInt();
        
        auto objRelic = getInfoRelic(nIdx);
        
        if(objRelic == nullptr)
        {
            objRelic = InfoAdventureRelic::create();
            objRelic->setIdx(nIdx);
            _listInfoRelic.pushBack(objRelic);
        }
        objRelic->setIsActive(true);
        objRelic->setCurrentLv(nLevel);
        objRelic->setCount(nCount);
        
    }
    
    
    if ( _callbackInfoRelic != nullptr )
    {
        _callbackInfoRelic(true, 1);
    }
    
    _callbackInfoRelic = nullptr;
}

void AdventureRelicManager::requestCraftRelic(const std::function<void(bool, int, Vector<InfoItems*>)>& callback,  int nCoreIdx, int nCount)
{
    _callbackCraftRelic = callback;
    
    //
    const std::string url = "/adventure/MakeAdventureRelic";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_idx", nCoreIdx);
    req->setData("_count", nCount);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseCraftRelic(response,dd);
    };
    req->send(cb);
}

void AdventureRelicManager::responseCraftRelic(cocos2d::network::HttpResponse* const response, const std::string &data)
{
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
       
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    Vector<InfoItems*> listReward;
    listReward.clear();
    
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
       
    if ( bError == true )
    {
        if ( _callbackCraftRelic != nullptr )
        {
            _callbackCraftRelic(false, 0, listReward);
        }
        _callbackCraftRelic = nullptr;
        return;
    }
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackCraftRelic != nullptr )
        {
            _callbackCraftRelic(false, nResult, listReward);
        }
        _callbackCraftRelic = nullptr;
        return;
    }
    
    
    for ( int i = 0; i < jsonParser["_core_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_core_list"][i];
        
        const int nIdx = jsonValue["idx"].GetInt();
        const int nLevel = jsonValue["level"].GetInt();
        const int nExp = jsonValue["exp"].GetInt();
        const int nCount = jsonValue["count"].GetInt();
        
        auto objCore = getInfoCore(nIdx);
        
        if(objCore == nullptr)
        {
            objCore = InfoAdventureCore::create();
            objCore->setIdx(nIdx);
            _listInfoCore.pushBack(objCore);
        }
        objCore->setIsActice(true);
        objCore->setExp(nExp);
        objCore->setCurrentLv(nLevel);
        objCore->setCount(nCount);
    }
    
    for ( int i = 0; i < jsonParser["_relic_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_relic_list"][i];
        
        const int nIdx = jsonValue["idx"].GetInt();
        const int nLevel = jsonValue["level"].GetInt();
        const int nCount = jsonValue["count"].GetInt();
        
        auto objRelic = getInfoRelic(nIdx);
        
        if(objRelic == nullptr)
        {
            objRelic = InfoAdventureRelic::create();
            objRelic->setIdx(nIdx);
            _listInfoRelic.pushBack(objRelic);
        }
        if(!objRelic->isActive())
        {
            objRelic->setIsActive(true);
           if(objRelic->getType() == E_ADVENTURE_PROPERTY_TYPE::ARTIFACT_GRADE_BONUS_KILL_GOLD ||
             objRelic->getType() == E_ADVENTURE_PROPERTY_TYPE::ARTIFACT_GRADE_BONUS_QUEST_GOLD ||
             objRelic->getType() == E_ADVENTURE_PROPERTY_TYPE::AREA_GRASS_RELIC_LEVEL_GOLD ||
             objRelic->getType() == E_ADVENTURE_PROPERTY_TYPE::AREA_DESERT_RELIC_LEVEL_GOLD ||
             objRelic->getType() == E_ADVENTURE_PROPERTY_TYPE::AREA_SNOW_RELIC_LEVEL_GOLD ||
             objRelic->getType() == E_ADVENTURE_PROPERTY_TYPE::AREA_VOLCANO_RELIC_LEVEL_GOLD)
             UserInfoManager::getInstance()->getSecondEarnGoldNormal(true);
        }
        objRelic->setCurrentLv(nLevel);
        objRelic->setCount(nCount);
        
    }
    
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    listReward = ItemsManager::getInstance()->getConvertArray(strReward);
    ItemsManager::getInstance()->addItems(strReward, false);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    if ( _callbackCraftRelic != nullptr )
    {
        _callbackCraftRelic(true, 1, listReward);
    }
    
    _callbackCraftRelic = nullptr;
    AdventureManager::getInstance()->requestMissionInfo(nullptr);
}

void AdventureRelicManager::requestRewardRelicCollect(const std::function<void(bool, int)>& callback,  int nIdx)
{
    _callbackRewardRelicCollect = callback;
    
    //
    const std::string url = "/adventure/RewardAdventureRelicCollection";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_idx", nIdx);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseRewardRelicCollect(response,dd);
    };
    req->send(cb);
}

void AdventureRelicManager::responseRewardRelicCollect(cocos2d::network::HttpResponse* const response, const std::string &data)
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
        if ( _callbackRewardRelicCollect != nullptr )
        {
            _callbackRewardRelicCollect(false, 0);
        }
        _callbackRewardRelicCollect = nullptr;
        return;
    }
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackRewardRelicCollect != nullptr )
        {
            _callbackRewardRelicCollect(false, nResult);
        }
        _callbackRewardRelicCollect = nullptr;
        return;
    }
    
    
    for ( int i = 0; i < jsonParser["_relic_collection_list"].Size(); i++ )
    {
        const int nIdx = jsonParser["_relic_collection_list"][i].GetInt();
        
        auto objCollect = getInfoCollect(nIdx);
        
        if(objCollect == nullptr)
            continue;
        
        objCollect->setIsActive(true);
    }
    
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    ItemsManager::getInstance()->addItems(strReward);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    if ( _callbackRewardRelicCollect != nullptr )
    {
        _callbackRewardRelicCollect(true, 1);
    }
    
    _callbackRewardRelicCollect = nullptr;
}

void AdventureRelicManager::requestInfoRelicCollect(const std::function<void(bool, int)>& callback)
{
    _callbackInfoRelicCollect = callback;
    //
    std::string url = "/adventure/relic/collection/list/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseInfoRelicCollect(response,dd);
    };
    req->send(cb);
}

void AdventureRelicManager::responseInfoRelicCollect(cocos2d::network::HttpResponse* const response, const std::string &data)
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
        if ( _callbackInfoRelicCollect != nullptr )
        {
            _callbackInfoRelicCollect(false, 0);
        }
        _callbackInfoRelicCollect = nullptr;
        return;
    }
    
    
    for ( int i = 0; i < jsonParser["_relic_collection_list"].Size(); i++ )
    {
        const int nIdx = jsonParser["_relic_collection_list"][i].GetInt();
        
        auto objCollect = getInfoCollect(nIdx);
        
        if(objCollect == nullptr)
            continue;
        
        objCollect->setIsActive(true);
    }
    
    
    if ( _callbackInfoRelicCollect != nullptr )
    {
        _callbackInfoRelicCollect(true, 1);
    }
    
    _callbackInfoRelicCollect = nullptr;
}
void AdventureRelicManager::requestRelicLevelUp(const std::function<void(bool, int)>& callback, int nIdx, int nLv)
{
    _callbackRewardRelicLevelUp = callback;
    
    //
    const std::string url = "/adventure/UpdateAdventureRelicLevel";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_idx", nIdx);
    req->setData("_count", nLv);
    MafResponseNew cb = [&, nIdx](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseRelicLevelUp(response, dd, nIdx);
    };
    req->send(cb);
}

void AdventureRelicManager::responseRelicLevelUp(cocos2d::network::HttpResponse* const response, const std::string &data, const int nIdx)
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
        if ( _callbackRewardRelicLevelUp != nullptr )
        {
            _callbackRewardRelicLevelUp(false, 0);
        }
        _callbackRewardRelicLevelUp = nullptr;
        return;
    }
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackRewardRelicLevelUp != nullptr )
        {
            _callbackRewardRelicLevelUp(false, nResult);
        }
        _callbackRewardRelicLevelUp = nullptr;
        return;
    }
    
    auto objRelic = getInfoRelic(nIdx);
    if(objRelic != nullptr &&
       (objRelic->getType() == E_ADVENTURE_PROPERTY_TYPE::ARTIFACT_GRADE_BONUS_KILL_GOLD ||
        objRelic->getType() == E_ADVENTURE_PROPERTY_TYPE::ARTIFACT_GRADE_BONUS_QUEST_GOLD ||
        objRelic->getType() == E_ADVENTURE_PROPERTY_TYPE::AREA_GRASS_RELIC_LEVEL_GOLD ||
        objRelic->getType() == E_ADVENTURE_PROPERTY_TYPE::AREA_DESERT_RELIC_LEVEL_GOLD ||
        objRelic->getType() == E_ADVENTURE_PROPERTY_TYPE::AREA_SNOW_RELIC_LEVEL_GOLD ||
        objRelic->getType() == E_ADVENTURE_PROPERTY_TYPE::AREA_VOLCANO_RELIC_LEVEL_GOLD))
        UserInfoManager::getInstance()->getSecondEarnGoldNormal(true);
    
    
    for ( int i = 0; i < jsonParser["_relic_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_relic_list"][i];
        
        const int nIdx = jsonValue["idx"].GetInt();
        const int nLevel = jsonValue["level"].GetInt();
        const int nCount = jsonValue["count"].GetInt();
        
        auto objRelic = getInfoRelic(nIdx);
        
        if(objRelic == nullptr)
        {
            objRelic = InfoAdventureRelic::create();
            objRelic->setIdx(nIdx);
            _listInfoRelic.pushBack(objRelic);
        }
        objRelic->setIsActive(true);
        objRelic->setCurrentLv(nLevel);
        objRelic->setCount(nCount);
        
    }
    
    if ( _callbackRewardRelicLevelUp != nullptr )
    {
        _callbackRewardRelicLevelUp(true, 1);
    }
    
    _callbackRewardRelicLevelUp = nullptr;
}

void AdventureRelicManager::requestRelicGrind(const std::function<void(bool, int)>& callback, int nIdx, int nCount)
{
    _callbackRewardRelicGrind = callback;
    
    //
    const std::string url = "/adventure/DecompositionAdventureRelic";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_idx", nIdx);
    req->setData("_count", nCount);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseRelicGrind(response,dd);
    };
    req->send(cb);
}

void AdventureRelicManager::responseRelicGrind(cocos2d::network::HttpResponse* const response, const std::string &data)
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
        if ( _callbackRewardRelicGrind != nullptr )
        {
            _callbackRewardRelicGrind(false, 0);
        }
        _callbackRewardRelicGrind = nullptr;
        return;
    }
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackRewardRelicGrind != nullptr )
        {
            _callbackRewardRelicGrind(false, nResult);
        }
        _callbackRewardRelicGrind = nullptr;
        return;
    }
    
    
    for ( int i = 0; i < jsonParser["_relic_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_relic_list"][i];
        
        const int nIdx = jsonValue["idx"].GetInt();
        const int nLevel = jsonValue["level"].GetInt();
        const int nCount = jsonValue["count"].GetInt();
        
        auto objRelic = getInfoRelic(nIdx);
        
        if(objRelic == nullptr)
        {
            objRelic = InfoAdventureRelic::create();
            objRelic->setIdx(nIdx);
            _listInfoRelic.pushBack(objRelic);
        }
        objRelic->setIsActive(true);
        objRelic->setCurrentLv(nLevel);
        objRelic->setCount(nCount);
        
    }
    
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    ItemsManager::getInstance()->addItems(strReward);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    if(jsonParser.HasMember("_items"))
    {
        auto listItems = ItemsManager::getInstance()->getConvertArray(strItems);
        for(auto& obj : listItems)
        {
            if( obj->getIdx() == (int)E_ITEMS::ADVENTURE_CORE )
            {
                const int nCount = std::atoi(obj->getCount().c_str());
                const int nTypeParams = obj->getTypeParams();
                auto objCore = getInfoCore(nTypeParams);
                if(objCore != nullptr)
                    objCore->setCount(nCount);
            }
        }
    }
    
    
    if ( _callbackRewardRelicGrind != nullptr )
    {
        _callbackRewardRelicGrind(true, 1);
    }
    
    _callbackRewardRelicGrind = nullptr;
}
#pragma mark - init
void AdventureRelicManager::setInitCore()
{
    _listInfoCore.clear();
    
    for(int i = 0; i < 7; ++i)
    {
        auto const objCore = InfoAdventureCore::create();
        objCore->setIdx(i+1);
        _listInfoCore.pushBack(objCore);
    }
}
