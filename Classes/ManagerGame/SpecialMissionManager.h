//
//  SpecialMissionManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/29.
//

#ifndef SpecialMissionManager_hpp
#define SpecialMissionManager_hpp

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoSpecialMission.h"

class SpecialMissionManager : public MafSingleton<SpecialMissionManager>
{
public:
    SpecialMissionManager();
    virtual ~SpecialMissionManager(void);
    bool init();
    
public:
    void setLoadMission();
    
    cocos2d::Vector<InfoSpecialMission*> getListSpecialMission();
    InfoSpecialMission* getInfoSpecialMission(const int idx);
    
    void setSpecialMissionClear(const int value, const bool bSave = true);
    const int getSpecialMissionClear()const;
    void setSpecialMissionComplete(const int value, const bool bSave = true);
    const int getSpecialMissionComplete()const;
    void clearSpecialMission(const int missionIdx);
    
    
    void checkSpeicalMission(const E_PLACE ePlace);
    const bool isCompleteSpeicalMission(const E_PLACE ePlace) const;
    
    void requestMissionReward(const std::function<void(bool, int)>& pCallback, int idx);
    void responseMissionReward(cocos2d::network::HttpResponse* response,std::string &data, const int idx);
    void missionRewardAfter(const int missionIdx);
    
    E_SHORTCUTS onShortCut(int idx);
private:
    int _nSpecialMissionClear;
    int _nSpecialMissionComplete;
    
    cocos2d::Vector<InfoSpecialMission*> _listSpecialMission;
    std::function<void(bool, int)> _callbackReward;
};

#endif /* SpecialMissionManager_hpp */
