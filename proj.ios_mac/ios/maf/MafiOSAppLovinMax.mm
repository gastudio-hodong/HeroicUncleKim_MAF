//
//  MafiOSUnityAds.m
//  NonsenseQuiz
//
//  Created by SanHeo on 2015. 2. 17..
//
//
#import "MafiOSAppLovinMax.h"
#import "MafRewardAdsManager.h"

#import "AppController.h"

@implementation MafiOSAppLovinMax

@synthesize strAdmobRewardId;

+ (MafiOSAppLovinMax*) sharediOSAppLovinMaxPlugin
{
    static MafiOSAppLovinMax* pInstance;
    if(pInstance == NULL)
    {
        pInstance = [[MafiOSAppLovinMax alloc] init];
    }
    return pInstance;
}

#pragma mark-
- (void) initAppLovinMax:(NSString*)adUnitId{
    self.strAdmobRewardId = adUnitId;
    
    [ALSdk shared].mediationProvider = @"max";
    [[ALSdk shared] initializeSdkWithCompletionHandler:^(ALSdkConfiguration *configuration) {
        // AppLovin SDK is initialized, start loading ads
        self.rewardedAd = [MARewardedAd sharedWithAdUnitIdentifier: self.strAdmobRewardId];
        self.rewardedAd.delegate = self;
        
        // Load the first ad
        [self loadAppLovinMax];
    }];
}

#pragma mark-
- (void) loadAppLovinMax
{
    [self.rewardedAd loadAd];
}

- (void) showAppLovinMax
{
    _bAdsWatchFinish = false;
    
    if ( [self.rewardedAd isReady] )
    {
        [self.rewardedAd showAd];
    }
    else
    {
        // 로드된 광고가 없을때
        MafRewardAdsManager::getInstance()->CBReceiveRewardAds(ADS_RESULT::LOAD);
    }
}

#pragma mark-
/**
 * This method is called when a new ad has been loaded.
 */
- (void)didLoadAd:(MAAd *)ad
{
    
}

/**
 * This method is called when an ad could not be retrieved.
 *
 * Common error codes:
 * 204 - no ad is available
 * 5xx - internal server error
 * negative number - internal errors
 *
 * @param adUnitIdentifier  Ad unit identifier for which the ad was requested.
 * @param errorCode         An error code representing the failure reason. Common error codes are defined in `MAErrorCode.h`.
 */
- (void)didFailToLoadAdForAdUnitIdentifier:(NSString *)adUnitIdentifier withErrorCode:(NSInteger)errorCode
{
    // Rewarded ad failed to load. We recommend re-trying in 3 seconds.
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 3 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
        [self loadAppLovinMax];
    });
}

/**
 * This method is invoked when an ad is displayed.
 *
 * This method is invoked on the main UI thread.
 */
- (void)didDisplayAd:(MAAd *)ad
{
    
}

/**
 * This method is invoked when an ad is hidden.
 *
 * This method is invoked on the main UI thread.
 */
- (void)didHideAd:(MAAd *)ad
{
    // Load the first ad
    [self loadAppLovinMax];
    
    if(_bAdsWatchFinish)
    {
        _bAdsWatchFinish = false;
        MafRewardAdsManager::getInstance()->CBReceiveRewardAds(ADS_RESULT::SUCCESS);
    }
    else
        MafRewardAdsManager::getInstance()->CBReceiveRewardAds(ADS_RESULT::BACKGROUND_SKIP);
    
    //
    [[UIApplication sharedApplication] setIdleTimerDisabled:YES];
}

/**
 * This method is invoked when the ad is clicked.
 *
 * This method is invoked on the main UI thread.
 */
- (void)didClickAd:(MAAd *)ad
{
    
}

/**
 * This method is invoked when the ad failed to displayed.
 *
 * This method is invoked on the main UI thread.
 *
 * @param ad        Ad that was just failed to display.
 * @param errorCode Error that indicates display failure. Common error codes are defined in `MAErrorCode.h`.
 */
- (void)didFailToDisplayAd:(MAAd *)ad withErrorCode:(NSInteger)errorCode
{
    // Rewarded ad failed to display. We recommend loading the next ad
    [self loadAppLovinMax];
}

#pragma mark-
/**
 * This method will be invoked when rewarded video has started.
 */
- (void)didStartRewardedVideoForAd:(MAAd *)ad
{
    
}

/**
 * This method will be invoked when rewarded video has completed.
 */
- (void)didCompleteRewardedVideoForAd:(MAAd *)ad
{
    
}

/**
 * This method will be invoked when a user should be granted a reward.
 *
 * @param ad     Ad for which reward ad was rewarded for.
 * @param reward The reward to be granted to the user.
 */
- (void)didRewardUserForAd:(MAAd *)ad withReward:(MAReward *)reward
{
    _bAdsWatchFinish = true;
}


@end
