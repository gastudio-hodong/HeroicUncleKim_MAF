//
//  MafAds.cpp
//  FantasyClicker-mobile
//
//  Created by maf on 16/09/2019.
//

#include "MafAds.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "jni/JniHelper.h"
#endif

static MafAds *s_MafAdsInstance = nullptr;
MafAds* MafAds::getInstance()
{
    if(s_MafAdsInstance==nullptr)
    {
        s_MafAdsInstance = new MafAds();
    }
    return s_MafAdsInstance;
}


void MafAds::initAds(){}
void MafAds::showAds(){}
