//
//  MafiOSAnalytics.h
//  Unity-iPhone
//
//  Created by SanHeo on 2014. 5. 19..
//
//


@interface MafiOSFirebaseAnalytics : NSObject


+ (MafiOSFirebaseAnalytics*) sharediOSFirebaseAnalyticsPlugin;

- (void) InitFirebaseAnalytics;

- (void) LogEvent:(NSString*)strEvent param:(NSString*)strParam value_int:(int)val;
- (void) LogEvent:(NSString*)strEvent param:(NSString*)strParam value_str:(NSString*)val;


@end
