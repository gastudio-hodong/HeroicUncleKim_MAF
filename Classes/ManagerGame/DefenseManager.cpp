//
//  DefenseManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/06.
//

#include "DefenseManager.h"

#include "Common/ConfigKey.h"

#include "GameObject/InfoSetting.h"
#include "GameObject/Defense/InfoDefenseFloor.h"
#include "GameObject/Defense/InfoDefenseMap.h"
#include "GameObject/Defense/InfoDefenseMonster.h"
#include "GameObject/Defense/InfoDefenseWeapon.h"
#include "GameObject/Defense/InfoDefenseCharacter.h"
#include "GameObject/Defense/InfoDefenseTrench.h"
#include "GameObject/Defense/InfoDefenseBullet.h"
#include "GameObject/Raid/InfoRaidMileageLv.h"
#include "GameObject/Grid/Grid.h"

#include "GameUIPopup/Other/Defense/ManaAMP/PopupManaAMPDefault.h"
#include "GameUIPopup/Common/PopupHelp.h"
#include "GameUIPopup/Common/PopupDefault.h"

#include "GameUIPopup/Common/PopupLoading.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/NewRaidManager.h"
#include "ManagerSystem/RefreshManager.h"
#include "ManagerGame/DialogManager.h"
#include "ManagerGame/DefenseManaAMPManager.h"

USING_NS_CC;

static bool isStrongWeapon(InfoDefenseWeapon* a, InfoDefenseWeapon* b)
{
    bool result = false;
    
    //if(a->getDFAttack() > b->getDFAttack())
        //result = true;
    //else if(std::fabs(a->getDFAttack() - b->getDFAttack()) < 0.01)
        result = a->getIndex() > b->getIndex();
    
    return result;
}
static bool isStrongCharacter(InfoDefenseCharacter* a, InfoDefenseCharacter* b)
{
    bool result = false;
    
    if(a->getDFAttack() > b->getDFAttack())
        result = true;
    else if(std::fabs(a->getDFAttack() - b->getDFAttack()) < 0.01)
        result = a->getIndex() > b->getIndex();
    
    return result;
}

DefenseManager::DefenseManager(void):
_isOpenContentsNow(false),
_isBackground(false),
_isFirstGift(false),
_currentFloor(1),
_highFloor(1),
_drawTicket_1(0),
_drawTicket_11(0),
_drawTicket_33(0),
_nAdsSummonCount(0),
_nAdsSummonCooltime(0),
_failRewardCount(0),
_drawMileage(0),
_callbackInfo(nullptr),
_callbackFloorUpdate(nullptr),
_defenseCoin(0),
_callbackSummon(nullptr)
{
    _listInfoMapData.clear();
    _listInfoWeaponData.clear();
    _listInfoBulletData.clear();
    _listInfoCharacterData.clear();
    _listInfoSetting.clear();
    _listInfoTrench.clear();
    
    _onUpdateWeapon.Clear();
    _onUpdateClear.Clear();
    _onUpdateFail.Clear();
    _onUpdateFloor.Clear();
    _onUpdateCharacter.Clear();
    _onServerInfo.Clear();
    _onServerEquip.Clear();
    _onServerRankup.Clear();
    _onServerLevelup.Clear();
    _onServerSmelting.Clear();
    _onServerSummon.Clear();
    _onServerUpdateFloor.Clear();
}

DefenseManager::~DefenseManager(void)
{
    _listInfoMapData.clear();
    _listInfoWeaponData.clear();
    _listInfoBulletData.clear();
    _listInfoCharacterData.clear();
    _listInfoSetting.clear();
    _listInfoTrench.clear();
    
    _onUpdateWeapon.Clear();
    _onUpdateClear.Clear();
    _onUpdateFail.Clear();
    _onUpdateFloor.Clear();
    _onUpdateCharacter.Clear();
    _onServerInfo.Clear();
    _onServerEquip.Clear();
    _onServerRankup.Clear();
    _onServerLevelup.Clear();
    _onServerSmelting.Clear();
    _onServerSummon.Clear();
    _onServerUpdateFloor.Clear();
}

bool DefenseManager::init()
{
    loadData();
    
    //사로 설정
    _listInfoTrench.clear();
    
    for(int i = 0; i < getMaxTrenchCount(); ++i)
    {
        auto obj = InfoDefenseTrench::create();
        obj->setNumber(i+1);
        _listInfoTrench.pushBack(obj);
    }
    
    return true;
}

#pragma mark - Load Save
void DefenseManager::loadLocalData()
{
    loadWeaponLvData();
    loadCharacterLvData();
    
    auto str = UserDefault::getInstance()->getStringForKey(KEY_DF_HIGH_FLOOR);
    if(!str.empty())
    {
        _highFloor = std::atoi(MafAes256::Decrypt(str).c_str());
    }
    str = UserDefault::getInstance()->getStringForKey(KEY_DF_CURRENT_FLOOR);
    if(!str.empty())
    {
        _currentFloor = std::atoi(MafAes256::Decrypt(str).c_str());
    }
    str = UserDefault::getInstance()->getStringForKey(KEY_DF_COIN);
    if(!str.empty())
    {
        _defenseCoin = std::atoll(MafAes256::Decrypt(str).c_str());
    }
    
    saveLocalData();
}
void DefenseManager::saveLocalData()
{
    saveWeaponLvData();
    saveCharacterLvData();
    
    UserDefault::getInstance()->setStringForKey(KEY_DF_HIGH_FLOOR, MafAes256::Encrypt(MafUtils::toString(_highFloor.value())));
    UserDefault::getInstance()->setStringForKey(KEY_DF_CURRENT_FLOOR, MafAes256::Encrypt(MafUtils::toString(_currentFloor.value())));
    UserDefault::getInstance()->setStringForKey(KEY_DF_COIN, MafAes256::Encrypt(std::to_string(_defenseCoin)));
}

void DefenseManager::loadWeaponLvData()
{
     std::string str = "";
     str.clear();
     str = UserDefault::getInstance()->getStringForKey(KEY_DF_WEAPON_LV);
     if(!str.empty()){
         str = MafAes256::Decrypt(str);
         
         for(auto info : _listInfoWeaponData)
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
    
    saveWeaponLvData();
}
void DefenseManager::saveWeaponLvData()
{
    std::stringstream str;
    
    str.str("");
     for(auto info : _listInfoWeaponData)
     {
         str << info->getCurrentLv() <<",";
     }
     UserDefault::getInstance()->setStringForKey(KEY_DF_WEAPON_LV, MafAes256::Encrypt(str.str()));
}
void DefenseManager::loadCharacterLvData()
{
    std::string str = "";
    str.clear();
    str = UserDefault::getInstance()->getStringForKey(KEY_DF_CHARACTER_LV);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        
        for(auto info : _listInfoCharacterData)
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
   
    saveCharacterLvData();
}
void DefenseManager::saveCharacterLvData()
{
    std::stringstream str;
    
    str.str("");
     for(auto info : _listInfoCharacterData)
     {
         str << info->getCurrentLv() <<",";
     }
     UserDefault::getInstance()->setStringForKey(KEY_DF_CHARACTER_LV, MafAes256::Encrypt(str.str()));
}

#pragma mark -Load Table Data
void DefenseManager::loadData()
{
    setLoadFloorData();
    setLoadMapData();
    setLoadMonsterData();
    setLoadWeaponData();
    setLoadBulletData();
    setLoadCharacterData();
    setLoadSetting();
    setLoadMileageLv();
}
void DefenseManager::setLoadFloorData()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::DEFENSE_FLOOR);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoFloorData.clear();
    std::string strCurType = "";
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];

        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        const int idx = jsonValue["idx"].GetInt();
        const int map_number = jsonValue["map_number"].GetInt();
        const int start_floor = jsonValue["start_floor"].GetInt();
        const int end_floor = jsonValue["end_floor"].GetInt();
        const double monster_hp = jsonValue["monster_hp"].GetDouble();
        const int monster_value = jsonValue["monster_value"].GetInt();
        const int monster_time = jsonValue["monster_time"].GetInt();
        const int normal_per = jsonValue["normal_per"].GetInt();
        const int runner_idx = jsonValue["runner_idx"].GetInt();
        const int runner_per = jsonValue["runner_per"].GetInt();
        const int tanker_idx = jsonValue["tanker_idx"].GetInt();
        const int tanker_per = jsonValue["tanker_per"].GetInt();
        const int boss_idx = jsonValue["boss_idx"].GetInt();
        const std::string reward_repeat = jsonValue["reward_repeat"].GetString();
        std::string reward_fail = "[{\"idx\": 125, \"count\": 0}]";
        if(jsonValue.HasMember("reward_fail"))
            reward_fail = jsonValue["reward_fail"].GetString();
        
        
        InfoDefenseFloor* obj = InfoDefenseFloor::create();
        obj->setIndex(idx);
        obj->setMapNumber(map_number);
        obj->setStartFloor(start_floor);
        obj->setEndFloor(end_floor);
        obj->setMonsterHp(monster_hp);
        obj->setMonsterValue(monster_value);
        obj->set10SecMonsterSpawnCount(monster_time);
        obj->setNormalPer(normal_per);
        obj->setRunnerIdx(runner_idx);
        obj->setRunnerPer(runner_per);
        obj->setTankerIdx(tanker_idx);
        obj->setTankerPer(tanker_per);
        obj->setBossIdx(boss_idx);
        obj->setRewardRepeat(reward_repeat);
        obj->setRewardFail(reward_fail);
        
        _listInfoFloorData.pushBack(obj);
    }
}


void DefenseManager::setLoadMapData()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::DEFENSE_MAP);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoMapData.clear();
    std::string strCurType = "";
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];

        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        const int idx = jsonValue["idx"].GetInt();
        const std::string imagePath = jsonValue["map_image_path"].GetString();
        const std::string monsterPath = jsonValue["monster_move_path"].GetString();
        
        std::vector<IntPoint> path;
        
        rapidjson::Document pathParser;
        pathParser.ParseInsitu<0>((char*)monsterPath.c_str());
        if(!pathParser.HasParseError())
        {
            const rapidjson::Value& jsonPathValue = pathParser[0];
            
            if(jsonPathValue.HasMember("path"))
            {
                for(int j = 0; j < jsonPathValue["path"].GetArray().Size(); ++j)
                {
                    const rapidjson::Value& PathIndex = jsonPathValue["path"][j];
                    
                    std::string strPos = PathIndex.GetString();
                    IntPoint pos;
                    
                    for(int i=0; i < 2; i++){
                        auto offset = strPos.find(",");
                        if(offset != std::string::npos){
                            auto result = strPos.substr(0,offset);
                            auto ret = std::atoi(result.c_str());
                            pos.X = ret;
                            strPos = strPos.substr(offset+1);
                        }
                        else{
                            auto ret = std::atoi(strPos.c_str());
                            pos.Y = ret;
                            break;
                        }
                    }
                    
                    path.push_back(pos);
                }
            }
        }
        
        InfoDefenseMap* obj = InfoDefenseMap::create();
        obj->setIndex(idx);
        obj->setImageFilePath(imagePath);
        obj->setPath(path);
        _listInfoMapData.pushBack(obj);
    }
}

void DefenseManager::setLoadMonsterData()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::DEFENSE_MONSTER);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoMonsterData.clear();
    std::string strCurType = "";
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];

        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        const int idx = jsonValue["idx"].GetInt();
        const E_DF_MonsterType type = (E_DF_MonsterType)jsonValue["type"].GetInt();
        const double hp_coefficient = jsonValue["hp_coefficient"].GetDouble();
        const double defensive_power = jsonValue["defensive_power"].GetDouble();
        const int speed = jsonValue["speed"].GetInt();
        
        InfoDefenseMonster* obj = InfoDefenseMonster::create();
        obj->setIndex(idx);
        obj->setType(type);
        obj->setHpCoefficient(hp_coefficient);
        obj->setDefensive(defensive_power);
        obj->setSpeed(speed);
        _listInfoMonsterData.pushBack(obj);
    }
}


void DefenseManager::setLoadWeaponData()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::DEFENSE_WEAPON);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoWeaponData.clear();
    std::string strCurType = "";
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];

        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        const int idx = jsonValue["idx"].GetInt();
        const int grade = jsonValue["grade"].GetInt();
        const int maxLv = jsonValue["max_lv"].GetInt();
        const double lvDfAttackIncrease = jsonValue["lv_df_attack_increase"].GetDouble();
        const int lvUpCost = jsonValue["lv_up_cost"].GetInt();
        const int maxSmelting = jsonValue["max_smelting"].GetInt();
        const double smeltingBsAttack = jsonValue["smelting_bs_attack"].GetDouble();
        const int smeltingCost = jsonValue["smelting_cost"].GetInt();
        const int weaponType = jsonValue["weapon_type"].GetInt();
        const int bulletType = jsonValue["bullet_type"].GetInt();
        const double baseDfAttack = jsonValue["base_df_attack"].GetDouble();
        const double baseDfAttackSpeed = jsonValue["base_df_attack_speed"].GetDouble();
        const double baseBsAttack = jsonValue["base_bs_attack"].GetDouble();
        
        InfoDefenseWeapon* obj = InfoDefenseWeapon::create();
        obj->setIndex(idx);
        obj->setGrade(grade);
        obj->setMaxLv(maxLv);
        obj->setDFAttackIncrease(lvDfAttackIncrease);
        obj->setLvUpCost(lvUpCost);
        obj->setMaxSmelting(maxSmelting);
        obj->setSmeltingBSAttack(smeltingBsAttack);
        obj->setSmeltingCost(smeltingCost);
        obj->setWeaponType((E_DF_WeaponType) weaponType);
        obj->setBulletType(bulletType);
        obj->setBaseDFAttack(baseDfAttack);
        obj->setBaseDFAttackSpeed(baseDfAttackSpeed);
        obj->setBaseBSAttak(baseBsAttack);
        
        _listInfoWeaponData.pushBack(obj);
    }
}

void DefenseManager::setLoadCharacterData()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::DEFENSE_CHARACTER);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoCharacterData.clear();
    std::string strCurType = "";
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];

        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        const int idx = jsonValue["idx"].GetInt();
        const int grade = jsonValue["grade"].GetInt();
        const int maxLv = jsonValue["max_lv"].GetInt();
        const double lvDfAttackIncrease = jsonValue["lv_df_attack_increase"].GetDouble();
        const int lvUpCost = jsonValue["lv_up_cost"].GetInt();
        const int maxSmelting = jsonValue["max_smelting"].GetInt();
        const double smeltingBsAttack = jsonValue["smelting_bs_attack"].GetDouble();
        const int smeltingCost = jsonValue["smelting_cost"].GetInt();
        const double baseDfAttack = jsonValue["base_df_attack"].GetDouble();
        const double baseDfAttackSpeed = jsonValue["base_df_attack_speed"].GetDouble();
        const double baseBsAttack = jsonValue["base_bs_attack"].GetDouble();
        
        InfoDefenseCharacter* obj = InfoDefenseCharacter::create();
        obj->setIndex(idx);
        obj->setGrade(grade);
        obj->setMaxLv(maxLv);
        obj->setDFAttackIncrease(lvDfAttackIncrease);
        obj->setLvUpCost(lvUpCost);
        obj->setMaxSmelting(maxSmelting);
        obj->setSmeltingBSAttack(smeltingBsAttack);
        obj->setSmeltingCost(smeltingCost);
        obj->setBaseDFAttack(baseDfAttack);
        obj->setBaseDFAttackSpeed(baseDfAttackSpeed);
        obj->setBaseBSAttak(baseBsAttack);
        
        _listInfoCharacterData.pushBack(obj);
    }
}

void DefenseManager::setLoadBulletData()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::DEFENSE_BULLET);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoBulletData.clear();
    std::string strCurType = "";
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];

        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        const int idx = jsonValue["idx"].GetInt();
        const E_DF_AttackType attackType = (E_DF_AttackType)jsonValue["attack_type"].GetInt();
        const E_DF_TargetType targetType = (E_DF_TargetType)jsonValue["target_type"].GetInt();
        const E_DF_PierceType pierceType = (E_DF_PierceType)jsonValue["pierce_type"].GetInt();
        const E_DF_ExplosionType explosionType = (E_DF_ExplosionType)jsonValue["explosion_type"].GetInt();
        const E_DF_ProjectileType projectileType = (E_DF_ProjectileType)jsonValue["projectile_type"].GetInt();
        const E_DF_TrajectoryType trajectoryType = (E_DF_TrajectoryType)jsonValue["trajectory_type"].GetInt();
        const E_DF_HitEffectType hitEffectType = (E_DF_HitEffectType)jsonValue["hit_effect_type"].GetInt();
        const E_DF_CartridgeType cartridgeType = (E_DF_CartridgeType)jsonValue["cartridge_effect_type"].GetInt();
        const int bulletValue = jsonValue["bullet_value"].GetInt();
        const int imageNum = jsonValue["image_num"].GetInt();
        const int speed = jsonValue["speed"].GetInt();
        const double collisionTimer = jsonValue["collision_timer"].GetDouble();
        const double collisionInterval = jsonValue["collision_interval"].GetDouble();
        const E_DF_MuzzleEffectType muzzleEffectType = (E_DF_MuzzleEffectType)jsonValue["muzzle_effect_type"].GetInt();
        
        InfoDefenseBullet* obj = InfoDefenseBullet::create();
        obj->setIndex(idx);
        obj->setAttackType(attackType);
        obj->setTargetType(targetType);
        obj->setPierceType(pierceType);
        obj->setExplosionType(explosionType);
        obj->setProjectileType(projectileType);
        obj->setHitEffectType(hitEffectType);
        obj->setTrajectoryType(trajectoryType);
        obj->setBulletValue(bulletValue);
        obj->setImageNum(imageNum);
        obj->setSpeed(speed);
        obj->setCollisionTimer(collisionTimer);
        obj->setCollisionInterval(collisionInterval);
        obj->setMuzzleEffectType(muzzleEffectType);
        obj->setCartridgeType(cartridgeType);
        _listInfoBulletData.pushBack(obj);
    }
}
void DefenseManager::setLoadSetting()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::DEFENSE_SETTING);
    
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

void DefenseManager::setLoadMileageLv()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::DEFENSE_GATCHA_LEVEL);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoMileageLv.clear();
    for ( int i = 0; i < jsonParser.Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[i];
        
        if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
        {
            continue;
        }
        
        int nLv = jsonValue["lv"].GetInt();
        int nValue = jsonValue["value"].GetInt();
        
        InfoRaidMileageLv* obj = InfoRaidMileageLv::create();
        obj->setLv(nLv);
        obj->setValue(nValue);
        
        _listInfoMileageLv.pushBack(obj);
    }
}

#pragma mark -Getter
Vector<InfoDefenseWeapon*> DefenseManager::getWeaponList() const
{
    return _listInfoWeaponData;
}
Vector<InfoDefenseCharacter*> DefenseManager::getCharacterList() const
{
    return _listInfoCharacterData;
}
Vector<InfoRaidMileageLv*> DefenseManager::getInfoMileageLvList() const
{
    return _listInfoMileageLv;
}
Vector<InfoDefenseWeapon*> DefenseManager::getTopStrongWeaponList() const
{
    Vector<InfoDefenseWeapon*> result;
    
    for(auto obj : _listInfoWeaponData)
    {
        if(obj->isHave())
            result.pushBack(obj);
    }
    
    std::sort(result.begin(), result.end(), isStrongWeapon);
    
    return result;
}
Vector<InfoDefenseCharacter*> DefenseManager::getTopStrongCharacterList() const
{
    Vector<InfoDefenseCharacter*> result;
    
    for(auto obj : _listInfoCharacterData)
    {
        if(obj->isHave())
            result.pushBack(obj);
    }
    std::sort(result.begin(), result.end(), isStrongCharacter);
    
    return result;
}
InfoDefenseFloor* DefenseManager::getFloorData(int floor) const
{
    InfoDefenseFloor* result = nullptr;
    
    for(auto data : _listInfoFloorData)
    {
        if(floor >= data->getStartFloor() &&
           floor <= data->getEndFloor())
        {
            result = data;
            break;
        }
    }
    
    return result;
}
InfoDefenseFloor* DefenseManager::getCurrentFloorData()
{
    return getFloorData(_currentFloor);
}
InfoDefenseMap* DefenseManager::getMapData(int idx) const
{
    InfoDefenseMap* result = nullptr;
    
    for(auto data : _listInfoMapData)
    {
        if(data->getIndex() == idx)
        {
            result = data;
            break;
        }
    }
    
    return result;
}

InfoDefenseMonster* DefenseManager::getMonsterData(int idx) const
{
    InfoDefenseMonster* result = nullptr;
    
    for(auto data : _listInfoMonsterData)
    {
        if(data->getIndex() == idx)
        {
            result = data;
            break;
        }
    }
    
    return result;
}

InfoDefenseWeapon* DefenseManager::getWeaponData(int idx) const
{
    InfoDefenseWeapon* result = nullptr;
    
    for(auto data : _listInfoWeaponData)
    {
        if(data->getIndex() == idx)
        {
            result = data;
            break;
        }
    }
    
    return result;
}

InfoDefenseBullet* DefenseManager::getBulletData(int idx) const
{
    InfoDefenseBullet* result = nullptr;
    
    for(auto data : _listInfoBulletData)
    {
        if(data->getIndex() == idx)
        {
            result = data;
            break;
        }
    }
    
    return result;
}

InfoDefenseCharacter* DefenseManager::getCharacterData(int idx) const
{
    InfoDefenseCharacter* result = nullptr;
    
    for(auto data : _listInfoCharacterData)
    {
        if(data->getIndex() == idx)
        {
            result = data;
            break;
        }
    }
    
    return result;
}

InfoDefenseWeapon* DefenseManager::getEquipWeaponDataByTrench(int num) const
{
    InfoDefenseWeapon* result = nullptr;
    
    if(auto trenchData = getInfoTrench(num))
    {
        result = trenchData->getInfoDefenseWeapon();
    }
    
    return result;
}

InfoDefenseTrench* DefenseManager::getInfoTrench(int num) const
{
    InfoDefenseTrench* result = nullptr;
    
    //TODO
    for(auto data : _listInfoTrench)
    {
        if(data->getNumber() == num)
        {
            result = data;
            break;
        }
    }
    
    return result;
}

InfoSetting* DefenseManager::getInfoSetting(std::string key) const
{
    InfoSetting* result = nullptr;
    
    //TODO
    for(auto data : _listInfoSetting)
    {
        if(data->getKey() == key)
        {
            result = data;
            break;
        }
    }
    
    return result;
}

InfoRaidMileageLv* DefenseManager::getInfoMileageLv(int value) const
{
    InfoRaidMileageLv* result = nullptr;
    
    for(auto obj : _listInfoMileageLv)
    {
        if(obj->getLv() == value)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}
bool DefenseManager::isOpenContentsNow()
{
    return _isOpenContentsNow;
}
void DefenseManager::setOpenContentsNow(bool isOpen)
{
    _isOpenContentsNow = isOpen;
}
bool DefenseManager::isBackground()
{
    return _isBackground;
}
void DefenseManager::setIsBackground(bool isBackground)
{
    _isBackground = isBackground;
    
    _onUpdateBackgound.Invoke();
}
int DefenseManager::getCurrentMileageLv()
{
    int result = 0;
    
    for(auto obj : _listInfoMileageLv)
    {
        if(obj->getValue() > _drawMileage)
        {
            break;
        }
        result++;
    }
    
    return result;
}

int64_t DefenseManager::getCurrentFloor()
{
    return _currentFloor;
}
int64_t DefenseManager::getHighFloor()
{
    return _highFloor;
}
int DefenseManager::getWeaponDataCount() const
{
    return _listInfoWeaponData.size();
}
int DefenseManager::getCharacterDataCount() const
{
    return _listInfoCharacterData.size();
}
int64_t DefenseManager::getFailStage() const
{
    TInteger tmp = _highFloor;
    
    int64_t result = tmp / 100;
    result *= 100;
    result++;
    
    return result;
}
int DefenseManager::getLastGrade(E_DF_Trench_SettingType type) const
{
    int result = 0;
    
    if(type == E_DF_Trench_SettingType::WEAPON)
    {
        for(auto weapon : _listInfoWeaponData)
        {
            if(weapon->getGrade() > result)
                result = weapon->getGrade();
        }
    }
    if(type == E_DF_Trench_SettingType::CHARACTER)
    {
        for(auto character : _listInfoCharacterData)
        {
            if(character->getGrade() > result)
                result = character->getGrade();
        }
    }
    return result;
}
int DefenseManager::getFailRewardCount() const
{
    return _failRewardCount;
}
int DefenseManager::getMaxTrenchCount() const
{
    int result = 6;
    if(auto setting = getInfoSetting("max_track_count"))
    {
        result = setting->getValueToInt();
    }
    
    return result;
}

void DefenseManager::setDrawTicket_1(int count)
{
    _drawTicket_1 = count;
}
void DefenseManager::setDrawTicket_11(int count)
{
    _drawTicket_11 = count;
}
void DefenseManager::setDrawTicket_33(int count)
{
    _drawTicket_33 = count;
}

int DefenseManager::getDrawTicket_1()
{
    return _drawTicket_1;
}

int DefenseManager::getDrawTicket_11()
{
    return _drawTicket_11;
}

int DefenseManager::getDrawTicket_33()
{
    return _drawTicket_33;
}
int DefenseManager::getMileage()
{
    return _drawMileage;
}
int DefenseManager::getAdsCount()
{
    return _nAdsSummonCount;
}
int DefenseManager::getAdsCooltime()
{
    return _nAdsSummonCooltime;
}
bool DefenseManager::isFirstGift()
{
    return _isFirstGift;
}

bool DefenseManager::isEquipWeapon(InfoDefenseWeapon* data, int& trenchNumber)
{
    bool result = false;
    if(data == nullptr)
        return result;
    
    for(auto trench : _listInfoTrench)
    {
        if(data->getIndex() == trench->getWeaponIdx())
        {
            trenchNumber = trench->getNumber();
            result = true;
            break;
        }
    }
    return result;
}

bool DefenseManager::isEquipCharacter(InfoDefenseCharacter* data, int& trenchNumber)
{
    bool result = false;
    if(data == nullptr)
        return result;
    
    for(auto trench : _listInfoTrench)
    {
        if(data->getIndex() == trench->getCharacterIdx())
        {
            trenchNumber = trench->getNumber();
            result = true;
            break;
        }
    }
    return result;
}

int DefenseManager::getObjectMaxGrade(E_DF_Trench_SettingType type) const
{
    int result = 0;
    
    switch(type)
    {
        case E_DF_Trench_SettingType::CHARACTER:
        {
            for(auto obj : _listInfoCharacterData)
            {
                if(obj->getGrade() > result)
                    result = obj->getGrade();
            }
        }break;
        case E_DF_Trench_SettingType::WEAPON:
        {
            for(auto obj : _listInfoWeaponData)
            {
                if(obj->getGrade() > result)
                    result = obj->getGrade();
            }
        }break;
    }
    
    return result;
}

bool DefenseManager::isReddotCondition(E_DF_Trench_SettingType type) const
{
    switch (type) {
        case E_DF_Trench_SettingType::CHARACTER:
        {
            for(auto data : _listInfoCharacterData)
            {
                if(data->isReddotCondition())
                    return true;
            }
        }break;
        case E_DF_Trench_SettingType::WEAPON:
        {
            for(auto data : _listInfoWeaponData)
            {
                if(data->isReddotCondition())
                    return true;
            }
        }break;
        default:
            break;
    }
    
    return false;
}
bool DefenseManager::isReddotCondition() const
{
    bool result = false;
    
    if(isReddotCondition(E_DF_Trench_SettingType::CHARACTER) || isReddotCondition(E_DF_Trench_SettingType::WEAPON))
        result = true;
    
    return result;
}
#pragma mark -Utils
void DefenseManager::clearEvents()
{
    _onUpdateWeapon.Clear();
    _onUpdateFail.Clear();
    _onUpdateClear.Clear();
    _onUpdateFloor.Clear();
    _onUpdateCharacter.Clear();
    _onServerInfo.Clear();
    _onServerEquip.Clear();
    _onServerRankup.Clear();
    _onServerLevelup.Clear();
    _onServerSmelting.Clear();
    _onServerSummon.Clear();
    _onServerUpdateFloor.Clear();
    _onUpdateBackgound.Clear();
}
void DefenseManager::updateTrenchCharacter(int num, int characterIdx)
{
    if(auto objTrench = getInfoTrench(num))
    {
        objTrench->setCharacterIdx(characterIdx);
    }
    _onUpdateCharacter.Invoke();
}
void DefenseManager::updateTrenchWeapon(int num, int weaponIdx)
{
    if(auto objTrench = getInfoTrench(num))
    {
        objTrench->setWeaponIdx(weaponIdx);
    }
    _onUpdateWeapon.Invoke();
}
void DefenseManager::updateObjectStatus(E_DF_Trench_SettingType type, bool isHave, int idx, int lv, int smelting, int count, int position)
{
    switch(type)
    {
        case E_DF_Trench_SettingType::CHARACTER : updateCharacterStatus(isHave, idx, lv, smelting, count, position); break;
        case E_DF_Trench_SettingType::WEAPON : updateWeaponStatus(isHave, idx, lv, smelting, count, position); break;
        default: break;
    }
}
void DefenseManager::updateWeaponStatus(bool isHave, int idx, int lv, int smelting, int count, int position)
{
    auto data = getWeaponData(idx);
    if(data == nullptr)
        return;
    
    data->setIsHave(isHave);
    data->setCurrentSmelting(smelting);
    data->setCurrentCount(count);
    
    if(position > 0)
        updateTrenchWeapon(position, data->getIndex());
    
    //서버는 n단계에 한번 씩만 저장함
    if(data->getCurrentLv() < lv)
        data->setCurrentLv(lv);
}
void DefenseManager::updateCharacterStatus(bool isHave, int idx, int lv, int smelting, int count, int position)
{
    auto data = getCharacterData(idx);
    if(data == nullptr)
        return;
    
    data->setIsHave(isHave);
    data->setCurrentSmelting(smelting);
    data->setCurrentCount(count);
    
    if(position > 0)
        updateTrenchCharacter(position, data->getIndex());
    
    //서버는 n단계에 한번 씩만 저장함
    if(data->getCurrentLv() < lv)
        data->setCurrentLv(lv);
}

void DefenseManager::updateObjectStatusServerData(E_DF_Trench_SettingType type, const rapidjson::Document& jsonParser)
{
    std::string key = "";
    switch(type)
    {
        case E_DF_Trench_SettingType::CHARACTER:
        {
            key = "_character_list";
            resetCharacterData();
        }break;
        case E_DF_Trench_SettingType::WEAPON :
        {
            key = "_weapon_list";
            resetWeaponData();
        }break;
    }
    
    for ( int i = 0; i < jsonParser[key.c_str()].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser[key.c_str()][i];
        
        int idx = jsonValue["idx"].GetInt();
        
        if(auto data = getWeaponData(idx))
        {
            int smelting = jsonValue["smelting"].GetInt();
            int level = jsonValue["level"].GetInt();
            int count = jsonValue["count"].GetInt();
            int position = jsonValue["position"].GetInt();
            
            checkObjectHack(type, idx, level, smelting);
            updateObjectStatus(type, true, idx, level, smelting, count, position);
        }
    }
}

void DefenseManager::checkObjectHack(E_DF_Trench_SettingType type, int idx, int level, int smeling)
{
    auto characterData = getCharacterData(idx);
    auto weaponData = getWeaponData(idx);
    bool isHack = false;
    int lvGap = 10;
    if(auto setting = getInfoSetting("defense_lvcheck_gap"))
        lvGap = setting->getValueToInt();
    
    int maxLv = (level + 1) * lvGap - 1;
    level *= lvGap;
    
    switch(type)
    {
        case E_DF_Trench_SettingType::CHARACTER:
        {
            if(characterData->getCurrentSmelting() > smeling)
            {
                //characterData->setCurrentSmelting(smeling);
                isHack = true;
            }
            if(characterData->getCurrentLv() <= level)
                characterData->setCurrentLv(level);
            
            if(characterData->getCurrentLv() > maxLv)
            {
                characterData->setCurrentLv(level);
                isHack = true;
            }
        }break;
        case E_DF_Trench_SettingType::WEAPON:
        {
            if(weaponData->getCurrentSmelting() > smeling)
            {
                //weaponData->setCurrentSmelting(smeling);
                isHack = true;
            }
            if(weaponData->getCurrentLv() <= level)
                weaponData->setCurrentLv(level);
            
            if(weaponData->getCurrentLv() > maxLv)
            {
                weaponData->setCurrentLv(level);
                isHack = true;
            }
        }break;
    }
    
    if(isHack)
    {
        std::string strMassage = "디펜스 컨텐츠 레벨, 제련 변조 감지\n";
        
        AccountManager::getInstance()->requestReport(false, E_REPORT_TYPE::HACK, strMassage, AccountManager::getInstance()->getUserIdx(), AccountManager::getInstance()->getUserPlatform(), nullptr);
    }
}
void DefenseManager::resetWeaponData()
{
    for(auto obj : _listInfoWeaponData)
    {
        updateObjectStatus(E_DF_Trench_SettingType::WEAPON, false, obj->getIndex(), obj->getCurrentLv(), 0, 0, 0);
    }
    for(auto obj : _listInfoTrench)
    {
        obj->setWeaponIdx(0);
    }
}

void DefenseManager::resetCharacterData()
{
    for(auto obj : _listInfoCharacterData)
    {
        updateObjectStatus(E_DF_Trench_SettingType::CHARACTER, false, obj->getIndex(), obj->getCurrentLv(), 0, 0, 0);
    }
    for(auto obj : _listInfoTrench)
    {
        obj->setCharacterIdx(0);
    }
}
#pragma mark -Calculation
std::string DefenseManager::getCharacterAttackCalculation(std::string value, bool isOrginal)
{
    for(auto info : _listInfoCharacterData)
    {
        if(!info->isHave())
            continue;
        
        auto tt = MafUtils::bigMulNum(value, MafUtils::doubleToString(info->getBSAttack()));
        if(tt.length() > 0)
            tt.pop_back();
        
        if(tt.length() > 0)
            tt.pop_back();
        
        if(tt.length() > 0)
            value = MafUtils::bigAddNum(value, tt);
    }
    
    return value;
}
std::string DefenseManager::getWeaponAttackCalculation(std::string value, bool isOrginal)
{
    for(auto info : _listInfoWeaponData)
    {
        if(!info->isHave())
            continue;
        
        auto tt = MafUtils::bigMulNum(value, MafUtils::doubleToString(info->getBSAttack()));
        if(tt.length() > 0)
            tt.pop_back();
        
        if(tt.length() > 0)
            tt.pop_back();
        
        if(tt.length() > 0)
            value = MafUtils::bigAddNum(value, tt);
    }
    
    return value;
}
std::string DefenseManager::getHighFloorAttackCalculation(std::string value, bool isOrginal)
{
    
    auto tt = MafUtils::bigMulNum(value, MafUtils::doubleToString(_highFloor));
    
    if(tt.length() > 0)
        value = MafUtils::bigAddNum(value, tt);
    
    return value;
}
std::string DefenseManager::getAttackCalculation(std::string value, bool isOrginal)
{
    bool isOpen = false;
    if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_switch"))
        isOpen = setting->getValueToBool();
    
    if(!_isFirstGift)
        return value;
    
    if(isOpen)
    {
        value = getCharacterAttackCalculation(value, isOrginal);
        value = getWeaponAttackCalculation(value, isOrginal);
        value = getHighFloorAttackCalculation(value, isOrginal);
    }

    return value;
}
#pragma mark - InGame
void DefenseManager::testclear(bool isReset)
{
    FailStage();
    if(isReset)
    {
        _currentFloor = 1;
        _highFloor = 1;
    }
    else
    {
        _currentFloor += 200;
        _currentFloor -= _currentFloor % 100 + 1;
    }
    clearFloor();
}
void DefenseManager::clearFloor()
{
    if(_currentFloor > _highFloor)
    {
        _highFloor = _currentFloor;
        
        int serverUpdateInterval = 100;
        if(auto setting = getInfoSetting("defense_floor_update_interval"))
            serverUpdateInterval = setting->getValueToInt();
        
        if(_highFloor >= serverUpdateInterval && _highFloor % serverUpdateInterval == 0)
        {
            requestUpdateInfo(false, [=](bool, int){
                auto ampOpenFloorList = DefenseManaAMPManager::getInstance()->getAMPOpenFloorList();
                for(auto floor : ampOpenFloorList)
                {
                    if(_highFloor == floor)
                    {
                        DefenseManaAMPManager::getInstance()->requestInfo(false);
                        break;
                    }
                }
            });
        }
        else if(_highFloor == 11)
        {
            requestUpdateInfo(false, [=](bool, int){
                
                auto ampOpenFloorList = DefenseManaAMPManager::getInstance()->getAMPOpenFloorList();
                for(auto floor : ampOpenFloorList)
                {
                    if(_highFloor == floor)
                    {
                        DefenseManaAMPManager::getInstance()->requestInfo(false);
                        break;
                    }
                }
                
                
                auto ampPopup = PopupManaAMPDefault::create(1);
                ampPopup->show();
                
                auto helpPopup = PopupHelp::create(1, "t_ui_defense_manaamp_info_title_%d", "t_ui_defense_manaamp_info_msg_%d");
                helpPopup->show();
                
                DialogManager::getInstance()->showDialog("DEFENSE_MANAAMP_OPENNING");
            });
        }
    }
    
    if(auto floorData = getCurrentFloorData())
    {
        ItemsManager::getInstance()->addItems(floorData->getRewardRepeat(), false);
    }
    
    _currentFloor++;
    
    saveLocalData();
    _onUpdateClear.Invoke();
    _onUpdateFloor.Invoke();
}
void DefenseManager::FailStage()
{
    if(auto floorData = getCurrentFloorData())
    {
        auto list = ItemsManager::getInstance()->getConvertArray(floorData->getRewardFail());
        if(list.size() > 0)
            _failRewardCount = std::atoi(list.at(0)->getCount().c_str());
        ItemsManager::getInstance()->addItems(list, false);
    }
    _currentFloor = getFailStage();
    
    saveLocalData();
    _onUpdateFail.Invoke();
    _onUpdateFloor.Invoke();
}

void DefenseManager::resetDefenseCoin(bool bSave)
{
    _defenseCoin = 0;
    
    // save
    if ( bSave )
    {
        std::string enData = MafAes256::Encrypt(std::to_string(_defenseCoin));
        UserDefault::getInstance()->setStringForKey(KEY_DF_COIN, enData.c_str());
    }

    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}

void DefenseManager::setDefenseCoin(int64_t plus, bool bSave, bool bRefresh)
{
    _defenseCoin += plus;
    if(_defenseCoin >= 99999999999999999)
        _defenseCoin = 99999999999999999;
   if(_defenseCoin <= 0)
       _defenseCoin = 0;
    
    if ( bSave )
    {
        std::string enData = MafAes256::Encrypt(std::to_string(_defenseCoin));
        UserDefault::getInstance()->setStringForKey(KEY_DF_COIN, enData.c_str());
    }
    
    //
    if ( bRefresh == true )
    {
        RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    }
}

int64_t DefenseManager::getDefenseCoin()
{
    return _defenseCoin;
}

#pragma mark - network : info
void DefenseManager::requestInfo(bool isShowLoading, const std::function<void(bool, int)>& callback, bool isIntro)
{
    if ( isShowLoading == true )
    {
        PopupLoading::show();
    }
    
    _callbackInfo = callback;
    //
    std::string url = "/defense/info/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(isShowLoading, isIntro, response, dd);
    };
    req->send(cb);
}

void DefenseManager::responseInfo(bool isShowLoading, bool isIntro, cocos2d::network::HttpResponse* response,std::string &data)
{
    if ( isShowLoading == true )
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
        _onServerInfo.Invoke(false, -1);
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false, -1);
            _callbackInfo = nullptr;
        }
        return;
    }
    
    int result = jsonParser["_result"].GetInt();
    if ( result != 1 )
    {
        //
        _onServerInfo.Invoke(false, result);
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false, result);
            _callbackInfo = nullptr;
        }
        if(result == 9 && !isIntro)
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_52"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            
            if(auto setting = getInfoSetting("defense_switch"))
            {
                setting->setValue("false");
            }
        }
        return;
    }
    if(auto setting = getInfoSetting("defense_switch"))
    {
        setting->setValue("true");
    }
    
    _isFirstGift = jsonParser["_is_recv_first_input_reward"].GetInt();
    _drawMileage = jsonParser["_mileage"].GetInt();
    _nAdsSummonCount = jsonParser["_defense_draw_ads_count"].GetInt();
    _nAdsSummonCooltime = jsonParser["_defense_draw_ads_cooltime"].GetInt();
    
    
    updateObjectStatusServerData(E_DF_Trench_SettingType::WEAPON, jsonParser);
    updateObjectStatusServerData(E_DF_Trench_SettingType::CHARACTER, jsonParser);
    
    std::string strItems = jsonParser["_items"].GetString();
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    //
    _onServerInfo.Invoke(true, result);
    if ( _callbackInfo != nullptr )
    {
        _callbackInfo(true, result);
        _callbackInfo = nullptr;
    }
    requestUpdateInfo(isShowLoading);
}

void DefenseManager::requestEquip(E_DF_Trench_SettingType type, int idx, int pos, bool isShowLoading)
{
    if ( isShowLoading == true )
    {
        PopupLoading::show();
    }
    //
    std::string url = "/defense/equip";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", (int)type);
    req->setData("_index", idx);
    req->setData("_position", pos);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseEquip(isShowLoading, response,dd);
    };
    req->send(cb);
}

void DefenseManager::responseEquip(bool isShowLoading, cocos2d::network::HttpResponse* response,std::string &data)
{
    if ( isShowLoading == true )
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
        _onServerEquip.Invoke(false, -1);
        return;
    }
    
    int result = jsonParser["_result"].GetInt();
    if ( result != 1 )
    {
        //
        _onServerEquip.Invoke(false, result);
        if(result == 9)
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_52"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            
            if(auto setting = getInfoSetting("defense_switch"))
            {
                setting->setValue("false");
            }
        }
        return;
    }
    if(auto setting = getInfoSetting("defense_switch"))
    {
        setting->setValue("true");
    }
    
    updateObjectStatusServerData(E_DF_Trench_SettingType::WEAPON, jsonParser);
    updateObjectStatusServerData(E_DF_Trench_SettingType::CHARACTER, jsonParser);
    
    _onUpdateCharacter.Invoke();
    _onUpdateWeapon.Invoke();
    //
    _onServerEquip.Invoke(true, result);
}

void DefenseManager::requestRankup(E_DF_Trench_SettingType type, int idx, int count, bool isShowLoading)
{
    if ( isShowLoading == true )
    {
        PopupLoading::show();
    }
    //
    std::string url = "/defense/advancement";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", (int)type);
    req->setData("_index", idx);
    req->setData("_count", count);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseRankup(isShowLoading, response,dd);
    };
    req->send(cb);
}

void DefenseManager::responseRankup(bool isShowLoading, cocos2d::network::HttpResponse* response,std::string &data)
{
    if ( isShowLoading == true )
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
        _onServerRankup.Invoke(false, -1);
        return;
    }
    
    int result = jsonParser["_result"].GetInt();
    if ( result != 1 )
    {
        //
        _onServerRankup.Invoke(false, result);
        if(result == 9)
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_52"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            
            if(auto setting = getInfoSetting("defense_switch"))
            {
                setting->setValue("false");
            }
        }
        return;
    }
    if(auto setting = getInfoSetting("defense_switch"))
    {
        setting->setValue("true");
    }
    
    updateObjectStatusServerData(E_DF_Trench_SettingType::WEAPON, jsonParser);
    updateObjectStatusServerData(E_DF_Trench_SettingType::CHARACTER, jsonParser);
    
    
    _onUpdateCharacter.Invoke();
    _onUpdateWeapon.Invoke();
    //
    _onServerRankup.Invoke(true, result);
}

void DefenseManager::requestLevelup(E_DF_Trench_SettingType type, int idx, bool isShowLoading)
{
    int gap = 10;
    int level = 0;
    if(auto setting = getInfoSetting("defense_lvcheck_gap"))
        gap = setting->getValueToInt();
    
    switch(type)
    {
        case E_DF_Trench_SettingType::CHARACTER:
        {
            if(auto data = getCharacterData(idx))
            {
                level = data->getCurrentLv() + 1;
                setDefenseCoin(data->getLvUpCost() * -1, true, true);
                if(level % gap == 0)
                {
                    requestServerLevelup(type, idx, data->getCurrentLv(), isShowLoading);
                    return;
                }
                    
                data->setCurrentLv(level);
            }
        }break;
        case E_DF_Trench_SettingType::WEAPON:
        {
            if(auto data = getWeaponData(idx))
            {
                level = data->getCurrentLv() + 1;
                setDefenseCoin(data->getLvUpCost() * -1, true, true);
                if(level % gap == 0)
                {
                    requestServerLevelup(type, idx, data->getCurrentLv(), isShowLoading);
                    return;
                }
                
                data->setCurrentLv(level);
            }
        }break;
    }
    
    saveLocalData();
    if ( isShowLoading == true )
    {
        PopupLoading::show();
    }
    SaveManager::getInstance()->requestSave([=](bool){
        
        if ( isShowLoading == true )
        {
            PopupLoading::hide();
            _onServerLevelup.Invoke(true, 1);
        }
    });
}

void DefenseManager::requestServerLevelup(E_DF_Trench_SettingType type, int idx, int level, bool isShowLoading)
{
    if ( isShowLoading == true )
    {
        PopupLoading::show();
    }
    //
    std::string url = "/defense/reinforcement";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", (int)type);
    req->setData("_index", idx);
    req->setData("_level", level);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseServerLevelup(isShowLoading, response,dd);
    };
    req->send(cb);
}

void DefenseManager::responseServerLevelup(bool isShowLoading, cocos2d::network::HttpResponse* response,std::string &data)
{
    if ( isShowLoading == true )
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
        _onServerLevelup.Invoke(false, -1);
        return;
    }
    
    int result = jsonParser["_result"].GetInt();
    if ( result != 1 )
    {
        //
        _onServerLevelup.Invoke(false, result);
        if(result == 9)
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_52"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            
            if(auto setting = getInfoSetting("defense_switch"))
            {
                setting->setValue("false");
            }
        }
        return;
    }
    if(auto setting = getInfoSetting("defense_switch"))
    {
        setting->setValue("true");
    }
    
    updateObjectStatusServerData(E_DF_Trench_SettingType::WEAPON, jsonParser);
    updateObjectStatusServerData(E_DF_Trench_SettingType::CHARACTER, jsonParser);
    
    std::string strItems = jsonParser["_items"].GetString();
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    saveLocalData();
    
    PopupLoading::show();
    SaveManager::getInstance()->requestSave([=](bool){
        PopupLoading::hide();
    });
    
    _onUpdateCharacter.Invoke();
    _onUpdateWeapon.Invoke();
    //
    _onServerLevelup.Invoke(true, result);
}

void DefenseManager::requestSmelting(E_DF_Trench_SettingType type, int idx, bool isShowLoading)
{
    if ( isShowLoading == true )
    {
        PopupLoading::show();
    }
    //
    std::string url = "/defense/smelting";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_type", (int)type);
    req->setData("_index", idx);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseSmelting(isShowLoading, response,dd);
    };
    req->send(cb);
}

void DefenseManager::responseSmelting(bool isShowLoading, cocos2d::network::HttpResponse* response,std::string &data)
{
    if ( isShowLoading == true )
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
        _onServerSmelting.Invoke(false, -1);
        return;
    }
    
    int result = jsonParser["_result"].GetInt();
    if ( result != 1 )
    {
        //
        _onServerSmelting.Invoke(false, result);
        if(result == 9)
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_52"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            
            if(auto setting = getInfoSetting("defense_switch"))
            {
                setting->setValue("false");
            }
        }
        return;
    }
    if(auto setting = getInfoSetting("defense_switch"))
    {
        setting->setValue("true");
    }
    
    updateObjectStatusServerData(E_DF_Trench_SettingType::WEAPON, jsonParser);
    updateObjectStatusServerData(E_DF_Trench_SettingType::CHARACTER, jsonParser);
    
    std::string strItems = jsonParser["_items"].GetString();
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    _onUpdateCharacter.Invoke();
    _onUpdateWeapon.Invoke();
    //
    _onServerSmelting.Invoke(true, result);
}

void DefenseManager::requestUpdateInfo(bool isShowLoading, const std::function<void(bool, int)>& callback)
{
    if ( isShowLoading == true )
    {
        PopupLoading::show();
    }
    _callbackFloorUpdate = callback;
    //
    std::string url = "/defense/result";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_current_floor", _currentFloor.valueInt());
    req->setData("_high_floor", _highFloor.valueInt());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseUpdateInfo(isShowLoading, response,dd);
    };
    req->send(cb);
}

void DefenseManager::responseUpdateInfo(bool isShowLoading, cocos2d::network::HttpResponse* response,std::string &data)
{
    if ( isShowLoading == true )
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
        if ( _callbackFloorUpdate != nullptr )
        {
            _callbackFloorUpdate(false, -1);
            _callbackFloorUpdate = nullptr;
        }
        _onServerUpdateFloor.Invoke(false, -1);
        return;
    }
    
    int result = jsonParser["_result"].GetInt();
    
    if(result == 1)
    {
        if(auto setting = getInfoSetting("defense_switch"))
        {
            setting->setValue("true");
        }
    }
    else
    {
        if(result == 9)
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_52"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            
            if(auto setting = getInfoSetting("defense_switch"))
            {
                setting->setValue("false");
            }
        }
        if ( _callbackFloorUpdate != nullptr )
        {
            _callbackFloorUpdate(false, result);
            _callbackFloorUpdate = nullptr;
        }
        _onServerUpdateFloor.Invoke(false, result);
        return;
    }
    
    if ( _callbackFloorUpdate != nullptr )
    {
        _callbackFloorUpdate(true, result);
        _callbackFloorUpdate = nullptr;
    }
    _onServerUpdateFloor.Invoke(true, result);
}

void DefenseManager::requestSummon(int count, bool isAds, bool isShowLoading, const std::function<void(bool, int, cocos2d::Vector<InfoItems*>)>& callback)
{
    if ( isShowLoading == true )
    {
        PopupLoading::show();
    }
    _callbackSummon = callback;
    //
    std::string url = "/defense/market/summon";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_isADS", (int)isAds);
    req->setData("_count", count);
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseSummon(isShowLoading, response,dd);
    };
    req->send(cb);
}

void DefenseManager::responseSummon(bool isShowLoading, cocos2d::network::HttpResponse* response,std::string &data)
{
    if ( isShowLoading == true )
    {
        PopupLoading::hide();
    }
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    cocos2d::Vector<InfoItems*> listReward;
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        if(_callbackSummon != nullptr)
            _callbackSummon(false, -1, listReward);
        _onServerSummon.Invoke(false, -1);
        return;
    }
    
    int result = jsonParser["_result"].GetInt();
    if ( result != 1 )
    {
        //
        if(_callbackSummon != nullptr)
            _callbackSummon(false, result, listReward);
        _onServerSummon.Invoke(false, result);
        if(result == 9)
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_52"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            
            if(auto setting = getInfoSetting("defense_gacha_switch"))
            {
                setting->setValue("false");
            }
        }
        return;
    }
    if(auto setting = getInfoSetting("defense_gacha_switch"))
    {
        setting->setValue("true");
    }
    
    
    if(jsonParser.HasMember("_mileage"))
        _drawMileage = jsonParser["_mileage"].GetInt();
    if(jsonParser.HasMember("_total_mileage"))
        _drawMileage = jsonParser["_total_mileage"].GetInt();
    _nAdsSummonCount = jsonParser["_defense_draw_ads_count"].GetInt();
    _nAdsSummonCooltime = jsonParser["_defense_draw_ads_cooltime"].GetInt();
    
    updateObjectStatusServerData(E_DF_Trench_SettingType::WEAPON, jsonParser);
    updateObjectStatusServerData(E_DF_Trench_SettingType::CHARACTER, jsonParser);
    
    std::string strReward = jsonParser["_reward"].GetString();
    listReward = ItemsManager::getInstance()->getConvertArray(strReward);
    ItemsManager::getInstance()->addItems(strReward, false);
    
    std::string strItems = jsonParser["_items"].GetString();
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    
    _onUpdateCharacter.Invoke();
    _onUpdateWeapon.Invoke();
    //
    if(_callbackSummon != nullptr)
        _callbackSummon(true, result, listReward);
    _onServerSummon.Invoke(true, result);
}
void DefenseManager::requestFirstGift(bool isShowLoading)
{
    if ( isShowLoading == true )
    {
        PopupLoading::show();
    }
    //
    std::string url = "/defense/receiveFirstInputReward";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseFirstGift(isShowLoading, response,dd);
    };
    req->send(cb);
}

void DefenseManager::responseFirstGift(bool isShowLoading, cocos2d::network::HttpResponse* response,std::string &data)
{
    if ( isShowLoading == true )
    {
        PopupLoading::hide();
    }
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    cocos2d::Vector<InfoItems*> listReward;
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        return;
    }
    
    int result = jsonParser["_result"].GetInt();
    if ( result != 1 )
    {
        return;
    }
    
    _isFirstGift = true;
    
}
