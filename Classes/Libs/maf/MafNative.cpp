//
//  MafNative.cpp
//  TapRed
//
//  Created by SanHeo on 2014. 8. 2..
//
//

#include "MafNative.h"

#include "Common/ConfigDefault.h"

#include "ManagerGame/FriendManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #import "MafiOSNative.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include <jni.h>
    #include "jni/JniHelper.h"
#endif

USING_NS_CC;

static MafNative *s_MafNative = nullptr;
MafNative* MafNative::getInstance(){
    if(s_MafNative==nullptr){
        s_MafNative = new MafNative();
    }
    return s_MafNative;
}


void MafNative::OpenUrl(const char* url){
    
    Application::getInstance()->openURL(url);
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    NSString *strUrl = [NSString stringWithUTF8String:url];
//    [[MafiOSNative sharediOSNativePlugin] OpenUrl:strUrl];
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    JniMethodInfo t;
//    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","OpenUrl","(Ljava/lang/String;)V")){
//        jstring jstr = t.env->NewStringUTF(url);
//        t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr);
//        t.env->DeleteLocalRef(jstr);
//        t.env->DeleteLocalRef(t.classID);
//    }
//#endif

}


void MafNative::SendEmail(const char* email){
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString *strEmail = [NSString stringWithUTF8String:email];
    [[MafiOSNative sharediOSNativePlugin] SendEmail:strEmail];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","SendEmail","(Ljava/lang/String;)V")){
        jstring jstr = t.env->NewStringUTF(email);
        t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr);
        t.env->DeleteLocalRef(jstr);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
    
}

void MafNative::SendEmail(const char* email, const char* subject, const char* body){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString *strEmail = [NSString stringWithUTF8String:email];
    NSString *strSubject = [NSString stringWithUTF8String:subject];
    NSString *strBody = [NSString stringWithUTF8String:body];
    [[MafiOSNative sharediOSNativePlugin] SendEmail:strEmail subject:strSubject body:strBody];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","SendEmail","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")){
        jstring jstrEmail = t.env->NewStringUTF(email);
        jstring jstrSubject = t.env->NewStringUTF(subject);
        jstring jstrBody = t.env->NewStringUTF(body);
        t.env->CallStaticVoidMethod(t.classID,t.methodID,jstrEmail, jstrSubject, jstrBody);
        t.env->DeleteLocalRef(jstrEmail);
        t.env->DeleteLocalRef(jstrSubject);
        t.env->DeleteLocalRef(jstrBody);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void MafNative::SetLocalAlarm(){

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","SetLocalAlarm","()V")){
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
    
}

std::string MafNative::GetGameVersion(){

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return [[MafiOSNative sharediOSNativePlugin]GetGameVersion];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","GetGameVersion",
                                      "()Ljava/lang/String;")){
        jstring jret = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        std::string ret = JniHelper::jstring2string(jret);
        
        t.env->DeleteLocalRef(jret);
        t.env->DeleteLocalRef(t.classID);
        //return ret.c_str();
        return ret;
    }
#endif
    return "0";
}

void MafNative::ShowUpdateAlert(const char* title, const char* message, const char* confirm, const char* shortcut){
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString *strTitle = [NSString stringWithUTF8String:title];
    NSString *strMessage = [NSString stringWithUTF8String:message];
    NSString *strConfirm = [NSString stringWithUTF8String:confirm];
    NSString *strShortcut = [NSString stringWithUTF8String:shortcut];
    [[MafiOSNative sharediOSNativePlugin] ShowUpdateAlert:strTitle message:strMessage confirm:strConfirm shortcut:strShortcut];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","ShowUpdateAlert","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")){
        jstring jstrTitle = t.env->NewStringUTF(title);
        jstring jstrMessage = t.env->NewStringUTF(message);
        jstring jstrConfirm = t.env->NewStringUTF(confirm);
        jstring jstrShortcut = t.env->NewStringUTF(shortcut);
        
        t.env->CallStaticVoidMethod(t.classID,t.methodID, jstrTitle, jstrMessage, jstrConfirm, jstrShortcut);
        t.env->DeleteLocalRef(jstrTitle);
        t.env->DeleteLocalRef(jstrMessage);
        t.env->DeleteLocalRef(jstrConfirm);
        t.env->DeleteLocalRef(jstrShortcut);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
    
}

bool MafNative::FindIllegalProgram(){
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return false;
#else
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","FindIllegalProgram","()Z")){
        jboolean bValue = t.env->CallStaticBooleanMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
        return bValue;
    }
#endif
    return true;
}

void MafNative::CheckPermission(int permission[], int size, const char* msg){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","CheckPermission","([ILjava/lang/String;)V")){
        jstring jstr = t.env->NewStringUTF(msg);
        jintArray jintArr = t.env->NewIntArray(size);
        t.env->SetIntArrayRegion(jintArr,0,size,permission);
        
        t.env->CallStaticVoidMethod(t.classID,t.methodID,jintArr,jstr);
        t.env->DeleteLocalRef(jstr);
        t.env->DeleteLocalRef(jintArr);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

bool MafNative::IsPermission(int permission){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return false;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","IsPermission","(I)Z")){
        jboolean bValue = t.env->CallStaticBooleanMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
        return bValue;
    }
    return false;
#endif
}


void MafNative::GetAdsID()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[MafiOSNative sharediOSNativePlugin] GetAdsID];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","getAdsID", "()V")){
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

std::string MafNative::GetDeviceID()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return [[MafiOSNative sharediOSNativePlugin] GetDeviceIDFA];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","getDeviceID",
                                      "()Ljava/lang/String;")){
        jstring jret = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        std::string ret = JniHelper::jstring2string(jret);
        
        t.env->DeleteLocalRef(jret);
        t.env->DeleteLocalRef(t.classID);
        //return ret.c_str();
        return ret;
    }
#endif
}


std::string MafNative::getDeviceModel() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return [[MafiOSNative sharediOSNativePlugin] GetDeviceModel];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","GetDeviceModel",
                                      "()Ljava/lang/String;")){
        jstring jret = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        std::string ret = JniHelper::jstring2string(jret);
        
        t.env->DeleteLocalRef(jret);
        t.env->DeleteLocalRef(t.classID);
        
        return ret;
    }
#endif
}

std::string MafNative::GetDeviceInstallStore()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return "";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","GetDeviceInstallStore",
                                      "()Ljava/lang/String;")){
        jstring jret = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        std::string ret = JniHelper::jstring2string(jret);
        
        t.env->DeleteLocalRef(jret);
        t.env->DeleteLocalRef(t.classID);
        
        return ret;
    }
#endif
}

std::string MafNative::getDeviceSystemVersion() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return [[MafiOSNative sharediOSNativePlugin] GetDeviceSystemVersion];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","GetOSVersion",
                                      "()Ljava/lang/String;")){
        jstring jret = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        std::string ret = JniHelper::jstring2string(jret);
        
        t.env->DeleteLocalRef(jret);
        t.env->DeleteLocalRef(t.classID);
        //return ret.c_str();
        return ret;
    }
#endif
}

bool MafNative::IsDeviceiPhoneX()
{
    bool device = false;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    device = [[MafiOSNative sharediOSNativePlugin] isDeviceiPhoneX];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    device = false;
#endif
    
    return device;
}


bool MafNative::IsLessThanCurrVersion(std::string version){
    if(version.compare(MafNative::GetGameVersion()) == 0)
        return true;
    
    if(version.empty())
        return true;
    
    auto vecVersion = MafUtils::split(version, '.');
    auto vecCurrVersion = MafUtils::split(MafNative::GetGameVersion(), '.');
    
    CCASSERT(vecVersion.size() == vecCurrVersion.size(), "version vector size is not correct");
    
    for(int i=0; i<vecVersion.size(); i++){
        auto verF = atof(vecVersion.at(i).c_str());
        auto currVerF = atof(vecCurrVersion.at(i).c_str());
        
        if ( verF != currVerF )
        {
            if ( verF < currVerF )
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    
    return true;
}


std::string MafNative::getKeyHash()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return "tJ8yuzxvSTu/tz+n/yeob3oj4RU=";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
        return "tJ8yuzxvSTu/tz+n/yeob3oj4RU=";
    #endif
    
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","getKeyHash",
                                      "()Ljava/lang/String;")){
        jstring jret = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        std::string ret = JniHelper::jstring2string(jret);
        
        t.env->DeleteLocalRef(jret);
        t.env->DeleteLocalRef(t.classID);
        
        return ret;
    }
#endif
}

void MafNative::SendLocalNotification(int notiKey, int timeTerm, const char* notiTitle, const char* notiDesc)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString *strDesc = [NSString stringWithUTF8String:notiDesc];
    NSString *strKey = [NSString stringWithFormat: @"%d", notiKey];
    [[MafiOSNative sharediOSNativePlugin] sendLocalNotification:strKey title:strDesc time:timeTerm];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","SendLocalNotifiaction",
                                      "(ILjava/lang/String;Ljava/lang/String;I)V")){
        jstring jTitle = t.env->NewStringUTF(notiTitle);
        jstring jDesc = t.env->NewStringUTF(notiDesc);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, notiKey, jTitle, jDesc, timeTerm);
        
        t.env->DeleteLocalRef(jTitle);
        t.env->DeleteLocalRef(jDesc);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}
void MafNative::CancelLocalNotification(int notiKey)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString *strKey = [NSString stringWithFormat: @"%d", notiKey];
    [[MafiOSNative sharediOSNativePlugin] deleteLocalNotification:strKey];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","deleteNotification",
                                      "(I)V")){
        t.env->CallStaticVoidMethod(t.classID, t.methodID, notiKey);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void MafNative::CreateDeepLink(const char* paramKey, const char* paramValue)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString * strKey = [NSString stringWithFormat:@"%s", paramKey];
    NSString * strValue = [NSString stringWithFormat:@"%s", paramValue];
    
    [[MafiOSNative sharediOSNativePlugin] createDeepLink:strKey value:strValue];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","createDeepLink",
                                      "(Ljava/lang/String;Ljava/lang/String;)V")){
        jstring jKey = t.env->NewStringUTF(paramKey);
        jstring jParam = t.env->NewStringUTF(paramValue);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jKey, jParam);
        
        t.env->DeleteLocalRef(jKey);
        t.env->DeleteLocalRef(jParam);
        t.env->DeleteLocalRef(t.classID);
    }
    
    /*JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","createDeepLink",
                                      "(Ljava/lang/String;Ljava/lang/String;)V")){
        t.env->CallStaticVoidMethod(t.classID, t.methodID, paramKey, paramValue);
        t.env->DeleteLocalRef(t.classID);
    }*/
#endif
}

void MafNative::copyString(const char* text)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString* strText = [NSString stringWithFormat:@"%s", text];
    
    [[MafiOSNative sharediOSNativePlugin] copyString:strText];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","addClippboard",
                                      "(Ljava/lang/String;)V")){
        jstring jText = t.env->NewStringUTF(text);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jText);
        
        t.env->DeleteLocalRef(jText);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void MafNative::shareString(const char* text)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString* strText = [NSString stringWithFormat:@"%s", text];
    
    [[MafiOSNative sharediOSNativePlugin] shareString:strText];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","showSendIntent",
                                      "(Ljava/lang/String;)V")){
        jstring jText = t.env->NewStringUTF(text);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jText);
        
        t.env->DeleteLocalRef(jText);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void MafNative::ReceiveDeepLink()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","receiveDeepLink",
                                      "()V")){
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
    
    /*JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","createDeepLink",
                                      "(Ljava/lang/String;Ljava/lang/String;)V")){
        t.env->CallStaticVoidMethod(t.classID, t.methodID, paramKey, paramValue);
        t.env->DeleteLocalRef(t.classID);
    }*/
#endif
}

#pragma mark-
void MafNative::CBReceiveAdsId(std::string strAdsID)
{
    if ( strAdsID.length() == 0 )
    {
        return;
    }
    
    std::string strAdsIDTemp = UserDefault::getInstance()->getStringForKey("ADS_ID", "");
    if ( strAdsID.compare(strAdsIDTemp) == 0 )
    {
        return;
    }
    
    std::string url = "/advertisingid/%s";
    url = MafUtils::format(url.c_str(), strAdsID.c_str());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){

        UserDefault::getInstance()->setStringForKey("ADS_ID", strAdsID);
    };
    req->send(cb);
}

void MafNative::CBReceiveDeepLink(std::string strParams)
{
    std::vector<std::string> vec;
    std::map<std::string, std::string> map;
    
    vec = MafUtils::split(strParams, '&');
    
    for(auto info : vec)
    {
        if(info != "")
        {
            auto sp = MafUtils::split(info, '=');
            map.insert(std::pair<std::string, std::string>(sp.at(0),sp.at(1)));
        }
    }
    
    std::string os = map.find("os")->second;
    std::string userid = map.find("user_id")->second;
    
    FriendManager::getInstance()->setInviteFriendId(userid, os);
}

#pragma mark-
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"{
    
    JNIEXPORT void JNICALL Java_com_maf_iab_MafActivity_NativeAdsId(JNIEnv *env, jclass obj, jstring jAdsID)
    {
        std::string strAdsID = JniHelper::jstring2string(jAdsID);
        MafNative::CBReceiveAdsId(strAdsID);
    }

    JNIEXPORT void JNICALL Java_com_maf_iab_MafActivity_NativeReceiveDeepLinkParam(JNIEnv *env, jclass obj, jstring jParams)
    {
        std::string strParams = JniHelper::jstring2string(jParams);
        //std::string strParamValue = JniHelper::jstring2string(jparamValue);
        
        MafNative::CBReceiveDeepLink(strParams);
        
    }
    
}
#endif
