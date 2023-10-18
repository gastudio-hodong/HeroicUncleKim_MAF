//
//  MafiOSAnalytics.h
//  Unity-iPhone
//
//  Created by SanHeo on 2014. 5. 19..
//
//

//#import <FBAudienceNetwork/FBAudienceNetwork.h>

@interface MafiOSFacebook : NSObject

//@property(nonatomic, strong) FBRewardedVideoAd *rewardedVideoAd;

+ (MafiOSFacebook*) sharediOSFacebookPlugin;

- (void) InitFacebook;

- (void) FacebookLog:(NSString*)strEvent param:(NSString*)strParam val_int:(int)value;
- (void) FacebookLog:(NSString*)strEvent param:(NSString*)strParam val_str:(NSString*)value;

//- (void) showFacebookAds;

//- (void) loadFacebookAds;


@end
