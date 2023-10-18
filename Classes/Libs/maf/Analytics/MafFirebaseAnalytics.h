//
//  MafFirebaseAnalytics.hpp
//  FantasyClicker
//
//  Created by SanHeo on 2016. 9. 6..
//
//

#ifndef MafFirebaseAnalytics_hpp
#define MafFirebaseAnalytics_hpp

#include "cocos2d.h"

class MafFirebaseAnalytics
{
public:
    
    static void InitFirebaseAnalytics();
    
    static void LogEvent(const char* eventName, const char* param, int value=1, bool once=true);
    static void LogEvent(const char* eventName, const char* param, const char* value, bool once=true);
    
    //static void SendTracking(const char* strCategory,const char* strAction,const char* strLabel,int val);
    
private:
    static void setLogEvent(const char* eventName, const char* param);
    static bool isLogEvent(const char* eventName, const char* param);
};

#endif /* MafFirebaseAnalytics_hpp */
