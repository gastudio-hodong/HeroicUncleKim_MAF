//
//  InfoEventRaceSimulateLog.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/03.
//

#ifndef InfoEventRaceSimulateLog_hpp
#define InfoEventRaceSimulateLog_hpp

#include "Common/ConfigDefault.h"

class InfoEventRaceSimulateLog : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoEventRaceSimulateLog);
    InfoEventRaceSimulateLog();
    virtual ~InfoEventRaceSimulateLog();
    virtual bool init();
    
public:
    const bool isBoost() const;
    void setIsBoost(const bool value);
    
    const int getTick() const;
    void setTick(const int value);
    
    const float getSpeed();
    void setSpeed(const float value);
    
    const float getStamina();
    void setStamina(const float value);
    
    const double getSection();
    void setSection(const double value);
protected:
    bool _isBoost;
    int _nTick;
    float _speed;
    float _stamina;
    double _curSection;
};

#endif /* InfoEventRaceSimulateLog_hpp */
