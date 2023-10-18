//
//  MafiOSGoogleRewardAds.h
//  MoneyHero
//
//  Created by SanHeo on 2016. 5. 9..
//
//

#import <GoogleMobileAds/GoogleMobileAds.h>

@interface MafiOSGoogleRewardAds : NSObject<GADFullScreenContentDelegate, GADBannerViewDelegate>
{
    NSString *strIdRewardedAd;
    NSString *strIdRewardedInterstitialAd;
    NSString *strIdInterstitial;
    NSString *strIdBanner;

    bool _bRewardedAdLoad;
    bool _bRewardedInterstitialAdLoad;
    bool _bInterstitialLoad;
    
    bool _bRewardFinish;
}

@property (retain, nonatomic) NSString *strIdRewardedAd;
@property (retain, nonatomic) NSString *strIdRewardedInterstitialAd;
@property (retain, nonatomic) NSString *strIdInterstitial;
@property (retain, nonatomic) NSString *strIdBanner;

@property(nonatomic, strong) GADRewardedAd *rewardedAd;
@property(nonatomic, strong) GADRewardedInterstitialAd *rewardedInterstitialAd;
@property(nonatomic, strong) GADInterstitialAd *interstitial;
@property(nonatomic, strong) GADBannerView *bannerView;

+ (MafiOSGoogleRewardAds*) sharediOSGoogleRewardAdsPlugin;

- (void) initAds;

- (void) initRewardedAd:(NSString*)strId;
- (void) loadRewardedAd;
- (void) showRewardedAd;

- (void) initRewardedInterstitialAd:(NSString*)strId;
- (void) loadRewardedInterstitialAd;
- (void) showRewardedInterstitialAd;

- (void) initInterstitial:(NSString*)strId;
- (void) loadInterstitial;
- (void) showInterstitial;

- (void) initBannerView:(NSString*)strId;
- (void) loadBannerView;
- (void) showBannerView;
- (void) hideBannerView;
 

@end
