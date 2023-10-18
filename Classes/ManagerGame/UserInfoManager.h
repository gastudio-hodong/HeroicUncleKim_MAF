//
//  UserInfoManager.hpp
//  FantasyClicker
//
//  Created by SanHeo on 2017. 8. 17..
//
//

#ifndef UserInfoManager_hpp
#define UserInfoManager_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/ConfigGameNormal.h"
#include "Common/ConfigGameDevil.h"
#include "Common/ConfigKey.h"
#include "Common/Observer/IBattleInfo.h"


class InfoArtifact;
class UserInfoManager{

public:
    static UserInfoManager *getInstance();
    static void destroyInstance();
    
    UserInfoManager();
    virtual ~UserInfoManager();
    virtual bool init();
    
    void resetEveryThing();
    void resetNormal(int floor);
    void resetDevil();
    std::function<void()> _onModifyFloor;
    std::function<void()> _onModifyEquip;
    
    float _fHeightIphoneX;
    
private:
    E_PLACE _eCurrentPlace;
    
    // floor
    TInteger _uFloor;
    TInteger _uFloorCount;
    TInteger _uFloorTotal;
    TInteger _uFloorHigh;
    
    TInteger _uFloorDevil;
    TInteger _uFloorDevilCount;
    TInteger _uFloorDevilHigh;
    
    //
    TInteger _uRevival;
    TInteger _uRevivalDevil;
    
    //cheat sensor
    int _nCheatSensorFloorIntervalCount;
    
    // quest
    TInteger _uQuestLevel[C_COUNT_QUEST+1];
    
    TInteger _uQuestLevelDevil[C_COUNT_QUEST_DEVIL+1];
    double _uQuestTimeDevil[C_COUNT_QUEST_DEVIL+1];
    
    // equip
    E_EQUIP_IDX _uDevilEquip[C_COUNT_EQUIP_TYPE_DEVIL];
    TInteger _uDevilEquipLv[C_COUNT_EQUIP_TYPE_DEVIL];
    TInteger _uDevilEquipReinforce[C_COUNT_EQUIP_TYPE_DEVIL];
    
    E_PRINCESS_BUFF _ePrincessBuff;
    E_PRINCESS_BUFF _ePrincessBuffPrison;
    E_GATEKEEPER_BUFF _eGatekeeperBuff;
    E_GATEKEEPER_BUFF _eGatekeeperBuffPrison;
    
    bool _bArtifactDevil[C_COUNT_ARTIFACT_DEVIL+1];
    TInteger _uArtifactLevel[C_COUNT_ARTIFACT+1];
    std::unordered_map<int, int> _listArtifactReinforce;
    
    TInteger _uHonorLevel[4];//C_COUNT_HONOR+1];
    
    float _uFeverTiem;
    float _uItemFeverTiem;
    int _uItemFeverCount;

    TInteger _uRelicLevel[C_COUNT_RELIC+1];
    
    int _uTimeBuff[12];
    int _uTimeAds[20];
    
    int _uTimeMyhomeAds[C_MAX_MYHOME];
    
    int _uAttendVip;
    int _uAttendDay;
    
    //
    int _secondEarnCheck1;
    std::string _secondEarnCheck2;
    std::string _secondEarnCheck3[C_COUNT_QUEST+1];
    std::string _saveSecondEarnGold;
    
    int _secondEarnCheckDevil1;
    std::string _secondEarnCheckDevil2;
    std::string _secondEarnCheckDevil3[C_COUNT_QUEST_DEVIL+1];
    std::string _saveSecondEarnGoldDevil;

private:
    int _uDailyQuestTime;
    
    int _uMonthQuestProgress[C_COUNT_MONTH_QUEST+1];
    int _uMonthQuestComplete[C_COUNT_MONTH_QUEST+1];
    
    
    int _uMyhomeLevelList[C_MAX_MYHOME];
    int _uMyhomeWatchcountList[C_MAX_MYHOME];
    
    
    
    
    bool _uGiftHave;
    bool _uAttendHave;
    
    
    //
    cocos2d::Node* _uiBettle;
    
    bool _bOneKill;
    bool _bOneKillPrison;
    
    int _hitCount;
    int _hitCountDungeon;
    int _honghitCount;
    int _honghitCountDungeon;
    
    int _hitCountDevil;         //pet 펭귄용
    int _hitCountDevilHedgehog; //펫 고슴도치용
    int _hitCountDevilFlash;    //펫 플래쉬
    
    int _prisonEnterCount;
    int _raidAtkCount;
    
    std::mutex _mutexDamage;
    std::mutex _mutexCritical;
    std::mutex _mutexSpeed;
    std::mutex _mutexAttackSpeed;
    std::mutex _mutexGoldSecond;
    std::mutex _mutexGoldMonsterKill;
    std::mutex _mutexGold;
    
public:

#pragma mark-
    E_PLACE getCurrentPlace();
    void setCurrentPlace(E_PLACE eplace);
    
    // battle :
    cocos2d::Node* getBattleUI();
    void setBattleUI(cocos2d::Node* node);
    
    
#pragma mark - battle : floor
    int getFloor(E_PLACE ePlace);
    void setFloor(E_PLACE ePlace, int nFloor, bool bSave = true);
    
    int getFloorCount(E_PLACE ePlace);
    void setFloorCount(E_PLACE ePlace, int nFloorCount, bool bDirect = true, bool bSave = true);
    
    int getHighFloor(E_PLACE ePlace);
    void setHighFloor(E_PLACE ePlace, int nFloor);
    
    int getTotalFloor();
    void setTotalFloor(int value);
    
    bool isFloorCheck(int floor, bool bUp);
    bool isMeterCheck(int meter);
    
#pragma mark - cheat : sensor
    
    bool isCheatSensorOn();
    void setCheatSensorCount(int value);
    int getCheatSensorCount();
    
    void checkCheating();
    
    
#pragma mark - battle : revival
    int getRevival(E_PLACE ePlace);
    void setRevival(E_PLACE ePlace, int nRevival);
    
    int getRevivalAds(E_PLACE ePlace);
    void setRevivalAds(E_PLACE ePlace, int nAdsCount);
    
    
#pragma mark - battle : buff
    int getBuffTime(E_BUFF_TYPE eType);
    void setBuffTime(E_BUFF_TYPE eType, int nTime, bool bSave = true);
    
    bool getBuffVisible(E_BUFF_TYPE eType);
    void setBuffVisible(E_BUFF_TYPE eType, bool bVisible);
    
    time_t getBuffRevival();
    void setBuffRevival(time_t nTime);
    
    
#pragma mark - battle : ads
    int getAdsTime(E_SHOP_ADS eType);
    void setAdsTime(E_SHOP_ADS eType, int nTime, bool bSave = true);
    
    void setTimeAdsAllMinus();
    
    
#pragma mark - battle : hit or kill
    bool getOneKill(E_PLACE ePlace);
    void setOneKill(E_PLACE ePlace, bool bKill);
    
    int getHitCount(E_PLACE ePlace);
    void setHitCount(E_PLACE ePlace, int hitCount);
    
    int getHongHitCount(E_PLACE ePlace);
    void setHongHitCount(E_PLACE ePlace, int hitCount);

    int getHitCountDevilHedgehog();
    void setHitCountDevilHedgehog(int nVal);
    
    int getHitCountDevilFlash();
    void setHitCountDevilFlash(int nVal);
    
    void setPrisonEnterCount(int nVal);
    int getPrisonEnterCount();
    void setRaidAttackCount(int nVal);
    int getRaidAttackCount();
    

#pragma mark - battle : 획득 골드
    std::string getSecondEarnGoldNormal(bool bChange = false);
    std::string getSecondEarnGoldDevil();
    
    std::string getEarnGoldFinal(E_PLACE ePlace, E_EARNGOLD eEARN, bool bShopBuy = false); //최종 획득골드
    
    
#pragma mark - battle : 적처치 골드 증가
    std::string getMonsterBonusRate(E_PLACE ePlace, bool bArtifactPercent = true);
    std::string getMonsterBonusRateNormal(E_PLACE ePlace, bool bArtifactPercent = true);
    
    
#pragma mark - battle : 퀘스트 무기 골드 감소
    int getBuySaleRate(E_PLACE ePlace);
    
    
#pragma mark - battle : 공격력
    std::string getAttackDamage(E_PLACE eplace,E_CHARACTER eCharac, bool bCri, bool bOffline);
    std::string getAttackDamageNormal(E_CHARACTER eCharac, bool bOffline);
    std::string getAttackDamagePrison(E_CHARACTER eCharac);
    std::string getAttackDamageDevil(); //only cell draw
    
    std::string getAttackCriDamage(E_PLACE ePlace, std::string &tmpAtk);//cri 분리
    
    
#pragma mark - battle : 크리티컬, 공격속도, 이동속도
    bool getCriticalRate(E_PLACE ePlace, bool bBongsik, bool bOffline);
    int getCriticalRatePercent(E_PLACE ePlace, bool bBongsik, bool bOffline);
    
    double getAttackSpeed(E_PLACE ePlace);
    double getUserSpeed(E_PLACE ePlace, bool bOffline);
    
#pragma mark - quest
    bool getQuest(E_PLACE ePlace, E_QUEST eQuest);
    int getQuestLevel(E_PLACE ePlace, E_QUEST eQuest);
    void setQuestLevel(E_PLACE ePlace, E_QUEST eQuest, int nLevel, bool bSave = true);
    
    double getQuestTime(E_PLACE ePlace, E_QUEST eQuest);
    void setQuestTime(E_PLACE ePlace, E_QUEST eQuest, double nTime, bool bSave = true);
    
    std::string getQuestEarnGold(E_PLACE ePlace, E_QUEST eQuest, int nLevel);
    std::string getQuestEarnGoldNomal(E_PLACE ePlace, E_QUEST eQuest, int nLevel);
    std::string getQuestEarnGoldDevil(E_PLACE ePlace, E_QUEST eQuest, int nLevel);
    
    std::string getQuestSecondEarnGoldNomal(E_PLACE ePlace, E_QUEST eQuest, int nLevel);
    
    std::string getQuestGoldRate(E_PLACE ePlace);
    
    double getQuestTimeRate(E_PLACE ePlace);
    
    
#pragma mark - equip
    int getEquipLevelDevil(E_EQUIP_TYPE type);
    void setEquipLevelDevil(E_EQUIP_TYPE type,int level,bool bSave = true);
    
    E_EQUIP_IDX getEquipDevil(E_EQUIP_TYPE type);
    void setEquipDevil(E_EQUIP_TYPE type,E_EQUIP_IDX equip,bool bSave = true);
    
    int getEquipReinforceLevelDevil(E_EQUIP_TYPE type);
    void setEquipReinforceLevelDevil(E_EQUIP_TYPE type,int level,bool bSave = true);
    
    
#pragma mark - artifact
    int getArtifactLevel(E_PLACE ePlace, E_ARTIFACT eArti);
    void setArtifactLevel(E_PLACE ePlace, E_ARTIFACT eArti, int nLevel, bool bSave = true);
    void addHighArtifact(E_PLACE ePlace, E_ARTIFACT_TYPE eArtiType, int nLevel);
    const int getArtifactCurrentGrade(const E_ARTIFACT_TYPE eArtiType);
    
    bool isArtifactHaveDevil(int idx);
    void setArtifactDevil(int idx,bool val,bool bSave = true);
    
    int getArtifactMaxLevelCount(E_PLACE ePlace);
    int getArtifactCurrentGeneration(E_PLACE ePlace, E_ARTIFACT_TYPE eArtiType);
    InfoArtifact* getHighArtifactInfo(E_PLACE ePlace, E_ARTIFACT eArti);
    
    bool isArtifactReinforce(E_PLACE ePlace, int idx);
    int getArtifactReinforce(E_PLACE ePlace, int idx);
    void setArtifactReinforce(E_PLACE ePlace, int idx, int count, bool bSave = true);
    
    int getArtifactExistCount(E_PLACE ePlace);
    
    int getArtifactPointGain(E_PLACE ePlace);
    void setArtifactPointGain(E_PLACE ePlace, int nPoint);
    
    
#pragma mark - relic
    bool getRelic(E_PLACE ePlace, E_RELIC eRelic);
    
    int getRelicLevel(E_PLACE ePlace, E_RELIC eRelic);
    void setRelicLevel(E_PLACE ePlace, E_RELIC eRelic, int nLevel, bool bSave = true);
    
    
    
    // cube
    void setCubeDay(int day);
    int getCubeDay();
    int getCubePlusDay();
    
    int getCubeGain();
    int getCubePlusGain();
    int getCubeTotalGain();
    
    void setFeverTime(float time);
    float getFeverTime();
    void setFeverDay(int day);
    int getFeverDay();
    
    void setItemFeverTime(float time, bool bSave = true);
    float getItemFeverTime();
    
    int getItemFeverCount();
    void setItemFeverCount(int value, bool bSave = true);
    
    bool getPetBafometzHave();
    void setPetBafometzHave(bool data);
    
    E_PRINCESS_BUFF getPetBuffPriencess(E_PLACE ePlace);
    void setPetBuffPriencess(E_PLACE ePlace, E_PRINCESS_BUFF eBuff);
    
    E_GATEKEEPER_BUFF getPetBuffGatekeeper(E_PLACE ePlace);
    void setPetBuffGatekeeper(E_PLACE ePlace, E_GATEKEEPER_BUFF eBuff);
    
    int getTicketMax();
    
#pragma mark-
    void setHonorLevel(E_HONOR honor, int level,bool bSave = true);
    int getHonorLevel(E_HONOR honor);
    int getHonorHaveCount();
    
#pragma mark-
    void loadLocalData();
    
    void setAttendVip(int value,bool bSave=true);
    int getAttendVip();
    void setAttendDay(int value,bool bSave=true);
    int getAttendDay();
    
    void setMyhomeLevel(int floor, int value, bool save = true);
    void setMyhomeWatchcount(int floor, int value, bool save = true);
   
    int getMyhomeLevel(int floor);
    int getMyhomeLevelMax();
    int getMyhomeWatchcount(int floor);
    void setMyhomeAdsTime(int floor,int plus,bool bSave);
    int getMyhomeAdsTime(int floor);
    void setTimeMyhomeAdsAllMinus();
    
    bool getGiftHave();
    void setGiftHave(bool bGift);
    
    bool getAttendHave();
    void setAttendHave(bool bAttend);

    double getEquipTotalEffectPercentDevil(E_EQUIP_TYPE type);
};


#endif /* UserInfoManager_hpp */
