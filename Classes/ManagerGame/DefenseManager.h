//
//  DefenseManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/06.
//

#ifndef DefenseManager_hpp
#define DefenseManager_hpp

#include "Libs/maf/MafSingleton.h"
#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoDefenseFloor;
class InfoDefenseMap;
class InfoDefenseMonster;
class InfoDefenseWeapon;
class InfoDefenseCharacter;
class InfoDefenseBullet;
class InfoDefenseTrench;
class Grid;
class InfoSetting;
class InfoRaidMileageLv;
class InfoItems;

class DefenseManager : public MafSingleton<DefenseManager>
{
private:
    DefenseManager(void);
    virtual ~DefenseManager(void);
    friend class MafSingleton;
public:
    virtual bool init();
    
public:
    void loadLocalData();
    void saveLocalData();
    
    void loadWeaponLvData();
    void saveWeaponLvData();
    void loadCharacterLvData();
    void saveCharacterLvData();
    //load data
    void loadData();
    void setLoadFloorData();
    void setLoadMapData();
    void setLoadMonsterData();
    void setLoadWeaponData();
    void setLoadCharacterData();
    void setLoadBulletData();
    void setLoadSetting();
    void setLoadMileageLv();
    
    // set, get
    
    Vector<InfoDefenseWeapon*> getWeaponList() const;
    Vector<InfoDefenseCharacter*> getCharacterList() const;
    Vector<InfoRaidMileageLv*> getInfoMileageLvList() const;
    
    
    Vector<InfoDefenseWeapon*> getTopStrongWeaponList() const;
    Vector<InfoDefenseCharacter*> getTopStrongCharacterList() const;
    
    InfoDefenseFloor* getFloorData(int floor) const;
    InfoDefenseFloor* getCurrentFloorData();
    InfoDefenseMap* getMapData(int idx) const;
    InfoDefenseMonster* getMonsterData(int idx) const;
    InfoDefenseWeapon* getWeaponData(int idx) const;
    InfoDefenseBullet* getBulletData(int idx) const;
    InfoDefenseCharacter* getCharacterData(int idx) const;
    InfoDefenseWeapon* getEquipWeaponDataByTrench(int num) const;
    InfoDefenseTrench* getInfoTrench(int num) const;
    InfoSetting* getInfoSetting(std::string key) const;
    InfoRaidMileageLv* getInfoMileageLv(int value) const;
    
    bool isOpenContentsNow();
    void setOpenContentsNow(bool isOpen);
    bool isBackground();
    void setIsBackground(bool isBackground);
    int getCurrentMileageLv();
    int64_t getCurrentFloor();
    int64_t getHighFloor();
    int getWeaponDataCount() const;
    int getCharacterDataCount() const;
    int64_t getFailStage() const;
    int getLastGrade(E_DF_Trench_SettingType type) const;
    int getFailRewardCount() const;
    
    void setDrawTicket_1(int count);
    void setDrawTicket_11(int count);
    void setDrawTicket_33(int count);
    int getDrawTicket_1();
    int getDrawTicket_11();
    int getDrawTicket_33();
    int getMileage();
    int getAdsCount();
    int getAdsCooltime();
    bool isFirstGift();
    
    int getMaxTrenchCount() const;
    bool isEquipWeapon(InfoDefenseWeapon* data, int& trenchNumber);
    bool isEquipCharacter(InfoDefenseCharacter* data, int& trenchNumber);
    int getObjectMaxGrade(E_DF_Trench_SettingType type) const;
    
    bool isReddotCondition(E_DF_Trench_SettingType type) const;
    bool isReddotCondition() const;
    //utils
    void clearEvents();
    
    void updateTrenchCharacter(int num, int characterIdx);
    void updateTrenchWeapon(int num, int weaponIdx);
    void updateObjectStatus(E_DF_Trench_SettingType type, bool isHave, int idx, int lv, int smelting, int count, int position);
    void updateWeaponStatus(bool isHave, int idx, int lv, int smelting, int count, int position);
    void updateCharacterStatus(bool isHave, int idx, int lv, int smelting, int count, int position);
    
    void updateObjectStatusServerData(E_DF_Trench_SettingType type, const rapidjson::Document& jsonParser);
    void checkObjectHack(E_DF_Trench_SettingType type, int idx, int level, int smeling);
    void resetWeaponData();
    void resetCharacterData();
    
    std::string getCharacterAttackCalculation(std::string value, bool isOrginal = false);
    std::string getWeaponAttackCalculation(std::string value, bool isOrginal = false);
    std::string getHighFloorAttackCalculation(std::string value, bool isOrginal = false);
    std::string getAttackCalculation(std::string value, bool isOrginal = false);
    //
    void testclear(bool isReset = false);
    void clearFloor();
    void FailStage();
    
    void resetDefenseCoin(bool bSave);
    void setDefenseCoin(int64_t plus, bool bSave = true, bool bRefresh = true);
    int64_t getDefenseCoin();
    
    //network : request
    void requestInfo(bool isShowLoading = true, const std::function<void(bool, int)>& callback = nullptr, bool isIntro = false);
    void requestEquip(E_DF_Trench_SettingType type, int idx, int pos, bool isShowLoading = true);
    void requestRankup(E_DF_Trench_SettingType type, int idx, int count, bool isShowLoading = true);
    void requestLevelup(E_DF_Trench_SettingType type, int idx, bool isShowLoading = true);
    void requestServerLevelup(E_DF_Trench_SettingType type, int idx, int level, bool isShowLoading = true);
    void requestSmelting(E_DF_Trench_SettingType type, int idx, bool isShowLoading = true);
    void requestUpdateInfo(bool isShowLoading = true, const std::function<void(bool, int)>& callback = nullptr);
    void requestSummon(int count, bool isAds, bool isShowLoading = true, const std::function<void(bool, int, cocos2d::Vector<InfoItems*>)>& callback = nullptr);
    void requestFirstGift(bool isShowLoading = true);
    
protected:
    // network : respons
    void responseInfo(bool isShowLoading, bool isIntro, cocos2d::network::HttpResponse* response,std::string &data);
    void responseEquip(bool isShowLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseRankup(bool isShowLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseServerLevelup(bool isShowLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseSmelting(bool isShowLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseUpdateInfo(bool isShowLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseSummon(bool isShowLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseFirstGift(bool isShowLoading, cocos2d::network::HttpResponse* response,std::string &data);
    
    
private:
    
    bool _isOpenContentsNow;
    bool _isBackground;
    bool _isFirstGift;
    TInteger _drawTicket_1;
    TInteger _drawTicket_11;
    TInteger _drawTicket_33;
    TInteger _drawMileage;
    TInteger _nAdsSummonCount;
    TInteger _nAdsSummonCooltime;
    
    TInteger _currentFloor;
    TInteger _highFloor;
    int _failRewardCount;
    
    int64_t _defenseCoin;
    
    Vector<InfoDefenseFloor*> _listInfoFloorData;
    Vector<InfoDefenseMap*> _listInfoMapData;
    Vector<InfoDefenseMonster*> _listInfoMonsterData;
    Vector<InfoDefenseWeapon*> _listInfoWeaponData;
    Vector<InfoDefenseCharacter*> _listInfoCharacterData;
    Vector<InfoDefenseBullet*> _listInfoBulletData;
    Vector<InfoDefenseTrench*> _listInfoTrench;
    cocos2d::Vector<InfoRaidMileageLv*> _listInfoMileageLv;
    
    cocos2d::Vector<InfoSetting*> _listInfoSetting;
    std::function<void(bool, int)> _callbackInfo;
    std::function<void(bool, int)> _callbackFloorUpdate;
    std::function<void(bool, int, cocos2d::Vector<InfoItems*>)> _callbackSummon;
public:
    
    MafDelegate<void(bool, int)> _onServerInfo;
    MafDelegate<void(bool, int)> _onServerEquip;
    MafDelegate<void(bool, int)> _onServerRankup;
    MafDelegate<void(bool, int)> _onServerLevelup;
    MafDelegate<void(bool, int)> _onServerSmelting;
    MafDelegate<void(bool, int)> _onServerSummon;
    MafDelegate<void(bool, int)> _onServerUpdateFloor;
    
    MafDelegate<void(void)> _onUpdateFail;
    MafDelegate<void(void)> _onUpdateClear;
    MafDelegate<void(void)> _onUpdateFloor;
    MafDelegate<void(void)> _onUpdateWeapon;
    MafDelegate<void(void)> _onUpdateCharacter;
    MafDelegate<void(void)> _onUpdateBackgound;
};

#endif /* DefenseManager_hpp */
