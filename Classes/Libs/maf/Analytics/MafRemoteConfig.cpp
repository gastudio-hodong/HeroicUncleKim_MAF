//
//  MafAnalytics.cpp
//  HexagonSnake
//
//  Created by SanHeo on 2014. 9. 16..
//
//

#include "MafRemoteConfig.h"

#include <external/json/document.h>
#include <external/json/writer.h>
#include <external/json/stringbuffer.h>

#include "Libs/maf/MafUtils.h"

#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    #import "MafiOSRemoteConfig.h"
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include <jni.h>
    #include "jni/JniHelper.h"
#endif

USING_NS_CC;

static MafRemoteConfig *s_MafRemoteConfiginstance = nullptr;
MafRemoteConfig* MafRemoteConfig::getInstance()
{
    if ( s_MafRemoteConfiginstance == nullptr )
    {
        s_MafRemoteConfiginstance = new MafRemoteConfig();
    }
    
    return s_MafRemoteConfiginstance;
}

void MafRemoteConfig::InitRemoteConfigAndLoadDefaultData(std::string file){
    
    LoadRemoteConfigData(file);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[MafiOSRemoteConfig sharedInstance] InitRemoteConfig];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","InitRemoteConfig","()V")){
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void MafRemoteConfig::LoadRemoteConfigData(std::string file){
    
    auto instance = getInstance();
    instance->_data.clear();
    
    std::string txt = FileUtils::getInstance()->getStringFromFile(file);
    rapidjson::Document jsondoc;
    jsondoc.ParseInsitu<0>((char*)(const char*)txt.c_str()).HasParseError();
    
    for(rapidjson::Value::ConstMemberIterator iter = jsondoc.MemberBegin(); iter != jsondoc.MemberEnd(); iter++){
        if(iter->value.IsInt())
            instance->_data.insert(std::pair<std::string, std::string>(iter->name.GetString(), MafUtils::toString(iter->value.GetInt())));
        else if(iter->value.IsBool())
            instance->_data.insert(std::pair<std::string, std::string>(iter->name.GetString(),MafUtils::toString(iter->value.GetBool()).c_str()));
        else if(iter->value.IsString())
            instance->_data.insert(std::pair<std::string, std::string>(iter->name.GetString(),iter->value.GetString()));
    }
}

#pragma mark - is
bool MafRemoteConfig::isRemoteConfigValue(std::string key)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return [[MafiOSRemoteConfig sharedInstance] IsRemoteConfigValue : [NSString stringWithUTF8String:key.c_str()]];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if ( JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","isRemoteConfigValue","(Ljava/lang/String;)Z") )
    {
        jstring jKey = t.env->NewStringUTF(key.c_str());
        
        jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, jKey);
        t.env->DeleteLocalRef(jKey);
        t.env->DeleteLocalRef(t.classID);
        return ret;
    }
#endif
}
 
#pragma mark - get
std::string MafRemoteConfig::GetRemoteValueString(std::string key)
{
    if(!isRemoteConfigValue(key))
        return getInstance()->_data.at(key);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return [[MafiOSRemoteConfig sharedInstance] GetRemoteConfigValueString : [NSString stringWithUTF8String:key.c_str()]];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if ( JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","GetRemoteConfigValueString","(Ljava/lang/String;)Ljava/lang/String;") )
    {
        jstring jKey = t.env->NewStringUTF(key.c_str());
        
        jstring jret = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID,jKey);
        std::string ret = cocos2d::StringUtils::getStringUTFCharsJNI(t.env, jret);
        t.env->DeleteLocalRef(jKey);
        t.env->DeleteLocalRef(t.classID);
        return ret;
    }
#endif
}

int MafRemoteConfig::GetRemoteValueInt(std::string key)
{
    if(!isRemoteConfigValue(key))
        return MafUtils::toInt(getInstance()->_data.at(key));
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return [[MafiOSRemoteConfig sharedInstance] GetRemoteConfigValueInt : [NSString stringWithUTF8String:key.c_str()]];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if ( JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","GetRemoteValueInt","(Ljava/lang/String;)I") )
    {
        jstring jKey = t.env->NewStringUTF(key.c_str());
        
        jint ret = t.env->CallStaticIntMethod(t.classID, t.methodID, jKey);
        t.env->DeleteLocalRef(jKey);
        t.env->DeleteLocalRef(t.classID);
        return ret;
    }
#endif
}

bool MafRemoteConfig::GetRemoteValueBool(std::string key)
{
    if(!isRemoteConfigValue(key))
        return MafUtils::toInt(getInstance()->_data.at(key));
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return [[MafiOSRemoteConfig sharedInstance] GetRemoteConfigValueBool : [NSString stringWithUTF8String:key.c_str()]];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if ( JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","GetRemoteValueBool","(Ljava/lang/String;)Z") )
    {
        jstring jKey = t.env->NewStringUTF(key.c_str());
        
        jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, jKey);
        t.env->DeleteLocalRef(jKey);
        t.env->DeleteLocalRef(t.classID);
        return ret;
    }
#endif
}


