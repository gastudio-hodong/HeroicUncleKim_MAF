//
//  MafiOSAnalytics.m
//  Unity-iPhone
//
//  Created by SanHeo on 2014. 5. 19..
//
//

#import "MafiOSRemoteConfig.h"

@implementation MafiOSRemoteConfig

+ (MafiOSRemoteConfig*) sharedInstance
{
    static MafiOSRemoteConfig* pInstance;
    if(pInstance == NULL){
        pInstance = [[MafiOSRemoteConfig alloc] init];
    }
    return pInstance;
}

- (void) InitRemoteConfig{
    
    // [START get_remote_config_instance]
    self.remoteConfig = [FIRRemoteConfig remoteConfig];
    // [END get_remote_config_instance]
    
    // Create a Remote Config Setting to enable developer mode, which you can use to increase
    // the number of fetches available per hour during development. See Best Practices in the
    // README for more information.
    // [START enable_dev_mode]
    FIRRemoteConfigSettings *remoteConfigSettings = [[FIRRemoteConfigSettings alloc] init];
    remoteConfigSettings.minimumFetchInterval = 10;
    self.remoteConfig.configSettings = remoteConfigSettings;
    // [END enable_dev_mode]
    
    [self.remoteConfig fetchWithExpirationDuration:3600 completionHandler:^(FIRRemoteConfigFetchStatus status, NSError *error) {
        if (status == FIRRemoteConfigFetchStatusSuccess) {
            NSLog(@"Config fetched!");
            [self.remoteConfig activateWithCompletion:^(BOOL changed, NSError * _Nullable error) {
                auto allKeys = [self.remoteConfig allKeysFromSource:FIRRemoteConfigSourceRemote];
                for (id key in allKeys){
                    NSLog(@"value : %@",self.remoteConfig[key].stringValue);
                }
            }];
        } else {
            NSLog(@"Config not fetched");
            NSLog(@"Error %@", error.localizedDescription);
        }
    }];
}

//    auto allKeys = [self.remoteConfig allKeysFromSource:FIRRemoteConfigSourceRemote];
//    for (id key in allKeys){
//        NSLog(@"%@",self.remoteConfig[key].stringValue);
//        [[NSUserDefaults standardUserDefaults] setObject:self.remoteConfig[key].stringValue forKey:key];
//    }


- (const char*) GetRemoteConfigValueString:(NSString*)key{
//    NSString *value = [[NSUserDefaults standardUserDefaults] objectForKey:key];
//    return [value UTF8String];
    return [self.remoteConfig[key].stringValue UTF8String];
}

- (int) GetRemoteConfigValueInt:(NSString*)key{
//    id value = [[NSUserDefaults standardUserDefaults] objectForKey:key];
//    return [value intValue];
    return [self.remoteConfig[key].numberValue intValue];
}
- (bool) GetRemoteConfigValueBool:(NSString*)key{
//    id value = [[NSUserDefaults standardUserDefaults] objectForKey:key];
//    return [value boolValue];
    return self.remoteConfig[key].boolValue;
}

- (bool) IsRemoteConfigValue:(NSString*)key{
//    if([[NSUserDefaults standardUserDefaults] objectForKey:key] == nil){
    if(self.remoteConfig[key].dataValue.length ==0){
        return false;
    }
    else{
        return true;
    }

}

@end
