//
//  MafGoogleRewardAds.cpp
//  TapRed
//
//  Created by SanHeo on 2014. 8. 1..
//
//

#include "MafGoogleRewardAds.h"

#include "ManagerSystem/AdsUtilsManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #import "MafiOSGoogleRewardAds.h"
    #import "MafiOSAdxRewardAds.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include <jni.h>
    #include "jni/JniHelper.h"
#endif

static MafGoogleRewardAds *s_MafGoogleRewardAds = nullptr;
MafGoogleRewardAds* MafGoogleRewardAds::getInstance()
{
    if ( s_MafGoogleRewardAds == nullptr )
    {
        s_MafGoogleRewardAds = new MafGoogleRewardAds();
    }
    
    return s_MafGoogleRewardAds;
}

MafGoogleRewardAds::MafGoogleRewardAds() :
_ResultParent(nullptr),
_ResultInitCB(nullptr),
_ResultRewardCB(nullptr)
{
    
}

MafGoogleRewardAds::~MafGoogleRewardAds()
{

}

void MafGoogleRewardAds::InitAds(std::function<void(void)> cb)
{
    getInstance()->_ResultInitCB = cb;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[MafiOSAdxRewardAds sharediOSAdxRewardAdsPlugin] initAds];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","initAds","()V")){
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

#pragma mark - reward video
void MafGoogleRewardAds::InitGoogleRewardAd(const char* strAdmobId){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString *str = [NSString stringWithUTF8String:strAdmobId];
    [[MafiOSAdxRewardAds sharediOSAdxRewardAdsPlugin] initRewardedAd:str];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","initRewardedAd","(Ljava/lang/String;)V")){
        jstring jstr = t.env->NewStringUTF(strAdmobId);
        t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr);
        t.env->DeleteLocalRef(jstr);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void MafGoogleRewardAds::ShowGoogleRewardedVideo(Node* parent, std::function<void(ADS_RESULT)> cb)
{
    //광고 ui 노출
    if ( AdsUtilsManager::getInstance()->getAdsCooldown() > 0 )
    {
        return;
    }
    AdsUtilsManager::getInstance()->setAdsCooldown();
    
    //
    if ( MafGoogleRewardAds::isGoogleRewardedVideo() == false )
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        [[MafiOSAdxRewardAds sharediOSAdxRewardAdsPlugin] loadRewardedAd];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        cocos2d::JniHelper::callStaticVoidMethod("com.maf.iab.MafActivity", "loadRewardedAd");
#endif
        if ( cb != nullptr )
        {
            cb(ADS_RESULT::LOAD);
        }
        return;
    }
    
    //
    getInstance()->_ResultParent = parent;
    getInstance()->_ResultParent->retain();
    
    getInstance()->_ResultRewardCB = cb;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[MafiOSAdxRewardAds sharediOSAdxRewardAdsPlugin] showRewardedAd];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","showRewardedAd","()V")){
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

bool MafGoogleRewardAds::isGoogleRewardedVideo()
{
    bool bLoad = false;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    bLoad = [[MafiOSAdxRewardAds sharediOSAdxRewardAdsPlugin] isRewardedAd];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    bLoad = cocos2d::JniHelper::callStaticBooleanMethod("com.maf.iab.MafActivity", "isLoadRewardedAd");
#endif
    return bLoad;
}

#pragma mark - reward interstitial
void MafGoogleRewardAds::InitGoogleInterstitial(const char* strAdmobId)
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        NSString *str = [NSString stringWithUTF8String:strAdmobId];
        [[MafiOSAdxRewardAds sharediOSAdxRewardAdsPlugin] initInterstitial:str];
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo t;
        if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","initInterstitial","(Ljava/lang/String;)V")){
            jstring jstr = t.env->NewStringUTF(strAdmobId);
            t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr);
            t.env->DeleteLocalRef(jstr);
            t.env->DeleteLocalRef(t.classID);
        }
    #endif
}
void MafGoogleRewardAds::ShowGoogleInterstitial()
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        [[MafiOSAdxRewardAds sharediOSAdxRewardAdsPlugin] showInterstitial];
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo t;
        if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","showInterstitial","()V")){
            t.env->CallStaticVoidMethod(t.classID,t.methodID);
            t.env->DeleteLocalRef(t.classID);
        }
    #endif
}

#pragma mark - banner
void MafGoogleRewardAds::InitGoogleBanner(const char* strAdmobId){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString *str = [NSString stringWithUTF8String:strAdmobId];
    [[MafiOSAdxRewardAds sharediOSAdxRewardAdsPlugin] initBannerView:str];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","initBannerView","(Ljava/lang/String;)V")){
        jstring jstr = t.env->NewStringUTF(strAdmobId);
        t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr);
        t.env->DeleteLocalRef(jstr);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}
void MafGoogleRewardAds::ShowGoogleBanner(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[MafiOSAdxRewardAds sharediOSAdxRewardAdsPlugin] showBannerView];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","showBannerView","()V")){
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}
void MafGoogleRewardAds::HideGoogleBanner(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[MafiOSAdxRewardAds sharediOSAdxRewardAdsPlugin] hideBannerView];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","hideBannerView","()V")){
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void MafGoogleRewardAds::InitGoogleTopBanner(const char* strAdmobId){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString *str = [NSString stringWithUTF8String:strAdmobId];
    [[MafiOSAdxRewardAds sharediOSAdxRewardAdsPlugin] initTopBannerView:str];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","initTopBannerView","(Ljava/lang/String;)V")){
        jstring jstr = t.env->NewStringUTF(strAdmobId);
        t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr);
        t.env->DeleteLocalRef(jstr);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}
void MafGoogleRewardAds::ShowGoogleTopBanner(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[MafiOSAdxRewardAds sharediOSAdxRewardAdsPlugin] showTopBannerView];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","showTopBannerView","()V")){
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}
void MafGoogleRewardAds::HideGoogleTopBanner(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[MafiOSAdxRewardAds sharediOSAdxRewardAdsPlugin] hideTopBannerView];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","hideTopBannerView","()V")){
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}


#pragma mark - callback
void MafGoogleRewardAds::CBReceiveAdsInit()
{
    auto callback = getInstance()->_ResultInitCB;
    if ( callback != nullptr )
    {
        callback();
    }
}

void MafGoogleRewardAds::CBReceiveAdsReward(ADS_RESULT result)
{
    AdsUtilsManager::getInstance()->setAdsCooldown();
    
    if ( getInstance()->_ResultParent != nullptr )
    {
        if ( getInstance()->_ResultRewardCB != nullptr )
        {
            getInstance()->_ResultRewardCB(result);
            getInstance()->_ResultRewardCB = nullptr;
        }
        getInstance()->_ResultParent->release();
        getInstance()->_ResultParent = nullptr;
    }
}


#pragma mark-
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"{
    
    JNIEXPORT void JNICALL Java_com_maf_iab_MafActivity_NativeGoogleAdsInitCB(JNIEnv *env, jclass obj)
    {
        MafGoogleRewardAds::CBReceiveAdsInit();
    }

    JNIEXPORT void JNICALL Java_com_maf_iab_MafActivity_NativeGoogleAdsRewardCB(JNIEnv *env, jclass obj, int nResult)
    {
        MafGoogleRewardAds::CBReceiveAdsReward((ADS_RESULT)nResult);
    }
}
#endif




