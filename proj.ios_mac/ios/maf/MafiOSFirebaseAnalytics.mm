//
//  MafiOSFirebaseAnalytics.m
//  FantasyClicker
//
//  Created by SanHeo on 2016. 9. 6..
//
//
#import "MafiOSFirebaseAnalytics.h"

#import "Firebase.h"


@implementation MafiOSFirebaseAnalytics

+ (MafiOSFirebaseAnalytics*) sharediOSFirebaseAnalyticsPlugin
{
    static MafiOSFirebaseAnalytics* pInstance;
    if(pInstance == NULL){
        pInstance = [[MafiOSFirebaseAnalytics alloc] init];
    }
    return pInstance;
}

- (void) InitFirebaseAnalytics{
    
    if ( [[UIDevice currentDevice].systemVersion floatValue] >= 7.0)
    {
//        [FIRApp configure];
    }
    
    
}

- (void) LogEvent:(NSString*)strEvent param:(NSString*)strParam value_int:(int)val{
    
    if ( strParam.length == 0 ) {
        [FIRAnalytics logEventWithName:strEvent parameters: nil];
    } else {
        [FIRAnalytics logEventWithName:strEvent
                            parameters:@{
                                         strParam: [NSNumber numberWithInt:val]
                                         }];
    }
}

- (void) LogEvent:(NSString*)strEvent param:(NSString*)strParam value_str:(NSString*)strVal{
    
    if ( strParam.length == 0 ) {
        [FIRAnalytics logEventWithName:strEvent parameters: nil];
    } else {
        [FIRAnalytics logEventWithName:strEvent
                            parameters:@{
                                         strParam: strVal
                                         }];
    }
}


@end
