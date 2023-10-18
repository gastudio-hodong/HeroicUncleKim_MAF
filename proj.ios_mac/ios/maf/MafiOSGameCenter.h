//
//  MafGameCenter.h
//  Unity-iPhone
//
//  Created by SanHeo on 2014. 7. 14..
//
//

#import <Foundation/Foundation.h>
#import <GameKit/GameKit.h>

@interface MafiOSGameCenter : NSObject<
//GKLeaderboardViewControllerDelegate
//,GKAchievementViewControllerDelegate,
GKGameCenterControllerDelegate

>
//,GKGameCenterControllerDelegate>
{
//    NSString *strGameCenterId;
//    UIViewController *loginvc;
    //BOOL enableGameCenter;
    
//    NSString *strErrorMsg;
    
    enum class STATE{
        SUCCESS=1,
        FAIL,
        TRY
    };
    STATE gameCenterState;
}

//@property (retain, nonatomic) NSString *strGameCenterId,*strErrorMsg;
@property (retain, nonatomic) NSString *strErrorMsg;
@property (retain, nonatomic) UIViewController *loginvc;

+ (MafiOSGameCenter*) sharediOSGameCenterPlugin;
- (void) initGameCenter;
- (void) loginGameCenter;
- (void) reloginGameCenter;
- (void) gameCenterAuthenticate;
//- (void)gameCenterAuthenticationChanged;
- (void) updateLeaderBoardScore:(NSString*)strLbId score:(int)score;
- (void) showLeaderBoard:(NSString*)strLbId;
- (BOOL) isSignedIn;
- (const char*) getGameCenterId;

- (void) showAchievement;
- (void) unlockProgressAchievement:(NSString*)strId point:(int)point clearPoint:(int)clearPoint;
- (void) incrementAchievement:(NSString*)strId point:(int)point clearPoint:(int)clearPoint;
- (void) IncrementImmediateAchievement:(NSString*)strId point:(int)point clearPoint:(int)clearPoint;

//- (void)leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)viewController;
//- (void) achievementViewControllerDidFinish:(GKAchievementViewController *)viewController;


@end
