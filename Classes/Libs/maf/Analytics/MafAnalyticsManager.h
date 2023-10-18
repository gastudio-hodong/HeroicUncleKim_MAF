//
//  MafAnalyticsManager.hpp
//  FantasyClicker-mobile
//
//  Created by maf on 07/11/2019.
//

#ifndef MafAnalyticsManager_hpp
#define MafAnalyticsManager_hpp

#include "cocos2d.h"
#include "Libs/maf/Analytics/MafAnalyticsEvent.h"

class MafAnalyticsManager
{
public:
    static void InitAnalytics();
    
    static void LogEvent(const char* eventName, bool once);
    static void LogEvent(const char* eventName, const char* param, int value, bool once);
    static void LogEvent(const char* eventName, const char* param, const char* value, bool once);
    
private:
    static void InitAppsFlyer();
};

#endif /* MafAnalyticsManager_hpp */
