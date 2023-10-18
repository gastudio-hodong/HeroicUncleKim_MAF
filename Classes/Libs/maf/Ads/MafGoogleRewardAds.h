//
//  MafGoogleRewardAds.h
//  TapRed
//
//  Created by SanHeo on 2016. 8. 18..
//
//

#ifndef __TapRed__MafGoogleRewardAds__
#define __TapRed__MafGoogleRewardAds__

#include "cocos2d.h"

#include "Libs/maf/MafConfig.h"
#include "Libs/maf/Ads/MafAds.h"

class MafGoogleRewardAds : public MafAds
{
public:
    static MafGoogleRewardAds* getInstance();
    MafGoogleRewardAds();
    virtual ~MafGoogleRewardAds(void);
    
    //
    std::function<void(void)> _ResultInitCB;
    std::function<void(ADS_RESULT)> _ResultRewardCB;
    cocos2d::Node* _ResultParent;
    
    //
    static void InitAds(std::function<void(void)> cb);
    
    static void InitGoogleRewardAd(const char* strAdmobId);
    static void ShowGoogleRewardedVideo(Node* parent, std::function<void(ADS_RESULT)> cb);
    static bool isGoogleRewardedVideo();

    static void InitGoogleInterstitial(const char* strAdmobId);
    static void ShowGoogleInterstitial();
    
    static void InitGoogleBanner(const char* strAdmobId);
    static void ShowGoogleBanner();
    static void HideGoogleBanner();
    
    static void InitGoogleTopBanner(const char* strAdmobId);
    static void ShowGoogleTopBanner();
    static void HideGoogleTopBanner();
    
    static void CBReceiveAdsInit();
    static void CBReceiveAdsReward(ADS_RESULT result);
    
};
 
#endif /* defined(__TapRed__MafGoogleRewardAds__) */



