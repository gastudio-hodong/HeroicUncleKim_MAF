//
//  MafiOSShareSNS.m
//  HexagonSnake
//
//  Created by SanHeo on 2014. 9. 3..
//
//

#import "MafiOSShareSNS.h"
#import "../AppController.h"
#import "Social/Social.h"

extern "C"
{
    
    void iOSPostFacebook(){
        //- (void)PostFacebook:(NSString*)strFileUrl title:(NSString*)strTitle link:(NSString*)strLinkUrl;
        //[[MafiOSShareSNS sharediOSMafiOSShareSNSPlugin] PostFacebook];
    }

    void iOSPostTwitter(){
        //[[MafiOSShareSNS sharediOSMafiOSShareSNSPlugin] PostTwitter];
    }
}


@implementation MafiOSShareSNS

+ (MafiOSShareSNS*) sharediOSMafiOSShareSNSPlugin
{
    static MafiOSShareSNS* pInstance;
    if(pInstance == NULL)
    {
        pInstance = [[MafiOSShareSNS alloc] init];
    }
    return pInstance;
}

- (void)PostFacebook:(NSString*)strFileUrl title:(NSString*)strTitle link:(NSString*)strLinkUrl
{
    AppController *app = GetAppController();
    NSURL *urlDownLoad = [NSURL URLWithString:strFileUrl];
    NSURLResponse * response = nil;
    NSError * error = nil;
    NSURLRequest *request = [NSURLRequest requestWithURL:urlDownLoad];
    NSData * data = [NSURLConnection sendSynchronousRequest:request
                                          returningResponse:&response
                                                      error:&error];
    if (error != nil){
        // Parse data here
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Sorry"
                                                            message:@"Error. Try again."
                                                           delegate:self cancelButtonTitle:@"OK"
                                                  otherButtonTitles: nil];
        [alertView show];
        return;
    }
    
    UIImage *img = [UIImage imageWithData:data];
    if ([SLComposeViewController isAvailableForServiceType:SLServiceTypeFacebook]) {
        SLComposeViewController *composer = [SLComposeViewController composeViewControllerForServiceType:SLServiceTypeFacebook];
        [composer setInitialText:strTitle];//초기 메시지.(타이틀이 아닌 제일앞에 메시지이다.
        [composer addImage:img]; //추가할 이미지
        [composer addURL:[NSURL URLWithString:strLinkUrl]];//추가할 URL
        composer.completionHandler = ^(SLComposeViewControllerResult result){
            switch(result) {
                    //  This means the user cancelled without sending the Tweet
                case SLComposeViewControllerResultCancelled:
                    break;
                    //  This means the user hit 'Send'
                case SLComposeViewControllerResultDone:
//                    [[MafiOSAnalytics sharediOSAnalyticsPlugin] SendTracking:@"ShareSNS" action:@"Post" label:@"Facebook_iOS" val:1];
                    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"" message:@"Thank you for sharing."
                                                                delegate:self cancelButtonTitle:@"OK" otherButtonTitles: nil];
                    [alertView show];
                    break;
            }
        };
        [app.viewController presentViewController:composer animated:YES completion:^{
            //NSLog(@"present completed");
        }];
    }else{
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Sorry"
                                                            message:@"You can't upload this page to Facebook right now, make sure your device has an internet connection and you have at least one Facebook account setup"
                                                           delegate:self cancelButtonTitle:@"OK"
                                                  otherButtonTitles: nil];
        [alertView show];
    }
    
    
    //    UIActivityViewController *controller = [[[UIActivityViewController alloc]initWithActivityItems:
    //                                             @[text, url, img] applicationActivities:nil]autorelease];
    //    controller.completionHandler = ^(NSString *activityType, BOOL completed){
    //        NSLog(@"Activity Type selected: %@", activityType);
    //        if (completed) {
    //            NSLog(@"Selected activity was performed.");
    //        } else {
    //            if (activityType == NULL) {
    //                NSLog(@"User dismissed the view controller without making a selection.");
    //            } else {
    //                NSLog(@"Activity was not performed.");
    //            }
    //        }
    //    };
    //    controller.excludedActivityTypes =
    //    @[UIActivityTypePostToTwitter,
    //    [app.viewController presentViewController:controller animated:YES completion:nil];
    
}

- (void)PostTwitter:(NSString*)strFileUrl title:(NSString*)strTitle link:(NSString*)strLinkUrl
{
    AppController *app = GetAppController();
    NSURL *urlDownLoad = [NSURL URLWithString:strFileUrl];
    NSURLResponse * response = nil;
    NSError * error = nil;
    NSURLRequest *request = [NSURLRequest requestWithURL:urlDownLoad];
    NSData * data = [NSURLConnection sendSynchronousRequest:request
                                          returningResponse:&response
                                                      error:&error];
    if (error != nil){
        // Parse data here
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Sorry"
                                                            message:@"Error. Try again."
                                                           delegate:self cancelButtonTitle:@"OK"
                                                  otherButtonTitles: nil];
        [alertView show];
        return;
    }
    
    UIImage *img = [UIImage imageWithData:data];
    if ([SLComposeViewController isAvailableForServiceType:SLServiceTypeTwitter]) {
        SLComposeViewController *composer = [SLComposeViewController composeViewControllerForServiceType:SLServiceTypeTwitter];
        [composer setInitialText:strTitle];//초기 메시지.(타이틀이 아닌 제일앞에 메시지이다.
        [composer addImage:img]; //추가할 이미지
        [composer addURL:[NSURL URLWithString:strLinkUrl]];//추가할 URL
        composer.completionHandler = ^(SLComposeViewControllerResult result){
            switch(result) {
                    //  This means the user cancelled without sending the Tweet
                case SLComposeViewControllerResultCancelled:
                    break;
                    //  This means the user hit 'Send'
                case SLComposeViewControllerResultDone:
//                    [[MafiOSAnalytics sharediOSAnalyticsPlugin] SendTracking:@"ShareSNS" action:@"Post" label:@"Twitter_iOS" val:1];
                    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"" message:@"Thank you for sharing."
                                                                       delegate:self cancelButtonTitle:@"OK" otherButtonTitles: nil];
                    [alertView show];
                    break;
            }
        };
        [app.viewController presentViewController:composer animated:YES completion:^{
            //NSLog(@"present completed");
        }];
    }else{
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Sorry"
                                                            message:@"You can't upload this page to Twitter right now, make sure your device has an internet connection and you have at least one Twitter account setup"
                                                           delegate:self cancelButtonTitle:@"OK"
                                                  otherButtonTitles: nil];
        [alertView show];
    }
}

- (void)PostFacebookFromLocal:(const char*)strData size:(int)nDataSize title:(NSString*)strTitle link:(NSString*)strLinkUrl
{
    AppController *app = GetAppController();
    
    //NSData* imageData = [[NSData alloc] initWithBytes:strData length:strlen(strData)];
    NSData* imageData = [[NSData alloc] initWithBytes:strData length:nDataSize];
    UIImage* img = [[UIImage alloc] initWithData:imageData];

    if ([SLComposeViewController isAvailableForServiceType:SLServiceTypeFacebook]) {
        SLComposeViewController *composer = [SLComposeViewController composeViewControllerForServiceType:SLServiceTypeFacebook];
        [composer setInitialText:strTitle];//초기 메시지.(타이틀이 아닌 제일앞에 메시지이다.
        [composer addImage:img]; //추가할 이미지
        [composer addURL:[NSURL URLWithString:strLinkUrl]];//추가할 URL
        composer.completionHandler = ^(SLComposeViewControllerResult result){
            switch(result) {
                    //  This means the user cancelled without sending the Tweet
                case SLComposeViewControllerResultCancelled:
                    break;
                    //  This means the user hit 'Send'
                case SLComposeViewControllerResultDone:
//                    [[MafiOSAnalytics sharediOSAnalyticsPlugin] SendTracking:@"ShareSNS" action:@"Post" label:@"Facebook_iOS" val:1];
                    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"" message:@"Thank you for sharing."
                                                                       delegate:self cancelButtonTitle:@"OK" otherButtonTitles: nil];
                    [alertView show];
                    break;
            }
        };
        [app.viewController presentViewController:composer animated:YES completion:^{
            //NSLog(@"present completed");
        }];
    }else{
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Sorry"
                                                            message:@"You can't upload this page to Facebook right now, make sure your device has an internet connection and you have at least one Facebook account setup"
                                                           delegate:self cancelButtonTitle:@"OK"
                                                  otherButtonTitles: nil];
        [alertView show];
    }
}

- (void)PostTwitterFromLocal:(const char*)strData size:(int)nDataSize title:(NSString*)strTitle link:(NSString*)strLinkUrl{
    
    AppController *app = GetAppController();
    
    //NSData* imageData = [[NSData alloc] initWithBytes:strData length:strlen(strData)];
    NSData* imageData = [[NSData alloc] initWithBytes:strData length:nDataSize];
    UIImage* img = [[UIImage alloc] initWithData:imageData];
    
    if ([SLComposeViewController isAvailableForServiceType:SLServiceTypeTwitter]) {
        SLComposeViewController *composer = [SLComposeViewController composeViewControllerForServiceType:SLServiceTypeTwitter];
        [composer setInitialText:strTitle];//초기 메시지.(타이틀이 아닌 제일앞에 메시지이다.
        [composer addImage:img]; //추가할 이미지
        [composer addURL:[NSURL URLWithString:strLinkUrl]];//추가할 URL
        composer.completionHandler = ^(SLComposeViewControllerResult result){
            switch(result) {
                    //  This means the user cancelled without sending the Tweet
                case SLComposeViewControllerResultCancelled:
                    break;
                    //  This means the user hit 'Send'
                case SLComposeViewControllerResultDone:
//                    [[MafiOSAnalytics sharediOSAnalyticsPlugin] SendTracking:@"ShareSNS" action:@"Post" label:@"Twitter_iOS" val:1];
                    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"" message:@"Thank you for sharing."
                                                                       delegate:self cancelButtonTitle:@"OK" otherButtonTitles: nil];
                    [alertView show];
                    break;
            }
        };
        [app.viewController presentViewController:composer animated:YES completion:^{
            //NSLog(@"present completed");
        }];
    }else{
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Sorry"
                                                            message:@"You can't upload this page to Twitter right now, make sure your device has an internet connection and you have at least one Twitter account setup"
                                                           delegate:self cancelButtonTitle:@"OK"
                                                  otherButtonTitles: nil];
        [alertView show];
    }
    
}


// called to determine data type. only the class of the return type is consulted. it should match what -itemForActivityType: returns later



/*!
 @abstract
 Tells the delegate that the view is now in logged in mode
 @param loginView   The login view that transitioned its view mode
 */
//- (void)loginViewShowingLoggedInUser:(FBLoginView *)loginView
//{
//    NSLog(@"loginViewShowingLoggedInUser");
//}

/*!
 @abstract
 Tells the delegate that the view is has now fetched user info
 @param loginView   The login view that transitioned its view mode
 @param user        The user info object describing the logged in user
 */
//- (void)loginViewFetchedUserInfo:(FBLoginView *)loginView user:(id<FBGraphUser>)user
//{
//    NSLog(@"loginViewFetchedUserInfo");
//    profilePictureView.profileID = [user objectID];
//}

/*!
 @abstract
 Tells the delegate that the view is now in logged out mode
 @param loginView   The login view that transitioned its view mode
 */
//- (void)loginViewShowingLoggedOutUser:(FBLoginView *)loginView
//{
//    NSLog(@"loginViewShowingLoggedOutUser");
//    profilePictureView.profileID = nil;
//}

/*!
 @abstract
 Tells the delegate that there is a communication or authorization error.
 @param loginView           The login view that transitioned its view mode
 @param error               An error object containing details of the error.
 @discussion See https://developers.facebook.com/docs/technical-guides/iossdk/errors/
 for error handling best practices.
 */
//- (void)loginView:(FBLoginView *)loginView handleError:(NSError *)error
//{
//    NSLog(@"loginView handleError");
//    NSString *alertMessage, *alertTitle;
//
//    // If the user should perform an action outside of you app to recover,
//    // the SDK will provide a message for the user, you just need to surface it.
//    // This conveniently handles cases like Facebook password change or unverified Facebook accounts.
//    if ([FBErrorUtility shouldNotifyUserForError:error]) {
//        alertTitle = @"Facebook error";
//        alertMessage = [FBErrorUtility userMessageForError:error];
//
//        // This code will handle session closures that happen outside of the app
//        // You can take a look at our error handling guide to know more about it
//        // https://developers.facebook.com/docs/ios/errors
//    } else if ([FBErrorUtility errorCategoryForError:error] == FBErrorCategoryAuthenticationReopenSession) {
//        alertTitle = @"Session Error";
//        alertMessage = @"Your current session is no longer valid. Please log in again.";
//
//        // If the user has cancelled a login, we will do nothing.
//        // You can also choose to show the user a message if cancelling login will result in
//        // the user not being able to complete a task they had initiated in your app
//        // (like accessing FB-stored information or posting to Facebook)
//    } else if ([FBErrorUtility errorCategoryForError:error] == FBErrorCategoryUserCancelled) {
//        NSLog(@"user cancelled login");
//
//        // For simplicity, this sample handles other errors with a generic message
//        // You can checkout our error handling guide for more detailed information
//        // https://developers.facebook.com/docs/ios/errors
//    } else {
//        alertTitle  = @"Something went wrong";
//        alertMessage = @"Please try again later.";
//        NSLog(@"Unexpected error:%@", error);
//    }
//
//    if (alertMessage) {
//        [[[UIAlertView alloc] initWithTitle:alertTitle
//                                    message:alertMessage
//                                   delegate:nil
//                          cancelButtonTitle:@"OK"
//                          otherButtonTitles:nil] show];
//    }
//}








@end
