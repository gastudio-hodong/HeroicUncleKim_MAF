//
//  InfoEventOriginData.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/02.
//

#ifndef InfoEventOriginData_hpp
#define InfoEventOriginData_hpp

#include "Common/ConfigDefault.h"

class InfoEventOriginData : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoEventOriginData);
    
    InfoEventOriginData();
    virtual ~InfoEventOriginData();
    virtual bool init();
    
public:
    const int getIdx() const;
    void setIdx(const int value);
    
    const int getEventIdx() const;
    void setEventIdx(const int value);
    
    const int getEventType() const;
    void setEventType(const int value);
    
    const int64_t getEventStartTime() const;
    void setEventStartTime(const int64_t value);
    void setEventStartTime(const std::string value);
    
    const int64_t getEventEndTime() const;
    void setEventEndTime(const int64_t value);
    void setEventEndTime(const std::string value);
    
    
    const std::string getOtherData() const;
    void setOtherData(const std::string value);
    
private:
    int _nIdx;
    int _nEventIdx;
    int _nEventType;
    
    int64_t _nEventStartTime;
    int64_t _nEventEndTime;
    
    std::string _strOtherData;
};

#endif /* InfoEventOriginData_hpp */
