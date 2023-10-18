//
//  MafiOSNative.h
//  Unity-iPhone
//
//  Created by SanHeo on 2014. 5. 20..
//
//

@interface MafiOSNative : NSObject <UIAlertViewDelegate>{
    BOOL _bHide;
}

+ (MafiOSNative *) sharediOSNativePlugin;
- (void) SendEmail:(NSString*)strEmail;
- (void) SendEmail:(NSString*)strEmail subject:(NSString*)strSubject body:(NSString*)strBody;

- (int) GetScreenSize:(BOOL)bHeight;
- (void) OpenUrl:(NSString*)strUrl;

- (const char*) GetDeviceIDFA;
- (const char*) GetDeviceModel;
- (const char*) GetDeviceSystemVersion;
- (void) GetAdsID;

- (const char*)GetGameVersion;
- (void) ShowUpdateAlert:(NSString*)strTitle message:(NSString*)strMessage confirm:(NSString*)strConfirm shortcut:(NSString*)strShortcut;

- (bool) isDeviceiPhoneX;
- (bool) isHideStatusBar;
- (void) hideStatusBar:(BOOL)bHide;

- (void) sendLocalNotification:(NSString*)key title:(NSString*)strTitle time:(int)timeTerm;
- (void) deleteLocalNotification:(NSString*)notiKey;

- (void) createDeepLink:(NSString*)paramKey value:(NSString*)paramValue;
- (void) receiveDeepLink:(NSString*)params;

- (void) copyString:(NSString*)text;
- (void) shareString:(NSString*)text;
@end
