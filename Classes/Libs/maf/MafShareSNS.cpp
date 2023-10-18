//
//  MafShareSNS.cpp
//  HexagonSnake
//
//  Created by SanHeo on 2014. 9. 3..
//
//

#include "MafShareSNS.h"

#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    #import "MafiOSShareSNS.h"
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include <jni.h>
    #include "jni/JniHelper.h"
#endif

USING_NS_CC;

static MafShareSNS *s_MafShareSNS = nullptr;

MafShareSNS* MafShareSNS::getInstance(){
    if(s_MafShareSNS==nullptr){
        s_MafShareSNS = new MafShareSNS();
    }
    return s_MafShareSNS;
}


void MafShareSNS::PostFacebook(const char* strFileUrl,const char* strTitle,const char* strLinkUrl){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString* fileurl = [NSString stringWithUTF8String:strFileUrl];
    NSString* title = [NSString stringWithUTF8String:strTitle];
    NSString* linkurl = [NSString stringWithUTF8String:strLinkUrl];
    [[MafiOSShareSNS sharediOSMafiOSShareSNSPlugin] PostFacebook:fileurl title:title link:linkurl];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","PostFacebook","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")){
        jstring jstr = t.env->NewStringUTF(strFileUrl);
        jstring jstr2 = t.env->NewStringUTF(strTitle);
        jstring jstr3 = t.env->NewStringUTF(strLinkUrl);
        t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr,jstr2,jstr3);
        t.env->DeleteLocalRef(jstr);
        t.env->DeleteLocalRef(jstr2);
        t.env->DeleteLocalRef(jstr3);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void MafShareSNS::PostTwitter(const char* strFileUrl,const char* strTitle,const char* strLinkUrl){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString* fileurl = [NSString stringWithUTF8String:strFileUrl];
    NSString* title = [NSString stringWithUTF8String:strTitle];
    NSString* linkurl = [NSString stringWithUTF8String:strLinkUrl];
    [[MafiOSShareSNS sharediOSMafiOSShareSNSPlugin] PostTwitter:fileurl title:title link:linkurl];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    JniMethodInfo t;
//    if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","PostFacebook","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")){
//        jstring jstr = t.env->NewStringUTF(strFileUrl);
//        jstring jstr2 = t.env->NewStringUTF(strTitle);
//        jstring jstr3 = t.env->NewStringUTF(strLinkUrl);
//        t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr,jstr2,jstr3);
//        t.env->DeleteLocalRef(jstr);
//        t.env->DeleteLocalRef(jstr2);
//        t.env->DeleteLocalRef(jstr3);
//        t.env->DeleteLocalRef(t.classID);
//    }
#endif
}

void MafShareSNS::PostFacebookFromLocal(cocos2d::Size captureSize,const char* strTitle,const char* strLinkUrl){

    getInstance()->_SNS         = SNS::FACEBOOK;
    getInstance()->_strTitle    = strTitle;
    getInstance()->_strLinkUrl  = strLinkUrl;
    
    CaptureScreen(captureSize);
}

void MafShareSNS::PostTwitterFromLocal(cocos2d::Size captureSize, const char* strTitle,const char* strLinkUrl){

    getInstance()->_SNS         = SNS::TWITTER;
    getInstance()->_strTitle    = strTitle;
    getInstance()->_strLinkUrl  = strLinkUrl;
    
    CaptureScreen(captureSize);
}

void MafShareSNS::PostSNSAtferCapture(RenderTexture* rt,const std::string& str){
    
    std::string path = FileUtils::getInstance()->getWritablePath() + "capture";
    Data data= FileUtils::getInstance()->getDataFromFile(path);
    const char* strData= (const char*)data.getBytes();
    int nDataSize = (int)data.getSize();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSString* title = [NSString stringWithUTF8String:getInstance()->_strTitle.c_str()];
    NSString* linkurl = [NSString stringWithUTF8String:getInstance()->_strLinkUrl.c_str()];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#endif
    
    if(getInstance()->_SNS == SNS::FACEBOOK){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        [[MafiOSShareSNS sharediOSMafiOSShareSNSPlugin] PostFacebookFromLocal:strData size:nDataSize title:title link:linkurl];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#endif
    }
    else if(getInstance()->_SNS == SNS::TWITTER){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        [[MafiOSShareSNS sharediOSMafiOSShareSNSPlugin] PostTwitterFromLocal:strData size:nDataSize title:title link:linkurl];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#endif
    }
}

void MafShareSNS::CaptureScreen(cocos2d::Size captureSize){

    RenderTexture *texture = RenderTexture::create(captureSize.width, captureSize.height);
    texture->begin();
    Director::getInstance()->getRunningScene()->visit();
    texture->end();
    
    texture->saveToFile("capture",cocos2d::Image::Format::PNG,true,PostSNSAtferCapture);
}


/*RenderTexture *texture = RenderTexture::create(visibleSize.width, visibleSize.height);
 texture->setPosition(visibleSize.width/2,visibleSize.height/2);
 texture->begin();
 Director::getInstance()->getRunningScene()->visit();
 texture->end();
 bool bSave = texture->saveToFile("capture");
 if(bSave)
 {
 std::string path = FileUtils::getInstance()->getWritablePath() + "capture";
 Data data= FileUtils::getInstance()->getDataFromFile(path);
 unsigned char* uData= data.getBytes();
 int nSize = data.getSize();
 MafShareSNS::PostFacebookFromLocal((const char*)uData,nSize, (const char*)charTitle,FACEBOOK_POST_URL_LINK);
 }*/

void MafShareSNS::OpenFacebookPage(const char* pageName){
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//        NSString *strUrl = [NSString stringWithUTF8String:pageName];
//        [[MafiOSNative sharediOSNativePlugin] OpenUrl:strUrl];
    
    
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo t;
        if(JniHelper::getStaticMethodInfo(t,"com.maf.iab.MafActivity","OpenFacebookPage","(Ljava/lang/String;)V")){
            jstring jstr = t.env->NewStringUTF(pageName);
            t.env->CallStaticVoidMethod(t.classID,t.methodID,jstr);
            t.env->DeleteLocalRef(jstr);
            t.env->DeleteLocalRef(t.classID);
        }
    #endif
}


