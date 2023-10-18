//
//  MafFacebook.cpp
//  FantasyClicker
//
//  Created by SanHeo on 2015. 9. 9..
//
//

#include "Libs/maf/MafFacebook.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #import "MafiOSFacebook.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include <jni.h>
    #include "jni/JniHelper.h"
#endif

USING_NS_CC;

static MafFacebook *s_MafFacebook = nullptr;
MafFacebook* MafFacebook::getInstance(){
    if(s_MafFacebook==nullptr){
        s_MafFacebook = new MafFacebook();
    }
    return s_MafFacebook;
}

MafFacebook::MafFacebook(){
}

void MafFacebook::FacebookInit(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[MafiOSFacebook sharediOSFacebookPlugin] InitFacebook];
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","FacebookInit","()V")){
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void MafFacebook::FacebookLog(const char *strEvent, const char* strParam, int value)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString *str1 = [NSString stringWithUTF8String:strEvent];
    NSString *str2 = [NSString stringWithUTF8String:strParam];
    
    [[MafiOSFacebook sharediOSFacebookPlugin] FacebookLog:str1 param:str2 val_int:value];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","FacebookLog", "(Ljava/lang/String;Ljava/lang/String;I)V"))
    {
        jstring jstrEvent = t.env->NewStringUTF(strEvent);
        jstring jstrParam = t.env->NewStringUTF(strParam);
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jstrEvent, jstrParam, value);
        t.env->DeleteLocalRef(jstrEvent);
        t.env->DeleteLocalRef(jstrParam);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void MafFacebook::FacebookLog(const char *strEvent, const char* strParam, const char* strValue)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString *str1 = [NSString stringWithUTF8String:strEvent];
    NSString *str2 = [NSString stringWithUTF8String:strParam];
    NSString *str3 = [NSString stringWithUTF8String:strValue];
    
    [[MafiOSFacebook sharediOSFacebookPlugin] FacebookLog:str1 param:str2 val_str:str3];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","FacebookLog", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring jstrEvent = t.env->NewStringUTF(strEvent);
        jstring jstrParam = t.env->NewStringUTF(strParam);
        jstring jstrValue = t.env->NewStringUTF(strValue);
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jstrEvent, jstrParam, jstrValue);
        t.env->DeleteLocalRef(jstrEvent);
        t.env->DeleteLocalRef(jstrParam);
        t.env->DeleteLocalRef(jstrValue);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}
