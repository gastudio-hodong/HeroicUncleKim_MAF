//
//  MafiOSShareSNS.h
//  HexagonSnake
//
//  Created by SanHeo on 2014. 9. 3..
//
//

#import <Foundation/Foundation.h>

@interface MafiOSShareSNS : NSObject<NSURLConnectionDelegate>

+ (MafiOSShareSNS*) sharediOSMafiOSShareSNSPlugin;


//(const char* strFileUrl,const char* strTitle,const char* strLinkUrl)
//- (void) SendTracking:(NSString*)strCate action:(NSString*)strAction label:(NSString*)strLabel val:(long)val;
- (void)PostFacebook:(NSString*)strFileUrl title:(NSString*)strTitle link:(NSString*)strLinkUrl;
- (void)PostTwitter:(NSString*)strFileUrl title:(NSString*)strTitle link:(NSString*)strLinkUrl;

- (void)PostFacebookFromLocal:(const char*)strData size:(int)nDataSize title:(NSString*)strTitle link:(NSString*)strLinkUrl;
- (void)PostTwitterFromLocal:(const char*)strData size:(int)nDataSize title:(NSString*)strTitle link:(NSString*)strLinkUrl;
//

@end
