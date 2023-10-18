//
//  MafiOSGoogleRewardAds.h
//  MoneyHero
//
//  Created by SanHeo on 2016. 5. 9..
//
//

#import <AppLovinSDK/AppLovinSDK.h>

@interface MafiOSAppLovinMax : NSObject<MARewardedAdDelegate>
{
    NSString *strAdmobRewardId;
    bool _bAdsWatchFinish;
}

@property(retain, nonatomic) NSString *strAdmobRewardId;
@property(nonatomic, strong) MARewardedAd *rewardedAd;

+ (MafiOSAppLovinMax*) sharediOSAppLovinMaxPlugin;

- (void) initAppLovinMax:(NSString*)adUnitId;
- (void) loadAppLovinMax;
- (void) showAppLovinMax;

@end
