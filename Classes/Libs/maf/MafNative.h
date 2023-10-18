//
//  MafNative.h
//  TapRed
//
//  Created by SanHeo on 2014. 8. 2..
//
//

#ifndef __TapRed__MafNative__
#define __TapRed__MafNative__

#include "cocos2d.h"

class MafNative
{
public:
    
    static MafNative* getInstance();
    
    static void OpenUrl(const char* url);
    static void SendEmail(const char* email);
    static void SendEmail(const char* email, const char* subject, const char* body);
    
    static void SetLocalAlarm();
    
    static std::string GetGameVersion();
    static void ShowUpdateAlert(const char* title, const char* message, const char* confirm, const char* shortcut);
    
 
    
    static bool FindIllegalProgram();
    
    enum E_PERMISSION{
        GET_ACCOUNTS=1,
        WRITE_EXTERNAL_STORAGE,
        READ_PHONE_STATE
    };
    static void CheckPermission(int permission[], int size, const char* msg);
    static bool IsPermission(int permission);
    //- (void) SendEmail:(NSString*)strEmail;
    
    static void GetAdsID();
    static std::string GetDeviceID();
    static std::string getDeviceModel();
    static std::string GetDeviceInstallStore();
    static std::string getDeviceSystemVersion();
    
    static bool IsLessThanCurrVersion(std::string version);
    
    static bool IsDeviceiPhoneX();
    
    static std::string getKeyHash();
    
    static void SendLocalNotification(int notiKey, int timeTerm, const char* notiTitle, const char* notiDesc);
    static void CancelLocalNotification(int notiKey);
    
    static void CreateDeepLink(const char* paramKey, const char* paramValue);
    static void ReceiveDeepLink();
    
    static void copyString(const char* text);
    static void shareString(const char* text);
    
    
    //
    static void CBReceiveAdsId(std::string strAdsID);
    static void CBReceiveDeepLink(std::string strParams);
};

#endif /* defined(__TapRed__MafNative__) */
