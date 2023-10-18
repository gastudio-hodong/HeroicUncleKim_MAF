//
//  MafiOSUnityAds.m
//  NonsenseQuiz
//
//  Created by SanHeo on 2015. 2. 17..
//
//
#import "MafiOSAdxRewardAds.h"
#import "MafGoogleRewardAds.h"
#import "AppController.h"
#import <ADXLibrary/ADXLibrary.h>

@implementation MafiOSAdxRewardAds

@synthesize strIdRewardedAd;

//@synthesize interstitial;
@synthesize rewardedAd;

+ (MafiOSAdxRewardAds*) sharediOSAdxRewardAdsPlugin
{
    static MafiOSAdxRewardAds* pInstance;
    if(pInstance == NULL)
    {
        pInstance = [[MafiOSAdxRewardAds alloc] init];
    }
    return pInstance;
}

- (void) initAds {
    
    _bRewardedAdLoad = false;
    _bInterstitialLoad = false;
    _bRewardFinish = false;
    
    //
    ADXConfiguration *configuration = [[ADXConfiguration alloc] initWithAppId:@"62a809016da6b10001000201" gdprType:ADXGdprTypePopupLocation];
    [[ADXSdk sharedInstance] initializeWithConfiguration:configuration completionHandler:^(BOOL result, ADXConsentState consentState) {
        NSLog(@"ADX Sdk Initialize");
        
        MafGoogleRewardAds::CBReceiveAdsInit();
    }];
}

#pragma mark - rewawrd video
- (void) initRewardedAd:(NSString*)strId {
    
    self.strIdRewardedAd = strId;
    self.rewardedAd = [[ADXRewardedAd alloc] initWithAdUnitId:self.strIdRewardedAd];
    self.rewardedAd.delegate = self;
    [self loadRewardedAd];
}

- (void) loadRewardedAd {
    
    if ( _bRewardedAdLoad ) {
        return;
    }
    
    _bRewardedAdLoad = true;
    
    [self.rewardedAd loadAd];
}

- (void) showRewardedAd {
    
    _bRewardFinish = false;
    
    //
    if (self.rewardedAd.isLoaded) {
        AppController *app = GetAppController();
        
        [self.rewardedAd showAdFromRootViewController:(UIViewController*)app.viewController];
    } else {
        //
        MafGoogleRewardAds::CBReceiveAdsReward(ADS_RESULT::LOAD);
        
        //
        [self loadRewardedAd];
    }
}

- (bool) isRewardedAd {
    return [self.rewardedAd isLoaded];
}

#pragma mark - interstitialAd
- (void) initInterstitial:(NSString*)strId {
    
    self.strIdInterstitial = strId;
    self.interstitialAd = [[ADXInterstitialAd alloc] initWithAdUnitId:self.strIdInterstitial];
    self.interstitialAd.delegate = self;
    [self loadInterstitial];
}

- (void) loadInterstitial {
    
    if ( _bInterstitialLoad ) {
        return;
    }

    _bInterstitialLoad = true;

    [self.interstitialAd loadAd];
}

- (void) showInterstitial {
 
    if (self.interstitialAd.isLoaded) {
        AppController *app = GetAppController();

        [self.interstitialAd showAdFromRootViewController:(UIViewController*)app.viewController];
    } else {
        MafGoogleRewardAds::CBReceiveAdsReward(ADS_RESULT::LOAD);

        //
        [self loadInterstitial];
    }
}

#pragma mark - banner
- (void) initBannerView:(NSString*)strId{
    
    self.strIdBanner = strId;

    AppController *app = GetAppController();
    self.adView = [[ADXAdView alloc] initWithAdUnitId:self.strIdBanner adSize:ADXAdSizeBanner rootViewController:(UIViewController*)app.viewController];
    self.adView.delegate = self;
    self.adView.frame = CGRectMake(
                                   (((UIViewController*)app.viewController).view.bounds.size.width - ADXAdSizeBanner.width) / 2,
                                   ((UIViewController*)app.viewController).view.bounds.size.height - ADXAdSizeBanner.height,
                                   ADXAdSizeBanner.width,
                                   ADXAdSizeBanner.height);
}

- (void) showBannerView{
    
    if (self.adView) {

        AppController *app = GetAppController();

        [((UIViewController*)app.viewController).view addSubview:self.adView];
        [((UIViewController*)app.viewController).view addConstraints:@[
            [NSLayoutConstraint constraintWithItem:self.adView
                                         attribute:NSLayoutAttributeBottom
                                         relatedBy:NSLayoutRelationEqual
                                            toItem:((UIViewController*)app.viewController).view.safeAreaLayoutGuide
                                         attribute:NSLayoutAttributeTop
                                        multiplier:1
                                          constant:0],
            
            [NSLayoutConstraint constraintWithItem:self.adView
                                         attribute:NSLayoutAttributeCenterX
                                         relatedBy:NSLayoutRelationEqual
                                            toItem:((UIViewController*)app.viewController).view
                                         attribute:NSLayoutAttributeCenterX
                                        multiplier:1
                                          constant:0]
        ]];
        [self.adView loadAd];
        [self.adView startAutomaticallyRefreshingContents];
    }
}

- (void) hideBannerView{
     
    if (self.adView) {
        [self.adView removeFromSuperview];
        [self.adView stopAutomaticallyRefreshingContents];
    }
}

- (void) initTopBannerView:(NSString*)strId{
    
    self.strIdTopBanner = strId;

    AppController *app = GetAppController();
    self.adTopView = [[ADXAdView alloc] initWithAdUnitId:self.strIdTopBanner adSize:ADXAdSizeBanner rootViewController:(UIViewController*)app.viewController];
    self.adTopView.delegate = self;
    self.adTopView.frame = CGRectMake(
                                   (((UIViewController*)app.viewController).view.bounds.size.width - ADXAdSizeBanner.width) / 2,
                                   //((UIViewController*)app.viewController).view.bounds.size.height - ADXAdSizeBanner.height,
                                      0,
                                   ADXAdSizeBanner.width,
                                   ADXAdSizeBanner.height);
}

- (void) showTopBannerView{
    
    if (self.adTopView) {

        AppController *app = GetAppController();

        [((UIViewController*)app.viewController).view addSubview:self.adTopView];
        [((UIViewController*)app.viewController).view addConstraints:@[
            [NSLayoutConstraint constraintWithItem:self.adTopView
                                         attribute:NSLayoutAttributeBottom
                                         relatedBy:NSLayoutRelationEqual
                                            toItem:((UIViewController*)app.viewController).view.safeAreaLayoutGuide
                                         attribute:NSLayoutAttributeTop
                                        multiplier:1
                                          constant:0],
            
            [NSLayoutConstraint constraintWithItem:self.adTopView
                                         attribute:NSLayoutAttributeCenterX
                                         relatedBy:NSLayoutRelationEqual
                                            toItem:((UIViewController*)app.viewController).view
                                         attribute:NSLayoutAttributeCenterX
                                        multiplier:1
                                          constant:0]
        ]];
        [self.adTopView loadAd];
        [self.adTopView startAutomaticallyRefreshingContents];
    }
}

- (void) hideTopBannerView{
     
    if (self.adTopView) {
        [self.adTopView removeFromSuperview];
        [self.adTopView stopAutomaticallyRefreshingContents];
    }
}

#pragma mark - delegate ADXRewardedAdDelegate
- (void)rewardedAdDidLoad:(ADXRewardedAd *)rewardedAd {
    NSLog(@"rewardedAdDidLoad");
    _bRewardedAdLoad = false;
}
- (void)rewardedAd:(ADXRewardedAd *)rewardedAd didFailToLoadWithError:(NSError *)error {
    NSLog(@"rewardedAd:didFailToLoadWithError: %@", error);
    _bRewardedAdLoad = false;
}
- (void)rewardedAd:(ADXRewardedAd *)rewardedAd didFailToShowWithError:(NSError *)error {
    NSLog(@"rewardedAd:didFailToShowWithError: %@", error);
}
- (void)rewardedAdWillPresentScreen:(ADXRewardedAd *)rewardedAd {
    NSLog(@"rewardedAdWillPresentScreen");
}
- (void)rewardedAdWillDismissScreen:(ADXRewardedAd *)rewardedAd {
    NSLog(@"rewardedAdWillDismissScreen");
}
- (void)rewardedAdDidDismissScreen:(ADXRewardedAd *)rewardedAd {
    NSLog(@"rewardedAdDidDismissScreen");
    
    if ( _bRewardFinish == true ) {
        MafGoogleRewardAds::CBReceiveAdsReward(ADS_RESULT::SUCCESS);
    } else {
        MafGoogleRewardAds::CBReceiveAdsReward(ADS_RESULT::BACKGROUND_SKIP);
    }
    
    [self loadRewardedAd];
}
- (void)rewardedAdDidRewardUser:(ADXRewardedAd *)rewardedAd withReward:(ADXReward *)reward {
    NSLog(@"rewardedAdDidRewardUser");
    _bRewardFinish = true;
}
- (void)rewardedAdDidClick:(ADXRewardedAd *)rewardedAd {
    NSLog(@"rewardedAdDidClick");
}

#pragma mark - delegate ADXInterstitialAdDelegate>
- (void)interstitialAdDidLoad:(ADXInterstitialAd *)interstitialAd {
    NSLog(@"interstitialAdDidLoad");
    _bInterstitialLoad = false;
}
- (void)interstitialAd:(ADXInterstitialAd *)interstitialAd didFailToLoadWithError:(NSError *)error {
    NSLog(@"interstitialAd:didFailToLoadWithError: %@", error);
    _bInterstitialLoad = false;
}
- (void)interstitialAd:(ADXInterstitialAd *)interstitialAd didFailToShowWithError:(NSError *)error {
    NSLog(@"interstitialAd:didFailToShowWithError: %@", error);
}
- (void)interstitialAdWillPresentScreen:(ADXInterstitialAd *)interstitialAd {
    NSLog(@"interstitialAdWillPresentScreen");
}
- (void)interstitialAdWillDismissScreen:(ADXInterstitialAd *)interstitialAd {
    NSLog(@"interstitialAdWillDismissScreen");
}
- (void)interstitialAdDidDismissScreen:(ADXInterstitialAd *)interstitialAd {
    NSLog(@"interstitialAdDidDismissScreen");
}
- (void)interstitialAdDidClick:(ADXInterstitialAd *)interstitialAd {
    NSLog(@"interstitialAdDidClick");
}

#pragma mark - delegate ADXAdViewDelegate>
- (void)adViewDidLoad:(ADXAdView *)adView {
    NSLog(@"adViewDidLoad");
}
- (void)adView:(ADXAdView *)adView didFailToLoadWithError:(NSError *)error {
    NSLog(@"adView:didFailToLoadWithError: %@", error);
}

- (void)adViewDidClick:(ADXAdView *)adView {
    NSLog(@"adViewDidClick");
}

@end
