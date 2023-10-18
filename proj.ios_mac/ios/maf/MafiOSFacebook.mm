//
//  MafiOSFacebook.m
//  FantasyClicker
//
//  Created by SanHeo on 2016. 9. 6..
//
//
#import "MafiOSFacebook.h"
#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import "AppController.h"

#import "MafRewardAdsManager.h"

//@interface MafiOSFacebook () <FBRewardedVideoAdDelegate>
//@property (nonatomic, strong) FBRewardedVideoAd *rewardedVideoAd;
//@property (nonatomic, strong) NSString * placementId;
//@property Boolean _bAdsWatchFinish;
//@end

@implementation MafiOSFacebook

+ (MafiOSFacebook*) sharediOSFacebookPlugin
{
    static MafiOSFacebook* pInstance;
    if(pInstance == NULL){
        pInstance = [[MafiOSFacebook alloc] init];
    }
    return pInstance;
}

- (void) InitFacebook
{
    
}

- (void) FacebookLog:(NSString*)strEvent param:(NSString*)strParam val_int:(int)value
{
    if ( strParam.length == 0 ) {
        [[FBSDKAppEvents shared] logEvent:strEvent];
    } else {
        NSDictionary *params = @{strParam : [NSNumber numberWithInt:value]};
        [[FBSDKAppEvents shared] logEvent:strEvent parameters:params];
    }
}

- (void) FacebookLog:(NSString*)strEvent param:(NSString*)strParam val_str:(NSString*)value
{
    if ( strParam.length == 0 ) {
        [[FBSDKAppEvents shared] logEvent:strEvent];
    } else {
        NSDictionary *params = @{strParam : value};
        [[FBSDKAppEvents shared] logEvent:strEvent parameters:params];
    }
}

//- (void)rewardedVideoAd:(FBRewardedVideoAd *)rewardedVideoAd didFailWithError:(NSError *)error
//{
//    NSLog(@"FB Ads - Rewarded video ad failed to load");
//    
//    self._bAdsWatchFinish = false;
//}
//
//- (void)rewardedVideoAdDidLoad:(FBRewardedVideoAd *)rewardedVideoAd
//{
//    NSLog(@"FB Ads - Video ad is loaded and ready to be displayed");
//}
//
//- (void)rewardedVideoAdDidClick:(FBRewardedVideoAd *)rewardedVideoAd
//{
//    NSLog(@"FB Ads - Video ad clicked");
//}
//
//- (void)rewardedVideoAdVideoComplete:(FBRewardedVideoAd *)rewardedVideoAd;
//{
//    NSLog(@"FB Ads - Rewarded Video ad video complete - this is called after a full video view, before the ad end card is shown. You can use this event to initialize your reward");
//    
//    self._bAdsWatchFinish = true;
//    
//    
//    [self loadFacebookAds];
//}
//          
//- (void)rewardedVideoAdDidClose:(FBRewardedVideoAd *)rewardedVideoAd
//{
//        NSLog(@"Rewarded Video ad closed - this can be triggered by closing the application, or closing the video end card");
//    
//    NSLog(@"%@", self._bAdsWatchFinish ? @"true":@"false");
//    if(self._bAdsWatchFinish == false)
//    {
//        MafRewardAdsManager::getInstance()->CBReceiveRewardAds(ADS_RESULT::BACKGROUND_SKIP);
//        
//        [self loadFacebookAds];
//    }
//    else if(self._bAdsWatchFinish == true)
//    {
//        MafRewardAdsManager::getInstance()->CBReceiveRewardAds(ADS_RESULT::SUCCESS);
//        
//        [self loadFacebookAds];
//    }
//    
//    //
//    [[UIApplication sharedApplication] setIdleTimerDisabled:YES];
//}
//
//- (void)showFacebookAds
//{
//    NSLog(@"FB ADS SHOW!!!");
//    
//    self._bAdsWatchFinish = false;
//    
//    if (self.rewardedVideoAd && self.rewardedVideoAd.isAdValid) {
//        AppController *app = GetAppController();
//        [self.rewardedVideoAd showAdFromRootViewController:(UIViewController*)app.viewController];
//    }else
//    {
//        MafRewardAdsManager::getInstance()->CBReceiveRewardAds(ADS_RESULT::LOAD);
//    }
//}
//
//- (void)loadFacebookAds
//{
//    if(self.rewardedVideoAd != nullptr)
//    {
//        self.rewardedVideoAd = nullptr;
//    }
//    
//    self.rewardedVideoAd = [[FBRewardedVideoAd alloc] initWithPlacementID:self.placementId];
//    self.rewardedVideoAd.delegate = self;
//    
//    //NSLog(@"the test device hash is %@",[FBAdSettings testDeviceHash]);
//    [FBAdSettings addTestDevice:[FBAdSettings testDeviceHash]];
//    
//    [self.rewardedVideoAd loadAd];
//}

@end
