//
//  MafGooglePlay.cpp
//  TapRed
//
//  Created by SanHeo on 2014. 8. 1..
//
//

#include "MafGooglePlay.h"

#include "Libs/maf/MafConfig.h"
#include "Libs/maf/MafAes256.h"

#include "Libs/maf/Analytics/MafAppsFlyer.h"

#include "ManagerGame/CashManager.h"
#include "ManagerGame/AccountManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #import "MafiOSGameCenter.h"
    #import "MafiOSInAppPlugin.h"
    #import "MafiOSGoogleRewardAds.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include <jni.h>
    #include "jni/JniHelper.h"
#endif

USING_NS_CC;

static MafGooglePlay *s_MafGooglePlay = nullptr;
MafGooglePlay* MafGooglePlay::getInstance(){
    if(s_MafGooglePlay==nullptr){
        s_MafGooglePlay = new MafGooglePlay();
    }
    return s_MafGooglePlay;
}

#pragma mark -

MafGooglePlay::MafGooglePlay() :
_strResponseData(""),
_strSignature(""),

//
_callbackPurchase(nullptr),
_callbackPurchaseHas(nullptr),

_ResultReviewCB(nullptr)
{
    
}

#pragma mark -

void MafGooglePlay::SetupGooglePlus(){
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    [[MafiOSGameCenter sharediOSGameCenterPlugin] performSelectorOnMainThread:@selector(initGameCenter) withObject:nil waitUntilDone:true];
    [[MafiOSGameCenter sharediOSGameCenterPlugin] initGameCenter];
#else
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","SetupGooglePlus","()V")){
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void MafGooglePlay::LoginGameCenter(){
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[MafiOSGameCenter sharediOSGameCenterPlugin] loginGameCenter];
#else
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","LoginGooglePlus","()V")){
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void MafGooglePlay::ReLoginGameCenter(){
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[MafiOSGameCenter sharediOSGameCenterPlugin] reloginGameCenter];
#else
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","ReLoginGooglePlus","()V")){
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void MafGooglePlay::CBReceiveLogin(bool result, std::string strID)
{
    AccountManager::getInstance()->callbackSocialLogin(result, strID);
}

bool MafGooglePlay::IsSignedIn(){
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return [[MafiOSGameCenter sharediOSGameCenterPlugin] isSignedIn];
#else
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","IsSignedIn","()Z")){
        jboolean bValue = t.env->CallStaticBooleanMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
        return bValue;
    }
#endif
    return true;
}

void MafGooglePlay::ShowLeaderBoardAll(){
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString *lbid = [NSString stringWithUTF8String:""];
    [[MafiOSGameCenter sharediOSGameCenterPlugin] showLeaderBoard:lbid];
#else
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","ShowLeaderBoardAll","()V")){
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
    
}

void MafGooglePlay::ShowLeaderBoard(const char* lb_id){
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString *lbid = [NSString stringWithUTF8String:lb_id];
    [[MafiOSGameCenter sharediOSGameCenterPlugin] showLeaderBoard:lbid];
#else
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","ShowLeaderBoard","(Ljava/lang/String;)V")){
        jstring jstr = t.env->NewStringUTF(lb_id);
        t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr);
        t.env->DeleteLocalRef(jstr);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
    
}


void MafGooglePlay::UpdateLeaderBoardScore(const char* lb_id,int score){
    
    if(!IsSignedIn())
        return;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString *lbid = [NSString stringWithUTF8String:lb_id];
    [[MafiOSGameCenter sharediOSGameCenterPlugin] updateLeaderBoardScore:lbid score:score];
#else
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","UpdateLeaderBoardScore","(Ljava/lang/String;I)V")){
        jstring jstr = t.env->NewStringUTF(lb_id);
        t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr,score);
        t.env->DeleteLocalRef(jstr);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void MafGooglePlay::ShowAchievements(){
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[MafiOSGameCenter sharediOSGameCenterPlugin] showAchievement];
#else
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","ShowAchievements","()V")){
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}
void MafGooglePlay::UnlockAchievement(const char* ach_id,int point,int clearPoint){
    
    if(point<clearPoint)
        return;
    
    if(!IsSignedIn())
        return;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString *achid = [NSString stringWithUTF8String:ach_id];
    [[MafiOSGameCenter sharediOSGameCenterPlugin] incrementAchievement:achid point:100 clearPoint:100];
#else
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","UnlockAchievement","(Ljava/lang/String;)V")){
        jstring jstr = t.env->NewStringUTF(ach_id);
        t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr);
        t.env->DeleteLocalRef(jstr);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void MafGooglePlay::UnlockProgressAchievement(const char* ach_id,int point,int clearPoint){
    
    if(!IsSignedIn())
        return;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    int nRest = cocos2d::UserDefault::getInstance()->getIntegerForKey(ach_id, 0);
    nRest+=point;
    int nValue = nRest/((double)clearPoint*0.01);
    int restValue = nRest%(int)((double)clearPoint*0.01);
    cocos2d::UserDefault::getInstance()->setIntegerForKey(ach_id, restValue);
    if(nValue < 1)
        return;
    int percent = (double)clearPoint*nValue*0.01;
    
    NSString *achid = [NSString stringWithUTF8String:ach_id];
    [[MafiOSGameCenter sharediOSGameCenterPlugin] unlockProgressAchievement:achid point:percent clearPoint:clearPoint];
#else
    if(point<clearPoint)
        return;
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","UnlockAchievement","(Ljava/lang/String;)V")){
        jstring jstr = t.env->NewStringUTF(ach_id);
        t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr);
        t.env->DeleteLocalRef(jstr);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void MafGooglePlay::IncrementAchievement(const char* ach_id,int point,int clearPoint){
    
    if(!IsSignedIn())
        return;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    int nRest = cocos2d::UserDefault::getInstance()->getIntegerForKey(ach_id, 0);
    nRest+=point;
    int nValue = nRest/((double)clearPoint*0.01);
    int restValue = (nRest*10000)%(int)((double)clearPoint*0.01*10000);
    cocos2d::UserDefault::getInstance()->setIntegerForKey(ach_id, restValue);
    if(nValue < 1)
        return;
    int percent = (double)clearPoint*nValue*0.01;
    
    NSString *achid = [NSString stringWithUTF8String:ach_id];
    [[MafiOSGameCenter sharediOSGameCenterPlugin] incrementAchievement:achid point:percent clearPoint:clearPoint];
#else
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","IncrementAchievement","(Ljava/lang/String;I)V")){
        jstring jstr = t.env->NewStringUTF(ach_id);
        //t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr,point,clearPoint);
        t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr,point);
        t.env->DeleteLocalRef(jstr);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}
void MafGooglePlay::IncrementImmediateAchievement(const char* ach_id,int point,int clearPoint){
    if(!IsSignedIn())
        return;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //    int nRest = point;//cocos2d::UserDefault::getInstance()->getIntegerForKey(ach_id, 0);
    //    nRest+=point;
    //    int nValue = nRest/((double)clearPoint*0.01);
    //    int restValue = nRest%(int)((double)clearPoint*0.01);
    //    cocos2d::UserDefault::getInstance()->setIntegerForKey(ach_id, restValue);
    //    if(nValue < 1)
    //        return;
    //    int percent = (double)clearPoint*nValue*0.01;
    
    NSString *achid = [NSString stringWithUTF8String:ach_id];
    [[MafiOSGameCenter sharediOSGameCenterPlugin] IncrementImmediateAchievement:achid point:point clearPoint:clearPoint];
#else
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","IncrementImmediateAchievement","(Ljava/lang/String;I)V")){
        jstring jstr = t.env->NewStringUTF(ach_id);
        //t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr,point,clearPoint);
        t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr,point);
        t.env->DeleteLocalRef(jstr);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}



#pragma mark-
void MafGooglePlay::PurchaseRestore()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[MafiOSInAppPlugin sharediOSInAppPlugin] restoreProductData];
#endif
}
void MafGooglePlay::Purchase(const char* strProductIdReal, const char* strProductIdWatch, const char* strPrice, const char* strPriceCurrencyCode, std::function<void(RESULT, std::string, std::string)> cb)
{
    getInstance()->_callbackPurchase = cb;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    NSString *productId = [NSString stringWithUTF8String:strProductIdReal];
    NSString *productId2 = [NSString stringWithUTF8String:strProductIdWatch];
    [[MafiOSInAppPlugin sharediOSInAppPlugin] requestProductPurchase:productId watchingId:productId2];
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","Purchase","(Ljava/lang/String;)V")){
        jstring jProductId = t.env->NewStringUTF(strProductIdReal);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jProductId);
        t.env->DeleteLocalRef(jProductId);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
    
}

void MafGooglePlay::PurchaseLoad(std::vector<std::string> listProductId)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSMutableSet *setProductId = [NSMutableSet setWithCapacity:listProductId.size()];
    
    for ( std::string str : listProductId )
    {
        [setProductId addObject:[NSString stringWithUTF8String:str.c_str()]];
    }
    
    [[MafiOSInAppPlugin sharediOSInAppPlugin] requestProductLoad:setProductId];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","PurchaseLoad","([Ljava/lang/String;)V")){
        jobjectArray jProductIdArray = t.env->NewObjectArray(listProductId.size(), t.env->FindClass("java/lang/String"), NULL);
        for ( int i = 0; i < listProductId.size(); i++ )
        {
            std::string strProductId = listProductId.at(i);
            
            jstring jstrProductId = t.env->NewStringUTF(strProductId.c_str());
            t.env->SetObjectArrayElement(jProductIdArray, i, jstrProductId);
        }
        
        t.env->CallStaticVoidMethod(t.classID,t.methodID,jProductIdArray);
        t.env->DeleteLocalRef(jProductIdArray);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void MafGooglePlay::PurchaseHas(const char* strProductIdReal,const char* strProductIdWatch, std::function<void(int)> cb)
{
    getInstance()->_callbackPurchaseHas = cb;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        
    NSString *productId = [NSString stringWithUTF8String:strProductIdReal];
    NSString *productId2 = [NSString stringWithUTF8String:strProductIdWatch];
    [[MafiOSInAppPlugin sharediOSInAppPlugin] requestProductHas:productId watchingId:productId2];
        
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","PurchaseHas","(Ljava/lang/String;)V")){
        jstring jProductId = t.env->NewStringUTF(strProductIdReal);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jProductId);
        t.env->DeleteLocalRef(jProductId);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void MafGooglePlay::PurchaseConsume(const char* strProductIdReal,const char* strProductIdWatch)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    NSString *productId = [NSString stringWithUTF8String:strProductIdReal];
    NSString *productId2 = [NSString stringWithUTF8String:strProductIdWatch];
    [[MafiOSInAppPlugin sharediOSInAppPlugin] consumeTransaction:productId watchingId:productId2];
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","PurchaseConsume","(Ljava/lang/String;)V")){
        jstring jstr = t.env->NewStringUTF(strProductIdReal);
        t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr);
        t.env->DeleteLocalRef(jstr);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void MafGooglePlay::PurchaseConsumeAll()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[MafiOSInAppPlugin sharediOSInAppPlugin] consumeTransactionAll];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","PurchaseConsumeAll","()V")){
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}



void MafGooglePlay::CBReceivePurchase(RESULT result, std::string data, std::string signature)
{
    if ( getInstance()->_callbackPurchase != nullptr )
    {
        getInstance()->_callbackPurchase(result, data, signature);
    }
}

void MafGooglePlay::CBReceivePurchaseLoad(std::string strProductID, std::string strProductPrice, std::string strPriceCurrencyCode)
{
    CashManager::getInstance()->setProductPrice(strProductID, strProductPrice, strPriceCurrencyCode);
}

void MafGooglePlay::CBReceivePurchaseHas(int result)
{
    if ( getInstance()->_callbackPurchaseHas != nullptr )
    {
        getInstance()->_callbackPurchaseHas(result);
    }
}

#pragma mark - InAppReview
void MafGooglePlay::InAppReview(std::function<void(RESULT)> cb)
{
    getInstance()->_ResultReviewCB = cb;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[MafiOSInAppPlugin sharediOSInAppPlugin] InAppReview];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    
    if (JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","InAppReview","()V"))
    {
        t.env->CallStaticVoidMethod(t.classID,t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void MafGooglePlay::CBReceiveInAppReview(RESULT result)
{
    if (getInstance()->_ResultReviewCB != nullptr)
    {
        getInstance()->_ResultReviewCB(result);
    }
}

#pragma mark-
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"{
    
    JNIEXPORT void JNICALL Java_com_maf_iab_MafActivity_NativePurchaseResultCB(JNIEnv *env, jclass obj, int nResult, jstring jData, jstring jSignature)
    {
        std::string signature = JniHelper::jstring2string(jSignature);
        std::string data = JniHelper::jstring2string(jData);
        
        
        MafGooglePlay::RESULT eResult = (MafGooglePlay::RESULT)nResult;
        if ( eResult == MafGooglePlay::RESULT::SUCCESS)
        {
            MafGooglePlay::CBReceivePurchase(eResult, data, signature);
        }
        else
        {
            MafGooglePlay::CBReceivePurchase(eResult, "", "");
        }
    }
    
    JNIEXPORT void JNICALL Java_com_maf_iab_MafActivity_NativePurchaseResultLoadCB(JNIEnv *env, jclass obj, jstring jProductID, jstring jPrice, jstring jPriceCurrencyCode)
    {
        std::string productID = cocos2d::StringUtils::getStringUTFCharsJNI(env, jProductID);
        std::string price = cocos2d::StringUtils::getStringUTFCharsJNI(env, jPrice);
        std::string priceCurrencyCode = cocos2d::StringUtils::getStringUTFCharsJNI(env, jPriceCurrencyCode);

        
        MafGooglePlay::CBReceivePurchaseLoad(productID, price, priceCurrencyCode);
    }

    JNIEXPORT void JNICALL Java_com_maf_iab_MafActivity_NativePurchaseResultHasCB(JNIEnv *env, jclass obj, int nResult)
    {
        MafGooglePlay::CBReceivePurchaseHas(nResult);
    }
    
    JNIEXPORT void JNICALL Java_com_maf_iab_MafActivity_NativeGoogleGamePlayerId(JNIEnv *env, jclass obj, jstring jData)
    {
        std::string responseData = JniHelper::jstring2string(jData);
        
        if ( responseData.length() != 0 )
        {
            MafGooglePlay::CBReceiveLogin(true, responseData);
        }
        else
        {
            MafGooglePlay::CBReceiveLogin(false, "");
        }
    }

    JNIEXPORT void JNICALL Java_com_maf_iab_MafActivity_NativeInAppReviewCB(JNIEnv *env, jclass obj, int nResult)
    {
        MafGooglePlay::RESULT res = (MafGooglePlay::RESULT)nResult;
        MafGooglePlay::CBReceiveInAppReview(res);
    }
    
}
#endif





