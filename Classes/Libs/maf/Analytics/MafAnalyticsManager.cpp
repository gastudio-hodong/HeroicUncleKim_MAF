#include "Libs/maf/Analytics/MafAnalyticsManager.h"
#include "Libs/maf/MafConfig.h"
#include "Libs/maf/Analytics/MafFirebaseAnalytics.h"
#include "Libs/maf/Analytics/MafAppsFlyer.h"
#include "Libs/maf/MafFacebook.h"


void MafAnalyticsManager::InitAnalytics(){
    MafFirebaseAnalytics::InitFirebaseAnalytics();
    MafAppsFlyer::InitAppsFlyer(APPS_FLYER_DEV, APPS_FLYER_APPID);
    MafFacebook::FacebookInit();
}

void MafAnalyticsManager::LogEvent(const char* eventName, bool once)
{
    MafFirebaseAnalytics::LogEvent(eventName, kAnalParamNone, kValue_1, once);
    MafAppsFlyer::TrackEvent(eventName, once);
    MafFacebook::FacebookLog(eventName, kAnalParamNone, kValue_1);
}

void MafAnalyticsManager::LogEvent(const char* eventName, const char* param, int value, bool once)
{
    MafFirebaseAnalytics::LogEvent(eventName, param, value, once);
    MafAppsFlyer::TrackEvent(eventName, once);
    MafFacebook::FacebookLog(eventName, param, value);
}

void MafAnalyticsManager::LogEvent(const char* eventName, const char* param, const char* value, bool once)
{
    MafFirebaseAnalytics::LogEvent(eventName, param, value, once);
    MafAppsFlyer::TrackEvent(eventName, once);
    MafFacebook::FacebookLog(eventName, param, value);
}
