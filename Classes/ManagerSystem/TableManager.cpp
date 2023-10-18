//
//  TableManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#include "TableManager.h"

#include "Utils/UtilsString.h"

// lib maf
#include "Libs/maf/MafAes256.h"
#include "Libs/maf/MafUtils.h"
#include "Libs/maf/MafHttpNew.h"

USING_NS_CC;

#pragma mark - ObjTable
ObjTable::ObjTable() :
_strName(""),
_strUrl(""),
_nSize(0)
{

}

ObjTable::~ObjTable()
{
    
}

bool ObjTable::init()
{
    return true;
}

#pragma mark - ObjTable : set, get
std::string ObjTable::getName()
{
    return _strName;
}
void ObjTable::setName(std::string strName)
{
    _strName = strName;
}

std::string ObjTable::getUrl()
{
    return _strUrl;
}
void ObjTable::setUrl(std::string strUrl)
{
    _strUrl = strUrl;
}

int64_t ObjTable::getSize()
{
    return _nSize;
}
void ObjTable::setSize(int64_t nSize)
{
    _nSize = nSize;
}


#pragma mark - TableManager
TableManager::TableManager(void) :
_objTableTemp(nullptr),
_strFilePath(""),

_nVersionTemp(0),
_bVersionNew(false),

_callbackInfo(nullptr),
_callbackInfoProgress(nullptr),

_nDownloadTotal(0),
_nDownloadSuccess(0),
_nDownloadError(0)
{
    
}

TableManager::~TableManager(void)
{
    if ( _objTableTemp != nullptr )
    {
        _objTableTemp->release();
        _objTableTemp = nullptr;
    }
}

bool TableManager::init()
{
    // download
    _downloader.reset(new network::Downloader());
    _downloader->onTaskProgress = std::bind(&TableManager::downloadTaskProgress, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    _downloader->onFileTaskSuccess = std::bind(&TableManager::downloadFileSuccess, this, std::placeholders::_1);
    _downloader->onTaskError = std::bind(&TableManager::downloadTaskError, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    
    //
    _strFilePath = FileUtils::getInstance()->getWritablePath() + "Download/Data/";
    if ( FileUtils::getInstance()->isDirectoryExist(_strFilePath) == false )
    {
        FileUtils::getInstance()->createDirectory(_strFilePath);
    }
    
    return true;
}

#pragma mark - load
void TableManager::setLoad()
{
    // file copy
    E_TABLE listTable[] = {
        E_TABLE::LANGUAGE_LIST,
        E_TABLE::LANGUAGE_TALK,
        E_TABLE::LANGUAGE_TALK_TIP,
        E_TABLE::LANGUAGE_TEXT,
        E_TABLE::LANGUAGE_DIALOG_TEXT,
        E_TABLE::LANGUAGE_BADWORD,
        E_TABLE::PARAMETER,
        E_TABLE::REVERSE_APK,
        E_TABLE::EVENT_DATE,
        E_TABLE::STORY,
        E_TABLE::ATTENDANCE_DAILY,
        E_TABLE::ATTENDANCE_MONTHLY,
        E_TABLE::GAME_LIST,
        E_TABLE::ACHIEVEMENTS,
        E_TABLE::ARTIFACT,
        E_TABLE::ITEMS,
        E_TABLE::FROST_PRISON_FLOOR,
        E_TABLE::FROST_PRISON_SKILL,
        E_TABLE::FROST_PRISON_SETTING,
        E_TABLE::PRISON_QUEST,
        E_TABLE::PRISON_HELL_QUEST,
        E_TABLE::PRISON_HELL_TRADING_MARKET,
        E_TABLE::EVENT_COOPERATION_RAID_ACHIEVE,
        E_TABLE::EVENT_COOPERATION_RAID_QUEST,
        E_TABLE::INFINITE_ARTIFACT_INFO,
        E_TABLE::EVENT_LIMIT_COIN_FIXED,
        E_TABLE::EVENT_FISHING_QUEST,
        E_TABLE::EVENT_FISHING_BOARD,
        E_TABLE::EVENT_CO_MISSION_PERSON,
        E_TABLE::EVENT_CO_MISSION,
        E_TABLE::EVENT_VOTE_ALL,
        E_TABLE::EVENT_VOTE_PERSONAL,
        E_TABLE::EVENT_TREASURE_HUNT_MAP,
        E_TABLE::EVENT_TREASURE_HUNT_REWARD,
        E_TABLE::EVENT_SUNFISH,
        E_TABLE::BADGE,
        E_TABLE::PURCHASE,
        E_TABLE::ACHIEVEMENTS_NORMAL,
        E_TABLE::PET_SKIN,
        E_TABLE::WEAPON,
        E_TABLE::COSTUME,
        E_TABLE::COSTUME_SKIN_LIST,
        E_TABLE::COSTUME_SKIN_SHOP,
        E_TABLE::PET_EQUIP_BOX_SHOP,
        E_TABLE::PET_EQUIP_DATA,
        E_TABLE::PET_EQUIP_AMOUNT,
        E_TABLE::EXPEDITION_LOOT,
        E_TABLE::TIMING_PACKAGE,
        E_TABLE::EVENT_BOARDGAME_BOARD,
        E_TABLE::EVENT_BOARDGAME_QUEST,
        E_TABLE::RAID_NEW_QUEST,
        E_TABLE::RAID_DRAGON,
        E_TABLE::RAID_BONG_LEVEL,
        E_TABLE::RAID_MILEAGE_LV,
        E_TABLE::RAID_MILEAGE_REWARD,
        E_TABLE::RAID_BONG_STAT,
        E_TABLE::RAID_BONG_SETTING,
        E_TABLE::RAID_STAT_INCREASE,
        E_TABLE::RAID_RANK_REWARD,
        E_TABLE::WEAPON_SPIRIT,
        E_TABLE::WEAPON_SPIRIT_SMELTING_COST,
        E_TABLE::WEAPON_SPIRIT_DRAW_RATE,
        E_TABLE::EVENT_TRADING_MARKET_REWARD,
        E_TABLE::EVENT_TRADING_MARKET_SETTING,
        E_TABLE::WEAPON_SPIRIT_BUFF,
        E_TABLE::WEAPON_SPIRIT_SYNERGY,
        E_TABLE::RAID_TRADING_MARKET,
        E_TABLE::ADVENTURE_BOSS,
        E_TABLE::ADVENTURE_PARTY_ABILITY,
        E_TABLE::ADVENTURE_PARTY_MEMBER,
        E_TABLE::ADVENTURE_FURNITURE,
        E_TABLE::ADVENTURE_SETTING,
        E_TABLE::ADVENTURE_PROPERTY_TYPE,
        E_TABLE::ADVENTURE_MAP,
        E_TABLE::ADVENTURE_BONG_LEVEL,
        E_TABLE::ADVENTURE_RELIC,
        E_TABLE::ADVENTURE_RELIC_LEVEL,
        E_TABLE::ADVENTURE_CORE_LEVEL,
        E_TABLE::ADVENTURE_RELIC_COLLECT,
        E_TABLE::ADVENTURE_FRIEND_ABILITY,
        E_TABLE::ADVENTURE_RELIC_RATE,
        E_TABLE::ADVENTURE_MISSION,
        E_TABLE::GROWTH_PACK_REWARD,
        E_TABLE::DIALOG_SCRIPT,
        E_TABLE::DIALOG_LIST,
        E_TABLE::DIALOG_ACTOR,
        E_TABLE::EVENT_RACE_STAT,
        E_TABLE::EVENT_RACE_RANK_REWARD,
        E_TABLE::EVENT_RACE_MISSION,
        E_TABLE::TOWN_REWARD,
        E_TABLE::DAILY_WEEKLY_MISSION,
        E_TABLE::SPECIAL_MISSION,
        E_TABLE::EVENT_LUCKYBAG_REWARD,
        E_TABLE::MINE_REFINE_SETTING,
        E_TABLE::FLASHMAN_MISSION,
        E_TABLE::FLASHMAN_RANK,
        E_TABLE::FLASHMAN_SETTING,
        E_TABLE::DEFENSE_MAP,
        E_TABLE::DEFENSE_CHARACTER,
        E_TABLE::DEFENSE_WEAPON,
        E_TABLE::DEFENSE_BULLET,
        E_TABLE::DEFENSE_MONSTER,
        E_TABLE::DEFENSE_FLOOR,
        E_TABLE::DEFENSE_SETTING,
        E_TABLE::DEFENSE_GATCHA_LEVEL,
        E_TABLE::DEFENSE_GATCHA_RATE,
        E_TABLE::DEFENSE_MANA_AMP_STONE,
    };
    for ( int i = 0; i < sizeof(listTable)/sizeof(E_TABLE); i++ )
    {
        std::string strName = getTableName(listTable[i]);
        
        std::string strFileNameOriginal = strName + ".dat";
        std::string strFileNameDuplication = getTableFileName(strName);
        std::string strFileNameDuplicationTemp = getTableFileNameTemp(strName);
        
        if ( _bVersionNew == true || FileUtils::getInstance()->isFileExist(_strFilePath+strFileNameDuplication) == false )
        {
            if ( FileUtils::getInstance()->isFileExist("DataServer/"+strFileNameOriginal) == true )
            {
                std::string strText = FileUtils::getInstance()->getStringFromFile("DataServer/"+strFileNameOriginal);
                FileUtils::getInstance()->writeStringToFile(strText, _strFilePath+strFileNameDuplication);
                FileUtils::getInstance()->writeStringToFile(strText, _strFilePath+strFileNameDuplicationTemp);
            }
        }
    }
    
    if ( _bVersionNew == true )
    {
        _bVersionNew = false;
        UserDefault::getInstance()->deleteValueForKey("table_version_live");
        UserDefault::getInstance()->deleteValueForKey("table_version_dev");
    }
    
    
    // temp 파일 사이즈 비교 해서 다시 받도록 설정
    for ( int i = 0; i < sizeof(listTable)/sizeof(E_TABLE); i++ )
    {
        std::string strName = getTableName(listTable[i]);
        std::string strFileName = getTableFileName(strName);
        std::string strFileNameTemp = getTableFileNameTemp(strName);
        
        int64_t nSizeOriginal = FileUtils::getInstance()->getFileSize(_strFilePath+strFileName);
        int64_t nSizeTemp = FileUtils::getInstance()->getFileSize(_strFilePath+strFileNameTemp);
        
        if ( nSizeOriginal == -1 || nSizeOriginal != nSizeTemp )
        {
            setVersion(0);
            break;
        }
    }
}

#pragma mark - set, get, add
int TableManager::getVersion()
{
    int nVersionLive = UserDefault::getInstance()->getIntegerForKey("table_version_live", 1273);
    int nVersionDev = UserDefault::getInstance()->getIntegerForKey("table_version_dev", 160);
    int nVersion = 0;
    #if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    {// dev
        nVersion = nVersionDev;
    }
    #else
    {// live
        nVersion = nVersionLive;
    }
    #endif
    
    return nVersion;
}
void TableManager::setVersion(int nVersion)
{
    std::string strKey = "table_version_live";
    #if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    {// dev
        strKey = "table_version_dev";
    }
    #endif
    
    UserDefault::getInstance()->setIntegerForKey(strKey.c_str(), nVersion);
}

void TableManager::setVersionNew(bool bNew)
{
    _bVersionNew = bNew;
}

std::string TableManager::getTableName(E_TABLE eType)
{
    std::string strName = "";
    
    switch (eType) {
        case E_TABLE::LANGUAGE_LIST:                            strName = "info_language_list";         break;
        case E_TABLE::LANGUAGE_TALK:                            strName = "info_language_talk";         break;
        case E_TABLE::LANGUAGE_TALK_TIP:                        strName = "info_language_talk_tip";     break;
        case E_TABLE::LANGUAGE_TEXT:                            strName = "info_language_text";         break;
        case E_TABLE::LANGUAGE_DIALOG_TEXT:                     strName = "info_language_dialog";         break;
        case E_TABLE::LANGUAGE_BADWORD:                         strName = "info_language_badword";      break;
        case E_TABLE::PARAMETER:                                strName = "info_parameter";             break;
        case E_TABLE::REVERSE_APK:                              strName = "info_reverse_apk";           break;
        case E_TABLE::EVENT_DATE:                               strName = "info_event_date";            break;
        case E_TABLE::STORY:                                    strName = "info_story";                 break;
        case E_TABLE::ATTENDANCE_DAILY:                         strName = "info_attendance_daily_vip"; break;
        case E_TABLE::ATTENDANCE_MONTHLY:                       strName = "info_attendance_monthly_season"; break;
        case E_TABLE::GAME_LIST:                                strName = "info_game_list";             break;
        case E_TABLE::ACHIEVEMENTS:                             strName = "info_achievements";          break;
        case E_TABLE::ARTIFACT:                                 strName = "info_artifact";              break;
        case E_TABLE::ITEMS:                                    strName = "info_items";                 break;
        case E_TABLE::FROST_PRISON_FLOOR:                       strName = "info_frost_prison_floor";    break;
        case E_TABLE::FROST_PRISON_SKILL:                       strName = "info_frost_prison_skill";    break;
        case E_TABLE::FROST_PRISON_SETTING:                     strName = "info_frost_prison_setting";  break;
        case E_TABLE::PRISON_QUEST:                             strName = "info_prison_quest";          break;
        case E_TABLE::PRISON_HELL_QUEST:                        strName = "info_prison_hell_quest";     break;
        case E_TABLE::PRISON_HELL_TRADING_MARKET:               strName = "info_prison_hell_trading_market"; break;
        case E_TABLE::EVENT_COOPERATION_RAID_ACHIEVE:           strName = "info_event_cooperation_raid_achieve"; break;
        case E_TABLE::EVENT_COOPERATION_RAID_QUEST:             strName = "info_event_cooperation_raid_quest"; break;
        case E_TABLE::INFINITE_ARTIFACT_INFO:                   strName = "info_artifact_beyond_v2"; break;
        case E_TABLE::EVENT_LIMIT_COIN_FIXED:                   strName = "info_event_limit_coin_fixed"; break;
        case E_TABLE::EVENT_FISHING_QUEST:                      strName = "info_event_fishing_quest"; break;
        case E_TABLE::EVENT_FISHING_BOARD:                      strName = "info_event_fishing_board_reward"; break;
        case E_TABLE::EVENT_CO_MISSION_PERSON:                  strName = "info_event_co_m_person"; break;
        case E_TABLE::EVENT_CO_MISSION:                         strName = "info_event_co_mission"; break;
        case E_TABLE::EVENT_VOTE_ALL:                           strName = "info_event_vote_all"; break;
        case E_TABLE::EVENT_VOTE_PERSONAL:                      strName = "info_event_vote_personal"; break;
        case E_TABLE::EVENT_TREASURE_HUNT_MAP:                  strName = "info_event_treasure_hunt_map"; break;
        case E_TABLE::EVENT_TREASURE_HUNT_REWARD:               strName = "info_event_treasure_hunt_reward"; break;
        case E_TABLE::EVENT_SUNFISH:                            strName = "info_event_sunfish"; break;
        case E_TABLE::BADGE:                                    strName = "info_badge"; break;
        case E_TABLE::PURCHASE:                                 strName = "info_purchase"; break;
        case E_TABLE::ACHIEVEMENTS_NORMAL:                      strName = "info_achievements_normal"; break;
        case E_TABLE::PET_SKIN:                                 strName = "info_pet_costume"; break;
        case E_TABLE::WEAPON:                                   strName = "info_weapon_v2"; break;
        case E_TABLE::COSTUME:                                  strName = "info_costume"; break;
        case E_TABLE::COSTUME_SKIN_LIST:                        strName = "info_skin_list"; break;
        case E_TABLE::COSTUME_SKIN_SHOP:                        strName = "info_skin_shop"; break;
        case E_TABLE::PET_EQUIP_BOX_SHOP:                       strName = "info_pet_equip_box_shop"; break;
        case E_TABLE::PET_EQUIP_DATA:                           strName = "info_pet_equip"; break;
        case E_TABLE::PET_EQUIP_AMOUNT:                         strName = "info_pet_equip_level"; break;
        case E_TABLE::EXPEDITION_LOOT:                          strName = "info_expedition_cat_loot"; break;
        case E_TABLE::TIMING_PACKAGE:                           strName = "info_timing_package"; break;
        case E_TABLE::RAID_NEW_QUEST:                           strName = "info_new_raid_quest"; break;
        case E_TABLE::RAID_DRAGON:                              strName = "info_new_raid_dragon_hp"; break;
        case E_TABLE::RAID_BONG_LEVEL:                          strName = "info_new_raid_exp"; break;
        case E_TABLE::RAID_MILEAGE_LV:                          strName = "info_new_raid_mileage_lv"; break;
        case E_TABLE::RAID_MILEAGE_REWARD:                      strName = "info_new_raid_mileage_reward"; break;
        case E_TABLE::RAID_BONG_STAT:                           strName = "info_new_raid_stat"; break;
        case E_TABLE::RAID_BONG_SETTING:                        strName = "info_new_raid_setting"; break;
        case E_TABLE::RAID_STAT_INCREASE:                       strName = "info_new_raid_stat_increase"; break;
        case E_TABLE::RAID_RANK_REWARD:                         strName = "info_new_raid_rank_reward"; break;
        case E_TABLE::WEAPON_SPIRIT:                            strName = "info_new_raid_weapon_spirit"; break;
        case E_TABLE::WEAPON_SPIRIT_SMELTING_COST:              strName = "info_new_raid_smelting_cost_spirit"; break;
        case E_TABLE::WEAPON_SPIRIT_DRAW_RATE:                  strName = "info_new_raid_draw_rate";break;
        case E_TABLE::EVENT_TRADING_MARKET_REWARD:              strName = "info_event_trading_market_reward"; break;
        case E_TABLE::EVENT_TRADING_MARKET_SETTING:             strName = "info_event_trading_market_setting"; break;
        case E_TABLE::WEAPON_SPIRIT_BUFF:                       strName = "info_new_raid_weapon_spirit_buff"; break;
        case E_TABLE::WEAPON_SPIRIT_SYNERGY:                    strName = "info_new_raid_weapon_spirit_synergy"; break;
        case E_TABLE::RAID_TRADING_MARKET:                      strName = "info_new_raid_trading_market_reward"; break;
        case E_TABLE::ADVENTURE_BOSS:                           strName = "info_advt_boss"; break;
        case E_TABLE::ADVENTURE_PARTY_ABILITY:                  strName = "info_advt_party_ability"; break;
        case E_TABLE::ADVENTURE_PARTY_MEMBER:                   strName = "info_advt_member"; break;
        case E_TABLE::ADVENTURE_FURNITURE:                      strName = "info_advt_office"; break;
        case E_TABLE::ADVENTURE_SETTING:                        strName = "info_advt_setting"; break;
        case E_TABLE::ADVENTURE_PROPERTY_TYPE:                  strName = "info_advt_property_type"; break;
        case E_TABLE::ADVENTURE_MAP:                            strName = "info_advt_map"; break;
        case E_TABLE::ADVENTURE_BONG_LEVEL:                     strName = "info_advt_exp"; break;
        case E_TABLE::ADVENTURE_RELIC:                          strName = "info_advt_relic"; break;
        case E_TABLE::ADVENTURE_RELIC_LEVEL:                    strName = "info_advt_relic_exp"; break;
        case E_TABLE::ADVENTURE_CORE_LEVEL:                     strName = "info_advt_craft_exp"; break;
        case E_TABLE::ADVENTURE_RELIC_COLLECT:                  strName = "info_advt_relic_collection"; break;
        case E_TABLE::ADVENTURE_FRIEND_ABILITY:                 strName = "info_advt_friend"; break;
        case E_TABLE::ADVENTURE_RELIC_RATE:                     strName = "info_advt_relic_rate"; break;
        case E_TABLE::ADVENTURE_MISSION:                        strName = "info_advt_quest"; break;
        case E_TABLE::GROWTH_PACK_REWARD:                       strName = "info_growth_pack_reward"; break;
        case E_TABLE::DIALOG_SCRIPT:                            strName = "info_dialog_script"; break;
        case E_TABLE::DIALOG_LIST:                              strName = "info_dialog_list";  break;
        case E_TABLE::DIALOG_ACTOR:                             strName = "info_dialog_actor"; break;
        case E_TABLE::EVENT_BOARDGAME_BOARD:                    strName = "info_event_boardgame_board_reward"; break;
        case E_TABLE::EVENT_BOARDGAME_QUEST:                    strName = "info_event_boardgame_quest"; break;
        case E_TABLE::EVENT_RACE_STAT:                          strName = "info_event_race_stat"; break;
        case E_TABLE::EVENT_RACE_RANK_REWARD:                   strName = "info_event_race_rank_reward"; break;
        case E_TABLE::EVENT_RACE_MISSION:                       strName = "info_event_race_mission"; break;
        case E_TABLE::TOWN_REWARD:                              strName = "info_bong_town_reward"; break;
        case E_TABLE::DAILY_WEEKLY_MISSION:                     strName = "info_daily_weekly_mission"; break;
        case E_TABLE::SPECIAL_MISSION:                          strName = "info_special_mission"; break;
        case E_TABLE::EVENT_LUCKYBAG_REWARD:                    strName = "info_event_luckybag_reward"; break;
        case E_TABLE::MINE_REFINE_SETTING:                      strName = "info_mine_mk2_setting"; break;
        case E_TABLE::FLASHMAN_MISSION:                         strName = "info_minigame_mission"; break;
        case E_TABLE::FLASHMAN_RANK:                            strName = "info_minigame_rank"; break;
        case E_TABLE::FLASHMAN_SETTING:                         strName = "info_minigame_setting"; break;
        case E_TABLE::DEFENSE_MAP:                              strName = "info_defense_map"; break;
        case E_TABLE::DEFENSE_CHARACTER:                        strName = "info_defense_character"; break;
        case E_TABLE::DEFENSE_WEAPON:                           strName = "info_defense_weapon"; break;
        case E_TABLE::DEFENSE_BULLET:                           strName = "info_defense_bullet"; break;
        case E_TABLE::DEFENSE_MONSTER:                          strName = "info_defense_monster"; break;
        case E_TABLE::DEFENSE_FLOOR:                            strName = "info_defense_floor"; break;
        case E_TABLE::DEFENSE_SETTING:                          strName = "info_defense_setting"; break;
        case E_TABLE::DEFENSE_GATCHA_LEVEL:                     strName = "info_defense_gacha_lv"; break;
        case E_TABLE::DEFENSE_GATCHA_RATE:                      strName = "info_defense_gacha"; break;
        case E_TABLE::DEFENSE_MANA_AMP_STONE:                      strName = "info_defense_mana_amp"; break;
        default:
            break;
    }
    
    return strName;
}
std::string TableManager::getTableFileName(std::string strName)
{
    if ( strName.length() != 0 )
    {
        strName = MafAes256::Encrypt(strName);
        strName = UtilsString::getInstance()->replaceAll(strName, "/", "%");
        strName.append(".dat");
    }
    
    return strName;
}
std::string TableManager::getTableFileNameTemp(std::string strName)
{
    if ( strName.length() != 0 )
    {
        strName.insert(0, "temp_");
        strName = MafAes256::Encrypt(strName);
        strName = UtilsString::getInstance()->replaceAll(strName, "/", "%");
        strName.append(".dat");
    }
    
    return strName;
}
std::string TableManager::getTableData(E_TABLE eType)
{
    std::string strResult = "";
    
    std::string strName = getTableName(eType);
    if ( strName.length() == 0 )
    {
        return strResult;
    }
    
    std::string strFileName = getTableFileName(strName);
    std::string strStoragePathFull = _strFilePath + strFileName;
    strResult = FileUtils::getInstance()->getStringFromFile(strStoragePathFull);
    strResult = MafAes256::Decrypt(strResult);
    
    return strResult;
}


#pragma mark - network : info
void TableManager::requestInfo(const std::function<void(bool)>& pCallback, const std::function<void(int, int, int)>& pCallbackProgress)
{
    //
    _callbackInfo = pCallback;
    _callbackInfoProgress = pCallbackProgress;
    
    //
    std::string url = "/table/v1/list/%d";
    url = MafUtils::format(url.c_str(), getVersion());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(response,dd);
    };
    req->send(cb);
}

void TableManager::responseInfo(network::HttpResponse* response,std::string &data)
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
        // callback
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false);
        }
        return;
    }
    
    //
    _nVersionTemp = jsonParser["_version"].GetInt();
    
    //
    _listTable.clear();
    for ( int i = 0; i < jsonParser["_tables"].Size(); i++ )
    {
        const rapidjson::Value& jsonEvent = jsonParser["_tables"][i];
        
        std::string strName = jsonEvent["tablename"].GetString();
        std::string strUrl = jsonEvent["url"].GetString();
        int64_t nSize = jsonEvent["size"].GetInt64();
        
        auto obj = ObjTable::create();
        obj->setName(strName);
        obj->setUrl(strUrl);
        obj->setSize(nSize);
        
        _listTable.pushBack(obj);
    }

    //
    if ( _listTable.size() == 0 )
    {
        if ( _callbackInfo != nullptr )
        {// callback
            _callbackInfo(true);
        }
    }
    else
    {
        _nDownloadTotal = (int)_listTable.size();
        _nDownloadSuccess = 0;
        _nDownloadError = 0;
        
        //
        downloadStart();
    }

}

void TableManager::requestLanguage(std::string strLang, const std::function<void(bool)>& pCallback, const std::function<void(int, int, int)>& pCallbackProgress)
{
    //
    _callbackInfo = pCallback;
    _callbackInfoProgress = pCallbackProgress;
    
    //
    std::string url = "/table/v0/language/%s";
    url = MafUtils::format(url.c_str(), strLang.c_str());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseLanguage(response,dd);
    };
    req->send(cb);
}
void TableManager::responseLanguage(cocos2d::network::HttpResponse* response,std::string &data)
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
        // callback
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false);
        }
        return;
    }
    
    //
    _nVersionTemp = getVersion();
    
    //
    _listTable.clear();
    std::string strName = jsonParser["tablename"].GetString();
    std::string strUrl = jsonParser["url"].GetString();
    int64_t nSize = jsonParser["size"].GetInt64();
    
    auto obj = ObjTable::create();
    obj->setName(strName);
    obj->setUrl(strUrl);
    obj->setSize(nSize);
    
    _listTable.pushBack(obj);
    
    //
    if ( _listTable.size() == 0 )
    {
        if ( _callbackInfo != nullptr )
        {// callback
            _callbackInfo(true);
        }
    }
    else
    {
        _nDownloadTotal = (int)_listTable.size();
        _nDownloadSuccess = 0;
        _nDownloadError = 0;
        
        //
        downloadStart();
    }
}

#pragma mark - download
void TableManager::downloadStart()
{
    if ( _listTable.size() == 0 )
    {
        downloadEnd();
        return;
    }
    
    // 파일 경로
    if ( _objTableTemp != nullptr )
    {
        _objTableTemp->release();
        _objTableTemp = nullptr;
    }
    _objTableTemp = _listTable.at(0);
    _objTableTemp->retain();
    _listTable.erase(_listTable.begin());

    //
    std::string strUrl = _objTableTemp->getUrl();
    std::string strName = _objTableTemp->getName();
    std::string strFileName = getTableFileName(strName);
    std::string strFileNameTemp = getTableFileNameTemp(strName);
    std::string strStoragePathFull = _strFilePath + strFileName;
    std::string strStoragePathFullTemp = _strFilePath + strFileNameTemp;
    
    // 이전에 파일이 있으면 삭제.
    FileUtils::getInstance()->removeDirectory(strStoragePathFull);
    FileUtils::getInstance()->removeDirectory(strStoragePathFull + ".tmp");
    FileUtils::getInstance()->removeFile(strStoragePathFull);
    FileUtils::getInstance()->removeFile(strStoragePathFull + ".tmp");
    
    //
    FileUtils::getInstance()->removeDirectory(strStoragePathFullTemp);
    FileUtils::getInstance()->removeDirectory(strStoragePathFullTemp + ".tmp");
    FileUtils::getInstance()->removeFile(strStoragePathFullTemp);
    FileUtils::getInstance()->removeFile(strStoragePathFullTemp + ".tmp");
    
    //
    _downloader->createDownloadFileTask(strUrl, strStoragePathFull);
}

void TableManager::downloadEnd()
{
    //
    setVersion(_nVersionTemp);
    _nVersionTemp = 0;
    
    //
    if ( _callbackInfo != nullptr )
    {
        _callbackInfo(true);
    }
    _callbackInfo = nullptr;
    _callbackInfoProgress = nullptr;
}

void TableManager::downloadTaskProgress(const cocos2d::network::DownloadTask& task, int64_t bytesReceived, int64_t totalBytesReceived, int64_t totalBytesExpected)
{
    double nPercent = double(totalBytesReceived * 100) / totalBytesExpected;
    if ( _callbackInfoProgress != nullptr )
    {
        _callbackInfoProgress(_nDownloadTotal, _nDownloadSuccess, nPercent);
    }
}

void TableManager::downloadFileSuccess(const cocos2d::network::DownloadTask& task)
{
    int64_t nSize = FileUtils::getInstance()->getFileSize(task.storagePath);
    if ( nSize != _objTableTemp->getSize() )
    {
        _nDownloadError++;
        if ( _callbackInfo != nullptr )
        {
            _callbackInfo(false);
        }
    }
    else
    {
        //
        std::string strText = FileUtils::getInstance()->getStringFromFile(task.storagePath);
        std::string strTextTemp = MafAes256::Decrypt(strText);
        rapidjson::Document jsonParser;
        jsonParser.Parse<0>((char*)strTextTemp.c_str());
        if( jsonParser.HasParseError() == false )
        {
            std::string strName = _objTableTemp->getName();
            std::string strFileNameTemp = getTableFileNameTemp(strName);
            std::string strStoragePathFullTemp = _strFilePath + strFileNameTemp;
            
            FileUtils::getInstance()->writeStringToFile(strText, strStoragePathFullTemp);
            
            //
            _nDownloadSuccess++;
            downloadStart();
        }
        else
        {
            _nDownloadError++;
            if ( _callbackInfo != nullptr )
            {
                _callbackInfo(false);
            }
        }
    }
}

void TableManager::downloadTaskError(const cocos2d::network::DownloadTask& task, int errorCode, int errorCodeInternal, const std::string& errorStr)
{
    _nDownloadError++;
    
    //
    if ( _callbackInfo != nullptr )
    {
        _callbackInfo(false);
    }
}
