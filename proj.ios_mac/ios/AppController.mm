/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "AppController.h"
#import "cocos2d.h"
#import "AppDelegate.h"
#import "RootViewController.h"

#import <AVFoundation/AVAudioSession.h>
#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBAudienceNetwork/FBAudienceNetwork.h>
#import <AppTrackingTransparency/ATTrackingManager.h>
#import <AppTrackingTransparency/AppTrackingTransparency.h>

#import "MafiOSAppsFlyer.h"
#import "MafiOSNative.h"

#if defined(__IPHONE_10_0) && __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_10_0
#import <UserNotifications/UserNotifications.h>
@interface AppController () <UNUserNotificationCenterDelegate>
@end
#endif

@implementation AppController

@synthesize window;

// cocos2d application instance
static AppDelegate s_sharedApplication;
NSString *const kGCMMessageIDKey = @"gcm.message_id";

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    // Override point for customization after application launch.
    
    if ([self isJailbroken])
    {
        exit(0);
        return NO;
    }
    
    cocos2d::Application *app = cocos2d::Application::getInstance();
    
    // Initialize the GLView attributes
    app->initGLContextAttrs();
    cocos2d::GLViewImpl::convertAttrs();
    
    //    [[CDAudioManager sharedManager] setResignBehavior: kAMRBStopPlay autoHandle:YES];
    //    [[CDAudioManager sharedManager] setMode:kAMM_FxPlusMusicIfNoOtherAudio];
    [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryAmbient error:NULL];
    [[AVAudioSession sharedInstance] setActive:YES error:NULL];
    
    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    
    // Use RootViewController to manage CCEAGLView
    _viewController = [[RootViewController alloc]init];
    //    _viewController.wantsFullScreenLayout = YES;
    
    
    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: _viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:_viewController];
    }
    
    [window makeKeyAndVisible];
    
    //
    [FIRApp configure];
    [FIROptions defaultOptions].deepLinkURLScheme = @"https";
    [FIRMessaging messaging].delegate = self;
    [FIRDynamicLinks performDiagnosticsWithCompletion:nil];
    [[FBSDKApplicationDelegate sharedInstance] application:application didFinishLaunchingWithOptions:launchOptions];
    
    // Register for remote notifications. This shows a permission dialog on first run, to
    // show the dialog at a more appropriate time move this registration accordingly.
    // [START register_for_notifications]
    if ([UNUserNotificationCenter class] != nil) {
        // iOS 10 or later
        // For iOS 10 display notification (sent via APNS)
        [UNUserNotificationCenter currentNotificationCenter].delegate = self;
        UNAuthorizationOptions authOptions = UNAuthorizationOptionAlert | UNAuthorizationOptionSound | UNAuthorizationOptionBadge;
        [[UNUserNotificationCenter currentNotificationCenter] requestAuthorizationWithOptions:authOptions completionHandler:^(BOOL granted, NSError * _Nullable error) {
            
        }];
    } else {
        // iOS 10 notifications aren't available; fall back to iOS 8-9 notifications.
        UIUserNotificationType allNotificationTypes = (UIUserNotificationTypeSound | UIUserNotificationTypeAlert | UIUserNotificationTypeBadge);
        UIUserNotificationSettings *settings = [UIUserNotificationSettings settingsForTypes:allNotificationTypes categories:nil];
        [application registerUserNotificationSettings:settings];
    }

    [application registerForRemoteNotifications];
    // [END register_for_notifications]
    
    //IDFA 동의 팝업
    if (@available(iOS 14.5, *))
    {
        [ATTrackingManager requestTrackingAuthorizationWithCompletionHandler:^(ATTrackingManagerAuthorizationStatus status) {
            [FBAdSettings setAdvertiserTrackingEnabled: status == ATTrackingManagerAuthorizationStatusAuthorized ? YES : NO];
        }];
    }
    
    //
    [[UIApplication sharedApplication] setIdleTimerDisabled:YES];
    
    // IMPORTANT: Setting the GLView should be done after creating the RootViewController
    cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView((__bridge void *)_viewController.view);
    cocos2d::Director::getInstance()->setOpenGLView(glview);
    
    //run the cocos2d-x game scene
    app->run();

    
    
    
    return YES;
}

#pragma mark - Push Notification
// [START receive_message]
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo {
    // If you are receiving a notification message while your app is in the background,
    // this callback will not be fired till the user taps on the notification launching the application.
    // TODO: Handle data of notification

    // With swizzling disabled you must let Messaging know about the message, for Analytics
    // [[FIRMessaging messaging] appDidReceiveMessage:userInfo];

    // Print message ID.
    if (userInfo[kGCMMessageIDKey]) {
        NSLog(@"Message ID: %@", userInfo[kGCMMessageIDKey]);
    }

    // Print full message.
    NSLog(@"%@", userInfo);
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler {
    // If you are receiving a notification message while your app is in the background,
    // this callback will not be fired till the user taps on the notification launching the application.
    // TODO: Handle data of notification

    // With swizzling disabled you must let Messaging know about the message, for Analytics
    // [[FIRMessaging messaging] appDidReceiveMessage:userInfo];

    // Print message ID.
    if (userInfo[kGCMMessageIDKey]) {
        NSLog(@"Message ID: %@", userInfo[kGCMMessageIDKey]);
    }

    // Print full message.
    NSLog(@"%@", userInfo);

    completionHandler(UIBackgroundFetchResultNewData);
}
// [END receive_message]

#if defined(__IPHONE_10_0) && __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_10_0
// [START ios_10_message_handling]
// Receive displayed notifications for iOS 10 devices.
// Handle incoming notification messages while app is in the foreground.
- (void)userNotificationCenter:(UNUserNotificationCenter *)center willPresentNotification:(UNNotification *)notification withCompletionHandler:(void (^)(UNNotificationPresentationOptions))completionHandler {
    NSDictionary *userInfo = notification.request.content.userInfo;

    // With swizzling disabled you must let Messaging know about the message, for Analytics
    // [[FIRMessaging messaging] appDidReceiveMessage:userInfo];

    // Print message ID.
    if (userInfo[kGCMMessageIDKey]) {
        NSLog(@"Message ID: %@", userInfo[kGCMMessageIDKey]);
    }

    // Print full message.
    NSLog(@"%@", userInfo);

    // Change this to your preferred presentation option
    completionHandler(UNNotificationPresentationOptionBadge | UNNotificationPresentationOptionAlert);
}

// Handle notification messages after display notification is tapped by the user.
- (void)userNotificationCenter:(UNUserNotificationCenter *)center didReceiveNotificationResponse:(UNNotificationResponse *)response withCompletionHandler:(void (^)())completionHandler {
    NSDictionary *userInfo = response.notification.request.content.userInfo;
    if (userInfo[kGCMMessageIDKey]) {
        NSLog(@"Message ID: %@", userInfo[kGCMMessageIDKey]);
    }

    // With swizzling disabled you must let Messaging know about the message, for Analytics
    // [[FIRMessaging messaging] appDidReceiveMessage:userInfo];

    // Print full message.
    NSLog(@"%@", userInfo);

    completionHandler();
}
// [END ios_10_message_handling]
#endif


// [START refresh_token]
- (void)messaging:(FIRMessaging *)messaging didReceiveRegistrationToken:(NSString *)fcmToken {
    NSLog(@"FCM registration token: %@", fcmToken);
    // Notify about received token.
    NSDictionary *dataDict = [NSDictionary dictionaryWithObject:fcmToken forKey:@"token"];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"FCMToken" object:nil userInfo:dataDict];
    // TODO: If necessary send token to application server.
    // Note: This callback is fired at each app startup and whenever a new token is generated.
    
    //
    cocos2d::UserDefault::getInstance()->setStringForKey("token", [fcmToken UTF8String]);
}
// [END refresh_token]


#pragma mark -
- (BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<UIApplicationOpenURLOptionsKey,id> *)options{
    return [self application:app
              openURL:url
    sourceApplication:options[UIApplicationOpenURLOptionsSourceApplicationKey]
           annotation:options[UIApplicationOpenURLOptionsAnnotationKey]];
}
- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {
    
    FIRDynamicLink *dynamicLink = [[FIRDynamicLinks dynamicLinks] dynamicLinkFromCustomSchemeURL:url];
    
    BOOL dynamicLinkResult = false;
    if (dynamicLink) {
        if (dynamicLink.url) {
            
            NSURL *url = dynamicLink.url;
            
            NSString * query_str = [url query];
            
            NSLog(@"%@", [url absoluteString]);
            NSLog(@"%@", [url query]);
            
            NSMutableDictionary *params = [[NSMutableDictionary alloc] init];
            if([query_str containsString:@"&"] == true)
            {
                for (NSString *param in [query_str componentsSeparatedByString:@"&"]) {
                    NSArray *parts = [param componentsSeparatedByString:@"="];
                    if([parts count] < 2) continue;
                    [params setObject:[parts objectAtIndex:1] forKey:[parts objectAtIndex:0]];
                }
            }else
            {
                if([[url query] containsString:@"="] == true)
                {
                    NSArray *parts = [query_str componentsSeparatedByString:@"="];
                    [params setObject:[parts objectAtIndex:1] forKey:[parts objectAtIndex:0]];
                }
                else
                {
                    [params setObject:query_str forKey:@"none"];
                }
            }
            
            NSString* result = @"";
            for(NSString * key in params)
            {
                NSLog(@"%@ : %@", key, params[key]);
                result = [NSString stringWithFormat:@"%@&%@=%@", result, key, params[key]];
                //
            }
            
            [[MafiOSNative sharediOSNativePlugin]receiveDeepLink:result];
            
        } else {
            // Dynamic link has empty deep link. This situation will happens if
            // Firebase Dynamic Links iOS SDK tried to retrieve pending dynamic link,
            // but pending link is not available for this device/App combination.
            // At this point you may display default onboarding view.
        }
        dynamicLinkResult = true;
    }else{
        dynamicLinkResult = false;
    }
    
    return ([[FBSDKApplicationDelegate sharedInstance] application:application
                                                          openURL:url
                                                sourceApplication:sourceApplication
                                                       annotation:annotation
            ]&&dynamicLinkResult);
}

- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
     //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->pause(); */
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
     //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->resume(); */
    
    [[MafiOSAppsFlyer sharediOSAppsFlyerPlugin] TrackAppLaunch];
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    if ([self isJailbroken])
    {
        exit(0);
        return;
    }
    
    cocos2d::Application::getInstance()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::Application::getInstance()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}

- (BOOL)application:(UIApplication *)application
continueUserActivity:(nonnull NSUserActivity *)userActivity
 restorationHandler:
#if defined(__IPHONE_12_0) && (__IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_12_0)
(nonnull void (^)(NSArray<id<UIUserActivityRestoring>> *_Nullable))restorationHandler {
#else
    (nonnull void (^)(NSArray *_Nullable))restorationHandler {
#endif  // __IPHONE_12_0
  BOOL handled = [[FIRDynamicLinks dynamicLinks] handleUniversalLink:userActivity.webpageURL
                                                          completion:^(FIRDynamicLink * _Nullable dynamicLink,
                                                                       NSError * _Nullable error) {
                                                            // ...
      if (dynamicLink) {
          if (dynamicLink.url) {
              
              NSURL *url = dynamicLink.url;
              
              NSString * query_str = [url query];
              
              NSLog(@"%@", [url absoluteString]);
              NSLog(@"%@", [url query]);
              
              NSMutableDictionary *params = [[NSMutableDictionary alloc] init];
              if([query_str containsString:@"&"] == true)
              {
                  for (NSString *param in [query_str componentsSeparatedByString:@"&"]) {
                      NSArray *parts = [param componentsSeparatedByString:@"="];
                      if([parts count] < 2) continue;
                      [params setObject:[parts objectAtIndex:1] forKey:[parts objectAtIndex:0]];
                  }
              }else
              {
                  if([[url query] containsString:@"="] == true)
                  {
                      NSArray *parts = [query_str componentsSeparatedByString:@"="];
                      [params setObject:[parts objectAtIndex:1] forKey:[parts objectAtIndex:0]];
                  }
                  else
                  {
                      [params setObject:query_str forKey:@"none"];
                  }
              }
              
              NSString* result = @"";
              for(NSString * key in params)
              {
                  NSLog(@"%@ : %@", key, params[key]);
                  result = [NSString stringWithFormat:@"%@&%@=%@", result, key, params[key]];
                  //
              }
              
              [[MafiOSNative sharediOSNativePlugin]receiveDeepLink:result];
              
          } else {
              // Dynamic link has empty deep link. This situation will happens if
              // Firebase Dynamic Links iOS SDK tried to retrieve pending dynamic link,
              // but pending link is not available for this device/App combination.
              // At this point you may display default onboarding view.
          }
      }else{
      }
  
                                                          
  }];

  return handled;
}


#pragma mark -

- (UIInterfaceOrientationMask)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(nullable UIWindow *)window{
    return UIInterfaceOrientationMaskAll;
}

#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}

#if __has_feature(objc_arc)
#else
- (void)dealloc {
    [window release];
    [_viewController release];
    [super dealloc];
}
#endif


- (BOOL)isJailbroken {
    NSString *strCnt = [[NSBundle mainBundle] bundleIdentifier];
    
    //com.maf.moneyhero
    const unsigned int noCnt[17] = {
        99, 111, 109, 46,
        109, 97, 102, 46,
        109, 111, 110, 101, 121, 104, 101, 114, 111};
    for (int i = 0; i < 15; ++i)
    {
        int cCnt = [strCnt characterAtIndex:i];
        if (cCnt != noCnt[i])
        {
            return YES;
        }
    }
#if !(TARGET_IPHONE_SIMULATOR)
    // Check 1 : 탈옥된 장치에 있는 파일 탐색
    if ([[NSFileManager defaultManager] fileExistsAtPath:@"/Applications/Cydia.app"] ||
        [[NSFileManager defaultManager] fileExistsAtPath:@"/Library/MobileSubstrate/MobileSubstrate.dylib"] ||
        [[NSFileManager defaultManager] fileExistsAtPath:@"/bin/bash"] ||
        [[NSFileManager defaultManager] fileExistsAtPath:@"/usr/sbin/sshd"] ||
        [[NSFileManager defaultManager] fileExistsAtPath:@"/etc/apt"] ||
        [[NSFileManager defaultManager] fileExistsAtPath:@"/private/var/lib/apt/"] ||
        [[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:@"cydia://package/com.example.package"]])
    {
        return YES;
    }
 
    FILE *f = NULL;
    if ((f = fopen("/bin/bash", "r")) ||
        (f = fopen("/Applications/Cydia.app", "r")) ||
        (f = fopen("/Library/MobileSubstrate/MobileSubstrate.dylib", "r")) ||
        (f = fopen("/usr/sbin/sshd", "r")) ||
        (f = fopen("/etc/apt", "r")))
    {
        fclose(f);
        return YES;
    }
    fclose(f);
 
    // Check 2 : 시스템 디렉토리 읽고 쓰기 (sandbox violation)
    NSError *error;
    [@"Jailbreak Test" writeToFile:@"/private/jailbreak.txt" atomically:YES encoding:NSUTF8StringEncoding error:&error];
 
    if(error == nil)
    {
        // 탈옥된 장치임 Rooting Device
        return YES;
    }
    else
    {
        [[NSFileManager defaultManager] removeItemAtPath:@"/private/jailbreak.txt" error:nil];
    }
#endif
    return NO;
}
@end
