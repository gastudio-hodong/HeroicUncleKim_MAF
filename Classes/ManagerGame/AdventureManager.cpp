
//
//  AdventureManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/14.
//

#include "AdventureManager.h"

#include "Common/ConfigKey.h"

#include "GameUIPopup/Other/Adventure/PopupAdventureTutorial.h"
#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/AdventureRelicManager.h"
#include "ManagerGame/PrisonManager.h"

USING_NS_CC;

AdventureManager::AdventureManager(void):
_isAdventurePopupShow(false)
,_isServerDispatch(false)

,_nTutorialStep(0)

,_nLevel(0)
,_nExp(0)

,_adventureTicket(0)
,_adventureResetTime(0)
,_adventureDailyFriendCount(0)

,_nTotalAvilityDrawUseGem(0)

,_nBossSeason(0)
,_nBossEndTime(0)
,_nBossIdx(0)

//
,_callbackDrawPartyAbility(nullptr)
,_callbackMemberUpgrade(nullptr)
,_callbackFurnitureUpgrade(nullptr)

,_callbackInfo(nullptr)
,_callbackAdventureRunning(nullptr)
,_callbackAdventureReward(nullptr)
,_callbackAdventureTimeReduction(nullptr)
,_callbackObtainGummie(nullptr)
,_callbackMissionInfo(nullptr)
,_callbackMissionComplete(nullptr)
,_callbackMissionAllComplete(nullptr)

,_callbackBossSelect(nullptr)
,_callbackBossAttack(nullptr)
{
    
}

AdventureManager::~AdventureManager(void)
{
    _eventMission.Clear();
}

bool AdventureManager::init()
{
    return true;
}

#pragma mark - Load Table Data
void AdventureManager::setLoadAll()
{
    setLoadSetting();
    setLoadAdventurePartyAbilityOrigin();
    setLoadAbilityGradeRate();
    setLoadFurniture();
    setLoadPartyMember();
    setLoadPropertyType();
    setLoadMap();
    setLoadFriendAbility();
    setLoadBongLevel();
    setLoadMission();
    setLoadBoss();
    
    _nTutorialStep = UserDefault::getInstance()->getIntegerForKey(KEY_ADVT_TUTORIAL_STEP, 1);
    _nTotalAvilityDrawUseGem = UserDefault::getInstance()->getIntegerForKey(KEY_ADVT_AVILITY_USE_GEM, 0);
}

void AdventureManager::setLoadAdventurePartyAbilityOrigin()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::ADVENTURE_PARTY_ABILITY);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listPartyAbilityOriginal.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        int nIdx = jsonValue["idx"].GetInt();
        E_ADVENTURE_PROPERTY_TYPE nType = (E_ADVENTURE_PROPERTY_TYPE)jsonValue["type"].GetInt();
        std::string strEffect = jsonValue["grade_effect"].GetString();
        
        InfoAdventurePartyAbilityOrigin* obj = InfoAdventurePartyAbilityOrigin::create();
        obj->setIdx(nIdx);
        obj->setType(nType);
        obj->setListEffect(strEffect);
        
        _listPartyAbilityOriginal.pushBack(obj);
    }
}
void AdventureManager::setLoadSetting()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::ADVENTURE_SETTING);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
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
    loadAdventurePartyAbilityData();
    setLoadAbilityGradeRate();
}

void AdventureManager::setLoadAbilityGradeRate()
{
    auto objSetting = getSetting("party_ability_grade_rate");
    
    if(objSetting == nullptr)
        return;
    
    std::string strText = objSetting->getValueToString();
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    std::vector<Color3B> listColor = {
        Color3B(173,122,71),
        Color3B(51,133,255),
        Color3B(204,128,255),
        Color3B(204,41,41),
        Color3B(41,204,139),
    };
    
    _listInfoAbilityGradeRate.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        auto obj = InfoAdventurePartyAbilityGradeRate::create();
        obj->setGrade(jsonValue["grade"].GetInt());
        obj->setRate((double)jsonValue["rate"].GetInt());
        obj->setColor(listColor[i]);
        _listInfoAbilityGradeRate.pushBack(obj);
    }
}

void AdventureManager::setLoadPartyMember()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::ADVENTURE_PARTY_MEMBER);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoParty.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        int nPartyNum = jsonValue["party_num"].GetInt();
        int nIdx = jsonValue["member_idx"].GetInt();
        int nImageIdx = 0;
        if(jsonValue.HasMember("image_idx"))
            nImageIdx = jsonValue["image_idx"].GetInt();
        int nRequireLv = jsonValue["require_bed_lv"].GetInt();
        int nPropertyType = jsonValue["property_type"].GetInt();
        std::string strProperty = jsonValue["property"].GetString();
        
        auto objParty = getInfoParty(nPartyNum);
        
        if(objParty == nullptr)
        {
            objParty = InfoAdventureParty::create();
            objParty->setIdx(nPartyNum);
            _listInfoParty.pushBack(objParty);
        }
        
        InfoAdventureMember* objMember = InfoAdventureMember::create();
        objMember->setIdx(nIdx);
        objMember->setImageIdx(nImageIdx);
        objMember->setParty(nPartyNum);
        objMember->setRequireLv(nRequireLv);
        objMember->setType((E_ADVENTURE_PROPERTY_TYPE)nPropertyType);
        
        rapidjson::Document jsonParserProperty;
        jsonParserProperty.ParseInsitu<0>((char*)strProperty.c_str());
        if( jsonParserProperty.HasParseError() == true )
        {
            continue;
        }
        
        Vector<InfoAdventureUpgradeAbility*> listProperty;
        listProperty.clear();
        for ( int j = 0; j < jsonParserProperty.Size(); j++ )
        {
            const rapidjson::Value& jsonPropertyValue = jsonParserProperty[j];
            
            float fValue = jsonPropertyValue["v"].GetFloat();
            int nLv = jsonPropertyValue["lv"].GetInt();
            auto arrayCost = jsonPropertyValue["cost"].GetArray();
            
            Vector<InfoItems*> listItems;
            listItems.clear();
            for(int k = 0; k < arrayCost.Size(); ++k)
            {
                const rapidjson::Value& jsonCostValue = arrayCost[k];
                int nItemsIdx = jsonCostValue["idx"].GetInt();
                int nItemsCount = jsonCostValue["count"].GetInt();
                int nItemsTypeParams = 0;
                
                if(jsonCostValue.HasMember("type_params"))
                    nItemsTypeParams = jsonCostValue["type_params"].GetInt();
                
                InfoItems* infoItems = ItemsManager::getInstance()->getInfoItems(nItemsIdx);
                if(infoItems != nullptr)
                {
                    infoItems->setCount(nItemsCount);
                    infoItems->setTypeParams(nItemsTypeParams);
                    listItems.pushBack(infoItems);
                }
            }
            
            InfoAdventureUpgradeAbility* objProperty = InfoAdventureUpgradeAbility::create();
            objProperty->setLv(nLv);
            objProperty->setEffect(fValue);
            objProperty->setCost(listItems);
            objProperty->setType(objMember->getType());
            listProperty.pushBack(objProperty);
        }
        objMember->setInfoUpgradeAbility(listProperty);
        
        objParty->pushBackMember(objMember);
    }
    
    loadAdventurePartyAbilityData();
}

void AdventureManager::setLoadFurniture()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::ADVENTURE_FURNITURE);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoFurniture.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        int nIdx = jsonValue["idx"].GetInt();
        int nPropertyType = jsonValue["property_type"].GetInt();
        std::string strProperty = jsonValue["property"].GetString();
        
        
        InfoAdventureFurniture* objFurniture = InfoAdventureFurniture::create();
        objFurniture->setIdx(nIdx);
        objFurniture->setType((E_ADVENTURE_PROPERTY_TYPE)nPropertyType);
        
        rapidjson::Document jsonParserProperty;
        jsonParserProperty.ParseInsitu<0>((char*)strProperty.c_str());
        if( jsonParserProperty.HasParseError() == true )
        {
            continue;
        }
        
        Vector<InfoAdventureUpgradeAbility*> listProperty;
        listProperty.clear();
        std::vector<int> listCondition;
        listCondition.clear();
        for ( int j = 0; j < jsonParserProperty.Size(); j++ )
        {
            const rapidjson::Value& jsonPropertyValue = jsonParserProperty[j];
            
            float fValue = jsonPropertyValue["v"].GetFloat();
            int nLv = jsonPropertyValue["lv"].GetInt();
            int nCondition = jsonPropertyValue["condition"].GetInt();
            auto arrayCost = jsonPropertyValue["cost"].GetArray();
            
            Vector<InfoItems*> listItems;
            listItems.clear();
            for(int k = 0; k < arrayCost.Size(); ++k)
            {
                const rapidjson::Value& jsonCostValue = arrayCost[k];
                int nItemsIdx = jsonCostValue["idx"].GetInt();
                int nItemsCount = jsonCostValue["count"].GetInt();
                int nItemsTypeParams = 0;
                
                if(jsonCostValue.HasMember("type_params"))
                    nItemsTypeParams = jsonCostValue["type_params"].GetInt();
                
                InfoItems* infoItems = ItemsManager::getInstance()->getInfoItems(nItemsIdx);
                if(infoItems != nullptr)
                {
                    infoItems->setCount(nItemsCount);
                    infoItems->setTypeParams(nItemsTypeParams);
                    listItems.pushBack(infoItems);
                }
            }
            
            InfoAdventureUpgradeAbility* objProperty = InfoAdventureUpgradeAbility::create();
            objProperty->setLv(nLv);
            objProperty->setEffect(fValue);
            objProperty->setCost(listItems);
            objProperty->setType(objFurniture->getType());
            listProperty.pushBack(objProperty);
            
            listCondition.push_back(nCondition);
        }
        objFurniture->setInfoUpgradeAbility(listProperty);
        objFurniture->setLevelupCondition(listCondition);
        
        _listInfoFurniture.pushBack(objFurniture);
    }
    
    loadFurnitureData();
}

void AdventureManager::setLoadBongLevel()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::ADVENTURE_BONG_LEVEL);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
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
        obj->setTotalEXP(jsonValue["exp"].GetFloat());
        
        _listInfoBongLevel.pushBack(obj);
    }
}

void AdventureManager::setLoadPropertyType()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::ADVENTURE_PROPERTY_TYPE);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoPropertyType.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        auto obj = InfoAdventurePropertyType::create();
        
        int nIdx = jsonValue["idx"].GetInt();
        bool isPartyOption = (bool)jsonValue["is_party_option"].GetInt();
        bool isRewardOption = false;
        if(jsonValue.HasMember("is_reward_option"))
           isRewardOption = (bool)jsonValue["is_reward_option"].GetInt();
        
        obj->setIdx(nIdx);
        obj->setIsPartyOption(isPartyOption);
        obj->setIsRewardOption(isRewardOption);
        _listInfoPropertyType.pushBack(obj);
    }
}
void AdventureManager::setLoadMap()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::ADVENTURE_MAP);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoMap.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        auto areaNum = jsonValue["category"].GetInt();
        auto stage = jsonValue["stage"].GetInt();
        auto requireLv = jsonValue["require_lv"].GetInt();
        auto time = jsonValue["time"].GetInt();
        auto exp = jsonValue["exp"].GetInt();
        
        auto reward = jsonValue["reward"].GetString();
        auto rewardBossPoint = jsonValue["boss_att_point"].GetString();
        
        auto objMap = getInfoMap(areaNum);
        if(objMap == nullptr)
        {
            objMap = InfoAdventureMap::create();
            objMap->setIdx(areaNum);
            _listInfoMap.pushBack(objMap);
        }
        
        InfoAdventureStage* objStage = InfoAdventureStage::create();
        objStage->setIdx(stage);
        objStage->setArea(areaNum);
        objStage->setRequireLv(requireLv);
        objStage->setTime(time);
        objStage->setReward(reward);
        objStage->setRewardBossPoint(rewardBossPoint);
        objStage->setExp(exp);
        
        objMap->pushBackStage(objStage);
    }
}
void AdventureManager::setLoadFriendAbility()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::ADVENTURE_FRIEND_ABILITY);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoFriendAbility.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        int nIdx = 0;
        if(jsonValue.HasMember("idx"))
            nIdx = jsonValue["idx"].GetInt();
        int nLv = jsonValue["lv"].GetInt();
        int nAdvtPoint = jsonValue["advt_point"].GetInt();
        std::string strProperty = jsonValue["property_value"].GetString();
        
        InfoAdventureFriendAbility* objFriendAbility = InfoAdventureFriendAbility::create();
        objFriendAbility->setIdx(nIdx);
        objFriendAbility->setMinLv(nLv);
        objFriendAbility->setAdventurePoint(nAdvtPoint);
        
        rapidjson::Document jsonParserProperty;
        jsonParserProperty.ParseInsitu<0>((char*)strProperty.c_str());
        if( jsonParserProperty.HasParseError() == true )
        {
            continue;
        }
        
        std::vector<std::pair<E_ADVENTURE_PROPERTY_TYPE, TDouble>> listProperty;
        listProperty.clear();
        
        for ( int j = 0; j < jsonParserProperty.Size(); j++ )
        {
            const rapidjson::Value& jsonPropertyValue = jsonParserProperty[j];
            
            double nValue = jsonPropertyValue["v"].GetDouble();
            int nType = jsonPropertyValue["type"].GetInt();
            
            listProperty.push_back(std::make_pair((E_ADVENTURE_PROPERTY_TYPE)nType, nValue));
        }
        objFriendAbility->setListEffect(listProperty);
        
        _listInfoFriendAbility.pushBack(objFriendAbility);
    }
}
void AdventureManager::setLoadMission()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::ADVENTURE_MISSION);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
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
        
        auto group = jsonValue["type"].GetInt();
        auto idx = jsonValue["idx"].GetInt();
        auto type = jsonValue["mission_type"].GetInt();
        auto countMax = jsonValue["value"].GetInt();
        auto titleKey = jsonValue["text_key"].GetString();
        auto desc = MafUtils::format("%d", (int)countMax);
        auto reward = jsonValue["reward"].GetString();
        
        auto objQuest = InfoQuest::create();
        objQuest->setGroup(group);
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

void AdventureManager::setLoadBoss()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::ADVENTURE_BOSS);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoBoss.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        auto idx = jsonValue["idx"].GetInt();
        
        auto bHard =  jsonValue["is_hard"].GetInt();
        
        auto nameKey = jsonValue["name_key"].GetString();
        auto iconBannerPath = jsonValue["img_banner"].GetString();
        auto iconBattlePath = jsonValue["img_battle"].GetString();
        auto iconBattleBGPath = jsonValue["img_battle_bg"].GetString();
        
        auto hpNow = 0;
        auto hpMax = jsonValue["hp"].GetInt();
        
        auto weakness1 = jsonValue["weakness_idx_1"].GetInt();
        auto weakness1CountNow = 0;
        auto weakness1CountMax = jsonValue["weakness_count_1"].GetInt();
        auto weakness2 = jsonValue["weakness_idx_2"].GetInt();
        auto weakness2CountNow = 0;
        auto weakness2CountMax = jsonValue["weakness_count_2"].GetInt();
        
        auto reward = jsonValue["reward"].GetString();
        auto rewardBonus = jsonValue["bonus_reward"].GetString();
        
        auto obj = InfoAdventureBoss::create();
        obj->setIdx(idx);
        obj->setHard(bHard);
        obj->setNameKey(nameKey);
        obj->setIconBannerPath(iconBannerPath);
        obj->setIconBattlePath(iconBattlePath);
        obj->setIconBattleBGPath(iconBattleBGPath);
        obj->setHpNow(hpNow);
        obj->setHpMax(hpMax);
        obj->setWeakness1(weakness1);
        obj->setWeakness1CountNow(weakness1CountNow);
        obj->setWeakness1CountMax(weakness1CountMax);
        obj->setWeakness2(weakness2);
        obj->setWeakness2CountNow(weakness2CountNow);
        obj->setWeakness2CountMax(weakness2CountMax);
        obj->setReward(reward);
        obj->setRewardBonus(rewardBonus);
        
        _listInfoBoss.pushBack(obj);
    }
}

#pragma mark - Load Save
void AdventureManager::loadFurnitureData()
{
     std::string str = "";
     str.clear();
     str = UserDefault::getInstance()->getStringForKey(KEY_ADVT_OFFICE_LV);
     if(!str.empty()){
         str = MafAes256::Decrypt(str);
         
         for(auto info : _listInfoFurniture)
         {
             auto offset = str.find(",");
             if(offset != std::string::npos)
             {
                 auto result = str.substr(0,offset);
                 int ret = std::atoi(result.c_str());
                 info->setCurrentLv(ret);
                 str = str.substr(offset+1);
             }
             else
             {
                 int ret = std::atoi(str.c_str());
                 info->setCurrentLv(ret);
                 break;
             }
         }
     }
    
    saveFurnitureData();
}
void AdventureManager::saveFurnitureData()
{
    std::stringstream str;
    
    str.str("");
     for(auto info : _listInfoFurniture)
     {
         str << info->getCurrentLv() <<",";
     }
     UserDefault::getInstance()->setStringForKey(KEY_ADVT_OFFICE_LV, MafAes256::Encrypt(str.str()));
    
}

void AdventureManager::loadAdventurePartyAbilityData()
{
    int nPresetCount = 3;
    auto objSettingPresetCount = getSetting("party_ability_preset_count");
    
    if(objSettingPresetCount != nullptr)
        nPresetCount = objSettingPresetCount->getValueToInt();
    
    for(int i = 0; i < _listInfoParty.size(); ++i)
    {
        auto objParty = _listInfoParty.at(i);
        auto listMember = objParty->getListMemeber();
        std::string strDefault = "";
        for(int j = 0; j < listMember.size(); ++j)
        {
            strDefault += "0,";
        }
        strDefault = MafAes256::Encrypt(strDefault);
        std::string str_type = "";
        std::string str_grade = "";
        std::string str_seal = "";
        std::string str_lv = "";
        int n_preset_idx = 0;
        str_type.clear();
        str_grade.clear();
        str_seal.clear();
        str_lv.clear();
        
        cocos2d::Vector<InfoAdventurePartyAbilityPreset*> listPreset;
        listPreset.clear();
        for(int j = 0; j < nPresetCount; ++j)
        {
            str_type = UserDefault::getInstance()->getStringForKey(MafUtils::format("advt_p_%d_a_t_%d", i+1, j+1).c_str(), strDefault.c_str());
            str_grade = UserDefault::getInstance()->getStringForKey(MafUtils::format("advt_p_%d_a_g_%d", i+1, j+1).c_str(), strDefault.c_str());
            str_seal = UserDefault::getInstance()->getStringForKey(MafUtils::format("advt_p_%d_a_s_%d", i+1, j+1).c_str(), strDefault.c_str());
            
            InfoAdventurePartyAbilityPreset* objPreset = InfoAdventurePartyAbilityPreset::create();
            objPreset->setIdx(j+1);
            
            Vector<InfoAdventurePartyAbility*> list;
            list.clear();
            
            if(!str_type.empty())
            {
                str_type = MafAes256::Decrypt(str_type);
                auto listMember = objParty->getListMemeber();
                for(auto info : listMember)
                {
                    InfoAdventurePartyAbility* objEffect = InfoAdventurePartyAbility::create();
                    auto offset = str_type.find(",");
                    int ret = 0;
                    if(offset != std::string::npos)
                    {
                        auto result = str_type.substr(0,offset);
                        ret = std::atoi(result.c_str());
                        str_type = str_type.substr(offset+1);
                    }
                    else
                    {
                        ret = std::atoi(str_type.c_str());
                    }
                    
                    objEffect->setMember(info);
                    objEffect->setIdx(ret);
                    list.pushBack(objEffect);
                }
            }
            
            if(!str_grade.empty())
            {
                str_grade = MafAes256::Decrypt(str_grade);

                for(auto obj : list)
                {
                    auto offset = str_grade.find(",");
                    int ret = 0;
                    if(offset != std::string::npos)
                    {
                        auto result = str_grade.substr(0,offset);
                        ret = std::atoi(result.c_str());
                        str_grade = str_grade.substr(offset+1);
                    }
                    else
                    {
                        ret = std::atoi(str_grade.c_str());
                    }
                    obj->setGrade(ret);
                }
            }
            
            std::vector<bool>listSeal;
            listSeal.clear();
            
            if(!str_seal.empty())
            {
                str_seal = MafAes256::Decrypt(str_seal);

                for(int j = 0; j < list.size(); ++j)
                {
                    auto offset = str_seal.find(",");
                    bool ret = 0;
                    if(offset != std::string::npos)
                    {
                        auto result = str_seal.substr(0,offset);
                        ret = std::atoi(result.c_str());
                        str_seal = str_seal.substr(offset+1);
                    }
                    else
                    {
                        ret = std::atoi(str_seal.c_str());
                    }
                    listSeal.push_back(ret);
                }
            }
            objPreset->setListAblity(list);
            objPreset->setListSeal(listSeal);
            listPreset.pushBack(objPreset);
            
        }
        n_preset_idx = UserDefault::getInstance()->getIntegerForKey(MafUtils::format("advt_p_%d_a_p", i+1).c_str(), 1);
        objParty->setPresetIdx(n_preset_idx);
        objParty->setListAblityPreset(listPreset);
        
        str_lv = UserDefault::getInstance()->getStringForKey(MafUtils::format("advt_p_%d_m_lv", i+1).c_str(), strDefault.c_str());
        
        if(!str_lv.empty())
        {
            str_lv = MafAes256::Decrypt(str_lv);

            for(auto obj : listMember)
            {
                auto offset = str_lv.find(",");
                int ret = 0;
                if(offset != std::string::npos)
                {
                    auto result = str_lv.substr(0,offset);
                    ret = std::atoi(result.c_str());
                    str_lv = str_lv.substr(offset+1);
                }
                else
                {
                    ret = std::atoi(str_lv.c_str());
                }
                obj->setCurrentLv(ret);
            }
        }
    }
    checkPartyAvilityError();
    saveAdventurePartyAbilityData();
}

void AdventureManager::saveAdventurePartyAbilityData()
{
    std::stringstream str_type;
    std::stringstream str_grade;
    std::stringstream str_seal;
    std::stringstream str_lv;

    for(int i = 0; i < _listInfoParty.size(); ++i)
    {
        str_lv.str("");

        auto objParty = _listInfoParty.at(i);
        if(objParty == nullptr)
            continue;

        auto listAbilityPreset = objParty->getListAblityPreset();
        auto listMember = objParty->getListMemeber();
        
        for(auto obj : listAbilityPreset)
        {
            str_type.str("");
            str_grade.str("");
            str_seal.str("");
            
            auto listAbility = obj->getListAblity();
            auto listSeal = obj->getListSeal();
            for(auto ablity : listAbility)
            {
                str_type << ablity->getIdx() <<",";
                str_grade << ablity->getGrade() <<",";
            }
            for(auto seal : listSeal)
            {
                str_seal << (int)seal <<",";
            }
            
            UserDefault::getInstance()->setStringForKey(MafUtils::format("advt_p_%d_a_t_%d", i+1, obj->getIdx()).c_str(), MafAes256::Encrypt(str_type.str()));
            UserDefault::getInstance()->setStringForKey(MafUtils::format("advt_p_%d_a_g_%d", i+1, obj->getIdx()).c_str(), MafAes256::Encrypt(str_grade.str()));
            UserDefault::getInstance()->setStringForKey(MafUtils::format("advt_p_%d_a_s_%d", i+1, obj->getIdx()).c_str(), MafAes256::Encrypt(str_seal.str()));
            
        }

        for(auto obj : listMember)
        {
            str_lv << obj->getCurrentLv() <<",";
        }

        UserDefault::getInstance()->setStringForKey(MafUtils::format("advt_p_%d_m_lv", i+1).c_str(), MafAes256::Encrypt(str_lv.str()));
        
        UserDefault::getInstance()->setIntegerForKey(MafUtils::format("advt_p_%d_a_p", i+1).c_str(), objParty->getPresetIdx());
    }
}

void AdventureManager::saveAdventurePartyAbilitySeal(const int nParty, const int nPresetIdx)
{
    std::stringstream str_seal;
    auto objParty = getInfoParty(nParty);
    if(objParty == nullptr)
        return;
    
    str_seal.str("");
    auto listSeal = objParty->getListSeal(nPresetIdx);
    
    for(auto obj : listSeal)
    {
        str_seal << (int)obj <<",";
    }
    
    UserDefault::getInstance()->setStringForKey(MafUtils::format("advt_p_%d_a_s_%d", nParty, nPresetIdx).c_str(), MafAes256::Encrypt(str_seal.str()));
    
}

void AdventureManager::saveAdventurePartyMemberLv(const int nParty)
{
    std::stringstream str_lv;
    auto objParty = getInfoParty(nParty);
    if(objParty == nullptr)
        return;
    
    str_lv.str("");
    auto listMember = objParty->getListMemeber();
    
    for(auto obj : listMember)
    {
        str_lv << obj->getCurrentLv() <<",";
    }
    
    UserDefault::getInstance()->setStringForKey(MafUtils::format("advt_p_%d_m_lv", nParty).c_str(), MafAes256::Encrypt(str_lv.str()));
    
}

#pragma mark - list get set
const Vector<InfoAdventurePartyAbilityOrigin*>& AdventureManager::getListInfoPartyAbilityOriginal() const
{
    return _listPartyAbilityOriginal;
}

const Vector<InfoAdventurePartyAbilityOrigin*> AdventureManager::getListInfoPartyAbilityOriginalFromGrade(const int nGrade) const
{
    Vector<InfoAdventurePartyAbilityOrigin*> result;
    result.clear();
    
    for(const auto obj : _listPartyAbilityOriginal)
    {
        if(obj->getInfoEffect(nGrade) == nullptr)
            continue;
        
        result.pushBack(obj);
    }
    
    return result;
}
const cocos2d::Vector<InfoAdventurePartyAbility*> AdventureManager::getListInfoPartyAbility(const int nParty) const
{
    cocos2d::Vector<InfoAdventurePartyAbility*> result;
    result.clear();
    
    for(const auto obj : _listInfoParty)
    {
        if(obj->getIdx() == nParty)
        {
            result = obj->getListAblity();
            break;
        }
    }
    
    return result;
}

const Vector<InfoAdventurePartyAbilityGradeRate*>& AdventureManager::getListInfoPartyAbilityGradeRate() const
{
    return _listInfoAbilityGradeRate;
}

const Vector<InfoAdventureParty*>& AdventureManager::getListInfoParty() const
{
    return _listInfoParty;
}

const Vector<InfoAdventureFurniture*>& AdventureManager::getListInfoFurniture() const
{
    return _listInfoFurniture;
}

const Vector<InfoAdventureMap*>& AdventureManager::getListInfoMap() const
{
    return _listInfoMap;
}
const Vector<InfoAdventureFriend*>& AdventureManager::getListInfoFriend() const
{
    return _listInfoFriend;
}

const Vector<InfoAdventureBoss*> AdventureManager::getListInfoBoss(bool bHard) const
{
    Vector<InfoAdventureBoss*> listResult;
    
    for ( auto& obj : _listInfoBoss )
    {
        if ( obj->isHard() == bHard )
        {
            listResult.pushBack(obj);
        }
    }
    
    return listResult;
}

const Vector<InfoQuest*> AdventureManager::getListInfoMissionFromGroup(const E_ADVENTURE_MISSION_GROUP eType) const
{
    Vector<InfoQuest*> result;
    result.clear();
    
    for(auto obj : _listInfoQuest)
    {
        if(obj->getGroup() == (int)eType)
            result.pushBack(obj);
    }
    
    return result;
}

#pragma mark - info get set
InfoSetting* const AdventureManager::getSetting(const std::string strKey) const
{
    InfoSetting* result = nullptr;
    
    for(const auto obj : _listInfoSetting)
    {
        if(obj->getKey() == strKey)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

InfoAdventurePartyAbilityOrigin* const AdventureManager::getPartyAbility(const int nType) const
{
    InfoAdventurePartyAbilityOrigin* result = nullptr;
    
    for(const auto obj : _listPartyAbilityOriginal)
    {
        if(obj->getType() == (E_ADVENTURE_PROPERTY_TYPE)nType)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

const int AdventureManager::getPartyAbilityGradeRate(const int nGrade) const
{
    int result = -1;
    
    for(const auto obj : _listInfoAbilityGradeRate)
    {
        if(obj->getGrade() == nGrade)
        {
            result = obj->getRate();
            break;
        }
    }
    
    return result;
}
InfoAdventurePartyAbilityGradeRate* const AdventureManager::getInfoPartyAbilityGradeRate(const int nGrade) const
{
    InfoAdventurePartyAbilityGradeRate* result = nullptr;
    
    for(const auto obj : _listInfoAbilityGradeRate)
    {
        if(obj->getGrade() == nGrade)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
InfoAdventureParty* const AdventureManager::getInfoParty(const int nParty) const
{
    InfoAdventureParty* result = nullptr;
    
    for(const auto obj : _listInfoParty)
    {
        if(obj->getIdx() == nParty)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

InfoAdventureFurniture* const AdventureManager::getInfoFurniture(const int nIdx) const
{
    InfoAdventureFurniture* result = nullptr;
    
    for(const auto obj : _listInfoFurniture)
    {
        if(obj->getIdx() == nIdx)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

InfoAdventurePropertyType* const AdventureManager::getPropertyType(const int nIdx) const
{
    InfoAdventurePropertyType* result = nullptr;
    
    for(const auto obj : _listInfoPropertyType)
    {
        if(obj->getIdx() == nIdx)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
InfoAdventurePropertyType* const AdventureManager::getPropertyType(const E_ADVENTURE_PROPERTY_TYPE eType) const
{
    InfoAdventurePropertyType* result = nullptr;
    
    for(const auto obj : _listInfoPropertyType)
    {
        if(obj->getType() == eType)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

InfoAdventureMap* const AdventureManager::getInfoMap(const int nArea) const
{
    InfoAdventureMap* result = nullptr;
    
    for(const auto obj : _listInfoMap)
    {
        if(obj->getIdx() == nArea)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
InfoAdventureStage* const AdventureManager::getInfoStage(const int nArea, const int nStage) const
{
    InfoAdventureStage* result = nullptr;
    
    const auto objMap = getInfoMap(nArea);
    
    if(objMap == nullptr)
        return result;
    
    result = objMap->getStage(nStage);
    
    return result;
}
InfoBongContentLevel* const AdventureManager::getInfoBongLevel(const int nLv) const
{
    InfoBongContentLevel* result = nullptr;
    
    for(const auto obj : _listInfoBongLevel)
    {
        if(obj->getLevel() == nLv)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
InfoAdventureFriendAbility* const AdventureManager::getInfoFriendAbilityFromIdx(const int nIdx)
{
    InfoAdventureFriendAbility* result = nullptr;
    
    for(const auto obj : _listInfoFriendAbility)
    {
        if(obj->getIdx() == nIdx)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
InfoAdventureFriendAbility* const AdventureManager::getInfoFriendAbilityFromLevel(const int nCurrentLevel)
{
    InfoAdventureFriendAbility* result = nullptr;
    
    for(const auto obj : _listInfoFriendAbility)
    {
        if(obj->getMinLv() > nCurrentLevel)
            break;
        
        result = obj;
    }
    
    return result;
}

InfoAdventureFriend* const AdventureManager::getInfoFriendFromUserIdx(const int nUseridx)
{
    InfoAdventureFriend* result = nullptr;
    
    for(const auto obj : _listInfoFriend)
    {
        if(obj->getUseridx() == nUseridx)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
InfoAdventureFriend* const AdventureManager::getInfoFriendFromPartyIdx(const int nPartyidx)
{
    InfoAdventureFriend* result = nullptr;
    
    for(const auto obj : _listInfoFriend)
    {
        if(obj->getPartyIdx() == nPartyidx)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
InfoAdventureBoss* const AdventureManager::getInfoBoss(const int nidx)
{
    InfoAdventureBoss* result = nullptr;
    
    for(const auto obj : _listInfoBoss)
    {
        if(obj->getIdx() == nidx)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
InfoQuest* AdventureManager::getInfoMission(const int nGroup, const int nIdx)
{
    InfoQuest* result = nullptr;
    
    for(auto obj : _listInfoQuest)
    {
        if(obj->getGroup() == nGroup && obj->getIdx() == nIdx)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
#pragma mark - utils get set
const bool AdventureManager::isServerDispatch() const
{
    return _isServerDispatch;
}
const int AdventureManager::getLevel()
{
    return _nLevel.valueInt();
}
const int AdventureManager::getMaxLevel() const
{
    return (int)_listInfoBongLevel.size();
}
const int AdventureManager::getExp()
{
    return _nExp.valueInt();
}
const int AdventureManager::getPartyLevel(const int nParty) const
{
    int result = 0;
    
    const auto objParty = getInfoParty(nParty);
    
    const auto list = objParty->getListMemeber();
    
    for(const auto obj : list)
    {
        result += obj->getCurrentLv();
    }
    
    return result;
}
const int AdventureManager::getTicket()
{
    return _adventureTicket.valueInt();
}

void AdventureManager::setTicket(const int value)
{
    _adventureTicket = value;
}

const int AdventureManager::getCurrentRunningParty() const
{
    int result = 0;
    
    for(auto obj : _listInfoParty)
    {
        if(obj->isRunning())
            result++;
    }
    
    return result;
}

const int AdventureManager::getTotalAvilityDrawUseGem()
{
    return _nTotalAvilityDrawUseGem.valueInt();
}

const bool AdventureManager::isAdventurePopupShow() const
{
    return _isAdventurePopupShow;
}
void AdventureManager::setIsAdventurePopupShow(const bool isShow)
{
    _isAdventurePopupShow = isShow;
}
void AdventureManager::checkLocalCompleteUpgrade()
{
    for(auto objParty : _listInfoParty)
    {
        objParty->checkLocalCompleteUpgrade();
    }
    for(auto objFurniture : _listInfoFurniture)
    {
        objFurniture->completeUpgrade();
    }
}
const int AdventureManager::getDailyFriendCount()
{
    return _adventureDailyFriendCount.valueInt();
}
const bool AdventureManager::isMissionComplete() const
{
    bool isResult = false;
    
    for(auto obj : _listInfoQuest)
    {
        if ( !obj->isReceive() && obj->getCountNow() >= obj->getCountMax())
        {
            isResult = true;
            break;
        }
    }
    
    return isResult;
}

bool AdventureManager::isAdventureBullet(int category, int stage)
{
    bool isResult = false;
    
    std::string value = MafUtils::format("%d_%d", category, stage);
    if ( std::find(_listAdventureBullet.begin(), _listAdventureBullet.end(), value) != _listAdventureBullet.end() )
    {
        isResult = true;
    }
 
    return isResult;
}


#pragma mark -Effect
const double AdventureManager::getFurniturePropertyEffect(const E_ADVENTURE_PROPERTY_TYPE eType) const
{
    double result = 0;
    
    for(const auto obj : _listInfoFurniture)
    {
        const auto objProperty = obj->getInfoUpgradeAbility();
        
        if(obj->getType() != eType || objProperty == nullptr)
            continue;
        
        result += objProperty->getEffect();
    }
    
    return result;
}

const double AdventureManager::getPartyMemberEffect(const E_ADVENTURE_PROPERTY_TYPE eType, const int nParty) const
{
    double result = 0;
    
    const auto objParty = getInfoParty(nParty);
    
    if(objParty == nullptr)
        return result;
    
    const auto listMember = objParty->getListMemeber();
    
    for(const auto obj : listMember)
    {
        const auto objProperty = obj->getInfoUpgradeAbility();
        
        if(obj->getType() != eType || objProperty == nullptr)
            continue;
        
        if(objProperty->isPartyOption() == false)
            continue;
        
        result += objProperty->getEffect();
    }
    
    return result;
}

const double AdventureManager::getPartyMemberPublicEffect(const E_ADVENTURE_PROPERTY_TYPE eType, int nParty, bool isInclueParty) const
{
    double result = 0;
    
    for(const auto objParty : _listInfoParty)
    {
        if(objParty == nullptr)
            continue;
        
        if(!isInclueParty)
        {
            if(objParty->getIdx() == nParty)
                continue;
        }
        
        const auto listMember = objParty->getListMemeber();
        
        for(const auto obj : listMember)
        {
            const auto objProperty = obj->getInfoUpgradeAbility();
            
            if(obj->getType() != eType || objProperty == nullptr)
                continue;
            
            if(objProperty->isPartyOption() == true)
                continue;
            
            result += objProperty->getEffect();
        }
        
    }
    
    return result;
}

const double AdventureManager::getPartyAbilityEffect(const E_ADVENTURE_PROPERTY_TYPE eType, const int nParty) const
{
    double result = 0;
    
    const auto objParty = getInfoParty(nParty);
    const int nPartyLevel = getPartyLevel(nParty);
    
    if(objParty == nullptr)
        return result;
    
    const auto listEffect = objParty->getListAblity();
    
    for(const auto obj : listEffect)
    {
        if(obj->isPartyOption() == false || obj->getType() != eType)
            continue;
        
        result += obj->getEffect(nPartyLevel);
    }
    
    return result;
}

const double AdventureManager::getPartyAbilityPublicEffect(const E_ADVENTURE_PROPERTY_TYPE eType, int nParty, bool isInclueParty) const
{
    double result = 0;
    
    for(const auto objParty : _listInfoParty)
    {
        const int nPartyLevel = getPartyLevel(objParty->getIdx());
        
        if(objParty == nullptr)
            continue;
        
        if(!isInclueParty)
        {
            if(objParty->getIdx() == nParty)
                continue;
        }
        
        const auto listEffect = objParty->getListAblity();
        
        for(const auto obj : listEffect)
        {
            if(obj->isPartyOption() == true || obj->getType() != eType)
                continue;
            
            result += obj->getEffect(nPartyLevel);
        }
    }
    
    return result;
}

const double AdventureManager::getPartyAbilityDamage() const
{
    double result = 1;
    bool isIncrese = false;
    
    for(const auto objParty : _listInfoParty)
    {
        const int nPartyLevel = getPartyLevel(objParty->getIdx());
        
        if(objParty == nullptr)
            continue;
        
        const auto listEffect = objParty->getListAblity();
        
        for(const auto obj : listEffect)
        {
            if(obj->getType() != E_ADVENTURE_PROPERTY_TYPE::DUNGEON_ATK)
                continue;
            isIncrese = true;
            result *= (1 + obj->getEffect(nPartyLevel) * 0.01);
        }
    }
    
    if(!isIncrese)
        result = 0;
    else
    {
        result *= 100;
        result -= 100;
    }
    
    return result;
}
const double AdventureManager::getEffectDecreseTime(const int nParty, bool isInclueParty) const
{
    double result = 0;
    
    result = getFurniturePropertyEffect(E_ADVENTURE_PROPERTY_TYPE::ALL_TIME_DECREASE)
    + getPartyMemberEffect(E_ADVENTURE_PROPERTY_TYPE::PARTY_TIME_DECREASE, nParty)
    + getPartyMemberPublicEffect(E_ADVENTURE_PROPERTY_TYPE::ALL_TIME_DECREASE, nParty, isInclueParty)
    + getPartyAbilityEffect(E_ADVENTURE_PROPERTY_TYPE::PARTY_TIME_DECREASE, nParty)
    + getPartyAbilityPublicEffect(E_ADVENTURE_PROPERTY_TYPE::ALL_TIME_DECREASE, nParty, isInclueParty);
    
    const auto objSettingMax = getSetting("advt_time_decrese_max");
    int nDecreseMax = 0;
    if(objSettingMax != nullptr)
        nDecreseMax = objSettingMax->getValueToInt();
    
    if(result >= nDecreseMax)
        result = nDecreseMax;
    
    return result;
}

const double AdventureManager::getAdventureTotalEffect(const E_ADVENTURE_PROPERTY_TYPE eType, const int nParty, bool isInclueParty) const
{
    double result = 0;
    
    result = getFurniturePropertyEffect(eType)
    + getPartyMemberEffect(eType, nParty)
    + getPartyMemberPublicEffect(eType, nParty, isInclueParty)
    + getPartyAbilityEffect(eType, nParty)
    + getPartyAbilityPublicEffect(eType, nParty, isInclueParty);
    
    return result;
}
const std::string AdventureManager::getPropertyEffectText(const E_ADVENTURE_PROPERTY_TYPE eType, const double dValue)
{
    std::string result = "";
    
    switch(eType)
    {
        case E_ADVENTURE_PROPERTY_TYPE::NONE :  break;
        case E_ADVENTURE_PROPERTY_TYPE::AREA_STAGE_OPEN :
        case E_ADVENTURE_PROPERTY_TYPE::PARTY_MEMBER_OPEN : result = MafUtils::format("%d", (int)dValue); break;
        case E_ADVENTURE_PROPERTY_TYPE::PRINCESSES_SKILL_FLOOR_INCREASE : result = GAME_TEXTFORMAT("t_ui_town_floor", (int)dValue);  break;
        case E_ADVENTURE_PROPERTY_TYPE::PARTY_GEM_REWARD_INCREASE : result = MafUtils::format("+%d", (int)dValue); break;
        default: result = MafUtils::format("%.1f%%", dValue); break;
    }
    
    return result;
}
#pragma mark - Party Ability
void AdventureManager::requestPartyAbilityDraw(const std::function<void(bool, int)>& callback, const int nParty)
{
    _callbackDrawPartyAbility = callback;
    
    const auto obj = getInfoParty(nParty);
    if(obj == nullptr)
    {
        
        if(_callbackDrawPartyAbility != nullptr)
            _callbackDrawPartyAbility(false, 0);
        
        _callbackDrawPartyAbility = nullptr;
        return;
    }
    
    if(obj->getSealCount() >= obj->getAbilityOpenCount())
        return;
    
    const int nNeedGemCost = getDrawPrice(nParty);
    
    if ( ItemsMoneyManager::getInstance()->getGem() < nNeedGemCost)
    {
        auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
        popup->show();
        
        if(_callbackDrawPartyAbility != nullptr)
            _callbackDrawPartyAbility(false, 2);
        return;
    }
    
    // 재화 감소
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    ItemsMoneyManager::getInstance()->setGem(-nNeedGemCost);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    _nTotalAvilityDrawUseGem += nNeedGemCost;
    UserDefault::getInstance()->setIntegerForKey(KEY_ADVT_AVILITY_USE_GEM, _nTotalAvilityDrawUseGem);
    
    const auto listAbility = getListInfoPartyAbility(nParty);
    const auto listSeal = obj->getListSeal();
    
    for(int i = 0; i < listAbility.size(); ++i)
    {
        bool isSeal = listSeal[i];
        if(isSeal)
            continue;
        
        auto obj = listAbility.at(i);
        drawPartyAbility(obj);
    }
    
    saveAdventurePartyAbilityData();
    
    if(_callbackDrawPartyAbility != nullptr)
        _callbackDrawPartyAbility(true, 1);
    _callbackDrawPartyAbility = nullptr;
}

void AdventureManager::drawPartyAbility(InfoAdventurePartyAbility* const infoAbility)
{
    if(infoAbility == nullptr || !infoAbility->isOpen())
        return;
    
    const int nRate1 = getPartyAbilityGradeRate(1);
    const int nRate2 = nRate1 + getPartyAbilityGradeRate(2);
    const int nRate3 = nRate2 + getPartyAbilityGradeRate(3);
    const int nRate4 = nRate3 + getPartyAbilityGradeRate(4);
    const int nRate5 = nRate4 + getPartyAbilityGradeRate(5);
    int nRandTmp = random(1, nRate5);
    
    if (nRandTmp > 0 && nRandTmp <= nRate1) //1 ~ 499
    {
        infoAbility->setGrade(1);
    }
    else if (nRandTmp > nRate1 && nRandTmp <= nRate2) // 500 ~ 879
    {
        infoAbility->setGrade(2);
    }
    else if (nRandTmp > nRate2 && nRandTmp <= nRate3) // 880 ~ 979
    {
        infoAbility->setGrade(3);
    }
    else if (nRandTmp > nRate3 && nRandTmp <= nRate4) // 980 ~ 999
    {
        infoAbility->setGrade(4);
    }
    else if (nRandTmp > nRate4 && nRandTmp <= nRate5) //999 ~ 1000
    {
        infoAbility->setGrade(5);
    }
    
    const Vector<InfoAdventurePartyAbilityOrigin*> listAbility = getListInfoPartyAbilityOriginalFromGrade(infoAbility->getGrade());
    
    nRandTmp = random(0, (int)listAbility.size() - 1);
    const auto obj = listAbility.at(nRandTmp);
    infoAbility->setIdx((int)obj->getType());
}
const int AdventureManager::getSealCount(int nParty) const
{
    int result = 0;
    
    const auto objParty = getInfoParty(nParty);
    if(objParty == nullptr)
        return result;
    
    return objParty->getSealCount();
}

const int AdventureManager::getDrawPrice(const int nParty) const
{
    int result = 0;
    int nDefault = 100;
    int nPlus = 100;
    
    const auto objSettingDefaultPrice = AdventureManager::getInstance()->getSetting("party_ability_cost");
    const auto objSettingPlusPrice = AdventureManager::getInstance()->getSetting("party_ability_cost_increase");
    
    if(objSettingDefaultPrice != nullptr)
        nDefault = objSettingDefaultPrice->getValueToInt();
    if(objSettingPlusPrice != nullptr)
        nPlus = objSettingPlusPrice->getValueToInt();
    
    result = nDefault + nPlus * getSealCount(nParty);
    
    return result;
}
void AdventureManager::checkPartyAvilityError()
{
    for(auto obj : _listInfoParty)
    {
        obj->checkPresetError();
    }
}

const int AdventureManager::getResetTime()
{
    return _adventureResetTime.valueInt();
}

const bool AdventureManager::isAdventureComplete() const
{
    bool result = false;
    
    for(const auto obj : _listInfoParty)
    {
        if(obj->getInfoStage() == nullptr)
            continue;
            
        auto time = obj->getEndTime() - UtilsDate::getInstance()->getTime();
            
        if(time < 0)
        {
            result = true;
            break;
        }
    }
    
    for(const auto obj : _listInfoFriend)
    {
        if(obj->getInfoStage() == nullptr)
            continue;
            
        auto objStage = obj->getInfoStage();
        
        auto time = objStage->getEndTime() - UtilsDate::getInstance()->getTime();
            
        if(time < 0)
        {
            result = true;
            break;
        }
    }
    
    return result;
}

void AdventureManager::setTutorialStep(const int nStep, const bool isSave)
{
    _nTutorialStep = nStep;
    if(_nTutorialStep == 7)
    {
        RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE);
        _nTutorialStep++;
    }
    if(isSave)
    {
        UserDefault::getInstance()->setIntegerForKey(KEY_ADVT_TUTORIAL_STEP, _nTutorialStep);
    }
}
const int AdventureManager::getTutorialStep() const
{
    return _nTutorialStep;
}

#pragma mark - Office
void AdventureManager::requestPartyMemberUpgrade(const std::function<void(bool, int)>& callback, const int nParty, const int nIdx)
{
    _callbackMemberUpgrade = callback;
    
    const auto objParty = getInfoParty(nParty);
    int nResult = 0;
    bool isResult = false;
    if(objParty == nullptr)
    {
        //없는 Ofiice
        if(_callbackMemberUpgrade != nullptr)
            _callbackMemberUpgrade(isResult, nResult);
        _callbackMemberUpgrade = nullptr;
        return;
    }
    const auto objMember = objParty->getMember(nIdx);
    
    nResult = objMember->requestUpgrade();
    
    if(nResult == 1)
    {
        isResult = true;
        //레벨업
        saveAdventurePartyMemberLv(nParty);
    }
    
    if(_callbackMemberUpgrade != nullptr)
        _callbackMemberUpgrade(isResult, nResult);
    _callbackMemberUpgrade = nullptr;
}

void AdventureManager::resetRunningPartyData()
{
    for(const auto objParty : _listInfoParty)
    {
        objParty->resetStageData();
    }
    for(const auto objMap : _listInfoMap)
    {
        objMap->resetStage();
    }
}

#pragma mark - Office
void AdventureManager::requestFurnitureUpgrade(const std::function<void(bool, int)>& callback, const int nIdx)
{
    _callbackFurnitureUpgrade = callback;
    
    const auto objFurniture = getInfoFurniture(nIdx);
    int nResult = 0;
    bool isResult = false;
    if(objFurniture == nullptr)
    {
        //없는 Ofiice
        if(_callbackFurnitureUpgrade != nullptr)
            _callbackFurnitureUpgrade(false, nResult);
        _callbackFurnitureUpgrade = nullptr;
        return;
    }
    
    const int nCondition = objFurniture->getLevelupCondition(objFurniture->getCurrentLv() + 1);
    
    if(nCondition == -1 || nCondition > _nLevel)
    {
        //레벨업 조건 미달
        nResult = 4;
        if(_callbackFurnitureUpgrade != nullptr)
            _callbackFurnitureUpgrade(false, nResult);
        _callbackFurnitureUpgrade = nullptr;
        return;
    }
    
    nResult = objFurniture->requestUpgrade();
    
    if(nResult == 1)
    {
        isResult = true;
        //레벨업
        saveFurnitureData();
    }
    
    if(_callbackFurnitureUpgrade != nullptr)
        _callbackFurnitureUpgrade(isResult, nResult);
    _callbackFurnitureUpgrade = nullptr;
}

const int AdventureManager::getFurnitureTotalLv() const
{
    int result = 0;
    
    for(auto obj : _listInfoFurniture)
        result += obj->getCurrentLv();
    
    return result;
}

#pragma mark - boss
int AdventureManager::getBossIdx()
{
    return _nBossIdx.valueInt();
}

int AdventureManager::getBossEndTime()
{
    return _nBossEndTime.valueInt();
}

int AdventureManager::getBossSeason()
{
    return _nBossSeason.valueInt();
}

#pragma mark - network
void AdventureManager::requestInfo(const std::function<void(bool, int)>& callback)
{
    _callbackInfo = callback;
    //
    std::string url = "/adventure/info/%d";
    _isServerDispatch = true;
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseInfo(response,dd);
    };
    req->send(cb);
}

void AdventureManager::responseInfo(cocos2d::network::HttpResponse* const response, const std::string &data)
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
        }
        _callbackInfo = nullptr;
        return;
    }
    
    _nLevel = jsonParser["_level"].GetInt();
    if(_nTutorialStep <= 1 && _nLevel >= 1)
        _nTutorialStep = 8;
    if(_nTutorialStep == 3)
    {
        auto objCore = AdventureRelicManager::getInstance()->getInfoCore(1);
        if(objCore->isActive() && objCore->getCount() <= 0)
            _nTutorialStep = 4;
    }
    
    _nExp = jsonParser["_exp"].GetDouble();
    _adventureResetTime = jsonParser["_reset_time"].GetInt();
    
    _adventureTicket = jsonParser["_adventure_ticket"].GetInt();
    _adventureDailyFriendCount = jsonParser["_friend_daily_count"].GetInt();
    
    _listInfoFriend.clear();
    for ( int i = 0; i < jsonParser["_friend_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_friend_list"][i];
        
        const bool isAdventure = jsonValue["is_adv"].GetBool();
        const int nUserIdx = jsonValue["useridx"].GetInt();
        const int nLevel = jsonValue["level"].GetInt();
        const std::string strPlatform = jsonValue["platform"].GetString();
        const std::string strNickName = jsonValue["nick"].GetString();
        
        const auto obj = InfoAdventureFriend::create();
        obj->setLevel(nLevel);
        obj->setUseridx(nUserIdx);
        obj->setIsAdventure(isAdventure);
        obj->setPlatform(strPlatform);
        obj->setNickName(strNickName);
        
        _listInfoFriend.pushBack(obj);
    }
    
    //
    resetRunningPartyData();
    
    //
    for ( int i = 0; i < jsonParser["_adventure_area_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_adventure_area_list"][i];
        
        const int nArea = jsonValue["category"].GetInt();
        const int nStage = jsonValue["stage"].GetInt();
        const int nParty = jsonValue["party"].GetInt();
        const int nEndTime = jsonValue["end_time"].GetInt();
        const bool isReward = jsonValue["is_reward"].GetBool();
        
        const auto objParty = getInfoParty(nParty);
        const auto objStage = getInfoStage(nArea, nStage);
        
        if(objStage == nullptr)
            continue;
        
        objStage->setEndTime(nEndTime);
        objStage->setIsComplete(isReward);
        if(!isReward)
        {
            objStage->setIsRunning(true);
            if(objParty != nullptr)
            {
                objParty->setInfoStage(objStage);
            }
            else
            {
                const auto objFriend = getInfoFriendFromPartyIdx(nParty);
                if(objFriend == nullptr)
                {
                    const auto obj = InfoAdventureFriend::create();
                    obj->setUseridxFromPartyIdx(nParty);
                    obj->setInfoStage(objStage);
                           
                    _listInfoFriend.pushBack(obj);
                }
                else
                    objFriend->setInfoStage(objStage);
            }
        }
    }
    
    //
    _listAdventureBullet.clear();
    if ( jsonParser.HasMember("_almighty_map") == true )
    {
        for ( int i = 0; i < jsonParser["_almighty_map"].Size(); i++ )
        {
            _listAdventureBullet.push_back(jsonParser["_almighty_map"][i].GetString());
        }
    }
    
    // boss
    if ( jsonParser.HasMember("_att_point") == true )
    {
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::ADVENTURE_BOSS_BULLET_FOREST, 0, jsonParser["_att_point"]["117"].GetInt()); //117,모험단 보스 속성탄[숲]
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::ADVENTURE_BOSS_BULLET_EARTH, 0, jsonParser["_att_point"]["118"].GetInt()); //118,모험단 보스 속성탄[대지]
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::ADVENTURE_BOSS_BULLET_GLACIER, 0, jsonParser["_att_point"]["119"].GetInt()); //119,모험단 보스 속성탄[얼음]
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::ADVENTURE_BOSS_BULLET_VOLCANO, 0, jsonParser["_att_point"]["120"].GetInt()); //120,모험단 보스 속성탄[화산]
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::ADVENTURE_BOSS_BULLET_ALL, 0, jsonParser["_att_point"]["121"].GetInt()); //121,모험단 보스 속성탄[만능]
    }
    
    
    
    _nBossEndTime = 0;
    if ( jsonParser.HasMember("_end_time") == true )
    {
        _nBossEndTime = jsonParser["_end_time"].GetInt();
    }
    
    _nBossSeason = 0;
    if ( jsonParser.HasMember("_season") == true )
    {
        _nBossSeason = jsonParser["_season"].GetInt();
    }
    
    _nBossIdx = 0;
    if ( jsonParser.HasMember("_boss_info") == true )
    {
        _nBossIdx = jsonParser["_boss_info"]["idx"].GetInt();
        
        auto hpNow = jsonParser["_boss_info"]["hp"].GetInt();
        auto weakness1Count = jsonParser["_boss_info"]["weakness1"].GetInt();
        auto weakness2Count = jsonParser["_boss_info"]["weakness2"].GetInt();
        
        auto objInfoBoss = getInfoBoss(_nBossIdx.valueInt());
        if ( objInfoBoss != nullptr )
        {
            objInfoBoss->setHpNow(hpNow);
            objInfoBoss->setWeakness1CountNow(weakness1Count);
            objInfoBoss->setWeakness2CountNow(weakness2Count);
        }
    }
    
    //
    if ( _callbackInfo != nullptr )
    {
        _callbackInfo(true, 1);
    }
    
    _callbackInfo = nullptr;
    requestMissionInfo(nullptr);
}

void AdventureManager::requestAdventureRunning(const std::function<void(bool, int)>& callback, InfoAdventureStage* const infoStage, const int nParty, int nFUserIdx, std::string strFPlatform)
{
    _callbackAdventureRunning = callback;
    _isServerDispatch = true;
    
    if(infoStage == nullptr)
    {
        if(_callbackAdventureRunning != nullptr)
            _callbackAdventureRunning(false, 0);
        _callbackAdventureRunning = nullptr;
        return;
    }
    
    //
    const std::string url = "/adventure/UpdateAdventureArea";
    
    double dEffect = 0;
    
    if(nFUserIdx == -1)
        dEffect = getEffectDecreseTime(nParty, true);
    else
    {
        auto objFriend = AdventureManager::getInstance()->getInfoFriendFromUserIdx(nFUserIdx);
        if(objFriend != nullptr)
            dEffect = objFriend->getAbilityEffect(E_ADVENTURE_PROPERTY_TYPE::PARTY_TIME_DECREASE);
    }
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_category", infoStage->getArea());
    req->setData("_stage", infoStage->getIdx());
    req->setData("_party_idx", nParty);
    req->setData("_time_decrese", dEffect);
    req->setData("_f_useridx", nFUserIdx);
    req->setData("_f_platform", strFPlatform);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseAdventureRunning(response,dd);
    };
    req->send(cb);
}

void AdventureManager::responseAdventureRunning(cocos2d::network::HttpResponse* const response, const std::string &data)
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
        if ( _callbackAdventureRunning != nullptr )
        {
            _callbackAdventureRunning(false, 0);
            _callbackAdventureRunning = nullptr;
        }
        return;
    }
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackAdventureRunning != nullptr )
        {
            _callbackAdventureRunning(false, nResult);
            _callbackAdventureRunning = nullptr;
        }
        return;
    }
    
    _adventureTicket   = jsonParser["_adventure_ticket"].GetInt();
    _adventureDailyFriendCount   = jsonParser["_friend_daily_count"].GetInt();
    
    _listInfoFriend.clear();
    for ( int i = 0; i < jsonParser["_friend_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_friend_list"][i];
        
        const bool isAdventure = jsonValue["is_adv"].GetBool();
        const int nUserIdx = jsonValue["useridx"].GetInt();
        const int nLevel = jsonValue["level"].GetInt();
        const std::string strPlatform = jsonValue["platform"].GetString();
        const std::string strNickName = jsonValue["nick"].GetString();
        
        const auto obj = InfoAdventureFriend::create();
        obj->setLevel(nLevel);
        obj->setUseridx(nUserIdx);
        obj->setIsAdventure(isAdventure);
        obj->setPlatform(strPlatform);
        obj->setNickName(strNickName);
        
        _listInfoFriend.pushBack(obj);
    }
    resetRunningPartyData();
    
    for ( int i = 0; i < jsonParser["_adventure_area_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_adventure_area_list"][i];
        
        const int nArea = jsonValue["category"].GetInt();
        const int nStage = jsonValue["stage"].GetInt();
        const int nParty = jsonValue["party"].GetInt();
        const int nEndTime = jsonValue["end_time"].GetInt();
        const bool isReward = jsonValue["is_reward"].GetBool();
        
        const auto objParty = getInfoParty(nParty);
        const auto objStage = getInfoStage(nArea, nStage);
        
        if(objStage == nullptr)
            continue;
        
        objStage->setEndTime(nEndTime);
        objStage->setIsComplete(isReward);
        if(!isReward)
        {
            objStage->setIsRunning(true);
            if(objParty != nullptr)
            {
                objParty->setInfoStage(objStage);
            }
            else
            {
                const auto objFriend = getInfoFriendFromPartyIdx(nParty);
                if(objFriend == nullptr)
                {
                    const auto obj = InfoAdventureFriend::create();
                    obj->setUseridxFromPartyIdx(nParty);
                    obj->setInfoStage(objStage);
                           
                    _listInfoFriend.pushBack(obj);
                }
                else
                    objFriend->setInfoStage(objStage);
            }
        }
    }
    
    if(jsonParser.HasMember("_reward"))
    {
        std::string strReward = jsonParser["_reward"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
    }
    
    if(jsonParser.HasMember("_items"))
    {
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->setItemsServer(strItems);
    }
    
    if ( _callbackAdventureRunning != nullptr )
    {
        _callbackAdventureRunning(true, 1);
    }
    
    _callbackAdventureRunning = nullptr;
    requestMissionInfo(nullptr);
}

void AdventureManager::requestAdventureReward(const std::function<void(bool, int)>& callback, const int nParty, bool isFriend)
{
    _callbackAdventureReward = callback;
    _isServerDispatch = true;
    
    //
    const std::string url = "/adventure/FinishAdventureArea";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    
    std::vector<std::pair<int, double>> listOther;
    if(isFriend)
    {
        auto objFriend = getInfoFriendFromPartyIdx(nParty);
        auto objStage = objFriend->getInfoStage();
        
        std::vector<MafHttpModelNew*> ArrayOfficeList;
        std::vector<MafHttpModelNew*> ArrayMemberList;
        req->setDataObjecArray("_offices", ArrayOfficeList);
        req->setDataObjecArray("_crews", ArrayMemberList);
        req->setData("_category", objStage->getArea());
        req->setData("_stage", objStage->getIdx());
        
        for(const auto obj : objFriend->getListEffect())
        {
            listOther.push_back(std::make_pair((int)obj.first, obj.second));
        }
    }
    else
    {
        const auto objParty = getInfoParty(nParty);
        if(objParty == nullptr || !objParty->isRunning())
        {
            if(_callbackAdventureReward != nullptr)
                _callbackAdventureReward(false, 0);
            _callbackAdventureReward = nullptr;
            _isServerDispatch = false;
            return;
        }
        
        req->setData("_category", objParty->getArea());
        req->setData("_stage", objParty->getStage());
        
        std::vector<MafHttpModelNew*> ArrayFurnitureList;
        for(const auto info : _listInfoFurniture)
        {
            std::vector<MafHttpModelNew*> ObjList;
            MafHttpModelNew *val1 = new MafHttpModelNew();
            MafHttpModelNew *val2 = new MafHttpModelNew();
            val1->setData("idx", info->getIdx());
            val2->setData("lv", info->getCurrentLv());
            ObjList.push_back(val1);
            ObjList.push_back(val2);
            MafHttpModelNew *valObj = new MafHttpModelNew();
            valObj->setData("_offices", ObjList, true);
            ArrayFurnitureList.push_back(valObj);
        }
        req->setDataObjecArray("_offices", ArrayFurnitureList);
        
        std::vector<MafHttpModelNew*> ArrayMemberList;
        for(const auto info : objParty->getListMemeber())
        {
            std::vector<MafHttpModelNew*> ObjList;
            MafHttpModelNew *val1 = new MafHttpModelNew();
            MafHttpModelNew *val2 = new MafHttpModelNew();
            val1->setData("idx", info->getIdx());
            val2->setData("lv", info->getCurrentLv());
            ObjList.push_back(val1);
            ObjList.push_back(val2);
            MafHttpModelNew *valObj = new MafHttpModelNew();
            valObj->setData("_crews", ObjList, true);
            ArrayMemberList.push_back(valObj);
        }
        req->setDataObjecArray("_crews", ArrayMemberList);
        for(const auto info : objParty->getListAblity())
        {
            if(!info->isRewardOption())
                continue;
            
            switch (info->getType())
            {
                case E_ADVENTURE_PROPERTY_TYPE::ALL_PARTY_REWARD_INCREASE:
                case E_ADVENTURE_PROPERTY_TYPE::PARTY_ALL_AREA_REWARD_INCREASE:
                case E_ADVENTURE_PROPERTY_TYPE::PARTY_GEM_REWARD_INCREASE:
                {
                    listOther.push_back(std::make_pair((int)info->getType(), info->getEffect(AdventureManager::getInstance()->getPartyLevel(nParty))));
                }break;
                case E_ADVENTURE_PROPERTY_TYPE::PARTY_GRASS_REWARD_INCREASE:
                {
                    if(objParty->getArea() == 1 || objParty->getArea() == 4)
                        listOther.push_back(std::make_pair((int)info->getType(), info->getEffect(AdventureManager::getInstance()->getPartyLevel(nParty))));
                }break;
                case E_ADVENTURE_PROPERTY_TYPE::PARTY_SNOW_REWARD_INCREASE:
                {
                    if(objParty->getArea() == 3 || objParty->getArea() == 5)
                        listOther.push_back(std::make_pair((int)info->getType(), info->getEffect(AdventureManager::getInstance()->getPartyLevel(nParty))));
                }break;
                case E_ADVENTURE_PROPERTY_TYPE::PARTY_DESERT_REWARD_INCREASE:
                {
                    if(objParty->getArea() == 2 || objParty->getArea() == 6)
                        listOther.push_back(std::make_pair((int)info->getType(), info->getEffect(AdventureManager::getInstance()->getPartyLevel(nParty))));
                }break;
                case E_ADVENTURE_PROPERTY_TYPE::PARTY_VOLCANO_REWARD_INCREASE:
                {
                    if(objParty->getArea() == 7)
                        listOther.push_back(std::make_pair((int)info->getType(), info->getEffect(AdventureManager::getInstance()->getPartyLevel(nParty))));
                }break;
                default:
                    break;
            }
            
        };
        
        for(const auto info : _listInfoPropertyType)
        {
            if(info->isPartyOption() || !info->isRewardOption())
                continue;
            int nType = info->getIdx();
            double dEffect = getPartyMemberPublicEffect(info->getType(), objParty->getIdx());
            if(dEffect > 0)
                listOther.push_back(std::make_pair(nType, dEffect));
            
            dEffect = getPartyAbilityPublicEffect(info->getType(), objParty->getIdx());
            if(dEffect > 0)
                listOther.push_back(std::make_pair(nType, dEffect));
        }
    }
    
    std::vector<MafHttpModelNew*> ArrayPartyAbilityList;
    for(const auto info : listOther)
    {
        std::vector<MafHttpModelNew*> ObjList;
        MafHttpModelNew *val1 = new MafHttpModelNew();
        MafHttpModelNew *val2 = new MafHttpModelNew();
        val1->setData("type", (int)info.first);
        val2->setDataDouble("v", info.second);
        ObjList.push_back(val1);
        ObjList.push_back(val2);
        MafHttpModelNew *valObj = new MafHttpModelNew();
        valObj->setData("_party_abilitys", ObjList, true);
        ArrayPartyAbilityList.push_back(valObj);
    }
    req->setDataObjecArray("_party_abilitys", ArrayPartyAbilityList);
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseAdventureReward(response,dd);
    };
    req->send(cb);
}

void AdventureManager::responseAdventureReward(cocos2d::network::HttpResponse* const response, const std::string &data)
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
        if ( _callbackAdventureReward != nullptr )
        {
            _callbackAdventureReward(false, 0);
        }
        _callbackAdventureReward = nullptr;
        return;
    }
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackAdventureReward != nullptr )
        {
            _callbackAdventureReward(false, nResult);
        }
        _callbackAdventureReward = nullptr;
        return;
    }
    
    _listInfoFriend.clear();
    for ( int i = 0; i < jsonParser["_friend_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_friend_list"][i];
        
        const bool isAdventure = jsonValue["is_adv"].GetBool();
        const int nUserIdx = jsonValue["useridx"].GetInt();
        const int nLevel = jsonValue["level"].GetInt();
        const std::string strPlatform = jsonValue["platform"].GetString();
        const std::string strNickName = jsonValue["nick"].GetString();
        
        const auto obj = InfoAdventureFriend::create();
        obj->setLevel(nLevel);
        obj->setUseridx(nUserIdx);
        obj->setIsAdventure(isAdventure);
        obj->setPlatform(strPlatform);
        obj->setNickName(strNickName);
        
        _listInfoFriend.pushBack(obj);
    }
    
    _nLevel = jsonParser["_level"].GetInt();
    _nExp = jsonParser["_exp"].GetDouble();
    
    resetRunningPartyData();
    
    for ( int i = 0; i < jsonParser["_adventure_area_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_adventure_area_list"][i];
        
        const int nArea = jsonValue["category"].GetInt();
        const int nStage = jsonValue["stage"].GetInt();
        const int nParty = jsonValue["party"].GetInt();
        const int nEndTime = jsonValue["end_time"].GetInt();
        const bool isReward = jsonValue["is_reward"].GetBool();
        
        const auto objParty = getInfoParty(nParty);
        const auto objStage = getInfoStage(nArea, nStage);
        
        if(objStage == nullptr)
            continue;
        
        objStage->setEndTime(nEndTime);
        objStage->setIsComplete(isReward);
        if(!isReward)
        {
            objStage->setIsRunning(true);
            if(objParty != nullptr)
            {
                objParty->setInfoStage(objStage);
            }
            else
            {
                const auto objFriend = getInfoFriendFromPartyIdx(nParty);
                if(objFriend == nullptr)
                {
                    const auto obj = InfoAdventureFriend::create();
                    obj->setUseridxFromPartyIdx(nParty);
                    obj->setInfoStage(objStage);
                           
                    _listInfoFriend.pushBack(obj);
                }
                else
                    objFriend->setInfoStage(objStage);
            }
        }
    }
    // reward
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    ItemsManager::getInstance()->addItems(strReward);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    bool isTutorialComplete = UserDefault::getInstance()->getIntegerForKey(KEY_ADVT_TUTORIAL_NEW_CORE, 0);
    
    if(AdventureRelicManager::getInstance()->getCountActiveAreaCore() == 2 && !isTutorialComplete)
    {
        auto popup = PopupAdventureTutorial::create(7);
        popup->show();
        
        UserDefault::getInstance()->setIntegerForKey(KEY_ADVT_TUTORIAL_NEW_CORE, 1);
    }
    //
    
    isTutorialComplete = UserDefault::getInstance()->getIntegerForKey(KEY_ADVT_TUTORIAL_FRIEND, 0);
    int nFriendOpenLv = -1;
    auto objSettingFriendLv = getSetting("advt_open_lv_friend");
    if(objSettingFriendLv != nullptr)
        nFriendOpenLv = objSettingFriendLv->getValueToInt();
    
    if(_nLevel == nFriendOpenLv && !isTutorialComplete)
    {
        auto popup = PopupAdventureTutorial::create(8);
        popup->show();
        
        UserDefault::getInstance()->setIntegerForKey(KEY_ADVT_TUTORIAL_FRIEND, 1);
    }
    
    if ( _callbackAdventureReward != nullptr )
    {
        _callbackAdventureReward(true, 1);
    }
    
    _callbackAdventureReward = nullptr;
    
    requestInfo(nullptr);
    requestMissionInfo(nullptr);
}

void AdventureManager::requestAdventureTimeReduction(const std::function<void(bool, int)>& callback, InfoAdventureStage* const infoStage, const int nPrice)
{
    _callbackAdventureTimeReduction = callback;
    _isServerDispatch = true;
    
    if(infoStage == nullptr)
    {
        if(_callbackAdventureTimeReduction != nullptr)
            _callbackAdventureTimeReduction(false, 0);
        _callbackAdventureTimeReduction = nullptr;
        return;
    }
    
    //
    const std::string url = "/adventure/SkipAdventureArea";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_category", infoStage->getArea());
    req->setData("_stage", infoStage->getIdx());
    req->setData("_coin", nPrice);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseAdventureTimeReduction(response,dd);
    };
    req->send(cb);
}

void AdventureManager::responseAdventureTimeReduction(cocos2d::network::HttpResponse* const response, const std::string &data)
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
        if ( _callbackAdventureTimeReduction != nullptr )
        {
            _callbackAdventureTimeReduction(false, 0);
            _callbackAdventureTimeReduction = nullptr;
        }
        return;
    }
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackAdventureTimeReduction != nullptr )
        {
            _callbackAdventureTimeReduction(false, nResult);
            _callbackAdventureTimeReduction = nullptr;
        }
        return;
    }
    
    resetRunningPartyData();
    for ( int i = 0; i < jsonParser["_adventure_area_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_adventure_area_list"][i];
        
        const int nArea = jsonValue["category"].GetInt();
        const int nStage = jsonValue["stage"].GetInt();
        const int nParty = jsonValue["party"].GetInt();
        const int nEndTime = jsonValue["end_time"].GetInt();
        const bool isReward = jsonValue["is_reward"].GetBool();
        
        const auto objParty = getInfoParty(nParty);
        const auto objStage = getInfoStage(nArea, nStage);
        
        if(objStage == nullptr)
            continue;
        
        objStage->setEndTime(nEndTime);
        objStage->setIsComplete(isReward);
        if(!isReward)
        {
            objStage->setIsRunning(true);
            if(objParty != nullptr)
            {
                objParty->setInfoStage(objStage);
            }
            else
            {
                const auto objFriend = getInfoFriendFromPartyIdx(nParty);
                if(objFriend != nullptr)
                    objFriend->setInfoStage(objStage);
            }
        }
    }
    
    // reward
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    ItemsManager::getInstance()->addItems(strReward);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    if ( _callbackAdventureTimeReduction != nullptr )
    {
        _callbackAdventureTimeReduction(true, 1);
    }
    
    _callbackAdventureTimeReduction = nullptr;
}

void AdventureManager::requestObtainGummie(const std::function<void(bool, int)>& callback)
{
    _callbackObtainGummie = callback;
    //
    const std::string url = "/adventure/GiftAdventureGummie";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseObtainGummie(response,dd);
    };
    req->send(cb);
}

void AdventureManager::responseObtainGummie(cocos2d::network::HttpResponse* const response, const std::string &data)
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
        if ( _callbackObtainGummie != nullptr )
        {
            _callbackObtainGummie(false, 0);
            _callbackObtainGummie = nullptr;
        }
        return;
    }
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackObtainGummie != nullptr )
        {
            _callbackObtainGummie(false, nResult);
            _callbackObtainGummie = nullptr;
        }
        return;
    }
    
    PrisonManager::getInstance()->setGummy(1);
    
    // reward
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    ItemsManager::getInstance()->addItems(strReward);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    if ( _callbackObtainGummie != nullptr )
    {
        _callbackObtainGummie(true, 1);
    }
    
    _callbackObtainGummie = nullptr;
}

void AdventureManager::requestMissionInfo(const std::function<void(bool, int)>& callback)
{
    _callbackMissionInfo = callback;
    std::string url = "/adventure/mission/list/%d";
    _isServerDispatch = true;
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseMissionInfo(response,dd);
    };
    req->send(cb);
}

void AdventureManager::responseMissionInfo(cocos2d::network::HttpResponse* const response, const std::string &data)
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
        if ( _callbackMissionInfo != nullptr )
        {
            _callbackMissionInfo(false, 0);
            _callbackMissionInfo = nullptr;
        }
        _eventMission();
        return;
    }
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackMissionInfo != nullptr )
        {
            _callbackMissionInfo(false, nResult);
            _callbackMissionInfo = nullptr;
        }
        _eventMission();
        return;
    }
    
    for ( int i = 0; i < jsonParser["_mission_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_mission_list"][i];
        
        const bool isComplete = jsonValue["is_complete"].GetBool();
        const int nIdx = jsonValue["idx"].GetInt();
        const int nType = jsonValue["type"].GetInt();
        const int nMissionType = jsonValue["task_type"].GetInt();
        const int nMissionMaxCount = jsonValue["task_count"].GetInt();
        const int nMissionCurrentCount = jsonValue["cureent_task_count"].GetInt();
        
        auto objMission = getInfoMission(nType, nIdx);
        if(objMission == nullptr)
            continue;
        
        objMission->setReceive(isComplete);
        objMission->setType(nMissionType);
        objMission->setCountNow(nMissionCurrentCount);
        objMission->setCountMax(nMissionMaxCount);
    }
    
    for ( int i = 0; i < jsonParser["_week_mission_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_week_mission_list"][i];
        
        const bool isComplete = jsonValue["is_complete"].GetBool();
        const int nIdx = jsonValue["idx"].GetInt();
        const int nType = jsonValue["type"].GetInt();
        const int nMissionType = jsonValue["task_type"].GetInt();
        const int nMissionMaxCount = jsonValue["task_count"].GetInt();
        const int nMissionCurrentCount = jsonValue["cureent_task_count"].GetInt();
        
        auto objMission = getInfoMission(nType, nIdx);
        if(objMission == nullptr)
            continue;
        
        objMission->setReceive(isComplete);
        objMission->setType(nMissionType);
        objMission->setCountNow(nMissionCurrentCount);
        objMission->setCountMax(nMissionMaxCount);
    }
    
    _eventMission();
    if ( _callbackMissionInfo != nullptr )
    {
        _callbackMissionInfo(true, 1);
    }
    
    _callbackMissionInfo = nullptr;
}

void AdventureManager::requestMissionComplete(const std::function<void(bool, int)>& callback, E_ADVENTURE_MISSION_GROUP eType, int nIdx)
{
    //
    _callbackMissionComplete = callback;
    const std::string url = "/adventure/RewardAdventureMission";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", (int)eType);
    req->setData("_idx", nIdx);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseMissionComplete(response,dd);
    };
    req->send(cb);
}

void AdventureManager::responseMissionComplete(cocos2d::network::HttpResponse* const response, const std::string &data)
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
        if ( _callbackMissionComplete != nullptr )
        {
            _callbackMissionComplete(false, 0);
            _callbackMissionComplete = nullptr;
        }
        _eventMission();
        return;
    }
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackMissionComplete != nullptr )
        {
            _callbackMissionComplete(false, nResult);
            _callbackMissionComplete = nullptr;
        }
        _eventMission();
        return;
    }
    
    for ( int i = 0; i < jsonParser["_mission_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_mission_list"][i];
        
        const bool isComplete = jsonValue["is_complete"].GetBool();
        const int nIdx = jsonValue["idx"].GetInt();
        const int nType = jsonValue["type"].GetInt();
        const int nMissionType = jsonValue["task_type"].GetInt();
        const int nMissionMaxCount = jsonValue["task_count"].GetInt();
        const int nMissionCurrentCount = jsonValue["cureent_task_count"].GetInt();
        
        auto objMission = getInfoMission(nType, nIdx);
        if(objMission == nullptr)
            continue;
        
        objMission->setReceive(isComplete);
        objMission->setType(nMissionType);
        objMission->setCountNow(nMissionCurrentCount);
        objMission->setCountMax(nMissionMaxCount);
    }
    
    for ( int i = 0; i < jsonParser["_week_mission_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_week_mission_list"][i];
        
        const bool isComplete = jsonValue["is_complete"].GetBool();
        const int nIdx = jsonValue["idx"].GetInt();
        const int nType = jsonValue["type"].GetInt();
        const int nMissionType = jsonValue["task_type"].GetInt();
        const int nMissionMaxCount = jsonValue["task_count"].GetInt();
        const int nMissionCurrentCount = jsonValue["cureent_task_count"].GetInt();
        
        auto objMission = getInfoMission(nType, nIdx);
        if(objMission == nullptr)
            continue;
        
        objMission->setReceive(isComplete);
        objMission->setType(nMissionType);
        objMission->setCountNow(nMissionCurrentCount);
        objMission->setCountMax(nMissionMaxCount);
    }
    
    // reward
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    ItemsManager::getInstance()->addItems(strReward);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    if ( _callbackMissionComplete != nullptr )
    {
        _callbackMissionComplete(true, 1);
        _callbackMissionComplete = nullptr;
    }
    _eventMission();
}

void AdventureManager::requestMissionAllComplete(const std::function<void(bool, int)>& callback)
{
    //
    _callbackMissionAllComplete = callback;
    const std::string url = "/adventure/RewardALLAdventureMission";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseMissionAllComplete(response,dd);
    };
    req->send(cb);
}

void AdventureManager::responseMissionAllComplete(cocos2d::network::HttpResponse* const response, const std::string &data)
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
        if ( _callbackMissionAllComplete != nullptr )
        {
            _callbackMissionAllComplete(false, 0);
            _callbackMissionAllComplete = nullptr;
        }
        _eventMission();
        return;
    }
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        if ( _callbackMissionAllComplete != nullptr )
        {
            _callbackMissionAllComplete(false, nResult);
            _callbackMissionAllComplete = nullptr;
        }
        _eventMission();
        return;
    }
    
    for ( int i = 0; i < jsonParser["_mission_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_mission_list"][i];
        
        const bool isComplete = jsonValue["is_complete"].GetBool();
        const int nIdx = jsonValue["idx"].GetInt();
        const int nType = jsonValue["type"].GetInt();
        const int nMissionType = jsonValue["task_type"].GetInt();
        const int nMissionMaxCount = jsonValue["task_count"].GetInt();
        const int nMissionCurrentCount = jsonValue["cureent_task_count"].GetInt();
        
        auto objMission = getInfoMission(nType, nIdx);
        if(objMission == nullptr)
            continue;
        
        objMission->setReceive(isComplete);
        objMission->setType(nMissionType);
        objMission->setCountNow(nMissionCurrentCount);
        objMission->setCountMax(nMissionMaxCount);
    }
    
    for ( int i = 0; i < jsonParser["_week_mission_list"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["_week_mission_list"][i];
        
        const bool isComplete = jsonValue["is_complete"].GetBool();
        const int nIdx = jsonValue["idx"].GetInt();
        const int nType = jsonValue["type"].GetInt();
        const int nMissionType = jsonValue["task_type"].GetInt();
        const int nMissionMaxCount = jsonValue["task_count"].GetInt();
        const int nMissionCurrentCount = jsonValue["cureent_task_count"].GetInt();
        
        auto objMission = getInfoMission(nType, nIdx);
        if(objMission == nullptr)
            continue;
        
        objMission->setReceive(isComplete);
        objMission->setType(nMissionType);
        objMission->setCountNow(nMissionCurrentCount);
        objMission->setCountMax(nMissionMaxCount);
    }
    
    // reward
    std::string strReward         = jsonParser["_reward"].GetString();
    std::string strItems         = jsonParser["_items"].GetString();
    
    ItemsManager::getInstance()->addItems(strReward);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    if ( _callbackMissionAllComplete != nullptr )
    {
        _callbackMissionAllComplete(true, 1);
        _callbackMissionAllComplete = nullptr;
    }
    _eventMission();
}

void AdventureManager::requestBossSelect(int idx, const std::function<void(bool, int)>& callback)
{
    _isServerDispatch = true;
    
    //
    _callbackBossSelect = callback;
    
    //
    const std::string url = "/adventure/bossChoice";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_boss_idx", idx);
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseBossSelect(response,dd);
    };
    req->send(cb);
}
void AdventureManager::responseBossSelect(cocos2d::network::HttpResponse* const response, const std::string &data)
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
        if ( _callbackBossSelect != nullptr )
        {
            _callbackBossSelect(false, -1);
            _callbackBossSelect = nullptr;
        }
        return;
    }
     
    /*
    10001: 이미 클리어한 보스
    10005: 공격볼?? 부족
    10007: 잘못된값 요청 (보스정보가없거나 DB에 저장된정보와 다른정보요청시)
     */
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        _nBossIdx = jsonParser["_boss_info"]["idx"].GetInt();
        auto hpNow = jsonParser["_boss_info"]["hp"].GetInt();
        auto weakness1Count = jsonParser["_boss_info"]["weakness1"].GetInt();
        auto weakness2Count = jsonParser["_boss_info"]["weakness2"].GetInt();
        
        auto objInfoBoss = getInfoBoss(_nBossIdx.valueInt());
        if ( objInfoBoss != nullptr )
        {
            objInfoBoss->setHpNow(hpNow);
            objInfoBoss->setWeakness1CountNow(weakness1Count);
            objInfoBoss->setWeakness2CountNow(weakness2Count);
        }
        
        //
        if ( _callbackBossSelect != nullptr )
        {
            _callbackBossSelect(true, nResult);
            _callbackBossSelect = nullptr;
        }
    }
    else
    {
        if ( _callbackBossSelect != nullptr )
        {
            _callbackBossSelect(false, nResult);
            _callbackBossSelect = nullptr;
        }
    }
}

void AdventureManager::requestBossAttack(int idx, int itemIdx, int itemCount, const std::function<void(bool, int)>& callback)
{
    _isServerDispatch = true;
    
    //
    _callbackBossAttack = callback;
    
    //
    const std::string url = "/adventure/bossAttack";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_boss_idx", idx);
    req->setData("_attack_idx", itemIdx);
    req->setData("_attack_count", itemCount);
    req->setData("_season", getBossSeason());
    
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        const std::string dd = data;
        responseBossAttack(response,dd);
    };
    req->send(cb);
}
void AdventureManager::responseBossAttack(cocos2d::network::HttpResponse* const response, const std::string &data)
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
        if ( _callbackBossAttack != nullptr )
        {
            _callbackBossAttack(false, -1);
            _callbackBossAttack = nullptr;
        }
        return;
    }
     
    /*
    10001: 이미 클리어한 보스
    10005: 공격볼?? 부족
    10007: 잘못된값 요청 (보스정보가없거나 DB에 저장된정보와 다른정보요청시)
     */
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        auto hpNow = jsonParser["_boss_info"]["hp"].GetInt();
        auto weakness1Count = jsonParser["_boss_info"]["weakness1"].GetInt();
        auto weakness2Count = jsonParser["_boss_info"]["weakness2"].GetInt();

        auto objInfoBoss = getInfoBoss(_nBossIdx.valueInt());
        if ( objInfoBoss != nullptr )
        {
            objInfoBoss->setHpNow(hpNow);
            objInfoBoss->setWeakness1CountNow(weakness1Count);
            objInfoBoss->setWeakness2CountNow(weakness2Count);
        }
        
        if ( jsonParser.HasMember("_att_point") == true )
        {
            ItemsManager::getInstance()->setItemsServer(E_ITEMS::ADVENTURE_BOSS_BULLET_FOREST, 0, jsonParser["_att_point"]["117"].GetInt()); //117,모험단 보스 속성탄[숲]
            ItemsManager::getInstance()->setItemsServer(E_ITEMS::ADVENTURE_BOSS_BULLET_EARTH, 0, jsonParser["_att_point"]["118"].GetInt()); //118,모험단 보스 속성탄[대지]
            ItemsManager::getInstance()->setItemsServer(E_ITEMS::ADVENTURE_BOSS_BULLET_GLACIER, 0, jsonParser["_att_point"]["119"].GetInt()); //119,모험단 보스 속성탄[얼음]
            ItemsManager::getInstance()->setItemsServer(E_ITEMS::ADVENTURE_BOSS_BULLET_VOLCANO, 0, jsonParser["_att_point"]["120"].GetInt()); //120,모험단 보스 속성탄[화산]
            ItemsManager::getInstance()->setItemsServer(E_ITEMS::ADVENTURE_BOSS_BULLET_ALL, 0, jsonParser["_att_point"]["121"].GetInt()); //121,모험단 보스 속성탄[만능]
        }
        
        if ( jsonParser.HasMember("_reward") )
        {
            std::string strReward = jsonParser["_reward"].GetString();
            ItemsManager::getInstance()->addItems(strReward);
        }
        
        if ( jsonParser.HasMember("_items") )
        {
            std::string strItems = jsonParser["_items"].GetString();
            ItemsManager::getInstance()->setItemsServer(strItems);
        }
        
        //
        if ( _callbackBossAttack != nullptr )
        {
            _callbackBossAttack(true, nResult);
            _callbackBossAttack = nullptr;
        }
    }
    else
    {
        if ( _callbackBossAttack != nullptr )
        {
            _callbackBossAttack(false, nResult);
            _callbackBossAttack = nullptr;
        }
    }
}

#pragma mark - delegate
void AdventureManager::subMissionEvent(MafFunction<void()>* func)
{
    _eventMission += func;
}

void AdventureManager::unSubMissionEvent(void* obj)
{
    _eventMission.Clear(obj);
}
