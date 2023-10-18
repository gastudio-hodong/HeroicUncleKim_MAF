//
//  EventReturnAttendManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/06/20.
//

#ifndef EventReturnAttendManager_h
#define EventReturnAttendManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/Event/InfoEventReturnAttendReward.h"

class EventReturnAttendManager : public MafSingleton<EventReturnAttendManager>
{
public:
    EventReturnAttendManager(void);
    virtual ~EventReturnAttendManager(void);
    virtual bool init();
    
public:
    const int getToday() const;
    const int getEndTime() const;
    const bool isRewardCondition() const;
    const bool isReturnEventCondition() const;
    const cocos2d::Vector<InfoEventReturnAttendReward*> getListInfoReward() const;
    const InfoEventReturnAttendReward* getEventReturnAttendReward(const int day) const;
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
    cocos2d::Vector<InfoEventReturnAttendReward*> _listInfoReward;
    
    // callback
    std::function<void(bool, int)> _callbackInfo;
    std::function<void(bool, int)> _callbackReward;
};

#endif /* EventReturnAttendManager_hpp */
