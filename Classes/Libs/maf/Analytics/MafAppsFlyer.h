//
//  MafAnalytics.h
//  HexagonSnake
//
//  Created by SanHeo on 2014. 9. 16..
//
//

#ifndef __HexagonSnake__MafAppsFlyer__
#define __HexagonSnake__MafAppsFlyer__

#include "cocos2d.h"

class MafAppsFlyer
{
public:
    
    static MafAppsFlyer* getInstance();

    static void InitAppsFlyer(const char* devKey,const char* AppId);
//    static void TrackAppLaunch();
    
    static void TrackEvent(const char* strEvent, bool once = true);
    static void TrackEventPurchase(const char* productId, const char* price, const char* nation);
//    static void TrackEventLevel(const char* strEvent, int level);
    
//    static void SendTracking(const char* strCategory,const char* strAction,const char* strLabel,int val);
    
private:
    static void setLogEvent(const char* eventName, const char* param);
    static bool isLogEvent(const char* eventName, const char* param);
    
};

#endif /* defined(__HexagonSnake__MafAnalytics__) */
