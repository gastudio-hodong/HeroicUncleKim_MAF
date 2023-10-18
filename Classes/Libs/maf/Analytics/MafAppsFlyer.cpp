//
//  MafAnalytics.cpp
//  HexagonSnake
//
//  Created by SanHeo on 2014. 9. 16..
//
//

#include "MafAppsFlyer.h"

#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    #import "MafiOSAppsFlyer.h"
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include <jni.h>
    #include "jni/JniHelper.h"
#endif

USING_NS_CC;

static MafAppsFlyer *s_MafAppsFlyer = nullptr;

MafAppsFlyer* MafAppsFlyer::getInstance(){
    if(s_MafAppsFlyer==nullptr){
        s_MafAppsFlyer = new MafAppsFlyer();
    }
    return s_MafAppsFlyer;
}

void MafAppsFlyer::InitAppsFlyer(const char* devKey,const char* AppId){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString *strKey = [NSString stringWithUTF8String:devKey];
    NSString *strApp = [NSString stringWithUTF8String:AppId];
    [[MafiOSAppsFlyer sharediOSAppsFlyerPlugin] Initialize:strKey AppId:strApp];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","InitAppsFlyer","(Ljava/lang/String;)V")){
        jstring jstr = t.env->NewStringUTF(devKey);
//        jstring jstr1 = t.env->NewStringUTF(AppId);
        t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr);
        t.env->DeleteLocalRef(jstr);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

//void MafAppsFlyer::TrackAppLaunch(){
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    [[MafiOSAppsFlyer sharediOSAppsFlyerPlugin] TrackAppLaunch];
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//   
//
//#endif
//}

void MafAppsFlyer::TrackEvent(const char* strEvent, bool once){
    
    if (once) {
        if (isLogEvent(StringUtils::format("af_%s", strEvent).c_str(), "")) {
            return;
        }else{
            setLogEvent(StringUtils::format("af_%s", strEvent).c_str(), "");
        }
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[MafiOSAppsFlyer sharediOSAppsFlyerPlugin] TrackEvent:[NSString stringWithUTF8String:strEvent]];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","AppsFlyerTrackEvent","(Ljava/lang/String;)V")){
        jstring jstr = t.env->NewStringUTF(strEvent);
        t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr);
        t.env->DeleteLocalRef(jstr);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void MafAppsFlyer::TrackEventPurchase(const char* productId, const char* price, const char* nation){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString *str1 = [NSString stringWithUTF8String:productId];
    NSString *str2 = [NSString stringWithUTF8String:price];
    NSString *str3 = [NSString stringWithUTF8String:nation];
    [[MafiOSAppsFlyer sharediOSAppsFlyerPlugin] TrackEventPurchase:str1 price:str2 currency:str3];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","AppsFlyerTrackEventPurchase","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")){
        jstring jstr = t.env->NewStringUTF(productId);
        jstring jstr2 = t.env->NewStringUTF(price);
        jstring jstr3 = t.env->NewStringUTF(nation);
        t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr,jstr2,jstr3);
        t.env->DeleteLocalRef(jstr);
        t.env->DeleteLocalRef(jstr2);
        t.env->DeleteLocalRef(jstr3);
        t.env->DeleteLocalRef(t.classID);
    }
    
#endif
}

//void MafAppsFlyer::TrackEventLevel(const char* strEvent, int level){
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    NSString *str1 = [NSString stringWithUTF8String:strEvent];
//    NSString *str2 = [NSString stringWithFormat:@"%d",level];
//    [[MafiOSAppsFlyer sharediOSAppsFlyerPlugin] TrackEventLevel:str1 level:str2];
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    
//#endif
//}



//void MafAppsFlyer::SendTracking(const char* strCategory,const char* strAction,const char* strLabel,int val){
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    NSString *str1 = [NSString stringWithUTF8String:strCategory];
//    NSString *str2 = [NSString stringWithUTF8String:strAction];
//    NSString *str3 = [NSString stringWithUTF8String:strLabel];
//    [[MafiOSAppsFlyer sharediOSAppsFlyerPlugin] SendTracking:str1 action:str2 label:str3 val:val];
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    JniMethodInfo t;
//    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","SendTracking","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V")){
//        jstring jstr = t.env->NewStringUTF(strCategory);
//        jstring jstr2 = t.env->NewStringUTF(strAction);
//        jstring jstr3 = t.env->NewStringUTF(strLabel);
//        t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr,jstr2,jstr3,val);
//        t.env->DeleteLocalRef(jstr);
//        t.env->DeleteLocalRef(jstr2);
//        t.env->DeleteLocalRef(jstr3);
//        t.env->DeleteLocalRef(t.classID);
//    }
//
//#endif
//}

void MafAppsFlyer::setLogEvent(const char* eventName, const char* param){
    UserDefault::getInstance()->setBoolForKey(StringUtils::format("%s_%s", eventName,param).c_str(), true);
}
bool MafAppsFlyer::isLogEvent(const char* eventName, const char* param){
    return UserDefault::getInstance()->getBoolForKey(StringUtils::format("%s_%s", eventName,param).c_str(), false);
}


