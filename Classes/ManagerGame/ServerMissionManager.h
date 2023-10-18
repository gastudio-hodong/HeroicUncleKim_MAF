//
//  ServerMissionManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/22.
//

#ifndef ServerMissionManager_hpp
#define ServerMissionManager_hpp

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoServerMission.h"

class ServerMissionManager : public MafSingleton<ServerMissionManager>
{
public:
    ServerMissionManager();
    virtual ~ServerMissionManager(void);
    bool init();
    
public:
    void setLoadMission();
    void resetServerMission(E_MISSION_RESET_TYPE type);
    
    //Mission
    void setServerMission(E_DAILY_QUEST mission, int plus);
    void resetServerMissionData(E_MISSION_RESET_TYPE type);
    bool checkServerMission();
    void checkMidnightReset();
    bool checkMissionCondition(E_DAILY_QUEST mission);
    
    //get
    cocos2d::Vector<InfoServerMission*> getListActivateServerMissionInfo();
    cocos2d::Vector<InfoServerMission*> getListActivateServerMissionInfo(E_MISSION_RESET_TYPE type);
    InfoServerMission* getActivateServerMissionInfo(std::string strType, E_MISSION_RESET_TYPE resetType, int completeCount);
    
    cocos2d::Vector<InfoServerMission*> getListServerMissionInfo();
    cocos2d::Vector<InfoServerMission*> getListServerMissionInfo(E_MISSION_RESET_TYPE type);
    InfoServerMission* getServerMissionInfo(const int nIdx);
    InfoServerMission* getServerMissionInfo(std::string strType, E_MISSION_RESET_TYPE resetType, int completeCount);
    
    int getDailySeason();
    int getWeeklySeason();
    int64_t getResetTime(E_MISSION_RESET_TYPE type);
    
    bool isServerMissionCompleteDaily();
    bool isServerMissionCompleteWeekly();
    bool isServerMissionCompleteAll();
    bool isServerMissionComplete(E_DAILY_QUEST eType, E_MISSION_RESET_TYPE eResetType = E_MISSION_RESET_TYPE::NONE);
    bool isCompleteDoneMission(E_MISSION_RESET_TYPE eType);
    static bool isServerMissionOpenCondition();
    
    //로컬 저장
    void loadServerMissionData(E_MISSION_RESET_TYPE type);
    void saveServerMissionAll();
    void saveServerMissionByResetType(E_MISSION_RESET_TYPE type);
    void writeJsonServerMission(E_MISSION_RESET_TYPE type);
    std::string getServerMissionData(E_MISSION_RESET_TYPE type);
    
    //utils
    //void removeCallback(E_MISSION_CALLBACK_TYPE eType);
    bool checkMissionReset(const E_MISSION_RESET_TYPE type, const int season);
    
    //network
    void requestInfoMission(const std::function<void(bool, int)>& pCallback, E_MISSION_RESET_TYPE type, bool isLoadData = false);
    void responseInfoMission(cocos2d::network::HttpResponse* response,std::string &data, E_MISSION_RESET_TYPE type, bool isLoadData);
    
    void requestUpdateMission(bool bPopupLoading, E_MISSION_RESET_TYPE type, const std::function<void(bool, int)>& callback);
    void responseUpdateMission(bool bPopupLoading, E_MISSION_RESET_TYPE type, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response, std::string &data);
    
    void requestMissionReward(InfoServerMission* info, std::function<void(bool, int)> pCallback);
    void responseMissionReward(cocos2d::network::HttpResponse* response, std::string &data, E_MISSION_RESET_TYPE type);
    
private:
    int _dailySeason;
    int _weeklySeason;
    
    int64_t _dailyResetTime;
    int64_t _weeklyResetTime;
    
    std::function<void(bool, int)> _callbackInfoDaily;
    std::function<void(bool, int)> _callbackInfoWeekly;
    std::function<void(bool, int)> _callbackReward;
    
    cocos2d::Vector<InfoServerMission*> _vecServerMissionInfo;
};

#endif /* ServerMissionManager_hpp */
