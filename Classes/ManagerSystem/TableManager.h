//
//  TableManager.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#ifndef TableManager_h
#define TableManager_h

#include "Libs/maf/MafSingleton.h"

// network
#include "network/HttpClient.h"
#include "network/HttpRequest.h"

enum class E_TABLE
{
    NONE = 0,
    
    LANGUAGE_LIST,
    LANGUAGE_TALK,
    LANGUAGE_TALK_TIP,
    LANGUAGE_TEXT,
    LANGUAGE_DIALOG_TEXT,
    LANGUAGE_BADWORD,
    PARAMETER,
    REVERSE_APK,
    EVENT_DATE,
    STORY,
    ATTENDANCE_DAILY,
    ATTENDANCE_MONTHLY,
    GAME_LIST,
    ACHIEVEMENTS,
    ARTIFACT,
    ITEMS,
    FROST_PRISON_FLOOR,
    FROST_PRISON_SKILL,
    FROST_PRISON_SETTING,
    PRISON_QUEST,
    PRISON_HELL_QUEST,
    PRISON_HELL_TRADING_MARKET,
    EVENT_COOPERATION_RAID_ACHIEVE,
    EVENT_COOPERATION_RAID_QUEST,
    INFINITE_ARTIFACT_INFO,
    EVENT_LIMIT_COIN_FIXED,
    EVENT_FISHING_QUEST,
    EVENT_FISHING_BOARD,
    EVENT_CO_MISSION_PERSON,
    EVENT_CO_MISSION,
    EVENT_VOTE_ALL,
    EVENT_VOTE_PERSONAL,
    EVENT_TREASURE_HUNT_MAP,
    EVENT_TREASURE_HUNT_REWARD,
    EVENT_SUNFISH,
    BADGE,
    TEAM_RAID_QUEST,
    TEAM_RAID_DRAW_RATE,
    PURCHASE,
    ACHIEVEMENTS_NORMAL,
    PET_SKIN,
    TEAM_RAID_REWARD,
    WEAPON,
    COSTUME,
    COSTUME_SKIN_LIST,
    COSTUME_SKIN_SHOP,
    PET_EQUIP_BOX_SHOP,
    PET_EQUIP_DATA,
    PET_EQUIP_AMOUNT,
    EXPEDITION_LOOT,
    TIMING_PACKAGE,
    EVENT_BOARDGAME_BOARD,
    EVENT_BOARDGAME_QUEST,
    RAID_NEW_QUEST,
    RAID_DRAGON,
    RAID_BONG_LEVEL,
    RAID_MILEAGE_LV,
    RAID_MILEAGE_REWARD,
    RAID_BONG_STAT,
    RAID_BONG_SETTING,
    RAID_STAT_INCREASE,
    RAID_RANK_REWARD,
    WEAPON_SPIRIT,
    WEAPON_SPIRIT_SMELTING_COST,
    WEAPON_SPIRIT_DRAW_RATE,
    EVENT_TRADING_MARKET_REWARD,
    EVENT_TRADING_MARKET_SETTING,
    WEAPON_SPIRIT_BUFF,
    WEAPON_SPIRIT_SYNERGY,
    RAID_TRADING_MARKET,
    ADVENTURE_BOSS,
    ADVENTURE_PARTY_ABILITY,
    ADVENTURE_PARTY_MEMBER,
    ADVENTURE_FURNITURE,
    ADVENTURE_SETTING,
    ADVENTURE_PROPERTY_TYPE,
    ADVENTURE_MAP,
    ADVENTURE_BONG_LEVEL,
    ADVENTURE_RELIC,
    ADVENTURE_RELIC_LEVEL,
    ADVENTURE_CORE_LEVEL,
    ADVENTURE_RELIC_COLLECT,
    ADVENTURE_FRIEND_ABILITY,
    ADVENTURE_RELIC_RATE,
    ADVENTURE_MISSION,
    GROWTH_PACK_REWARD,
    DIALOG_SCRIPT,
    DIALOG_LIST,
    DIALOG_ACTOR,
    EVENT_RACE_STAT,
    EVENT_RACE_RANK_REWARD,
    EVENT_RACE_MISSION,
    TOWN_REWARD,
    DAILY_WEEKLY_MISSION,
    SPECIAL_MISSION,
    EVENT_LUCKYBAG_REWARD,
    MINE_REFINE_SETTING,
    FLASHMAN_MISSION,
    FLASHMAN_RANK,
    FLASHMAN_SETTING,
    DEFENSE_MAP,
    DEFENSE_CHARACTER,
    DEFENSE_WEAPON,
    DEFENSE_BULLET,
    DEFENSE_MONSTER,
    DEFENSE_FLOOR,
    DEFENSE_SETTING,
    DEFENSE_GATCHA_LEVEL,
    DEFENSE_GATCHA_RATE,
    DEFENSE_MANA_AMP_STONE,
};

class ObjTable : public cocos2d::Ref
{
public:
    CREATE_FUNC(ObjTable);

    ObjTable();
    virtual ~ObjTable();
    virtual bool init();

public:
    std::string getName();
    void setName(std::string strName);
    
    std::string getUrl();
    void setUrl(std::string strUrl);
    
    int64_t getSize();
    void setSize(int64_t nSize);
    

protected:
    std::string _strName;
    std::string _strUrl;
    int64_t _nSize;
    
};

class TableManager : public MafSingleton<TableManager>
{    
public:
    TableManager(void);
    virtual ~TableManager(void);
    virtual bool init();
    
public:
    // load
    void setLoad();
    
    int getVersion();
    void setVersion(int nVersion);
    void setVersionNew(bool bNew);
    
    std::string getTableName(E_TABLE eType);
    std::string getTableFileName(std::string strName);
    std::string getTableFileNameTemp(std::string strName);
    std::string getTableData(E_TABLE eType);
    
    // network
    void requestInfo(const std::function<void(bool)>& pCallback, const std::function<void(int, int, int)>& pCallbackProgress);
    void requestLanguage(std::string strLang, const std::function<void(bool)>& pCallback, const std::function<void(int, int, int)>& pCallbackProgress);
    
protected:
    // network
    void responseInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void responseLanguage(cocos2d::network::HttpResponse* response,std::string &data);
    
    // download
    void download(int nIdx, const std::function<void(int)>& callbackStart, const std::function<void(double)>& callbackProgress, const std::function<void(int, int)>& callbackCount, const std::function<void(void)>& callbackEnd);
    void downloadStart();
    void downloadEnd();
    void downloadTaskProgress(const cocos2d::network::DownloadTask& task, int64_t bytesReceived, int64_t totalBytesReceived, int64_t totalBytesExpected);
    
    void downloadFileSuccess(const cocos2d::network::DownloadTask& task);
    void downloadTaskError(const cocos2d::network::DownloadTask& task, int errorCode, int errorCodeInternal, const std::string& errorStr);
    
private:
    cocos2d::Vector<ObjTable*> _listTable;
    ObjTable* _objTableTemp;
    
    std::string _strFilePath;
    
    int _nVersionTemp;
    
    bool _bVersionNew;

    //
    std::function<void(bool)> _callbackInfo;
    std::function<void(int, int, int)> _callbackInfoProgress;
    
    //
    std::unique_ptr<cocos2d::network::Downloader> _downloader;
    int _nDownloadTotal;
    int _nDownloadSuccess;
    int _nDownloadError;
    
};

#endif /* TableManager_h */
