//
//  InfoServerMission.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/22.
//

#ifndef InfoServerMission_hpp
#define InfoServerMission_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoItems;
class InfoServerMission : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoServerMission);
    
    InfoServerMission();
    virtual ~InfoServerMission();
    virtual bool init();
    
public:
    const bool isActivate() const;
    void setIsActivate(const bool value);
    
    const bool isComplete() const;
    
    const bool isServerComplete() const;
    void setIsServerComplete(const bool value);
    
    const bool hasCondition() const;
    void setHasCondition(const bool value);
    
    const bool isEventMission() const;
    void setIsEventMission(const bool value);
    
    const int getSeason() const;
    void setSeason(const int value);
    
    const int getIdx() const;
    void setIdx(const int value);
    
    const E_MISSION_RESET_TYPE getResetType() const;
    void setResetType(const E_MISSION_RESET_TYPE value);
    
    const E_SHORTCUTS getShortcut() const;
    void setShortcut(const E_SHORTCUTS value);
    
    const int getCurrentCount() const;
    void setCurrentCount(const int value);
    
    const int getCompleteCount() const;
    void setCompleteCount(const int value);
    
    const std::string getMissionTypeByString() const;
    void setMissionType(const std::string value);
    const E_DAILY_QUEST getMissionType() const;
    
    const std::string getTitle() const;
    void setTitle(const std::string value);
    
    const int getPassPoint() const;
    void setPassPoint(const int value);
    
    const int64_t getStartTime() const;
    void setStartTime(const int64_t nTime);
    
    const int64_t getEndTime() const;
    void setEndTime(const int64_t nTime);
    
    cocos2d::Vector<InfoItems*> getListMissionReward() const;
    void setListMissionReward(const cocos2d::Vector<InfoItems*> list);
    
    cocos2d::Vector<InfoItems*> getListEventReward() const;
    void setListEventReward(const cocos2d::Vector<InfoItems*> list);
    
    void resetData();
private:
    bool _isActivate;
    bool _isServerComplete;
    bool _hasCondition;
    bool _isEventMission;
    
    int _nSeason;
    
    int _nIdx;
    
    E_MISSION_RESET_TYPE _eResetType;
    
    E_SHORTCUTS _eShortcut;
    int _nCurrentCount;
    int _nCompleteCount;
    std::string _strMissionType;
    std::string _strTitle;
    int _nPassPoint;
    
    int64_t _nStartTime;
    int64_t _nEndTime;
    
    cocos2d::Vector<InfoItems*> _listMissionReward;
    cocos2d::Vector<InfoItems*> _listEventReward;
};

#endif /* InfoServerMission_hpp */
