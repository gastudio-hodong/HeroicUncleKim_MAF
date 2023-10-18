//
//  MafiOSAnalytics.h
//  Unity-iPhone
//
//  Created by SanHeo on 2014. 5. 19..
//
//

#import <AppsFlyerLib/AppsFlyerLib.h>

@interface MafiOSAppsFlyer : NSObject//<AppsFlyerTrackerDelegate>
{

}



+ (MafiOSAppsFlyer*) sharediOSAppsFlyerPlugin;
- (void) Initialize:(NSString*)strDevKey AppId:(NSString*)strAppId;
- (void) TrackAppLaunch;

- (void) TrackEventPurchase:(NSString*)productId price:(NSString*)price currency:(NSString*)nation;
- (void) TrackEvent:(NSString*)strEventName;
/*
 [[AppsFlyerTracker sharedTracker] trackEvent:AFEventPurchase withValues: @{AFEventParamContentId:@"11111",
 AFEventParamCurrency:@"JPY",
 AFEventParamRevenue: @200.00}];
 */
//- (void) SendTracking:(NSString*)strCate action:(NSString*)strAction label:(NSString*)strLabel val:(int)val;

@end
