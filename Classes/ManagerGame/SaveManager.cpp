//
//  SaveManager.cpp
//  HexagonSnake
//
//  Created by SanHeo on 2014. 8. 6..
//
//

#include "SaveManager.h"

#include "GameScene/HelloWorldScene.h"
#include "GameScene/DevilDomScene.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/UserInfoStackManager.h"
#include "ManagerGame/UserInfoConfigManager.h"
#include "ManagerGame/DataManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/PetNewDevilManager.h"
#include "ManagerGame/StoryManager.h"
#include "ManagerGame/OfflineRewardManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/CostumeSkinManager.h"
#include "ManagerGame/ServerMissionManager.h"
#include "ManagerGame/InfiniteArtifactManager.h"
#include "ManagerGame/TimeTravelManager.h"
#include "ManagerGame/ChatManager.h"
#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/InfiniteArtifactManager.h"
#include "ManagerGame/PrisonManager.h"
#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/AdventureRelicManager.h"
#include "ManagerGame/WeaponSpiritManager.h"
#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/TownManager.h"
#include "ManagerGame/SpecialMissionManager.h"
#include "ManagerGame/DialogManager.h"
#include "ManagerGame/WeaponManager.h"
#include "ManagerGame/MineralManager.h"
#include "ManagerGame/FlashmanManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/MonsterManager.h"
#include "ManagerGame/StoneManager.h"

SaveManager::SaveManager() :
_strSaveData(""),
_nSaveTime(0),
_nSaveFloorHigh(0),
_nSaveFloorTotal(0),

_callbackSave(nullptr),
_callbackLoad(nullptr),
_callbackDestroyUserData(nullptr)
{
    
}

SaveManager::~SaveManager(void)
{
    
}

void SaveManager::timerUpdate(float dt)
{
    // 플레이 시간 저장(1초)
    int nDeltaTime = (int)dt;
    
    std::string strPlayTime = SaveManager::getHistoryData(E_HISTORY_DATA::K_PLAY_TIME);
    long nTime = atol(strPlayTime.c_str());
    
    nTime += nDeltaTime;
    SaveManager::setHistoryData(E_HISTORY_DATA::K_PLAY_TIME, MafUtils::toString(nTime));
    
    // 오프라인 보상 : 현재 시간 저장
    OfflineRewardManager::getInstance()->setOfflineLastTime();
    
    // 광고 대기
    if ( AdsUtilsManager::getInstance()->getAdsCooldown() > 0 )
    {
        AdsUtilsManager::getInstance()->addAdsCooldown();
    }
    
    //
    int nTimeMin = (int)nTime / 60;
    int nTimeSec = (int)nTime % 60;
    if ( nTimeSec == 0 )
    {
        std::string strEventName = MafUtils::format(kAnalEventPlayTime, nTimeMin);
        switch (nTimeMin) {
            case 5: case 10: case 15: case 20: case 25: case 30: case 35: case 40: case 45: case 50: case 55: case 60:
                MafAnalyticsManager::LogEvent(strEventName.c_str(), kRepeatFalse);
                break;
            default:
                break;
        }
    }
    
}

#pragma mark - cloud
std::string SaveManager::saveCloudData()
{
    saveAllData();
    
    std::stringstream str;
    std::string strTrim;
    std::string temp;
    UserInfoManager *info = UserInfoManager::getInstance();
    UserDefault *user = UserDefault::getInstance();
    
    rapidjson::StringBuffer bf;
    rapidjson::Writer<rapidjson::StringBuffer> wr(bf);
    wr.StartObject();
    
    wr.String(KEY_VERSION);
    wr.String(UserDefault::getInstance()->getStringForKey(KEY_VERSION, "").c_str());
    
    temp.clear();
    temp = user->getStringForKey(KEY_HACK_SIZE);
    if(!temp.empty()){
        wr.String(KEY_HACK_SIZE);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_HACK_HASH);
    if(!temp.empty()){
        wr.String(KEY_HACK_HASH);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_HACK_STORE);
    if(!temp.empty()){
        wr.String(KEY_HACK_STORE);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(DATA_REVIVAL);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(DATA_REVIVAL);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(DATA_REVIVAL_ADS);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(DATA_REVIVAL_ADS);
        wr.String(temp.c_str());
    }
    
    strTrim.clear();
    strTrim = MafUtils::toString(ItemsMoneyManager::getInstance()->getGem());
    if(!strTrim.empty()){
        wr.String(KEY_GEM);
        wr.String(strTrim.c_str());
    }
    
    strTrim.clear();
    strTrim = MafUtils::toString(ItemsMoneyManager::getInstance()->getPoint());
    if(!strTrim.empty()){
        wr.String(KEY_POINT);
        wr.String(strTrim.c_str());
    }
    
    strTrim.clear();
    strTrim = MafUtils::toString(ItemsMoneyManager::getInstance()->getCube());
    if(!strTrim.empty()){
        wr.String(KEY_CUBE);
        wr.String(strTrim.c_str());
    }
    
    strTrim.clear();
    strTrim = MafUtils::toString(ItemsMoneyManager::getInstance()->getMythril());
    if(!strTrim.empty()){
        wr.String(KEY_MYTHRIL);
        wr.String(strTrim.c_str());
    }
    
    // 골드, 열쇠, 보석 획득 최대 길이(품위 유지용 셉터 코스튬)
    strTrim.clear();
    strTrim = MafUtils::toString(ItemsMoneyManager::getInstance()->getGoldLength());
    if(!strTrim.empty()){
        wr.String(KEY_LENGTH_GOLD);
        wr.String(strTrim.c_str());
    }
    
    strTrim.clear();
    strTrim = MafUtils::toString(ItemsMoneyManager::getInstance()->getKeyLength());
    if(!strTrim.empty()){
        wr.String(KEY_LENGTH_KEY);
        wr.String(strTrim.c_str());
    }
    
    strTrim.clear();
    strTrim = MafUtils::toString(ItemsMoneyManager::getInstance()->getGemLength());
    if(!strTrim.empty()){
        wr.String(KEY_LENGTH_GEM);
        wr.String(strTrim.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_MONSTER_SOUL);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_MONSTER_SOUL);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_MONSTER_SOUL_LEVEL);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_MONSTER_SOUL_LEVEL);
        wr.String(temp.c_str());
    }
    
    str.str("");
    for(int i=1; i<=C_COUNT_ARTIFACT;i++){
        if(i<C_COUNT_ARTIFACT)
            str << info->getArtifactLevel(E_PLACE::DG_NORMAL, (E_ARTIFACT)i) <<",";
        else
            str << info->getArtifactLevel(E_PLACE::DG_NORMAL, (E_ARTIFACT)i);
    }
    strTrim.clear();
    strTrim = str.str();
    if(!strTrim.empty()){
        wr.String(KEY_ARTIFACT_LEVEL);
        wr.String(strTrim.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_ARTIFACT_REINFORCE);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_ARTIFACT_REINFORCE);
        wr.String(temp.c_str());
    }
    
    wr.String(KEY_USER_AGREE);
    wr.Int((int)user->getBoolForKey(KEY_USER_AGREE));
    
    wr.String(KEY_PLACE);
    wr.Int(user->getIntegerForKey(KEY_PLACE));
    
    temp.clear();
    temp = user->getStringForKey(DATA_HIGH_FLOOR);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(DATA_HIGH_FLOOR);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(DATA_TOTAL_FLOOR);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(DATA_TOTAL_FLOOR);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_COSTUME_EQUIP);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_COSTUME_EQUIP);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_COSTUME_BUY);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_COSTUME_BUY);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_COSTUME_AURA_EQUIP);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_COSTUME_AURA_EQUIP);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_COSTUME_AURA_BUY);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_COSTUME_AURA_BUY);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_COSTUME_RENT_LIST);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_COSTUME_RENT_LIST);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_COSTUME_RENT_TIME);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_COSTUME_RENT_TIME);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_COSTUME_COUPON);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_COSTUME_COUPON);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_COSTUME_SKIN);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_COSTUME_SKIN);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_COSTUME_SKIN_BUY);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_COSTUME_SKIN_BUY);
        wr.String(temp.c_str());
    }

    temp.clear();
    temp = user->getStringForKey(KEY_MONSTER_SOUL_PIECE);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_MONSTER_SOUL_PIECE);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_MINE_MINERAL);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_MINE_MINERAL);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_MINE_TIME);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_MINE_TIME);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_MINE_BUY_MINERAL);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_MINE_BUY_MINERAL);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_MINE_SPECIAL);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_MINE_SPECIAL);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_MINE_REFINE_OPEN);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_MINE_REFINE_OPEN);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_MINE_REFINE_BUY_RESETTIME);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_MINE_REFINE_BUY_RESETTIME);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_MINE_REFINE_BUY_COUNT);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_MINE_REFINE_BUY_COUNT);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_MINE_REFINE_BUY_ADS_RESETTIME);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_MINE_REFINE_BUY_ADS_RESETTIME);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_MINE_REFINE_BUY_ADS_COUNT);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_MINE_REFINE_BUY_ADS_COUNT);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_MINE_REFINE_PRODUCTION_COUNT);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_MINE_REFINE_PRODUCTION_COUNT);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_MINE_REFINE_PRODUCTION_STARTTIME);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_MINE_REFINE_PRODUCTION_STARTTIME);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_MINE_REFINE_PRODUCTION_ITEM);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_MINE_REFINE_PRODUCTION_ITEM);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_MINE_REFINE_UPGRADE_LEVEL);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_MINE_REFINE_UPGRADE_LEVEL);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_FLASHMAN_SCORE_BEST);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_FLASHMAN_SCORE_BEST);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_MYHOME_LEVEL);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_MYHOME_LEVEL);
        wr.String(temp.c_str());
    }
        
    temp.clear();
    temp = user->getStringForKey(KEY_MYHOME_WATCHCOUNT);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_MYHOME_WATCHCOUNT);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_MYHOME_COUNT);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_MYHOME_COUNT);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_TIME_ADS_MYHOME_BUFF);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_TIME_ADS_MYHOME_BUFF);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_TIME_ADS_MYHOME_GEM);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_TIME_ADS_MYHOME_GEM);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_TIME_ADS_MYHOME_GOLD);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_TIME_ADS_MYHOME_GOLD);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_TIME_ADS_MYHOME_CUBE);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_TIME_ADS_MYHOME_CUBE);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_TIME_ADS_MYHOME_COSTUMECOIN);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_TIME_ADS_MYHOME_COSTUMECOIN);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_TIME_ADS_MYHOME_MINE);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_TIME_ADS_MYHOME_MINE);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_TIME_ADS_MYHOME_BONTRIUM);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_TIME_ADS_MYHOME_BONTRIUM);
        wr.String(temp.c_str());
    }
    strTrim.clear();
    strTrim = MafUtils::toString(info->getHitCount(E_PLACE::DG_NORMAL));
    if(!strTrim.empty()){
        wr.String(KEY_HITCOUNT_NORMAL);
        wr.String(strTrim.c_str());
    }
    strTrim.clear();
    strTrim = MafUtils::toString(info->getHitCount(E_PLACE::DG_PRISON));
    if(!strTrim.empty()){
        wr.String(KEY_HITCOUNT_DUNGEON);
        wr.String(strTrim.c_str());
    }
    strTrim.clear();
    strTrim = MafUtils::toString(info->getHongHitCount(E_PLACE::DG_NORMAL));
    if(!strTrim.empty()){
        wr.String(KEY_HITCOUNT_HONG_NORMAL);
        wr.String(strTrim.c_str());
    }
    strTrim.clear();
    strTrim = MafUtils::toString(info->getHongHitCount(E_PLACE::DG_PRISON));
    if(!strTrim.empty()){
        wr.String(KEY_HITCOUNT_HONG_DUNGEON);
        wr.String(strTrim.c_str());
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    strTrim.clear();
    strTrim = ItemsMoneyManager::getInstance()->getGold().c_str();
    if(!strTrim.empty()){
        wr.String(KEY_GOLD);
        wr.String(strTrim.c_str());
    }
    
    strTrim.clear();
    strTrim = ItemsMoneyManager::getInstance()->getKey();
    if(!strTrim.empty()){
        wr.String(KEY_KEY);
        wr.String(strTrim.c_str());
    }
    
    strTrim.clear();
    strTrim = MafUtils::toString(info->getFloor(E_PLACE::DG_NORMAL));
    if(!strTrim.empty()){
        wr.String(KEY_FLOOR);
        wr.String(strTrim.c_str());
    }
    
    strTrim.clear();
    strTrim = MafUtils::toString(info->getFloorCount(E_PLACE::DG_NORMAL));
    if(!strTrim.empty()){
        wr.String(KEY_FLOOR_COUNT);
        wr.String(strTrim.c_str());
    }
    
    strTrim.clear();
    strTrim = user->getStringForKey(KEY_EQUIP);
    if(!strTrim.empty()){
        strTrim = MafAes256::Decrypt(strTrim);
        wr.String(KEY_EQUIP);
        wr.String(strTrim.c_str());
    }
    
    strTrim.clear();
    strTrim = user->getStringForKey(KEY_EQUIP_VISUAL);
    if(!strTrim.empty()){
        strTrim = MafAes256::Decrypt(strTrim);
        wr.String(KEY_EQUIP_VISUAL);
        wr.String(strTrim.c_str());
    }
    
    strTrim.clear();
    strTrim = user->getStringForKey(KEY_EQUIP_HIGH);
    if(!strTrim.empty()){
        strTrim = MafAes256::Decrypt(strTrim);
        wr.String(KEY_EQUIP_HIGH);
        wr.String(strTrim.c_str());
    }
    
    strTrim.clear();
    strTrim = user->getStringForKey(KEY_EQUIP_REBORN);
    if(!strTrim.empty()){
        strTrim = MafAes256::Decrypt(strTrim);
        wr.String(KEY_EQUIP_REBORN);
        wr.String(strTrim.c_str());
    }
    
    strTrim.clear();
    strTrim = user->getStringForKey(KEY_EQUIP_PERMANENT);
    if(!strTrim.empty()){
        strTrim = MafAes256::Decrypt(strTrim);
        wr.String(KEY_EQUIP_PERMANENT);
        wr.String(strTrim.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_EQUIP_LEVEL);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_EQUIP_LEVEL);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_EQUIP_REINFORCE);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_EQUIP_REINFORCE);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_ENCHANT_STONE_STORAGE);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_ENCHANT_STONE_STORAGE);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_ENCHANT_STONE);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_ENCHANT_STONE);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_ENCHANT_STONE_LOCAL);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_ENCHANT_STONE_LOCAL);
        wr.String(temp.c_str());
    }
    
    str.str("");
    for(int i=1; i<=C_COUNT_RELIC;i++){
        if(i<C_COUNT_RELIC)
            str << info->getRelicLevel(E_PLACE::DG_NORMAL, (E_RELIC)i) <<",";
        else
            str << info->getRelicLevel(E_PLACE::DG_NORMAL, (E_RELIC)i);
    }
    strTrim.clear();
    strTrim = str.str();
    if(!strTrim.empty()){
        wr.String(KEY_RELIC_LEVEL);
        wr.String(strTrim.c_str());
    }
    
    strTrim.clear();
    strTrim = user->getStringForKey(KEY_JELLY_NEW);
    if(!strTrim.empty()){
        strTrim = MafAes256::Decrypt(strTrim);
        wr.String(KEY_JELLY_NEW);
        wr.String(strTrim.c_str());
    }
    
    str.str("");
    for(int i=1; i<=C_COUNT_BUILDING;i++){
        if(i<C_COUNT_BUILDING)
            str << TownManager::getInstance()->getTownLevel((E_TOWN)i) <<",";
        else
            str << TownManager::getInstance()->getTownLevel((E_TOWN)i);
    }
    strTrim.clear();
    strTrim = str.str();
    if(!strTrim.empty()){
        wr.String(KEY_TOWN_LEVEL);
        wr.String(strTrim.c_str());
    }
    
    str.str("");
    for(int i=1; i<=C_COUNT_BUILDING;i++){
        if(i<C_COUNT_BUILDING){
            str << TownManager::getInstance()->getBuildingProgress((E_TOWN)i) <<",";
        }
        else{
            str << TownManager::getInstance()->getBuildingProgress((E_TOWN)i);
        }
    }
    strTrim.clear();
    strTrim = str.str();
    
    if(!strTrim.empty()){
        wr.String(KEY_TOWN_BUILDING_PROGRESS);
        wr.String(strTrim.c_str());
    }
    
    strTrim.clear();
    strTrim = user->getStringForKey(KEY_TOWN_SATELLITE_STEP);
    if(!strTrim.empty()){
        strTrim = MafAes256::Decrypt(strTrim);
        wr.String(KEY_TOWN_SATELLITE_STEP);
        wr.String(strTrim.c_str());
    }
    
    strTrim.clear();
    strTrim = user->getStringForKey(KEY_TOWN_SATELLITE_TUTORIAL);
    if(!strTrim.empty()){
        strTrim = MafAes256::Decrypt(strTrim);
        wr.String(KEY_TOWN_SATELLITE_TUTORIAL);
        wr.String(strTrim.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_HONOR);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_HONOR);
        wr.String(temp.c_str());
    }
    
    strTrim.clear();
    strTrim = user->getStringForKey(KEY_PET_LEVEL);
    if(!strTrim.empty()){
        strTrim = MafAes256::Decrypt(strTrim);
        wr.String(KEY_PET_LEVEL);
        wr.String(strTrim.c_str());
    } 
    
    strTrim.clear();
    strTrim = user->getStringForKey(KEY_PET_EVOLUTION);
    if(!strTrim.empty()){
        strTrim = MafAes256::Decrypt(strTrim);
        wr.String(KEY_PET_EVOLUTION);
        wr.String(strTrim.c_str());
    }
    
    strTrim.clear();
    strTrim = user->getStringForKey(KEY_PET_EVOLUTION_REWARD);
    if(!strTrim.empty()){
        strTrim = MafAes256::Decrypt(strTrim);
        wr.String(KEY_PET_EVOLUTION_REWARD);
        wr.String(strTrim.c_str());
    }
    
    strTrim.clear();
    strTrim = user->getStringForKey(KEY_PET_SKIN);
    if(!strTrim.empty()){
        strTrim = MafAes256::Decrypt(strTrim);
        wr.String(KEY_PET_SKIN);
        wr.String(strTrim.c_str());
    }
    
    strTrim.clear();
    strTrim = user->getStringForKey(KEY_PET_SKIN_BUY);
    if(!strTrim.empty()){
        strTrim = MafAes256::Decrypt(strTrim);
        wr.String(KEY_PET_SKIN_BUY);
        wr.String(strTrim.c_str());
    }
    
    strTrim.clear();
    strTrim = user->getStringForKey(KEY_PET_REINFORCE_LEVEL);
    if(!strTrim.empty()){
        strTrim = MafAes256::Decrypt(strTrim);
        wr.String(KEY_PET_REINFORCE_LEVEL);
        wr.String(strTrim.c_str());
    }
    
    // 펫(바포메트)
    strTrim.clear();
    strTrim = user->getStringForKey(KEY_PET_BAFOMETZ);
    if(!strTrim.empty()){
        strTrim = MafAes256::Decrypt(strTrim);
        wr.String(KEY_PET_BAFOMETZ);
        wr.String(strTrim.c_str());
    }
    
    str.str("");
    for(int i=1; i<=C_COUNT_QUEST;i++){
        if(i<C_COUNT_QUEST)
            str << info->getQuestLevel(E_PLACE::DG_NORMAL, (E_QUEST)i) <<",";
        else
            str << info->getQuestLevel(E_PLACE::DG_NORMAL, (E_QUEST)i);
    }
    strTrim.clear();
    strTrim = str.str();
    if(!strTrim.empty()){
        wr.String(KEY_QUEST_LEVEL);
        wr.String(strTrim.c_str());
    }
    
    strTrim.clear();
    
    strTrim = MafUtils::toString(info->getBuffTime(E_BUFF_TYPE::BUFF_ATTACK_BONUS));
    wr.String(KEY_TIME_ATTACK_BONUS);
    wr.String(strTrim.c_str());
    
    strTrim = MafUtils::toString(info->getBuffTime(E_BUFF_TYPE::BUFF_ATTACK_9_BONUS));
    wr.String(KEY_TIME_ATTACK_9_BONUS);
    wr.String(strTrim.c_str());
    
    strTrim = MafUtils::toString(info->getBuffTime(E_BUFF_TYPE::BUFF_ATTACK_10_BONUS));
    wr.String(KEY_TIME_ATTACK_10_BONUS);
    wr.String(strTrim.c_str());

    strTrim.clear();
    strTrim = MafUtils::toString(info->getBuffTime(E_BUFF_TYPE::BUFF_COIN_BONUS));
    wr.String(KEY_TIME_COIN_BONUS);
    wr.String(strTrim.c_str());
    
    strTrim.clear();
    strTrim = MafUtils::toString(info->getBuffTime(E_BUFF_TYPE::BUFF_MOVE_BONUS));
    wr.String(KEY_TIME_MOVE_BONUS);
    wr.String(strTrim.c_str());
    
    strTrim.clear();
    strTrim = MafUtils::toString(info->getBuffTime(E_BUFF_TYPE::BUFF_KEY_BONUS));
    wr.String(KEY_COUNT_KEY_BONUS);
    wr.String(strTrim.c_str());
    
    strTrim.clear();
    strTrim = MafUtils::toString(info->getBuffTime(E_BUFF_TYPE::BUFF_HERO_BONUS));
    wr.String(KEY_COUNT_HERO_BONUS);
    wr.String(strTrim.c_str());
    
    strTrim.clear();
    strTrim = MafUtils::toString(info->getAdsTime(E_SHOP_ADS::SHOP_GOLD_ADS));
    wr.String(KEY_TIME_ADS_GOLD_BONUS);
    wr.String(strTrim.c_str());
    
    strTrim.clear();
    strTrim = MafUtils::toString(info->getAdsTime(E_SHOP_ADS::SHOP_GEM_ADS));
    wr.String(KEY_TIME_ADS_GEM_BONUS);
    wr.String(strTrim.c_str());
    
    strTrim.clear();
    strTrim = MafUtils::toString(info->getAdsTime(E_SHOP_ADS::SHOP_BUFF_ADS));
    wr.String(KEY_TIME_ADS_BUFF_BONUS);
    wr.String(strTrim.c_str());
    
    strTrim.clear();
    strTrim = MafUtils::toString(info->getAdsTime(E_SHOP_ADS::SHOP_MINE_ADS));
    wr.String(KEY_TIME_ADS_MINE_BONUS);
    wr.String(strTrim.c_str());
    
    strTrim.clear();
    strTrim = MafUtils::toString(info->getAdsTime(E_SHOP_ADS::SHOP_SELECT_ADS));
    wr.String(KEY_TIME_ADS_SELECT_BONUS);
    wr.String(strTrim.c_str());
    
    strTrim.clear();
    strTrim = MafUtils::toString(info->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_ATK));
    wr.String(KEY_TIME_ADS_BUFF_ATK);
    wr.String(strTrim.c_str());
    
    strTrim.clear();
    strTrim = MafUtils::toString(info->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_GOLD));
    wr.String(KEY_TIME_ADS_BUFF_GOLD);
    wr.String(strTrim.c_str());
    
    strTrim.clear();
    strTrim = MafUtils::toString(info->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_MOVE));
    wr.String(KEY_TIME_ADS_BUFF_MOVE);
    wr.String(strTrim.c_str());
    
    strTrim.clear();
    strTrim = MafUtils::toString(info->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_ATK));
    wr.String(KEY_TIME_ADS_BUFF_D_ATK);
    wr.String(strTrim.c_str());
    
    strTrim.clear();
    strTrim = MafUtils::toString(info->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_ASPD));
    wr.String(KEY_TIME_ADS_BUFF_D_ASPD);
    wr.String(strTrim.c_str());
    
    strTrim.clear();
    strTrim = MafUtils::toString(info->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_GOLD));
    wr.String(KEY_TIME_ADS_BUFF_D_GOLD);
    wr.String(strTrim.c_str());
    
    strTrim.clear();
    strTrim = MafUtils::toString(info->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_MOVE));
    wr.String(KEY_TIME_ADS_BUFF_D_MOVE);
    wr.String(strTrim.c_str());
    
    strTrim.clear();
    strTrim = MafUtils::toString(info->getAdsTime(E_SHOP_ADS::SHOP_BUFF_GOLD_INTERSTITIAL));
    wr.String(KEY_TIME_ADS_BUFF_GOLD_NTERSTITIAL);
    wr.String(strTrim.c_str());
    
    strTrim.clear();
    strTrim = MafUtils::toString(info->getItemFeverTime());
    wr.String(KEY_TIME_ITEM_FEVER);
    wr.String(strTrim.c_str());
    
    strTrim.clear();
    strTrim = MafUtils::toString(info->getItemFeverCount());
    wr.String(KEY_TIME_ITEM_FEVER_COUNT);
    wr.String(strTrim.c_str());
    
    strTrim.clear();
    strTrim = MafAes256::Decrypt(UserDefault::getInstance()->getStringForKey(KEY_R_S_MAX_LV, "0"));
    wr.String(KEY_R_S_MAX_LV);
    wr.String(strTrim.c_str());
    
    strTrim.clear();
    strTrim = MafAes256::Decrypt(UserDefault::getInstance()->getStringForKey(KEY_R_S_MAX_DAMAGE, "0"));
    wr.String(KEY_R_S_MAX_DAMAGE);
    wr.String(strTrim.c_str());
    
    temp.clear();
    temp = user->getStringForKey(KEY_LOGIN_GAMECENTER_ID);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_LOGIN_GAMECENTER_ID);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_LOGIN_GOOGLEPLAY_ID);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_LOGIN_GOOGLEPLAY_ID);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_LOGIN_GUEST);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_LOGIN_GUEST);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_DRAGON_SKILLCHECK);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_DRAGON_SKILLCHECK);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_REVIVAL_ATK_BUFF);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_REVIVAL_ATK_BUFF);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_REVIVAL_QUEST_BUFF);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_REVIVAL_QUEST_BUFF);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_REVIVAL_KEY_BUFF1);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_REVIVAL_KEY_BUFF1);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_REVIVAL_KEY_BUFF2);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_REVIVAL_KEY_BUFF2);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_E_REVIVAL_ATK_BUFF);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_E_REVIVAL_ATK_BUFF);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_HIT_BUFF);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_HIT_BUFF);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_DRAGON_BUFF);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_DRAGON_BUFF);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_HELL_BUFF);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_HELL_BUFF);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_QUEST_25_BUFF);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_QUEST_25_BUFF);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_REVIVE_SPECIAL);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_REVIVE_SPECIAL);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_REVIVE_KEYBOOSTER);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_REVIVE_KEYBOOSTER);
        wr.String(temp.c_str());
    }
    
    
    
    temp.clear();
    temp = user->getStringForKey(KEY_SPECIAL_CLEAR);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_SPECIAL_CLEAR);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_SPECIAL_COMPLETE);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_SPECIAL_COMPLETE);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_ATTEND_VIP);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_ATTEND_VIP);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_ATTEND_DAY);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_ATTEND_DAY);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_ATTEND_ADS);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_ATTEND_ADS);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_MONSTER_HP,"0");
    wr.String(KEY_MONSTER_HP);
    wr.String(temp.c_str());

    // 다음 버전에는 지워야 함.
    auto count = user->getIntegerForKey("package", 0);
    if(count!=0){
        wr.String("package");
        wr.Int(count);
    }
    count = user->getIntegerForKey("package2", 0);
    if(count!=0){
        wr.String("package2");
        wr.Int(count);
    }
    count = user->getIntegerForKey("packagenew1", 0);
    if(count!=0){
        wr.String("packagenew1");
        wr.Int(count);
    }
    count = user->getIntegerForKey("packagenew2", 0);
    if(count!=0){
        wr.String("packagenew2");
        wr.Int(count);
    }
    
    // 보물 결제
    for(int i=E_ARTIFACT::ARTI_1_EVOL1; i< E_ARTIFACT::E_ARTIFACT_999_NONE; i++)
    {
        std::string str = "";
        
        str = MafUtils::format("pack_%d",i).c_str();
        count = user->getIntegerForKey(str.c_str(), 0);
        if(count!=0)
        {
            wr.String(str.c_str());
            wr.Int(count);
        }
        
        str = MafUtils::format("pack_e_%d",i).c_str();
        count = user->getIntegerForKey(str.c_str(), 0);
        if(count!=0)
        {
            wr.String(str.c_str());
            wr.Int(count);
        }
        
        str = MafUtils::format("pack_w_%d",i).c_str();
        count = user->getIntegerForKey(str.c_str(), 0);
        if(count!=0)
        {
            wr.String(str.c_str());
            wr.Int(count);
        }
    }
    //무한 보물 결제
    
    for(int i = 1; i <= InfiniteArtifactManager::getInstance()->getMaxGrade(); i++)
    {
        std::string str = "";
        
        for(int j = 1; j <= INFINITE_ARTIFACT_KIND; ++j)
        {
            str = MafUtils::format("Infinite_pack_%d_%d",i, j).c_str();
            count = user->getIntegerForKey(str.c_str(), 0);
            if(count!=0)
            {
                wr.String(str.c_str());
                wr.Int(count);
            }
        }
    }
    
    // 사용자 결제 총 건수
    wr.String(KEY_PURCHASE_COUNT);
    wr.Int(CashManager::getInstance()->getHistoryTotal());

    
    // 사용자 결제 개별 건수
    temp.clear();
    wr.String(KEY_PURCHASE_HISTORY);
    wr.String(CashManager::getInstance()->getHistoryProductCloud().c_str());
    
    // 사용자 결제 이벤트 성 건수(초보자, 중급자)
    temp.clear();
    wr.String(KEY_PURCHASE_ADS);
    wr.String(CashManager::getInstance()->getHistoryAdsCloud().c_str());
    
    temp.clear();
    wr.String(KEY_SERVER_MISSION_DAILY);
    wr.String(ServerMissionManager::getInstance()->getServerMissionData(E_MISSION_RESET_TYPE::DAILY).c_str());
    
    temp.clear();
    wr.String(KEY_SERVER_MISSION_WEEKLY);
    wr.String(ServerMissionManager::getInstance()->getServerMissionData(E_MISSION_RESET_TYPE::WEEK).c_str());
    
    temp.clear();
    wr.String(KEY_SERVER_MISSION_SEASON_DAILY);
    wr.String(user->getStringForKey(KEY_SERVER_MISSION_SEASON_DAILY).c_str());
    
    temp.clear();
    wr.String(KEY_SERVER_MISSION_SEASON_WEEKLY);
    wr.String(user->getStringForKey(KEY_SERVER_MISSION_SEASON_WEEKLY).c_str());
    
    temp.clear();
    wr.String(KEY_INFINITE_ARTI);
    wr.String(InfiniteArtifactManager::getInstance()->getInfiniteArtifactData().c_str());
    wr.String(KEY_ACHIEVE_NORMAL_COUNT);
    wr.String(MafAes256::Decrypt(UserDefault::getInstance()->getStringForKey(KEY_ACHIEVE_NORMAL_COUNT, "")).c_str());
    
    temp.clear();
    wr.String(KEY_ACHIEVE_NORMAL_COMPLETE);
    wr.String(MafAes256::Decrypt(UserDefault::getInstance()->getStringForKey(KEY_ACHIEVE_NORMAL_COMPLETE, "")).c_str());
    
    temp.clear();
    wr.String(KEY_ACHIEVE_NORMAL_GRADE);
    wr.String(MafAes256::Decrypt(UserDefault::getInstance()->getStringForKey(KEY_ACHIEVE_NORMAL_GRADE, "")).c_str());
    
    temp.clear();
    wr.String(KEY_TIMING_PACKAGE_FREE_COMPLETE);
    wr.String(MafAes256::Decrypt(UserDefault::getInstance()->getStringForKey(KEY_TIMING_PACKAGE_FREE_COMPLETE, "")).c_str());
    
    temp.clear();
    wr.String("NAS_KEY");
    wr.String(MafNative::GetDeviceID().c_str());
    
    // 만능 포인트 보관함
    temp.clear();
    temp = user->getStringForKey(KEY_REVIVE_POINT);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_REVIVE_POINT);
        wr.String(temp.c_str());
    }
    
    // 피버 하루 사용 수
    temp.clear();
    temp = user->getStringForKey(KEY_REVIVE_FEVER_DAY);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_REVIVE_FEVER_DAY);
        wr.String(temp.c_str());
    }
    
    // 큐브 하루 획득 수
    temp.clear();
    temp = user->getStringForKey(KEY_REVIVE_CUBE_DAY);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_REVIVE_CUBE_DAY);
        wr.String(temp.c_str());
    }
    
    // 환생 5배 버프
    temp.clear();
    temp = user->getStringForKey(KEY_REVIVE_BUFF);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_REVIVE_BUFF);
        wr.String(temp.c_str());
    }
    
    
    // 스토리
    wr.String(KEY_COMIC_SHOW);
    wr.String(StoryManager::getInstance()->getCloudStory().c_str());
    
    // 다이얼로그
    wr.String(KEY_DIALOG_SHOW);
    wr.String(DialogManager::getInstance()->getCloud().c_str());
    
    wr.String(KEY_HISTORY_DATA);
    wr.String(getHistorySaveToServerData().c_str());
    
    
    wr.String(KEY_TODAY);
    wr.String(UserDefault::getInstance()->getStringForKey(KEY_TODAY, "").c_str());
    
    
    temp.clear();
    temp = user->getStringForKey(DATA_D_HIGH_FLOOR);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(DATA_D_HIGH_FLOOR);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(DATA_D_TOTAL_FLOOR);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(DATA_D_TOTAL_FLOOR);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_D_FLOOR);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_D_FLOOR);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_D_FLOOR_COUNT);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_D_FLOOR_COUNT);
        wr.String(temp.c_str());
    }
   
    temp.clear();
    temp = user->getStringForKey(KEY_D_REVIVAL);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_D_REVIVAL);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_D_MONSTER_HP);
    if(!temp.empty()){
        wr.String(KEY_D_MONSTER_HP);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_D_GOLD);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_D_GOLD);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_D_KEY);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_D_KEY);
        wr.String(temp.c_str());
    }
    
    
    temp.clear();
    temp = user->getStringForKey(KEY_D_QUEST_LEVEL);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_D_QUEST_LEVEL);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_D_QUEST_TIME);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_D_QUEST_TIME);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_D_EQUIP);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_D_EQUIP);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_D_EQUIP_LEVEL);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_D_EQUIP_LEVEL);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_D_EQUIP_REINFORCE);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_D_EQUIP_REINFORCE);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_D_PET_LEVEL);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_D_PET_LEVEL);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_D_PET_GRADE);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_D_PET_GRADE);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_D_ARTIFACT);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_D_ARTIFACT);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_D_TIME_BUFF);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_D_TIME_BUFF);
        wr.String(temp.c_str());
    }
    temp.clear();
    temp = user->getStringForKey(KEY_D_TIME_ADS);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_D_TIME_ADS);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_D_HIT_COUNT_HEDGEHOG);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_D_HIT_COUNT_HEDGEHOG);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_D_HIT_COUNT_FLASH);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_D_HIT_COUNT_FLASH);
        wr.String(temp.c_str());
    }
    
    
    temp.clear();
    temp = user->getStringForKey(KEY_SAVE_TIME);
    if(!temp.empty()){
        wr.String(KEY_SAVE_TIME);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_TUTORIAL_PROGRESS);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_TUTORIAL_PROGRESS);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_TIME_TRAVEL_LEVEL);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_TIME_TRAVEL_LEVEL);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_TIME_TRAVEL_COUNT);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_TIME_TRAVEL_COUNT);
        wr.String(temp.c_str());
    }
    
    temp.clear();
    temp = user->getStringForKey(KEY_TIME_TRAVEL_TIME);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_TIME_TRAVEL_TIME);
        wr.String(temp.c_str());
    }
    
    
    temp.clear();
    temp = user->getStringForKey(KEY_ADVT_OFFICE_LV);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_ADVT_OFFICE_LV);
        wr.String(temp.c_str());
    }
    
    auto list = AdventureManager::getInstance()->getListInfoParty();
    int nPresetCount = 3;
    auto objSettingPresetCount = AdventureManager::getInstance()->getSetting("party_ability_preset_count");
    if(objSettingPresetCount != nullptr)
        nPresetCount = objSettingPresetCount->getValueToInt();
    
    int nPartyCount = 4;
    auto objSettingPartyCount = AdventureManager::getInstance()->getSetting("party_max");
    if(objSettingPartyCount != nullptr)
        nPartyCount = objSettingPartyCount->getValueToInt() + 1;
    
    for(int i = 0; i < list.size(); ++i)
    {
        std::string str_lv = MafUtils::format("advt_p_%d_m_lv", i+1).c_str();
        std::string str_preset_idx = MafUtils::format("advt_p_%d_a_p", i+1).c_str();
        std::string strDefault = "";
        for(int j = 0; j < nPartyCount; ++j)
        {
            strDefault += "0,";
        }
        strDefault = MafAes256::Encrypt(strDefault);
        
        for(int j = 0; j < nPresetCount; ++j)
        {
            std::string str_type = MafUtils::format("advt_p_%d_a_t_%d", i+1, j+1).c_str();
            std::string str_grade = MafUtils::format("advt_p_%d_a_g_%d", i+1, j+1).c_str();
            std::string str_seal = MafUtils::format("advt_p_%d_a_s_%d", i+1, j+1).c_str();
            
            temp.clear();
            temp = user->getStringForKey(str_type.c_str(), strDefault.c_str());
            if(!temp.empty()){
                temp = MafAes256::Decrypt(temp);
                wr.String(str_type.c_str());
                wr.String(temp.c_str());
            }
            
            temp.clear();
            temp = user->getStringForKey(str_grade.c_str(), strDefault.c_str());
            if(!temp.empty()){
                temp = MafAes256::Decrypt(temp);
                wr.String(str_grade.c_str());
                wr.String(temp.c_str());
            }
            
            temp.clear();
            temp = user->getStringForKey(str_seal.c_str(), strDefault.c_str());
            if(!temp.empty()){
                temp = MafAes256::Decrypt(temp);
                wr.String(str_seal.c_str());
                wr.String(temp.c_str());
            }
        }

        temp.clear();
        temp = user->getStringForKey(str_lv.c_str(), strDefault.c_str());
        if(!temp.empty()){
            temp = MafAes256::Decrypt(temp);
            wr.String(str_lv.c_str());
            wr.String(temp.c_str());
        }
        
        int nIdx = user->getIntegerForKey(str_preset_idx.c_str(), 1);
        wr.String(str_preset_idx.c_str());
        wr.Int(nIdx);
    }
    wr.String(KEY_ADVT_TUTORIAL_STEP);
    wr.Int(AdventureManager::getInstance()->getTutorialStep());
    wr.String(KEY_ADVT_TUTORIAL_FRIEND);
    wr.Int(UserDefault::getInstance()->getIntegerForKey(KEY_ADVT_TUTORIAL_FRIEND, 0));
    wr.String(KEY_ADVT_TUTORIAL_NEW_CORE);
    wr.Int(UserDefault::getInstance()->getIntegerForKey(KEY_ADVT_TUTORIAL_NEW_CORE, 0));
    wr.String(KEY_ADVT_AVILITY_USE_GEM);
    wr.Int(AdventureManager::getInstance()->getTotalAvilityDrawUseGem());
    
    //디펜스
    temp.clear();
    temp = user->getStringForKey(KEY_DF_COIN);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_DF_COIN);
        wr.String(temp.c_str());
    }
    temp.clear();
    temp = user->getStringForKey(KEY_DF_HIGH_FLOOR);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_DF_HIGH_FLOOR);
        wr.String(temp.c_str());
    }
    temp.clear();
    temp = user->getStringForKey(KEY_DF_CURRENT_FLOOR);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_DF_CURRENT_FLOOR);
        wr.String(temp.c_str());
    }
    temp.clear();
    temp = user->getStringForKey(KEY_DF_CHARACTER_LV);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_DF_CHARACTER_LV);
        wr.String(temp.c_str());
    }
    temp.clear();
    temp = user->getStringForKey(KEY_DF_WEAPON_LV);
    if(!temp.empty()){
        temp = MafAes256::Decrypt(temp);
        wr.String(KEY_DF_WEAPON_LV);
        wr.String(temp.c_str());
    }
    
    
    wr.EndObject();
    std::string saveData=bf.GetString();

    return saveData;
    
}

void SaveManager::loadCloudData()
{
    if ( _strSaveData.length() == 0 )
    {
        return;
    }
    
    E_PLACE currentPlace = UserInfoManager::getInstance()->getCurrentPlace();
    if ( currentPlace == E_PLACE::DG_NORMAL )
    {
        HelloWorldScene::getInstance()->stopAllActions();
        HelloWorldScene::getInstance()->unscheduleAllCallbacks();
        
        HelloWorldScene::getInstance()->unschedule(schedule_selector(HelloWorldScene::timeSchedule));
    }
    else if ( currentPlace == E_PLACE::DG_DEVIL_DOM )
    {
        DevilDomScene::getInstance()->stopAllActions();
        DevilDomScene::getInstance()->unscheduleAllCallbacks();
    }
    
    auto uiBattle = dynamic_cast<IBattleInfo*>(UserInfoManager::getInstance()->getBattleUI());
    if ( uiBattle != nullptr )
    {
        auto unitHero = uiBattle->getUnitHero();
        if ( unitHero != nullptr )
        {
            unitHero->heroStopAllAction();
        }
        
        auto listPets = uiBattle->getUnitPets();
        for ( auto unitPet : listPets )
        {
            unitPet->stopAllActions();
            unitPet->unscheduleUpdate();
        }
    }
    
    //
    UserInfoManager::destroyInstance();
    
    //
    std::string loadData = _strSaveData;
    std::string str;
    _strSaveData = "";
    
    //UserInfo *info = UserInfoManager::getInstance();
    UserDefault *user = UserDefault::getInstance();
    
    rapidjson::Document jsondoc;
    jsondoc.ParseInsitu<0>((char*)loadData.c_str()).HasParseError();
        
    if(jsondoc.HasMember(KEY_HACK_SIZE)){
        str = jsondoc[KEY_HACK_SIZE].GetString();
        user->setStringForKey(KEY_HACK_SIZE, str);
    }
    else{
        user->setStringForKey(KEY_HACK_SIZE, "");
    }
    
    if(jsondoc.HasMember(KEY_HACK_HASH)){
        str = jsondoc[KEY_HACK_HASH].GetString();
        user->setStringForKey(KEY_HACK_HASH, str);
    }
    else{
        user->setStringForKey(KEY_HACK_HASH, "");
    }
    
    if(jsondoc.HasMember(KEY_HACK_STORE)){
        str = jsondoc[KEY_HACK_STORE].GetString();
        user->setStringForKey(KEY_HACK_STORE, str);
    }
    else{
        user->setStringForKey(KEY_HACK_STORE, "");
    }
    
    if(jsondoc.HasMember(KEY_USER_AGREE)){
        user->setBoolForKey(KEY_USER_AGREE, (bool)jsondoc[KEY_USER_AGREE].GetInt());
    }
    
    if(jsondoc.HasMember(KEY_GOLD)){
        str = MafAes256::Encrypt(jsondoc[KEY_GOLD].GetString());
        user->setStringForKey(KEY_GOLD, str);
    } else {
        str = MafAes256::Encrypt("100");
        user->setStringForKey(KEY_GOLD, str);
    }
    
    if(jsondoc.HasMember(KEY_KEY)){
        str = MafAes256::Encrypt(jsondoc[KEY_KEY].GetString());
        user->setStringForKey(KEY_KEY, str);
    } else {
        str = MafAes256::Encrypt("0");
        user->setStringForKey(KEY_KEY, str);
    }
    
    if(jsondoc.HasMember(KEY_GEM)){
        str = MafAes256::Encrypt(jsondoc[KEY_GEM].GetString());
        user->setStringForKey(KEY_GEM, str);
    } else {
        str = MafAes256::Encrypt("0");
        user->setStringForKey(KEY_GEM, str);
    }
    
    if(jsondoc.HasMember(KEY_POINT)){
        str = MafAes256::Encrypt(jsondoc[KEY_POINT].GetString());
        user->setStringForKey(KEY_POINT, str);
    } else {
        str = MafAes256::Encrypt("0");
        user->setStringForKey(KEY_POINT, str);
    }
    
    if(jsondoc.HasMember(KEY_CUBE)){
        str = MafAes256::Encrypt(jsondoc[KEY_CUBE].GetString());
        user->setStringForKey(KEY_CUBE, str);
    } else {
        str = MafAes256::Encrypt("0");
        user->setStringForKey(KEY_CUBE, str);
    }
    
    if(jsondoc.HasMember(KEY_MYTHRIL)){
        str = MafAes256::Encrypt(jsondoc[KEY_MYTHRIL].GetString());
        user->setStringForKey(KEY_MYTHRIL, str);
    } else {
        str = MafAes256::Encrypt("0");
        user->setStringForKey(KEY_MYTHRIL, str);
    }
    
    
    // 골드, 열쇠, 보석 획득 최대 길이(품위 유지용 셉터 코스튬)
    if(jsondoc.HasMember(KEY_LENGTH_GOLD)){
        str = MafAes256::Encrypt(jsondoc[KEY_LENGTH_GOLD].GetString());
        user->setStringForKey(KEY_LENGTH_GOLD, str);
    } else {
        str = MafAes256::Encrypt("0");
        user->setStringForKey(KEY_LENGTH_GOLD, str);
    }
    
    if(jsondoc.HasMember(KEY_LENGTH_KEY)){
        str = MafAes256::Encrypt(jsondoc[KEY_LENGTH_KEY].GetString());
        user->setStringForKey(KEY_LENGTH_KEY, str);
    } else {
        str = MafAes256::Encrypt("0");
        user->setStringForKey(KEY_LENGTH_KEY, str);
    }
    
    if(jsondoc.HasMember(KEY_LENGTH_GEM)){
        str = MafAes256::Encrypt(jsondoc[KEY_LENGTH_GEM].GetString());
        user->setStringForKey(KEY_LENGTH_GEM, str);
    } else {
        str = MafAes256::Encrypt("0");
        user->setStringForKey(KEY_LENGTH_GEM, str);
    }
    
    
    //
    if(jsondoc.HasMember(KEY_TOWN_LEVEL)){
        str = MafAes256::Encrypt(jsondoc[KEY_TOWN_LEVEL].GetString());
        user->setStringForKey(KEY_TOWN_LEVEL, str);
    } else {
        user->setStringForKey(KEY_TOWN_LEVEL, "");
    }

    if(jsondoc.HasMember(KEY_TOWN_BUILDING_PROGRESS)){
        str = MafAes256::Encrypt(jsondoc[KEY_TOWN_BUILDING_PROGRESS].GetString());
        user->setStringForKey(KEY_TOWN_BUILDING_PROGRESS, str);
    } else {
        user->setStringForKey(KEY_TOWN_BUILDING_PROGRESS, "");
    }
    
    if(jsondoc.HasMember(KEY_TOWN_SATELLITE_STEP)){
        str = MafAes256::Encrypt(jsondoc[KEY_TOWN_SATELLITE_STEP].GetString());
        user->setStringForKey(KEY_TOWN_SATELLITE_STEP, str);
    } else {
        user->setStringForKey(KEY_TOWN_SATELLITE_STEP, "");
    }
    
    if(jsondoc.HasMember(KEY_TOWN_SATELLITE_TUTORIAL)){
        str = MafAes256::Encrypt(jsondoc[KEY_TOWN_SATELLITE_TUTORIAL].GetString());
        user->setStringForKey(KEY_TOWN_SATELLITE_TUTORIAL, str);
    } else {
        user->setStringForKey(KEY_TOWN_SATELLITE_TUTORIAL, "");
    }
    
    
    if(jsondoc.HasMember(KEY_COSTUME_EQUIP)){
        str = MafAes256::Encrypt(jsondoc[KEY_COSTUME_EQUIP].GetString());
        user->setStringForKey(KEY_COSTUME_EQUIP, str);
    } else {
        user->setStringForKey(KEY_COSTUME_EQUIP, "");
    }
        
    
    if(jsondoc.HasMember(KEY_COSTUME_BUY)){
        str = MafAes256::Encrypt(jsondoc[KEY_COSTUME_BUY].GetString());
        user->setStringForKey(KEY_COSTUME_BUY, str);
    } else {
        user->setStringForKey(KEY_COSTUME_BUY, "");
    }
    
    if(jsondoc.HasMember(KEY_COSTUME_AURA_EQUIP)){
        str = MafAes256::Encrypt(jsondoc[KEY_COSTUME_AURA_EQUIP].GetString());
        user->setStringForKey(KEY_COSTUME_AURA_EQUIP, str);
    }else{
        user->setStringForKey(KEY_COSTUME_AURA_EQUIP, "");
    }
    
    if(jsondoc.HasMember(KEY_COSTUME_AURA_BUY)){
        str = MafAes256::Encrypt(jsondoc[KEY_COSTUME_AURA_BUY].GetString());
        user->setStringForKey(KEY_COSTUME_AURA_BUY, str);
    }else{
        user->setStringForKey(KEY_COSTUME_AURA_BUY, "");
    }
    
    if(jsondoc.HasMember(KEY_COSTUME_RENT_LIST)){
        str = MafAes256::Encrypt(jsondoc[KEY_COSTUME_RENT_LIST].GetString());
        user->setStringForKey(KEY_COSTUME_RENT_LIST, str);
    }else{
        user->setStringForKey(KEY_COSTUME_RENT_LIST, "");
    }
    
    if(jsondoc.HasMember(KEY_COSTUME_RENT_TIME)){
        str = MafAes256::Encrypt(jsondoc[KEY_COSTUME_RENT_TIME].GetString());
        user->setStringForKey(KEY_COSTUME_RENT_TIME, str);
    }else{
        user->setStringForKey(KEY_COSTUME_RENT_TIME, "");
    }
    
    if(jsondoc.HasMember(KEY_COSTUME_SOCKET)){
        str = MafAes256::Encrypt(jsondoc[KEY_COSTUME_SOCKET].GetString());
        user->setStringForKey(KEY_COSTUME_SOCKET, str);
    }else{
        user->setStringForKey(KEY_COSTUME_SOCKET, "");
    }
    
    if(jsondoc.HasMember(KEY_COSTUME_COUPON)){
        str = MafAes256::Encrypt(jsondoc[KEY_COSTUME_COUPON].GetString());
        user->setStringForKey(KEY_COSTUME_COUPON, str);
    }else{
        user->setStringForKey(KEY_COSTUME_COUPON, "");
    }
    
    if(jsondoc.HasMember(KEY_COSTUME_SKIN)){
        str = MafAes256::Encrypt(jsondoc[KEY_COSTUME_SKIN].GetString());
        user->setStringForKey(KEY_COSTUME_SKIN, str);
    }else{
        user->setStringForKey(KEY_COSTUME_SKIN, "");
    }
    
    if(jsondoc.HasMember(KEY_COSTUME_SKIN_BUY)){
        str = MafAes256::Encrypt(jsondoc[KEY_COSTUME_SKIN_BUY].GetString());
        user->setStringForKey(KEY_COSTUME_SKIN_BUY, str);
    }else{
        user->setStringForKey(KEY_COSTUME_SKIN_BUY, "");
    }
    
    if(jsondoc.HasMember(KEY_MONSTER_SOUL_PIECE)){
        str = MafAes256::Encrypt(jsondoc[KEY_MONSTER_SOUL_PIECE].GetString());
        user->setStringForKey(KEY_MONSTER_SOUL_PIECE, str);
    }else{
        user->setStringForKey(KEY_MONSTER_SOUL_PIECE, "");
    }
    
    if(jsondoc.HasMember(KEY_SERVER_MISSION_SEASON_WEEKLY)){
        str = jsondoc[KEY_SERVER_MISSION_SEASON_WEEKLY].GetString();
        user->setStringForKey(KEY_SERVER_MISSION_SEASON_WEEKLY, str);
    }else{
        user->setStringForKey(KEY_SERVER_MISSION_SEASON_WEEKLY, "");
    }
    
    if(jsondoc.HasMember(KEY_SERVER_MISSION_SEASON_DAILY)){
        str = jsondoc[KEY_SERVER_MISSION_SEASON_DAILY].GetString();
        user->setStringForKey(KEY_SERVER_MISSION_SEASON_DAILY, str);
    }else{
        user->setStringForKey(KEY_SERVER_MISSION_SEASON_DAILY, "");
    }
    
    if(jsondoc.HasMember(KEY_SERVER_MISSION_DAILY)){
        str = MafAes256::Encrypt(jsondoc[KEY_SERVER_MISSION_DAILY].GetString());
        user->setStringForKey(KEY_SERVER_MISSION_DAILY, str);
    }else{
        user->setStringForKey(KEY_SERVER_MISSION_DAILY, "");
    }
    
    if(jsondoc.HasMember(KEY_SERVER_MISSION_WEEKLY)){
        str = MafAes256::Encrypt(jsondoc[KEY_SERVER_MISSION_WEEKLY].GetString());
        user->setStringForKey(KEY_SERVER_MISSION_WEEKLY, str);
    }else{
        user->setStringForKey(KEY_SERVER_MISSION_WEEKLY, "");
    }
    
    if(jsondoc.HasMember(KEY_INFINITE_ARTI)){
        str = MafAes256::Encrypt(jsondoc[KEY_INFINITE_ARTI].GetString());
        user->setStringForKey(KEY_INFINITE_ARTI, str);
    }else{
        user->setStringForKey(KEY_INFINITE_ARTI, "");
    }
    
    if(jsondoc.HasMember(KEY_ACHIEVE_NORMAL_COUNT)){
        str = MafAes256::Encrypt(jsondoc[KEY_ACHIEVE_NORMAL_COUNT].GetString());
        user->setStringForKey(KEY_ACHIEVE_NORMAL_COUNT, str);
    }else{
        user->setStringForKey(KEY_ACHIEVE_NORMAL_COUNT, "");
    }
    
    if(jsondoc.HasMember(KEY_ACHIEVE_NORMAL_COMPLETE)){
        str = MafAes256::Encrypt(jsondoc[KEY_ACHIEVE_NORMAL_COMPLETE].GetString());
        user->setStringForKey(KEY_ACHIEVE_NORMAL_COMPLETE, str);
    }else{
        user->setStringForKey(KEY_ACHIEVE_NORMAL_COMPLETE, "");
    }
    
    if(jsondoc.HasMember(KEY_TIMING_PACKAGE_FREE_COMPLETE)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIMING_PACKAGE_FREE_COMPLETE].GetString());
        user->setStringForKey(KEY_TIMING_PACKAGE_FREE_COMPLETE, str);
    }else{
        user->setStringForKey(KEY_TIMING_PACKAGE_FREE_COMPLETE, "");
    }
    
    if(jsondoc.HasMember(KEY_ACHIEVE_NORMAL_GRADE)){
        str = MafAes256::Encrypt(jsondoc[KEY_ACHIEVE_NORMAL_GRADE].GetString());
        user->setStringForKey(KEY_ACHIEVE_NORMAL_GRADE, str);
    }else{
        user->setStringForKey(KEY_ACHIEVE_NORMAL_GRADE, "");
    }
    
    if(jsondoc.HasMember(KEY_MINE_PICKAX)){
        str = MafAes256::Encrypt(jsondoc[KEY_MINE_PICKAX].GetString());
        user->setStringForKey(KEY_MINE_PICKAX, str);
    }else{
        user->setStringForKey(KEY_MINE_PICKAX, "");
    }
    
    if(jsondoc.HasMember(KEY_MINE_MINERAL)){
        str = MafAes256::Encrypt(jsondoc[KEY_MINE_MINERAL].GetString());
        user->setStringForKey(KEY_MINE_MINERAL, str);
    }else{
        user->setStringForKey(KEY_MINE_MINERAL, "");
    }
    
    if(jsondoc.HasMember(KEY_MINE_TIME)){
        str = MafAes256::Encrypt(jsondoc[KEY_MINE_TIME].GetString());
        user->setStringForKey(KEY_MINE_TIME, str);
    }else{
        user->setStringForKey(KEY_MINE_TIME, "");
    }
    
    if(jsondoc.HasMember(KEY_MINE_BUY_MINERAL)){
        str = MafAes256::Encrypt(jsondoc[KEY_MINE_BUY_MINERAL].GetString());
        user->setStringForKey(KEY_MINE_BUY_MINERAL, str);
    }else{
        user->setStringForKey(KEY_MINE_BUY_MINERAL, "");
    }
    
    if(jsondoc.HasMember(KEY_MINE_SPECIAL)){
        str = MafAes256::Encrypt(jsondoc[KEY_MINE_SPECIAL].GetString());
        user->setStringForKey(KEY_MINE_SPECIAL, str);
    }else{
        user->setStringForKey(KEY_MINE_SPECIAL, "");
    }
    
    if(jsondoc.HasMember(KEY_MINE_REFINE_OPEN)){
        str = MafAes256::Encrypt(jsondoc[KEY_MINE_REFINE_OPEN].GetString());
        user->setStringForKey(KEY_MINE_REFINE_OPEN, str);
    }else{
        user->setStringForKey(KEY_MINE_REFINE_OPEN, "");
    }
    
    if(jsondoc.HasMember(KEY_MINE_REFINE_BUY_RESETTIME)){
        str = MafAes256::Encrypt(jsondoc[KEY_MINE_REFINE_BUY_RESETTIME].GetString());
        user->setStringForKey(KEY_MINE_REFINE_BUY_RESETTIME, str);
    }else{
        user->setStringForKey(KEY_MINE_REFINE_BUY_RESETTIME, "");
    }
    
    if(jsondoc.HasMember(KEY_MINE_REFINE_BUY_COUNT)){
        str = MafAes256::Encrypt(jsondoc[KEY_MINE_REFINE_BUY_COUNT].GetString());
        user->setStringForKey(KEY_MINE_REFINE_BUY_COUNT, str);
    }else{
        user->setStringForKey(KEY_MINE_REFINE_BUY_COUNT, "");
    }
    
    if(jsondoc.HasMember(KEY_MINE_REFINE_BUY_ADS_RESETTIME)){
        str = MafAes256::Encrypt(jsondoc[KEY_MINE_REFINE_BUY_ADS_RESETTIME].GetString());
        user->setStringForKey(KEY_MINE_REFINE_BUY_ADS_RESETTIME, str);
    }else{
        // 나중에 들어가는 기능이라 초기화 하지 않음
    }
    
    if(jsondoc.HasMember(KEY_MINE_REFINE_BUY_ADS_COUNT)){
        str = MafAes256::Encrypt(jsondoc[KEY_MINE_REFINE_BUY_ADS_COUNT].GetString());
        user->setStringForKey(KEY_MINE_REFINE_BUY_ADS_COUNT, str);
    }else{
        // 나중에 들어가는 기능이라 초기화 하지 않음
    }
    
    if(jsondoc.HasMember(KEY_MINE_REFINE_PRODUCTION_COUNT)){
        str = MafAes256::Encrypt(jsondoc[KEY_MINE_REFINE_PRODUCTION_COUNT].GetString());
        user->setStringForKey(KEY_MINE_REFINE_PRODUCTION_COUNT, str);
    }else{
        user->setStringForKey(KEY_MINE_REFINE_PRODUCTION_COUNT, "");
    }
    
    if(jsondoc.HasMember(KEY_MINE_REFINE_PRODUCTION_STARTTIME)){
        str = MafAes256::Encrypt(jsondoc[KEY_MINE_REFINE_PRODUCTION_STARTTIME].GetString());
        user->setStringForKey(KEY_MINE_REFINE_PRODUCTION_STARTTIME, str);
    }else{
        user->setStringForKey(KEY_MINE_REFINE_PRODUCTION_STARTTIME, "");
    }
    
    if(jsondoc.HasMember(KEY_MINE_REFINE_PRODUCTION_ITEM)){
        str = MafAes256::Encrypt(jsondoc[KEY_MINE_REFINE_PRODUCTION_ITEM].GetString());
        user->setStringForKey(KEY_MINE_REFINE_PRODUCTION_ITEM, str);
    }else{
        user->setStringForKey(KEY_MINE_REFINE_PRODUCTION_ITEM, "");
    }
    
    if(jsondoc.HasMember(KEY_MINE_REFINE_UPGRADE_LEVEL)){
        str = MafAes256::Encrypt(jsondoc[KEY_MINE_REFINE_UPGRADE_LEVEL].GetString());
        user->setStringForKey(KEY_MINE_REFINE_UPGRADE_LEVEL, str);
    }else{
        user->setStringForKey(KEY_MINE_REFINE_UPGRADE_LEVEL, "");
    }
    
    if(jsondoc.HasMember(KEY_FLASHMAN_SCORE_BEST)){
        str = MafAes256::Encrypt(jsondoc[KEY_FLASHMAN_SCORE_BEST].GetString());
        user->setStringForKey(KEY_FLASHMAN_SCORE_BEST, str);
    }else{
        user->setStringForKey(KEY_FLASHMAN_SCORE_BEST, "");
    }
    
    if(jsondoc.HasMember(KEY_HITCOUNT_NORMAL)){
        str = MafAes256::Encrypt(jsondoc[KEY_HITCOUNT_NORMAL].GetString());
        user->setStringForKey(KEY_HITCOUNT_NORMAL, str);
    } else {
        user->setStringForKey(KEY_HITCOUNT_NORMAL, "");
    }
    
    
    if(jsondoc.HasMember(KEY_HITCOUNT_DUNGEON)){
        str = MafAes256::Encrypt(jsondoc[KEY_HITCOUNT_DUNGEON].GetString());
        user->setStringForKey(KEY_HITCOUNT_DUNGEON, str);
    } else {
        user->setStringForKey(KEY_HITCOUNT_DUNGEON, "");
    }
    
    if(jsondoc.HasMember(KEY_HITCOUNT_HONG_NORMAL)){
        str = MafAes256::Encrypt(jsondoc[KEY_HITCOUNT_HONG_NORMAL].GetString());
        user->setStringForKey(KEY_HITCOUNT_HONG_NORMAL, str);
    } else {
        user->setStringForKey(KEY_HITCOUNT_HONG_NORMAL, "");
    }
     
    if(jsondoc.HasMember(KEY_HITCOUNT_HONG_DUNGEON)){
        str = MafAes256::Encrypt(jsondoc[KEY_HITCOUNT_HONG_DUNGEON].GetString());
        user->setStringForKey(KEY_HITCOUNT_HONG_DUNGEON, str);
    } else {
        user->setStringForKey(KEY_HITCOUNT_HONG_DUNGEON, "");
    }
    
    if(jsondoc.HasMember(KEY_FLOOR)){
        str = MafAes256::Encrypt(jsondoc[KEY_FLOOR].GetString());        
        user->setStringForKey(KEY_FLOOR, str);
    } else {
        user->setStringForKey(KEY_FLOOR, "");
    }

    if(jsondoc.HasMember(KEY_FLOOR_COUNT)){
        str = MafAes256::Encrypt(jsondoc[KEY_FLOOR_COUNT].GetString());
        user->setStringForKey(KEY_FLOOR_COUNT, str);
    } else {
        user->setStringForKey(KEY_FLOOR_COUNT, "");
    }
    
    if(jsondoc.HasMember(KEY_EQUIP)){
        str = MafAes256::Encrypt(jsondoc[KEY_EQUIP].GetString());
        user->setStringForKey(KEY_EQUIP, str);
    } else {
        user->setStringForKey(KEY_EQUIP, "");
    }
    
    if(jsondoc.HasMember(KEY_EQUIP_VISUAL)){
        str = MafAes256::Encrypt(jsondoc[KEY_EQUIP_VISUAL].GetString());
        user->setStringForKey(KEY_EQUIP_VISUAL, str);
    } else {
        user->setStringForKey(KEY_EQUIP_VISUAL, "");
    }
    
    if(jsondoc.HasMember(KEY_EQUIP_HIGH)){
        str = MafAes256::Encrypt(jsondoc[KEY_EQUIP_HIGH].GetString());
        user->setStringForKey(KEY_EQUIP_HIGH, str);
    } else {
        user->setStringForKey(KEY_EQUIP_HIGH, "");
    }
    
    if(jsondoc.HasMember(KEY_EQUIP_REBORN)){
        str = MafAes256::Encrypt(jsondoc[KEY_EQUIP_REBORN].GetString());
        user->setStringForKey(KEY_EQUIP_REBORN, str);
    } else {
        user->setStringForKey(KEY_EQUIP_REBORN, "");
    }
    
    if(jsondoc.HasMember(KEY_EQUIP_PERMANENT)){
        str = MafAes256::Encrypt(jsondoc[KEY_EQUIP_PERMANENT].GetString());
        user->setStringForKey(KEY_EQUIP_PERMANENT, str);
    } else {
        str = MafAes256::Encrypt("1");
        user->setStringForKey(KEY_EQUIP_PERMANENT, str);
    }

    if(jsondoc.HasMember(KEY_EQUIP_LEVEL)){
        str = MafAes256::Encrypt(jsondoc[KEY_EQUIP_LEVEL].GetString());
        user->setStringForKey(KEY_EQUIP_LEVEL, str);
    }else{
        user->setStringForKey(KEY_EQUIP_LEVEL, "");
    }
    
    if(jsondoc.HasMember(KEY_EQUIP_REINFORCE)){
        str = MafAes256::Encrypt(jsondoc[KEY_EQUIP_REINFORCE].GetString());
        user->setStringForKey(KEY_EQUIP_REINFORCE, str);
    }else{
        user->setStringForKey(KEY_EQUIP_REINFORCE, "");
    }

    if(jsondoc.HasMember(KEY_ENCHANT_STONE)){
        str = MafAes256::Encrypt(jsondoc[KEY_ENCHANT_STONE].GetString());
        user->setStringForKey(KEY_ENCHANT_STONE, str);
    }else{
        user->setStringForKey(KEY_ENCHANT_STONE, "");
    }
    
    if(jsondoc.HasMember(KEY_ENCHANT_STONE_LOCAL)){
        str = MafAes256::Encrypt(jsondoc[KEY_ENCHANT_STONE_LOCAL].GetString());
        user->setStringForKey(KEY_ENCHANT_STONE_LOCAL, str);
    }else{
        user->setStringForKey(KEY_ENCHANT_STONE_LOCAL, "");
    }
    
    if(jsondoc.HasMember(KEY_ENCHANT_STONE_STORAGE)){
        str = MafAes256::Encrypt(jsondoc[KEY_ENCHANT_STONE_STORAGE].GetString());
        user->setStringForKey(KEY_ENCHANT_STONE_STORAGE, str);
    }else{
        user->setStringForKey(KEY_ENCHANT_STONE_STORAGE, "");
    }
    
    if(jsondoc.HasMember(KEY_MONSTER_SOUL)){
        str = MafAes256::Encrypt(jsondoc[KEY_MONSTER_SOUL].GetString());
        user->setStringForKey(KEY_MONSTER_SOUL, str);
    }else{
        user->setStringForKey(KEY_MONSTER_SOUL, "");
    }
    
    if(jsondoc.HasMember(KEY_MONSTER_SOUL_LEVEL)){
        str = MafAes256::Encrypt(jsondoc[KEY_MONSTER_SOUL_LEVEL].GetString());
        user->setStringForKey(KEY_MONSTER_SOUL_LEVEL, str);
    }else{
        user->setStringForKey(KEY_MONSTER_SOUL_LEVEL, "");
    }
    
    if(jsondoc.HasMember(KEY_ARTIFACT_LEVEL)){
        str = MafAes256::Encrypt(jsondoc[KEY_ARTIFACT_LEVEL].GetString());
        user->setStringForKey(KEY_ARTIFACT_LEVEL, str);
    }else{
        user->setStringForKey(KEY_ARTIFACT_LEVEL, "");
    }
    
    if(jsondoc.HasMember(KEY_ARTIFACT_REINFORCE)){
        str = MafAes256::Encrypt(jsondoc[KEY_ARTIFACT_REINFORCE].GetString());
        user->setStringForKey(KEY_ARTIFACT_REINFORCE, str);
    }else{
        user->setStringForKey(KEY_ARTIFACT_REINFORCE, "");
    }
    
    if(jsondoc.HasMember(KEY_RELIC_LEVEL)){
        str = MafAes256::Encrypt(jsondoc[KEY_RELIC_LEVEL].GetString());
        user->setStringForKey(KEY_RELIC_LEVEL, str);
    }else{
        user->setStringForKey(KEY_RELIC_LEVEL, "");
    }
    
    if(jsondoc.HasMember(KEY_JELLY_NEW)){
        str = MafAes256::Encrypt(jsondoc[KEY_JELLY_NEW].GetString());
        user->setStringForKey(KEY_JELLY_NEW, str);
    }else{
        user->setStringForKey(KEY_JELLY_NEW, "");
    }
    
    if(jsondoc.HasMember(KEY_HONOR)){
        str = MafAes256::Encrypt(jsondoc[KEY_HONOR].GetString());
        user->setStringForKey(KEY_HONOR, str);
    }else{
        user->setStringForKey(KEY_HONOR, "");
    }
    
    if(jsondoc.HasMember(KEY_PET_LEVEL)){
        str = MafAes256::Encrypt(jsondoc[KEY_PET_LEVEL].GetString());
        user->setStringForKey(KEY_PET_LEVEL, str);
    }else{
        user->setStringForKey(KEY_PET_LEVEL, "");
    }
    
    if(jsondoc.HasMember(KEY_PET_REINFORCE_LEVEL)){
        str = MafAes256::Encrypt(jsondoc[KEY_PET_REINFORCE_LEVEL].GetString());
        user->setStringForKey(KEY_PET_REINFORCE_LEVEL, str);
    }else{
        user->setStringForKey(KEY_PET_REINFORCE_LEVEL, "");
    }
    
    if(jsondoc.HasMember(KEY_PET_EVOLUTION)){
        str = MafAes256::Encrypt(jsondoc[KEY_PET_EVOLUTION].GetString());
        user->setStringForKey(KEY_PET_EVOLUTION, str);
    } else {
        user->setStringForKey(KEY_PET_EVOLUTION, "");
    }
    
    if(jsondoc.HasMember(KEY_PET_EVOLUTION_REWARD)){
        str = MafAes256::Encrypt(jsondoc[KEY_PET_EVOLUTION_REWARD].GetString());
        user->setStringForKey(KEY_PET_EVOLUTION_REWARD, str);
    } else {
        user->setStringForKey(KEY_PET_EVOLUTION_REWARD, "");
    }
    
    if(jsondoc.HasMember(KEY_PET_SKIN)){
        str = MafAes256::Encrypt(jsondoc[KEY_PET_SKIN].GetString());
        user->setStringForKey(KEY_PET_SKIN, str);
    } else {
        user->setStringForKey(KEY_PET_SKIN, "");
    }
    
    if(jsondoc.HasMember(KEY_PET_SKIN_BUY)){
        str = MafAes256::Encrypt(jsondoc[KEY_PET_SKIN_BUY].GetString());
        user->setStringForKey(KEY_PET_SKIN_BUY, str);
    } else {
        user->setStringForKey(KEY_PET_SKIN_BUY, "");
    }
    
    // 펫(바포메트)
    if(jsondoc.HasMember(KEY_PET_BAFOMETZ)){
        str = MafAes256::Encrypt(jsondoc[KEY_PET_BAFOMETZ].GetString());
        user->setStringForKey(KEY_PET_BAFOMETZ, str);
    } else {
        user->setStringForKey(KEY_PET_BAFOMETZ, "");
    }
    
    if(jsondoc.HasMember(KEY_QUEST_LEVEL)){
        str = MafAes256::Encrypt(jsondoc[KEY_QUEST_LEVEL].GetString());
        user->setStringForKey(KEY_QUEST_LEVEL, str);
    } else {
        user->setStringForKey(KEY_QUEST_LEVEL, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_ATTACK_9_BONUS)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ATTACK_9_BONUS].GetString());
        user->setStringForKey(KEY_TIME_ATTACK_9_BONUS, str);
    } else {
        user->setStringForKey(KEY_TIME_ATTACK_9_BONUS, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_ATTACK_10_BONUS)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ATTACK_10_BONUS].GetString());
        user->setStringForKey(KEY_TIME_ATTACK_10_BONUS, str);
    } else {
        user->setStringForKey(KEY_TIME_ATTACK_10_BONUS, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_ATTACK_BONUS)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ATTACK_BONUS].GetString());
        user->setStringForKey(KEY_TIME_ATTACK_BONUS, str);
    }

    if(jsondoc.HasMember(KEY_TIME_COIN_BONUS)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_COIN_BONUS].GetString());
        user->setStringForKey(KEY_TIME_COIN_BONUS, str);
    } else {
        user->setStringForKey(KEY_TIME_COIN_BONUS, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_MOVE_BONUS)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_MOVE_BONUS].GetString());
        user->setStringForKey(KEY_TIME_MOVE_BONUS, str);
    } else {
        user->setStringForKey(KEY_TIME_MOVE_BONUS, "");
    }

    if(jsondoc.HasMember(KEY_COUNT_KEY_BONUS)){
        str = MafAes256::Encrypt(jsondoc[KEY_COUNT_KEY_BONUS].GetString());
        user->setStringForKey(KEY_COUNT_KEY_BONUS, str);
    } else {
        user->setStringForKey(KEY_COUNT_KEY_BONUS, str);
    }
    
    if(jsondoc.HasMember(KEY_COUNT_HERO_BONUS)){
        str = MafAes256::Encrypt(jsondoc[KEY_COUNT_HERO_BONUS].GetString());
        user->setStringForKey(KEY_COUNT_HERO_BONUS, str);
    }else{
        str = MafAes256::Encrypt("0");
        user->setStringForKey(KEY_COUNT_HERO_BONUS, str);
    }
    
    if(jsondoc.HasMember(KEY_TIME_ADS_GOLD_BONUS)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ADS_GOLD_BONUS].GetString());
        user->setStringForKey(KEY_TIME_ADS_GOLD_BONUS, str);
    } else {
        user->setStringForKey(KEY_TIME_ADS_GOLD_BONUS, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_ADS_GEM_BONUS)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ADS_GEM_BONUS].GetString());
        user->setStringForKey(KEY_TIME_ADS_GEM_BONUS, str);
    } else {
        user->setStringForKey(KEY_TIME_ADS_GEM_BONUS, "");
    }

    if(jsondoc.HasMember(KEY_TIME_ADS_BUFF_BONUS)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ADS_BUFF_BONUS].GetString());
        user->setStringForKey(KEY_TIME_ADS_BUFF_BONUS, str);
    } else {
        user->setStringForKey(KEY_TIME_ADS_BUFF_BONUS, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_ADS_MINE_BONUS)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ADS_MINE_BONUS].GetString());
        user->setStringForKey(KEY_TIME_ADS_MINE_BONUS, str);
    } else {
        user->setStringForKey(KEY_TIME_ADS_MINE_BONUS, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_ADS_SELECT_BONUS)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ADS_SELECT_BONUS].GetString());
        user->setStringForKey(KEY_TIME_ADS_SELECT_BONUS, str);
    } else {
        user->setStringForKey(KEY_TIME_ADS_SELECT_BONUS, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_ADS_BUFF_ATK)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ADS_BUFF_ATK].GetString());
        user->setStringForKey(KEY_TIME_ADS_BUFF_ATK, str);
    } else {
        user->setStringForKey(KEY_TIME_ADS_BUFF_ATK, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_ADS_BUFF_GOLD)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ADS_BUFF_GOLD].GetString());
        user->setStringForKey(KEY_TIME_ADS_BUFF_GOLD, str);
    } else {
        user->setStringForKey(KEY_TIME_ADS_BUFF_GOLD, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_ADS_BUFF_MOVE)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ADS_BUFF_MOVE].GetString());
        user->setStringForKey(KEY_TIME_ADS_BUFF_MOVE, str);
    } else {
        user->setStringForKey(KEY_TIME_ADS_BUFF_MOVE, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_ADS_BUFF_D_ATK)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ADS_BUFF_D_ATK].GetString());
        user->setStringForKey(KEY_TIME_ADS_BUFF_D_ATK, str);
    } else {
        user->setStringForKey(KEY_TIME_ADS_BUFF_D_ATK, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_ADS_BUFF_D_ASPD)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ADS_BUFF_D_ASPD].GetString());
        user->setStringForKey(KEY_TIME_ADS_BUFF_D_ASPD, str);
    } else {
        user->setStringForKey(KEY_TIME_ADS_BUFF_D_ASPD, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_ADS_BUFF_D_GOLD)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ADS_BUFF_D_GOLD].GetString());
        user->setStringForKey(KEY_TIME_ADS_BUFF_D_GOLD, str);
    } else {
        user->setStringForKey(KEY_TIME_ADS_BUFF_D_GOLD, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_ADS_BUFF_D_MOVE)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ADS_BUFF_D_MOVE].GetString());
        user->setStringForKey(KEY_TIME_ADS_BUFF_D_MOVE, str);
    } else {
        user->setStringForKey(KEY_TIME_ADS_BUFF_D_MOVE, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_ADS_BUFF_GOLD_NTERSTITIAL)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ADS_BUFF_GOLD_NTERSTITIAL].GetString());
        user->setStringForKey(KEY_TIME_ADS_BUFF_GOLD_NTERSTITIAL, str);
    } else {
        user->setStringForKey(KEY_TIME_ADS_BUFF_GOLD_NTERSTITIAL, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_ITEM_FEVER)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ITEM_FEVER].GetString());
        user->setStringForKey(KEY_TIME_ITEM_FEVER, str);
    } else {
        user->setStringForKey(KEY_TIME_ITEM_FEVER, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_ITEM_FEVER_COUNT)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ITEM_FEVER_COUNT].GetString());
        user->setStringForKey(KEY_TIME_ITEM_FEVER_COUNT, str);
    } else {
        user->setStringForKey(KEY_TIME_ITEM_FEVER_COUNT, "");
    }

    if(jsondoc.HasMember(DATA_HIGH_FLOOR)){
        str = MafAes256::Encrypt(jsondoc[DATA_HIGH_FLOOR].GetString());
        user->setStringForKey(DATA_HIGH_FLOOR, str);
    } else {
        user->setStringForKey(DATA_HIGH_FLOOR, "");
    }

    if(jsondoc.HasMember(DATA_TOTAL_FLOOR)){
        str = MafAes256::Encrypt(jsondoc[DATA_TOTAL_FLOOR].GetString());
        user->setStringForKey(DATA_TOTAL_FLOOR, str);
    } else {
        user->setStringForKey(DATA_TOTAL_FLOOR, "");
    }

    if(jsondoc.HasMember(DATA_REVIVAL)){
        str = MafAes256::Encrypt(jsondoc[DATA_REVIVAL].GetString());
        user->setStringForKey(DATA_REVIVAL, str);
    } else {
        user->setStringForKey(DATA_REVIVAL, "");
    }
    
    if(jsondoc.HasMember(DATA_REVIVAL_ADS)){
        str = MafAes256::Encrypt(jsondoc[DATA_REVIVAL_ADS].GetString());
        user->setStringForKey(DATA_REVIVAL_ADS, str);
    } else {
        user->setStringForKey(DATA_REVIVAL_ADS, "");
    }
    
    if(jsondoc.HasMember(KEY_LOGIN_GAMECENTER_ID)){
        str = MafAes256::Encrypt(jsondoc[KEY_LOGIN_GAMECENTER_ID].GetString());
        user->setStringForKey(KEY_LOGIN_GAMECENTER_ID, str);
    } else {
        user->setStringForKey(KEY_LOGIN_GAMECENTER_ID, "");
    }
    
    if(jsondoc.HasMember(KEY_LOGIN_GOOGLEPLAY_ID)){
        str = MafAes256::Encrypt(jsondoc[KEY_LOGIN_GOOGLEPLAY_ID].GetString());
        user->setStringForKey(KEY_LOGIN_GOOGLEPLAY_ID, str);
    } else {
        user->setStringForKey(KEY_LOGIN_GOOGLEPLAY_ID, "");
    }
    
    if(jsondoc.HasMember(KEY_LOGIN_GUEST)){
        str = MafAes256::Encrypt(jsondoc[KEY_LOGIN_GUEST].GetString());
        user->setStringForKey(KEY_LOGIN_GUEST, str);
    } else {
        user->setStringForKey(KEY_LOGIN_GUEST, "");
    }
    
    if(jsondoc.HasMember(KEY_DRAGON_SKILLCHECK)){
        str = MafAes256::Encrypt(jsondoc[KEY_DRAGON_SKILLCHECK].GetString());
        user->setStringForKey(KEY_DRAGON_SKILLCHECK, str);
    } else {
        user->setStringForKey(KEY_DRAGON_SKILLCHECK, "");
    }
    
    if(jsondoc.HasMember(KEY_REVIVAL_ATK_BUFF)){
        str = MafAes256::Encrypt(jsondoc[KEY_REVIVAL_ATK_BUFF].GetString());
        user->setStringForKey(KEY_REVIVAL_ATK_BUFF, str);
    } else {
        user->setStringForKey(KEY_REVIVAL_ATK_BUFF, "");
    }
    
    if(jsondoc.HasMember(KEY_REVIVAL_QUEST_BUFF)){
        str = MafAes256::Encrypt(jsondoc[KEY_REVIVAL_QUEST_BUFF].GetString());
        user->setStringForKey(KEY_REVIVAL_QUEST_BUFF, str);
    } else {
        user->setStringForKey(KEY_REVIVAL_QUEST_BUFF, "");
    }
    
    if(jsondoc.HasMember(KEY_REVIVAL_KEY_BUFF1)){
        str = MafAes256::Encrypt(jsondoc[KEY_REVIVAL_KEY_BUFF1].GetString());
        user->setStringForKey(KEY_REVIVAL_KEY_BUFF1, str);
    }else{
        user->setStringForKey(KEY_REVIVAL_KEY_BUFF1, "");
    }
    
    if(jsondoc.HasMember(KEY_REVIVAL_KEY_BUFF2)){
        str = MafAes256::Encrypt(jsondoc[KEY_REVIVAL_KEY_BUFF2].GetString());
        user->setStringForKey(KEY_REVIVAL_KEY_BUFF2, str);
    }else{
        user->setStringForKey(KEY_REVIVAL_KEY_BUFF2, "");
    }
    
    if(jsondoc.HasMember(KEY_E_REVIVAL_ATK_BUFF)){
        str = MafAes256::Encrypt(jsondoc[KEY_E_REVIVAL_ATK_BUFF].GetString());
        user->setStringForKey(KEY_E_REVIVAL_ATK_BUFF, str);
    } else {
        user->setStringForKey(KEY_E_REVIVAL_ATK_BUFF, "");
    }
    
    if(jsondoc.HasMember(KEY_HIT_BUFF)){
        str = MafAes256::Encrypt(jsondoc[KEY_HIT_BUFF].GetString());
        user->setStringForKey(KEY_HIT_BUFF, str);
    } else {
        user->setStringForKey(KEY_HIT_BUFF, "");
    }
    
    if(jsondoc.HasMember(KEY_DRAGON_BUFF)){
        str = MafAes256::Encrypt(jsondoc[KEY_DRAGON_BUFF].GetString());
        user->setStringForKey(KEY_DRAGON_BUFF, str);
    } else {
        user->setStringForKey(KEY_DRAGON_BUFF, "");
    }
    
    if(jsondoc.HasMember(KEY_HELL_BUFF)){
        str = MafAes256::Encrypt(jsondoc[KEY_HELL_BUFF].GetString());
        user->setStringForKey(KEY_HELL_BUFF, str);
    } else {
        user->setStringForKey(KEY_HELL_BUFF, "");
    }
    
    if(jsondoc.HasMember(KEY_QUEST_25_BUFF)){
        str = MafAes256::Encrypt(jsondoc[KEY_QUEST_25_BUFF].GetString());
        user->setStringForKey(KEY_QUEST_25_BUFF, str);
    } else {
        user->setStringForKey(KEY_QUEST_25_BUFF, "");
    }
    
    if(jsondoc.HasMember(KEY_REVIVE_SPECIAL)){
        str = MafAes256::Encrypt(jsondoc[KEY_REVIVE_SPECIAL].GetString());
        user->setStringForKey(KEY_REVIVE_SPECIAL, str);
    } else {
        user->setStringForKey(KEY_REVIVE_SPECIAL, "");
    }
    
    if(jsondoc.HasMember(KEY_REVIVE_KEYBOOSTER)){
        str = MafAes256::Encrypt(jsondoc[KEY_REVIVE_KEYBOOSTER].GetString());
        user->setStringForKey(KEY_REVIVE_KEYBOOSTER, str);
    } else {
        user->setStringForKey(KEY_REVIVE_KEYBOOSTER, "");
    }
    
    if(jsondoc.HasMember(KEY_SPECIAL_CLEAR)){
        str = MafAes256::Encrypt(jsondoc[KEY_SPECIAL_CLEAR].GetString());
        user->setStringForKey(KEY_SPECIAL_CLEAR, str);
    } else {
        user->setStringForKey(KEY_SPECIAL_CLEAR, "");
    }
    
    if(jsondoc.HasMember(KEY_SPECIAL_COMPLETE)){
        str = MafAes256::Encrypt(jsondoc[KEY_SPECIAL_COMPLETE].GetString());
        user->setStringForKey(KEY_SPECIAL_COMPLETE, str);
    } else {
        user->setStringForKey(KEY_SPECIAL_COMPLETE, "");
    }
    
    if(jsondoc.HasMember(KEY_ATTEND_VIP)){
        str = MafAes256::Encrypt(jsondoc[KEY_ATTEND_VIP].GetString());
        user->setStringForKey(KEY_ATTEND_VIP, str);
    }else{
        user->setStringForKey(KEY_ATTEND_VIP, "");
    }
    
    if(jsondoc.HasMember(KEY_ATTEND_DAY)){
        str = MafAes256::Encrypt(jsondoc[KEY_ATTEND_DAY].GetString());
        user->setStringForKey(KEY_ATTEND_DAY, str);
    }else{
        user->setStringForKey(KEY_ATTEND_DAY, "");
    }
    
    if(jsondoc.HasMember(KEY_ATTEND_ADS)){
        str = MafAes256::Encrypt(jsondoc[KEY_ATTEND_ADS].GetString());
        user->setStringForKey(KEY_ATTEND_ADS, str);
    }else{
        user->setStringForKey(KEY_ATTEND_ADS, "");
    }
    
    if(jsondoc.HasMember(KEY_MYHOME_LEVEL)){
        str = MafAes256::Encrypt(jsondoc[KEY_MYHOME_LEVEL].GetString());
        user->setStringForKey(KEY_MYHOME_LEVEL, str);
    }else{
        user->setStringForKey(KEY_MYHOME_LEVEL, "");
    }
    
    if(jsondoc.HasMember(KEY_MYHOME_WATCHCOUNT)){
        str = MafAes256::Encrypt(jsondoc[KEY_MYHOME_WATCHCOUNT].GetString());
        user->setStringForKey(KEY_MYHOME_WATCHCOUNT, str);
    }else{
        user->setStringForKey(KEY_MYHOME_WATCHCOUNT, "");
    }
    
    if(jsondoc.HasMember(KEY_MYHOME_COUNT)){
        str = MafAes256::Encrypt(jsondoc[KEY_MYHOME_COUNT].GetString());
        user->setStringForKey(KEY_MYHOME_COUNT, str);
    }else{
        user->setStringForKey(KEY_MYHOME_COUNT, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_ADS_MYHOME_BUFF)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ADS_MYHOME_BUFF].GetString());
        user->setStringForKey(KEY_TIME_ADS_MYHOME_BUFF, str);
    }else{
        user->setStringForKey(KEY_TIME_ADS_MYHOME_BUFF, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_ADS_MYHOME_GEM)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ADS_MYHOME_GEM].GetString());
        user->setStringForKey(KEY_TIME_ADS_MYHOME_GEM, str);
    }else{
        user->setStringForKey(KEY_TIME_ADS_MYHOME_GEM, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_ADS_MYHOME_GOLD)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ADS_MYHOME_GOLD].GetString());
        user->setStringForKey(KEY_TIME_ADS_MYHOME_GOLD, str);
    }else{
        user->setStringForKey(KEY_TIME_ADS_MYHOME_GOLD, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_ADS_MYHOME_CUBE)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ADS_MYHOME_CUBE].GetString());
        user->setStringForKey(KEY_TIME_ADS_MYHOME_CUBE, str);
    }else{
        user->setStringForKey(KEY_TIME_ADS_MYHOME_CUBE, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_ADS_MYHOME_COSTUMECOIN)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ADS_MYHOME_COSTUMECOIN].GetString());
        user->setStringForKey(KEY_TIME_ADS_MYHOME_COSTUMECOIN, str);
    }else{
        user->setStringForKey(KEY_TIME_ADS_MYHOME_COSTUMECOIN, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_ADS_MYHOME_MINE)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ADS_MYHOME_MINE].GetString());
        user->setStringForKey(KEY_TIME_ADS_MYHOME_MINE, str);
    }else{
        user->setStringForKey(KEY_TIME_ADS_MYHOME_MINE, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_ADS_MYHOME_BONTRIUM)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_ADS_MYHOME_BONTRIUM].GetString());
        user->setStringForKey(KEY_TIME_ADS_MYHOME_BONTRIUM, str);
    }else{
        user->setStringForKey(KEY_TIME_ADS_MYHOME_BONTRIUM, "");
    }

    if(jsondoc.HasMember(KEY_MONSTER_HP)) {
        user->setStringForKey(KEY_MONSTER_HP, jsondoc[KEY_MONSTER_HP].GetString());
    } else {
        user->setStringForKey(KEY_MONSTER_HP, "");
    }
    
    if(jsondoc.HasMember(KEY_R_S_MAX_LV)){
        str = MafAes256::Encrypt(jsondoc[KEY_R_S_MAX_LV].GetString());
        user->setStringForKey(KEY_R_S_MAX_LV, str);
    } else {
        user->setStringForKey(KEY_R_S_MAX_LV, "");
    }
     
    if(jsondoc.HasMember(KEY_R_S_MAX_DAMAGE)){
        str = MafAes256::Encrypt(jsondoc[KEY_R_S_MAX_DAMAGE].GetString());
        user->setStringForKey(KEY_R_S_MAX_DAMAGE, str);
    } else {
        user->setStringForKey(KEY_R_S_MAX_DAMAGE, "");
    }
    
    // 보물 결제
    for(int i=E_ARTIFACT::ARTI_1_EVOL1; i< E_ARTIFACT::E_ARTIFACT_999_NONE; i++)
    {
        std::string str = "";
        
        str = MafUtils::format("pack_%d",i).c_str();
        if(jsondoc.HasMember(str.c_str()))
            user->setIntegerForKey(str.c_str(), jsondoc[str.c_str()].GetInt());
        else
            user->setIntegerForKey(str.c_str(), 0);
        
        str = MafUtils::format("pack_e_%d",i).c_str();
        if(jsondoc.HasMember(str.c_str()))
            user->setIntegerForKey(str.c_str(), jsondoc[str.c_str()].GetInt());
        else
            user->setIntegerForKey(str.c_str(), 0);
    }
    
    //무한 보물 결제
    for(int i=E_ARTIFACT::ARTI_1_EVOL1; i< E_ARTIFACT::E_ARTIFACT_999_NONE; i++)
    {
        std::string str = "";
        for(int j = 1; j <= INFINITE_ARTIFACT_KIND; ++j)
        {
            str = MafUtils::format("Infinite_pack_%d_%d",i, j).c_str();
            if(jsondoc.HasMember(str.c_str()))
                user->setIntegerForKey(str.c_str(), jsondoc[str.c_str()].GetInt());
            else
                user->setIntegerForKey(str.c_str(), 0);
        }
    }
    
    // 사용자 결제 총 건수
    if(jsondoc.HasMember(KEY_PURCHASE_COUNT))
        CashManager::getInstance()->setHistoryTotal(jsondoc[KEY_PURCHASE_COUNT].GetInt());
    else
        CashManager::getInstance()->setHistoryTotal(0);
    
    // 사용자 결제 개별 건수
    if(jsondoc.HasMember(KEY_PURCHASE_HISTORY))
        CashManager::getInstance()->setHistoryProductCloud(jsondoc[KEY_PURCHASE_HISTORY].GetString());
    else
        CashManager::getInstance()->setHistoryProductCloud("");
    
    // 사용자 결제 이벤트 성 건수(초보자, 중급자)
    if(jsondoc.HasMember(KEY_PURCHASE_ADS))
        CashManager::getInstance()->setHistoryAdsCloud(jsondoc[KEY_PURCHASE_ADS].GetString());
    else
        CashManager::getInstance()->setHistoryAdsCloud("");
    
    // 만능 포인트 보관함
    if(jsondoc.HasMember(KEY_REVIVE_POINT)){
        str = MafAes256::Encrypt(jsondoc[KEY_REVIVE_POINT].GetString());
        user->setStringForKey(KEY_REVIVE_POINT, str);
    }
    else
    {
        str = MafAes256::Encrypt("0");
        user->setStringForKey(KEY_REVIVE_POINT, str);
    }
    
    // 피버 하루 사용 수
    if(jsondoc.HasMember(KEY_REVIVE_FEVER_DAY)){
        str = MafAes256::Encrypt(jsondoc[KEY_REVIVE_FEVER_DAY].GetString());
        user->setStringForKey(KEY_REVIVE_FEVER_DAY, str);
    }
    else
    {
        str = MafAes256::Encrypt(MafUtils::toString(C_MAX_FEVER_DAY));
        user->setStringForKey(KEY_REVIVE_FEVER_DAY, str);
    }
    
    // 큐브 하루 획득 수
    if(jsondoc.HasMember(KEY_REVIVE_CUBE_DAY)){
        str = MafAes256::Encrypt(jsondoc[KEY_REVIVE_CUBE_DAY].GetString());
        user->setStringForKey(KEY_REVIVE_CUBE_DAY, str);
    }
    else
    {
        str = MafAes256::Encrypt(MafUtils::toString(C_MAX_CUBE_DAY));
        user->setStringForKey(KEY_REVIVE_CUBE_DAY, str);
    }
    
    // 환생 5배 버프
    if(jsondoc.HasMember(KEY_REVIVE_BUFF)){
        str = MafAes256::Encrypt(jsondoc[KEY_REVIVE_BUFF].GetString());
        user->setStringForKey(KEY_REVIVE_BUFF, str);
    }
    else
    {
        user->setStringForKey(KEY_REVIVE_BUFF, "");
    }
    
    str.clear();
    if(jsondoc.HasMember(KEY_COMIC_SHOW)){
        str = jsondoc[KEY_COMIC_SHOW].GetString();
    }
    if(!str.empty()){
        StoryManager::getInstance()->setCloudStory(str);
    }
    
    str.clear();
    if(jsondoc.HasMember(KEY_DIALOG_SHOW)){
        str = jsondoc[KEY_DIALOG_SHOW].GetString();
    }
    if(!str.empty()){
        DialogManager::getInstance()->setCloud(str);
    }
    
    str.clear();
    if(jsondoc.HasMember(KEY_TODAY)){
        str = jsondoc[KEY_TODAY].GetString();
        if(user->getStringForKey(KEY_TODAY) == "")
        {
            user->setStringForKey(KEY_TODAY, str);
        }
    }
    
    str.clear();
    if(jsondoc.HasMember(KEY_HISTORY_DATA)){
        str = jsondoc[KEY_HISTORY_DATA].GetString();
        setHistorySaveFromServerData(str);
    }
    
    E_PLACE loadPlace = E_PLACE::DG_NORMAL;
    if(jsondoc.HasMember(KEY_PLACE)){
        loadPlace = (E_PLACE)jsondoc[KEY_PLACE].GetInt();
        
        user->setIntegerForKey(KEY_PLACE, (int)loadPlace);
    }
    
    /////시작
    if(jsondoc.HasMember(KEY_D_GOLD)){
        str = MafAes256::Encrypt(jsondoc[KEY_D_GOLD].GetString());
        user->setStringForKey(KEY_D_GOLD, str);
    }else{
        user->setStringForKey(KEY_D_GOLD, "");
    }
    
    if(jsondoc.HasMember(KEY_D_KEY)){
        str = MafAes256::Encrypt(jsondoc[KEY_D_KEY].GetString());
        user->setStringForKey(KEY_D_KEY, str);
    }else{
        user->setStringForKey(KEY_D_KEY, "");
    }

    if(jsondoc.HasMember(KEY_D_QUEST_LEVEL)){
        str = MafAes256::Encrypt(jsondoc[KEY_D_QUEST_LEVEL].GetString());
        user->setStringForKey(KEY_D_QUEST_LEVEL, str);
    }else{
        user->setStringForKey(KEY_D_QUEST_LEVEL, "");
    }
    
    if(jsondoc.HasMember(KEY_D_QUEST_TIME)){
        str = MafAes256::Encrypt(jsondoc[KEY_D_QUEST_TIME].GetString());
        user->setStringForKey(KEY_D_QUEST_TIME, str);
    }else{
        user->setStringForKey(KEY_D_QUEST_TIME, "");
    }
    
    if(jsondoc.HasMember(KEY_D_EQUIP)){
        str = MafAes256::Encrypt(jsondoc[KEY_D_EQUIP].GetString());
        user->setStringForKey(KEY_D_EQUIP, str);
    }else{
        user->setStringForKey(KEY_D_EQUIP, "");
    }
    
    if(jsondoc.HasMember(KEY_D_EQUIP_LEVEL)){
        str = MafAes256::Encrypt(jsondoc[KEY_D_EQUIP_LEVEL].GetString());
        user->setStringForKey(KEY_D_EQUIP_LEVEL, str);
    }else{
        user->setStringForKey(KEY_D_EQUIP_LEVEL, "");
    }
    
    if(jsondoc.HasMember(KEY_D_EQUIP_REINFORCE)){
        str = MafAes256::Encrypt(jsondoc[KEY_D_EQUIP_REINFORCE].GetString());
        user->setStringForKey(KEY_D_EQUIP_REINFORCE, str);
    }else{
        user->setStringForKey(KEY_D_EQUIP_REINFORCE, "");
    }
    
    if(jsondoc.HasMember(KEY_D_PET_LEVEL)){
        str = MafAes256::Encrypt(jsondoc[KEY_D_PET_LEVEL].GetString());
        user->setStringForKey(KEY_D_PET_LEVEL, str);
    }else{
        user->setStringForKey(KEY_D_PET_LEVEL, "");
    }
    
    if(jsondoc.HasMember(KEY_D_PET_GRADE)){
        str = MafAes256::Encrypt(jsondoc[KEY_D_PET_GRADE].GetString());
        user->setStringForKey(KEY_D_PET_GRADE, str);
    }else{
        user->setStringForKey(KEY_D_PET_GRADE, "");
    }
    
    if(jsondoc.HasMember(KEY_D_ARTIFACT)){
        str = MafAes256::Encrypt(jsondoc[KEY_D_ARTIFACT].GetString());
        user->setStringForKey(KEY_D_ARTIFACT, str);
    }else{
        user->setStringForKey(KEY_D_ARTIFACT, "");
    }
    
    if(jsondoc.HasMember(KEY_D_TIME_BUFF)){
        str = MafAes256::Encrypt(jsondoc[KEY_D_TIME_BUFF].GetString());
        user->setStringForKey(KEY_D_TIME_BUFF, str);
    }else{
        user->setStringForKey(KEY_D_TIME_BUFF, "");
    }
    
    if(jsondoc.HasMember(KEY_D_TIME_ADS)){
        str = MafAes256::Encrypt(jsondoc[KEY_D_TIME_ADS].GetString());
        user->setStringForKey(KEY_D_TIME_ADS, str);
    }else{
        user->setStringForKey(KEY_D_TIME_ADS, "");
    }
    
    if(jsondoc.HasMember(KEY_D_HIT_COUNT_HEDGEHOG)){
        str = MafAes256::Encrypt(jsondoc[KEY_D_HIT_COUNT_HEDGEHOG].GetString());
        user->setStringForKey(KEY_D_HIT_COUNT_HEDGEHOG, str);
    }else{
        user->setStringForKey(KEY_D_HIT_COUNT_HEDGEHOG, "");
    }
    
    if(jsondoc.HasMember(KEY_D_HIT_COUNT_FLASH)){
        str = MafAes256::Encrypt(jsondoc[KEY_D_HIT_COUNT_FLASH].GetString());
        user->setStringForKey(KEY_D_HIT_COUNT_FLASH, str);
    }else{
        user->setStringForKey(KEY_D_HIT_COUNT_FLASH, "");
    }

    //끝
    if(jsondoc.HasMember(DATA_D_HIGH_FLOOR)){
        str = MafAes256::Encrypt(jsondoc[DATA_D_HIGH_FLOOR].GetString());
        user->setStringForKey(DATA_D_HIGH_FLOOR, str);
    }else{
        user->setStringForKey(DATA_D_HIGH_FLOOR, "");
    }
    
    if(jsondoc.HasMember(DATA_D_TOTAL_FLOOR)){
        str = MafAes256::Encrypt(jsondoc[DATA_D_TOTAL_FLOOR].GetString());
        user->setStringForKey(DATA_D_TOTAL_FLOOR, str);
    }else{
        user->setStringForKey(DATA_D_TOTAL_FLOOR, "");
    }
    
    if(jsondoc.HasMember(KEY_D_FLOOR)){
        str = MafAes256::Encrypt(jsondoc[KEY_D_FLOOR].GetString());
        user->setStringForKey(KEY_D_FLOOR, str);
    }else{
        user->setStringForKey(KEY_D_FLOOR, "");
    }
    
    if(jsondoc.HasMember(KEY_D_FLOOR_COUNT)){
        str = MafAes256::Encrypt(jsondoc[KEY_D_FLOOR_COUNT].GetString());
        user->setStringForKey(KEY_D_FLOOR_COUNT, str);
    }else{
        user->setStringForKey(KEY_D_FLOOR_COUNT, "");
    }
    
    if(jsondoc.HasMember(KEY_D_REVIVAL)){
        str = MafAes256::Encrypt(jsondoc[KEY_D_REVIVAL].GetString());
        user->setStringForKey(KEY_D_REVIVAL, str);
    }else{
        user->setStringForKey(KEY_D_REVIVAL, ""); 
    }
    
    if(jsondoc.HasMember(KEY_D_MONSTER_HP)){
        str = jsondoc[KEY_D_MONSTER_HP].GetString();
        user->setStringForKey(KEY_D_MONSTER_HP, str);
    }else{
        user->setStringForKey(KEY_D_MONSTER_HP, "");
    }
    
    
    if(jsondoc.HasMember(KEY_SAVE_TIME)){
        str = jsondoc[KEY_SAVE_TIME].GetString();
        user->setStringForKey(KEY_SAVE_TIME, str);
    }else{
        user->setStringForKey(KEY_SAVE_TIME, "0");
    }
    
    if(jsondoc.HasMember(KEY_TUTORIAL_PROGRESS)){
        str = MafAes256::Encrypt(jsondoc[KEY_TUTORIAL_PROGRESS].GetString());
        user->setStringForKey(KEY_TUTORIAL_PROGRESS, str);
    }else{
        user->setStringForKey(KEY_TUTORIAL_PROGRESS, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_TRAVEL_LEVEL)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_TRAVEL_LEVEL].GetString());
        user->setStringForKey(KEY_TIME_TRAVEL_LEVEL, str);
    }else{
        user->setStringForKey(KEY_TIME_TRAVEL_LEVEL, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_TRAVEL_COUNT)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_TRAVEL_COUNT].GetString());
        user->setStringForKey(KEY_TIME_TRAVEL_COUNT, str);
    }else{
        user->setStringForKey(KEY_TIME_TRAVEL_COUNT, "");
    }
    
    if(jsondoc.HasMember(KEY_TIME_TRAVEL_TIME)){
        str = MafAes256::Encrypt(jsondoc[KEY_TIME_TRAVEL_TIME].GetString());
        user->setStringForKey(KEY_TIME_TRAVEL_TIME, str);
    }else{
        user->setStringForKey(KEY_TIME_TRAVEL_TIME, "");
    }
    
    if(jsondoc.HasMember(KEY_ADVT_OFFICE_LV)){
        str = MafAes256::Encrypt(jsondoc[KEY_ADVT_OFFICE_LV].GetString());
        user->setStringForKey(KEY_ADVT_OFFICE_LV, str);
    }else{
        user->setStringForKey(KEY_ADVT_OFFICE_LV, "");
    }
    
    auto list = AdventureManager::getInstance()->getListInfoParty();
    int nPresetCount = 3;
    auto objSettingPresetCount = AdventureManager::getInstance()->getSetting("party_ability_preset_count");
    if(objSettingPresetCount != nullptr)
        nPresetCount = objSettingPresetCount->getValueToInt();
    
    for(int i = 0; i < list.size(); ++i)
    {
        std::string str_lv = MafUtils::format("advt_p_%d_m_lv", i+1).c_str();
        std::string str_preset_idx = MafUtils::format("advt_p_%d_a_p", i+1).c_str();
        
        for(int j = 0; j < nPresetCount; ++j)
        {
            std::string str_type = MafUtils::format("advt_p_%d_a_t_%d", i+1, j+1).c_str();
            std::string str_grade = MafUtils::format("advt_p_%d_a_g_%d", i+1, j+1).c_str();
            std::string str_seal = MafUtils::format("advt_p_%d_a_s_%d", i+1, j+1).c_str();
            
            if(jsondoc.HasMember(str_type.c_str())){
                str = MafAes256::Encrypt(jsondoc[str_type.c_str()].GetString());
                user->setStringForKey(str_type.c_str(), str);
            }else{
                user->setStringForKey(str_type.c_str(), "");
            }
            
            if(jsondoc.HasMember(str_grade.c_str())){
                str = MafAes256::Encrypt(jsondoc[str_grade.c_str()].GetString());
                user->setStringForKey(str_grade.c_str(), str);
            }else{
                user->setStringForKey(str_grade.c_str(), "");
            }
            
            if(jsondoc.HasMember(str_seal.c_str())){
                str = MafAes256::Encrypt(jsondoc[str_seal.c_str()].GetString());
                user->setStringForKey(str_seal.c_str(), str);
            }else{
                user->setStringForKey(str_seal.c_str(), "");
            }
        }
        
        if(jsondoc.HasMember(str_lv.c_str())){
            str = MafAes256::Encrypt(jsondoc[str_lv.c_str()].GetString());
            user->setStringForKey(str_lv.c_str(), str);
        }else{
            user->setStringForKey(str_lv.c_str(), "");
        }
        
        if(jsondoc.HasMember(str_preset_idx.c_str())){
            int nIdx = jsondoc[str_preset_idx.c_str()].GetInt();
            user->setIntegerForKey(str_preset_idx.c_str(), nIdx);
        }else{
            user->setIntegerForKey(str_preset_idx.c_str(), 1);
        }
    }
    
    if(jsondoc.HasMember(KEY_ADVT_TUTORIAL_STEP)){
        int value = jsondoc[KEY_ADVT_TUTORIAL_STEP].GetInt();
        user->setIntegerForKey(KEY_ADVT_TUTORIAL_STEP, value);
    }else{
        user->setIntegerForKey(KEY_ADVT_TUTORIAL_STEP, 1);
    }
    
    if(jsondoc.HasMember(KEY_ADVT_TUTORIAL_FRIEND)){
        int value = jsondoc[KEY_ADVT_TUTORIAL_FRIEND].GetInt();
        user->setIntegerForKey(KEY_ADVT_TUTORIAL_FRIEND, value);
    }else{
        user->setIntegerForKey(KEY_ADVT_TUTORIAL_FRIEND, 0);
    }
    
    if(jsondoc.HasMember(KEY_ADVT_TUTORIAL_NEW_CORE)){
        int value = jsondoc[KEY_ADVT_TUTORIAL_NEW_CORE].GetInt();
        user->setIntegerForKey(KEY_ADVT_TUTORIAL_NEW_CORE, value);
    }else{
        user->setIntegerForKey(KEY_ADVT_TUTORIAL_NEW_CORE, 0);
    }
    
    if(jsondoc.HasMember(KEY_ADVT_AVILITY_USE_GEM)){
        int value = jsondoc[KEY_ADVT_AVILITY_USE_GEM].GetInt();
        user->setIntegerForKey(KEY_ADVT_AVILITY_USE_GEM, value);
    }else{
        user->setIntegerForKey(KEY_ADVT_AVILITY_USE_GEM, 0);
    }
    
    //디펜스
    /*
     #define KEY_DF_HIGH_FLOOR  "df_h_f"
     #define KEY_DF_CURRENT_FLOOR  "df_c_f"
     #define KEY_DF_COIN  "df_c"
     #define KEY_DF_WEAPON_LV  "df_w_l"
     #define KEY_DF_CHARACTER_LV  "df_c_l"
     */
    if(jsondoc.HasMember(KEY_DF_HIGH_FLOOR)){
        str = MafAes256::Encrypt(jsondoc[KEY_DF_HIGH_FLOOR].GetString());
        user->setStringForKey(KEY_DF_HIGH_FLOOR, str);
    }else{
        user->setStringForKey(KEY_DF_HIGH_FLOOR, "");
    }
    if(jsondoc.HasMember(KEY_DF_CURRENT_FLOOR)){
        str = MafAes256::Encrypt(jsondoc[KEY_DF_CURRENT_FLOOR].GetString());
        user->setStringForKey(KEY_DF_CURRENT_FLOOR, str);
    }else{
        user->setStringForKey(KEY_DF_CURRENT_FLOOR, "");
    }
    if(jsondoc.HasMember(KEY_DF_COIN)){
        str = MafAes256::Encrypt(jsondoc[KEY_DF_COIN].GetString());
        user->setStringForKey(KEY_DF_COIN, str);
    }else{
        user->setStringForKey(KEY_DF_COIN, "");
    }
    if(jsondoc.HasMember(KEY_DF_WEAPON_LV)){
        str = MafAes256::Encrypt(jsondoc[KEY_DF_WEAPON_LV].GetString());
        user->setStringForKey(KEY_DF_WEAPON_LV, str);
    }else{
        user->setStringForKey(KEY_DF_WEAPON_LV, "");
    }
    if(jsondoc.HasMember(KEY_DF_CHARACTER_LV)){
        str = MafAes256::Encrypt(jsondoc[KEY_DF_CHARACTER_LV].GetString());
        user->setStringForKey(KEY_DF_CHARACTER_LV, str);
    }else{
        user->setStringForKey(KEY_DF_CHARACTER_LV, "");
    }
    
    //
    loadAllData();
    
    //
    MafHttpNew::_vecRequest.clear();
    
    //
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->unscheduleAllWithMinPriority(Scheduler::PRIORITY_NON_SYSTEM_MIN);
    
    MafGoogleRewardAds::HideGoogleBanner();
    Director::getInstance()->pause();
    UtilsScene::getInstance()->scenePopRoot();
    Director::getInstance()->resume();

}

#pragma mark - load data
void SaveManager::loadAllDataDevil(){
    
    UserInfoManager *info = UserInfoManager::getInstance();
    UserDefault *user = UserDefault::getInstance();
    std::string str;
    bool bSaveFalse = false;
    
    str.clear();
    ItemsMoneyManager::getInstance()->setDevilGoldZero(bSaveFalse);
    str = user->getStringForKey(KEY_D_GOLD);
    if(!str.empty()){
        ItemsMoneyManager::getInstance()->setDevilGold(MafAes256::Decrypt(str),bSaveFalse);
    }
    str.clear();
    ItemsMoneyManager::getInstance()->setDevilKeyZero();
    str = user->getStringForKey(KEY_D_KEY);
    if(!str.empty()){
        ItemsMoneyManager::getInstance()->setDevilKey(MafAes256::Decrypt(str),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_D_FLOOR);
    if(!str.empty()){
        info->setFloor(E_PLACE::DG_DEVIL_DOM, std::atoi(MafAes256::Decrypt(str).c_str()), bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_D_FLOOR_COUNT);
    if(!str.empty()){
        info->setFloorCount(E_PLACE::DG_DEVIL_DOM, std::atoi(MafAes256::Decrypt(str).c_str()),false, bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(DATA_D_HIGH_FLOOR);
    if(!str.empty()){
        info->setHighFloor(E_PLACE::DG_DEVIL_DOM, std::atoi(MafAes256::Decrypt(str).c_str()));
    }
    
    str.clear();
    str = user->getStringForKey(KEY_D_REVIVAL);
    if(!str.empty()){
        info->setRevival(E_PLACE::DG_DEVIL_DOM, std::atoi(MafAes256::Decrypt(str).c_str()));
    }
    
    
    str.clear();
    str = user->getStringForKey(KEY_D_QUEST_LEVEL);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        for(int i=1; i <= C_COUNT_QUEST_DEVIL; i++){
            auto offset = str.find(",");
            if(offset != std::string::npos){
                auto result = str.substr(0,offset);
                int ret = std::atoi(result.c_str());
                info->setQuestLevel(E_PLACE::DG_DEVIL_DOM,(E_QUEST)i,ret,bSaveFalse);
                str = str.substr(offset+1);
            }
            else{
                int ret = std::atoi(str.c_str());
                info->setQuestLevel(E_PLACE::DG_DEVIL_DOM,(E_QUEST)i,ret,bSaveFalse);
                break;
            }
        }
    }
    str.clear();
    str = user->getStringForKey(KEY_D_QUEST_TIME);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        for(int i=2; i <= C_COUNT_QUEST_DEVIL; i++){
            auto offset = str.find(",");
            if(offset != std::string::npos){
                auto result = str.substr(0,offset);
                auto ret = std::atof(result.c_str());
                info->setQuestTime(E_PLACE::DG_DEVIL_DOM,(E_QUEST)i, ret,bSaveFalse);
                str = str.substr(offset+1);
            }
            else{
                auto ret = std::atof(str.c_str());
                info->setQuestTime(E_PLACE::DG_DEVIL_DOM,(E_QUEST)i, ret,bSaveFalse);
                break;
            }
        }
    }
    str.clear();
    str = user->getStringForKey(KEY_D_EQUIP);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        for(int i=0; i < C_COUNT_EQUIP_TYPE_DEVIL; i++){
            auto offset = str.find(",");
            if(offset != std::string::npos){
                auto result = str.substr(0,offset);
                auto ret = std::atoi(result.c_str());
                info->setEquipDevil((E_EQUIP_TYPE)i,(E_EQUIP_IDX)ret,bSaveFalse);
                str = str.substr(offset+1);
            }
            else{
                auto ret = std::atoi(str.c_str());
                info->setEquipDevil((E_EQUIP_TYPE)i,(E_EQUIP_IDX)ret,bSaveFalse);
                break;
            }
        }
    }
    str.clear();
    str = user->getStringForKey(KEY_D_EQUIP_LEVEL);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        for(int i=0; i < C_COUNT_EQUIP_TYPE_DEVIL; i++){
            auto offset = str.find(",");
            if(offset != std::string::npos){
                auto result = str.substr(0,offset);
                auto ret = std::atoi(result.c_str());
                info->setEquipLevelDevil((E_EQUIP_TYPE)i,ret,bSaveFalse);
                str = str.substr(offset+1);
            }
            else{
                auto ret = std::atoi(str.c_str());
                info->setEquipLevelDevil((E_EQUIP_TYPE)i,ret,bSaveFalse);
                break;
            }
        }
    }
    str.clear();
    str = user->getStringForKey(KEY_D_EQUIP_REINFORCE);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        for(int i=0; i < C_COUNT_EQUIP_TYPE_DEVIL; i++){
            auto offset = str.find(",");
            if(offset != std::string::npos){
                auto result = str.substr(0,offset);
                auto ret = std::atoi(result.c_str());
                info->setEquipReinforceLevelDevil((E_EQUIP_TYPE)i,ret,bSaveFalse);
                str = str.substr(offset+1);
            }
            else{
                auto ret = std::atoi(str.c_str());
                info->setEquipReinforceLevelDevil((E_EQUIP_TYPE)i,ret,bSaveFalse);
                break;
            }
        }
    }
    
    str.clear();
    str = user->getStringForKey(KEY_D_ARTIFACT);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        for(int i=1; i <= C_COUNT_ARTIFACT_DEVIL; i++){
            auto offset = str.find(",");
            if(offset != std::string::npos){
                auto result = str.substr(0,offset);
                int ret = std::atoi(result.c_str());
                info->setArtifactDevil(i, ret > 0 ? true : false, bSaveFalse);
                str = str.substr(offset+1);
            }
            else{
                int ret = std::atoi(str.c_str());
                info->setArtifactDevil(i, ret > 0 ? true : false, bSaveFalse);
                break;
            }
        }
    }
    
    str.clear();
    str = user->getStringForKey(KEY_D_TIME_BUFF);
    if ( !str.empty())
    {
        str = MafAes256::Decrypt(str);
        
        E_BUFF_TYPE listBuff[] = {
            E_BUFF_TYPE::BUFF_D_ATK_DMG,
            E_BUFF_TYPE::BUFF_D_QUEST_GOLD,
            E_BUFF_TYPE::BUFF_D_MOVE_SPD,
            E_BUFF_TYPE::BUFF_D_ATK_SPD
        };
        
        for( int i = 0; i < sizeof(listBuff)/ sizeof(E_BUFF_TYPE); i++ )
        {
            auto offset = str.find(",");
            if(offset != std::string::npos)
            {
                auto result = str.substr(0,offset);
                int ret = std::atoi(result.c_str());
                info->setBuffTime(listBuff[i],ret,bSaveFalse);
                str = str.substr(offset+1);
            }
            else
            {
                int ret = std::atoi(str.c_str());
                info->setBuffTime(listBuff[i],ret,bSaveFalse);
                break;
            }
        }
    }
    
    str.clear();
    str = user->getStringForKey(KEY_D_TIME_ADS);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        for(int i=5; i <= 7; i++){
            auto offset = str.find(",");
            if(offset != std::string::npos){
                auto result = str.substr(0,offset);
                int ret = std::atoi(result.c_str());
                info->setAdsTime((E_SHOP_ADS)i,ret,bSaveFalse);
                str = str.substr(offset+1);
            }
            else{
                int ret = std::atoi(str.c_str());
                info->setAdsTime((E_SHOP_ADS)i,ret,bSaveFalse);
                break;
            }
        }
    }
    
    
    str.clear();
    str = user->getStringForKey(KEY_D_HIT_COUNT_HEDGEHOG);
    if(!str.empty()){
        info->setHitCountDevilHedgehog(atoi(MafAes256::Decrypt(str).c_str()));
    }
    
    str.clear();
    str = user->getStringForKey(KEY_D_HIT_COUNT_FLASH);
    if(!str.empty()){
        info->setHitCountDevilFlash(atoi(MafAes256::Decrypt(str).c_str()));
    }
    
    
    
    
}

void SaveManager::loadAllData()
{
    getInstance();
    
    loadAllDataDevil();
    
    UserInfoManager *info = UserInfoManager::getInstance();
    UserDefault *user = UserDefault::getInstance();
    std::string str;
    str.clear();
    
    bool bSaveFalse = false;
    
    // 재화   
    ItemsMoneyManager::getInstance()->initGold();
    ItemsMoneyManager::getInstance()->initKey();
    ItemsMoneyManager::getInstance()->initGem();
    ItemsMoneyManager::getInstance()->initPoint();
    ItemsMoneyManager::getInstance()->initCube();
    ItemsMoneyManager::getInstance()->initCostumeCoupon();
    ItemsMoneyManager::getInstance()->initMonsterSoulPiece();
    ItemsMoneyManager::getInstance()->initMythril();
    
    //
    UserDefault::getInstance()->setBoolForKey(KEY_NOTIFY_EFFECT_QUEST, false);
    UserDefault::getInstance()->setBoolForKey(KEY_NOTIFY_EFFECT_EQUIP, false);

    str.clear();
    str = user->getStringForKey(KEY_COSTUME_EQUIP);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        for(int i=1; i <= C_COUNT_COSTUME_TYPE; i++){
            auto offset = str.find(",");
            if(offset != std::string::npos){
                auto result = str.substr(0,offset);
                int ret = std::atoi(result.c_str());
                CostumeManager::getInstance()->setEquip((E_COSTUME)i, (E_COSTUME_IDX)ret);
                str = str.substr(offset+1);
            }
            else{
                int ret = std::atoi(str.c_str());
                CostumeManager::getInstance()->setEquip((E_COSTUME)i, (E_COSTUME_IDX)ret);
                break;
            }
        }
    }
    
    str.clear();
    str = user->getStringForKey(KEY_COSTUME_BUY);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        for(int i=1; i<=C_COUNT_COSTUME;i++){
            for(int j=1; j<=C_COUNT_COSTUME_TYPE; j++){
                auto offset = str.find(",");
                if(offset != std::string::npos){
                    auto result = str.substr(0,offset);
                    int ret = std::atoi(result.c_str());
                    
                    CostumeManager::getInstance()->setExist((E_COSTUME)j, (E_COSTUME_IDX)i, ret);
                    str = str.substr(offset+1);
                }
                else{
                    int ret = std::atoi(str.c_str());
                    str.clear();
                    
                    CostumeManager::getInstance()->setExist((E_COSTUME)j, (E_COSTUME_IDX)i, ret);
                    break;
                }
            }
            if (str.length() == 0) {
                break;
            }
        }
    }
    
    str.clear();
    str = user->getStringForKey(KEY_COSTUME_AURA_EQUIP);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        CostumeManager::getInstance()->setEquip(E_COSTUME::SETITEM, (E_COSTUME_IDX)std::atoi(str.c_str()));
    }
    
    str.clear();
    str = user->getStringForKey(KEY_COSTUME_AURA_BUY);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        for(int i=1; i <= C_COUNT_COSTUME; i++){
            auto offset = str.find(",");
            if(offset != std::string::npos){
                auto result = str.substr(0,offset);
                bool ret = (bool)std::atoi(result.c_str());
                CostumeManager::getInstance()->setExist(E_COSTUME::SETITEM, (E_COSTUME_IDX)i, ret);
                str = str.substr(offset+1);
            }
            else{
                bool ret = (bool)std::atoi(str.c_str());
                CostumeManager::getInstance()->setExist(E_COSTUME::SETITEM, (E_COSTUME_IDX)i, ret);
                break;
            }
        }
    }
    
    str.clear();
    str = user->getStringForKey(KEY_COSTUME_RENT_LIST);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        for(int i=1; i <= C_COUNT_COSTUME_TYPE; i++){
            auto offset = str.find(",");
            if(offset != std::string::npos){
                auto result = str.substr(0,offset);
                int ret = std::atoi(result.c_str());
                CostumeManager::getInstance()->setRent((E_COSTUME)i, (E_COSTUME_IDX)ret);// _uCostumeRentList[i] = ret;
                str = str.substr(offset+1);
            }
            else{
                int ret = std::atoi(str.c_str());
                CostumeManager::getInstance()->setRent((E_COSTUME)i, (E_COSTUME_IDX)ret);//info->_uCostumeRentList[i] = ret;
                break;
            }
        }
    }
    
    str.clear();
    str = user->getStringForKey(KEY_COSTUME_RENT_TIME);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        for(int i=1; i <= C_COUNT_COSTUME_TYPE; i++){
            auto offset = str.find(",");
            if(offset != std::string::npos){
                auto result = str.substr(0,offset);
                int ret = std::atoi(result.c_str());
                CostumeManager::getInstance()->setRentTime((E_COSTUME)i, ret, false);//info->_uCostumeRentTime[i] = ret;
                str = str.substr(offset+1);
            }
            else{
                int ret = std::atoi(str.c_str());
                CostumeManager::getInstance()->setRentTime((E_COSTUME)i, ret, false);//info->_uCostumeRentTime[i] = ret;
                break;
            }
        }
    }
    
    str.clear();
    str = user->getStringForKey(KEY_FLOOR);
    if(!str.empty()){
        info->setFloor(E_PLACE::DG_NORMAL, std::atoi(MafAes256::Decrypt(str).c_str()), bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_FLOOR_COUNT);
    if(!str.empty()){
        info->setFloorCount(E_PLACE::DG_NORMAL,std::atoi(MafAes256::Decrypt(str).c_str()),false, bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(DATA_HIGH_FLOOR);
    if(!str.empty()){
        info->setHighFloor(E_PLACE::DG_NORMAL, std::atoi(MafAes256::Decrypt(str).c_str()));
    }
    
    str.clear();
    str = user->getStringForKey(DATA_TOTAL_FLOOR);
    if(!str.empty()){
        info->setTotalFloor(std::atoi(MafAes256::Decrypt(str).c_str()));
    }
    
    str.clear();
    str = user->getStringForKey(DATA_REVIVAL);
    if(!str.empty()){
        info->setRevival(E_PLACE::DG_NORMAL, std::atoi(MafAes256::Decrypt(str).c_str()));
    }
    
    str.clear();
    str = user->getStringForKey(KEY_ARTIFACT_LEVEL);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        for(int i=1; i <= C_COUNT_ARTIFACT; i++){
            auto offset = str.find(",");
            if(offset != std::string::npos){
                auto result = str.substr(0,offset);
                int ret = std::atoi(result.c_str());
//                info->setArtifact((E_ARTIFACT)i,ret > 0 ? true : false,bSaveFalse);
                info->setArtifactLevel(E_PLACE::DG_NORMAL, (E_ARTIFACT)i,ret,bSaveFalse);
                str = str.substr(offset+1);
            }
            else{
                int ret = std::atoi(str.c_str());
//                info->setArtifact((E_ARTIFACT)i,ret > 0 ? true : false,bSaveFalse);
                info->setArtifactLevel(E_PLACE::DG_NORMAL, (E_ARTIFACT)i,ret,bSaveFalse);
                break;
            }
        }
    }
    
    str.clear();
    str = user->getStringForKey(KEY_ARTIFACT_REINFORCE);
    if(!str.empty()){
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
                
                info->setArtifactReinforce(E_PLACE::DG_NORMAL, atoi(str1.c_str()), atoi(str2.c_str()), bSaveFalse);
            }
        }
    }
    
    str.clear();
    str = user->getStringForKey(KEY_RELIC_LEVEL);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        for(int i=1; i <= C_COUNT_RELIC; i++){
            auto offset = str.find(",");
            if(offset != std::string::npos){
                auto result = str.substr(0,offset);
                int ret = std::atoi(result.c_str());
                info->setRelicLevel(E_PLACE::DG_NORMAL, (E_RELIC)i, ret, bSaveFalse);
                str = str.substr(offset+1);
            }
            else{
                int ret = std::atoi(str.c_str());
                info->setRelicLevel(E_PLACE::DG_NORMAL, (E_RELIC)i, ret, bSaveFalse);
                break;
            }
        }
    }
    
    str.clear();
    str = user->getStringForKey(KEY_HONOR);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        for(int i=E_HONOR::HONOR_1; i <= E_HONOR::HONOR_3; i++){
            auto offset = str.find(",");
            if(offset != std::string::npos){
                auto result = str.substr(0,offset);
                int ret = std::atoi(result.c_str());
                info->setHonorLevel((E_HONOR)i,ret,bSaveFalse);
                str = str.substr(offset+1);
            }
            else{
                int ret = std::atoi(str.c_str());
                info->setHonorLevel((E_HONOR)i,ret,bSaveFalse);
                break;
            }
        }
    }
    
    str.clear();
    str = user->getStringForKey(KEY_QUEST_LEVEL);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        for(int i=1; i <= C_COUNT_QUEST; i++){
            auto offset = str.find(",");
            if(offset != std::string::npos){
                auto result = str.substr(0,offset);
                int ret = std::atoi(result.c_str());
                info->setQuestLevel(E_PLACE::DG_NORMAL,(E_QUEST)i,ret,bSaveFalse);
                str = str.substr(offset+1);
            }
            else{
                int ret = std::atoi(str.c_str());
                info->setQuestLevel(E_PLACE::DG_NORMAL,(E_QUEST)i,ret,bSaveFalse);
                break;
            }
        }
    }
    
//    str.clear();
//    str = user->getStringForKey(KEY_QUEST_AUTO);
//    if(!str.empty()){
//        info->setQuestAuto((E_QUEST)std::atoi(MafAes256::Decrypt(str)),bSaveFalse);
//    }
    
    str.clear();
    str = user->getStringForKey(KEY_COUNT_KEY_BONUS);
    if(!str.empty()){
        info->setBuffTime(E_BUFF_TYPE::BUFF_KEY_BONUS, std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    str.clear();
    str = user->getStringForKey(KEY_COUNT_HERO_BONUS);
    if(!str.empty()){
        info->setBuffTime(E_BUFF_TYPE::BUFF_HERO_BONUS, std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    str.clear();
    str = user->getStringForKey(KEY_TIME_ATTACK_BONUS);
    if(!str.empty()){
        info->setBuffTime(E_BUFF_TYPE::BUFF_ATTACK_BONUS, std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    str.clear();
    str = user->getStringForKey(KEY_TIME_ATTACK_9_BONUS);
    if(!str.empty()){
        info->setBuffTime(E_BUFF_TYPE::BUFF_ATTACK_9_BONUS, std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    str.clear();
    str = user->getStringForKey(KEY_TIME_ATTACK_10_BONUS);
    if(!str.empty()){
        info->setBuffTime(E_BUFF_TYPE::BUFF_ATTACK_10_BONUS, std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    str.clear();
    str = user->getStringForKey(KEY_TIME_COIN_BONUS);
    if(!str.empty()){
        info->setBuffTime(E_BUFF_TYPE::BUFF_COIN_BONUS, std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    str.clear();
    str = user->getStringForKey(KEY_TIME_MOVE_BONUS);
    if(!str.empty()){
        info->setBuffTime(E_BUFF_TYPE::BUFF_MOVE_BONUS, std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_TIME_ADS_GOLD_BONUS);
    if(!str.empty()){
        info->setAdsTime(E_SHOP_ADS::SHOP_GOLD_ADS, std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_TIME_ADS_GEM_BONUS);
    if(!str.empty()){
        info->setAdsTime(E_SHOP_ADS::SHOP_GEM_ADS, std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_TIME_ADS_BUFF_BONUS);
    if(!str.empty()){
        info->setAdsTime(E_SHOP_ADS::SHOP_BUFF_ADS, std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_TIME_ADS_MINE_BONUS);
    if(!str.empty()){
        info->setAdsTime(E_SHOP_ADS::SHOP_MINE_ADS, std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_TIME_ADS_SELECT_BONUS);
    if(!str.empty()){
        info->setAdsTime(E_SHOP_ADS::SHOP_SELECT_ADS, std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_TIME_ADS_BUFF_ATK);
    if(!str.empty()){
        info->setAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_ATK, std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_TIME_ADS_BUFF_GOLD);
    if(!str.empty()){
        info->setAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_GOLD, std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_TIME_ADS_BUFF_MOVE);
    if(!str.empty()){
        info->setAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_MOVE, std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_TIME_ADS_BUFF_D_ATK);
    if(!str.empty()){
        info->setAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_ATK, std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_TIME_ADS_BUFF_D_ASPD);
    if(!str.empty()){
        info->setAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_ASPD, std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_TIME_ADS_BUFF_D_GOLD);
    if(!str.empty()){
        info->setAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_GOLD, std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_TIME_ADS_BUFF_D_MOVE);
    if(!str.empty()){
        info->setAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_MOVE, std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_TIME_ADS_BUFF_GOLD_NTERSTITIAL);
    if(!str.empty()){
        info->setAdsTime(E_SHOP_ADS::SHOP_BUFF_GOLD_INTERSTITIAL, std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_TIME_ITEM_FEVER);
    if(!str.empty()){
        info->setItemFeverTime(std::atoi(MafAes256::Decrypt(str).c_str()), bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_TIME_ITEM_FEVER_COUNT);
    if(!str.empty()){
        info->setItemFeverCount(std::atoi(MafAes256::Decrypt(str).c_str()), bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_DRAGON_SKILLCHECK);
    if(!str.empty()){
        UserInfoStackManager::getInstance()->setDragonSkillCheck(std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_REVIVAL_ATK_BUFF);
    if(!str.empty()){
        UserInfoStackManager::getInstance()->setRevivalAttackBuff(std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_REVIVAL_QUEST_BUFF);
    if(!str.empty()){
        UserInfoStackManager::getInstance()->setRevivalQuestBuff(std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_REVIVAL_KEY_BUFF1);
    if(!str.empty()){
        UserInfoStackManager::getInstance()->setRevivalKeyBuff1(std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_REVIVAL_KEY_BUFF2);
    if(!str.empty()){
        UserInfoStackManager::getInstance()->setRevivalKeyBuff2(std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_E_REVIVAL_ATK_BUFF);
    if(!str.empty()){
        UserInfoStackManager::getInstance()->setEnchantRevivalAttackBuff(std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_HIT_BUFF);
    if(!str.empty()){
        UserInfoStackManager::getInstance()->setHitBuff(std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_DRAGON_BUFF);
    if(!str.empty()){
        UserInfoStackManager::getInstance()->setDragonBuff(std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_HELL_BUFF);
    if(!str.empty()){
        UserInfoStackManager::getInstance()->setHellBuff(std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_QUEST_25_BUFF);
    if(!str.empty()){
        UserInfoStackManager::getInstance()->setQuest25Buff(std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_REVIVE_SPECIAL);
    if(!str.empty()){
        UserInfoStackManager::getInstance()->setReviveSpecial(std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_REVIVE_KEYBOOSTER);
    if(!str.empty()){
        UserInfoStackManager::getInstance()->setReviveKeyBooster(std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_SPECIAL_CLEAR);
    if(!str.empty()){
        SpecialMissionManager::getInstance()->setSpecialMissionClear(std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_SPECIAL_COMPLETE);
    if(!str.empty()){
        SpecialMissionManager::getInstance()->setSpecialMissionComplete(std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_ATTEND_VIP);
    if(!str.empty()){
        info->setAttendVip(std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_ATTEND_DAY);
    if(!str.empty()){
        info->setAttendDay(std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_MYHOME_LEVEL);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        for(int i=1; i <= C_MAX_MYHOME; i++){
            auto offset = str.find(",");
            if(offset != std::string::npos){
                auto result = str.substr(0,offset);
                int ret = std::atoi(result.c_str());
                info->setMyhomeLevel(i, ret, bSaveFalse);
                str = str.substr(offset+1);
            }
            else{
                int ret = std::atoi(str.c_str());
                info->setMyhomeLevel(i, ret, bSaveFalse);
                break;
            }
        }
    }
    
    str.clear();
    str = user->getStringForKey(KEY_MYHOME_WATCHCOUNT);
    if(!str.empty()){
        str = MafAes256::Decrypt(str);
        for(int i=1; i <= C_MAX_MYHOME; i++){
            auto offset = str.find(",");
            if(offset != std::string::npos){
                auto result = str.substr(0,offset);
                int ret = std::atoi(result.c_str());
                info->setMyhomeWatchcount(i, ret, bSaveFalse);
                str = str.substr(offset+1);
            }
            else{
                int ret = std::atoi(str.c_str());
                info->setMyhomeWatchcount(i, ret, bSaveFalse);
                break;
            }
        }
    }
    str.clear();
    str = user->getStringForKey(KEY_TIME_ADS_MYHOME_BUFF);
    if(!str.empty()){
        info->setMyhomeAdsTime(1, std::atoi(MafAes256::Decrypt(str).c_str()), false);
    }
    str.clear();
    str = user->getStringForKey(KEY_TIME_ADS_MYHOME_GEM);
    if(!str.empty()){
        info->setMyhomeAdsTime(2, std::atoi(MafAes256::Decrypt(str).c_str()), false);
    }
    str.clear();
    str = user->getStringForKey(KEY_TIME_ADS_MYHOME_GOLD);
    if(!str.empty()){
        info->setMyhomeAdsTime(3, std::atoi(MafAes256::Decrypt(str).c_str()), false);
    }
//    str.clear();
//    str = user->getStringForKey(KEY_TIME_ADS_MYHOME_COSTUMECOIN);
//    if(!str.empty()){
//        info->setAttendDay(std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
//    }
    str.clear();
    str = user->getStringForKey(KEY_TIME_ADS_MYHOME_MINE);
    if(!str.empty()){
        info->setMyhomeAdsTime(4, std::atoi(MafAes256::Decrypt(str).c_str()), false);
    }
    str.clear();
    str = user->getStringForKey(KEY_TIME_ADS_MYHOME_CUBE);
    if(!str.empty()){
        info->setMyhomeAdsTime(5,std::atoi(MafAes256::Decrypt(str).c_str()),false);
    }
//    str.clear();
//    str = user->getStringForKey(KEY_TIME_ADS_MYHOME_BONTRIUM);
//    if(!str.empty()){
//        info->setAttendDay(std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
//    }
//
    
    // 디바이스 기록 저장
    {
        time_t timer_s = UtilsDate::getInstance()->getTime();
        std::string nowTime = MafUtils::toString(timer_s);
        std::string savedTime = SaveManager::getHistoryData(E_HISTORY_DATA::K_LAST_DEVICE_TIME);
        if(MafUtils::compareStr(nowTime, savedTime)){
            SaveManager::setHistoryData(E_HISTORY_DATA::K_LAST_DEVICE_TIME, nowTime);
        }
    }
    
    str.clear();
    str = user->getStringForKey(KEY_MINE_PICKAX);
    if(!str.empty()){
        //info->setMinePickaxLevel(std::atoi(MafAes256::Decrypt(str).c_str()),bSaveFalse);
        info->setArtifactLevel(E_PLACE::DG_NORMAL, E_ARTIFACT::ARTI_34_PICKAX, std::atoi(MafAes256::Decrypt(str).c_str()));
        user->deleteValueForKey(KEY_MINE_PICKAX);
    }
    
    str.clear();
    str = user->getStringForKey(KEY_HITCOUNT_NORMAL);
    if(!str.empty()){
        info->setHitCount(E_PLACE::DG_NORMAL, std::atoi(MafAes256::Decrypt(str).c_str()));
    }
    
    str.clear();
    str = user->getStringForKey(KEY_HITCOUNT_DUNGEON);
    if(!str.empty()){
        info->setHitCount(E_PLACE::DG_PRISON, std::atoi(MafAes256::Decrypt(str).c_str()));
    }
    
    str.clear();
    str = user->getStringForKey(KEY_HITCOUNT_HONG_NORMAL);
    if(!str.empty()){
        info->setHongHitCount(E_PLACE::DG_NORMAL, std::atoi(MafAes256::Decrypt(str).c_str()));
    }
    
    str.clear();
    str = user->getStringForKey(KEY_HITCOUNT_HONG_DUNGEON);
    if(!str.empty()){
        info->setHongHitCount(E_PLACE::DG_PRISON, std::atoi(MafAes256::Decrypt(str).c_str()));
    }
    
    //
    TownManager::getInstance()->loadData();
    TimeTravelManager::getInstance()->loadData();
    ChatManager::getInstance()->loadData();
    CostumeSkinManager::getInstance()->loadData();
    PetNewManager::getInstance()->loadData();
    PetNewDevilManager::getInstance()->loadData();
    WeaponManager::getInstance()->loadData();
    MineralManager::getInstance()->loadData();
    FlashmanManager::getInstance()->loadData();
    MonsterManager::getInstance()->loadData();
    StoneManager::getInstance()->loadData();
}

#pragma mark - save data
void SaveManager::saveAllData()
{
    UserInfoManager *info = UserInfoManager::getInstance();
    UserDefault *user = UserDefault::getInstance();
    std::stringstream str;
    
    //1초에 한번씩 저장하는것들이 많은데 굳이 안해도 되는것 추리면 ?
    saveQuest(E_PLACE::DG_NORMAL);
    saveQuest(E_PLACE::DG_DEVIL_DOM);
    saveBuffTime();
    saveAdsTime();
    
    saveEquip(E_PLACE::DG_NORMAL);
    saveEquip(E_PLACE::DG_DEVIL_DOM);
    
    
    str.str("");
    for(int i=1; i<=C_COUNT_ARTIFACT;i++){
        str << info->getArtifactLevel(E_PLACE::DG_NORMAL, (E_ARTIFACT)i) <<",";
    }
    user->setStringForKey(KEY_ARTIFACT_LEVEL, MafAes256::Encrypt(str.str()));
    
    str.str("");
    for(int i=1; i<=C_COUNT_RELIC;i++){
        str << info->getRelicLevel(E_PLACE::DG_NORMAL, (E_RELIC)i) <<",";
    }
    user->setStringForKey(KEY_RELIC_LEVEL, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << MafUtils::toString(info->getHitCount(E_PLACE::DG_NORMAL));
    user->setStringForKey(KEY_HITCOUNT_NORMAL, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << MafUtils::toString(info->getHitCount(E_PLACE::DG_PRISON));
    user->setStringForKey(KEY_HITCOUNT_DUNGEON, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << MafUtils::toString(info->getHongHitCount(E_PLACE::DG_NORMAL));
    user->setStringForKey(KEY_HITCOUNT_HONG_NORMAL, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << MafUtils::toString(info->getHongHitCount(E_PLACE::DG_PRISON));
    user->setStringForKey(KEY_HITCOUNT_HONG_DUNGEON, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << MafUtils::toString(info->getHitCountDevilHedgehog());
    user->setStringForKey(KEY_D_HIT_COUNT_HEDGEHOG, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << MafUtils::toString(info->getHitCountDevilFlash());
    user->setStringForKey(KEY_D_HIT_COUNT_FLASH, MafAes256::Encrypt(str.str()));
    
    
    str.str("");
    for(int i=1; i<=C_COUNT_ARTIFACT_DEVIL;i++){
        str << info->isArtifactHaveDevil(i) <<",";
    }
    user->setStringForKey(KEY_D_ARTIFACT, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << MafUtils::toString(info->getMyhomeAdsTime(1));
    user->setStringForKey(KEY_TIME_ADS_MYHOME_BUFF,MafAes256::Encrypt(str.str()));

    str.str("");
    str << MafUtils::toString(info->getMyhomeAdsTime(2));
    user->setStringForKey(KEY_TIME_ADS_MYHOME_GEM,MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << MafUtils::toString(info->getMyhomeAdsTime(3));
    user->setStringForKey(KEY_TIME_ADS_MYHOME_GOLD,MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << MafUtils::toString(info->getMyhomeAdsTime(4));
    user->setStringForKey(KEY_TIME_ADS_MYHOME_MINE,MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << MafUtils::toString(info->getMyhomeAdsTime(5));
    user->setStringForKey(KEY_TIME_ADS_MYHOME_CUBE,MafAes256::Encrypt(str.str()));
    
    //
    TownManager::getInstance()->saveData();
    InfiniteArtifactManager::getInstance()->saveInfiniteArtiData();
    TimeTravelManager::getInstance()->saveData();
    ChatManager::getInstance()->saveData();
    CostumeSkinManager::getInstance()->saveData();
    PetNewManager::getInstance()->saveData();
    PetNewDevilManager::getInstance()->saveData();
    WeaponManager::getInstance()->saveData();
    MineralManager::getInstance()->saveData();
    FlashmanManager::getInstance()->saveData();
    MonsterManager::getInstance()->saveData();
    StoneManager::getInstance()->saveData();
    
    //
    user->flush();
}

void SaveManager::saveCostume()
{
    UserInfoManager *info = UserInfoManager::getInstance();
    UserDefault *user = UserDefault::getInstance();
    std::stringstream str;
    
    str.str("");
    for(int i=1; i<=C_COUNT_COSTUME;i++){
        for(int j=1; j<=C_COUNT_COSTUME_TYPE; j++){
            str << CostumeManager::getInstance()->getExist((E_COSTUME)j, (E_COSTUME_IDX)i) <<",";
        }
    }
    user->setStringForKey(KEY_COSTUME_BUY, MafAes256::Encrypt(str.str()));
    
    str.str("");
    for(int i=1; i<=C_COUNT_COSTUME_TYPE;i++){
        str << CostumeManager::getInstance()->getEquip((E_COSTUME)i) <<",";
    }
    user->setStringForKey(KEY_COSTUME_EQUIP, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << CostumeManager::getInstance()->getEquip(E_COSTUME::SETITEM);
    user->setStringForKey(KEY_COSTUME_AURA_EQUIP, MafAes256::Encrypt(str.str()));
    
    str.str("");
    for(int i=1; i<=C_COUNT_COSTUME;i++){
        str << CostumeManager::getInstance()->getExist(E_COSTUME::SETITEM, (E_COSTUME_IDX)i) <<",";
    }
    user->setStringForKey(KEY_COSTUME_AURA_BUY, MafAes256::Encrypt(str.str()));
}

void SaveManager::saveCostumeRentList()
{
    UserInfoManager *info = UserInfoManager::getInstance();
    UserDefault *user = UserDefault::getInstance();
    
    std::stringstream str;
    str.str("");
    for(int i=1; i<=C_COUNT_COSTUME_TYPE;i++){
        str << CostumeManager::getInstance()->getRent((E_COSTUME)i)<<",";
    }
    user->setStringForKey(KEY_COSTUME_RENT_LIST, MafAes256::Encrypt(str.str()));
}

void SaveManager::saveCostumeRentTime()
{
    UserInfoManager *info = UserInfoManager::getInstance();
    UserDefault *user = UserDefault::getInstance();
    
    std::stringstream str;
    str.str("");
    for(int i=1; i<=C_COUNT_COSTUME_TYPE;i++){
        str << CostumeManager::getInstance()->getRentTime((E_COSTUME)i) <<",";
    }
    user->setStringForKey(KEY_COSTUME_RENT_TIME, MafAes256::Encrypt(str.str()));
}


void SaveManager::saveEquip(E_PLACE eplace)
{
    UserInfoManager *info = UserInfoManager::getInstance();
    UserDefault *user = UserDefault::getInstance();
    std::stringstream str;

    if(eplace == E_PLACE::DG_NORMAL)
    {
        
    }
    else if(eplace == E_PLACE::DG_DEVIL_DOM)
    {
        //devil
        str.str("");
        for(int i=0; i<C_COUNT_EQUIP_TYPE_DEVIL;i++)
            str << info->getEquipDevil((E_EQUIP_TYPE)i) <<",";
        user->setStringForKey(KEY_D_EQUIP, MafAes256::Encrypt(str.str()));
        str.str("");
        for(int i=0; i<C_COUNT_EQUIP_TYPE_DEVIL;i++)
            str << info->getEquipLevelDevil((E_EQUIP_TYPE)i) <<",";
        user->setStringForKey(KEY_D_EQUIP_LEVEL, MafAes256::Encrypt(str.str()));
        str.str("");
        for(int i=0; i<C_COUNT_EQUIP_TYPE_DEVIL;i++)
            str << info->getEquipReinforceLevelDevil((E_EQUIP_TYPE)i) <<",";
        user->setStringForKey(KEY_D_EQUIP_REINFORCE, MafAes256::Encrypt(str.str()));
    }
}

void SaveManager::saveQuest(E_PLACE eplace)
{
    UserInfoManager *info = UserInfoManager::getInstance();
    UserDefault *user = UserDefault::getInstance();
    std::stringstream str;
    
    if(eplace == E_PLACE::DG_NORMAL){
        for(int i=1; i<=C_COUNT_QUEST;i++){
            str << info->getQuestLevel(E_PLACE::DG_NORMAL,(E_QUEST)i) <<",";
        }
        user->setStringForKey(KEY_QUEST_LEVEL, MafAes256::Encrypt(str.str()));
    }
    else if(eplace == E_PLACE::DG_DEVIL_DOM){
        for(int i=1; i<=C_COUNT_QUEST_DEVIL;i++){
            str << info->getQuestLevel(E_PLACE::DG_DEVIL_DOM,(E_QUEST)i) <<",";
        }
        user->setStringForKey(KEY_D_QUEST_LEVEL, MafAes256::Encrypt(str.str()));
    }
}

void SaveManager::saveQuestTime(E_PLACE eplace)
{
    
    //진행중인 퀘스트시간
    UserInfoManager *info = UserInfoManager::getInstance();
    UserDefault *user = UserDefault::getInstance();
    
    std::stringstream str;

    int qLv = 0;
    if(eplace == E_PLACE::DG_NORMAL){
        
    }
    else if(eplace == E_PLACE::DG_DEVIL_DOM){
        for(int i=2; i<=C_COUNT_QUEST_DEVIL;i++){ //1번은 1초니까 그냥완료했다고처리.
            qLv = info->getQuestLevel(E_PLACE::DG_DEVIL_DOM,(E_QUEST)i);
            if(qLv==0)  str << DataManager::GetQuestSpendTime(E_PLACE::DG_DEVIL_DOM,(E_QUEST)i);
            else        str << info->getQuestTime(E_PLACE::DG_DEVIL_DOM,(E_QUEST)i);
            
            if(i<C_COUNT_QUEST_DEVIL)
                str << ",";
        }
        user->setStringForKey(KEY_D_QUEST_TIME, MafAes256::Encrypt(str.str()));
    }
}

void SaveManager::saveBuffTime()
{
    UserInfoManager *info = UserInfoManager::getInstance();
    UserDefault *user = UserDefault::getInstance();
    std::stringstream str;
    
    str << info->getBuffTime(E_BUFF_TYPE::BUFF_ATTACK_BONUS);
    user->setStringForKey(KEY_TIME_ATTACK_BONUS, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << info->getBuffTime(E_BUFF_TYPE::BUFF_ATTACK_9_BONUS);
    user->setStringForKey(KEY_TIME_ATTACK_9_BONUS, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << info->getBuffTime(E_BUFF_TYPE::BUFF_ATTACK_10_BONUS);
    user->setStringForKey(KEY_TIME_ATTACK_10_BONUS, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << info->getBuffTime(E_BUFF_TYPE::BUFF_COIN_BONUS);
    user->setStringForKey(KEY_TIME_COIN_BONUS, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << info->getBuffTime(E_BUFF_TYPE::BUFF_MOVE_BONUS);
    user->setStringForKey(KEY_TIME_MOVE_BONUS, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << info->getBuffTime(E_BUFF_TYPE::BUFF_KEY_BONUS);
    user->setStringForKey(KEY_COUNT_KEY_BONUS, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << info->getBuffTime(E_BUFF_TYPE::BUFF_HERO_BONUS);
    user->setStringForKey(KEY_COUNT_HERO_BONUS, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << info->getItemFeverTime();
    user->setStringForKey(KEY_TIME_ITEM_FEVER, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << info->getItemFeverCount();
    user->setStringForKey(KEY_TIME_ITEM_FEVER_COUNT, MafAes256::Encrypt(str.str()));
    
    str.str("");
    E_BUFF_TYPE listBuff[] = {
        E_BUFF_TYPE::BUFF_D_ATK_DMG,
        E_BUFF_TYPE::BUFF_D_QUEST_GOLD,
        E_BUFF_TYPE::BUFF_D_MOVE_SPD,
        E_BUFF_TYPE::BUFF_D_ATK_SPD
    };
    
    for( int i = 0; i < sizeof(listBuff)/ sizeof(E_BUFF_TYPE); i++ )
    {
        str << info->getBuffTime(listBuff[i]) << ",";
    }
    user->setStringForKey(KEY_D_TIME_BUFF, MafAes256::Encrypt(str.str()));
}

void SaveManager::saveAdsTime()
{
    UserInfoManager *info = UserInfoManager::getInstance();
    UserDefault *user = UserDefault::getInstance();
    std::stringstream str;
    
    str.str("");
    str << info->getAdsTime(E_SHOP_ADS::SHOP_GOLD_ADS);
    user->setStringForKey(KEY_TIME_ADS_GOLD_BONUS, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << info->getAdsTime(E_SHOP_ADS::SHOP_GEM_ADS);
    user->setStringForKey(KEY_TIME_ADS_GEM_BONUS, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << info->getAdsTime(E_SHOP_ADS::SHOP_BUFF_ADS);
    user->setStringForKey(KEY_TIME_ADS_BUFF_BONUS, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << info->getAdsTime(E_SHOP_ADS::SHOP_MINE_ADS);
    user->setStringForKey(KEY_TIME_ADS_MINE_BONUS, MafAes256::Encrypt(str.str()));
    
    str.str("");
    for(int i=5; i<=7;i++){
        str << info->getAdsTime((E_SHOP_ADS)i) << ",";
    }
    user->setStringForKey(KEY_D_TIME_ADS, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str<< info->getAdsTime(E_SHOP_ADS::SHOP_SELECT_ADS);
    user->setStringForKey(KEY_TIME_ADS_SELECT_BONUS, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << info->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_ATK);
    user->setStringForKey(KEY_TIME_ADS_BUFF_ATK, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << info->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_GOLD);
    user->setStringForKey(KEY_TIME_ADS_BUFF_GOLD, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << info->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_MOVE);
    user->setStringForKey(KEY_TIME_ADS_BUFF_MOVE, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << info->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_ATK);
    user->setStringForKey(KEY_TIME_ADS_BUFF_D_ATK, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << info->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_ASPD);
    user->setStringForKey(KEY_TIME_ADS_BUFF_D_ASPD, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << info->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_GOLD);
    user->setStringForKey(KEY_TIME_ADS_BUFF_D_GOLD, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << info->getAdsTime(E_SHOP_ADS::SHOP_BUFF_COOLDOWN_D_MOVE);
    user->setStringForKey(KEY_TIME_ADS_BUFF_D_MOVE, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << info->getAdsTime(E_SHOP_ADS::SHOP_BUFF_GOLD_INTERSTITIAL);
    user->setStringForKey(KEY_TIME_ADS_BUFF_GOLD_NTERSTITIAL, MafAes256::Encrypt(str.str()));
}

void SaveManager::saveMyhomeLevel()
{
    UserInfoManager *info = UserInfoManager::getInstance();
    UserDefault *user = UserDefault::getInstance();
    
    std::stringstream str;
    str.str("");
    for(int i=1; i<=C_MAX_MYHOME;i++){
        str << info->getMyhomeLevel(i) <<",";
    }
    user->setStringForKey(KEY_MYHOME_LEVEL, MafAes256::Encrypt(str.str()));
}

void SaveManager::saveMyhomeWatchcount()
{
    UserInfoManager *info = UserInfoManager::getInstance();
    UserDefault *user = UserDefault::getInstance();
    
    std::stringstream str;
    str.str("");
    for(int i=1; i<=C_MAX_MYHOME;i++){
        str << info->getMyhomeWatchcount(i) <<",";
    }
    user->setStringForKey(KEY_MYHOME_WATCHCOUNT, MafAes256::Encrypt(str.str()));
}

void SaveManager::saveMyhomeAdsTime()
{
    UserInfoManager *info = UserInfoManager::getInstance();
    UserDefault *user = UserDefault::getInstance();
    std::stringstream str;
    
    str.str("");
    str << info->getMyhomeAdsTime(1);
    user->setStringForKey(KEY_TIME_ADS_MYHOME_BUFF, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << info->getMyhomeAdsTime(2);
    user->setStringForKey(KEY_TIME_ADS_MYHOME_GEM, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << info->getMyhomeAdsTime(3);
    user->setStringForKey(KEY_TIME_ADS_MYHOME_GOLD, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << info->getMyhomeAdsTime(4);
    user->setStringForKey(KEY_TIME_ADS_MYHOME_MINE, MafAes256::Encrypt(str.str()));
    
    str.str("");
    str << info->getMyhomeAdsTime(5);
    user->setStringForKey(KEY_TIME_ADS_MYHOME_CUBE, MafAes256::Encrypt(str.str()));
}

#pragma mark - history data
std::string SaveManager::getHistorySaveToServerData()
{
    std::stringstream str;
    for(int i=0 ; i < (int)E_HISTORY_DATA::K_NONE_ONLY_COUNT; i++){
        str << getHistoryData((E_HISTORY_DATA)i) << ",";
    }
    std::string data = MafAes256::Encrypt(str.str());
    return data;
}

void SaveManager::setHistorySaveFromServerData(std::string data)
{
    if(data.length() ==0)
        return;
    
    data = MafAes256::Decrypt(data);
//    data = MafAes256::Encrypt(data);
//    MafUtils::WriteTextFile("/Git/cocos2dx/FantasyClicker/Resources/Data/info_personal_data.txt", data);
//    log("\n\n%s\n\n", data.c_str());
    
    for(int i=0; i < (int)E_HISTORY_DATA::K_NONE_ONLY_COUNT; i++){
        auto offset = data.find(",");
        if(offset != std::string::npos){
            auto result = data.substr(0,offset);
            setHistoryData((E_HISTORY_DATA)i, result);
            data = data.substr(offset+1);
        }
    }
}

void SaveManager::setHistoryData(E_HISTORY_DATA eHistory, const int value)
{
    setHistoryData(eHistory, MafUtils::toString(value));
}

void SaveManager::setHistoryData(E_HISTORY_DATA eHistory, const std::string & value)
{
    UserDefault *user = UserDefault::getInstance();
    std::stringstream str;
    str << "his_" << (int)eHistory;
    user->setStringForKey(str.str().c_str(), value) ;
}

int SaveManager::getHistoryDataInt(E_HISTORY_DATA eHistory)
{
    std::string data = getHistoryData(eHistory);
    return atoi(data.c_str());
}

std::string SaveManager::getHistoryData(E_HISTORY_DATA eHistory)
{
    std::string value;
    std::stringstream str;
    auto info = UserInfoManager::getInstance();
    if(eHistory==E_HISTORY_DATA::K_WEAPON_HIGH){
        value = GAME_TEXTFORMAT("t_ui_equip_msg_5" , WeaponManager::getInstance()->getEquipHigh());
        return value; // = str.str();
    }
    else if(eHistory==E_HISTORY_DATA::D_WEAPON_HIGH){
        value = GAME_TEXT(MafUtils::format("t_equip_d2_title_0_%d", info->getEquipDevil(E_EQUIP_TYPE::E_D_WEAPON)));
        return value; // = str.str();
    }
    else if(eHistory==E_HISTORY_DATA::D_HEAD_HIGH){
        value = GAME_TEXT(MafUtils::format("t_equip_d2_title_1_%d", info->getEquipDevil(E_EQUIP_TYPE::E_D_HEAD)));
        return value; // = str.str();
    }
    else if(eHistory==E_HISTORY_DATA::D_BODY_HIGH){
        value = GAME_TEXT(MafUtils::format("t_equip_d2_title_2_%d", info->getEquipDevil(E_EQUIP_TYPE::E_D_BODY)));
        return value; // = str.str();
    }
    else if(eHistory==E_HISTORY_DATA::D_CAPE_HIGH){
        value = GAME_TEXT(MafUtils::format("t_equip_d2_title_3_%d", info->getEquipDevil(E_EQUIP_TYPE::E_D_CAPE)));
        return value; // = str.str();
    }
    else if(eHistory==E_HISTORY_DATA::K_REVIVAL){
        return value = MafUtils::toString(info->getRevival(E_PLACE::DG_NORMAL));
    }
    else if(eHistory==E_HISTORY_DATA::K_HIGH_FLOOR){
        return value = MafUtils::toString(info->getHighFloor(E_PLACE::DG_NORMAL));
    }
    else if(eHistory==E_HISTORY_DATA::K_TOTAL_FLOOR){
        return value = MafUtils::toString(info->getTotalFloor());
    }
    else if(eHistory==E_HISTORY_DATA::D_PET_LEVEL_1)
    {
        value = MafUtils::format("%d", PetNewDevilManager::getInstance()->getPetLevel(E_PET::D_1_PENGUIN));
        return value;
    }
    else if(eHistory==E_HISTORY_DATA::D_PET_LEVEL_2)
    {
        value = MafUtils::format("%d", PetNewDevilManager::getInstance()->getPetLevel(E_PET::D_2_HEDGEHOG));
        return value;
    }
    else if(eHistory==E_HISTORY_DATA::D_PET_LEVEL_3)
    {
        value = MafUtils::format("%d", PetNewDevilManager::getInstance()->getPetLevel(E_PET::D_3_GATEKEEPER));
        return value;
    }
    else if(eHistory==E_HISTORY_DATA::D_PET_LEVEL_4)
    {
        value = MafUtils::format("%d", PetNewDevilManager::getInstance()->getPetLevel(E_PET::D_4_FLASH));
        return value;
    }
    else if(eHistory==E_HISTORY_DATA::D_REVIVAL){
        return value = MafUtils::toString(info->getRevival(E_PLACE::DG_DEVIL_DOM));
    }
    else if(eHistory==E_HISTORY_DATA::D_HIGH_FLOOR){
        return value = MafUtils::toString(info->getHighFloor(E_PLACE::DG_DEVIL_DOM));
    }
    else if(eHistory == E_HISTORY_DATA::D_ARTIFACT_HIGH_ATK)
    {
        UserInfoManager* info = UserInfoManager::getInstance();
        auto vecartifact = DataManager::getInstance()->_vecDevilArtifact[0];
        std::string strmax = "";
        for(int i = 0; i < vecartifact.size(); i++)
        {
            if(info->isArtifactHaveDevil(vecartifact.at(i)->_idx) == true)
               {
                   strmax = vecartifact.at(i)->_title;
               }
        }
        return  strmax;
        
    }
    else if(eHistory == E_HISTORY_DATA::D_ARTIFACT_HIGH_CRI)
    {
        
        UserInfoManager* info = UserInfoManager::getInstance();
        auto vecartifact = DataManager::getInstance()->_vecDevilArtifact[1];
        std::string strmax = "";
        for(int i = 0; i < vecartifact.size(); i++)
        {
            if(info->isArtifactHaveDevil(vecartifact.at(i)->_idx) == true)
            {
                strmax = vecartifact.at(i)->_title;
            }
        }
        return  strmax;
        
    }
    else if(eHistory == E_HISTORY_DATA::D_ARTIFACT_HIGH_GOLD)
    {
        
        UserInfoManager* info = UserInfoManager::getInstance();
        auto vecartifact = DataManager::getInstance()->_vecDevilArtifact[2];
        std::string strmax = "";
        for(int i = 0; i < vecartifact.size(); i++)
        {
            if(info->isArtifactHaveDevil(vecartifact.at(i)->_idx) == true)
            {
                strmax = vecartifact.at(i)->_title;
            }
        }
        return  strmax;
        
    }
    else if(eHistory == E_HISTORY_DATA::D_ARTIFACT_HIGH_ATKSPD)
    {
        
        UserInfoManager* info = UserInfoManager::getInstance();
        auto vecartifact = DataManager::getInstance()->_vecDevilArtifact[3];
        std::string strmax = "";
        for(int i = 0; i < vecartifact.size(); i++)
        {
            if(info->isArtifactHaveDevil(vecartifact.at(i)->_idx) == true)
            {
                strmax = vecartifact.at(i)->_title;
            }
        }
        return  strmax;
        
    }
    else if(eHistory == E_HISTORY_DATA::D_ARTIFACT_HIGH_MOVESPD)
    {
        
        UserInfoManager* info = UserInfoManager::getInstance();
        auto vecartifact = DataManager::getInstance()->_vecDevilArtifact[4];
        std::string strmax = "";
        for(int i = 0; i < vecartifact.size(); i++)
        {
            if(info->isArtifactHaveDevil(vecartifact.at(i)->_idx) == true)
            {
                strmax = vecartifact.at(i)->_title;
            }
        }
        return  strmax;
        
    }
    
    
    UserDefault *user = UserDefault::getInstance();
    str << "his_" << (int)eHistory;
    value = user->getStringForKey(str.str().c_str(), "0");

    if(eHistory==E_HISTORY_DATA::K_START_DAY){
        if(value.compare("0") == 0){
            value = "-";
        }
    }
    
    
    return value;
}

std::string SaveManager::getHistoryKey(E_HISTORY_DATA eHistory)
{
    std::string key;
    
    if(eHistory == E_HISTORY_DATA::K_ATK_SEC)
        key = GAME_TEXT("t_ui_stat_atk");
    else if(eHistory == E_HISTORY_DATA::K_CRI_SEC)
        key = GAME_TEXT("t_ui_stat_cri");//"치명타확률";
    else if(eHistory == E_HISTORY_DATA::K_QUEST_SEC)
        key = GAME_TEXT("t_ui_stat_quest");//"퀘스트보상";
    else if(eHistory == E_HISTORY_DATA::K_REWARD_SEC)
        key = GAME_TEXT("t_ui_stat_reward");//"적처리보상";
    else if(eHistory == E_HISTORY_DATA::K_MOVE)
        key = GAME_TEXT("t_ui_stat_move");//"이동속도";
    
    else if(eHistory == E_HISTORY_DATA::K_PRISON_METER)
        key = GAME_TEXT("t_ui_stat_prison_meter");//"지하감옥 최대거리";
    else if(eHistory == E_HISTORY_DATA::K_PRISON_COUNT)
        key = GAME_TEXT("t_ui_stat_prison_count");//"지하감옥 최대입장";
    else if(eHistory == E_HISTORY_DATA::K_RAID_TOTAL_DMG)
        key = GAME_TEXT("t_ui_stat_raid_total_dmg");//"레이드 누적데미지";
    else if(eHistory == E_HISTORY_DATA::K_RAID_HIGH_DMG)
        key = GAME_TEXT("t_ui_stat_raid_high_dmg");//"레이드 최고데미지";
    else if(eHistory == E_HISTORY_DATA::K_VIP_DAY)
        key = GAME_TEXT("t_ui_stat_vip_count");//"출석체크 횟수";
    else if(eHistory == E_HISTORY_DATA::K_GIFT_COUNT)
        key = GAME_TEXT("t_ui_stat_gift_ui_count");//"선물수령 횟수";
    else if(eHistory == E_HISTORY_DATA::K_BUFF_COUNT)
        key = GAME_TEXT("t_ui_stat_buff_count");//"버프구매 횟수";
    
    else if(eHistory == E_HISTORY_DATA::K_REVIVAL || eHistory == E_HISTORY_DATA::D_REVIVAL)
        key = GAME_TEXT("t_ui_stat_revival_1");//"환생";
    else if(eHistory == E_HISTORY_DATA::K_REVIVAL_2 || eHistory == E_HISTORY_DATA::D_REVIVAL_2)
        key = GAME_TEXT("t_ui_stat_revival_2");//"강화환생";
    else if(eHistory == E_HISTORY_DATA::K_REVIVAL_3 || eHistory == E_HISTORY_DATA::D_REVIVAL_3)
        key = GAME_TEXT("t_ui_stat_revival_3");//"초강화환생";
    else if(eHistory == E_HISTORY_DATA::K_HIGH_FLOOR || eHistory == E_HISTORY_DATA::D_HIGH_FLOOR)
        key = GAME_TEXT("t_ui_stat_high_floor");//"최고층";
    else if(eHistory == E_HISTORY_DATA::K_TOTAL_FLOOR)
        key = GAME_TEXT("t_ui_stat_total_floor");//"누적층";
    else if(eHistory == E_HISTORY_DATA::K_TOTAL_GEM)
        key = GAME_TEXT("t_ui_stat_total_gem");//"누적보석";
    else if(eHistory == E_HISTORY_DATA::K_TOTAL_KEY)
        key = GAME_TEXT("t_ui_stat_total_key");//"누적열쇠";
    
    else if(eHistory == E_HISTORY_DATA::K_WEAPON_HIGH || eHistory == E_HISTORY_DATA::D_WEAPON_HIGH)
        key = GAME_TEXT("t_ui_stat_high_weapon");//"최고무기";
    else if(eHistory == E_HISTORY_DATA::K_MINE_HIGH)
        key = GAME_TEXT("t_ui_stat_high_mine");//"광산 최대거리";
    
    else if(eHistory == E_HISTORY_DATA::K_REBORN_KEY)
        key = GAME_TEXT("t_ui_stat_high_rewardkey");//"환생 최대열쇠";
    else if(eHistory == E_HISTORY_DATA::K_ONE_SHOT)
        key = GAME_TEXT("t_ui_stat_high_onekill");//"원킬최고층";
    else if(eHistory == E_HISTORY_DATA::K_START_DAY)
        key = GAME_TEXT("t_ui_stat_start_ui_day");//"모험의시작";
    else if(eHistory == E_HISTORY_DATA::K_PLAY_TIME)
        key = GAME_TEXT("t_ui_stat_play_day");//"봉식인생";
    else if(eHistory == E_HISTORY_DATA::K_VIDEO_COUNT)
        key = GAME_TEXT("t_ui_stat_video_count");//"동영상 횟수";
    
    else if(eHistory == E_HISTORY_DATA::K_LAST_DEVICE_TIME)
        key = GAME_TEXT("t_ui_stat_device_time");//"타임머신";
    
    else if(eHistory == E_HISTORY_DATA::D_HEAD_HIGH)
        key = GAME_TEXT("t_ui_stat_high_head");
    else if(eHistory == E_HISTORY_DATA::D_BODY_HIGH)
        key = GAME_TEXT("t_ui_stat_high_body");
    else if(eHistory == E_HISTORY_DATA::D_CAPE_HIGH)
        key = GAME_TEXT("t_ui_stat_high_cape");
    else if(eHistory == E_HISTORY_DATA::D_PET_LEVEL_1)
        key = GAME_TEXT("t_ui_stat_pet_level_1");
    else if(eHistory == E_HISTORY_DATA::D_PET_LEVEL_2)
        key = GAME_TEXT("t_ui_stat_pet_level_2");
    else if(eHistory == E_HISTORY_DATA::D_PET_LEVEL_3)
        key = GAME_TEXT("t_ui_stat_pet_level_3");
    else if(eHistory == E_HISTORY_DATA::D_PET_LEVEL_4)
        key = GAME_TEXT("t_ui_stat_pet_level_4");
    
    else if(eHistory == E_HISTORY_DATA::D_ARTIFACT_HIGH_ATK)
        key = GAME_TEXT("t_ui_stat_high_arti_atk");
    else if(eHistory == E_HISTORY_DATA::D_ARTIFACT_HIGH_CRI)
        key = GAME_TEXT("t_ui_stat_high_arti_cri");
    else if(eHistory == E_HISTORY_DATA::D_ARTIFACT_HIGH_GOLD)
        key = GAME_TEXT("t_ui_stat_high_arti_gold");
    else if(eHistory == E_HISTORY_DATA::D_ARTIFACT_HIGH_ATKSPD)
        key = GAME_TEXT("t_ui_stat_high_arti_atkspd");
    else if(eHistory == E_HISTORY_DATA::D_ARTIFACT_HIGH_MOVESPD)
        key = GAME_TEXT("t_ui_stat_high_arti_movespd");
    
//    else if(eHistory == E_HISTORY_DATA::)
//        key = "";
    
    
    return key;
}
#pragma mark - Destroy All Data
void SaveManager::destroyAllUserData()
{
    cocos2d::Director::getInstance()->pause();
    auto user = UserDefault::getInstance();
    
    user->deleteValueForKey(KEY_HACK_SIZE);
    user->deleteValueForKey(KEY_HACK_HASH);
    user->deleteValueForKey(KEY_HACK_STORE);
    user->deleteValueForKey(KEY_USER_AGREE);
    user->deleteValueForKey(KEY_GOLD);
    user->deleteValueForKey(KEY_KEY);
    user->deleteValueForKey(KEY_GEM);
    user->deleteValueForKey(KEY_POINT);
    user->deleteValueForKey(KEY_CUBE);
    user->deleteValueForKey(KEY_MYTHRIL);
    user->deleteValueForKey(KEY_LENGTH_GOLD);
    user->deleteValueForKey(KEY_LENGTH_KEY);
    user->deleteValueForKey(KEY_LENGTH_GEM);
    user->deleteValueForKey(KEY_TOWN_LEVEL);
    user->deleteValueForKey(KEY_TOWN_BUILDING_PROGRESS);
    user->deleteValueForKey(KEY_TOWN_SATELLITE_STEP);
    user->deleteValueForKey(KEY_TOWN_SATELLITE_TUTORIAL);
    user->deleteValueForKey(KEY_COSTUME_EQUIP);
    user->deleteValueForKey(KEY_COSTUME_BUY);
    user->deleteValueForKey(KEY_COSTUME_AURA_EQUIP);
    user->deleteValueForKey(KEY_COSTUME_AURA_BUY);
    user->deleteValueForKey(KEY_COSTUME_RENT_LIST);
    user->deleteValueForKey(KEY_COSTUME_RENT_TIME);
    user->deleteValueForKey(KEY_COSTUME_SOCKET);
    user->deleteValueForKey(KEY_COSTUME_COUPON);
    user->deleteValueForKey(KEY_COSTUME_SKIN);
    user->deleteValueForKey(KEY_COSTUME_SKIN_BUY);
    user->deleteValueForKey(KEY_MONSTER_SOUL_PIECE);
    user->deleteValueForKey(KEY_SERVER_MISSION_SEASON_WEEKLY);
    user->deleteValueForKey(KEY_SERVER_MISSION_SEASON_DAILY);
    user->deleteValueForKey(KEY_SERVER_MISSION_DAILY);
    user->deleteValueForKey(KEY_SERVER_MISSION_WEEKLY);
    user->deleteValueForKey(KEY_INFINITE_ARTI);
    user->deleteValueForKey(KEY_ACHIEVE_NORMAL_COUNT);
    user->deleteValueForKey(KEY_ACHIEVE_NORMAL_COMPLETE);
    user->deleteValueForKey(KEY_TIMING_PACKAGE_FREE_COMPLETE);
    user->deleteValueForKey(KEY_ACHIEVE_NORMAL_GRADE);
    user->deleteValueForKey(KEY_MINE_PICKAX);
    user->deleteValueForKey(KEY_MINE_MINERAL);
    user->deleteValueForKey(KEY_MINE_TIME);
    user->deleteValueForKey(KEY_MINE_BUY_MINERAL);
    user->deleteValueForKey(KEY_MINE_SPECIAL);
    user->deleteValueForKey(KEY_MINE_REFINE_OPEN);
    user->deleteValueForKey(KEY_MINE_REFINE_BUY_RESETTIME);
    user->deleteValueForKey(KEY_MINE_REFINE_BUY_COUNT);
    user->deleteValueForKey(KEY_MINE_REFINE_PRODUCTION_COUNT);
    user->deleteValueForKey(KEY_MINE_REFINE_PRODUCTION_STARTTIME);
    user->deleteValueForKey(KEY_MINE_REFINE_PRODUCTION_ITEM);
    user->deleteValueForKey(KEY_MINE_REFINE_UPGRADE_LEVEL);
    user->deleteValueForKey(KEY_HITCOUNT_NORMAL);
    user->deleteValueForKey(KEY_HITCOUNT_DUNGEON);
    user->deleteValueForKey(KEY_HITCOUNT_HONG_NORMAL);
    user->deleteValueForKey(KEY_HITCOUNT_HONG_DUNGEON);
    user->deleteValueForKey(KEY_FLOOR);
    user->deleteValueForKey(KEY_FLOOR_COUNT);
    user->deleteValueForKey(KEY_EQUIP);
    user->deleteValueForKey(KEY_EQUIP_VISUAL);
    user->deleteValueForKey(KEY_EQUIP_HIGH);
    user->deleteValueForKey(KEY_EQUIP_REBORN);
    user->deleteValueForKey(KEY_EQUIP_PERMANENT);
    user->deleteValueForKey(KEY_EQUIP_LEVEL);
    user->deleteValueForKey(KEY_EQUIP_REINFORCE);
    user->deleteValueForKey(KEY_ENCHANT_STONE);
    user->deleteValueForKey(KEY_ENCHANT_STONE_LOCAL);
    user->deleteValueForKey(KEY_ENCHANT_STONE_STORAGE);
    user->deleteValueForKey(KEY_MONSTER_SOUL);
    user->deleteValueForKey(KEY_MONSTER_SOUL_LEVEL);
    user->deleteValueForKey(KEY_ARTIFACT_LEVEL);
    user->deleteValueForKey(KEY_ARTIFACT_REINFORCE);
    user->deleteValueForKey(KEY_RELIC_LEVEL);
    user->deleteValueForKey(KEY_JELLY_NEW);
    user->deleteValueForKey(KEY_HONOR);
    user->deleteValueForKey(KEY_PET_LEVEL);
    user->deleteValueForKey(KEY_PET_REINFORCE_LEVEL);
    user->deleteValueForKey(KEY_PET_EVOLUTION);
    user->deleteValueForKey(KEY_PET_EVOLUTION_REWARD);
    user->deleteValueForKey(KEY_PET_SKIN);
    user->deleteValueForKey(KEY_PET_SKIN_BUY);
    user->deleteValueForKey(KEY_PET_BAFOMETZ);
    user->deleteValueForKey(KEY_QUEST_LEVEL);
    user->deleteValueForKey(KEY_TIME_ATTACK_9_BONUS);
    user->deleteValueForKey(KEY_TIME_ATTACK_10_BONUS);
    user->deleteValueForKey(KEY_TIME_ATTACK_BONUS);
    user->deleteValueForKey(KEY_TIME_COIN_BONUS);
    user->deleteValueForKey(KEY_TIME_MOVE_BONUS);
    user->deleteValueForKey(KEY_COUNT_KEY_BONUS);
    user->deleteValueForKey(KEY_COUNT_HERO_BONUS);
    user->deleteValueForKey(KEY_TIME_ADS_GOLD_BONUS);
    user->deleteValueForKey(KEY_TIME_ADS_GEM_BONUS);
    user->deleteValueForKey(KEY_TIME_ADS_BUFF_BONUS);
    user->deleteValueForKey(KEY_TIME_ADS_MINE_BONUS);
    user->deleteValueForKey(KEY_TIME_ADS_SELECT_BONUS);
    user->deleteValueForKey(KEY_TIME_ADS_BUFF_ATK);
    user->deleteValueForKey(KEY_TIME_ADS_BUFF_GOLD);
    user->deleteValueForKey(KEY_TIME_ADS_BUFF_MOVE);
    user->deleteValueForKey(KEY_TIME_ADS_BUFF_D_ATK);
    user->deleteValueForKey(KEY_TIME_ADS_BUFF_D_ASPD);
    user->deleteValueForKey(KEY_TIME_ADS_BUFF_D_GOLD);
    user->deleteValueForKey(KEY_TIME_ADS_BUFF_D_MOVE);
    user->deleteValueForKey(KEY_TIME_ADS_BUFF_GOLD_NTERSTITIAL);
    user->deleteValueForKey(KEY_TIME_ITEM_FEVER);
    user->deleteValueForKey(KEY_TIME_ITEM_FEVER_COUNT);
    user->deleteValueForKey(DATA_HIGH_FLOOR);
    user->deleteValueForKey(DATA_TOTAL_FLOOR);
    user->deleteValueForKey(DATA_REVIVAL);
    user->deleteValueForKey(DATA_REVIVAL_ADS);
    user->deleteValueForKey(KEY_DRAGON_SKILLCHECK);
    user->deleteValueForKey(KEY_REVIVAL_ATK_BUFF);
    user->deleteValueForKey(KEY_REVIVAL_QUEST_BUFF);
    user->deleteValueForKey(KEY_REVIVAL_KEY_BUFF1);
    user->deleteValueForKey(KEY_REVIVAL_KEY_BUFF2);
    user->deleteValueForKey(KEY_E_REVIVAL_ATK_BUFF);
    user->deleteValueForKey(KEY_HIT_BUFF);
    user->deleteValueForKey(KEY_DRAGON_BUFF);
    user->deleteValueForKey(KEY_HELL_BUFF);
    user->deleteValueForKey(KEY_QUEST_25_BUFF);
    user->deleteValueForKey(KEY_REVIVE_SPECIAL);
    user->deleteValueForKey(KEY_REVIVE_KEYBOOSTER);
    user->deleteValueForKey(KEY_SPECIAL_CLEAR);
    user->deleteValueForKey(KEY_SPECIAL_COMPLETE);
    user->deleteValueForKey(KEY_ATTEND_VIP);
    user->deleteValueForKey(KEY_ATTEND_DAY);
    user->deleteValueForKey(KEY_ATTEND_ADS);
    user->deleteValueForKey(KEY_MYHOME_LEVEL);
    user->deleteValueForKey(KEY_MYHOME_WATCHCOUNT);
    user->deleteValueForKey(KEY_MYHOME_COUNT);
    user->deleteValueForKey(KEY_TIME_ADS_MYHOME_BUFF);
    user->deleteValueForKey(KEY_TIME_ADS_MYHOME_GEM);
    user->deleteValueForKey(KEY_TIME_ADS_MYHOME_GOLD);
    user->deleteValueForKey(KEY_TIME_ADS_MYHOME_CUBE);
    user->deleteValueForKey(KEY_TIME_ADS_MYHOME_COSTUMECOIN);
    user->deleteValueForKey(KEY_TIME_ADS_MYHOME_MINE);
    user->deleteValueForKey(KEY_TIME_ADS_MYHOME_BONTRIUM);
    user->deleteValueForKey(KEY_MONSTER_HP);
    user->deleteValueForKey(KEY_R_S_MAX_LV);
    user->deleteValueForKey(KEY_R_S_MAX_DAMAGE);
    user->deleteValueForKey(KEY_PURCHASE_COUNT);
    user->deleteValueForKey(KEY_PURCHASE_HISTORY);
    user->deleteValueForKey(KEY_PURCHASE_ADS);
    user->deleteValueForKey(KEY_REVIVE_POINT);
    user->deleteValueForKey(KEY_REVIVE_FEVER_DAY);
    user->deleteValueForKey(KEY_REVIVE_CUBE_DAY);
    user->deleteValueForKey(KEY_REVIVE_BUFF);
    user->deleteValueForKey(KEY_COMIC_SHOW);
    user->deleteValueForKey(KEY_TODAY);
    user->deleteValueForKey(KEY_HISTORY_DATA);
    user->deleteValueForKey(KEY_PLACE);
    user->deleteValueForKey(KEY_D_GOLD);
    user->deleteValueForKey(KEY_D_KEY);
    user->deleteValueForKey(KEY_D_QUEST_LEVEL);
    user->deleteValueForKey(KEY_D_QUEST_TIME);
    user->deleteValueForKey(KEY_D_EQUIP);
    user->deleteValueForKey(KEY_D_EQUIP_LEVEL);
    user->deleteValueForKey(KEY_D_EQUIP_REINFORCE);
    user->deleteValueForKey(KEY_D_PET_LEVEL);
    user->deleteValueForKey(KEY_D_PET_GRADE);
    user->deleteValueForKey(KEY_D_ARTIFACT);
    user->deleteValueForKey(KEY_D_TIME_BUFF);
    user->deleteValueForKey(KEY_D_TIME_ADS);
    user->deleteValueForKey(KEY_D_HIT_COUNT_HEDGEHOG);
    user->deleteValueForKey(KEY_D_HIT_COUNT_FLASH);
    user->deleteValueForKey(DATA_D_HIGH_FLOOR);
    user->deleteValueForKey(DATA_D_TOTAL_FLOOR);
    user->deleteValueForKey(KEY_D_FLOOR);
    user->deleteValueForKey(KEY_D_FLOOR_COUNT);
    user->deleteValueForKey(KEY_D_REVIVAL);
    user->deleteValueForKey(KEY_D_MONSTER_HP);
    user->deleteValueForKey(KEY_SAVE_TIME);
    user->deleteValueForKey(KEY_TUTORIAL_PROGRESS);
    user->deleteValueForKey(KEY_TIME_TRAVEL_LEVEL);
    user->deleteValueForKey(KEY_TIME_TRAVEL_COUNT);
    user->deleteValueForKey(KEY_TIME_TRAVEL_TIME);
    user->deleteValueForKey(KEY_ADVT_OFFICE_LV);
    user->deleteValueForKey(KEY_ADVT_TUTORIAL_STEP);
    user->deleteValueForKey(KEY_ADVT_TUTORIAL_FRIEND);
    user->deleteValueForKey(KEY_ADVT_TUTORIAL_NEW_CORE);
    user->deleteValueForKey(KEY_ADVT_AVILITY_USE_GEM);
    user->deleteValueForKey(KEY_LOGIN_GAMECENTER_ID);
    user->deleteValueForKey(KEY_LOGIN_GOOGLEPLAY_ID);
    user->deleteValueForKey(KEY_LOGIN_GUEST);
    user->deleteValueForKey(KEY_CLOUD_VER);
    user->deleteValueForKey(KEY_DF_HIGH_FLOOR);
    user->deleteValueForKey(KEY_DF_CURRENT_FLOOR);
    user->deleteValueForKey(KEY_DF_COIN);
    user->deleteValueForKey(KEY_DF_WEAPON_LV);
    user->deleteValueForKey(KEY_DF_CHARACTER_LV);
    
    // 보물 결제
    for(int i=E_ARTIFACT::ARTI_1_EVOL1; i< E_ARTIFACT::E_ARTIFACT_999_NONE; i++)
    {
        std::string str = "";
        
        str = MafUtils::format("pack_%d",i).c_str();
        user->deleteValueForKey(str.c_str());
        
        str = MafUtils::format("pack_e_%d",i).c_str();
        user->deleteValueForKey(str.c_str());
    }
    
    //무한 보물 결제
    for(int i=E_ARTIFACT::ARTI_1_EVOL1; i< E_ARTIFACT::E_ARTIFACT_999_NONE; i++)
    {
        std::string str = "";
        for(int j = 1; j <= INFINITE_ARTIFACT_KIND; ++j)
        {
            str = MafUtils::format("Infinite_pack_%d_%d",i, j).c_str();
            user->deleteValueForKey(str.c_str());
        }
    }
    
    //모험단 파티 데이터
    auto list = AdventureManager::getInstance()->getListInfoParty();
    int nPresetCount = 3;
    auto objSettingPresetCount = AdventureManager::getInstance()->getSetting("party_ability_preset_count");
    if(objSettingPresetCount != nullptr)
        nPresetCount = objSettingPresetCount->getValueToInt();
    
    for(int i = 0; i < list.size(); ++i)
    {
        std::string str_lv = MafUtils::format("advt_p_%d_m_lv", i+1).c_str();
        std::string str_preset_idx = MafUtils::format("advt_p_%d_a_p", i+1).c_str();
        
        for(int j = 0; j < nPresetCount; ++j)
        {
            std::string str_type = MafUtils::format("advt_p_%d_a_t_%d", i+1, j+1).c_str();
            std::string str_grade = MafUtils::format("advt_p_%d_a_g_%d", i+1, j+1).c_str();
            std::string str_seal = MafUtils::format("advt_p_%d_a_s_%d", i+1, j+1).c_str();
            
            user->deleteValueForKey(str_type.c_str());
            user->deleteValueForKey(str_grade.c_str());
            user->deleteValueForKey(str_seal.c_str());
        }
        
        user->deleteValueForKey(str_lv.c_str());
        user->deleteValueForKey(str_preset_idx.c_str());
    }
    for(int i = (int)E_HISTORY_DATA::K_ATK_SEC; i < (int)E_HISTORY_DATA::END; ++i)
    {
        user->deleteValueForKey(MafUtils::format("his_%d", i).c_str());
    }
    
    user->flush();
    //
    MafHttpNew::_vecRequest.clear();
    
    //
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->unscheduleAllWithMinPriority(Scheduler::PRIORITY_NON_SYSTEM_MIN);
    
    MafGoogleRewardAds::HideGoogleBanner();
    
    UserInfoManager::destroyInstance();
    ItemsMoneyManager::destroyInstance();
    NewRaidManager::destroyInstance();
    InfiniteArtifactManager::destroyInstance();
    PrisonManager::destroyInstance();
    AdventureManager::destroyInstance();
    AdventureRelicManager::destroyInstance();
    WeaponSpiritManager::destroyInstance();
    
    cocos2d::Director::getInstance()->resume();
}

#pragma mark - set, get
time_t SaveManager::getSaveTime()
{
    return _nSaveTime;
}

int SaveManager::getSaveFloorHigh()
{
    return _nSaveFloorHigh;
}

int SaveManager::getSaveFloorTotal()
{
    return _nSaveFloorTotal;
}

#pragma mark - network
void SaveManager::requestSave(const std::function<void(bool)>& pCallback)
{
    //
    _callbackSave = pCallback;
    
    // 저장 시간
    time_t sec = UtilsDate::getInstance()->getTime();
    UserInfoConfigManager::getInstance()->setSaveTime(sec);
    
    //
    std::string url = "/save";
    
    int nCloudVer = 0;
    std::string strCloudVer = UserDefault::getInstance()->getStringForKey(KEY_CLOUD_VER);
    if ( !strCloudVer.empty() )
    {
        strCloudVer = MafAes256::Decrypt(strCloudVer);
        nCloudVer = atoi(strCloudVer.c_str());
    }
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_userid", AccountManager::getInstance()->getUserID().c_str());
    req->setData("_savever", nCloudVer);
    req->setData("_savedata", saveCloudData().c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseSave(response, dd);
    };
    req->send(cb);
}

void SaveManager::responseSave(network::HttpResponse* response,std::string &data)
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
        if ( _callbackSave != nullptr )
        {
            _callbackSave(false);
        }
        return;
    }
    
    bool bResult = jsonParser["_success"].GetBool();
    int nCloudVer = jsonParser["_savever"].GetInt();

    if ( bResult == true )
    {
        //
        std::string strCloudVer = MafUtils::toString(nCloudVer);
        strCloudVer = MafAes256::Encrypt(strCloudVer);
        UserDefault::getInstance()->setStringForKey(KEY_CLOUD_VER, strCloudVer);
        
        // network
        if ( _callbackSave != nullptr )
        {
            _callbackSave(true);
        }
    }
    else
    {
        if ( _callbackSave != nullptr )
        {
            _callbackSave(false);
        }
    }
}

void SaveManager::requestLoad(const std::function<void(bool, bool)>& pCallback)
{
    //
    _callbackLoad = pCallback;
    _strSaveData = "";
    _nSaveTime = 0;
    _nSaveFloorHigh = 0;
    _nSaveFloorTotal = 0;
    
    //
    std::string url = "/v4/load/%s";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserID().c_str());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseLoad(response,dd);
    };
    req->send(cb);
}

void SaveManager::responseLoad(network::HttpResponse* response,std::string &data)
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
        if ( _callbackLoad != nullptr )
        {
            _callbackLoad(false, false);
        }
        return;
    }
    
    bool bResult = jsonParser["_success"].GetBool();
    int nCloudVer = jsonParser["_savever"].GetInt();

    if ( bResult == true )
    {
        //
        std::string strCloudVer = MafUtils::toString(nCloudVer);
        strCloudVer = MafAes256::Encrypt(strCloudVer);
        UserDefault::getInstance()->setStringForKey(KEY_CLOUD_VER, strCloudVer);
        
        //
        _strSaveData = jsonParser["_savedata"].GetString();
        _nSaveTime = jsonParser["_savetime"].GetInt64();
        
        rapidjson::Document jsonParserSave;
        jsonParserSave.Parse<0>((char*)_strSaveData.c_str());
        if ( jsonParserSave.HasParseError() == false )
        {
            if ( jsonParserSave.HasMember(DATA_HIGH_FLOOR) )
            {
                std::string strTemp = jsonParserSave[DATA_HIGH_FLOOR].GetString();
                _nSaveFloorHigh = atoi(strTemp.c_str());
            }

            if ( jsonParserSave.HasMember(DATA_TOTAL_FLOOR) )
            {
                std::string strTemp = jsonParserSave[DATA_TOTAL_FLOOR].GetString();
                _nSaveFloorTotal = atoi(strTemp.c_str());
            }
        }
        
        // network
        if ( _callbackLoad != nullptr )
        {
            _callbackLoad(true, true);
        }
    }
    else
    {
        if ( _callbackLoad != nullptr )
        {
            _callbackLoad(true, false);
        }
    }
}

void SaveManager::requestLoadFinish()
{
    //
    std::string url = "/load/complete/%s";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserID().c_str());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        //
    };
    req->send(cb);
}

void SaveManager::requestDestroyUserData(const std::function<void(int)>& pCallback)
{
    _callbackDestroyUserData = pCallback;
    //
    std::string url = "/v0/user/deleteUserAccount";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_userid", AccountManager::getInstance()->getUserID().c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseDestroyUserData(response, dd);
    };
    req->send(cb);
}

void SaveManager::responseDestroyUserData(network::HttpResponse* response,std::string &data)
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
        if(_callbackDestroyUserData != nullptr)
            _callbackDestroyUserData(0);
        return;
    }
    
    int bResult = jsonParser["_result"].GetInt();

    if ( bResult == 0 )
    {
        if(_callbackDestroyUserData != nullptr)
            _callbackDestroyUserData(0);
        return;
    }
    
    if(_callbackDestroyUserData != nullptr)
        _callbackDestroyUserData(bResult);
}
