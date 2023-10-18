//
//  MafFirebaseAnalytics.cpp
//  FantasyClicker
//
//  Created by SanHeo on 2016. 9. 6..
//
//

#include "Libs/maf/Analytics/MafFirebaseAnalytics.h"

#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    #import "MafiOSFirebaseAnalytics.h"
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include <jni.h>
    #include "jni/JniHelper.h"
#endif

USING_NS_CC;

void MafFirebaseAnalytics::InitFirebaseAnalytics(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[MafiOSFirebaseAnalytics sharediOSFirebaseAnalyticsPlugin] InitFirebaseAnalytics];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","InitFirebaseAnalytics","()V")){
        t.env->CallStaticVoidMethod(t.classID,t.methodID);        
        t.env->DeleteLocalRef(t.classID);
    }
#endif
    
}


void MafFirebaseAnalytics::LogEvent(const char* eventName, const char* param, int value, bool once)
{
    if (once) {
        if (isLogEvent(eventName, param)) {
            return;
        }else{
            setLogEvent(eventName, param);
        }
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString *str1 = [NSString stringWithUTF8String:eventName];
    NSString *str2 = [NSString stringWithUTF8String:param];
    
    [[MafiOSFirebaseAnalytics sharediOSFirebaseAnalyticsPlugin] LogEvent:str1 param:str2 value_int:value];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","LogEvent","(Ljava/lang/String;Ljava/lang/String;I)V")){
        jstring jstr = t.env->NewStringUTF(eventName);
        jstring jstr2 = t.env->NewStringUTF(param);
        t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr,jstr2,value);
        t.env->DeleteLocalRef(jstr);
        t.env->DeleteLocalRef(jstr2);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void MafFirebaseAnalytics::LogEvent(const char* eventName, const char* param, const char* value, bool once)
{
    if (once) {
        if (isLogEvent(eventName, param)) {
            return;
        }else{
            setLogEvent(eventName, param);
        }
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString *str1 = [NSString stringWithUTF8String:eventName];
    NSString *str2 = [NSString stringWithUTF8String:param];
    NSString *str3 = [NSString stringWithUTF8String:value];
    
    [[MafiOSFirebaseAnalytics sharediOSFirebaseAnalyticsPlugin] LogEvent:str1 param:str2 value_str:str3];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","LogEvent","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")){
        jstring jstr1 = t.env->NewStringUTF(eventName);
        jstring jstr2 = t.env->NewStringUTF(param);
        jstring jstr3 = t.env->NewStringUTF(value);
        t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr1,jstr2,jstr3);
        t.env->DeleteLocalRef(jstr1);
        t.env->DeleteLocalRef(jstr2);
        t.env->DeleteLocalRef(jstr3);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void MafFirebaseAnalytics::setLogEvent(const char* eventName, const char* param){
    UserDefault::getInstance()->setBoolForKey(StringUtils::format("%s_%s", eventName,param).c_str(), true);
}
bool MafFirebaseAnalytics::isLogEvent(const char* eventName, const char* param){
    return UserDefault::getInstance()->getBoolForKey(StringUtils::format("%s_%s", eventName,param).c_str(), false);
}



