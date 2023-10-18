//
//  EventNewUserAttendManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/03.
//

#ifndef EventNewUserAttendManager_hpp
#define EventNewUserAttendManager_hpp

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/Event/InfoEventNewAttendReward.h"

class EventNewUserAttendManager : public MafSingleton<EventNewUserAttendManager>
{
public:
    EventNewUserAttendManager(void);
    virtual ~EventNewUserAttendManager(void);
    virtual bool init();
    
public:
    const int getToday() const;
    const int getEndTime() const;
    const bool isRewardCondition() const;
    const bool isNewUserEventCondition() const;
    const cocos2d::Vector<InfoEventNewAttendReward*> getListInfoReward() const;
    const InfoEventNewAttendReward* getEventNewAttendReward(const int day) const;
    // network : request
    void requestInfo(const std::function<void(bool, int)>& callback);
    void requestReward(const std::function<void(bool, int)>& callback);
    
protected:
    // network : respons
    void responseInfo(cocos2d::network::HttpResponse* const response,const std::string &data);
    void responseReward(cocos2d::network::HttpResponse* const response,const std::string &data);
    
private:
    int _nResult;
    int _nEndTime;
    bool _isRewardCondition;
    cocos2d::Vector<InfoEventNewAttendReward*> _listInfoReward;
    
    // callback
    std::function<void(bool, int)> _callbackInfo;
    std::function<void(bool, int)> _callbackReward;
};

#endif /* EventNewUserAttendManager_hpp */
