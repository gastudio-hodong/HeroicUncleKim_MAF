//
//  MafiOSNative.m
//  Unity-iPhone
//
//  Created by SanHeo on 2014. 5. 20..
//
//

#import "MafiOSNative.h"
#import "MafNative.h"

//
#import "../AppController.h"
#import "platform/ios/CCEAGLView-ios.h"

//
#import <FirebaseDynamicLinks/FirebaseDynamicLinks.h>
#import <AdSupport/ASIdentifierManager.h>
#import <AppTrackingTransparency/AppTrackingTransparency.h>
#import <AppTrackingTransparency/ATTrackingManager.h>
#import <sys/utsname.h>

@implementation MafiOSNative

+ (MafiOSNative *) sharediOSNativePlugin
{
    static MafiOSNative* pInstance;
    if(pInstance == NULL){
        pInstance = [[MafiOSNative alloc] init];
    }
    return pInstance;
}

- (void) SendEmail:(NSString*)strEmail{
    
    NSString *bundleIdentifier = [[NSBundle mainBundle] bundleIdentifier];
    NSString *version = [[NSBundle mainBundle] objectForInfoDictionaryKey: @"CFBundleShortVersionString"];
    
    NSString *subject = [NSString stringWithFormat:@"%@, ios_ver: %@",bundleIdentifier,version];
    NSString *body = @"";
    NSString *address = strEmail;
    NSString *path = [NSString stringWithFormat:@"mailto:%@?subject=%@&body=%@", address, subject, body];
    NSURL *url = [NSURL URLWithString:[path stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
    [[UIApplication sharedApplication] openURL:url];
    
}

- (void) SendEmail:(NSString*)strEmail subject:(NSString*)strSubject body:(NSString*)strBody{
    
    NSString *subject = strSubject;
    NSString *body = strBody;
    NSString *address = strEmail;
    NSString *path = [NSString stringWithFormat:@"mailto:%@?subject=%@&body=%@", address, subject, body];
    NSURL *url = [NSURL URLWithString:[path stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
    [[UIApplication sharedApplication] openURL:url];
    
}

- (int) GetScreenSize:(BOOL)bHeight{
    
    if(bHeight)
        return [UIScreen mainScreen].bounds.size.height;
    else
        return [UIScreen mainScreen].bounds.size.width;
}

- (void) OpenUrl:(NSString*)strUrl{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:strUrl]];
}

- (const char*)GetGameVersion{
    NSDictionary *info = [[NSBundle mainBundle] infoDictionary];
    NSString *version  = [info objectForKey:@"CFBundleShortVersionString"];
    return [version UTF8String];
}

- (void) ShowUpdateAlert:(NSString*)strTitle message:(NSString*)strMessage confirm:(NSString*)strConfirm shortcut:(NSString*)strShortcut {

//    UIAlertView *alert = [[UIAlertView alloc]initWithTitle:@"업데이트 가능" message:@"최신버전이 존재합니다."
//                                                  delegate:self cancelButtonTitle:@"확인" otherButtonTitles:@"바로받기", nil];
    UIAlertView *alert = [[UIAlertView alloc]initWithTitle:strTitle message:strMessage
                                                  delegate:self cancelButtonTitle:strConfirm otherButtonTitles:nil, nil];
        [alert show];
        [alert release];
}

- (void) GetAdsID{
    if (@available(iOS 14.5, *))
    {
        [ATTrackingManager requestTrackingAuthorizationWithCompletionHandler:^(ATTrackingManagerAuthorizationStatus status) {
            switch (status)
            {
                //인증 상태 확인되지 않음
                case ATTrackingManagerAuthorizationStatus::ATTrackingManagerAuthorizationStatusNotDetermined:
                //권한 부여 상태 제한
                case ATTrackingManagerAuthorizationStatus::ATTrackingManagerAuthorizationStatusRestricted:
                //권한 거부
                case ATTrackingManagerAuthorizationStatus::ATTrackingManagerAuthorizationStatusDenied:
                //권한 승인
                case ATTrackingManagerAuthorizationStatus::ATTrackingManagerAuthorizationStatusAuthorized:
                {
                    NSString *idfaString;
                    idfaString = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
                    MafNative::CBReceiveAdsId([idfaString UTF8String]);
                }
                    break;
            }
        }];

    }
    else
    {
        if([[ASIdentifierManager sharedManager] isAdvertisingTrackingEnabled]) {
            NSString *idfaString = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
            MafNative::CBReceiveAdsId([idfaString UTF8String]);
        }
    }
}

- (const char*) GetDeviceIDFA{
    
    if (@available(iOS 14.5, *))
    {
        [ATTrackingManager requestTrackingAuthorizationWithCompletionHandler:^(ATTrackingManagerAuthorizationStatus status) {
            switch (status)
            {
                //인증 상태 확인되지 않음
                case ATTrackingManagerAuthorizationStatus::ATTrackingManagerAuthorizationStatusNotDetermined:
                //권한 부여 상태 제한
                case ATTrackingManagerAuthorizationStatus::ATTrackingManagerAuthorizationStatusRestricted:
                //권한 거부
                case ATTrackingManagerAuthorizationStatus::ATTrackingManagerAuthorizationStatusDenied:
                //권한 승인
                case ATTrackingManagerAuthorizationStatus::ATTrackingManagerAuthorizationStatusAuthorized:
                {
                    NSString *idfaString;
                    idfaString = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
                    MafNative::CBReceiveAdsId([idfaString UTF8String]);
                }
                    break;
            }
        }];

    }
    else
    {
        if([[ASIdentifierManager sharedManager] isAdvertisingTrackingEnabled]) {
            NSString *idfaString = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
            return [idfaString UTF8String];
        }
    }
    return "";
}

- (const char*) GetDeviceModel{
    struct utsname systemInfo;
    uname(&systemInfo);

    NSString *modelString = [NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding];
    return [modelString UTF8String];
}

- (const char*) GetDeviceSystemVersion{
    
    return [[[UIDevice currentDevice] systemVersion] cStringUsingEncoding:NSUTF8StringEncoding];
}

- (bool) isDeviceiPhoneX{

    bool bResult = false;
    
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 110000
    float version = [[UIDevice currentDevice].systemVersion floatValue];
    if (version >= 11.0f)
    {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpartial-availability"
        auto glview = cocos2d::Director::getInstance()->getOpenGLView();

        if ( glview )
        {
            CCEAGLView *eaglview = (__bridge CCEAGLView *)glview->getEAGLView();
            if (eaglview)
            {
                UIEdgeInsets safeAreaInsets = eaglview.safeAreaInsets;
                if ( safeAreaInsets.top != 0 )
                {
                    bResult = true;
                }
            }
        }
#pragma clang diagnostic pop

    }
#endif
    
    return bResult;
}

- (BOOL) isHideStatusBar{
    return _bHide;
}

- (void) hideStatusBar:(BOOL)bHide{
    _bHide = bHide;
    [[UIApplication sharedApplication].keyWindow.rootViewController setNeedsStatusBarAppearanceUpdate];
}

//- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex{
//    //cancel 0 , other 1,2,3,4
//}
- (void) sendLocalNotification:(NSString*)key title:(NSString*)strTitle time:(int)timeTerm{
    UILocalNotification *noti = [[UILocalNotification alloc] init];
    noti.fireDate = [NSDate dateWithTimeIntervalSinceNow:timeTerm];
    noti.timeZone = [NSTimeZone defaultTimeZone];
    noti.alertBody = strTitle;
    noti.applicationIconBadgeNumber = 1;
    noti.soundName = UILocalNotificationDefaultSoundName;
    
    NSData *notiData = [NSKeyedArchiver archivedDataWithRootObject:noti];
    [[NSUserDefaults standardUserDefaults] setObject:notiData forKey:key];
    
    [[UIApplication sharedApplication] scheduleLocalNotification:noti];
}

- (void) deleteLocalNotification:(NSString*)notiKey {
    NSData *dataNoti = [[NSUserDefaults standardUserDefaults] objectForKey:notiKey];
    UILocalNotification *noti = [NSKeyedUnarchiver unarchiveObjectWithData:dataNoti];
    
    [[UIApplication sharedApplication] cancelLocalNotification:noti];
    [UIApplication sharedApplication].applicationIconBadgeNumber = 0;
}

 // Called when we cancel a view (eg. the user clicks the Home button). This is not called when the user clicks the cancel button.
 // If not defined in the delegate, we simulate a click in the cancel button
//- (void)alertViewCancel:(UIAlertView *)alertView{}
 
//- (void)willPresentAlertView:(UIAlertView *)alertView{}  // before animation and showing view
//- (void)didPresentAlertView:(UIAlertView *)alertView{}  // after animation
 
//- (void)alertView:(UIAlertView *)alertView willDismissWithButtonIndex:(NSInteger)buttonIndex{} // before animation and hiding view
//- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex{}  // after animation
 
 // Called after edits in any of the default fields added by the style
//- (BOOL)alertViewShouldEnableFirstOtherButton:(UIAlertView *)alertView{}


- (void) createDeepLink:(NSString*)paramKey value:(NSString*)paramValue{
    NSString *fullLink = @"https://apps.apple.com/us/app/mr-kim-a-half-baked-hero/id987072755?l=ko&ls=1&os=ios&%@=%@";
    
    fullLink = [NSString stringWithFormat:fullLink, paramKey, paramValue];
    NSURL *link = [[NSURL alloc] initWithString:fullLink];
    NSString *dynamicLinksDomainURIPrefix = @"https://moneyhero.page.link";
    FIRDynamicLinkComponents *linkBuilder = [[FIRDynamicLinkComponents alloc]
                                             initWithLink:link
                                                   domainURIPrefix:dynamicLinksDomainURIPrefix];
    linkBuilder.iOSParameters = [[FIRDynamicLinkIOSParameters alloc]
                                 initWithBundleID:@"com.maf.moneyhero"];
    linkBuilder.iOSParameters.appStoreID=@"987072755";
    linkBuilder.androidParameters = [[FIRDynamicLinkAndroidParameters alloc]
                                     initWithPackageName:@"com.maf.moneyhero"];
    
    NSLog(@"%@", linkBuilder.url.absoluteString);

    [linkBuilder shortenWithCompletion:^(NSURL* _Nullable shortURL, NSArray<NSString *>* _Nullable warnings, NSError* _Nullable error){
        
        if(error || shortURL == nil) { return; }
        
        NSLog(@"The short URL is: %@", shortURL);
        
        NSString* title = @"Invite Friend";
        NSArray* postItems = @[title, shortURL];
        //send
        UIActivityViewController *activityVC = [[UIActivityViewController alloc] initWithActivityItems:postItems applicationActivities:nil];
        activityVC.excludedActivityTypes = @[];
        
        AppController *app = GetAppController();
        [app.viewController presentViewController:activityVC animated:YES completion:nil];
    }];
}

- (void) receiveDeepLink:(NSString*)params{    
    MafNative::CBReceiveDeepLink([params UTF8String]);
}

- (void) copyString:(NSString*)text{
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    pasteboard.string = text;
}

- (void) shareString:(NSString*)text{
    NSArray* postItems = @[text];
    
    UIActivityViewController *activityVC = [[UIActivityViewController alloc] initWithActivityItems:postItems applicationActivities:nil];
    activityVC.excludedActivityTypes = @[];
    
    AppController *app = GetAppController();
    [app.viewController presentViewController:activityVC animated:YES completion:nil];
}
@end
