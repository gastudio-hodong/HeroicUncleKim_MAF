#import <UIKit/UIKit.h>
#import "Firebase.h"
@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate, FIRMessagingDelegate> {

}

@property(nonatomic, readonly) RootViewController* viewController;


@end

inline AppController*	GetAppController()
{
    return (AppController*)[UIApplication sharedApplication].delegate;
}
