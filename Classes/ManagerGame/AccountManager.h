//
//  AccountManager.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#ifndef AccountManager_h
#define AccountManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/ConfigGameNormal.h"

class AccountManager : public MafSingleton<AccountManager>
{
public:
    AccountManager(void);
    virtual ~AccountManager(void);
    virtual bool init();
   
    
public:
    // set, get
    int getUserIdx();
    
    std::string getUserID();
    std::string getUserSocialID();
    std::string getUserPlatform();
    std::string getUserNick();
    
    bool isUserNickFirst();
    bool isGuest();
    bool isLoadData();
    
    int getReportCount();
    int getReportCountMax();
    
    
    
    // callback
    void callbackSocialLogin(bool bResult, std::string socialID);
    
    // event : social login
    void onSocialLogin();
    void onSocialLoginAgain(const std::function<void(void)>& callback);
    
    // event : login
    void onLogin(bool bPopupLoading, const std::function<void(bool, int)>& callback);
    
    
    
    // network
    void requestGuestRegister(bool bPopupLoading, const std::function<void(bool)>& callback = nullptr);
    void requestGuestCheck(bool bPopupLoading, const std::function<void(bool)>& callback = nullptr);
    void requestGuestLink(bool bPopupLoading, const std::function<void(bool)>& callback = nullptr);
    
    void requestInfo(bool bPopupLoading, const std::function<void(bool, int)>& callback = nullptr);
    void requestBan(bool bPopupLoading, const std::function<void(bool, std::string, E_BAN_TYPE)>& callback = nullptr);
    void requestReportCount(bool bPopupLoading, const std::function<void(bool)>& callback = nullptr);
    void requestReport(bool bPopupLoading, E_REPORT_TYPE eType, std::string strMessage, int nToUseridx, std::string strToPlatform, const std::function<void(bool, int)>& callback = nullptr);
    void requestNickChange(bool bPopupLoading, std::string strNick, const std::function<void(bool, int)>& callback = nullptr);
    
    
protected:
    // network
    void responseGuestRegister(bool bPopupLoading, const std::function<void(bool)>& callback, cocos2d::network::HttpResponse* response,std::string &data);
    void responseGuestCheck(bool bPopupLoading, const std::function<void(bool)>& callback, cocos2d::network::HttpResponse* response,std::string &data);
    void responseGuestLink(bool bPopupLoading, const std::function<void(bool)>& callback, cocos2d::network::HttpResponse* response,std::string &data);
    
    void responseInfo(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data);
    void responseBan(bool bPopupLoading, const std::function<void(bool, std::string, E_BAN_TYPE)>& callback, cocos2d::network::HttpResponse* response,std::string &data);
    void responseReportCount(bool bPopupLoading, const std::function<void(bool)>& callback, cocos2d::network::HttpResponse* response,std::string &data);
    void responseReport(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data);
    void responseNickChnage(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data);
    
private:
    int _userIdx;
    
    std::string _userID;
    std::string _userSocialID;
    std::string _userNick;
    
    bool _bUserGuest;
    bool _bUserNickFirst;
    
    bool _bLoadData;    // 데이터 불러와야 할 경우
    
    int _nReportCount; // 신고 횟수
    
   
    //
    std::function<void(void)> _callbackSocialLogin;
};

#endif /* AccountManager_h */
