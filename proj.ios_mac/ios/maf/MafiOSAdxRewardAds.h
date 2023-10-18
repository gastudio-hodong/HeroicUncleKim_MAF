//
//  MafiOSAdxRewardAds.h
//  MoneyHero
//
//  Created by SanHeo on 2016. 5. 9..
//
//

#import <ADXLibrary/ADXRewardedAd.h>
#import <ADXLibrary/ADXInterstitialAd.h>
#import <ADXLibrary/ADXAdView.h>


@interface MafiOSAdxRewardAds : NSObject<ADXRewardedAdDelegate, ADXInterstitialAdDelegate, ADXAdViewDelegate>
{
    NSString *strIdRewardedAd;
    NSString *strIdInterstitial;
    NSString *strIdBanner;
    NSString *strIdTopBanner;

    bool _bRewardedAdLoad;
    bool _bInterstitialLoad;
    bool _bRewardFinish;
}

@property (retain, nonatomic) NSString *strIdRewardedAd;
@property (retain, nonatomic) NSString *strIdInterstitial;
@property (retain, nonatomic) NSString *strIdBanner;
@property (retain, nonatomic) NSString *strIdTopBanner;

@property(nonatomic, strong) ADXRewardedAd *rewardedAd;
@property(nonatomic, strong) ADXInterstitialAd *interstitialAd;
@property(nonatomic, strong) ADXAdView *adView;
@property(nonatomic, strong) ADXAdView *adTopView;


+ (MafiOSAdxRewardAds*) sharediOSAdxRewardAdsPlugin;

- (void) initAds;

- (void) initRewardedAd:(NSString*)strId;
- (void) loadRewardedAd;
- (void) showRewardedAd;
- (bool) isRewardedAd;

- (void) initInterstitial:(NSString*)strId;
- (void) loadInterstitial;
- (void) showInterstitial;

- (void) initBannerView:(NSString*)strId;
- (void) showBannerView;
- (void) hideBannerView;

- (void) initTopBannerView:(NSString*)strId;
- (void) showTopBannerView;
- (void) hideTopBannerView;

@end
