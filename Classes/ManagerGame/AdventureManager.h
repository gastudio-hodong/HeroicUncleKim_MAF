//
//  AdventureManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/14.
//

#ifndef AdventureManager_hpp
#define AdventureManager_hpp

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoQuest.h"
#include "GameObject/InfoSetting.h"
#include "GameObject/InfoBongContentLevel.h"
#include "GameObject/Adventure/InfoAdventureUpgradeAbility.h"
#include "GameObject/Adventure/InfoAdventurePartyAbilityPreset.h"
#include "GameObject/Adventure/InfoAdventurePartyAbilityOrigin.h"
#include "GameObject/Adventure/InfoAdventurePartyAbilityGradeRate.h"
#include "GameObject/Adventure/InfoAdventureFurniture.h"
#include "GameObject/Adventure/InfoAdventureParty.h"
#include "GameObject/Adventure/InfoAdventurePropertyType.h"
#include "GameObject/Adventure/InfoAdventureMap.h"
#include "GameObject/Adventure/InfoAdventureStage.h"
#include "GameObject/Adventure/InfoAdventureMember.h"
#include "GameObject/Adventure/InfoAdventureFriend.h"
#include "GameObject/Adventure/InfoAdventureBoss.h"

class AdventureManager : public MafSingleton<AdventureManager>
{
private:
    AdventureManager(void);
    virtual ~AdventureManager(void);
    friend class MafSingleton;
    
public:
    virtual bool init();
    
    //load table
    void setLoadAll();
    void setLoadAdventurePartyAbilityOrigin();
    void setLoadSetting();
    void setLoadAbilityGradeRate();
    void setLoadPartyMember();
    void setLoadFurniture();
    void setLoadBongLevel();
    void setLoadPropertyType();
    void setLoadMap();
    void setLoadFriendAbility();
    void setLoadMission();
    void setLoadBoss();
    
    //load save
    void loadFurnitureData();
    void saveFurnitureData();
    void loadAdventurePartyAbilityData();
    void saveAdventurePartyAbilityData();
    void saveAdventurePartyAbilitySeal(const int nParty, const int nPresetIdx);
    void saveAdventurePartyMemberLv(const int nParty);
    
    //list get set
    const cocos2d::Vector<InfoAdventurePartyAbilityOrigin*>& getListInfoPartyAbilityOriginal() const;
    const cocos2d::Vector<InfoAdventurePartyAbilityOrigin*> getListInfoPartyAbilityOriginalFromGrade(const int nGrade) const;
    const cocos2d::Vector<InfoAdventurePartyAbilityGradeRate*>& getListInfoPartyAbilityGradeRate() const;
    const cocos2d::Vector<InfoAdventureParty*>& getListInfoParty() const;
    const cocos2d::Vector<InfoAdventureFurniture*>& getListInfoFurniture() const;
    const cocos2d::Vector<InfoAdventureMap*>& getListInfoMap() const;
    const cocos2d::Vector<InfoAdventureFriend*>& getListInfoFriend() const;
    const cocos2d::Vector<InfoAdventurePartyAbility*> getListInfoPartyAbility(const int nParty) const;
    const cocos2d::Vector<InfoAdventureBoss*> getListInfoBoss(bool bHard) const;
    const Vector<InfoQuest*> getListInfoMissionFromGroup(const E_ADVENTURE_MISSION_GROUP eType) const;
    
    //info get set
    InfoSetting* const getSetting(const std::string strKey) const;
    InfoAdventurePartyAbilityOrigin* const getPartyAbility(const int nType) const;
    const int getPartyAbilityGradeRate(const int nGrade) const;
    InfoAdventurePartyAbilityGradeRate* const getInfoPartyAbilityGradeRate(const int nGrade) const;
    InfoAdventureParty* const getInfoParty(const int nParty) const;
    InfoAdventureFurniture* const getInfoFurniture(const int nIdx) const;
    InfoAdventurePropertyType* const getPropertyType(const int nIdx) const;
    InfoAdventurePropertyType* const getPropertyType(const E_ADVENTURE_PROPERTY_TYPE eType) const;
    InfoAdventureMap* const getInfoMap(const int nArea) const;
    InfoAdventureStage* const getInfoStage(const int nArea, const int nStage) const;
    InfoBongContentLevel* const getInfoBongLevel(const int nLv) const;
    InfoAdventureFriendAbility* const getInfoFriendAbilityFromIdx(const int nIdx);
    InfoAdventureFriendAbility* const getInfoFriendAbilityFromLevel(const int nCurrentLevel);
    InfoAdventureFriend* const getInfoFriendFromUserIdx(const int nUseridx);
    InfoAdventureFriend* const getInfoFriendFromPartyIdx(const int nPartyidx);
    InfoAdventureBoss* const getInfoBoss(const int nidx);
    InfoQuest* getInfoMission(const int nGroup, const int nIdx);
    
    //get utils
    const bool isServerDispatch() const;
    const int getLevel();
    const int getMaxLevel() const;
    const int getExp();
    const int getTicket();
    void setTicket(const int value);
    const int getCurrentRunningParty() const;
    const int getTotalAvilityDrawUseGem();
    const bool isAdventurePopupShow() const;
    void setIsAdventurePopupShow(const bool isShow);
    void checkLocalCompleteUpgrade();
    const int getDailyFriendCount();
    const bool isMissionComplete() const;
    bool isAdventureBullet(int category, int stage);
    
    //get effect
    const double getFurniturePropertyEffect(const E_ADVENTURE_PROPERTY_TYPE eType) const;
    const double getPartyMemberEffect(const E_ADVENTURE_PROPERTY_TYPE eType, const int nParty) const;
    const double getPartyMemberPublicEffect(const E_ADVENTURE_PROPERTY_TYPE eType, int nParty = 0, bool isInclueParty = false) const;
    const double getPartyAbilityEffect(const E_ADVENTURE_PROPERTY_TYPE eType, const int nParty) const;
    const double getPartyAbilityPublicEffect(const E_ADVENTURE_PROPERTY_TYPE eType, int nParty = 0, bool isInclueParty = false) const;
    const double getPartyAbilityDamage() const;
    const double getEffectDecreseTime(const int nParty, bool isInclueParty = false) const;
    const double getAdventureTotalEffect(const E_ADVENTURE_PROPERTY_TYPE eType, const int nParty, bool isInclueParty = false) const;
    static const std::string getPropertyEffectText(const E_ADVENTURE_PROPERTY_TYPE eType, const double dValue);
    
    //Party Ability
    void requestPartyAbilityDraw(const std::function<void(bool, int)>& callback, const int nParty);
    void drawPartyAbility(InfoAdventurePartyAbility* const infoAbility);
    const int getSealCount(const int nParty) const;
    const int getDrawPrice(const int nParty) const;
    void checkPartyAvilityError();
    
    const int getResetTime();
    const bool isAdventureComplete() const;
    void setTutorialStep(const int nStep, const bool isSave = true);
    const int getTutorialStep() const;
    
    //Party
    void requestPartyMemberUpgrade(const std::function<void(bool, int)>& callback, const int nParty, const int nIdx);
    void resetRunningPartyData();
    const int getPartyLevel(const int nParty) const;
    
    //Furniture
    void requestFurnitureUpgrade(const std::function<void(bool, int)>& callback, const int nIdx);
    const int getFurnitureTotalLv() const;
    
    // boss
    int getBossIdx();
    int getBossEndTime();
    int getBossSeason();
    
    //network : request
    void requestInfo(const std::function<void(bool, int)>& callback);
    void requestAdventureRunning(const std::function<void(bool, int)>& callback, InfoAdventureStage* const infoStage, const int nParty, int nFUserIdx = -1, std::string strFPlatform = "");
    void requestAdventureReward(const std::function<void(bool, int)>& callback, const int nParty, bool isFriend = false);
    void requestAdventureTimeReduction(const std::function<void(bool, int)>& callback, InfoAdventureStage* const infoStage, const int nPrice);
    void requestObtainGummie(const std::function<void(bool, int)>& callback);
    void requestMissionInfo(const std::function<void(bool, int)>& callback);
    void requestMissionComplete(const std::function<void(bool, int)>& callback, E_ADVENTURE_MISSION_GROUP eType, int nIdx);
    void requestMissionAllComplete(const std::function<void(bool, int)>& callback);
    void requestBossSelect(int idx, const std::function<void(bool, int)>& callback);
    void requestBossAttack(int idx, int itemIdx, int itemCount, const std::function<void(bool, int)>& callback);
    
    //delegate
    void subMissionEvent(MafFunction<void()>* func);
    void unSubMissionEvent(void* obj);
protected:
    // network : respons
    void responseInfo(cocos2d::network::HttpResponse* const response, const std::string &data);
    void responseAdventureRunning(cocos2d::network::HttpResponse* const response, const std::string &data);
    void responseAdventureReward(cocos2d::network::HttpResponse* const response, const std::string &data);
    void responseAdventureTimeReduction(cocos2d::network::HttpResponse* const response, const std::string &data);
    void responseObtainGummie(cocos2d::network::HttpResponse* const response, const std::string &data);
    void responseMissionInfo(cocos2d::network::HttpResponse* const response, const std::string &data);
    void responseMissionComplete(cocos2d::network::HttpResponse* const response, const std::string &data);
    void responseMissionAllComplete(cocos2d::network::HttpResponse* const response, const std::string &data);
    void responseBossSelect(cocos2d::network::HttpResponse* const response, const std::string &data);
    void responseBossAttack(cocos2d::network::HttpResponse* const response, const std::string &data);
    
private:
    bool _isAdventurePopupShow;
    bool _isServerDispatch;
    
    int _nTutorialStep;
    
    TInteger _nLevel; // 모험단 레벨
    TInteger _nExp; // 모험단 경험치
    
    TInteger _adventureTicket; // 탐험 티켓
    TInteger _adventureResetTime; // 탐험 초기화 시간
    TInteger _adventureDailyFriendCount; // 탐험 친구 횟수
    std::vector<std::string> _listAdventureBullet;
    
    TInteger _nTotalAvilityDrawUseGem; // 보너스 능력 총 보석 소모
    
    TInteger _nBossSeason;
    TInteger _nBossEndTime;
    TInteger _nBossIdx;
    
    Vector<InfoAdventurePartyAbilityOrigin*> _listPartyAbilityOriginal;
    Vector<InfoSetting*> _listInfoSetting;
    Vector<InfoAdventurePartyAbilityGradeRate*> _listInfoAbilityGradeRate;
    Vector<InfoAdventureParty*> _listInfoParty;
    Vector<InfoAdventureFurniture*> _listInfoFurniture;
    Vector<InfoAdventurePropertyType*> _listInfoPropertyType;
    Vector<InfoAdventureMap*> _listInfoMap;
    Vector<InfoBongContentLevel*> _listInfoBongLevel;
    Vector<InfoAdventureFriend*> _listInfoFriend;
    Vector<InfoAdventureFriendAbility*> _listInfoFriendAbility;
    Vector<InfoAdventureBoss*> _listInfoBoss;
    Vector<InfoQuest*> _listInfoQuest;
    
    std::function<void(bool, int)> _callbackDrawPartyAbility;
    std::function<void(bool, int)> _callbackMemberUpgrade;
    std::function<void(bool, int)> _callbackFurnitureUpgrade;
    
    std::function<void(bool, int)> _callbackInfo;
    std::function<void(bool, int)> _callbackAdventureRunning;
    std::function<void(bool, int)> _callbackAdventureReward;
    std::function<void(bool, int)> _callbackAdventureTimeReduction;
    std::function<void(bool, int)> _callbackObtainGummie;
    std::function<void(bool, int)> _callbackMissionInfo;
    std::function<void(bool, int)> _callbackMissionComplete;
    std::function<void(bool, int)> _callbackMissionAllComplete;
    
    std::function<void(bool, int)> _callbackBossSelect;
    std::function<void(bool, int)> _callbackBossAttack;
    
    MafDelegate<void()> _eventMission;
};
#endif /* AdventureManager_hpp */
