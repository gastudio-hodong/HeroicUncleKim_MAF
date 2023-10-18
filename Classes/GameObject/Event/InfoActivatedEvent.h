//
//  InfoActivatedEvent.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/18.
//

#ifndef InfoActivatedEvent_hpp
#define InfoActivatedEvent_hpp

#include "Common/ConfigDefault.h"

class InfoActivatedEvent : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoActivatedEvent);
    
    InfoActivatedEvent();
    virtual ~InfoActivatedEvent();
    virtual bool init();
    
public:
    int getIdx();
    void setIdx(int nIdx);
    
    int64_t getEventStart();
    void setEventStart(int64_t value);
    
    int64_t getEventEnd();
    void setEventEnd(int64_t value);
    
    int64_t getEventRewardDay();
    void setEventRewardDay(int64_t value);
    
    
private:
    int _nIdx;
    
    int64_t _nEventStart;
    int64_t _nEventEnd;
    int64_t _nEventRewardDay;
    
    
};
#endif /* InfoActivatedEvent_hpp */
