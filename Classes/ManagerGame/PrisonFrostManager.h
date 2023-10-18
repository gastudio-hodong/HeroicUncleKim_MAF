//
//  PrisonFrostManager.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#ifndef PrisonFrostManager_h
#define PrisonFrostManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/PrisonFrost/InfoFrostFloor.h"
#include "GameObject/PrisonFrost/InfoFrostSkill.h"
#include "GameObject/InfoSetting.h"

class PrisonFrostManager : public MafSingleton<PrisonFrostManager>
{
public:
    PrisonFrostManager(void);
    virtual ~PrisonFrostManager(void);
    virtual bool init();
    
public:
    // update
    void update(float dt);
    
    // load
    void setLoad();
    void setLoadFloor();
    void setLoadSkill();
    void setLoadSetting();
    
    // set, get : prison
    cocos2d::Vector<InfoFrostFloor*> getListFloor();
    cocos2d::Vector<InfoFrostSkill*> getListSkillAll();
    cocos2d::Vector<InfoFrostSkill*> getListSkill();
    InfoFrostFloor* getInfoFloor(int idx);
    InfoFrostSkill* getInfoSkill(int idx, int step);
    InfoSetting* getInfoSetting(std::string key);
    
    // set, get :
    int getClearFloor();
    int getClearFloorMax();
    
    bool isBattleAuto();
    void setBattleAuto(bool bEnable);
    int getBattleAutoNow();
    void setBattleAutoNow(int count);
    int getBattleAutoMax();
    void setBattleAutoMax(int count);
    
    
    int64_t getTimeReset();
    
    
    
    // set, get : skill
    std::map<int, int> getSkillStepAll();
    int getSkillStep(int idx);
    void setSkillStep(int idx, int step);
    
    std::map<int, int> getSkillLevelAll();
    int getSkillLevel(int idx);
    void setSkillLevel(int idx, int level);
    int getSkillLevelCost(int idx, int levelFrom, int levelTo);
    
    std::map<int, double> getSkillCoolTimeAll();
    double getSkillCoolTime(int idx);
    void setSkillCoolTime(int idx, double coolTime);
    
    std::map<int, double> getSkillDurationAll();
    double getSkillDuration(int idx);
    void setSkillDuration(int idx, double duration);
    
    double getSkillEffect(int idx, int level);
    double getSkillBuffEffect(int idx, int level);
    void setSkillUse(int idx);
    
    // set, get : ads
    int getAdsCount();
    int getAdsCountMax();
    double getAdsEffect();
    
    // delegate
    void subDelegateInfo(MafFunction<void(bool, int)>* func);
    void invokeDelegateInfo(bool bResult, int nResult);
    void unSubDelegateInfo(void* obj);
    
    void subDelegateSkillLevelUp(MafFunction<void(bool, int)>* func);
    void invokeDelegateSkillLevelUp(bool bResult, int nResult);
    void unSubDelegateSkillLevelUp(void* obj);
    
    void subDelegateSkillReset(MafFunction<void(bool, int)>* func);
    void invokeDelegateSkillReset(bool bResult, int nResult);
    void unSubDelegateSkillReset(void* obj);
    
    void subDelegateResult(MafFunction<void(bool, int, std::string)>* func);
    void invokeDelegateResult(bool bResult, int nResult, std::string reward);
    void unSubDelegateResult(void* obj);
    
    // network
    void requestInfo(bool bPopupLoading, const std::function<void(bool, int)>& callback = nullptr);
    void requestSkillLevel(bool bPopupLoading, int skillIdx, int levelCount);
    void requestSkillReset(bool bPopupLoading);
    void requestAds(bool bPopupLoading, const std::function<void(bool, int)>& callback = nullptr);
    void requestResult(bool bPopupLoading, int floor, bool bClear, std::string damage);
    
protected:
    // network
    void responseInfo(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseSkillLevel(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseSkillReset(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseAds(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseResult(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    
    

private:
    //
    cocos2d::Vector<InfoFrostFloor*> _listFloor;
    cocos2d::Vector<InfoFrostSkill*> _listSkillAll;
    cocos2d::Vector<InfoFrostSkill*> _listSkill;
    cocos2d::Vector<InfoSetting*> _listSetting;
    std::map<int, int> _listSkillStep;
    std::map<int, int> _listSkillLevel;
    std::map<int, double> _listSkillCoolTime;
    std::map<int, double> _listSkillDuration;
    int _skillUse;
    
    TInteger _clearFloor;
    int64_t _timeReset;
    TInteger _adsCount;
    
    bool _bBattleAuto;
    int _battleAutoNow;
    int _battleAutoMax;
    
    
    //
    MafDelegate<void(bool, int)> _delegateInfo;
    MafDelegate<void(bool, int)> _delegateSkillLevel;
    MafDelegate<void(bool, int)> _delegateSkillReset;
    MafDelegate<void(bool, int, std::string)> _delegateResult;
    
    std::function<void(bool, int)> _callbackInfo;
    std::function<void(bool, int)> _callbackAds;
    
};

#endif /* PrisonFrostManager_h */
