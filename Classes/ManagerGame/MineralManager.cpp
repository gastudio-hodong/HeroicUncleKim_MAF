//
//  MineralManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF on 2022/05/12.
//

#include "MineralManager.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/UserInfoConfigManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/PassManager.h"
#include "ManagerGame/ServerMissionManager.h"
#include "ManagerGame/WeaponManager.h"

USING_NS_CC;

MineralManager::MineralManager(void):
_ePlace(E_PLACE::DG_NORMAL),

//
_eMineralSpecial(E_MINERAL::MINERAL_NONE),

//
_uMineRefineOpen(0),
_uMineRefineBuyResetTime(0),
_uMineRefineBuyCount(0),
_uMineRefineBuyAdsResetTime(0),
_uMineRefineBuyAdsCount(0),
_uMineRefineProductionCount(0),
_uMineRefineProductionStartTime(0),
_uMineRefineProductionItem(0)
{
    
}

MineralManager::~MineralManager(void)
{
    
}

bool MineralManager::init()
{
    _listMine.push_back(E_MINERAL::MINERAL_1);
    _listMine.push_back(E_MINERAL::MINERAL_2);
    _listMine.push_back(E_MINERAL::MINERAL_3);
    _listMine.push_back(E_MINERAL::MINERAL_4);
    _listMine.push_back(E_MINERAL::MINERAL_5);
    _listMine.push_back(E_MINERAL::MINERAL_6);
    _listMine.push_back(E_MINERAL::MINERAL_7_SAPPHIRE);
    
    for ( int i = 0; i < sizeof(_uMineMineralCount) / sizeof(TInteger); i++ )
    {
        _uMineMineralCount[i] = 0;
    }
    
    for ( int i = 0; i < sizeof(_uMineBuyMineral) / sizeof(TInteger); i++ )
    {
        _uMineBuyMineral[i] = 0;
    }

    for ( int i = 0; i < sizeof(_uMineTime) / sizeof(double); i++ )
    {
        _uMineTime[i]= DataManager::GetMineSpendTime((E_MINERAL)i);
    }
    
    return true;
}

#pragma mark - update
void MineralManager::update(float dt)
{
    {// 광물 획득
        if ( WeaponManager::getInstance()->getEquipHigh() < E_WEAPON::WEAPON_76_MINE )
        {
            return;
        }
        
        for ( auto& eType : _listMine )
        {
            if ( eType >= E_MINERAL::MINERAL_7_SAPPHIRE )
            {
                eType = getSpecialMineral();
            }
            
            double pickaxEffect = getPickaxEffect(eType);
            
            double time = getTimeReal(eType);
            double timeMax = DataManager::GetMineSpendTime(eType);
            
            time -= (pickaxEffect * dt);
            if ( time <= 0 )
            {
                time = timeMax;
                
                int countAdd = getCountAdd(eType);
                int count = getCount(eType) + countAdd;
                count = count + (countAdd * PetNewManager::getInstance()->getPetEquipEffect(703, 0));
                
                setCount(eType, count);
            }
            setTime(eType, time);
        }
    }
}


#pragma mark - load
void MineralManager::saveData()
{
    std::stringstream str;
    
    // buy
    str.str("");
    for ( int i = 1; i <= C_COUNT_MINE_QUEST; i++ )
    {
        str << getBuyCount((E_MINERAL)i) << ",";
    }
    UserDefault::getInstance()->setStringForKey(KEY_MINE_BUY_MINERAL, MafAes256::Encrypt(str.str()));
    
    // count
    str.str("");
    for ( int i = 1; i <= C_COUNT_MINE_MINERAL; i++ )
    {
        str << getCount((E_MINERAL)i) << ",";
    }
    UserDefault::getInstance()->setStringForKey(KEY_MINE_MINERAL, MafAes256::Encrypt(str.str()));
    
    // time
    str.str("");
    for ( int i = 1; i <= C_COUNT_MINE_QUEST; i++ )
    {
        str << MafUtils::doubleToString(getTimeReal((E_MINERAL)i)) << ",";;
    }
    UserDefault::getInstance()->setStringForKey(KEY_MINE_TIME, MafAes256::Encrypt(str.str()));
    
    // refine open
    str.str("");
    str << MafUtils::toString(_uMineRefineOpen.valueInt());
    UserDefault::getInstance()->setStringForKey(KEY_MINE_REFINE_OPEN, MafAes256::Encrypt(str.str()));
    
    // refine buy reset time
    str.str("");
    str << MafUtils::toString(_uMineRefineBuyResetTime.value());
    UserDefault::getInstance()->setStringForKey(KEY_MINE_REFINE_BUY_RESETTIME, MafAes256::Encrypt(str.str()));
    
    // refine buy count
    str.str("");
    str << MafUtils::toString(_uMineRefineBuyCount.valueInt());
    UserDefault::getInstance()->setStringForKey(KEY_MINE_REFINE_BUY_COUNT, MafAes256::Encrypt(str.str()));
    
    // refine buy ads reset time
    str.str("");
    str << MafUtils::toString(_uMineRefineBuyAdsResetTime.value());
    UserDefault::getInstance()->setStringForKey(KEY_MINE_REFINE_BUY_ADS_RESETTIME, MafAes256::Encrypt(str.str()));
    
    // refine buy ads count
    str.str("");
    str << MafUtils::toString(_uMineRefineBuyAdsCount.valueInt());
    UserDefault::getInstance()->setStringForKey(KEY_MINE_REFINE_BUY_ADS_COUNT, MafAes256::Encrypt(str.str()));
    
    // refine production count
    str.str("");
    str << MafUtils::toString(_uMineRefineProductionCount.valueInt());
    UserDefault::getInstance()->setStringForKey(KEY_MINE_REFINE_PRODUCTION_COUNT, MafAes256::Encrypt(str.str()));
    
    // refine production start time
    str.str("");
    str << MafUtils::toString(_uMineRefineProductionStartTime.value());
    UserDefault::getInstance()->setStringForKey(KEY_MINE_REFINE_PRODUCTION_STARTTIME, MafAes256::Encrypt(str.str()));
    
    // refine production item
    str.str("");
    str << MafUtils::toString(_uMineRefineProductionItem.valueInt());
    UserDefault::getInstance()->setStringForKey(KEY_MINE_REFINE_PRODUCTION_ITEM, MafAes256::Encrypt(str.str()));
    
    // refine upgrade level
    str.str("");
    for( const auto& obj : _listInfoRefineUpgrade )
    {
        int idx = obj->getIdx();
        
        if ( str.str().length() != 0 )
        {
            str << ",";
        }
        str << MafUtils::format("%d_%d", idx, getRefineUpgradeLevel(idx));
    }
    UserDefault::getInstance()->setStringForKey(KEY_MINE_REFINE_UPGRADE_LEVEL, MafAes256::Encrypt(str.str()));
}

void MineralManager::loadData()
{
    std::string str = "";
    
    // buy
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_MINE_BUY_MINERAL);
    if ( !str.empty() )
    {
        str = MafAes256::Decrypt(str);
        
        for(int i=1; i <= C_COUNT_MINE_QUEST; i++)
        {
            auto offset = str.find(",");
            if(offset != std::string::npos)
            {
                auto result = str.substr(0,offset);
                setBuyCount((E_MINERAL)i, std::atoi(result.c_str()));
                
                str = str.substr(offset+1);
            }
            else
            {
                setBuyCount((E_MINERAL)i, std::atoi(str.c_str()));
                break;
            }
        }
    }
    
    // count
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_MINE_MINERAL);
    if ( !str.empty() )
    {
        str = MafAes256::Decrypt(str);
        
        for(int i=1; i <= C_COUNT_MINE_MINERAL; i++)
        {
            auto offset = str.find(",");
            if(offset != std::string::npos)
            {
                auto result = str.substr(0,offset);
                setCount((E_MINERAL)i, std::atoi(result.c_str()));
                
                str = str.substr(offset+1);
            }
            else
            {
                setCount((E_MINERAL)i, std::atoi(str.c_str()));
                break;
            }
        }
    }
    
    // time
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_MINE_TIME);
    if ( !str.empty() )
    {
        str = MafAes256::Decrypt(str);
        
        for(int i=1; i <= C_COUNT_MINE_QUEST; i++)
        {
            auto offset = str.find(",");
            if(offset != std::string::npos)
            {
                auto result = str.substr(0,offset);
                setTime((E_MINERAL)i, std::atoi(result.c_str()));
                
                str = str.substr(offset+1);
            }
            else
            {
                setTime((E_MINERAL)i, std::atoi(str.c_str()));
                break;
            }
        }
    }
    
    // refine open
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_MINE_REFINE_OPEN);
    if ( !str.empty() )
    {
        _uMineRefineOpen = std::atoi(MafAes256::Decrypt(str).c_str());
    }
    else
    {
        _uMineRefineOpen = 0;
    }
    
    // refine buy reset time
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_MINE_REFINE_BUY_RESETTIME);
    if ( !str.empty() )
    {
        _uMineRefineBuyResetTime = std::atoll(MafAes256::Decrypt(str).c_str());
    }
    else
    {
        _uMineRefineBuyResetTime = 0;
    }
    
    // refine buy count
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_MINE_REFINE_BUY_COUNT);
    if ( !str.empty() )
    {
        _uMineRefineBuyCount = std::atoi(MafAes256::Decrypt(str).c_str());
    }
    else
    {
        _uMineRefineBuyCount = 0;
    }
    
    // refine buy ads reset time
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_MINE_REFINE_BUY_ADS_RESETTIME);
    if ( !str.empty() )
    {
        _uMineRefineBuyAdsResetTime = std::atoi(MafAes256::Decrypt(str).c_str());
    }
    else
    {
        _uMineRefineBuyAdsResetTime = 0;
    }
    
    // refine buy ads count
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_MINE_REFINE_BUY_ADS_COUNT);
    if ( !str.empty() )
    {
        _uMineRefineBuyAdsCount = std::atoi(MafAes256::Decrypt(str).c_str());
    }
    else
    {
        _uMineRefineBuyAdsCount = 0;
    }
    
    // refine production count
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_MINE_REFINE_PRODUCTION_COUNT);
    if ( !str.empty() )
    {
        _uMineRefineProductionCount = std::atoi(MafAes256::Decrypt(str).c_str());
    }
    else
    {
        _uMineRefineProductionCount = 0;
    }
    
    // refine production start time
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_MINE_REFINE_PRODUCTION_STARTTIME);
    if ( !str.empty() )
    {
        _uMineRefineProductionStartTime = std::atoll(MafAes256::Decrypt(str).c_str());
    }
    else
    {
        _uMineRefineProductionStartTime = 0;
    }
    
    // refine production item
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_MINE_REFINE_PRODUCTION_ITEM);
    if ( !str.empty() )
    {
        _uMineRefineProductionItem = std::atoi(MafAes256::Decrypt(str).c_str());
    }
    else
    {
        _uMineRefineProductionItem = 0;
    }
    
    // refine upgrade level
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_MINE_REFINE_UPGRADE_LEVEL);
    if ( !str.empty() )
    {
        str = MafAes256::Decrypt(str);
        
        std::istringstream stream(str);
        std::string field = "";
        while ( std::getline(stream, field, ',') )
        {
            auto offset = field.find("_");
            if(offset != std::string::npos)
            {
                std::string str1 = field.substr(0,offset);
                std::string str2 = field.substr(offset+1);
                
                if ( MafUtils::isNumeric(str1) == false || MafUtils::isNumeric(str2) == false )
                {
                    continue;
                }
                
                setRefineUpgradeLevel(atoi(str1.c_str()), atoi(str2.c_str()));
            }
        }
    }
    
}

#pragma mark - load
void MineralManager::setLoad()
{
    setLoadRefineSetting();
}

void MineralManager::setLoadRefineSetting()
{
    _listInfoRefineSetting.clear();
    _listInfoRefineOpenCost.clear();
    _listInfoRefineOpenCostMine.clear();
    _listInfoRefineBuyCost.clear();
    _listInfoRefineBuyCostMine.clear();
    _listInfoRefineProductionCost.clear();
    _listInfoRefineProductionCostMine.clear();
    _listInfoRefineUpgrade.clear();
    
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::MINE_REFINE_SETTING);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        
        auto obj = InfoSetting::create();
        obj->setKey(jsonValue["key"].GetString());
        obj->setType(jsonValue["type"].GetString());
        obj->setValue(jsonValue["value"].GetString());
        _listInfoRefineSetting.pushBack(obj);
    }
    
    // open
    if ( std::string json = getInfoRefineSetting("release_cost")->getValueToString(); json.length() != 0 )
    {
        rapidjson::Document jsonParser;
        jsonParser.Parse<0>((char*)(const char*)json.c_str()).HasParseError();
        if( jsonParser.HasParseError() == false )
        {
            auto objItem = ItemsManager::getInstance()->getInfoItems(jsonParser["cost"]["idx"].GetInt());
            if ( objItem != nullptr )
            {
                objItem->setCount(jsonParser["cost"]["count"].GetInt());
                _listInfoRefineOpenCost.pushBack(objItem);
            }
            
            for ( int i = 0; i < jsonParser["mine"].Size(); i++ )
            {
                const rapidjson::Value& jsonValue = jsonParser["mine"][i];
                
                auto objItem = ItemsManager::getInstance()->getInfoItems(jsonValue["idx"].GetInt());
                if ( objItem != nullptr )
                {
                    objItem->setCount(jsonValue["count"].GetInt());
                    _listInfoRefineOpenCostMine.pushBack(objItem);
                }
            }
        }
    }
    
    // buy
    if ( std::string json = getInfoRefineSetting("buy_cost")->getValueToString(); json.length() != 0 )
    {
        rapidjson::Document jsonParser;
        jsonParser.Parse<0>((char*)(const char*)json.c_str()).HasParseError();
        if( jsonParser.HasParseError() == false )
        {
            auto objItem = ItemsManager::getInstance()->getInfoItems(jsonParser["cost"]["idx"].GetInt());
            if ( objItem != nullptr )
            {
                objItem->setCount(jsonParser["cost"]["count"].GetInt());
                _listInfoRefineBuyCost.pushBack(objItem);
            }
            
            for ( int i = 0; i < jsonParser["mine"].Size(); i++ )
            {
                const rapidjson::Value& jsonValue = jsonParser["mine"][i];
                
                auto objItem = ItemsManager::getInstance()->getInfoItems(jsonValue["idx"].GetInt());
                if ( objItem != nullptr )
                {
                    objItem->setCount(jsonValue["count"].GetInt());
                    _listInfoRefineBuyCostMine.pushBack(objItem);
                }
            }
        }
    }
    
    // production
    if ( std::string json = getInfoRefineSetting("production_cost")->getValueToString(); json.length() != 0 )
    {
        rapidjson::Document jsonParser;
        jsonParser.Parse<0>((char*)(const char*)json.c_str()).HasParseError();
        if( jsonParser.HasParseError() == false )
        {
            auto objItem = ItemsManager::getInstance()->getInfoItems(jsonParser["cost"]["idx"].GetInt());
            if ( objItem != nullptr )
            {
                objItem->setCount(jsonParser["cost"]["count"].GetInt());
                _listInfoRefineProductionCost.pushBack(objItem);
            }
            
            for ( int i = 0; i < jsonParser["mine"].Size(); i++ )
            {
                const rapidjson::Value& jsonValue = jsonParser["mine"][i];
                
                auto objItem = ItemsManager::getInstance()->getInfoItems(jsonValue["idx"].GetInt());
                if ( objItem != nullptr )
                {
                    objItem->setCount(jsonValue["count"].GetInt());
                    _listInfoRefineProductionCostMine.pushBack(objItem);
                }
            }
        }
    }
    
    //
    int upgradeCount = getInfoRefineSetting("upgrade_type_count")->getValueToInt();
    for ( int i = 1; i <= upgradeCount; i++ )
    {
        double effect = getInfoRefineSetting(MafUtils::format("upgrade_stat_%d", i))->getValueToFloat();
        int levelMax = getInfoRefineSetting(MafUtils::format("upgrade_stat_max_%d", i))->getValueToInt();
        std::string cost = getInfoRefineSetting(MafUtils::format("upgrade_cost_%d", i))->getValueToString();
        double costIncrease = getInfoRefineSetting(MafUtils::format("upgrade_cost_increase_%d", i))->getValueToFloat();
        
        auto obj = InfoMineralRefineUpgrade::create();
        obj->setIdx(i);
        obj->setEffect(effect);
        obj->setLevelMax(levelMax);
        obj->setCost(cost);
        obj->setCostIncrease(costIncrease);
        
        _listInfoRefineUpgrade.pushBack(obj);
    }
}

#pragma mark - get, set : info
InfoSetting* MineralManager::getInfoRefineSetting(std::string key)
{
    InfoSetting* result = nullptr;
    
    for(auto obj : _listInfoRefineSetting)
    {
        if ( obj->getKey().compare(key) == 0 )
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

cocos2d::Vector<InfoMineralRefineUpgrade*> MineralManager::getInfoRefineUpgradeAll()
{
    return _listInfoRefineUpgrade;
}

InfoMineralRefineUpgrade* MineralManager::getInfoRefineUpgrade(int idx)
{
    InfoMineralRefineUpgrade* result = nullptr;
    
    for ( auto& obj : _listInfoRefineUpgrade )
    {
        if ( obj->getIdx() == idx )
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

#pragma mark - get, set : default
E_MINERAL MineralManager::getSpecialMineral()
{
    if ( _eMineralSpecial == E_MINERAL::MINERAL_NONE )
    {
        std::string mineralSpecial = UserDefault::getInstance()->getStringForKey(KEY_MINE_SPECIAL);
        if ( mineralSpecial.empty() == false )
        {
            mineralSpecial = MafAes256::Decrypt(mineralSpecial);
            _eMineralSpecial = (E_MINERAL)atoi(mineralSpecial.c_str());
        }
        else
        {
            _eMineralSpecial = E_MINERAL::MINERAL_7_SAPPHIRE;
        }
    }
    
    return _eMineralSpecial;
}

void MineralManager::setSpecialMineral(E_MINERAL eType)
{
    _eMineralSpecial = eType;
    
    std::string encrypt = MafAes256::Encrypt(MafUtils::toString(eType));
    UserDefault::getInstance()->setStringForKey(KEY_MINE_SPECIAL, encrypt);
}

double MineralManager::getPickaxEffect()
{
    //
    std::lock_guard<std::mutex> lock(_mutexMine);
    
    int nLevelPick = UserInfoManager::getInstance()->getArtifactLevel(_ePlace, E_ARTIFACT::ARTI_34_PICKAX);
    
    
    int nEffect = DataManager::GetMinePickaxEffectData(nLevelPick);
    
    double nPer = DataManager::GetRelicEffect(E_RELIC::RELIC_7, UserInfoManager::getInstance()->getRelicLevel(_ePlace, E_RELIC::RELIC_7)) / 100.0f;
    if ( nPer > 0 )
    {
        nEffect += nEffect * nPer;
    }
    
    if ( CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_7) )
    {
        nEffect += nEffect * 0.3;
    }
    
    if ( CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_11) )
    {
        nEffect += nEffect * 0.1;
    }
    
    if ( CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_38) )
    {
        nPer = 0.0;
        nPer += PetNewManager::getInstance()->getPetLevel(E_PET::CHICK_1);
        nPer += PetNewManager::getInstance()->getPetLevel(E_PET::HAMSTER_2);
        nPer += PetNewManager::getInstance()->getPetLevel(E_PET::DRAGON_3);
        nPer += PetNewManager::getInstance()->getPetLevel(E_PET::PRINCESS_4);
        nPer += PetNewManager::getInstance()->getPetLevel(E_PET::PURIRING_5);
        nPer += PetNewManager::getInstance()->getPetLevel(E_PET::HONG_6);
        nPer += PetNewManager::getInstance()->getPetLevel(E_PET::MOLE_7);
        nPer += PetNewManager::getInstance()->getPetLevel(E_PET::HANDSOME_8);
        
        nEffect += nPer;
    }
    
    if ( UserInfoConfigManager::getInstance()->getNewbieStep() >= E_NEWBIE_STEP::STEP_1 && UserInfoConfigManager::getInstance()->getNewbieStep() <= E_NEWBIE_STEP::STEP_5 )
    {
        E_NEWBIE_STEP eNewbie = UserInfoConfigManager::getInstance()->getNewbieStep();
        
        nPer = 0.0;
        if ( eNewbie == E_NEWBIE_STEP::STEP_2 )
            nPer = 0.4;
        else if ( eNewbie == E_NEWBIE_STEP::STEP_3 )
            nPer = 0.3;
        else if ( eNewbie == E_NEWBIE_STEP::STEP_4 )
            nPer = 0.2;
        else if ( eNewbie == E_NEWBIE_STEP::STEP_5 )
            nPer = 0.1;
        
        if ( nPer > 0.0 )
        {
            nEffect += nEffect * nPer;
        }
    }
    
    if ( PetNewManager::getInstance()->getPet(E_PET::MOLE_7) == true )
    {
        nPer = PetNewManager::getInstance()->getPetEffect(E_PET::MOLE_7, PetNewManager::getInstance()->getPetLevel(E_PET::MOLE_7)) / 100.0f;
        
        double evolutionPer = PetNewManager::getInstance()->getPetEvolutionEffect1(E_PET::MOLE_7, PetNewManager::getInstance()->getPetEvolution(E_PET::MOLE_7));
        if ( evolutionPer > 0 )
        {
            nPer += evolutionPer;
        }
        
        double reinforcePer = PetNewManager::getInstance()->getPetReinforceEffect(E_PET::MOLE_7, PetNewManager::getInstance()->getPetReinforce(E_PET::MOLE_7));
        if ( reinforcePer > 0 )
        {
            nPer += reinforcePer;
        }
        
        if ( nPer > 0)
        {
            nEffect += nEffect * nPer;
        }
    }
    
    nEffect *= PetNewManager::getInstance()->getPetEquipEffect(700, 1);
    
    return nEffect;
}

double MineralManager::getPickaxEffect(E_MINERAL eType)
{
    double pickaxEffect = getPickaxEffect();
    if ( eType < E_MINERAL::MINERAL_7_SAPPHIRE )
    {
        pickaxEffect *= PetNewManager::getInstance()->getPetEquipEffect(701, 1);
    }
    else
    {
        pickaxEffect *= PetNewManager::getInstance()->getPetEquipEffect(702, 1);
    }
    
    return pickaxEffect;
}

#pragma mark - get, set : time
double MineralManager::getTimeMax(E_MINERAL eType)
{
    return DataManager::GetMineSpendTime(eType) / getPickaxEffect(eType);
}

double MineralManager::getTimeReal(E_MINERAL eType)
{
    if ( eType > E_MINERAL::MINERAL_6)
    {
        eType = E_MINERAL::MINERAL_7_SAPPHIRE;
    }
    
    return _uMineTime[eType];
}

double MineralManager::getTime(E_MINERAL eType)
{
    return getTimeReal(eType) / getPickaxEffect(eType);
}

void MineralManager::setTime(E_MINERAL eType, double time)
{
    if ( eType > E_MINERAL::MINERAL_6)
    {
        eType = E_MINERAL::MINERAL_7_SAPPHIRE;
    }
    
    _uMineTime[eType] = time;
}

#pragma mark - get, set : count
int MineralManager::getCountAdd(E_MINERAL eType)
{
    return DataManager::GetMineMineralCount(eType);
}

int MineralManager::getCount(E_MINERAL eType)
{
    return _uMineMineralCount[eType].valueInt();
}

void MineralManager::setCount(E_MINERAL eType, int count)
{
    if ( count > 2100000000 )
    {
        count = 2100000000;
    }
    _uMineMineralCount[eType] = count;
}

#pragma mark - get, set : buy
int MineralManager::getBuyAdd(E_MINERAL eType)
{
    int count = 0;
    if ( eType <= E_MINERAL::MINERAL_6 )
        count = S_BUY_MINERAL_COUNT[eType] * PetNewManager::getInstance()->getPetEquipEffect(704, 1);
    else
        count = S_BUY_MINERAL_COUNT[E_MINERAL::MINERAL_7_SAPPHIRE];
    
    return count;
}

int MineralManager::getBuyPrice(E_MINERAL eType)
{
    int nBuyDefaultMax = 1;
    if ( CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_19) == true )
    {
        nBuyDefaultMax += 1;
    }
    
    int nPrice = C_BUY_MINERAL;
    if ( nBuyDefaultMax < getBuyCount(eType) + 1 )
    {
        nPrice = 250;
    }
    
    return nPrice;
}

int MineralManager::getBuyCount(E_MINERAL eType)
{
    if ( eType > E_MINERAL::MINERAL_6 )
    {
        eType = E_MINERAL::MINERAL_7_SAPPHIRE;
    }
    
    return _uMineBuyMineral[eType].valueInt();
}

int MineralManager::getBuyCountMax()
{
    int nBuyMax = 1;
    if ( CostumeManager::getInstance()->isExist(E_COSTUME::SETITEM, E_COSTUME_IDX::IDX_19) == true )
    {
        nBuyMax += 1;
    }
    
    if ( PassManager::getInstance()->isBuyPass(E_PASS_TYPE::PASS_DUNGEON) == true )
    {
        nBuyMax += 2;
    }
    
    return nBuyMax;
}

void MineralManager::setBuyCount(E_MINERAL eType, int count)
{
    if( eType > E_MINERAL::MINERAL_6 )
    {
        eType = E_MINERAL::MINERAL_7_SAPPHIRE;
    }
    
    _uMineBuyMineral[eType] = count;
}

E_CASH MineralManager::getBuyPurchaseType(E_MINERAL eType)
{
    E_CASH eCash = E_CASH::CASH_MINE_1;
    switch (eType) {
        case E_MINERAL::MINERAL_1:                      eCash = E_CASH::CASH_MINE_1;            break;
        case E_MINERAL::MINERAL_2:                      eCash = E_CASH::CASH_MINE_2;            break;
        case E_MINERAL::MINERAL_3:                      eCash = E_CASH::CASH_MINE_3;            break;
        case E_MINERAL::MINERAL_4:                      eCash = E_CASH::CASH_MINE_4;            break;
        case E_MINERAL::MINERAL_5:                      eCash = E_CASH::CASH_MINE_5;            break;
        case E_MINERAL::MINERAL_6:                      eCash = E_CASH::CASH_MINE_6;            break;
        case E_MINERAL::MINERAL_7_SAPPHIRE:             eCash = E_CASH::CASH_MINE_7;            break;
        case E_MINERAL::MINERAL_8_OPAL:                 eCash = E_CASH::CASH_MINE_8;            break;
        case E_MINERAL::MINERAL_9_AQUA:                 eCash = E_CASH::CASH_MINE_9;            break;
        case E_MINERAL::MINERAL_10_EMERALD:             eCash = E_CASH::CASH_MINE_10;           break;
        case E_MINERAL::MINERAL_11_AMETHYST:            eCash = E_CASH::CASH_MINE_11;           break;

        default:
            break;
    }
    
    return eCash;
}

#pragma mark - get, set : refine
int MineralManager::getRefineOpen()
{
    return _uMineRefineOpen.valueInt();
}

void MineralManager::setRefineOpen(int open)
{
    _uMineRefineOpen = open;
}

int64_t MineralManager::getRefineBuyResetTime()
{
    return _uMineRefineBuyResetTime;
}
void MineralManager::setRefineBuyResetTime(int64_t time)
{
    _uMineRefineBuyResetTime = time;
}

int MineralManager::getRefineBuyCount()
{
    return _uMineRefineBuyCount.valueInt();
}

void MineralManager::setRefineBuyCount(int count)
{
    _uMineRefineBuyCount = count;
}

int64_t MineralManager::getRefineBuyAdsResetTime()
{
    return _uMineRefineBuyAdsResetTime;
}

void MineralManager::setRefineBuyAdsResetTime(int64_t time)
{
    _uMineRefineBuyAdsResetTime = time;
}

int MineralManager::getRefineBuyAdsCount()
{
    return _uMineRefineBuyAdsCount.valueInt();
}

int MineralManager::getRefineBuyAdsCountMax()
{
    return 5;
}

int MineralManager::getRefineUpgradeLevel(int idx)
{
    int level = 0;
    
    auto iter = _listMineRefineUpgradeLevel.find(idx);
    if ( iter != _listMineRefineUpgradeLevel.end() )
    {
        level = _listMineRefineUpgradeLevel[idx];
    }
    
    return level;
}

void MineralManager::setRefineUpgradeLevel(int idx, int level)
{
    _listMineRefineUpgradeLevel[idx] = level;
}

double MineralManager::getRefineUpgradeEffect(int idx, int level/* = 0*/)
{
    auto objRefineUpgrade = getInfoRefineUpgrade(idx);
    
    if ( level == 0 )
    {
        level = getRefineUpgradeLevel(idx);
    }
    int levelMax = objRefineUpgrade->getLevelMax();
    
    if ( level > levelMax )
    {
        level = levelMax;
    }
    
    return objRefineUpgrade->getEffect() * level;
}

int MineralManager::getRefineProductionAdd()
{
    return MineralManager::getInstance()->getInfoRefineSetting("production_output_mine")->getValueToInt();
}

int MineralManager::getRefineProductionTimeNeed()
{
    double effect = getRefineUpgradeEffect(1);
    effect = MIN(MAX(effect, 0), 100);
    
    int time = MineralManager::getInstance()->getInfoRefineSetting("production_time")->getValueToInt();
    time = time - (time * effect / 100.0);
     
    return time;
}

int64_t MineralManager::getRefineProductionTimeStart()
{
    return _uMineRefineProductionStartTime;
}

void MineralManager::setRefineProductionTimeStart(int64_t time)
{
    _uMineRefineProductionStartTime = time;
}

int MineralManager::getRefineProductionCount()
{
    return _uMineRefineProductionCount.valueInt();
}

void MineralManager::setRefineProductionCount(int count)
{
    _uMineRefineProductionCount = count;
}

int MineralManager::getRefineProductionCountMax()
{
    return 1 + getRefineUpgradeEffect(3);
}

int MineralManager::getRefineProductionItem()
{
    return _uMineRefineProductionItem.valueInt();
}

void MineralManager::setRefineProductionItem(int count)
{
    _uMineRefineProductionItem = count;
}

int MineralManager::getRefineProductionItemMax()
{
    return MineralManager::getInstance()->getInfoRefineSetting("production_output_mine_max")->getValueToInt();
}

#pragma mark - get, set : refine cost
InfoItems* MineralManager::getRefineOpenCost()
{
    InfoItems* cost = nullptr;
    
    if ( _listInfoRefineOpenCost.size() > 0 )
    {
        auto item = _listInfoRefineOpenCost.at(0);
        cost = item->clone();
        cost->setCount(item->getCount());
    }
    
    return cost;
}

cocos2d::Vector<InfoItems*> MineralManager::getRefineOpenCostMine()
{
    cocos2d::Vector<InfoItems*> listCost;
    
    for ( int i = 0; i < _listInfoRefineOpenCostMine.size(); i++ )
    {
        auto item = _listInfoRefineOpenCostMine.at(i);
        auto cost = item->clone();
        cost->setCount(item->getCount());
        
        listCost.pushBack(cost);
    }
    
    return listCost;
}

InfoItems* MineralManager::getRefineBuyCost()
{
    double increase = getInfoRefineSetting("buy_cost_increase")->getValueToFloat();
    
    InfoItems* cost = nullptr;
    
    if ( _listInfoRefineBuyCost.size() > 0 )
    {
        auto item = _listInfoRefineBuyCost.at(0);
        cost = item->clone();
        
        int64_t count =  atoll(item->getCount().c_str());
        int buyCount = getRefineBuyCount();
        if ( buyCount > 0 )
        {
            count = count * std::pow(increase, buyCount);
        }
        cost->setCount(MafUtils::toString(count));
    }
    
    return cost;
}

cocos2d::Vector<InfoItems*> MineralManager::getRefineBuyCostMine()
{
    double increase = getInfoRefineSetting("buy_cost_increase")->getValueToFloat();
    
    cocos2d::Vector<InfoItems*> listCost;
    
    for ( int i = 0; i < _listInfoRefineBuyCostMine.size(); i++ )
    {
        auto item = _listInfoRefineBuyCostMine.at(i);
        auto cost = item->clone();
        
        int64_t count =  atoll(item->getCount().c_str());
        int buyCount = getRefineBuyCount();
        if ( buyCount > 0 )
        {
            count = count * std::pow(increase, buyCount);
        }
        cost->setCount(MafUtils::toString(count));
        
        listCost.pushBack(cost);
    }
    
    return listCost;
}

InfoItems* MineralManager::getRefineUpgradeCost(int idx)
{
    InfoItems* cost = nullptr;
    
    auto objRefineUpgrade = getInfoRefineUpgrade(idx);
    auto item = objRefineUpgrade->getCost();
    if ( item != nullptr )
    {
        cost = item->clone();
        
        int64_t count =  atoll(item->getCount().c_str());
        int level = getRefineUpgradeLevel(idx);
        if ( level > 0 )
        {
            count = count * std::pow(objRefineUpgrade->getCostIncrease(), level);
        }
        cost->setCount(MafUtils::toString(count));
    }
    
    return cost;
}

cocos2d::Vector<InfoItems*> MineralManager::getRefineUpgradeCostMine(int idx)
{
    cocos2d::Vector<InfoItems*> listCost;
    
    auto objRefineUpgrade = getInfoRefineUpgrade(idx);
    for ( int i = 0; i < objRefineUpgrade->getCostMine().size(); i++ )
    {
        auto item = objRefineUpgrade->getCostMine().at(i);
        auto cost = item->clone();
        
        int64_t count =  atoll(item->getCount().c_str());
        int level = getRefineUpgradeLevel(idx);
        if ( level > 0 )
        {
            count = count * std::pow(objRefineUpgrade->getCostIncrease(), level);
        }
        cost->setCount(MafUtils::toString(count));
        
        listCost.pushBack(cost);
    }
    
    return listCost;
}

InfoItems* MineralManager::getRefineProductionCost()
{
    InfoItems* cost = nullptr;
    
    if ( _listInfoRefineProductionCost.size() > 0 )
    {
        auto item = _listInfoRefineProductionCost.at(0);
        cost = item->clone();
        cost->setCount(item->getCount());
    }
    
    return cost;
}

cocos2d::Vector<InfoItems*> MineralManager::getRefineProductionCostMine()
{
    cocos2d::Vector<InfoItems*> listCost;
    
    for ( int i = 0; i < _listInfoRefineProductionCostMine.size(); i++ )
    {
        auto item = _listInfoRefineProductionCostMine.at(i);
        auto cost = item->clone();
        cost->setCount(item->getCount());
        
        listCost.pushBack(cost);
    }
    
    return listCost;
}

#pragma mark - event
int MineralManager::onBuyInit()
{
    int result = 0;
    /*
     0 : 성공
     */
    
    for ( int i = 0; i < sizeof(_uMineBuyMineral) / sizeof(TInteger); i++ )
    {
        _uMineBuyMineral[i] = 0;
    }
    
    //
    saveData();
    
    return result;
}

int MineralManager::onBuy(E_MINERAL eType)
{
    int result = 0;
    /*
     0 : 성공
     1 : 가격 부족
     2 : 구매 제한
     */
    
    int count = getCount(eType);
    
    int buyAdd = getBuyAdd(eType);
    int buyCount = getBuyCount(eType);
    int buyCountMax = getBuyCountMax();
    int buyPrice = getBuyPrice(eType);
    int currency = ItemsMoneyManager::getInstance()->getGem();
    
    if ( currency < buyPrice )
    {
        result = 1;
        return result;
    }
    else if ( buyCount >= buyCountMax )
    {
        result = 2;
        return result;
    }
    
    
    // apply
    setBuyCount(eType, buyCount + 1);
    setCount(eType, count + buyAdd);
    
    ItemsMoneyManager::getInstance()->setGem(-buyPrice);
    
    //
    saveData();
    
    // mission
    ServerMissionManager::getInstance()->setServerMission(E_DAILY_QUEST::MINERAL_BUY, 1);
    
    // analytics
    if ( eType <= E_MINERAL::MINERAL_6 )
        MafAnalyticsManager::LogEvent(kAnalEventMineBuyNomal, kRepeatTrue);
    else
        MafAnalyticsManager::LogEvent(kAnalEventMineBuySP, kRepeatTrue);
    
    return result;
}

int MineralManager::onRefineOpen()
{
    int result = 0;
    /*
     0 : 성공
     1 : 재화 부족
     2 : 구매 제한
     */
    
    auto cost = getRefineOpenCost();
    auto costMine = getRefineOpenCostMine();
    
    if ( WeaponManager::getInstance()->getEquipHigh() < E_WEAPON::WEAPON_180_L )
    {
        return 2;
    }
    
    // cost
    auto itemNeed = cost->getCount();
    auto itemNow = ItemsManager::getInstance()->getItems((E_ITEMS)cost->getIdx());
    if ( MafUtils::compareStr(itemNow, itemNeed) == false )
    {
        return 1;
    }
    
    // cost mine
    for ( int i = 0; i < costMine.size(); i++ )
    {
        auto mine = costMine.at(i);
        
        auto itemNeed = mine->getCount();
        auto itemNow = ItemsManager::getInstance()->getItems((E_ITEMS)mine->getIdx());
        if ( MafUtils::compareStr(itemNow, itemNeed) == false )
        {
            return 1;
        }
    }
    
    //
    setRefineOpen(1);

    //
    ItemsManager::getInstance()->subItem(cost);
    for ( int i = 0; i < costMine.size(); i++ )
    {
        auto mine = costMine.at(i);
        ItemsManager::getInstance()->subItem(mine);
    }
    
    //
    saveData();
    
    //
    PopupLoading::show();
    SaveManager::getInstance()->requestSave([=](const bool bResult){
        PopupLoading::hide();
    });
    
    return result;
}

int MineralManager::onRefineBuyInit()
{
    int result = 0;
    /*
     0 : 성공
     1 : 조건 안됨
     */
    
    time_t timeNow = UtilsDate::getInstance()->getTime();
    if ( timeNow < getRefineBuyResetTime() )
    {
        return 1;
    }
    
    //
    setRefineBuyCount(0);
    
    struct tm* tm = localtime(&timeNow);
    tm->tm_mday += (7 - tm->tm_wday + 1);
    tm->tm_hour = 0;
    tm->tm_min = 0;
    tm->tm_sec = 0;
    setRefineBuyResetTime(mktime(tm));
    
    //
    saveData();
    
    return result;
}

int MineralManager::onRefineBuy()
{
    int result = 0;
    /*
     0 : 성공
     1 : 재화 부족
     2 : 구매 제한
     */
    
    auto cost = getRefineBuyCost();
    auto costMine = getRefineBuyCostMine();
    
    // cost
    auto itemNeed = cost->getCount();
    auto itemNow = ItemsManager::getInstance()->getItems((E_ITEMS)cost->getIdx());
    if ( MafUtils::compareStr(itemNow, itemNeed) == false )
    {
        return 1;
    }
    
    // cost mine
    for ( int i = 0; i < costMine.size(); i++ )
    {
        auto mine = costMine.at(i);
        
        auto itemNeed = mine->getCount();
        auto itemNow = ItemsManager::getInstance()->getItems((E_ITEMS)mine->getIdx());
        if ( MafUtils::compareStr(itemNow, itemNeed) == false )
        {
            return 1;
        }
    }
    
    //
    setRefineBuyCount(getRefineBuyCount() + 1);
    
    //
    int itemAdd = getRefineProductionAdd();
    ItemsManager::getInstance()->addItem(E_ITEMS::MYTHRIL, 0, itemAdd);
    
    //
    ItemsManager::getInstance()->subItem(cost);
    for ( int i = 0; i < costMine.size(); i++ )
    {
        auto mine = costMine.at(i);
        ItemsManager::getInstance()->subItem(mine);
    }
    
    //
    saveData();
    
    //
    PopupLoading::show();
    SaveManager::getInstance()->requestSave([=](const bool bResult){
        PopupLoading::hide();
    });
    
    return result;
}

int MineralManager::onRefineBuyAdsInit()
{
    int result = 0;
    /*
     0 : 성공
     1 : 조건 안됨
     */
    time_t timeNow = UtilsDate::getInstance()->getTime();
    if ( timeNow < getRefineBuyAdsResetTime() )
    {
        return 1;
    }
    
    //
    _uMineRefineBuyAdsCount = 0;
    
    struct tm* tm = localtime(&timeNow);
    tm->tm_mday += 1;
    tm->tm_hour = 0;
    tm->tm_min = 0;
    tm->tm_sec = 0;
    setRefineBuyAdsResetTime(mktime(tm));
    
    //
    saveData();
    
    return result;
}

int MineralManager::onRefineBuyAds()
{
    int result = 0;
    /*
     0 : 성공
     1 : 재화 부족
     2 : 구매 제한
     */
    
    //
    int count = MineralManager::getInstance()->getRefineBuyAdsCount();
    int countMax = MineralManager::getInstance()->getRefineBuyAdsCountMax();
    if ( count >= countMax )
    {
        return 2;
    }
    
    _uMineRefineBuyAdsCount += 1;
    
    //
    int itemAdd = getRefineProductionAdd();
    ItemsManager::getInstance()->addItem(E_ITEMS::MYTHRIL, 0, itemAdd);
    
    //
    saveData();
    
    //
    PopupLoading::show();
    SaveManager::getInstance()->requestSave([=](const bool bResult){
        PopupLoading::hide();
    });
    
    return result;
}

int MineralManager::onRefineUpgrade(int idx)
{
    int result = 0;
    /*
     0 : 성공
     1 : 재화 부족
     2 : 구매 제한
     */
    auto objRefineUpgrade = getInfoRefineUpgrade(idx);
    
    auto cost = getRefineUpgradeCost(idx);
    auto costMine = getRefineUpgradeCostMine(idx);
    
    int level = getRefineUpgradeLevel(idx);
    int levelMax = objRefineUpgrade->getLevelMax();
    
    if ( level >= levelMax )
    {
        return 2;
    }
    
    // cost
    auto itemNeed = cost->getCount();
    auto itemNow = ItemsManager::getInstance()->getItems((E_ITEMS)cost->getIdx());
    if ( MafUtils::compareStr(itemNow, itemNeed) == false )
    {
        return 1;
    }
    
    // cost mine
    for ( int i = 0; i < costMine.size(); i++ )
    {
        auto mine = costMine.at(i);
        
        auto itemNeed = mine->getCount();
        auto itemNow = ItemsManager::getInstance()->getItems((E_ITEMS)mine->getIdx());
        if ( MafUtils::compareStr(itemNow, itemNeed) == false )
        {
            return 1;
        }
    }
    
    //
    setRefineUpgradeLevel(idx, level + 1);

    //
    ItemsManager::getInstance()->subItem(cost);
    for ( int i = 0; i < costMine.size(); i++ )
    {
        auto mine = costMine.at(i);
        ItemsManager::getInstance()->subItem(mine);
    }
    
    //
    saveData();
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::MINE_REFINE_UPGRADE);
    
    //
    PopupLoading::show();
    SaveManager::getInstance()->requestSave([=](const bool bResult){
        PopupLoading::hide();
    });
    
    return result;
}

int MineralManager::onRefineProduction()
{
    int result = 0;
    /*
     0 : 성공
     1 : 재화 부족
     2 : 구매 제한
     */
    
    auto cost = getRefineProductionCost();
    auto costMine = getRefineProductionCostMine();
    
    int count = getRefineProductionCount();
    int countMax = getRefineProductionCountMax();
    if ( count >= countMax )
    {
        return 2;
    }
    
    // cost
    auto itemNeed = cost->getCount();
    auto itemNow = ItemsManager::getInstance()->getItems((E_ITEMS)cost->getIdx());
    if ( MafUtils::compareStr(itemNow, itemNeed) == false )
    {
        return 1;
    }
    
    // cost mine
    for ( int i = 0; i < costMine.size(); i++ )
    {
        auto mine = costMine.at(i);
        
        auto itemNeed = mine->getCount();
        auto itemNow = ItemsManager::getInstance()->getItems((E_ITEMS)mine->getIdx());
        if ( MafUtils::compareStr(itemNow, itemNeed) == false )
        {
            return 1;
        }
    }
    
    //
    setRefineProductionCount(count + 1);
    if ( count <= 0 )
    {
        int64_t timeNow = UtilsDate::getInstance()->getTime();
        setRefineProductionTimeStart(timeNow);
    }
    
    //
    ItemsManager::getInstance()->subItem(cost);
    for ( int i = 0; i < costMine.size(); i++ )
    {
        auto mine = costMine.at(i);
        ItemsManager::getInstance()->subItem(mine);
    }
    
    //
    saveData();
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::MINE_REFINE_PRODUCTION);
    
    //
    PopupLoading::show();
    SaveManager::getInstance()->requestSave([=](const bool bResult){
        PopupLoading::hide();
    });
    
    return result;
}

int MineralManager::onRefineProductionUpdate()
{
    int result = 0;
    /*
     0 : 성공
     1 : 조건 안됨
     */
    
    int count = getRefineProductionCount();
    if ( count <= 0 )
    {
        return 1;
    }
    
    int64_t timeNow = UtilsDate::getInstance()->getTime();
    int64_t timeStart = MineralManager::getInstance()->getRefineProductionTimeStart();
    int64_t timeNeed = MineralManager::getInstance()->getRefineProductionTimeNeed();
    int64_t timeDiff = (timeStart + timeNeed)  - timeNow;
    if ( timeDiff > 0 )
    {
        return 1;
    }
    
    //
    int itemAdd = getRefineProductionAdd();
    if ( random(1.0, 100.0) <= getRefineUpgradeEffect(2) )
    {
        itemAdd = itemAdd * 1.4;
    }
    
    setRefineProductionCount(count - 1);
    setRefineProductionItem(getRefineProductionItem() + itemAdd);
    
    setRefineProductionTimeStart(0);
    if ( getRefineProductionCount() > 0 )
    {
        setRefineProductionTimeStart(timeStart + timeNeed);
    }
    
    //
    saveData();
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::MINE_REFINE_PRODUCTION);
    
    return result;
}

int MineralManager::onRefineProductionReward()
{
    int result = 0;
    /*
     0 : 성공
     1 : 조건 안됨
     */
    
    int itemNow = getRefineProductionItem();
    if ( itemNow <= 0 )
    {
        return 1;
    }
    
    //
    setRefineProductionItem(0);
    ItemsManager::getInstance()->addItem(E_ITEMS::MYTHRIL, 0, itemNow);
    
    //
    saveData();
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::MINE_REFINE_PRODUCTION);
    
    //
    PopupLoading::show();
    SaveManager::getInstance()->requestSave([=](const bool bResult){
        PopupLoading::hide();
    });
    
    return result;
}

#pragma mark - network
