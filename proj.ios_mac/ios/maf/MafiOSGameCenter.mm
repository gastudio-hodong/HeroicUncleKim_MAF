//
//  MafGameCenter.m
//  Unity-iPhone
//
//  Created by SanHeo on 2014. 7. 14..
//
//

#import "MafiOSGameCenter.h"
#import "MafGooglePlay.h"
#import "../AppController.h"
@implementation MafiOSGameCenter

@synthesize strErrorMsg,loginvc;//strGameCenterId

+ (MafiOSGameCenter*) sharediOSGameCenterPlugin{
    static MafiOSGameCenter* pInstance;
    if(pInstance == NULL)
    {
        pInstance = [[MafiOSGameCenter alloc] init];
    }
    return pInstance;
}

- (void) initGameCenter{
    loginvc = nil;
    gameCenterState = STATE::TRY;
    strErrorMsg = nil;
}

- (void) loginGameCenter
{
    //
    [self gameCenterAuthenticate];
}

- (void) reloginGameCenter{
    
    if ( loginvc != nil )
    {
        AppController *app = GetAppController();
        [app.viewController  presentViewController:loginvc animated:YES completion:nil];
    }
    else
    {
        if ( gameCenterState == STATE::FAIL )
        { 
            //
            MafGooglePlay::CBReceiveLogin(false, "");
        }
    }
}

//- (void)gameCenterAuthenticationChanged {
//    [self gameCenterAuthenticate]; // In case the player makes logout we prompt it with login again.
//}

- (void)gameCenterAuthenticate
{    
    GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
    if (localPlayer.isAuthenticated == NO) {
        localPlayer.authenticateHandler = ^(UIViewController *viewController, NSError *error){
            if (viewController != nil)
            {
                //
                MafGooglePlay::CBReceiveLogin(false, "");
                
                //
                if(loginvc != nil)
                    [loginvc release];
                loginvc = viewController;
                [loginvc retain];
            }
            else if ( localPlayer.isAuthenticated )
            {// Player has been authenticated!
                gameCenterState = STATE::SUCCESS;
                
                //
                MafGooglePlay::CBReceiveLogin(true, localPlayer.playerID.UTF8String);
                
                //
                if(loginvc != nil)
                    [loginvc release];
                loginvc = nil;
            }
            else
            {
                gameCenterState = STATE::FAIL;
                
                //
                MafGooglePlay::CBReceiveLogin(false, "");
            }
        };
    }
}


- (BOOL) isSignedIn{
    GKLocalPlayer *player = [GKLocalPlayer localPlayer];
    return player.isAuthenticated;
}

- (const char*) getGameCenterId{
    GKLocalPlayer *player = [GKLocalPlayer localPlayer];
    if(player.isAuthenticated)
        return player.playerID.UTF8String;
//    if (self.strGameCenterId != nil)
//        return self.strGameCenterId.UTF8String;
    else
        return "";
}


- (void) updateLeaderBoardScore:(NSString*)strLbId score:(int)score{
//    GKScore *scoreReporter = [[[GKScore alloc]initWithCategory:strLbId]autorelease];
//    scoreReporter.value = score;
//    [scoreReporter reportScoreWithCompletionHandler:^(NSError *error){
//        if(error != nil){
//            NSLog(@"updateLeaderBoardScore error");
//        }
//    }];
    GKScore *scoreReporter = [[[GKScore alloc]initWithLeaderboardIdentifier:strLbId]autorelease];
    scoreReporter.value = score;
    NSArray *scores = @[scoreReporter];
    //+ (void)reportScores:(NSArray *)scores withCompletionHandler:(void(^)(NSError *error))completionHandler
    [GKScore reportScores:scores withCompletionHandler:^(NSError *error){
        if(error != nil){
            NSLog(@"updateLeaderBoardScore error");
        }
    }];
    
//    [scoreReporter reportScoreWithCompletionHandler:^(NSError *error){
//        if(error != nil){
//            NSLog(@"updateLeaderBoardScore error");
//        }
//    }];

    
}

- (void) showLeaderBoard:(NSString*)strLbId{
    
    if([self isSignedIn]==false){
        [self reloginGameCenter];
    }
    
    GKGameCenterViewController *gameCenterController = [[GKGameCenterViewController alloc] init];
    if (gameCenterController != nil)
    {
        gameCenterController.gameCenterDelegate = self;
        gameCenterController.viewState = GKGameCenterViewControllerStateLeaderboards;
        gameCenterController.leaderboardTimeScope = GKLeaderboardTimeScopeAllTime;
//        gameCenterController.leaderboardCategory = strLbId;
//        gameCenterController.leaderboardIdentifier = strLbId;
//        [self presentViewController: gameCenterController animated: YES completion:nil];
        
        AppController *app = GetAppController();
        [app.viewController presentViewController:gameCenterController animated:YES completion:nil];
    }
    
//    GKLeaderboardViewController *pController = [[[GKLeaderboardViewController alloc]init]autorelease];
//    if(pController != nil){
//        pController.leaderboardDelegate = self;
//        AppController *app = GetAppController();
//        //[app.viewController presentModalViewController:pController animated:YES];
//        [app.viewController presentViewController:pController animated:NO completion:NULL];
//    }
}

#pragma mark -
- (void) showAchievement{

    if([self isSignedIn]==false){
        [self reloginGameCenter];
    }
    
//    GKAchievementViewController* pGKViewController = [[[GKAchievementViewController alloc] init] autorelease];
//    if( pGKViewController != nil ){
//        pGKViewController.achievementDelegate = self;
//        AppController *app = GetAppController();
//        //[app.viewController presentModalViewController:pGKViewController animated:YES];
//        [app.viewController presentViewController:pGKViewController animated:NO completion:NULL];
//    }
    GKGameCenterViewController *gameCenterController = [[GKGameCenterViewController alloc] init];
    if (gameCenterController != nil)
    {
        gameCenterController.gameCenterDelegate = self;
        gameCenterController.viewState = GKGameCenterViewControllerStateAchievements;
        gameCenterController.leaderboardTimeScope = GKLeaderboardTimeScopeAllTime;
        //        [self presentViewController: gameCenterController animated: YES completion:nil];
        
        AppController *app = GetAppController();
        [app.viewController presentViewController:gameCenterController animated:YES completion:nil];
    }
    

}

- (void) unlockProgressAchievement:(NSString*)strId point:(int)point clearPoint:(int)clearPoint{
    
    NSMutableDictionary *achievementsDictionary = [[[NSMutableDictionary alloc]init]autorelease];
    [GKAchievement loadAchievementsWithCompletionHandler:^(NSArray *achievements, NSError *error){
        if (error == nil)
        {
            double percent = 0;
            for (GKAchievement* achievement in achievements)
                [achievementsDictionary setObject: achievement forKey: achievement.identifier];
            
            GKAchievement* pAchievement = [[[GKAchievement alloc] initWithIdentifier:strId] autorelease];
            if( pAchievement){
                GKAchievement *originAchievement =[achievementsDictionary objectForKey:strId];
                if(originAchievement != nil){
                    if(originAchievement.completed)
                        return;
//                    percent += originAchievement.percentComplete;
                }
                
                if(point >= clearPoint)    percent = 100;
                else                       percent = (double)point/clearPoint*100;
                
                if(percent>100)
                    percent = 100;
                
                pAchievement.showsCompletionBanner = YES;
                pAchievement.percentComplete = percent;
                
                NSArray *arr = [NSArray arrayWithObjects:pAchievement, nil];
                [GKAchievement reportAchievements:arr withCompletionHandler:^(NSError *error)
                 {
                     if( error != nil ){
                     }
                     else{
                     }
                     
                 }];
            }
        }
    }];
}

- (void) incrementAchievement:(NSString*)strId point:(int)point clearPoint:(int)clearPoint{
    
    NSMutableDictionary *achievementsDictionary = [[[NSMutableDictionary alloc]init]autorelease];
    [GKAchievement loadAchievementsWithCompletionHandler:^(NSArray *achievements, NSError *error){
        if (error == nil)
        {
            double percent = 0;
            for (GKAchievement* achievement in achievements)
                [achievementsDictionary setObject: achievement forKey: achievement.identifier];
            
            GKAchievement* pAchievement = [[[GKAchievement alloc] initWithIdentifier:strId] autorelease];
            if( pAchievement){
                GKAchievement *originAchievement =[achievementsDictionary objectForKey:strId];
                if(originAchievement != nil){
                    if(originAchievement.completed)
                        return;
                    percent += originAchievement.percentComplete;
//                    NSLog(@"origin %f",percent);
                }
                
                if(point >= clearPoint)    percent = 100;
                else                       percent += (double)point/clearPoint*100;
                
                if(percent>100)
                    percent = 100;
                
                pAchievement.showsCompletionBanner = YES;
                pAchievement.percentComplete = percent;
                
                NSArray *arr = [NSArray arrayWithObjects:pAchievement, nil];
                [GKAchievement reportAchievements:arr withCompletionHandler:^(NSError *error)
                 {
                     if( error != nil ){
                     }
                     else{
//                         NSLog(@"incrementAchievement %@ , %f",strId,percent);
                     }
                     
                 }];
            }
        }
    }];
}

- (void) IncrementImmediateAchievement:(NSString*)strId point:(int)point clearPoint:(int)clearPoint{
    
    NSMutableDictionary *achievementsDictionary = [[[NSMutableDictionary alloc]init]autorelease];
    [GKAchievement loadAchievementsWithCompletionHandler:^(NSArray *achievements, NSError *error){
        if (error == nil)
        {
            double percent = 0;
            for (GKAchievement* achievement in achievements)
                [achievementsDictionary setObject: achievement forKey: achievement.identifier];
            
            GKAchievement* pAchievement = [[[GKAchievement alloc] initWithIdentifier:strId] autorelease];
            if( pAchievement){
                GKAchievement *originAchievement =[achievementsDictionary objectForKey:strId];
                if(originAchievement != nil){
                    if(originAchievement.completed)
                        return;
                }
                
                if(point >= clearPoint)    percent = 100;
                else                       percent = (double)point/clearPoint*100;
                
                if(percent>100)
                    percent = 100;
                
                pAchievement.showsCompletionBanner = YES;
                pAchievement.percentComplete = percent;
                
                NSArray *arr = [NSArray arrayWithObjects:pAchievement, nil];
                [GKAchievement reportAchievements:arr withCompletionHandler:^(NSError *error)
                 {
                     if( error != nil ){
                     }
                     else{
//                         NSLog(@"incrementAchievement %@ , %f",strId,percent);
                     }
                     
                 }];
            }
        }
    }];
}


//- (void) leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)viewController
//{
//    //[viewController dismissModalViewControllerAnimated:YES];
//    [viewController dismissViewControllerAnimated:NO  completion:NULL];
//}
//
//- (void) achievementViewControllerDidFinish:(GKAchievementViewController *)viewController
//{
//    //[viewController dismissModalViewControllerAnimated:YES];
//    [viewController dismissViewControllerAnimated:NO  completion:NULL];
//}


- (void)gameCenterViewControllerDidFinish:(GKGameCenterViewController *)gameCenterViewController
{
    [gameCenterViewController dismissViewControllerAnimated:YES completion:nil];
}








@end
