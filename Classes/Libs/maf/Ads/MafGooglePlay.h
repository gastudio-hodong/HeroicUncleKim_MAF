//
//  MafGooglePlay.h
//  TapRed
//
//  Created by SanHeo on 2014. 8. 1..
//
//

#ifndef __TapRed__MafGooglePlay__
#define __TapRed__MafGooglePlay__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

#include "network/HttpClient.h"
#include "network/HttpRequest.h"

class MafGooglePlay
{
public:
    
    enum RESULT
    {
        FAIL=-1,
        CANCEL=0,
        SUCCESS=1,        
        //        LACK
    };
    MafGooglePlay();
    static MafGooglePlay* getInstance();
    
    static void SetupGooglePlus();
    static void LoginGameCenter();
    static void ReLoginGameCenter();
    static void CBReceiveLogin(bool result, std::string strID);
    static bool IsSignedIn();
    
    static void ShowLeaderBoardAll();
    static void ShowLeaderBoard(const char* lb_id);
    static void UpdateLeaderBoardScore(const char* lb_id,int score);
    
    static void ShowAchievements();
    static void UnlockAchievement(const char* ach_id,int point,int clearPoint);
    static void UnlockProgressAchievement(const char* ach_id,int point,int clearPoint);
    static void IncrementAchievement(const char* ach_id,int point,int clearPoint);
    static void IncrementImmediateAchievement(const char* ach_id,int point,int clearPoint);
    
    //
    static void PurchaseRestore();
    static void Purchase(const char* strProductIdReal,const char* strProductIdWatch,const char* strPrice, const char* strPriceCurrencyCode, std::function<void(RESULT, std::string, std::string)> cb);
    static void PurchaseLoad(std::vector<std::string> listProductId);
    static void PurchaseHas(const char* strProductIdReal,const char* strProductIdWatch, std::function<void(int)> cb);
    static void PurchaseConsume(const char* strProductIdReal,const char* strProductIdWatch);
    static void PurchaseConsumeAll();
    
    
    std::function<void(RESULT, std::string, std::string)> _callbackPurchase;
    std::function<void(int)> _callbackPurchaseHas;
    static void CBReceivePurchase(RESULT result, std::string data, std::string signature);
    static void CBReceivePurchaseLoad(std::string strProductID, std::string strProductPrice, std::string strPriceCurrencyCode);
    static void CBReceivePurchaseHas(int result);
    
    std::function<void(RESULT)> _ResultReviewCB;
    static void InAppReview(std::function<void(RESULT)> cb);
    static void CBReceiveInAppReview(RESULT result);

    
    std::string _strResponseData;
    std::string _strSignature;
};


#endif /* defined(__TapRed__MafGooglePlay__) */



