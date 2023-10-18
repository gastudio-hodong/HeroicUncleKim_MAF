//
//  DataManager.cpp
//  HexagonSnake
//
//  Created by SanHeo on 2014. 8. 6..
//
//

#include "DataManager.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/UserInfoStackManager.h"
#include "ManagerGame/UserInfoConfigManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/WeaponSpiritManager.h"
#include "ManagerGame/AdventureRelicManager.h"
#include "ManagerGame/TownManager.h"
#include "ManagerGame/WeaponManager.h"
#include "ManagerGame/StoneManager.h"

USING_NS_CC;

static std::map<int, std::string> s_MonsterHpInfo[C_COUNT_PLACE];

static double _saveArtifactPrice[3][C_COUNT_ARTIFACT+1] = {0,};
static double _saveArtifactPriceLevel[3][C_COUNT_ARTIFACT+1] = {0,};

static double _saveArtifactEffect[C_COUNT_ARTIFACT+1] = {0,};
static double _saveArtifactLevel[C_COUNT_ARTIFACT+1] = {0,};

static double _saveRelicEffect[C_COUNT_RELIC+1] = {0,};
static double _saveRelicLevel[C_COUNT_RELIC+1] = {0,};

#pragma mark-
DataManager::DataManager()
{
    
}

DataManager::~DataManager(void)
{
    
}

bool DataManager::init()
{
    return true;
}

#pragma mark -
void DataManager::loadArtifactData()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::ARTIFACT);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)(const char*)strText.c_str());
    if( jsonParser.HasParseError() )
    {
        return;
    }
    
    _mapArtifact.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue[("version")].GetString()) == false )
        {
            continue;
        }
        
        E_ARTIFACT _idx = (E_ARTIFACT)jsonValue[("idx")].GetInt();
        E_ARTIFACT_TYPE _type = (E_ARTIFACT_TYPE)jsonValue[("type")].GetInt();
        
        std::string _title = jsonValue["name_key"].GetString();
        std::string _desc = jsonValue["comment_key"].GetString();
        std::string _iconPath = std::string("Assets/icon_artifact/").append(jsonValue["icon_path"].GetString());
        
        int _generation = jsonValue[("generation")].GetInt();
        int _condition = jsonValue.HasMember("condition") ? jsonValue[("condition")].GetInt() : 0;
        int _levelMax = jsonValue[("level_max")].GetInt();
        int _levelReinforceMax = jsonValue[("level_reinforce_max")].GetInt();
        
        E_ITEMS _price_type = enumFromInt<E_ITEMS>(jsonValue["price_type_idx"].GetInt());
        int _price_point = jsonValue[("price_point")].GetInt();
        
        int _prev_artifact = jsonValue[("prev_artifact")].GetInt();
        
        int _is_calculator = jsonValue[("is_calculator")].GetInt();
        int _is_reinforce = jsonValue[("is_reinforce")].GetInt();
        
        //
        auto objInfo = new InfoArtifact();
        objInfo->setIdx(_idx);
        objInfo->setType(_type);
        
        objInfo->setTitleKey(_title);
        objInfo->setDescKey(_desc);
        objInfo->setIconPath(_iconPath);
        
        objInfo->setGeneration(_generation);
        objInfo->setCondition(_condition);
        objInfo->setLevelMax(_levelMax);
        objInfo->setLevelReinforceMax(_levelReinforceMax);
        
        
        objInfo->setPriceType(_price_type);
        objInfo->setPricePoint(_price_point);
        
        objInfo->setPrevArtifact(_prev_artifact);
        
        objInfo->setCalculator(_is_calculator);
        objInfo->setReinforce(_is_reinforce);
    
        _mapArtifact.insert(_idx, objInfo);
    }
}

void DataManager::LoadDevilArtifactData(E_D_ARTIFACT_TYPE type)
{
    if(getInstance()->_vecDevilArtifact[type].size()>0)
        return;
    
    std::string txt = FileUtils::getInstance()->getStringFromFile(DATA_FILE_D2_ARTIFACT);
    ModelInfoDevil::DevilArtifactInfo *info;
    rapidjson::Document jsondoc;
    jsondoc.ParseInsitu<0>((char*)(const char*)txt.c_str()).HasParseError();
    
    auto iter = jsondoc.Begin();
    for(; iter != jsondoc.End(); iter++){
        const rapidjson::Value& oneItem = *iter;
        info = new ModelInfoDevil::DevilArtifactInfo();
        //            info->_type = cType;
        info->_type = (E_D_ARTIFACT_TYPE)oneItem[("_type")].GetInt();
        if(info->_type !=type)
        {
            info->release();
            continue;
        }
        
        info->_idx  = oneItem[("_idx")].GetInt();
        info->_price = oneItem[("_price")].GetDouble();
        info->_effect = oneItem[("_effect")].GetDouble();
        
        
        std::string strKeyTitle = MafUtils::format("t_arti_d2_title_%d", info->_idx);
        info->_title = GAME_TEXT(strKeyTitle);
        
        std::string strKeyContents = MafUtils::format("t_arti_d2_contents_%d", info->_idx);
        info->_text  = GAME_TEXT(strKeyContents);

        getInstance()->_vecDevilArtifact[info->_type].pushBack(info);
        info->release();
    }
    
}


#pragma mark-
std::string DataManager::GetMonsterHp(E_PLACE ePlace, int nFloor)
{
    if ( nFloor == 0 )
    {
        nFloor = UserInfoManager::getInstance()->getFloor(ePlace);
    }
    
    int nFloorOriginal = nFloor;
    std::string sHp = "500";
    int cur  = 0;
    
    auto find = s_MonsterHpInfo[ePlace].find(nFloor);
    if ( find != s_MonsterHpInfo[ePlace].end() )
    {
        return find->second;
    }
    
    find = s_MonsterHpInfo[ePlace].find(nFloor-1);
    if ( nFloor >= 1000 && find != s_MonsterHpInfo[ePlace].end() )
    {
        sHp = find->second;
        
        sHp = MafUtils::bigMulNum(sHp, "105");
        sHp.pop_back();
        sHp.pop_back();

        s_MonsterHpInfo[ePlace].insert( std::pair<int, std::string>(nFloorOriginal, sHp));
        return sHp;
    }
    
    while (true) {
        double cal =0;
        double per = 1.15;
        if(cur==0)      per = 1.15;
        else if(cur==1) per = 1.10;
        else if(cur==2) per = 1.08;
        else if(cur<=4) per = 1.07;
        else            per = 1.05;
        
        if(nFloor>=FLOOR_100){
            cal = pow(per, 100);
            
            cal*=100;
            std::string strCal = MafUtils::doubleToString(cal);
            sHp = MafUtils::bigMulNum(sHp, strCal);
            sHp.pop_back();
            sHp.pop_back();
            
            cur += 1;
            nFloor-= FLOOR_100;
        }
        else if(nFloor==0){
            break;
        }
        else {
            cal = pow(per, nFloor);
            cal*=100;
            std::string strCal = MafUtils::doubleToString(cal);
            sHp = MafUtils::bigMulNum(sHp, strCal);
            sHp.pop_back();
            sHp.pop_back();
            break;
        }
    }

    s_MonsterHpInfo[ePlace].insert( std::pair<int, std::string>(nFloorOriginal, sHp));
    return sHp;
}


std::string DataManager::GetMonsterHpDevil()
{
    int floor = UserInfoManager::getInstance()->getFloor(E_PLACE::DG_DEVIL_DOM);
    int originalFloor = floor;
    std::string sHp = "100";
    int cur  = 0;
    
    auto find = s_MonsterHpInfo[E_PLACE::DG_DEVIL_DOM].find(floor);
    if(find != s_MonsterHpInfo[E_PLACE::DG_DEVIL_DOM].end()){
        return find->second;
    }
    
    while (true) {
        double cal =0;
        double per = 1.05;
        
        if ( floor >= FLOOR_100 )
        {
            cal = pow(per, 100);
            
            cal*=100;
            std::string strCal = MafUtils::doubleToString(cal);
            sHp = MafUtils::bigMulNum(sHp, strCal);
            sHp.pop_back();
            sHp.pop_back();
            
            cur += 1;
            floor-= FLOOR_100;
        }
        else if(floor==0){
            break;
        }
        else {
            cal = pow(per, floor);
            cal*=100;
            std::string strCal = MafUtils::doubleToString(cal);
            sHp = MafUtils::bigMulNum(sHp, strCal);
            sHp.pop_back();
            sHp.pop_back();
            break;
        }
    }
    
    s_MonsterHpInfo[E_PLACE::DG_DEVIL_DOM].insert( std::pair<int, std::string>(originalFloor, sHp));
    
    return sHp;
}

std::string DataManager::GetMonsterHpPrison(int kill){
    int floor = kill;
    double hp = 500;
    std::string sHp = "1";
    
    int cur  = 0;
    int newFloor = floor-1;
    
    while (true) {
        
        double per = 1.1;
        if(cur==0)      per = 1.1;
        else if(cur==1) per = 1.08;
        else if(cur==2) per = 1.06;
        else            per = 1.05;
        
        if(newFloor>=FLOOR_100){
            double cal = pow(per, 19);
            cal = cal/2;
            cal = pow(cal,5);
            cal = std::atof(MafUtils::format("%.1lf",cal).c_str());
            
            hp = hp * cal;
//            if (hp > std::pow(2, 1000)) {
            if (hp > HP_CALCULATION) {
                sHp = MafUtils::bigMulNum(sHp, MafUtils::doubleToString(hp));
                hp = 1;
            }
            cur += 1;
            newFloor-= FLOOR_100;
        }
        else if(newFloor==0){
            break;
        }
        else {
            
            int val = newFloor/20;
            int rest= newFloor%20;
            
            if(val>0){
                double cal = pow(per, 19);
                cal = cal/2;
                cal = pow(cal,val);
                cal = std::atof(MafUtils::format("%.1lf",cal).c_str());
                
                hp = hp * cal;
//                if (hp > std::pow(2, 1000)) {
                if (hp > HP_CALCULATION) {
                    sHp = MafUtils::bigMulNum(sHp, MafUtils::doubleToString(hp));
                    hp = 1;
                }
            }
            
            if(rest>0){
                double one = pow(per,rest);
                hp = hp * one;
//                if (hp > std::pow(2, 1000)) {
                if (hp > HP_CALCULATION) {
                    sHp = MafUtils::bigMulNum(sHp, MafUtils::doubleToString(hp));
                    hp = 1;
                }
            }
            break;
        }
    }

    if(sHp.size()>1){
        sHp = MafUtils::bigMulNum(sHp, MafUtils::doubleToString(hp));
    }
    else{
        sHp = MafUtils::doubleToString(hp);
    }
//    if (hp > 1) {
//        sHp = MafUtils::bigMulNum(sHp, MafUtils::doubleToString(hp));
//    }
    
    //hp 0.4로 감소
    sHp = MafUtils::bigMulNum(sHp, MafUtils::toString(4));
    sHp.pop_back();
    
    return sHp;
}


#pragma mark - quest
std::string DataManager::GetQuestName(E_PLACE ePlace, E_QUEST eQuest)
{
    
    std::string strKey;
    if ( ePlace == E_PLACE::DG_NORMAL )
    {
        strKey = MafUtils::format("t_quest_d1_title_%d", eQuest);
    }
    else if ( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        strKey = MafUtils::format("t_quest_d2_title_%d", eQuest);
    }
    
    std::string strText = "";
    if ( strKey.length() != 0 )
    {
        strText = GAME_TEXT(strKey);
    }
    return strText;
}

//퀘스트 골드 주는 곳 : 퀘스트 골드 디폴트
std::string DataManager::GetQuestEarnMoney(E_PLACE ePlace, E_QUEST eQuest, int nLevel)
{
    double nGold = 0;
    if ( ePlace == E_PLACE::DG_NORMAL )
    {
        if     (eQuest == E_QUEST::QUEST_1)   nGold   = 10.0;
        else if(eQuest == E_QUEST::QUEST_2)   nGold  = 240.0;
        else if(eQuest == E_QUEST::QUEST_3)   nGold  = 4200.0;
        else if(eQuest == E_QUEST::QUEST_4)   nGold  = 68000.0;
        else if(eQuest == E_QUEST::QUEST_5)   nGold  = 2000000.0;
        else if(eQuest == E_QUEST::QUEST_6)   nGold  = 40000000.0;
        else if(eQuest == E_QUEST::QUEST_7)   nGold  = 800000000.0;
        else if(eQuest == E_QUEST::QUEST_8)   nGold  = 24000000000.0;
        else if(eQuest == E_QUEST::QUEST_9)   nGold  = 630000000000.0;
        else if(eQuest == E_QUEST::QUEST_10)  nGold = 26000000000000.0;
        else if(eQuest == E_QUEST::QUEST_11)  nGold = 800000000000000.0;
        else if(eQuest == E_QUEST::QUEST_12)  nGold = 3700000000000000.0;
        else if(eQuest == E_QUEST::QUEST_13)  nGold = 110000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_14)  nGold = 7700000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_15)  nGold = 4500000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_16)  nGold = 14000000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_17)  nGold = 5100000000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_18)  nGold = 680000000000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_19)  nGold = 46500000000000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_20)  nGold = 93000000000000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_21)  nGold = 17300000000000000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_22)  nGold = 150000000000000000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_23)  nGold = 7300000000000000000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_24)  nGold = 1030000000000000000000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_25)  nGold = 145800000000000000000000000000000000.0;
        
        if ( nLevel > 0 )
        {
            int tmpLevel = nLevel + DataManager::GetHonorEffect(E_HONOR::HONOR_2, UserInfoManager::getInstance()->getHonorLevel(E_HONOR::HONOR_2));
            if ( tmpLevel > 1 )
            {
                double temp = nGold * 0.5 * (tmpLevel-1); //50%
                nGold += temp;
            }
        }
        
        // 2018년 4월 12일 퀘스트 골드 20% 상향
        nGold += nGold * 0.2;
        
    }
    else if ( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        if     (eQuest == E_QUEST::QUEST_1)   nGold   = 50.0;
        else if(eQuest == E_QUEST::QUEST_2)   nGold  = 300.0;
        else if(eQuest == E_QUEST::QUEST_3)   nGold  = 2000.0;
        else if(eQuest == E_QUEST::QUEST_4)   nGold  = 16800.0;
        else if(eQuest == E_QUEST::QUEST_5)   nGold  = 192000.0;
        else if(eQuest == E_QUEST::QUEST_6)   nGold  = 5760000.0;
        else if(eQuest == E_QUEST::QUEST_7)   nGold  = 138240000.0;
        else if(eQuest == E_QUEST::QUEST_8)   nGold  = 32256000.0;
        else if(eQuest == E_QUEST::QUEST_9)   nGold  = 1548288000.0;
        else if(eQuest == E_QUEST::QUEST_10)  nGold = 46448640000.0;
        else if(eQuest == E_QUEST::QUEST_11)  nGold = 1300561920000.0;
        else if(eQuest == E_QUEST::QUEST_12)  nGold = 40874803200000.0;
        else if(eQuest == E_QUEST::QUEST_13)  nGold = 2942985830400000.0;
        else if(eQuest == E_QUEST::QUEST_14)  nGold = 153035263180800000.0;
        else if(eQuest == E_QUEST::QUEST_15)  nGold = 71416456151040000.0;

        int tmpLevel = nLevel;
        if ( tmpLevel > 1 )
        {
            double temp = nGold * 0.2 * (tmpLevel-1); //20%
            nGold += temp;
        }
    }
    
    std::string strGold = MafUtils::doubleToString(nGold);
    return strGold;
}

double DataManager::GetQuestUpgradeMoney(E_PLACE ePlace, E_QUEST eQuest, int nLevel)
{
    double nGold = 1;
    double upgradePer = 0.0;
    
    if ( ePlace == E_PLACE::DG_NORMAL )
    {
        upgradePer = 1.12;
        if(eQuest == E_QUEST::QUEST_1)nGold = 10.0;
        else if(eQuest == E_QUEST::QUEST_2) nGold = 130.0;
        else if(eQuest == E_QUEST::QUEST_3) nGold = 1800.0;
        else if(eQuest == E_QUEST::QUEST_4) nGold = 27000.0;
        else if(eQuest == E_QUEST::QUEST_5) nGold = 437000.0;
        else if(eQuest == E_QUEST::QUEST_6) nGold = 7420000.0;
        else if(eQuest == E_QUEST::QUEST_7) nGold = 133000000.0;
        else if(eQuest == E_QUEST::QUEST_8) nGold = 2540000000.0;
        else if(eQuest == E_QUEST::QUEST_9) nGold = 50800000000.0;
        else if(eQuest == E_QUEST::QUEST_10) nGold = 1060000000000.0;
        else if(eQuest == E_QUEST::QUEST_11) nGold = 23400000000000.0;
        else if(eQuest == E_QUEST::QUEST_12) nGold = 539000000000000.0;
        else if(eQuest == E_QUEST::QUEST_13) nGold = 12900000000000000.0;
        else if(eQuest == E_QUEST::QUEST_14) nGold = 324000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_15) nGold = 8420000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_16) nGold = 227000000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_17) nGold = 6360000000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_18) nGold = 184000000000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_19) nGold = 5530000000000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_20) nGold = 171000000000000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_21) nGold = 46500000000000000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_22) nGold = 8000000000000000000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_23) nGold = 1780000000000000000000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_24) nGold = 489000000000000000000000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_25) nGold = 110000000000000000000000000000000000000.0;
        
        if ( nLevel == 0 )
            return nGold;
        
        auto temp = std::pow(upgradePer, nLevel);
        nGold = nGold * temp;
    }
    else if ( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        upgradePer = 1.05;
        if     (eQuest == E_QUEST::QUEST_1)    nGold = 100.0;
        else if(eQuest == E_QUEST::QUEST_2)    nGold = 700.0;
        else if(eQuest == E_QUEST::QUEST_3)    nGold = 19600.0;
        else if(eQuest == E_QUEST::QUEST_4)    nGold = 1234800.0;
        else if(eQuest == E_QUEST::QUEST_5)    nGold = 138297600.0;
        else if(eQuest == E_QUEST::QUEST_6)    nGold = 24202080000.0;
        else if(eQuest == E_QUEST::QUEST_7)    nGold = 6098924160000.0;
        else if(eQuest == E_QUEST::QUEST_8)    nGold = 2091930986880000.0;
        else if(eQuest == E_QUEST::QUEST_9)    nGold = 937185082122240000.0;
        else if(eQuest == E_QUEST::QUEST_10)   nGold = 531383941563310000000.0;
        else if(eQuest == E_QUEST::QUEST_11)   nGold = 371968759094317000000000.0;
        else if(eQuest == E_QUEST::QUEST_12)   nGold = 315057538952887000000000000.0;
        else if(eQuest == E_QUEST::QUEST_13)   nGold = 317577999264510000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_14)   nGold = 375694773129915000000000000000000.0;
        else if(eQuest == E_QUEST::QUEST_15)   nGold = 515453228734243000000000000000000000.0;
        
        if ( nLevel == 0 )
            return nGold;
        
        auto temp = std::pow(upgradePer, nLevel);
        nGold = nGold * temp;
    }
    
    return nGold;
}

//퀘스트 골드 주는 곳 : 퀘스트 시간
double DataManager::GetQuestSpendTime(E_PLACE ePlace, E_QUEST eQuest)
{
    double nTime = 1;
    
    if ( ePlace == E_PLACE::DG_NORMAL )
    {
        switch (eQuest) {
            case E_QUEST::QUEST_1:  nTime = 1;                  break;
            case E_QUEST::QUEST_2:  nTime = 2;                  break;
            case E_QUEST::QUEST_3:  nTime = 5;                  break;
            case E_QUEST::QUEST_4:  nTime = 10;                 break;
            case E_QUEST::QUEST_5:  nTime = 30;                 break;
            case E_QUEST::QUEST_6:  nTime = 60;                 break;
            case E_QUEST::QUEST_7:  nTime = 120;                break;
            case E_QUEST::QUEST_8:  nTime = 300;                break;
            case E_QUEST::QUEST_9:  nTime = 600;                break;
            case E_QUEST::QUEST_10: nTime = 1800;               break;
            case E_QUEST::QUEST_11: nTime = 3600;               break;
            case E_QUEST::QUEST_12: nTime = 180;                break;
            case E_QUEST::QUEST_13: nTime = 1800;               break;
            case E_QUEST::QUEST_14: nTime = 7200;               break;
            case E_QUEST::QUEST_15: nTime = 120;                break;
            case E_QUEST::QUEST_16: nTime = 60;                 break;
            case E_QUEST::QUEST_17: nTime = 600;                break;
            case E_QUEST::QUEST_18: nTime = 3600;               break;
            case E_QUEST::QUEST_19: nTime = 10800;              break;
            case E_QUEST::QUEST_20: nTime = 300;                break;
            case E_QUEST::QUEST_21: nTime = 3600;               break;
            case E_QUEST::QUEST_22: nTime = 600;                break;
            case E_QUEST::QUEST_23: nTime = 600;                break;
            case E_QUEST::QUEST_24: nTime = 600;                break;
            case E_QUEST::QUEST_25: nTime = 600;                break;
            default:
                break;
        }
    
    }
    else if ( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        switch (eQuest) {
            case E_QUEST::QUEST_1:  nTime = 1;                  break;
            case E_QUEST::QUEST_2:  nTime = 3;                  break;
            case E_QUEST::QUEST_3:  nTime = 5;                  break;
            case E_QUEST::QUEST_4:  nTime = 7;                  break;
            case E_QUEST::QUEST_5:  nTime = 10;                 break;
            case E_QUEST::QUEST_6:  nTime = 30;                 break;
            case E_QUEST::QUEST_7:  nTime = 60;                 break;
            case E_QUEST::QUEST_8:  nTime = 1;                  break;
            case E_QUEST::QUEST_9:  nTime = 3;                  break;
            case E_QUEST::QUEST_10: nTime = 5;                  break;
            case E_QUEST::QUEST_11: nTime = 7;                  break;
            case E_QUEST::QUEST_12: nTime = 10;                 break;
            case E_QUEST::QUEST_13: nTime = 30;                 break;
            case E_QUEST::QUEST_14: nTime = 60;                 break;
            case E_QUEST::QUEST_15: nTime = 1;                  break;
            default:
                break;
        }
    }

    return nTime;
}
 

#pragma mark - equip
double DataManager::GetEquipUpgradeMoneyDevil(E_EQUIP_IDX idx,int level){
    double money = 100;
    if(level>0){
        auto val = std::pow(1.1,level);
        money *= val;
    }
    return money;
}
std::string DataManager::GetEquipReinforceKeyDevil(E_EQUIP_IDX idx,int reinforcelevel){
    
    //무기단계에 따라 비용이 다르다
    double key = 10;
    if(idx==E_EQUIP_IDX::E_D_IDX_1)
        key = 10.0;
    else if(idx==E_EQUIP_IDX::E_D_IDX_2)
        key = 100.0;
    else if(idx==E_EQUIP_IDX::E_D_IDX_3)
        key = 1000.0;
    else if(idx==E_EQUIP_IDX::E_D_IDX_4)
        key = 10000.0;
    else if(idx==E_EQUIP_IDX::E_D_IDX_5)
        key = 100000.0;
    else if(idx==E_EQUIP_IDX::E_D_IDX_6)
        key = 1000000.0;
    else if(idx==E_EQUIP_IDX::E_D_IDX_7)
        key = 10000000.0;
    else if(idx==E_EQUIP_IDX::E_D_IDX_8)
        key = 100000000.0;
    else if(idx==E_EQUIP_IDX::E_D_IDX_9)
        key = 1000000000.0;
    else if(idx==E_EQUIP_IDX::E_D_IDX_10)
        key = 10000000000.0;
    else if(idx==E_EQUIP_IDX::E_D_IDX_11)
        key = 100000000000.0;
    else if(idx==E_EQUIP_IDX::E_D_IDX_12)
        key = 1000000000000.0;
    else if(idx==E_EQUIP_IDX::E_D_IDX_13)
        key = 10000000000000.0;
    else if(idx==E_EQUIP_IDX::E_D_IDX_14)
        key = 100000000000000.0;
    else if(idx==E_EQUIP_IDX::E_D_IDX_15)
        key = 1000000000000000.0;
    else if(idx==E_EQUIP_IDX::E_D_IDX_16)
        key = 10000000000000000.0;
    else if(idx==E_EQUIP_IDX::E_D_IDX_17)
        key = 100000000000000000.0;
    //강화단계별로 비용증가
    auto per = std::pow(1.10,reinforcelevel);
    key *= per;
    
    std::string strKey = MafUtils::doubleToString(key);
    return strKey;
    
}
stEvolutionDevil DataManager::GetEquipEvolutionSpendDevil(E_EQUIP_IDX idx,int reinforcelevel){
    
    stEvolutionDevil stSpend;
    stSpend.costumeCoin = 200;
    if(idx==E_EQUIP_IDX::E_D_IDX_1){
        stSpend.devilKey = 500;
    }
    else if(idx==E_EQUIP_IDX::E_D_IDX_2){
        stSpend.devilKey = 5000;
    }
    else if(idx==E_EQUIP_IDX::E_D_IDX_3){
        stSpend.devilKey = 50000;
    }
    else if(idx==E_EQUIP_IDX::E_D_IDX_4){
        stSpend.devilKey = 500000;
    }
    else if(idx==E_EQUIP_IDX::E_D_IDX_5){
        stSpend.devilKey = 5000000;
    }
    else if(idx==E_EQUIP_IDX::E_D_IDX_6){
        stSpend.devilKey = 50000000;
    }
    else if(idx==E_EQUIP_IDX::E_D_IDX_7){
        stSpend.devilKey = 500000000;
    }
    else if(idx==E_EQUIP_IDX::E_D_IDX_8){
        stSpend.devilKey = 5000000000;
    }
    else if(idx==E_EQUIP_IDX::E_D_IDX_9){
        stSpend.devilKey = 50000000000;
    }
    else if(idx==E_EQUIP_IDX::E_D_IDX_10){
        stSpend.devilKey = 500000000000;
    }
    else if(idx==E_EQUIP_IDX::E_D_IDX_11){
        stSpend.devilKey = 5000000000000;
    }
    else if(idx==E_EQUIP_IDX::E_D_IDX_12){
        stSpend.devilKey = 50000000000000;
    }
    else if(idx==E_EQUIP_IDX::E_D_IDX_13){
        stSpend.devilKey = 500000000000000;
    }
    else if(idx==E_EQUIP_IDX::E_D_IDX_14){
        stSpend.devilKey = 5000000000000000;
    }
    else if(idx==E_EQUIP_IDX::E_D_IDX_15){
        stSpend.devilKey = 50000000000000000;
    }
    else if(idx==E_EQUIP_IDX::E_D_IDX_16){
        stSpend.devilKey = 500000000000000000;
    }
    else if(idx==E_EQUIP_IDX::E_D_IDX_17){
        stSpend.devilKey = 5000000000000000000;
    }
    
    return stSpend;
}

double DataManager::GetEquipEffectRateDevil(E_EQUIP_TYPE type,E_EQUIP_IDX idx,int level){
    double effect = 0;
    if(level>0){
        double per = 10;
        if(type ==  E_EQUIP_TYPE::E_D_WEAPON){
            per = 10; //무기는 합연산10씩, 나중에 다른곳에서 사용시 주의, 퍼센트 아님!!!!!
        }
        else if(type ==  E_EQUIP_TYPE::E_D_HEAD){
            per = 1;
        }
        else if(type ==  E_EQUIP_TYPE::E_D_BODY){
            per = 0.01;
        }
        else if(type ==  E_EQUIP_TYPE::E_D_CAPE){
            per = 2;
        }
        effect += per*level;
    }
    return effect;
//    std::string streffect = MafUtils::doubleToString(effect);
//    return streffect;
}

double DataManager::GetEquipReinforceRateDevil(int reinforcelevel){
    double rate = 0;
    if(reinforcelevel>0){
        rate = std::pow(1.1,reinforcelevel);
    }
    return rate;
}

double DataManager::GetEquipEvolutionRateDevil(E_EQUIP_IDX idx){
    double rate = 1;
    if(idx>E_EQUIP_IDX::E_D_IDX_1){
        rate = std::pow(7,(int)idx-1);
    }
    return rate;
}

std::string DataManager::GetEquipOptionExplainDevil(E_EQUIP_TYPE type,E_EQUIP_IDX idx,int level,E_EQUIP_OPTION option){
    
    int percent = GetEquipOptionOneRateDevil(type,idx,level,option);
    std::string strExplain;
    if(option == E_EQUIP_OPTION::E_D_DMG)
        strExplain = GAME_TEXTFORMAT("t_equip_d2_option_1", percent);
    else if(option == E_EQUIP_OPTION::E_D_CRITICAL)
        strExplain = GAME_TEXTFORMAT("t_equip_d2_option_2", percent);
    else if(option == E_EQUIP_OPTION::E_D_QUEST_TIME)
        strExplain = GAME_TEXTFORMAT("t_equip_d2_option_3", percent);
    else if(option == E_EQUIP_OPTION::E_D_QUEST_REWARD)
        strExplain = GAME_TEXTFORMAT("t_equip_d2_option_4", percent);
    else if(option == E_EQUIP_OPTION::E_D_SPEED)
        strExplain = GAME_TEXTFORMAT("t_equip_d2_option_5", percent);
    else if(option == E_EQUIP_OPTION::E_D_KEY)
        strExplain = GAME_TEXTFORMAT("t_equip_d2_option_6", percent);
    else if(option == E_EQUIP_OPTION::E_D_CRI_DMG)
        strExplain = GAME_TEXTFORMAT("t_equip_d2_option_7", percent);
    
    return strExplain;
}

int DataManager::GetEquipOptionOneRateDevil(E_EQUIP_TYPE type,E_EQUIP_IDX idx,int level,E_EQUIP_OPTION option){
    int percent = 0; //현재단계에서 전단계의 값을 빼주면 현재단계의 값만 구할수있다.
    int subPercent = 0;
    percent =  GetEquipOptionTotalRateDevil(type,idx,level,option);
    subPercent =  GetEquipOptionTotalRateDevil(type,idx,level-50,option);
    percent -= subPercent;
    return percent;
}

int DataManager::GetEquipOptionTotalRateDevil(E_EQUIP_TYPE type,E_EQUIP_IDX idx,int level,E_EQUIP_OPTION option){
    //key는 아직 미구현했다.열쇠획득량이 없어서
    int percent = 0;
    if(level<50)
        return percent;

    if(type == E_EQUIP_TYPE::E_D_WEAPON){
        if (option == E_EQUIP_OPTION::E_D_CRITICAL) {
            if(idx >= E_EQUIP_IDX::E_D_IDX_1 && level>=50)
                percent +=2;
            if(idx >= E_EQUIP_IDX::E_D_IDX_1 && level>=150)
                percent +=2;
            if(idx >= E_EQUIP_IDX::E_D_IDX_3 && level>=250)
                percent +=2;
            if(idx >= E_EQUIP_IDX::E_D_IDX_5 && level>=350)
                percent +=2;
            if(idx >= E_EQUIP_IDX::E_D_IDX_7 && level>=450)
                percent +=5;
        }
        else if (option == E_EQUIP_OPTION::E_D_DMG) {
            if(idx >= E_EQUIP_IDX::E_D_IDX_1 && level>=100)
                percent +=50;
            if(idx >= E_EQUIP_IDX::E_D_IDX_3 && level>=200)
                percent +=50;
            if(idx >= E_EQUIP_IDX::E_D_IDX_5 && level>=300)
                percent +=50;
            if(idx >= E_EQUIP_IDX::E_D_IDX_7 && level>=400)
                percent +=50;
            if(idx >= E_EQUIP_IDX::E_D_IDX_10 && level>=500)
                percent +=500;
        }
    }
    else if(type == E_EQUIP_TYPE::E_D_HEAD){
        if (option == E_EQUIP_OPTION::E_D_QUEST_TIME) {
            if(idx >= E_EQUIP_IDX::E_D_IDX_1 && level>=50)
                percent +=10;
            if(idx >= E_EQUIP_IDX::E_D_IDX_1 && level>=150)
                percent +=10;
            if(idx >= E_EQUIP_IDX::E_D_IDX_3 && level>=250)
                percent +=10;
            if(idx >= E_EQUIP_IDX::E_D_IDX_5 && level>=350)
                percent +=10;
            if(idx >= E_EQUIP_IDX::E_D_IDX_7 && level>=450)
                percent +=20;
        }
        else if (option == E_EQUIP_OPTION::E_D_QUEST_REWARD) {
            if(idx >= E_EQUIP_IDX::E_D_IDX_1 && level>=100)
                percent +=50;
            if(idx >= E_EQUIP_IDX::E_D_IDX_3 && level>=200)
                percent +=50;
            if(idx >= E_EQUIP_IDX::E_D_IDX_5 && level>=300)
                percent +=50;
            if(idx >= E_EQUIP_IDX::E_D_IDX_7 && level>=400)
                percent +=50;
            if(idx >= E_EQUIP_IDX::E_D_IDX_10 && level>=500)
                percent +=500;
        }
    }
    else if(type == E_EQUIP_TYPE::E_D_BODY){
        if (option == E_EQUIP_OPTION::E_D_SPEED) {
            if(idx >= E_EQUIP_IDX::E_D_IDX_1 && level>=50)
                percent +=5;
            if(idx >= E_EQUIP_IDX::E_D_IDX_1 && level>=150)
                percent +=5;
            if(idx >= E_EQUIP_IDX::E_D_IDX_3 && level>=250)
                percent +=5;
            if(idx >= E_EQUIP_IDX::E_D_IDX_5 && level>=350)
                percent +=5;
            if(idx >= E_EQUIP_IDX::E_D_IDX_7 && level>=450)
                percent +=30;
        }
        else if (option == E_EQUIP_OPTION::E_D_KEY) {
            if(idx >= E_EQUIP_IDX::E_D_IDX_1 && level>=100)
                percent +=5;
            if(idx >= E_EQUIP_IDX::E_D_IDX_3 && level>=200)
                percent +=5;
            if(idx >= E_EQUIP_IDX::E_D_IDX_5 && level>=300)
                percent +=5;
            if(idx >= E_EQUIP_IDX::E_D_IDX_7 && level>=400)
                percent +=5;
            if(idx >= E_EQUIP_IDX::E_D_IDX_10 && level>=500)
                percent +=50;
        }
    }
    else if(type == E_EQUIP_TYPE::E_D_CAPE){
        if (option == E_EQUIP_OPTION::E_D_CRITICAL) {
            if(idx >= E_EQUIP_IDX::E_D_IDX_1 && level>=50)
                percent +=2;
            if(idx >= E_EQUIP_IDX::E_D_IDX_1 && level>=150)
                percent +=2;
            if(idx >= E_EQUIP_IDX::E_D_IDX_3 && level>=250)
                percent +=2;
            if(idx >= E_EQUIP_IDX::E_D_IDX_5 && level>=350)
                percent +=2;
            if(idx >= E_EQUIP_IDX::E_D_IDX_7 && level>=450)
                percent +=5;
        }
        else if (option == E_EQUIP_OPTION::E_D_CRI_DMG) {
            if(idx >= E_EQUIP_IDX::E_D_IDX_1 && level>=100)
                percent +=50;
            if(idx >= E_EQUIP_IDX::E_D_IDX_3 && level>=200)
                percent +=50;
            if(idx >= E_EQUIP_IDX::E_D_IDX_5 && level>=300)
                percent +=50;
            if(idx >= E_EQUIP_IDX::E_D_IDX_7 && level>=400)
                percent +=50;
            if(idx >= E_EQUIP_IDX::E_D_IDX_10 && level>=500)
                percent +=500;
        }
    }
    
    
    
    return percent;
}


#pragma mark-

InfoArtifact* DataManager::GetArtifactInfo(E_ARTIFACT arti)
{
    InfoArtifact* objArtifact = nullptr;
    
    auto find = getInstance()->_mapArtifact.find(arti);
    if ( find != getInstance()->_mapArtifact.end() )
    {
        objArtifact = find->second;
    }
    
    return objArtifact;
}

InfoArtifact* DataManager::GetArtifactInfo(E_ARTIFACT_TYPE type, int gen)
{
    InfoArtifact* objArtifact = nullptr;
    
    for(auto iter : getInstance()->_mapArtifact)
    {
        auto info = iter.second;
        if(info->getType() == type && info->getGeneration() == gen)
        {
            objArtifact = info;
            break;
        }
    }
    
    return objArtifact;
}

int DataManager::GetArtifactMaxLevel(E_ARTIFACT arti)
{
    auto find = getInstance()->_mapArtifact.find(arti);
    auto info = find->second;
    return info->getLevelMax();
}

int DataManager::GetArtifactGeneration(E_ARTIFACT arti)
{
    int nGeneration = 0;
    auto find = getInstance()->_mapArtifact.find(arti);
    if ( find != getInstance()->_mapArtifact.end() )
    {
        auto info = find->second;
        nGeneration = info->getGeneration();
    }
    
    return nGeneration;
}

int DataManager::GetArtifactHighGeneration(E_ARTIFACT_TYPE type){
    /*
    int highGeneration = 1;
    //보물의 최대차수를 구하는게 없다...
    for(int i=E_ARTIFACT::E_ARTIFACT_999_NONE-1; i>0; i--){
        highGeneration = GetArtifactGeneration((E_ARTIFACT)i);
        if(highGeneration > 0)
            break;
    }
    return highGeneration;
     */
    
    // 문서에 15차까지 추가 되면서 하드코딩
    return 15;
}

E_ARTIFACT_TYPE DataManager::GetArtifactType(E_ARTIFACT arti)
{
    E_ARTIFACT_TYPE eType = E_ARTIFACT_TYPE::ARTI_TYPE_NONE;
    auto find = getInstance()->_mapArtifact.find(arti);
    if ( find != getInstance()->_mapArtifact.end() )
    {
        auto info = find->second;
        eType = info->getType();
    }
    
    return eType;
}

bool DataManager::IsArtifactTypeNormal(E_ARTIFACT arti)
{// 힘의반지, 폭퐁의칼날, 독수리의상, 황금수정
    auto find = getInstance()->_mapArtifact.find(arti);
    auto info = find->second;
    if ( info->getType() != E_ARTIFACT_TYPE::ARTI_TYPE_NONE && info->getType() != E_ARTIFACT_TYPE::ARTI_TYPE_CROSS )
        return true;
    else
        return false;
}
bool DataManager::IsArtifactTypeCross(E_ARTIFACT arti)
{// 십자가
    auto find = getInstance()->_mapArtifact.find(arti);
    auto info = find->second;
    if ( info->getType() == E_ARTIFACT_TYPE::ARTI_TYPE_CROSS )
        return true;
    else
        return false;
}
bool DataManager::IsArtifactTypeSpecial(E_ARTIFACT arti)
{// 쾌속의 반지 등등(힘의반지, 폭퐁의칼날, 독수리의상, 황금수정, 삽지가 외)
    auto find = getInstance()->_mapArtifact.find(arti);
    auto info = find->second;
    if ( info->getType() == E_ARTIFACT_TYPE::ARTI_TYPE_NONE )
        return true;
    else
        return false;
}

double DataManager::GetArtifactUpgradeKey(E_PLACE ePlace, E_ARTIFACT eArti, int nLevel)
{
    int nFloorHigh = UserInfoManager::getInstance()->getHighFloor(ePlace);
    int nFloorTotal = UserInfoManager::getInstance()->getTotalFloor();
    
    return GetArtifactUpgradeKey(ePlace, eArti, nLevel, nFloorHigh, nFloorTotal);
}

double DataManager::GetArtifactUpgradeKey(E_PLACE ePlace, E_ARTIFACT eArti, int nLevel, int nFloorHigh, int nFloorTotal)
{
    
    if ( _saveArtifactPriceLevel[ePlace][eArti] == nLevel )
    {
        if (_saveArtifactPrice[ePlace][eArti] > 0) {
            return _saveArtifactPrice[ePlace][eArti];
        }
    }
    
    double key = 0.0;
    
    if ( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        //마왕성에서 6차 보물부터 업그레이드가 병행해서 가능하다.
        //일던에선 열쇠로, 마왕성에선 마계영혼으로.
        
        double devilKey = 0.0;
        if(eArti == E_ARTIFACT::ARTI_35_EVOL6)              devilKey = 100000000.0;
        else if(eArti == E_ARTIFACT::ARTI_36_EVOL6)         devilKey = 100000000.0;
        else if(eArti == E_ARTIFACT::ARTI_37_EVOL6)         devilKey = 100000000.0;
        else if(eArti == E_ARTIFACT::ARTI_38_EVOL6)         devilKey = 100000000.0;
        else if(eArti == E_ARTIFACT::ARTI_40_EVOL7)         devilKey = 1000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_41_EVOL7)         devilKey = 1000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_42_EVOL7)         devilKey = 1000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_43_EVOL7)         devilKey = 1000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_46_EVOL8)         devilKey = 1000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_47_EVOL8)         devilKey = 1000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_48_EVOL8)         devilKey = 1000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_49_EVOL8)         devilKey = 1000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_54_EVOL9)         devilKey = 10000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_55_EVOL9)         devilKey = 10000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_56_EVOL9)         devilKey = 10000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_57_EVOL9)         devilKey = 10000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_59_EVOL10)        devilKey = 10000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_60_EVOL10)        devilKey = 10000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_61_EVOL10)        devilKey = 10000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_62_EVOL10)        devilKey = 10000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_64_EVOL11)        devilKey = 10000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_65_EVOL11)        devilKey = 10000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_66_EVOL11)        devilKey = 10000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_67_EVOL11)        devilKey = 10000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_69_EVOL12)        devilKey = 10000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_70_EVOL12)        devilKey = 10000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_71_EVOL12)        devilKey = 10000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_72_EVOL12)        devilKey = 10000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_74_EVOL13)        devilKey = 10000000000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_75_EVOL13)        devilKey = 10000000000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_76_EVOL13)        devilKey = 10000000000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_77_EVOL13)        devilKey = 10000000000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_79_EVOL14)        devilKey = 10000000000000000000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_80_EVOL14)        devilKey = 10000000000000000000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_81_EVOL14)        devilKey = 10000000000000000000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_82_EVOL14)        devilKey = 10000000000000000000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_84_EVOL15)        devilKey = 100000000000000000000000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_85_EVOL15)        devilKey = 100000000000000000000000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_86_EVOL15)        devilKey = 100000000000000000000000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_87_EVOL15)        devilKey = 100000000000000000000000000000000000000000000000000000000000000000000000000000000000000.0;

        
        
        //level per
        if ( eArti == E_ARTIFACT::ARTI_35_EVOL6 || eArti == E_ARTIFACT::ARTI_36_EVOL6 ||
             eArti == E_ARTIFACT::ARTI_37_EVOL6 || eArti == E_ARTIFACT::ARTI_38_EVOL6 )
        {
            devilKey += 10000000.0 * nLevel;
        }
        else if ( eArti == E_ARTIFACT::ARTI_40_EVOL7 || eArti == E_ARTIFACT::ARTI_41_EVOL7 ||
                  eArti == E_ARTIFACT::ARTI_42_EVOL7 || eArti == E_ARTIFACT::ARTI_43_EVOL7 )
        {
            devilKey += 100000000000000000.0 * nLevel;
        }
        else if ( eArti == E_ARTIFACT::ARTI_46_EVOL8 || eArti == E_ARTIFACT::ARTI_47_EVOL8 ||
                  eArti == E_ARTIFACT::ARTI_48_EVOL8 || eArti == E_ARTIFACT::ARTI_49_EVOL8 )
        {
            devilKey += 100000000000000000000000000.0 * nLevel;
        }
        else if ( eArti == E_ARTIFACT::ARTI_54_EVOL9 ||eArti == E_ARTIFACT::ARTI_55_EVOL9 ||
                  eArti == E_ARTIFACT::ARTI_56_EVOL9 || eArti == E_ARTIFACT::ARTI_57_EVOL9 )
        {
            devilKey += 1000000000000000000000000000000000000.0 * nLevel;
        }
        else if ( eArti == E_ARTIFACT::ARTI_59_EVOL10 || eArti == E_ARTIFACT::ARTI_60_EVOL10 ||
                  eArti == E_ARTIFACT::ARTI_61_EVOL10 || eArti == E_ARTIFACT::ARTI_62_EVOL10 )
        {
            devilKey += 1000000000000000000000000000000000000000000000.0 * nLevel;
        }
        else if ( eArti == E_ARTIFACT::ARTI_64_EVOL11 || eArti == E_ARTIFACT::ARTI_65_EVOL11 ||
                  eArti == E_ARTIFACT::ARTI_66_EVOL11 || eArti == E_ARTIFACT::ARTI_67_EVOL11 )
        {
            devilKey += 1000000000000000000000000000000000000000000000000000000.0 * nLevel;
        }
        else if ( eArti == E_ARTIFACT::ARTI_69_EVOL12 || eArti == E_ARTIFACT::ARTI_70_EVOL12 ||
                  eArti == E_ARTIFACT::ARTI_71_EVOL12 || eArti == E_ARTIFACT::ARTI_72_EVOL12 )
        {
            devilKey += 1000000000000000000000000000000000000000000000000000000000000000.0 * nLevel;
        }
        else if ( eArti == E_ARTIFACT::ARTI_74_EVOL13 || eArti == E_ARTIFACT::ARTI_75_EVOL13 ||
                  eArti == E_ARTIFACT::ARTI_76_EVOL13 || eArti == E_ARTIFACT::ARTI_77_EVOL13 )
        {
            devilKey += 1000000000000000000000000000000000000000000000000000000000000000000000000.0 * nLevel;
        }
        else if ( eArti == E_ARTIFACT::ARTI_79_EVOL14 || eArti == E_ARTIFACT::ARTI_80_EVOL14 ||
                  eArti == E_ARTIFACT::ARTI_81_EVOL14 || eArti == E_ARTIFACT::ARTI_82_EVOL14 )
        {
            devilKey += 1000000000000000000000000000000000000000000000000000000000000000000000000000000000.0 * nLevel;
        }
        else if ( eArti == E_ARTIFACT::ARTI_84_EVOL15 || eArti == E_ARTIFACT::ARTI_85_EVOL15 ||
                  eArti == E_ARTIFACT::ARTI_86_EVOL15 || eArti == E_ARTIFACT::ARTI_87_EVOL15 )
        {
            devilKey += 10000000000000000000000000000000000000000000000000000000000000000000000000000000000000.0 * nLevel;
        }
        
        key = devilKey;
    }
    else
    {
        if     (eArti == E_ARTIFACT::ARTI_1_EVOL1)               key = 20.0;
        else if(eArti == E_ARTIFACT::ARTI_2)                     key = 50.0;
        else if(eArti == E_ARTIFACT::ARTI_3_EVOL1)               key = 40.0;
        else if(eArti == E_ARTIFACT::ARTI_4_EVOL1)               key = 40.0;
        else if(eArti == E_ARTIFACT::ARTI_5)                     key = 100.0;
        else if(eArti == E_ARTIFACT::ARTI_6)                     key = 40.0;
        else if(eArti == E_ARTIFACT::ARTI_7)                     key = 200.0;
        else if(eArti == E_ARTIFACT::ARTI_8_EVOL1)               key = 40.0;
        else if(eArti == E_ARTIFACT::ARTI_9_KEY)                 key = 200.0; //500
        else if(eArti == E_ARTIFACT::ARTI_10_KEY)                key = 150.0; //500
        else if(eArti == E_ARTIFACT::ARTI_11_KEY)                key = 150.0; //500
        else if(eArti == E_ARTIFACT::ARTI_12_CROSS_EVOL1)        key = 5000000.0;//10000000.0*0.5;
        else if(eArti == E_ARTIFACT::ARTI_13_EVOL2)              key = 1000000.0;
        else if(eArti == E_ARTIFACT::ARTI_14_EVOL2)              key = 2000000.0;
        else if(eArti == E_ARTIFACT::ARTI_15_EVOL2)              key = 2000000.0;
        else if(eArti == E_ARTIFACT::ARTI_16_EVOL2)              key = 2000000.0;
        else if(eArti == E_ARTIFACT::ARTI_17_CROSS_EVOL2)        key = 5000000000.0;//10000000000.0*0.5;
        else if(eArti == E_ARTIFACT::ARTI_18_EVOL3)              key = 1000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_19_EVOL3)              key = 1000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_20_EVOL3)              key = 1000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_21_EVOL3)              key = 1000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_22_CROSS_EVOL3)        key = 100000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_23_EVOL4)              key = 10000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_24_EVOL4)              key = 10000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_25_EVOL4)              key = 10000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_26_EVOL4)              key = 10000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_27_CROSS_EVOL4)        key = 1000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_28_EVOL5)              key = 10000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_29_EVOL5)              key = 10000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_30_EVOL5)              key = 10000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_31_EVOL5)              key = 10000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_32_PRISON_WEAPON)      key = 100.0;
        else if(eArti == E_ARTIFACT::ARTI_33_CROSS_EVOL5)        key = 1000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_34_PICKAX)             key = 500;
        
        else if(eArti == E_ARTIFACT::ARTI_35_EVOL6)              key = 10000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_36_EVOL6)              key = 10000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_37_EVOL6)              key = 10000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_38_EVOL6)              key = 10000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_39_CROSS_EVOL6)        key = 10000000000000000000000000000000000000000000.0;
        
        else if(eArti == E_ARTIFACT::ARTI_40_EVOL7)              key = 100000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_41_EVOL7)              key = 100000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_42_EVOL7)              key = 100000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_43_EVOL7)              key = 100000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_44_CROSS_EVOL7)        key = 1000000000000000000000000000000000000000000000.0;
        
        else if(eArti == E_ARTIFACT::ARTI_46_EVOL8)              key = 10000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_47_EVOL8)              key = 10000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_48_EVOL8)              key = 10000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_49_EVOL8)              key = 10000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_53_CROSS_EVOL8)        key = 1000000000000000000000000000000000000000000000000.0;
  
        else if(eArti == E_ARTIFACT::ARTI_50)                    key = 500.0;
        else if(eArti == E_ARTIFACT::ARTI_51)                    key = 600.0;
        else if(eArti == E_ARTIFACT::ARTI_52)                    key = 150.0;
        
        else if(eArti == E_ARTIFACT::ARTI_54_EVOL9)              key = 1000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_55_EVOL9)              key = 1000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_56_EVOL9)              key = 1000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_57_EVOL9)              key = 1000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_58_CROSS_EVOL9)        key = 100000000000000000000000000000000000000000000000000000.0;
        
        else if(eArti == E_ARTIFACT::ARTI_59_EVOL10)             key = 100000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_60_EVOL10)             key = 100000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_61_EVOL10)             key = 100000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_62_EVOL10)             key = 100000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_63_CROSS_EVOL10)       key = 10000000000000000000000000000000000000000000000000000000000.0;
        
        else if(eArti == E_ARTIFACT::ARTI_64_EVOL11)             key = 10000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_65_EVOL11)             key = 10000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_66_EVOL11)             key = 10000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_67_EVOL11)             key = 10000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_68_CROSS_EVOL11)       key = 1000000000000000000000000000000000000000000000000000000000000000.0;
        
        else if(eArti == E_ARTIFACT::ARTI_69_EVOL12)             key = 1000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_70_EVOL12)             key = 1000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_71_EVOL12)             key = 1000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_72_EVOL12)             key = 1000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_73_CROSS_EVOL12)       key = 100000000000000000000000000000000000000000000000000000000000000000000.0;
        
        else if(eArti == E_ARTIFACT::ARTI_74_EVOL13)             key = 100000000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_75_EVOL13)             key = 100000000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_76_EVOL13)             key = 100000000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_77_EVOL13)             key = 100000000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_78_CROSS_EVOL13)       key = 10000000000000000000000000000000000000000000000000000000000000000000000000.0;
        
        else if(eArti == E_ARTIFACT::ARTI_79_EVOL14)             key = 1000000000000000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_80_EVOL14)             key = 1000000000000000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_81_EVOL14)             key = 1000000000000000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_82_EVOL14)             key = 1000000000000000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_83_CROSS_EVOL14)       key = 100000000000000000000000000000000000000000000000000000000000000000000000000000000.0;

        else if(eArti == E_ARTIFACT::ARTI_84_EVOL15)             key = 25000000000000000000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_85_EVOL15)             key = 25000000000000000000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_86_EVOL15)             key = 25000000000000000000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_87_EVOL15)             key = 25000000000000000000000000000000000000000000000000000000000000000000000000000000000.0;
        else if(eArti == E_ARTIFACT::ARTI_88_CROSS_EVOL15)       key = 1250000000000000000000000000000000000000000000000000000000000000000000000000000000000.0;

        else if(eArti == E_ARTIFACT::ARTI_89_DISCOUNT_1)         key = 50.0;
        else if(eArti == E_ARTIFACT::ARTI_90_DISCOUNT_2)         key = 155.0;
        else if(eArti == E_ARTIFACT::ARTI_91_DISCOUNT_3)         key = 260.0;
        else if(eArti == E_ARTIFACT::ARTI_92_DISCOUNT_4)         key = 365.0;
        else if(eArti == E_ARTIFACT::ARTI_93_DISCOUNT_5)         key = 450.0;
        else if(eArti == E_ARTIFACT::ARTI_94_DISCOUNT_6)         key = 470.0;
        else if(eArti == E_ARTIFACT::ARTI_95_DISCOUNT_7)         key = 490.0;
        else if(eArti == E_ARTIFACT::ARTI_96_DISCOUNT_8)         key = 510.0;
        else if(eArti == E_ARTIFACT::ARTI_97_DISCOUNT_9)         key = 530.0;
        else if(eArti == E_ARTIFACT::ARTI_98_DISCOUNT_10)        key = 550.0;
        else if(eArti == E_ARTIFACT::ARTI_99_DISCOUNT_11)        key = 570.0;
        else if(eArti == E_ARTIFACT::ARTI_100_DISCOUNT_12)       key = 590.0;
        else if(eArti == E_ARTIFACT::ARTI_101_DISCOUNT_13)       key = 610.0;
        else if(eArti == E_ARTIFACT::ARTI_102_DISCOUNT_14)       key = 1500.0;
        
        else if(eArti == E_ARTIFACT::ARTI_103_BERSERKER_SPRIT)   key = 4500.0;
        else if(eArti == E_ARTIFACT::ARTI_104_BERSERKER_AMULET)  key = 5000.0;
        else if(eArti == E_ARTIFACT::ARTI_105_INFINITE_DISCOUNT) key = 5000.0;
        else if(eArti == E_ARTIFACT::ARTI_106) key = 750.0;
        
        //_key 붙은 애들이 골든키 시리즈일 가능성 높음
        if ( nLevel > 0 )
        {
            if     (eArti == E_ARTIFACT::ARTI_1_EVOL1) key += 10.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_2)       key += 25.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_3_EVOL1) key += 20.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_4_EVOL1) key += 20.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_5)       key += 50.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_6)       key += 20.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_7)       key += 100.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_8_EVOL1) key += 20.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_9_KEY)   key += 200.0 * nLevel; // 250
            else if(eArti == E_ARTIFACT::ARTI_10_KEY)
            {
                key += 250 * nLevel; // 250
                
                if(key>=2200)
                    key=2200;
            }
            else if(eArti == E_ARTIFACT::ARTI_11_KEY)
            {
                key += 250 * nLevel; // 250
                if(key>=2200)
                    key=2200;
            }
            else if(eArti == E_ARTIFACT::ARTI_12_CROSS_EVOL1)
            {
                for (int i=0; i<nLevel; i++)
                {
                    key *= 1.05;
                }
            }
            else if(eArti == E_ARTIFACT::ARTI_13_EVOL2)   key += 10000.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_14_EVOL2)   key += 20000.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_15_EVOL2)   key += 20000.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_16_EVOL2)   key += 20000.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_17_CROSS_EVOL2)
            {
                key = key * pow(1.0001, nLevel);
            }
            else if(eArti == E_ARTIFACT::ARTI_18_EVOL3)   key += 100000000.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_19_EVOL3)   key += 100000000.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_20_EVOL3)   key += 100000000.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_21_EVOL3)   key += 100000000.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_22_CROSS_EVOL3) key += 10000000000000.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_23_EVOL4 ||
                    eArti == E_ARTIFACT::ARTI_24_EVOL4 ||
                    eArti == E_ARTIFACT::ARTI_25_EVOL4 ||
                    eArti == E_ARTIFACT::ARTI_26_EVOL4)
            {
                key += 1000000000000000000.0 * nLevel;
            }
            else if(eArti == E_ARTIFACT::ARTI_27_CROSS_EVOL4)    key += 100000000000000000000000000000000.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_28_EVOL5 ||
                    eArti == E_ARTIFACT::ARTI_29_EVOL5 ||
                    eArti == E_ARTIFACT::ARTI_30_EVOL5 ||
                    eArti == E_ARTIFACT::ARTI_31_EVOL5)
            {
                key += 1000000000000000000000000.0 * nLevel;
            }
            else if(eArti == E_ARTIFACT::ARTI_32_PRISON_WEAPON)    key += 100.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_33_CROSS_EVOL5)    key += 100000000000000000000000000000000000000.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_34_PICKAX)         key = 500 + ((nLevel-1)*100);
            else if(eArti == E_ARTIFACT::ARTI_35_EVOL6 ||
                    eArti == E_ARTIFACT::ARTI_36_EVOL6 ||
                    eArti == E_ARTIFACT::ARTI_37_EVOL6 ||
                    eArti == E_ARTIFACT::ARTI_38_EVOL6)
            {
                key += 1000000000000000000000000000000000000.0 * nLevel;
            }
            else if(eArti == E_ARTIFACT::ARTI_39_CROSS_EVOL6)    key += 1000000000000000000000000000000000000000000.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_40_EVOL7 ||
                    eArti == E_ARTIFACT::ARTI_41_EVOL7 ||
                    eArti == E_ARTIFACT::ARTI_42_EVOL7 ||
                    eArti == E_ARTIFACT::ARTI_43_EVOL7)
            {
                key += 10000000000000000000000000000000000000000.0 * nLevel;
            }
            else if(eArti == E_ARTIFACT::ARTI_44_CROSS_EVOL7)    key += 100000000000000000000000000000000000000000000.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_46_EVOL8 ||
                    eArti == E_ARTIFACT::ARTI_47_EVOL8 ||
                    eArti == E_ARTIFACT::ARTI_48_EVOL8 ||
                    eArti == E_ARTIFACT::ARTI_49_EVOL8)
            {
                key += 1000000000000000000000000000000000000000000000.0 * nLevel;
            }
            else if(eArti == E_ARTIFACT::ARTI_53_CROSS_EVOL8)    key += 100000000000000000000000000000000000000000000000.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_50)                  key += 350.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_51)                  key += 200.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_52)                  key += 50.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_54_EVOL9 ||
                    eArti == E_ARTIFACT::ARTI_55_EVOL9 ||
                    eArti == E_ARTIFACT::ARTI_56_EVOL9 ||
                    eArti == E_ARTIFACT::ARTI_57_EVOL9)
            {
                key += 100000000000000000000000000000000000000000000000000.0 * nLevel;
            }
            else if(eArti == E_ARTIFACT::ARTI_58_CROSS_EVOL9)    key += 10000000000000000000000000000000000000000000000000000.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_59_EVOL10 ||
                    eArti == E_ARTIFACT::ARTI_60_EVOL10 ||
                    eArti == E_ARTIFACT::ARTI_61_EVOL10 ||
                    eArti == E_ARTIFACT::ARTI_62_EVOL10)
            {
                key += 10000000000000000000000000000000000000000000000000000000.0 * nLevel;
            }
            else if(eArti == E_ARTIFACT::ARTI_63_CROSS_EVOL10)   key += 100000000000000000000000000000000000000000000000000000000.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_64_EVOL11 ||
                    eArti == E_ARTIFACT::ARTI_65_EVOL11 ||
                    eArti == E_ARTIFACT::ARTI_66_EVOL11 ||
                    eArti == E_ARTIFACT::ARTI_67_EVOL11)
            {
                key += 1000000000000000000000000000000000000000000000000000000000000.0 * nLevel;
            }
            else if(eArti == E_ARTIFACT::ARTI_68_CROSS_EVOL11)   key += 10000000000000000000000000000000000000000000000000000000000000.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_69_EVOL12 ||
                    eArti == E_ARTIFACT::ARTI_70_EVOL12 ||
                    eArti == E_ARTIFACT::ARTI_71_EVOL12 ||
                    eArti == E_ARTIFACT::ARTI_72_EVOL12)
            {
                key += 100000000000000000000000000000000000000000000000000000000000000000.0 * nLevel;
            }
            else if(eArti == E_ARTIFACT::ARTI_73_CROSS_EVOL12)   key += 1000000000000000000000000000000000000000000000000000000000000000000.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_74_EVOL13 ||
                    eArti == E_ARTIFACT::ARTI_75_EVOL13 ||
                    eArti == E_ARTIFACT::ARTI_76_EVOL13 ||
                    eArti == E_ARTIFACT::ARTI_77_EVOL13)
            {
                key += 10000000000000000000000000000000000000000000000000000000000000000000000.0 * nLevel;
            }
            else if(eArti == E_ARTIFACT::ARTI_78_CROSS_EVOL13)   key += 100000000000000000000000000000000000000000000000000000000000000000000000.0 * nLevel;
            else if(eArti == E_ARTIFACT::ARTI_79_EVOL14 ||
                    eArti == E_ARTIFACT::ARTI_80_EVOL14 ||
                    eArti == E_ARTIFACT::ARTI_81_EVOL14 ||
                    eArti == E_ARTIFACT::ARTI_82_EVOL14)
            {
                key = key * std::pow(nLevel, 1.5); //key += 1000000000000000000000000000000000000000000000000000000000000000000000000000000.0*level;
            }
            else if(eArti == E_ARTIFACT::ARTI_83_CROSS_EVOL14)   key = key * std::pow(nLevel, 1.5); //key += 10000000000000000000000000000000000000000000000000000000000000000000000000000000.0*level;
            else if(eArti == E_ARTIFACT::ARTI_84_EVOL15 ||
                    eArti == E_ARTIFACT::ARTI_85_EVOL15 ||
                    eArti == E_ARTIFACT::ARTI_86_EVOL15 ||
                    eArti == E_ARTIFACT::ARTI_87_EVOL15)
            {
                key = key * std::pow(nLevel, 1.15);
            }
            
            else if(eArti == E_ARTIFACT::ARTI_88_CROSS_EVOL15) key = key * std::pow(nLevel, 1.15);
            else if(eArti == E_ARTIFACT::ARTI_103_BERSERKER_SPRIT) key += nLevel * 2200;
            else if(eArti == E_ARTIFACT::ARTI_104_BERSERKER_AMULET) key += nLevel * 1000;
            else if(eArti == E_ARTIFACT::ARTI_105_INFINITE_DISCOUNT) key = 5000.0;
            else if(eArti == E_ARTIFACT::ARTI_106)
            {
                key = (int64_t)750 * std::pow(nLevel + 1 + 4 * (int)(0.2 * (nLevel + 1)), 1.15);
                key = (int64_t)ceil(key / 100.0f) * 100;
            }
        }
        
        auto artiInfo = DataManager::GetArtifactInfo(eArti);
        if ( IsArtifactTypeNormal(eArti) == true && artiInfo->getPriceType() == E_ITEMS::KEY )
        {
            if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD,E_COSTUME_IDX::IDX_15)) {
                key = key * 0.8;
            }
        }
        
        /*
         비용 감소는 기본식의 수정으로 합칠수 있도록 해야한다.
         */
        if (artiInfo->getGeneration()==1 && artiInfo->getType() != ARTI_TYPE_CROSS && artiInfo->getPriceType() == E_ITEMS::KEY){// 1차 보물
            key = key*0.25;
        }
        else if (artiInfo->getGeneration()==2 && artiInfo->getType() != ARTI_TYPE_CROSS && artiInfo->getPriceType() == E_ITEMS::KEY){// 2차 보물
            key = key*0.5;
        }
        else if (artiInfo->getGeneration()==3 && artiInfo->getType() != ARTI_TYPE_CROSS && artiInfo->getPriceType() == E_ITEMS::KEY){// 3차 보물
            key = key*0.5;
        }
        else if ( artiInfo->getGeneration()==4 && artiInfo->getType() != ARTI_TYPE_CROSS && artiInfo->getPriceType() == E_ITEMS::KEY){ //4차
            key = key*0.1;
        }
        
        if ( E_ARTIFACT::ARTI_22_CROSS_EVOL3 == eArti )
        {// 3차 십자가
            key = key*0.1;
        }
        
        if ( artiInfo->getGeneration() == 1 && UserInfoManager::getInstance()->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_89_DISCOUNT_1) > 0 )
        {
            key = key*0.5;
        }
        else if ( artiInfo->getGeneration() == 2 && UserInfoManager::getInstance()->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_90_DISCOUNT_2) > 0 )
        {
            key = key*0.7;
        }
        else if ( artiInfo->getGeneration() == 3 && UserInfoManager::getInstance()->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_91_DISCOUNT_3) > 0 )
        {
            key = key*0.7;
        }
        else if ( artiInfo->getGeneration() == 4 && UserInfoManager::getInstance()->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_92_DISCOUNT_4) > 0 )
        {
            key = key*0.2;
        }
        else if ( artiInfo->getGeneration() == 5 && UserInfoManager::getInstance()->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_93_DISCOUNT_5) > 0 )
        {
            key = key*0.7;
        }
        else if ( artiInfo->getGeneration() == 6 && UserInfoManager::getInstance()->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_94_DISCOUNT_6) > 0 )
        {
            key = key*0.7;
        }
        else if ( artiInfo->getGeneration() == 7 && UserInfoManager::getInstance()->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_95_DISCOUNT_7) > 0 )
        {
            key = key*0.7;
        }
        else if ( artiInfo->getGeneration() == 8 && UserInfoManager::getInstance()->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_96_DISCOUNT_8) > 0 )
        {
            key = key*0.7;
        }
        else if ( artiInfo->getGeneration() == 9 && UserInfoManager::getInstance()->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_97_DISCOUNT_9) > 0 )
        {
            key = key*0.25;
        }
        else if ( artiInfo->getGeneration() == 10 && UserInfoManager::getInstance()->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_98_DISCOUNT_10) > 0 )
        {
            key = key*0.7;
        }
        else if ( artiInfo->getGeneration() == 11 && UserInfoManager::getInstance()->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_99_DISCOUNT_11) > 0 )
        {
            key = key*0.7;
        }
        else if ( artiInfo->getGeneration() == 12 && UserInfoManager::getInstance()->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_100_DISCOUNT_12) > 0 )
        {
            key = key*0.7;
        }
        else if ( artiInfo->getGeneration() == 13 && UserInfoManager::getInstance()->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_101_DISCOUNT_13) > 0 )
        {
            key = key*0.7;
        }
        else if ( artiInfo->getGeneration() == 14 && UserInfoManager::getInstance()->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_102_DISCOUNT_14) > 0 )
        {
            key = key*0.001;
        }
    }
    
    /*
     보물 업그레이드 비용 감소 밸런스 패치
     1,2,3차 십자가
     1,2,3차 보물
     */
    
    _saveArtifactPriceLevel[ePlace][eArti] = nLevel;
    _saveArtifactPrice[ePlace][eArti] = key;
    
    return key;
}

double DataManager::GetArtifactEffect(E_ARTIFACT arti, int level, int reinforce)
{
    // 보물 강화 4차 이상 : 레벨 증가
    if ( arti >= E_ARTIFACT::ARTI_23_EVOL4 )
    {
        level += reinforce;
    }
    
    if (arti == E_ARTIFACT::ARTI_9_KEY)
    {
        int honorEffect = DataManager::GetHonorEffect(E_HONOR::HONOR_3, UserInfoManager::getInstance()->getHonorLevel(E_HONOR::HONOR_3));
        if (level > 0 && honorEffect > 0)
        {
            level += honorEffect;
        }
    }
    
    if ( _saveArtifactLevel[arti] == level )
    {
        if ( _saveArtifactEffect[arti] > 0 )
        {
            double nResult = _saveArtifactEffect[arti];
            
            // 보물 강화 3차 이하 : 효과 증가
            if ( arti <= E_ARTIFACT::ARTI_21_EVOL3 )
            {
                if ( reinforce != 0 )
                {
                    nResult += DataManager::GetArtifactReinforceEffect(arti, reinforce);
                }
            }
            return nResult;
        }
    }
    
    double attack = 0;
    //여기 없는 보물들은 전부 0부터 시작
    if(arti==E_ARTIFACT::ARTI_2)                    attack = 5;
    else if(arti==E_ARTIFACT::ARTI_3_EVOL1)         attack = 50;
    else if(arti==E_ARTIFACT::ARTI_7)               attack = 0.5;
    else if(arti==E_ARTIFACT::ARTI_32_PRISON_WEAPON) attack = level;
    
    if(arti==E_ARTIFACT::ARTI_1_EVOL1){
        const int up = 15;
        const int val = level/25 + 1;
        const int rest = level%25;
        attack += (double)((val/2.0f)*up) * (level-rest);
        for(int i=1;i<=rest;i++){
            attack += val*up;
        }
    }
    else if(arti==E_ARTIFACT::ARTI_2) attack += 1*level;
    else if(arti==E_ARTIFACT::ARTI_3_EVOL1) attack += 25*level;
    else if(arti==E_ARTIFACT::ARTI_4_EVOL1){
        const int up = 10;
        const int rate = 5;
        const int val = level/50;
        const int rest = level%50;
        if (val > 0) {
            attack += (double)(((val-1)/2.0f*rate)+up) * (level-rest);
        }
        for(int i=1;i<=rest;i++){
            attack += val*rate+up;
        }
    }
    else if(arti==E_ARTIFACT::ARTI_5) attack += 0.1*level;
    else if(arti==E_ARTIFACT::ARTI_6) attack += 2*level;
    else if(arti==E_ARTIFACT::ARTI_7) attack -= 0.04*level;
    else if(arti==E_ARTIFACT::ARTI_8_EVOL1) attack += 10*level;
    else if(arti==E_ARTIFACT::ARTI_9_KEY) attack += 10*level;
    else if(arti==E_ARTIFACT::ARTI_10_KEY) attack += 5*level;
    else if(arti==E_ARTIFACT::ARTI_11_KEY) attack += 5*level;
    else if(arti==E_ARTIFACT::ARTI_12_CROSS_EVOL1){
        for(int i=1;i<=level;i++){
            int val = i/10;
            int val2 = (i-150)/10 + 1;
            int rest = i%10;
            if(rest==0){
                val--;
                val2--;
            }
            long long addvalue = 0;
            if (i > 150) {
                addvalue = pow(2, val+val2);
            }else{
                addvalue = pow(2, val);
            }
            if (addvalue <= 0) {
                addvalue = 1;
            }
            attack += addvalue;
        }
    }
    else if(arti==E_ARTIFACT::ARTI_13_EVOL2){
        const int count = level/100;
        const int rest = level%100;
        const double up = 0.01;
        
        double lastup = 0;
        for(int i=1;i<=count;i++){
            int value = i*4;
            attack += (value*up) * (value+1) * 0.5 * 25;
            lastup += (value*up);
        }
        
        for(int i=1;i<=rest;i++){
            int rest = (i-1)%25;
            if(rest==0)
                lastup += (count+1)*up;

            attack += lastup;
        }
    }
    else if(arti==E_ARTIFACT::ARTI_14_EVOL2 || arti==E_ARTIFACT::ARTI_16_EVOL2){
        //attack += 0.01f*level;
        const double up = 0.01;
        const double val = level/50 + 1;
        const double rest = level%50;
        
        attack += ((val/2.0f)*up) * (level-rest);
        
        for(int i=1;i<=rest;i++){
            attack += val*up;
        }
    }
    else if(arti==E_ARTIFACT::ARTI_15_EVOL2){
        const int count = level/100;
        const int rest = level%100;
        const double up = 0.01f;
        
        double lastup = 0;
        for(int i=1;i<=count;i++){
            int value = i*2;
            attack += (value*up) * (value+1) * 0.5 * 50;
            lastup += (value*up);
        }
        
        for(int i=1;i<=rest;i++){
            int rest = (i-1)%50;
            if(rest==0)
                lastup += (count+1)*up;
            
            attack += lastup;
        }
    }
    else if(arti==E_ARTIFACT::ARTI_17_CROSS_EVOL2){
        double up = 0.1;
        for(int i=1;i<=level;i++){
            int rest = i%50;
            if (i > 1) {
                if(rest==1){
                    up *= 1.015f;
                }
            }
            attack += up;
        }
    }
    else if(arti==E_ARTIFACT::ARTI_18_EVOL3){
        double value = 0.01;
        double up = 0.01;
        int evol = 25;
        int evol2 = 5000;
        
        for (int i=1; i<=level; i++) {
            if (i > 1) {
                if ((i-1) % evol2 == 0)
                    up *= 2;
                if ((i-1) % evol == 0)
                    value += up;
            }
            attack += value;
        }
    }
    else if(arti==E_ARTIFACT::ARTI_20_EVOL3){
        double value = 0.01;
        double up = 0.01;
        int evol = 50;
        int evol2 = 5000;
        
        for (int i=1; i<=level; i++) {
            if (i > 1) {
                if ((i-1) % evol2 == 0)
                    up *= 2;
                if ((i-1) % evol == 0)
                    value += up;
            }
            attack += value;
        }
    }
    else if(arti==E_ARTIFACT::ARTI_19_EVOL3 || arti==E_ARTIFACT::ARTI_21_EVOL3){
        double value = 0.01;
        double up = 0.01;
        int evol = 50;
        int evol2 = 5000;
        
        for (int i=1; i<=level; i++) {
            if (i > 1) {
                if ((i-1) % evol2 == 0)
                    up *= 2;
                if ((i-1) % evol == 0)
                    value += up;
            }
            attack += value;
        }
    }
    else if(arti==E_ARTIFACT::ARTI_22_CROSS_EVOL3){
        double value = 0.1;
        double up = 0.1;
        int evol = 50;
        int evol2 = 4000;
        
        for (int i=1; i<=level; i++) {
            if (i > 1) {
                if ((i-1) % evol2 == 0)
                    up *= 2;
                if ((i-1) % evol == 0)
                    value += up;
            }
            attack += value;
        }
    }
    else if(arti == E_ARTIFACT::ARTI_23_EVOL4 ||
            arti == E_ARTIFACT::ARTI_24_EVOL4 ||
            arti == E_ARTIFACT::ARTI_25_EVOL4 ||
            arti == E_ARTIFACT::ARTI_26_EVOL4){
        double value = 1.0;
        double up = 1.0;
        int evol = 50;
        int evol2 = 5000;
        
        for (int i=1; i<=level; i++) {
            if (i > 1) {
                if ((i-1) % evol2 == 0)
                    up *= 2;
                if ((i-1) % evol == 0)
                    value += up;
            }
            attack += value;
        }
    }
    else if(arti==E_ARTIFACT::ARTI_27_CROSS_EVOL4 || arti==E_ARTIFACT::ARTI_33_CROSS_EVOL5 ||
            arti==E_ARTIFACT::ARTI_39_CROSS_EVOL6 || arti==E_ARTIFACT::ARTI_44_CROSS_EVOL7 ||
            arti==E_ARTIFACT::ARTI_53_CROSS_EVOL8 || arti==E_ARTIFACT::ARTI_58_CROSS_EVOL9 ||
            arti==E_ARTIFACT::ARTI_63_CROSS_EVOL10 || arti==E_ARTIFACT::ARTI_68_CROSS_EVOL11 ||
            arti==E_ARTIFACT::ARTI_73_CROSS_EVOL12 || arti==E_ARTIFACT::ARTI_78_CROSS_EVOL13 ||
            arti==E_ARTIFACT::ARTI_83_CROSS_EVOL14 ||  arti==E_ARTIFACT::ARTI_88_CROSS_EVOL15)
    {
        double value = 1.0;
        double up = 1.0;
        int evol = 50;
        int evol2 = 5000;
        
        for (int i=1; i<=level; i++) {
            if (i > 1) {
                if ((i-1) % evol2 == 0)
                    up *= 2;
                if ((i-1) % evol == 0)
                    value += up;
            }
            attack += value;
        }
    }
    else if(arti == E_ARTIFACT::ARTI_28_EVOL5 ||
            arti == E_ARTIFACT::ARTI_29_EVOL5 ||
            arti == E_ARTIFACT::ARTI_30_EVOL5 ||
            arti == E_ARTIFACT::ARTI_31_EVOL5){
        double value = 1.0;
        double up = 1.0;
        int evol = 50;
        int evol2 = 5000;
        
        for (int i=1; i<=level; i++) {
            if (i > 1) {
                if ((i-1) % evol2 == 0)
                    up *= 2;
                if ((i-1) % evol == 0)
                    value += up;
            }
            attack += value;
        }
    }
    else if(arti==E_ARTIFACT::ARTI_34_PICKAX){
        attack = GetMinePickaxEffectData(level);
    }
    else if(arti == E_ARTIFACT::ARTI_35_EVOL6 || arti == E_ARTIFACT::ARTI_36_EVOL6 || arti == E_ARTIFACT::ARTI_37_EVOL6 || arti == E_ARTIFACT::ARTI_38_EVOL6 ||
            arti == E_ARTIFACT::ARTI_40_EVOL7 || arti == E_ARTIFACT::ARTI_41_EVOL7 || arti == E_ARTIFACT::ARTI_42_EVOL7 || arti == E_ARTIFACT::ARTI_43_EVOL7 ||
            arti == E_ARTIFACT::ARTI_46_EVOL8 || arti == E_ARTIFACT::ARTI_47_EVOL8 || arti == E_ARTIFACT::ARTI_48_EVOL8 || arti == E_ARTIFACT::ARTI_49_EVOL8 ||
            arti == E_ARTIFACT::ARTI_54_EVOL9 || arti == E_ARTIFACT::ARTI_55_EVOL9 || arti == E_ARTIFACT::ARTI_56_EVOL9 || arti == E_ARTIFACT::ARTI_57_EVOL9 ||
            arti == E_ARTIFACT::ARTI_59_EVOL10 || arti == E_ARTIFACT::ARTI_60_EVOL10 || arti == E_ARTIFACT::ARTI_61_EVOL10 || arti == E_ARTIFACT::ARTI_62_EVOL10 ||
            arti == E_ARTIFACT::ARTI_64_EVOL11 || arti == E_ARTIFACT::ARTI_65_EVOL11 || arti == E_ARTIFACT::ARTI_66_EVOL11 || arti == E_ARTIFACT::ARTI_67_EVOL11 ||
            arti == E_ARTIFACT::ARTI_69_EVOL12 || arti == E_ARTIFACT::ARTI_70_EVOL12 || arti == E_ARTIFACT::ARTI_71_EVOL12 || arti == E_ARTIFACT::ARTI_72_EVOL12 ||
            arti == E_ARTIFACT::ARTI_74_EVOL13 || arti == E_ARTIFACT::ARTI_75_EVOL13 || arti == E_ARTIFACT::ARTI_76_EVOL13 || arti == E_ARTIFACT::ARTI_77_EVOL13 ||
            arti == E_ARTIFACT::ARTI_79_EVOL14 || arti == E_ARTIFACT::ARTI_80_EVOL14 || arti == E_ARTIFACT::ARTI_81_EVOL14 || arti == E_ARTIFACT::ARTI_82_EVOL14 ||
            arti == E_ARTIFACT::ARTI_84_EVOL15 || arti == E_ARTIFACT::ARTI_85_EVOL15 || arti == E_ARTIFACT::ARTI_86_EVOL15 || arti == E_ARTIFACT::ARTI_87_EVOL15 )
    {
        double value = 1.0;
        double up = 1.0;
        int evol = 50;
        int evol2 = 5000;
        
        for (int i=1; i<=level; i++) {
            if (i > 1) {
                if ((i-1) % evol2 == 0)
                    up *= 2;
                if ((i-1) % evol == 0)
                    value += up;
            }
            attack += value;
        }
    }
    else if(arti==E_ARTIFACT::ARTI_45){
        attack = level;
    }
    else if(arti==E_ARTIFACT::ARTI_50 && level != 0){
        attack = 1100.0;
        attack -= 100*level;
    }
    else if(arti==E_ARTIFACT::ARTI_51){
        attack += 10*level;
    }
    else if(arti==E_ARTIFACT::ARTI_52){
        attack += 1*level;
    }
    else if(arti==E_ARTIFACT::ARTI_89_DISCOUNT_1){
        attack += 50*level;
    }
    else if(arti==E_ARTIFACT::ARTI_90_DISCOUNT_2){
        attack += 30*level;
    }
    else if(arti==E_ARTIFACT::ARTI_91_DISCOUNT_3){
        attack += 30*level;
    }
    else if(arti==E_ARTIFACT::ARTI_92_DISCOUNT_4){
        attack += 80*level;
    }
    else if(arti==E_ARTIFACT::ARTI_93_DISCOUNT_5){
        attack += 30*level;
    }
    else if(arti==E_ARTIFACT::ARTI_94_DISCOUNT_6){
        attack += 30*level;
    }
    else if(arti==E_ARTIFACT::ARTI_95_DISCOUNT_7){
        attack += 30*level;
    }
    else if(arti==E_ARTIFACT::ARTI_96_DISCOUNT_8){
        attack += 30*level;
    }
    else if(arti==E_ARTIFACT::ARTI_97_DISCOUNT_9){
        attack += 75*level;
    }
    else if(arti==E_ARTIFACT::ARTI_98_DISCOUNT_10){
        attack += 30*level;
    }
    else if(arti==E_ARTIFACT::ARTI_99_DISCOUNT_11){
        attack += 30*level;
    }
    else if(arti==E_ARTIFACT::ARTI_100_DISCOUNT_12){
        attack += 30*level;
    }
    else if(arti==E_ARTIFACT::ARTI_101_DISCOUNT_13){
        attack += 30*level;
    }
    else if(arti==E_ARTIFACT::ARTI_102_DISCOUNT_14){
        attack += 99.9*level;
    }
    else if(arti==E_ARTIFACT::ARTI_103_BERSERKER_SPRIT)
    {
        if(level >= 1)
            attack = (0.1 + (((double)level - 1.f) * 0.05f));
    }
    else if(arti==E_ARTIFACT::ARTI_104_BERSERKER_AMULET)
    {
        if(level >= 1)
            attack = 1000000 * std::pow(1.5,(level-1));
    }
    else if(arti==E_ARTIFACT::ARTI_105_INFINITE_DISCOUNT)
    {
        if(level >= 1)
            attack = (level * std::pow(1.002f,(level * 10)) + 1) * 100;
    }
    else if(arti==E_ARTIFACT::ARTI_106)
    {
        attack = 3.5f*level;
    }
    _saveArtifactLevel[arti] = level;
    _saveArtifactEffect[arti] = attack;
    
    // 보물 강화 3차 이하 : 효과 증가
    if ( arti <= E_ARTIFACT::ARTI_21_EVOL3 )
    {
        if ( reinforce != 0 )
        {
            attack += DataManager::GetArtifactReinforceEffect(arti, reinforce);
        }
    }
    return attack;
}

std::string DataManager::GetArtifacEffecttCalculation(std::string strValue, double nPer)
{
    std::string strResult = "0";
    
    nPer *= 10000; //4자리
    strResult = MafUtils::bigMulNum(strValue, MafUtils::doubleToString(nPer));
    if ( strResult.length() > 6 ) // 4자리 + 2자리
    {
        strResult.pop_back();
        strResult.pop_back();
        strResult.pop_back();
        strResult.pop_back();
        strResult.pop_back();
        strResult.pop_back();
    }
    else
    {
        strResult = "0";
    }
    
    return strResult;
}

Vector<ModelInfoDevil::DevilArtifactInfo*> DataManager::GetDevilArtifactDataType(E_D_ARTIFACT_TYPE type){
    
    LoadDevilArtifactData(type);
    
    return getInstance()->_vecDevilArtifact[type];
}
void DataManager::initArtifactSaveData(E_PLACE place)
{
    for (int i = 1; i < C_COUNT_ARTIFACT; i++ )
    {
        _saveArtifactPrice[place][i] = 0;
    }
}

double DataManager::GetArtifactReinforceEffect(E_ARTIFACT arti,int level)
{
    double nResult = 0;
    if ( level > 0 )
    {
        nResult = (int)(25*(std::pow(level, 1.3)-(level)+2)) + 50;
    }
    
    return nResult;
}

int DataManager::GetArtifactReinforceExpandPrice(E_ARTIFACT arti)
{
    int nResult = 0;
    if(GetArtifactGeneration(arti) <= 4)
        nResult = 500;      //1~4차 보물
    else if(GetArtifactGeneration(arti) <= 5)
        nResult = 700;      //5차 보물
    else if(GetArtifactGeneration(arti) <= 6)
        nResult = 1000;      //6차 보물
    else if(GetArtifactGeneration(arti) <= 7)
        nResult = 2000;      //7차 보물
    else if(GetArtifactGeneration(arti) <= 8)
        nResult = 4000;      //8차 보물
//    else if(GetArtifactGeneration(arti) <= 9)
//        nResult = 10000;      //9차 보물
//    else if(GetArtifactGeneration(arti) <= 10)
//        nResult = 10000;      //10차 보물
//    else if(GetArtifactGeneration(arti) <= 11)
//        nResult = 10000;      //11차 보물
//    else if(GetArtifactGeneration(arti) <= 12)
//        nResult = 10000;      //12차 보물
//    else if(GetArtifactGeneration(arti) <= 13)
//        nResult = 10000;      //13차 보물
//    else if(GetArtifactGeneration(arti) <= 14)
//        nResult = 10000;      //14차 보물
    else
        nResult = 10000;
    
    return nResult;
}

int DataManager::GetArtifactReinforcePrice(E_ARTIFACT arti)
{
    return 1;
}

#pragma mark-
std::string DataManager::GetRelicName(E_RELIC relic){
    std::string strKey = MafUtils::format("t_relic_title_%d",relic);
    std::string strText = "";
    if ( strKey.length() != 0 )
    {
        strText = GAME_TEXT(strKey);
    }
    return strText;
}

std::string DataManager::GetRelicExplain(E_RELIC relic){
    std::string strKey = MafUtils::format("t_relic_contents_%d", relic);
    std::string strText = "";
    if ( strKey.length() != 0 )
    {
        strText = GAME_TEXT(strKey);
    }
    return strText;
}

int DataManager::GetRelicUpgradeGem(E_RELIC relic,int level){
    int gem = 0;
    if      (relic==E_RELIC::RELIC_1)       gem = 500;
    else if (relic==E_RELIC::RELIC_2)       gem = 1000;
    else if (relic==E_RELIC::RELIC_3)       gem = 1500;
    else if (relic==E_RELIC::RELIC_4)       gem = 2000;
    else if (relic==E_RELIC::RELIC_5)       gem = 200;
    else if (relic==E_RELIC::RELIC_6)       gem = 500;
    else if (relic==E_RELIC::RELIC_7)       gem = 2000;
    else if (relic==E_RELIC::RELIC_8)       gem = 2000;
    else if (relic==E_RELIC::RELIC_9)       gem = 2000;
    
    if      (relic==E_RELIC::RELIC_1)       gem += 100*(level-1);
    else if (relic==E_RELIC::RELIC_2)       gem += 100*(level-1);
    else if (relic==E_RELIC::RELIC_3)       gem += 100*(level-1);
    else if (relic==E_RELIC::RELIC_4)       gem += 200*(level-1);
    else if (relic==E_RELIC::RELIC_5)       gem += 100*(level-1);
    else if (relic==E_RELIC::RELIC_6)       gem += 100*(level-1);
    else if (relic==E_RELIC::RELIC_7)       gem += 200*(level-1);
    else if (relic==E_RELIC::RELIC_8)       gem += 200*(level-1);
    else if (relic==E_RELIC::RELIC_9)       gem += 200*(level-1);
    
    return gem;
}

double DataManager::GetRelicEffect(E_RELIC relic,int level){
    if (_saveRelicLevel[relic] == level) {
        if (_saveRelicEffect[relic] > 0) {
            return _saveRelicEffect[relic];
        }
    }
    
    double attack = 0;
    
    if(relic==E_RELIC::RELIC_1){
        attack = level*0.0025;
    }else if(relic==E_RELIC::RELIC_2){
        attack = level*40;
    }else if(relic==E_RELIC::RELIC_3){
        attack = level*30;
    }else if(relic==E_RELIC::RELIC_4){
        attack = 15 + (7.5f*(level-1));
        attack *= 100;
        if (level<=0)
            attack = 0;
    }else if(relic==E_RELIC::RELIC_5){
        attack = level*250;
    }else if(relic==E_RELIC::RELIC_6){
        attack = level*50;
    }else if(relic==E_RELIC::RELIC_7){
        attack = level*2;
    }else if(relic==E_RELIC::RELIC_8){
        attack = level*20;
    }else if(relic==E_RELIC::RELIC_9){
        attack = level*2;
    }
    
    _saveRelicLevel[relic] = level;
    _saveRelicEffect[relic] = attack;
    return attack;
}

int DataManager::GetRelicMaxLevel(E_RELIC relic){
    int max = 20; //전부20
    return max;
}
int DataManager::GetRelicOpenFloor(E_RELIC relic){
    int floor = 0;
    
    if      (relic == E_RELIC::RELIC_1)     floor = 3001;
    else if (relic == E_RELIC::RELIC_2)     floor = 4001;
    else if (relic == E_RELIC::RELIC_3)     floor = 5001;
    else if (relic == E_RELIC::RELIC_4)     floor = 6001;
    else if (relic == E_RELIC::RELIC_5)     floor = 1001;
    else if (relic == E_RELIC::RELIC_6)     floor = 2001;
    else if (relic == E_RELIC::RELIC_7)     floor = 7001;
    else if (relic == E_RELIC::RELIC_8)     floor = 8001;
    else if (relic == E_RELIC::RELIC_9)     floor = 9001;
    
    return floor;
}
#pragma mark-
int DataManager::GetHonorPrice(E_HONOR honor, int level)
{
    int value = 0;
    switch (honor) {
        case E_HONOR::HONOR_1:
        case E_HONOR::HONOR_2:  value = 2;      break;
        case E_HONOR::HONOR_3:  value = 100;    break;
        default:
            value = 10000;
            break;
    }
    
    switch (honor) {
        case E_HONOR::HONOR_1:  value += (level)*2;     break;
        case E_HONOR::HONOR_2:  value += (level)*2;     break;
        case E_HONOR::HONOR_3:  value += (level)*10;    break;
        default:
            value = 10000;
            break;
    }
    
    if (honor == E_HONOR::HONOR_1 || honor == E_HONOR::HONOR_2) {
        if (value > 10)
            value = 10;
    }else{
        if (value > 300)
            value = 300;
    }
    
    
    return value;
}
int DataManager::GetHonorEffect(E_HONOR honor, int level)
{
    int value = 0;
    switch (honor) {
        case E_HONOR::HONOR_1:
            value += (level)*1; break;
        case E_HONOR::HONOR_2:
            value += (level)*10; break;
        case E_HONOR::HONOR_3:
            value += (level)*1; break;
        default:
            break;
    }
    return value;
}
int DataManager::GetHonorMaxLevel(E_HONOR honor)
{
    if (honor == E_HONOR::HONOR_1 || honor == E_HONOR::HONOR_2) {
        return 100;
    }else{
        return 50;
    }
}
#pragma mark-
std::string DataManager::GetBonggiokUpgradeKey(long long level){
    
    std::string price = "10000";
    for (int i=1; i<=level; i++) {
        price = MafUtils::bigMulNum(price, "12");
        price.pop_back();
    }
    
    return price;
}

double DataManager::GetBonggiokEffect(int nLevel)
{
    int nLevelMax = GetBonggiokMaxLevel();
    if ( nLevel > nLevelMax )
    {
        nLevel = nLevelMax;
    }
    
    long long effect = 5;
    effect += nLevel;
    
    return effect;
}
int DataManager::GetBonggiokMaxLevel(){
    return 300;
}
#pragma mark-

std::string DataManager::GetShopAdsName(E_SHOP_ADS ads){

    std::string strKey = "";
    switch (ads) {
        case E_SHOP_ADS::SHOP_GEM_ADS:          strKey = "t_ui_currency_gem";       break;
        case E_SHOP_ADS::SHOP_GOLD_ADS:         strKey = "t_ui_currency_gold";      break;
        case E_SHOP_ADS::SHOP_BUFF_ADS:         strKey = "t_ui_buff";               break;
        case E_SHOP_ADS::SHOP_MINE_ADS:         strKey = "t_ui_mineral";            break;
            
        case E_SHOP_ADS::SHOP_D_GEM_ADS:        strKey = "t_ui_currency_gem";       break;
        case E_SHOP_ADS::SHOP_D_GOLD_ADS:       strKey = "t_ui_currency_gold";      break;
        case E_SHOP_ADS::SHOP_D_BUFF_ADS:       strKey = "t_ui_buff";               break;
            
        default:
            break;
    }
    
    std::string strText = "";
    if ( strKey.length() != 0 )
    {
        strText = GAME_TEXT(strKey);
    }
    return strText;
}

std::string DataManager::GetShopName(E_SHOP shop){
    
    std::string strKey = "";
    switch (shop) {
        case E_SHOP::SHOP_HERO_GEM:         strKey = "t_item_d1_title_1";            break;
        case E_SHOP::SHOP_ATTACK_BONUS:     strKey = "t_item_d1_title_2";            break;
        case E_SHOP::SHOP_MOVE_BONUS:       strKey = "t_item_d1_title_3";            break;
        case E_SHOP::SHOP_COIN_BONUS:       strKey = "t_item_d1_title_4";            break;
        case E_SHOP::SHOP_GOLD_GEM:         strKey = "t_item_d1_title_5";            break;
        case E_SHOP::SHOP_GOLD_GEM_BIG:     strKey = "t_item_d1_title_6";            break;
        case E_SHOP::SHOP_FEVER:            strKey = "t_item_d1_title_7";            break;
        
        case E_SHOP::SHOP_D_MOVE_SPD:       strKey = "t_item_d2_title_1";            break;
        case E_SHOP::SHOP_D_ATK_SPD:        strKey = "t_item_d2_title_2";            break;
        case E_SHOP::SHOP_D_ATK_DMG:        strKey = "t_item_d2_title_3";            break;
        case E_SHOP::SHOP_D_QUEST_GOLD:     strKey = "t_item_d2_title_4";            break;
        case E_SHOP::SHOP_D_GOLD_GEM:       strKey = "t_item_d2_title_5";            break;

        default:
            break;
    }
    
    std::string strText = "";
    if ( strKey.length() != 0 )
    {
        strText = GAME_TEXT(strKey);
    }
    return strText;
}


std::string DataManager::GetShopExpain(E_SHOP shop){
    
    std::string strKey = "";
    switch (shop) {
        case E_SHOP::SHOP_HERO_GEM:         strKey = "t_item_d1_contents_1";            break;
        case E_SHOP::SHOP_ATTACK_BONUS:     strKey = "t_item_d1_contents_2";            break;
        case E_SHOP::SHOP_MOVE_BONUS:       strKey = "t_item_d1_contents_3";            break;
        case E_SHOP::SHOP_COIN_BONUS:       strKey = "t_item_d1_contents_4";            break;
        case E_SHOP::SHOP_GOLD_GEM:         strKey = "t_item_d1_contents_5";            break;
        case E_SHOP::SHOP_GOLD_GEM_BIG:     strKey = "t_item_d1_contents_6";            break;
            
        case E_SHOP::SHOP_D_MOVE_SPD:       strKey = "t_item_d2_contents_1";            break;
        case E_SHOP::SHOP_D_ATK_SPD:        strKey = "t_item_d2_contents_2";            break;
        case E_SHOP::SHOP_D_ATK_DMG:        strKey = "t_item_d2_contents_3";            break;
        case E_SHOP::SHOP_D_QUEST_GOLD:     strKey = "t_item_d2_contents_4";            break;
        case E_SHOP::SHOP_D_GOLD_GEM:       strKey = "t_item_d2_contents_5";            break;
            
        default:
            break;
    }
    
    std::string strText = "";
    if ( strKey.length() != 0 )
    {
        strText = GAME_TEXT(strKey);
    }
    return strText;
}

int DataManager::GetShopPrice(E_SHOP ads){
    
    int result = 0;
    if ( ads == E_SHOP::SHOP_HERO_GEM )
    {
        result = 20;
    }
    else if ( ads == E_SHOP::SHOP_FEVER )
    {
        result = 300;
    }
    
    return result;
}

#pragma mark-
std::string DataManager::GetCostumeIconPath(E_COSTUME type, E_COSTUME_IDX cidx){
    std::string str;
    if(type == E_COSTUME::BODY)
        str = "Assets/icon_costume/body_0";
    else if(type == E_COSTUME::HEAD)
        str = "Assets/icon_costume/head_0";
    else if(type == E_COSTUME::HAND)
        str = "Assets/icon_costume/hand_0";
    else if(type == E_COSTUME::FOOT)
        str = "Assets/icon_costume/foot_0";
    else if(type == E_COSTUME::CAPE)
        str = "Assets/icon_costume/cape_0";
    else if(type == E_COSTUME::JEWEL)
        str = "Assets/icon_costume/jewel_0";
    else if(type == E_COSTUME::SETITEM)
        str = "Assets/icon_costume/cape_0"; //set는 aura라서 불꽃 따로 처리 한다. 사용하진 않지만 파일명만 남겨둠.
    
    if (cidx > 9) {
        str.erase(str.size()-1);
    }
    str.append(MafUtils::toString(cidx)).append(".png");
    
    return str;
}
int DataManager::GetCostumeLegendMaterial(E_COSTUME type)
{
    if      (type == E_COSTUME::HEAD)    return 20;
    else if (type == E_COSTUME::BODY)    return 20;
    else if (type == E_COSTUME::CAPE)    return 18;
    else if (type == E_COSTUME::HAND)    return 18;
    else if (type == E_COSTUME::FOOT)    return 18;
    else if (type == E_COSTUME::JEWEL)    return 30;
    else                                return 100;
}
int DataManager::GetCostumeLegendTermsCount(E_COSTUME type)
{
    if (type == E_COSTUME::JEWEL) {
        return 40;
    }else{
        return 35;
    }
}
double DataManager::GetCostumeLegendEffect(E_COSTUME type, int level)
{
    if (level <= 0) {
        return 0;
    }
    
    double value = GetCostumeLegendValue(type);
    
    if (type == E_COSTUME::JEWEL)
    {
        double add = GetCostumeLegendIncrement(type);
        for (int i=1; i<level; i++) {
            value += add;
        }
    }
    else
    {
        double mul = GetCostumeLegendIncrement(type);
        for (int i=1; i<level; i++) {
            value *= mul;
        }
    }
    
    return value;
}
double DataManager::GetCostumeLegendValue(E_COSTUME type)
{
    if      (type == E_COSTUME::HEAD)    return 5;
    else if (type == E_COSTUME::BODY)    return 0.2;
    else if (type == E_COSTUME::CAPE)    return 4;
    else if (type == E_COSTUME::HAND)    return 10;
    else if (type == E_COSTUME::FOOT)    return 0.5;
    else if (type == E_COSTUME::JEWEL)    return 0.01;
    else                                return 0;
}
double DataManager::GetCostumeLegendIncrement(E_COSTUME type)
{
    if      (type == E_COSTUME::HEAD)    return 5;
    else if (type == E_COSTUME::BODY)    return 2;
    else if (type == E_COSTUME::CAPE)    return 5;
    else if (type == E_COSTUME::HAND)    return 10;
    else if (type == E_COSTUME::FOOT)    return 5;
    else if (type == E_COSTUME::JEWEL)    return 0.01;
    else                                return 0;
}
int DataManager::GetCostumeLegendMaxLevel()
{
    return 5;
}
double DataManager::GetCostumeLegendSetEffect(int count)
{
    if      (count == 1)    return 10.0;
    else if (count == 2)    return 5.0;
    else if (count == 3)    return 5.0;
    else if (count == 4)    return 0.5;
    else if (count == 5)    return 0.9;
    else                    return 0;
}
std::string DataManager::GetCostumeLegendName(E_COSTUME eType, E_COSTUME_IDX eIdx)
{
    auto objCostume = CostumeManager::getInstance()->getCostume(eType, eIdx);
    
    std::string strResult = MafUtils::format("LV.%d %s", CostumeManager::getInstance()->getLegendLevel(eType), objCostume->getTitle().c_str());
    return strResult;
}
std::string DataManager::GetCostumeLegendDesc(E_COSTUME eType, E_COSTUME_IDX eIdx)
{
    auto objCostume = CostumeManager::getInstance()->getCostume(eType, eIdx);
    
    std::string strReslut = MafUtils::format(objCostume->getDesc().c_str(), GetCostumeLegendEffect(eType, CostumeManager::getInstance()->getLegendLevel(eType)) * 100);
    return strReslut;
}

#pragma mark-
Color3B DataManager::GetAuraColor(E_COSTUME_IDX idx){
    switch (idx) {
        case E_COSTUME_IDX::IDX_1_DEFAULT :         return Color3B(255,255,255);
        case E_COSTUME_IDX::IDX_2 :                 return Color3B(207,206,181);
        case E_COSTUME_IDX::IDX_3 :                 return Color3B(248,224,90);
        case E_COSTUME_IDX::IDX_4 :                 return Color3B(122,130,147);
        case E_COSTUME_IDX::IDX_5 :                 return Color3B(83,3,255);
        case E_COSTUME_IDX::IDX_6 :                 return Color3B(255,209,3);
        case E_COSTUME_IDX::IDX_9 :                 return Color3B(255,2,44);
        case E_COSTUME_IDX::IDX_12 :                return Color3B(255,100,0);
        case E_COSTUME_IDX::IDX_30 :                return Color3B(0,0,0);
        case E_COSTUME_IDX::IDX_23 :                return Color3B(0,0,100);
        case E_COSTUME_IDX::IDX_14 :                return Color3B(125,125,125);
        case E_COSTUME_IDX::IDX_10 :                return Color3B(37,4,51);
        case E_COSTUME_IDX::IDX_28 :                return Color3B(242,255,0);
        case E_COSTUME_IDX::IDX_7 :                 return Color3B(0,161,24);
        case E_COSTUME_IDX::IDX_8 :                 return Color3B(75,159,204);
        case E_COSTUME_IDX::IDX_34 :                return Color3B(27,171,149);
        case E_COSTUME_IDX::IDX_16 :                return Color3B(101,31,77);
        case E_COSTUME_IDX::IDX_17 :                return Color3B(194,136,0);
        case E_COSTUME_IDX::IDX_18 :                return Color3B(194,0,90);
        case E_COSTUME_IDX::IDX_15 :                return Color3B(0,128,19);
        case E_COSTUME_IDX::IDX_25 :                return Color3B(0,215,200);
        case E_COSTUME_IDX::IDX_24 :                return Color3B(255,251,107);
        case E_COSTUME_IDX::IDX_36 :                return Color3B(105,135,85);
        case E_COSTUME_IDX::IDX_11 :                return Color3B(225,127,0);
        case E_COSTUME_IDX::IDX_19 :                return Color3B(255,198,153);
        case E_COSTUME_IDX::IDX_22 :                return Color3B(250,153,159);
        case E_COSTUME_IDX::IDX_27 :                return Color3B(43,0,0);
        case E_COSTUME_IDX::IDX_29 :                return Color3B(255,102,219);
        case E_COSTUME_IDX::IDX_39 :                return Color3B(192,255,255);
        case E_COSTUME_IDX::IDX_32 :                return Color3B(252,255,163);
        case E_COSTUME_IDX::IDX_33 :                return Color3B(51,50,62);
        case E_COSTUME_IDX::IDX_26 :                return Color3B(229,237,238);
        case E_COSTUME_IDX::IDX_31 :                return Color3B(23,43,104);
        case E_COSTUME_IDX::IDX_35 :                return Color3B(38,118,189);
        case E_COSTUME_IDX::IDX_37 :                return Color3B(255,63,56);
        case E_COSTUME_IDX::IDX_38 :                return Color3B(20,36,64);
        case E_COSTUME_IDX::IDX_13 :                return Color3B(158,255,31);
        case E_COSTUME_IDX::IDX_20 :                return Color3B(204,33,33);
        case E_COSTUME_IDX::IDX_21 :                return Color3B(178,73,7);
        case E_COSTUME_IDX::IDX_LEGEND :                return Color3B(29,35,36);
            
            
        default:                                    return Color3B(27,171,149);
    }
}
#pragma mark-

double DataManager::GetMineSpendTime(E_MINERAL mineral){
    
    if(mineral> E_MINERAL::MINERAL_6)
        mineral = E_MINERAL::MINERAL_7_SAPPHIRE;
    
    double time = 1260000;
    switch (mineral) {
        case E_MINERAL::MINERAL_1 :         time = 21600;  break;
        case E_MINERAL::MINERAL_2 :         time = 21600; break;
        case E_MINERAL::MINERAL_3 :         time = 17280; break;
        case E_MINERAL::MINERAL_4 :         time = 54000; break;
        case E_MINERAL::MINERAL_5 :         time = 43200; break;
        case E_MINERAL::MINERAL_6 :         time = 129600; break;
        case E_MINERAL::MINERAL_7_SAPPHIRE :  time = 1134000; break;
        default:                            time = 1134000; break;
    }
    // 2018년 5월 2일 은광석 20%감소(4800)
    // 2018년 4월 12일 동괴 20%감소(12000)
    // 2018년 4월 12일 은괴 20%감소(36000)
    // 2018년 6월 15일 전체광물 10%감소
    return time;
}

double DataManager::GetMinePickaxEffectData(int level)
{
    if (level > C_MAX_PICKAX) {
        level = C_MAX_PICKAX;
    }
    
    int value = 100;
    for (int i=1; i<=level-1; i++) {
        value += i/10 + 1;
    }

    return value;
}
double DataManager::GetMinePickaxUpgradeGem(int level)
{
    return 500 + ((level-1)*100);
}
std::string DataManager::GetMineMineralName(E_MINERAL mineral)
{
    switch (mineral) {
        case E_MINERAL::MINERAL_1 :   return GAME_TEXT("t_ui_mineral_stone_iron");
        case E_MINERAL::MINERAL_2 :   return GAME_TEXT("t_ui_mineral_stone_copper");
        case E_MINERAL::MINERAL_3 :   return GAME_TEXT("t_ui_mineral_stone_silver");
        case E_MINERAL::MINERAL_4 :   return GAME_TEXT("t_ui_mineral_piece_iron");
        case E_MINERAL::MINERAL_5 :   return GAME_TEXT("t_ui_mineral_piece_copper");
        case E_MINERAL::MINERAL_6 :   return GAME_TEXT("t_ui_mineral_piece_silver");
        case E_MINERAL::MINERAL_7_SAPPHIRE :    return GAME_TEXT("t_ui_mineral_sapphire");
        case E_MINERAL::MINERAL_8_OPAL :        return GAME_TEXT("t_ui_mineral_opal");
        case E_MINERAL::MINERAL_9_AQUA :        return GAME_TEXT("t_ui_mineral_aquamarine");
        case E_MINERAL::MINERAL_10_EMERALD :    return GAME_TEXT("t_ui_mineral_emerald");
        case E_MINERAL::MINERAL_11_AMETHYST :   return GAME_TEXT("t_ui_mineral_amethyst");
        default:                                return "";
    }
}
std::string DataManager::GetMineMineralPath(E_MINERAL mineral)
{
    switch (mineral) {
        case E_MINERAL::MINERAL_1 :         return "Assets/icon/ore_1.png";
        case E_MINERAL::MINERAL_2 :         return "Assets/icon/ore_2.png";
        case E_MINERAL::MINERAL_3 :         return "Assets/icon/ore_3.png";
        case E_MINERAL::MINERAL_4 :         return "Assets/icon/ore_4.png";
        case E_MINERAL::MINERAL_5 :         return "Assets/icon/ore_5.png";
        case E_MINERAL::MINERAL_6 :         return "Assets/icon/ore_6.png";
        case E_MINERAL::MINERAL_7_SAPPHIRE :  return "Assets/icon/ore_sp_1.png";
        case E_MINERAL::MINERAL_8_OPAL :      return "Assets/icon/ore_sp_2.png";
        case E_MINERAL::MINERAL_9_AQUA :      return "Assets/icon/ore_sp_3.png";
        case E_MINERAL::MINERAL_10_EMERALD :   return "Assets/icon/ore_sp_4.png";
        case E_MINERAL::MINERAL_11_AMETHYST :  return "Assets/icon/ore_sp_5.png";
        default:                            return "Assets/icon/ore_1.png";
    }
}

int DataManager::GetMineMineralCount(E_MINERAL mineral)
{
    return 1;
}





#pragma mark-
std::string DataManager::GetPrestigeKey(E_PLACE ePlace, int nFloor)
{
    std::string key = "0";
    
    if ( ePlace == E_PLACE::DG_NORMAL )
    {
        key = GetPrestigeKeyNormal(ePlace, nFloor);
    }
    else if ( ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        key = GetPrestigeKeyDevil(ePlace, nFloor);
    }
    
    return key;
}

std::string DataManager::GetPrestigeKeyDevil(E_PLACE ePlace, int nFloor)
{
    if ( nFloor < 100 )
        return "0";

    double key = 100;
    for ( int i = 100; i < nFloor; i++ )
    {
        float per = 0;
        if(i<=150)        per = 0.02;
        else if(i<=300)   per = 0.01;
        else              per = 0.005;

        key += key*per;
    }
    
    auto idx = UserInfoManager::getInstance()->getEquipDevil(E_EQUIP_TYPE::E_D_BODY);
    auto level = UserInfoManager::getInstance()->getEquipLevelDevil(E_EQUIP_TYPE::E_D_BODY);
    auto optionPer = (double)GetEquipOptionTotalRateDevil(E_EQUIP_TYPE::E_D_BODY, idx, level, E_EQUIP_OPTION::E_D_KEY);
    if(optionPer>0){
        key += key*optionPer*0.01;
    }
    
    std::string strKey = MafUtils::doubleToString(key);
    return strKey;
}

std::string DataManager::GetPrestigeKeyNormal(E_PLACE ePlace, int nFloor)
{
    int level11 = UserInfoManager::getInstance()->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_11_KEY);
    int reinforce11 = UserInfoManager::getInstance()->getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_11_KEY);
    int key11 = GetArtifactEffect(E_ARTIFACT::ARTI_11_KEY,level11, reinforce11);
    if ( key11 > 0 )
    {
        nFloor += key11;
    }
    
    if ( nFloor > 150000 )
    {
        nFloor = 150000;
    }
    
    double temp = 100;
    for ( int i = 100; i < nFloor; i++ )
    {
        float per = 0;
        if(i<=300)        per = 0.02;
        else if(i<=400)   per = 0.01;
        else if(i<=600)   per = 0.005;
        else if(i>600)    per = 0.004;
        
        temp += temp*per;
    }
    
    std::string key = MafUtils::doubleToString(temp);
    
    //골든키
    key = MafUtils::bigAddNum(key, GetPrestigeGoldenKeyNormal(key), false);
    
    //코스튬
    double costumeBuff = 1.0f;
    if(UserInfoManager::getInstance()->isFloorCheck(1000, false)){
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD,E_COSTUME_IDX::IDX_5)){
            costumeBuff += costumeBuff*0.5;
        }
    }
    if(UserInfoManager::getInstance()->isFloorCheck(1001, true)){
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD,E_COSTUME_IDX::IDX_4)){
            costumeBuff += costumeBuff*0.2;
        }
    }
    if(UserInfoManager::getInstance()->isFloorCheck(2001, true)){
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE,E_COSTUME_IDX::IDX_19)){
            costumeBuff += costumeBuff*0.25;
        }
    }
    if(UserInfoManager::getInstance()->isFloorCheck(2000, false)){
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD,E_COSTUME_IDX::IDX_21)){
            costumeBuff += costumeBuff*0.50;
        }
    }
    if(UserInfoManager::getInstance()->isFloorCheck(3001, true)){
        if(CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE,E_COSTUME_IDX::IDX_21)){
            costumeBuff += costumeBuff*0.30;
        }
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD,E_COSTUME_IDX::IDX_17)){
        if (UserInfoStackManager::getInstance()->getRevivalKeyBuff1() > 0) {
            double per = UserInfoStackManager::getInstance()->getRevivalKeyBuff1() * 0.05;   //5%씩증가
            costumeBuff += costumeBuff * per;
        }
    }
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_35)){
        if (UserInfoStackManager::getInstance()->getRevivalKeyBuff2() > 0) {
            double per = UserInfoStackManager::getInstance()->getRevivalKeyBuff2() * 0.005;   //0.5%씩증가
            costumeBuff += costumeBuff * per;
        }
    }
    
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::HAND,E_COSTUME_IDX::IDX_22)){
        
        int64_t nLength = ItemsMoneyManager::getInstance()->getGold().length();
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_23))
        {
            nLength = ItemsMoneyManager::getInstance()->getGoldLength();
        }
        
        double goldBuff = nLength * 0.005f;
        if (goldBuff > 0) {
            costumeBuff += costumeBuff * goldBuff;   //0.5%씩 증가
        }
    }
    
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::CAPE,E_COSTUME_IDX::IDX_29)){
        if(UserInfoManager::getInstance()->isFloorCheck(3000, false)){
            if (UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_KEY_BONUS) > 0) {
                costumeBuff += costumeBuff*0.30;
            }
        }
    }
    
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::BODY,E_COSTUME_IDX::IDX_34)){
        if(UserInfoManager::getInstance()->isFloorCheck(5000, false))
            costumeBuff += costumeBuff * 0.30;   //5000층 이하에서 획득량 300%
    }
    
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL,E_COSTUME_IDX::IDX_7)){
        int count = CostumeManager::getInstance()->getExistTypeCount(E_COSTUME::CAPE);
        double per = count * 0.005f;
        if (per > 0) {
            costumeBuff += costumeBuff * per;
        }
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD,E_COSTUME_IDX::IDX_39)) {
        //지감 보물 열쇠량 증가 여기에도 추가
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY,E_COSTUME_IDX::IDX_6)) {
            costumeBuff += costumeBuff*0.2f;
        }
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD,E_COSTUME_IDX::IDX_13)) {
            costumeBuff += costumeBuff*0.2f;
        }
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY,E_COSTUME_IDX::IDX_15)) {
            costumeBuff += costumeBuff*0.3f;
        }
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY,E_COSTUME_IDX::IDX_17)) {
            costumeBuff += costumeBuff*0.4f;
        }
    }
    
    
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY,E_COSTUME_IDX::IDX_LEGEND)) {
        double per = DataManager::GetCostumeLegendEffect(E_COSTUME::BODY, CostumeManager::getInstance()->getLegendLevel(E_COSTUME::BODY));
        if (per > 0) {
            costumeBuff += costumeBuff * per;
        }
    }
    
    if (CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_16)) {
        
        int64_t nLength = ItemsMoneyManager::getInstance()->getKey().length();
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_23))
        {
            nLength = ItemsMoneyManager::getInstance()->getKeyLength();
        }
        
        double keyBuff = nLength * 0.01f;
        if (keyBuff > 0) {
            costumeBuff += costumeBuff * keyBuff;   //20%씩 증가
        }
    }
    
    if (CostumeManager::getInstance()->getLegendCount() >= 4) {
        costumeBuff += costumeBuff * DataManager::GetCostumeLegendSetEffect(4);
    }
    
    int reinforceLevel = PetNewManager::getInstance()->getPetReinforce(E_PET::PURIRING_5);
    double reinforcePer = PetNewManager::getInstance()->getPetReinforceEffect(E_PET::PURIRING_5, reinforceLevel);
    if (reinforcePer > 0) {
        costumeBuff *= reinforcePer;
    }
    
    // 뉴비 지원 버프
    if ( UserInfoConfigManager::getInstance()->getNewbieStep() >= E_NEWBIE_STEP::STEP_1 && UserInfoConfigManager::getInstance()->getNewbieStep() <= E_NEWBIE_STEP::STEP_5 )
    {
        E_NEWBIE_STEP eNewbie = UserInfoConfigManager::getInstance()->getNewbieStep();
        
        double per = 0.0;
        if ( eNewbie == E_NEWBIE_STEP::STEP_1 )
            per = 2.5;
        else if ( eNewbie == E_NEWBIE_STEP::STEP_2 )
            per = 2.0;
        else if ( eNewbie == E_NEWBIE_STEP::STEP_3 )
            per = 1.5;
        else if ( eNewbie == E_NEWBIE_STEP::STEP_4 )
            per = 1.0;
        else if ( eNewbie == E_NEWBIE_STEP::STEP_5 )
            per = 0.5;
        
        if ( per > 0.0 )
        {
            costumeBuff += costumeBuff * per;
        }
    }
    
    costumeBuff *= 100;
    key = MafUtils::bigMulNum(key, MafUtils::doubleToString(costumeBuff));
    key.pop_back();
    key.pop_back();
    
    //
    double stoneBuff = 1.0f;
    stoneBuff += StoneManager::getInstance()->getEffect(E_STONE::KEY);
    stoneBuff *= 100;
    key = MafUtils::bigMulNum(key, MafUtils::doubleToString(stoneBuff));
    if (key.length()>2) {
        key.pop_back();
        key.pop_back();
    }
    
    double stoneForceBuff = 1.0f;
    stoneForceBuff += StoneManager::getInstance()->getEffect(E_STONE::FORCE_KEY);
    stoneForceBuff *= 100;
    key = MafUtils::bigMulNum(key, MafUtils::doubleToString(stoneForceBuff));
    if (key.length()>2) {
        key.pop_back();
        key.pop_back();
    }
    
    double stoneForce2Buff = 1.0f;
    stoneForce2Buff += StoneManager::getInstance()->getEffect(E_STONE::FORCE_2_KEY);
    stoneForce2Buff *= 100;
    key = MafUtils::bigMulNum(key, MafUtils::doubleToString(stoneForce2Buff));
    if (key.length()>2) {
        key.pop_back();
        key.pop_back();
    }
    
    //봉식타워3
    int buildinglevel = TownManager::getInstance()->getTownLevel(E_TOWN::BUILDING_3);
    if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_5)){
        buildinglevel += 7;
    }
    if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_28)){
        buildinglevel += PetNewManager::getInstance()->getPetLevel(E_PET::HAMSTER_2);
    }
    double relicPer = DataManager::GetRelicEffect(E_RELIC::RELIC_9, UserInfoManager::getInstance()->getRelicLevel(E_PLACE::DG_NORMAL, E_RELIC::RELIC_9));
    if (relicPer > 0) {
        buildinglevel += relicPer;
    }
    
    double buildingPer = TownManager::getInstance()->getTownEffect(E_TOWN::BUILDING_3, buildinglevel);
    if(buildingPer>0){
        auto tt = MafUtils::bigMulNum(key, MafUtils::doubleToString(buildingPer));
        tt.pop_back();
        tt.pop_back();
        key = MafUtils::bigAddNum(key, tt);
    }
    
    //정령 효과
    key = WeaponSpiritManager::getInstance()->getSpiritCalculation(key, WeaponSpiritManager::E_EFFECT_TYPE::KEY);
    
    //모험 유물 적용
    double advtRelicBuff = 100.f;
    advtRelicBuff += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::AREA_GRASS_RELIC_LEVEL_KEY);
    advtRelicBuff += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::AREA_DESERT_RELIC_LEVEL_KEY);
    advtRelicBuff += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::AREA_SNOW_RELIC_LEVEL_KEY);
    advtRelicBuff += AdventureRelicManager::getInstance()->getRelicTotalEffect(E_ADVENTURE_PROPERTY_TYPE::AREA_VOLCANO_RELIC_LEVEL_KEY);
       
    key = MafUtils::bigMulNum(key, MafUtils::doubleToString(advtRelicBuff));
    key.pop_back();
    key.pop_back();
    
    //new raid key
    float raidAccrueBuff = 100.f;
    
    raidAccrueBuff += NewRaidManager::getInstance()->getAccrueEffect(E_RAID_ACCRUE_EFFECT_TYPE::KEY);
    key = MafUtils::bigMulNum(key, MafUtils::doubleToString(raidAccrueBuff));
    if ( key.length() > 2 )
    {
        key.pop_back();
        key.pop_back();
    }
    else
    {
        key = "0";
    }
    
    
    
    //보너스 계산
    key = MafUtils::bigAddNum(key, GetPrestigeBonusKeyNormal(key));
    
    //초월보물
    //증폭기 주석처리 (추가 예정)
//    if(UserInfoManager::getInstance()->getArtifactLevel(E_ARTIFACT::ARTI_105_INFINITE_DISCOUNT) >= 1)
//    {
//        double infiniteKeyBonus = DataManager::GetArtifactEffect(E_ARTIFACT::ARTI_105_INFINITE_DISCOUNT, UserInfoManager::getInstance()->getArtifactLevel(E_ARTIFACT::ARTI_105_INFINITE_DISCOUNT), UserInfoManager::getInstance()->getArtifactReinforce(ARTI_105_INFINITE_DISCOUNT));
//
//        auto tt = MafUtils::bigMulNum(key, MafUtils::doubleToString(infiniteKeyBonus));
//        tt.pop_back();
//        tt.pop_back();
//        key = MafUtils::bigAddNum(key, tt);
//    }
    

    return key;
}

std::string DataManager::GetPrestigeBonusKeyNormal(std::string value)
{
    std::string result = "0";
    
    if (UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_KEY_BONUS) > 0)
    {
        int keyBouns = 3;
        if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY,E_COSTUME_IDX::IDX_23)) {
            keyBouns = 6;
        }
        
        result = MafUtils::bigMulNum(value, MafUtils::toString(keyBouns));//30%
        result.pop_back();
    }
    
    return result;
}

std::string DataManager::GetPrestigeGoldenKeyNormal(std::string value)
{
    std::string result = "0";
    
    std::string tmp;
    int level = UserInfoManager::getInstance()->getArtifactLevel(DG_NORMAL, E_ARTIFACT::ARTI_9_KEY);
    int reinforce = UserInfoManager::getInstance()->getArtifactReinforce(DG_NORMAL, E_ARTIFACT::ARTI_9_KEY);
    if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_6))
    {
        level += 10;
    }
    
    auto keyPer = GetArtifactEffect(E_ARTIFACT::ARTI_9_KEY, level, reinforce);
    if ( keyPer > 0 )
    {
        int level10 = UserInfoManager::getInstance()->getArtifactLevel(DG_NORMAL, E_ARTIFACT::ARTI_10_KEY);
        int reinforce10 = UserInfoManager::getInstance()->getArtifactReinforce(DG_NORMAL, E_ARTIFACT::ARTI_10_KEY);
        auto key10 = GetArtifactEffect(E_ARTIFACT::ARTI_10_KEY, level10, reinforce10);
        if(key10>0)
        {
            keyPer = (keyPer*100) + (keyPer*key10);
            tmp = MafUtils::bigMulNum(value, MafUtils::doubleToString(keyPer),false);
            if (tmp.length() > 4)
            {
                tmp.pop_back();
                tmp.pop_back();
                tmp.pop_back();
                tmp.pop_back();
            }
        }
        else
        {
            tmp = MafUtils::bigMulNum(value, MafUtils::doubleToString(keyPer),false);
            if (tmp.length() > 2)
            {
                tmp.pop_back();
                tmp.pop_back();
            }
        }
        result = tmp;
    }
    
    return result;
}

std::string DataManager::GetDungeonEarnKey(int kill)
{
    E_PLACE ePlace = E_PLACE::DG_NORMAL;
    
    if ( kill > 26500 )
    {
        kill = 26500;
    }

    std::string key = "20";
    if(kill<=20)        key = "36";
    else if(kill<=40)   key = "65";
    else if(kill<=60)   key = "117";
    else if(kill<=80)   key = "210";
    else if(kill<=100)   key = "378";
    else if(kill<=120)   key = "680";
    else if(kill<=140)   key = "1224";
    else if(kill<=160)   key = "2204";
    else if(kill<=180)   key = "3526";
    else if(kill<=200)   key = "5642";
    else if(kill<=220)   key = "8463";
    else if(kill<=240)   key = "10156";
    else if(kill<=260)   key = "10968";
    else if(kill<=280)   key = "11517";
    else if(kill<=300)   key = "11862";
    else if(kill>300)
    {
        key = "11862";
        
        std::string temp;
        int count = kill-301;
        count = count/20+1;
        
        double per = 1.03;
        double cal = pow(per, count)*100;
        
        temp = MafUtils::doubleToString(cal);
        key = MafUtils::bigMulNum(key, temp);
        key.pop_back();
        key.pop_back();
    }
    
    // 2018년 4월 12일 지하감옥 열쇠상자 50% 상향
    std::string tmp = MafUtils::bigMulNum(key, MafUtils::toString(5));
    tmp.pop_back();
    
    key = MafUtils::bigAddNum(key, tmp);
    

    int level = UserInfoManager::getInstance()->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_9_KEY);
    int reinforce = UserInfoManager::getInstance()->getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_9_KEY);
    if(CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_6)){
        level += 10;
    }
    auto keyPer = GetArtifactEffect(E_ARTIFACT::ARTI_9_KEY, level, reinforce);
    if ( keyPer > 0 )
    {
        int level10 = UserInfoManager::getInstance()->getArtifactLevel(ePlace, E_ARTIFACT::ARTI_10_KEY);
        int reinforce10 = UserInfoManager::getInstance()->getArtifactReinforce(ePlace, E_ARTIFACT::ARTI_10_KEY);
        
        auto key10 = GetArtifactEffect(E_ARTIFACT::ARTI_10_KEY, level10, reinforce10);
        if ( key10 > 0 )
        {
            keyPer = (keyPer*100) + (keyPer*key10);
            tmp = MafUtils::bigMulNum(key, MafUtils::doubleToString(keyPer),false);
            if (tmp.length() > 4)
            {
                tmp.pop_back();
                tmp.pop_back();
                tmp.pop_back();
                tmp.pop_back();
            }
        }
        else
        {
            tmp = MafUtils::bigMulNum(key, MafUtils::doubleToString(keyPer),false);
            if (tmp.length() > 2)
            {
                tmp.pop_back();
                tmp.pop_back();
            }
        }
        key = MafUtils::bigAddNum(key, tmp,false);
    }
    
    
    //코스튬
    double costumeBuff = 1.0f;
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY,E_COSTUME_IDX::IDX_6)) {
        costumeBuff += costumeBuff*0.2f;
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::HEAD,E_COSTUME_IDX::IDX_13)) {
        if (UserInfoManager::getInstance()->isMeterCheck(1000)) {
            costumeBuff += costumeBuff*0.2f;
        }
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY,E_COSTUME_IDX::IDX_15)) {
        costumeBuff += costumeBuff*0.3f;
    }
    if (CostumeManager::getInstance()->isEquip(E_COSTUME::BODY,E_COSTUME_IDX::IDX_17)) {
        costumeBuff += costumeBuff*0.4f;
    }
    
    if(CostumeManager::getInstance()->isEquip(E_COSTUME::FOOT,E_COSTUME_IDX::IDX_34)){
        if (UserInfoManager::getInstance()->isMeterCheck(5000)) {
            costumeBuff += costumeBuff * 0.30;   //5000m 이하에서 획득량 30%
        }
    }
    
    long long costumeBuff2 = costumeBuff*100+0.01f;
    key = MafUtils::bigMulNum(key, MafUtils::toString(costumeBuff2), false);
    if (key.length()>2) {
        key.pop_back();
        key.pop_back();
    }
    
    //
    double stoneBuff = 1.0f;
    stoneBuff += StoneManager::getInstance()->getEffect(E_STONE::KEY);
    stoneBuff *= 100;
    key = MafUtils::bigMulNum(key, MafUtils::doubleToString(stoneBuff));
    if (key.length()>2) {
        key.pop_back();
        key.pop_back();
    }
    
    double stoneForceBuff = 1.0f;
    stoneForceBuff += StoneManager::getInstance()->getEffect(E_STONE::FORCE_KEY);
    stoneForceBuff *= 100;
    key = MafUtils::bigMulNum(key, MafUtils::doubleToString(stoneForceBuff));
    if (key.length()>2) {
        key.pop_back();
        key.pop_back();
    }
    
    double stoneForce2Buff = 1.0f;
    stoneForce2Buff += StoneManager::getInstance()->getEffect(E_STONE::FORCE_2_KEY);
    stoneForce2Buff *= 100;
    key = MafUtils::bigMulNum(key, MafUtils::doubleToString(stoneForce2Buff));
    if (key.length()>2) {
        key.pop_back();
        key.pop_back();
    }
    
    if (MafUtils::compareStr(key, "9200000000000000000")) {
        key = "9200000000000000000";
    }
    
    return key;
}


#pragma mark - prison jelly
std::string DataManager::GetJellyIcon(E_JELLY type)
{
    std::string path = "";
    switch (type) {
        case E_JELLY::JELLY_TIME:       path = "Assets/icon/icon_time_1_2.png"; break;
        case E_JELLY::JELLY_DOUBLE:     path = "Assets/icon/icon_double_freepass.png"; break;
        case E_JELLY::JELLY_EXPAND:     path = "Assets/icon/icon_locker.png"; break;
        default:
            break;
    }
    
    return path;
}

std::string DataManager::GetJellyName(E_JELLY type)
{
    std::string strKey = "";
    switch (type) {
        case E_JELLY::JELLY_TIME:       strKey = "t_ui_prison_jelly_item_title_1";            break;
        case E_JELLY::JELLY_DOUBLE:     strKey = "t_ui_prison_jelly_item_title_2";            break;
        case E_JELLY::JELLY_EXPAND:        strKey = "t_ui_prison_jelly_item_title_3";            break;
        default:
            break;
    }
    
    std::string strText = "";
    if ( strKey.length() != 0 )
    {
        strText = GAME_TEXT(strKey);
    }
    return strText;
}

std::string DataManager::GetJellyExplain(E_JELLY type)
{
    std::string strKey = "";
    switch (type) {
        case E_JELLY::JELLY_TIME:       strKey = "t_ui_prison_jelly_item_contents_1";            break;
        case E_JELLY::JELLY_DOUBLE:     strKey = "t_ui_prison_jelly_item_contents_2";            break;
        case E_JELLY::JELLY_EXPAND:        strKey = "t_ui_prison_jelly_item_contents_3";            break;
        default:
            break;
    }
    
    std::string strText = "";
    if ( strKey.length() != 0 )
    {
        strText = GAME_TEXT(strKey);
    }
    return strText;
}

std::string DataManager::GetJellyOptionKey(E_JELLY type)
{
    std::string strKey = "";
    switch (type) {
        case E_JELLY::JELLY_TIME:       strKey = "t_ui_prison_jelly_item_option_1";            break;
        case E_JELLY::JELLY_DOUBLE:     strKey = "t_ui_prison_jelly_item_option_2";            break;
        case E_JELLY::JELLY_EXPAND:        strKey = "t_ui_prison_jelly_item_option_3";            break;
        default:
            break;
    }
    
    return strKey;
}

int DataManager::GetJellyUpgrade(E_JELLY type, int level)
{
    int listTime[] = {10,20,30,40,50,50,60,60,70,70,80,80,90,90,100,100,110,110,120,120,130,130};
    int listDouble[] = {3,3,3,3,3,5,5,5,5,5,10,15,20,25,30,35,40,45,50,55};
    int listBox[] = {50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70};
    int value = 0;
    
    switch (type) {
        case E_JELLY::JELLY_TIME:       value = listTime[level];    break;
        case E_JELLY::JELLY_DOUBLE:     value = listDouble[level];  break;
        case E_JELLY::JELLY_EXPAND:        value = listBox[level];     break;
        default:
            value = 99999;
            break;
    }

    return value;
}

int DataManager::GetJellyEffect(E_JELLY type, int level)
{
    int value = 0;
    switch (type) {
        case E_JELLY::JELLY_TIME:       value = 1800; break;
        case E_JELLY::JELLY_DOUBLE:     value = 0; break;
        case E_JELLY::JELLY_EXPAND:        value = 20; break;
        default:
            break;
    }
    
    switch (type) {
        case E_JELLY::JELLY_TIME:
            value -= (level)*60; break;
        case E_JELLY::JELLY_DOUBLE:
            value += level; break;
        case E_JELLY::JELLY_EXPAND:
            value += level; break;
        default:
            break;
    }
    return value;
}

int DataManager::GetJellyMaxLevel(E_JELLY type)
{
    int max = 1;
    switch (type) {
        case E_JELLY::JELLY_TIME:       max = 22;            break;
        case E_JELLY::JELLY_DOUBLE:     max = 20;            break;
        case E_JELLY::JELLY_EXPAND:        max = 50;            break;
        default:
            break;
    }
    
    return max;
}

#pragma mark - event
bool DataManager::isEventChristmas()
{
    time_t timer_s = UtilsDate::getInstance()->getTime();
    if ( timer_s <= 1640908800 )
    {
        return true;
    }
    return false;
}

bool DataManager::isEvent5th()
{
    return false;
}

bool DataManager::isNewYear()
{
    time_t timer_s = UtilsDate::getInstance()->getTime();
    if ( timer_s <= 1644364800 )
    {
        return true;
    }
    
    return false;
}

bool DataManager::isEventSummer()
{
    return false;
}

bool DataManager::isEventThanksgiving()
{
    return false;
}

bool DataManager::isEventHalloween()
{
    return false;
}

bool DataManager::isEventFoolsDay()
{
    time_t timer_s = UtilsDate::getInstance()->getTime();
    if ( timer_s >= 1648771200 &&
        timer_s <= 1648857599)
    {
        return true;
    }
    return false;
}

E_NEWBIE_STEP DataManager::GetNewbieStep(int highFloor, int totalFloor, int specialMission)
{
    auto value = MafRemoteConfig::GetRemoteValueInt("newbie_start_mission");
    if(value > specialMission)
        return E_NEWBIE_STEP::STEP_NONE;
    
    E_NEWBIE_STEP newbie = E_NEWBIE_STEP::STEP_NONE;
    
    if(highFloor < 2000 && totalFloor < 200000){
        if(highFloor >= 10)
            newbie = E_NEWBIE_STEP::STEP_1;
    }
    else if(highFloor < 4000 && totalFloor < 400000){
        newbie = E_NEWBIE_STEP::STEP_2;
    }
    else if(highFloor < 6000 && totalFloor < 600000){
        newbie = E_NEWBIE_STEP::STEP_3;
    }
    else if(highFloor < 8000 && totalFloor < 800000){
        newbie = E_NEWBIE_STEP::STEP_4;
    }
    else if(highFloor < 10000 && totalFloor < 1000000){
        newbie = E_NEWBIE_STEP::STEP_5;
    }
    else{
        newbie = E_NEWBIE_STEP::STEP_FINISH;
    }
    
    return newbie;
}

void DataManager::requestPushInfo(std::string strKey)
{
    std::vector<std::string> listPush;
    if ( strKey.length() == 0 )
    {
        listPush.push_back("prison");       // 지하감옥 무료입장권
        listPush.push_back("jelly");        // 지하감옥 젤리탐험
    }
    else
    {
        listPush.push_back(strKey);
    }
    
    requestPushInfo(listPush);
}

void DataManager::requestPushInfo(std::vector<std::string> listPush)
{
    std::vector<std::string> listPushTemp;
    
    for ( std::string strTemp : listPush )
    {
        if ( strTemp.compare("prison") == 0 && UserInfoConfigManager::getInstance()->isAlarm(E_ALARM_ACTIVE::PRISON_ENTER) == true )
        {
            listPushTemp.push_back(strTemp);
        }
        else if ( strTemp.compare("jelly") == 0 && UserInfoConfigManager::getInstance()->isAlarm(E_ALARM_ACTIVE::JELLY) == true )
        {
            listPushTemp.push_back(strTemp);
        }
    }
    
    if ( listPushTemp.size() == 0 )
    {
        MafNative::CancelLocalNotification(E_NOTI::NOTI_PRISON_ENTER);
        MafNative::CancelLocalNotification(E_NOTI::NOTI_PRISON_JELLY);
        return;
    }
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, "/push/info");
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_push_list", listPushTemp);
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responsePushInfo(response,dd);
    };
    req->send(cb);
}

void DataManager::responsePushInfo(network::HttpResponse* response,std::string &data)
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
        return;
    }
    
    /*
     NOTI_RAID = 1001,
     NOTI_RAID_BONGGIOK = 1002,
     NOTI_PRISON_ENTER = 1003,
     NOTI_PRISON_JELLY = 1004,
     */
    
    MafNative::CancelLocalNotification(E_NOTI::NOTI_PRISON_ENTER);
    MafNative::CancelLocalNotification(E_NOTI::NOTI_PRISON_JELLY);
    if ( jsonParser.HasMember("prison") == true )
    {
        int64_t nTime = jsonParser["prison"].GetInt64() - UtilsDate::getInstance()->getTime();
        if ( nTime > 0 )
        {
            MafNative::SendLocalNotification(E_NOTI::NOTI_PRISON_ENTER, (int)nTime, GAME_TEXT("t_ui_push_prison_enter_title").c_str(), GAME_TEXT("t_ui_push_prison_enter_contents").c_str());
        }
    }
    if ( jsonParser.HasMember("jelly") == true )
    {
        int64_t nTime = jsonParser["jelly"].GetInt64() - UtilsDate::getInstance()->getTime();
        if ( nTime > 0 )
        {
            MafNative::SendLocalNotification(E_NOTI::NOTI_PRISON_JELLY, (int)nTime, GAME_TEXT("t_ui_push_prison_jelly_title").c_str(), GAME_TEXT("t_ui_push_prison_jelly_contents").c_str());
        }
    }
}


void DataManager::requestPushTopic(std::string strKey, bool bSubscribe)
{
    std::vector<std::string> listPush;
    listPush.push_back(strKey);
    
    // raid
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, "/push/subscribe");
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_push_list", listPush);
    req->setData("_subscribe", bSubscribe);
    req->send(nullptr);
}
