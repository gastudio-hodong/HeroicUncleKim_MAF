//
//  MafiOSRemoteConfig.h
//  Unity-iPhone
//
//  Created by SanHeo on 2014. 5. 19..
//
//
#import "Firebase.h"

@interface MafiOSRemoteConfig : NSObject

@property(nonatomic, strong) FIRRemoteConfig *remoteConfig;

+ (MafiOSRemoteConfig*) sharedInstance;
- (void) InitRemoteConfig;

- (const char*) GetRemoteConfigValueString:(NSString*)key;
- (int) GetRemoteConfigValueInt:(NSString*)key;
- (bool) GetRemoteConfigValueBool:(NSString*)key;

- (bool) IsRemoteConfigValue:(NSString*)key;

@end
