//
//  MafiOSUnityAds.m
//  NonsenseQuiz
//
//  Created by SanHeo on 2015. 2. 17..
//
//
#import "MafiOSGoogleRewardAds.h"
#import "MafGoogleRewardAds.h"
#import "AppController.h"

@implementation MafiOSGoogleRewardAds

@synthesize strIdRewardedAd;//, strAppLovinMaxId;
@synthesize strIdRewardedInterstitialAd;
@synthesize strIdInterstitial;
@synthesize strIdBanner;

//@synthesize interstitial;
@synthesize rewardedAd;
@synthesize rewardedInterstitialAd;
@synthesize interstitial;
@synthesize bannerView;

+ (MafiOSGoogleRewardAds*) sharediOSGoogleRewardAdsPlugin
{
    static MafiOSGoogleRewardAds* pInstance;
    if(pInstance == NULL)
    {
        pInstance = [[MafiOSGoogleRewardAds alloc] init];
    }
    return pInstance;
}

- (void) initAds {
    
    _bRewardedAdLoad = false;
    _bRewardedInterstitialAdLoad = false;
    _bInterstitialLoad = false;
    _bRewardFinish = false;
    
    GADMobileAds *ads = [GADMobileAds sharedInstance];
    [ads startWithCompletionHandler:^(GADInitializationStatus *status) {
        
        // Start loading ads here...
        [self loadRewardedAd];
        [self loadRewardedInterstitialAd];
        [self loadInterstitial];
        [self loadBannerView];
    }];
    
}

#pragma mark - rewawrd video
- (void) initRewardedAd:(NSString*)strId {
    
    self.strIdRewardedAd = strId;
}

- (void) loadRewardedAd {
    
    if ( _bRewardedAdLoad ) {
        return;
    }
    
    _bRewardedAdLoad = true;
    rewardedAd = nil;
    
    GADRequest *request = [GADRequest request];
    [GADRewardedAd loadWithAdUnitID:self.strIdRewardedAd request:request completionHandler:^(GADRewardedAd *ad, NSError *error) {
        
        _bRewardedAdLoad = false;
        
        if (error) {
          NSLog(@"Rewarded ad failed to load with error: %@", [error localizedDescription]);
          return;
        }
        self.rewardedAd = ad;
        self.rewardedAd.fullScreenContentDelegate = self;
        NSLog(@"Rewarded ad loaded.");
    }];
}

- (void) showRewardedAd {
    
    _bRewardFinish = false;
    
    if ( self.rewardedAd ) {
        AppController *app = GetAppController();
        
        [self.rewardedAd presentFromRootViewController:(UIViewController*)app.viewController userDidEarnRewardHandler:^{
            GADAdReward *reward = self.rewardedAd.adReward;
            // TODO: Reward the user!
            _bRewardFinish = true;
        }];
    } else {
        NSLog(@"Ad wasn't ready");
        
        //
        MafGoogleRewardAds::CBReceiveAdsReward(ADS_RESULT::LOAD);
        
        //
        [self loadRewardedAd];
    }
}

#pragma mark - reward interstitialAd
- (void) initRewardedInterstitialAd:(NSString*)strId {
    
    self.strIdRewardedInterstitialAd = strId;
}

- (void)  loadRewardedInterstitialAd{
    
    if ( _bRewardedInterstitialAdLoad ) {
        return;
    }
    
    _bRewardedInterstitialAdLoad = true;
    rewardedInterstitialAd = nil;
    
    GADRequest *request = [GADRequest request];
    [GADRewardedInterstitialAd loadWithAdUnitID:self.strIdRewardedInterstitialAd request:request completionHandler:^(GADRewardedInterstitialAd* _Nullable rewardedInterstitialAd, NSError* _Nullable error) {
    
        _bRewardedInterstitialAdLoad = false;
        
        if (error) {
          NSLog(@"Interstitial ad failed to load with error: %@", [error localizedDescription]);
          return;
        }
        self.rewardedInterstitialAd = rewardedInterstitialAd;
        self.rewardedInterstitialAd.fullScreenContentDelegate = self;
        NSLog(@"Interstitial ad loaded.");
    }];
}

- (void) showRewardedInterstitialAd {
    
    _bRewardFinish = false;
    
    if ( self.rewardedInterstitialAd ) {
        AppController *app = GetAppController();
        
        [self.rewardedInterstitialAd presentFromRootViewController:(UIViewController*)app.viewController userDidEarnRewardHandler:^{
            GADAdReward *reward = self.rewardedInterstitialAd.adReward;
            // TODO: Reward the user!
            _bRewardFinish = true;
        }];
    } else {
        NSLog(@"Ad wasn't ready");
        
        //
        MafGoogleRewardAds::CBReceiveAdsReward(ADS_RESULT::LOAD);
        
        //
        [self loadRewardedInterstitialAd];
    }
}

#pragma mark - reward interstitialAd
- (void) initInterstitial:(NSString*)strId {
    
    self.strIdInterstitial = strId;
}

- (void) loadInterstitial {
    
    if ( _bInterstitialLoad ) {
        return;
    }
    
    _bInterstitialLoad = true;
    interstitial = nil;
    
    GADRequest *request = [GADRequest request];
    [GADInterstitialAd loadWithAdUnitID:self.strIdInterstitial request:request completionHandler:^(GADInterstitialAd *ad, NSError *error) {
        
        _bInterstitialLoad = false;
        
        if (error) {
            NSLog(@"Failed to load interstitial ad with error: %@", [error localizedDescription]);
            return;
        }
        self.interstitial = ad;
        self.interstitial.fullScreenContentDelegate = self;
    }];
}

- (void) showInterstitial {
 
    if (self.interstitial) {
        AppController *app = GetAppController();
        
        [self.interstitial presentFromRootViewController:(UIViewController*)app.viewController];
    } else {
        NSLog(@"Ad wasn't ready");
        
        //
        MafGoogleRewardAds::CBReceiveAdsReward(ADS_RESULT::LOAD);
    }
}

#pragma mark - banner
- (void) initBannerView:(NSString*)strId{
    
    self.strIdBanner = strId;
    self.bannerView = [[GADBannerView alloc] initWithAdSize:GADAdSizeBanner];
    
    self.bannerView.adUnitID = self.strIdBanner;
    self.bannerView.delegate = self;
    
    AppController *app = GetAppController();
    self.bannerView.rootViewController = (UIViewController*)app.viewController;
    
    // Step 2 - Determine the view width to use for the ad width.
    CGRect frame = ((UIViewController*)app.viewController).view.frame;
    // Here safe area is taken into account, hence the view frame is used after
    // the view has been laid out.
    if (@available(iOS 11.0, *)) {
      frame = UIEdgeInsetsInsetRect(((UIViewController*)app.viewController).view.frame, ((UIViewController*)app.viewController).view.safeAreaInsets);
    }
    CGFloat viewWidth = frame.size.width;
    
    // Step 3 - Get Adaptive GADAdSize and set the ad view.
    // Here the current interface orientation is used. If the ad is being
    // preloaded for a future orientation change or different orientation, the
    // function for the relevant orientation should be used.
    GADAdSize adSize = GADPortraitAnchoredAdaptiveBannerAdSizeWithWidth(viewWidth);
    self.bannerView.adSize = adSize;
}

- (void) loadBannerView{
    GADRequest *request = [GADRequest request];
    [self.bannerView loadRequest:request];
}

- (void) showBannerView{
    
    if (self.bannerView) {
        
        AppController *app = GetAppController();
        
        self.bannerView.translatesAutoresizingMaskIntoConstraints = NO;
        [((UIViewController*)app.viewController).view addSubview:self.bannerView];
        [((UIViewController*)app.viewController).view addConstraints:@[
            [NSLayoutConstraint constraintWithItem:self.bannerView
                                         attribute:NSLayoutAttributeBottom
                                         relatedBy:NSLayoutRelationEqual
                                            toItem:((UIViewController*)app.viewController).bottomLayoutGuide
                                         attribute:NSLayoutAttributeTop
                                        multiplier:1
                                          constant:0],
            [NSLayoutConstraint constraintWithItem:self.bannerView
                                         attribute:NSLayoutAttributeCenterX
                                         relatedBy:NSLayoutRelationEqual
                                            toItem:((UIViewController*)app.viewController).view
                                         attribute:NSLayoutAttributeCenterX multiplier:1
                                          constant:0]
        ]];
    }
}

- (void) hideBannerView{
     
    if (self.bannerView) {
        [self.bannerView removeFromSuperview];
    }
}


#pragma mark - delegate
/// Tells the delegate that the ad failed to present full screen content.
- (void)ad:(nonnull id<GADFullScreenPresentingAd>)ad didFailToPresentFullScreenContentWithError:(nonnull NSError *)error {
    NSLog(@"Ad did fail to present full screen content.");
    
    if ( [ad isKindOfClass:[GADRewardedAd class]] ) {
        
        MafGoogleRewardAds::CBReceiveAdsReward(ADS_RESULT::LOAD);
        [self loadRewardedAd];
    } else if ( [ad isKindOfClass:[GADRewardedInterstitialAd class]] ) {
        
        MafGoogleRewardAds::CBReceiveAdsReward(ADS_RESULT::LOAD);
        [self loadRewardedInterstitialAd];
    } else if ( [ad isKindOfClass:[GADInterstitialAd class]] ) {
        
        [self loadInterstitial];
    }
}

/// Tells the delegate that the ad presented full screen content.
- (void)adDidPresentFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad {
    NSLog(@"Ad did present full screen content.");
}

/// Tells the delegate that the ad dismissed full screen content.
- (void)adDidDismissFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad {
    NSLog(@"Ad did dismiss full screen content.");
    
    if ( [ad isKindOfClass:[GADRewardedInterstitialAd class]] || [ad isKindOfClass:[GADRewardedAd class]] ) {
        
        if ( _bRewardFinish ) {
            MafGoogleRewardAds::CBReceiveAdsReward(ADS_RESULT::SUCCESS);
        } else {
            MafGoogleRewardAds::CBReceiveAdsReward(ADS_RESULT::BACKGROUND_SKIP);
        }
        
        //
        if ( [ad isKindOfClass:[GADRewardedAd class]] ) {
            [self loadRewardedAd];
        }
        else if ( [ad isKindOfClass:[GADRewardedInterstitialAd class]] ) {
            [self loadRewardedInterstitialAd];
        }
    }
    else if ( [ad isKindOfClass:[GADInterstitialAd class]] ) {
        [self loadInterstitial];
    }
}

#pragma mark - delegate banner
- (void)bannerViewDidReceiveAd:(nonnull GADBannerView *)bannerView {
    NSLog(@"adViewDidReceiveAd");
}

/// Tells the delegate that an ad request failed. The failure is normally due to network
/// connectivity or ad availablility (i.e., no fill).
- (void)bannerView:(nonnull GADBannerView *)bannerView didFailToReceiveAdWithError:(nonnull NSError *)error {
    NSLog(@"adView:didFailToReceiveAdWithError: %@", [error localizedDescription]);
}

/// Tells the delegate that an impression has been recorded for an ad.
- (void)bannerViewDidRecordImpression:(nonnull GADBannerView *)bannerView {
    
}

/// Tells the delegate that a full screen view will be presented in response to the user clicking on
/// an ad. The delegate may want to pause animations and time sensitive interactions.
- (void)bannerViewWillPresentScreen:(nonnull GADBannerView *)bannerView {
    NSLog(@"adViewWillPresentScreen");
}

/// Tells the delegate that the full screen view will be dismissed.
- (void)bannerViewWillDismissScreen:(nonnull GADBannerView *)bannerView {
    NSLog(@"adViewWillDismissScreen");
}

/// Tells the delegate that the full screen view has been dismissed. The delegate should restart
/// anything paused while handling bannerViewWillPresentScreen:.
- (void)bannerViewDidDismissScreen:(nonnull GADBannerView *)bannerView {
    NSLog(@"adViewDidDismissScreen");
}

@end
