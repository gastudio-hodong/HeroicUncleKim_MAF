//
//  MafiOSAnalytics.m
//  Unity-iPhone
//
//  Created by SanHeo on 2014. 5. 19..
//
//

#import "MafiOSAppsFlyer.h"
//#import "GAI.h"
//#import "GAIDictionaryBuilder.h"



@implementation MafiOSAppsFlyer

+ (MafiOSAppsFlyer*) sharediOSAppsFlyerPlugin
{
    static MafiOSAppsFlyer* pInstance;
    if(pInstance == NULL){
        pInstance = [[MafiOSAppsFlyer alloc] init];
    }
    return pInstance;
}

- (void) Initialize:(NSString*)strDevKey AppId:(NSString*)strAppId{
    
    [AppsFlyerLib shared].appsFlyerDevKey = strDevKey;
    [AppsFlyerLib shared].appleAppID = strAppId;
//    NSLog(@"getAppsFlyerUID : %@",[[AppsFlyerTracker sharedTracker] getAppsFlyerUID]);
//    [AppsFlyerTracker sharedTracker].customerUserID = [[AppsFlyerTracker sharedTracker] getAppsFlyerUID];
//    [AppsFlyerTracker sharedTracker].customerUserID = [self GetIdentifierForAdvertising];
}


- (void) TrackAppLaunch{
    [[AppsFlyerLib shared] start];
    //trackAppLaunch
//    [AppsFlyerTracker sharedTracker].delegate = self; //Delegate methods below
}


- (void) TrackEventPurchase:(NSString*)productId price:(NSString*)price currency:(NSString*)nation{

    [AppsFlyerLib shared].currencyCode = nation;
    [[AppsFlyerLib shared] logEvent:AFEventPurchase withValues: @{AFEventParamContentId:productId,
                                                                         AFEventParamContentType:@"InApp",
                                                                               AFEventParamCurrency:nation,
                                                                               AFEventParamRevenue: price}];
    
}

- (void) TrackEvent:(NSString*)strEventName{
    [[AppsFlyerLib shared] logEvent:strEventName withValues:@{}];
}




//#pragma AppsFlyerTrackerDelegate methods
//- (void) onConversionDataReceived:(NSDictionary*) installData{
//    id status = [installData objectForKey:@"af_status"];
//    if([status isEqualToString:@"Non-organic"]) {
//        id sourceID = [installData objectForKey:@"media_source"];
//        id campaign = [installData objectForKey:@"campaign"];
//        NSLog(@"This is a none organic install.");
//        NSLog(@"Media source: %@",sourceID);
//        NSLog(@"Campaign: %@",campaign);
//    } else if([status isEqualToString:@"Organic"]) {
//        NSLog(@"This is an organic install.");
//    }
//}
//
//- (void) onConversionDataRequestFailure:(NSError *)error{
//    NSLog(@"Failed to get data from AppsFlyer's server: %@",[error localizedDescription]);
//}


@end
