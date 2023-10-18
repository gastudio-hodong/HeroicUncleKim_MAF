//
//  AccountManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 19/09/2019.
//

#include "AccountManager.h"

#include "Common/ConfigKey.h"

#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

AccountManager::AccountManager(void) :
_userIdx(0),

_userID(""),
_userSocialID(""),
_userNick(""),

_bUserGuest(false),
_bUserNickFirst(false),

_bLoadData(false),

_nReportCount(0),

//
_callbackSocialLogin(nullptr)
{
    
}

AccountManager::~AccountManager(void)
{
    
}

bool AccountManager::init()
{
    MafGooglePlay::SetupGooglePlus();
    if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    {
        _userID = UserDefault::getInstance()->getStringForKey(KEY_LOGIN_GOOGLEPLAY_ID);
        _userID = MafAes256::Decrypt(_userID);
    }
    else
    {
        _userID = UserDefault::getInstance()->getStringForKey(KEY_LOGIN_GAMECENTER_ID);
        _userID = MafAes256::Decrypt(_userID);
    }
    
    auto userGueestDecrypt = UserDefault::getInstance()->getStringForKey(KEY_LOGIN_GUEST);
    userGueestDecrypt = MafAes256::Decrypt(userGueestDecrypt);
    _bUserGuest = atol(userGueestDecrypt.c_str());
    
    return true;
}

#pragma mark - set, get, add
int AccountManager::getUserIdx()
{
    return _userIdx;
}

std::string AccountManager::getUserID()
{
    return _userID;
}

std::string AccountManager::getUserSocialID()
{
    return _userSocialID;
}

std::string AccountManager::getUserPlatform()
{
    std::string platform = CC_TARGET_PLATFORM == CC_PLATFORM_IOS ? "ios" : "aos";
    
    return platform;
}

std::string AccountManager::getUserNick()
{
    return _userNick;
}


int AccountManager::getReportCount()
{
    return _nReportCount;
}

int AccountManager::getReportCountMax()
{
    return 5;
}

bool AccountManager::isUserNickFirst()
{
    return _bUserNickFirst;
}

bool AccountManager::isGuest()
{
    return _bUserGuest;
}

bool AccountManager::isLoadData()
{
    return _bLoadData;
}

#pragma mark - callback
void AccountManager::callbackSocialLogin(bool bResult, std::string socialID)
{
    PopupLoading::hide();
    
    if ( bResult == false || socialID.empty() == true )
    {
        if ( _callbackSocialLogin != nullptr )
        {
            _callbackSocialLogin();
            _callbackSocialLogin = nullptr;
        }
        return;
    }
    
    //
    _userSocialID = socialID;
    
    //
    if ( _callbackSocialLogin != nullptr )
    {
        _callbackSocialLogin();
        _callbackSocialLogin = nullptr;
    }
}

#pragma mark - event : social login
void AccountManager::onSocialLogin()
{
    MafGooglePlay::LoginGameCenter();
}

void AccountManager::onSocialLoginAgain(const std::function<void(void)>& callback)
{
    _callbackSocialLogin = callback;
    
    //
    PopupLoading::show();
    MafGooglePlay::ReLoginGameCenter();
}

#pragma mark - event : login
void AccountManager::onLogin(bool bPopupLoading, const std::function<void(bool, int)>& callback)
{
    if ( _userID.empty() == true )
    {
        _userID = _userSocialID;
        
        if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        {
            UserDefault::getInstance()->setStringForKey(KEY_LOGIN_GOOGLEPLAY_ID, MafAes256::Encrypt(_userID));
            UserDefault::getInstance()->setStringForKey(KEY_LOGIN_GAMECENTER_ID, "");
        }
        else
        {
            UserDefault::getInstance()->setStringForKey(KEY_LOGIN_GOOGLEPLAY_ID, "");
            UserDefault::getInstance()->setStringForKey(KEY_LOGIN_GAMECENTER_ID, MafAes256::Encrypt(_userID));
        }
    }
    
    requestInfo(bPopupLoading, callback);
}


#pragma mark - network
void AccountManager::requestGuestRegister(bool bPopupLoading, const std::function<void(bool)>& callback/* = nullptr*/)
{
    //
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/user/guestlogin";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseGuestRegister(bPopupLoading, callback, response,dd);
    };
    req->send(cb);
}

void AccountManager::responseGuestRegister(bool bPopupLoading, const std::function<void(bool)>& callback, cocos2d::network::HttpResponse* response,std::string &data)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::hide();
    }
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        if ( callback != nullptr )
        {
            callback(false);
        }
        return;
    }
    
    //
    auto nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        _bUserGuest = true;
        UserDefault::getInstance()->setStringForKey(KEY_LOGIN_GUEST, MafAes256::Encrypt(std::to_string(_bUserGuest)));
        
        //
        _userID = jsonParser["_userid"].GetString();
        if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        {
            UserDefault::getInstance()->setStringForKey(KEY_LOGIN_GOOGLEPLAY_ID, MafAes256::Encrypt(_userID));
            UserDefault::getInstance()->setStringForKey(KEY_LOGIN_GAMECENTER_ID, "");
        }
        else
        {
            UserDefault::getInstance()->setStringForKey(KEY_LOGIN_GOOGLEPLAY_ID, "");
            UserDefault::getInstance()->setStringForKey(KEY_LOGIN_GAMECENTER_ID, MafAes256::Encrypt(_userID));
        }
        
        // callback
        if ( callback != nullptr )
        {
            callback(true);
        }
    }
    else
    {
        // callback
        if ( callback != nullptr )
        {
            callback(false);
        }
    }
}

void AccountManager::requestGuestCheck(bool bPopupLoading, const std::function<void(bool)>& callback/* = nullptr*/)
{
    //
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/social/info";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_userid", getUserSocialID());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseGuestCheck(bPopupLoading, callback, response, dd);
    };
    req->send(cb);
}

void AccountManager::responseGuestCheck(bool bPopupLoading, const std::function<void(bool)>& callback, cocos2d::network::HttpResponse* response,std::string &data)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::hide();
    }
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        if ( callback != nullptr )
        {
            callback(false);
        }
        return;
    }
    
    //
    auto nResult = jsonParser["_result"].GetInt();
    if ( nResult != 1 )
    {
        // callback
        if ( callback != nullptr )
        {
            callback(true);
        }
    }
    else
    {
        // callback
        if ( callback != nullptr )
        {
            callback(false);
        }
    }
}

void AccountManager::requestGuestLink(bool bPopupLoading, const std::function<void(bool)>& callback/* = nullptr*/)
{
    //
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/social/change";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_userid", getUserID());
    req->setData("_new_userid", getUserSocialID());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseGuestLink(bPopupLoading, callback, response, dd);
    };
    req->send(cb);
}

void AccountManager::responseGuestLink(bool bPopupLoading, const std::function<void(bool)>& callback, cocos2d::network::HttpResponse* response,std::string &data)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::hide();
    }
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        if ( callback != nullptr )
        {
            callback(false);
        }
        return;
    }
    
    //
    auto nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        _bUserGuest = false;
        UserDefault::getInstance()->setStringForKey(KEY_LOGIN_GUEST, MafAes256::Encrypt(std::to_string(_bUserGuest)));
        
        //
        _userID = _userSocialID;
        if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        {
            UserDefault::getInstance()->setStringForKey(KEY_LOGIN_GOOGLEPLAY_ID, MafAes256::Encrypt(_userID));
            UserDefault::getInstance()->setStringForKey(KEY_LOGIN_GAMECENTER_ID, "");
        }
        else
        {
            UserDefault::getInstance()->setStringForKey(KEY_LOGIN_GOOGLEPLAY_ID, "");
            UserDefault::getInstance()->setStringForKey(KEY_LOGIN_GAMECENTER_ID, MafAes256::Encrypt(_userID));
        }
        
        // callback
        if ( callback != nullptr )
        {
            callback(true);
        }
    }
    else
    {
        // callback
        if ( callback != nullptr )
        {
            callback(false);
        }
    }
}

void AccountManager::requestInfo(bool bPopupLoading, const std::function<void(bool, int)>& callback/* = nullptr*/)
{
    //
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/v3/user/info";
    std::string token = UserDefault::getInstance()->getStringForKey("token");
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_userid", getUserID());
    req->setData("_platform", getUserPlatform());
    if ( token.length() != 0 )
    {
        req->setData("_push_key", token.c_str());
    }
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(bPopupLoading, callback, response,dd);
    };
    req->send(cb);
}

void AccountManager::responseInfo(bool bPopupLoading, const std::function<void(bool, int)>& callback, network::HttpResponse* response, std::string &data)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::hide();
    }
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        // callback
        if ( callback != nullptr )
        {
            callback(false, -1);
        }
        return;
    }
    
    //
    int result = jsonParser["_result"].GetInt();
    if ( result == 1 )
    {
        _userIdx = jsonParser["_useridx"].GetInt();
        
        _bLoadData = jsonParser["_isSaveData"].GetBool();
        
        if ( jsonParser.HasMember("_nick") )
        {
            _userNick = jsonParser["_nick"].GetString();
        }
        
        if ( jsonParser.HasMember("_is_first") )
        {
            _bUserNickFirst = jsonParser["_is_first"].GetBool();
        }
        
        // callback
        if ( callback != nullptr )
        {
            callback(true, result);
        }
    }
    else
    {
        // callback
        if ( callback != nullptr )
        {
            callback(false, result);
        }
    }
}

void AccountManager::requestBan(bool bPopupLoading, const std::function<void(bool, std::string, E_BAN_TYPE)>& callback/* = nullptr*/)
{
    //
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/ban/%s";
    url = MafUtils::format(url.c_str(), _userID.c_str());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseBan(bPopupLoading, callback, response,dd);
    };
    req->send(cb);
}

void AccountManager::responseBan(bool bPopupLoading, const std::function<void(bool, std::string, E_BAN_TYPE)>& callback, network::HttpResponse* response,std::string &data)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::hide();
    }
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == false && jsonParser.HasMember("_ban") == false )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        if ( callback != nullptr )
        {
            callback(false, "0", E_BAN_TYPE::BAN_NONE);
        }
        return;
    }
    
    //
    auto bBan = jsonParser["_ban"].GetBool();
    auto strDay = jsonParser["_end_data"].GetString();
    auto eType = (E_BAN_TYPE)jsonParser["_ban_type"].GetInt();
    
    // callback
    if ( callback != nullptr )
    {
        callback(bBan, strDay, eType);
    }
}

void AccountManager::requestReportCount(bool bPopupLoading, const std::function<void(bool)>& callback/* = nullptr*/)
{
    //
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/user/count/info/%d";
    url = MafUtils::format(url.c_str(), getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseReportCount(bPopupLoading, callback, response,dd);
    };
    req->send(cb);
}

void AccountManager::responseReportCount(bool bPopupLoading, const std::function<void(bool)>& callback, network::HttpResponse* response,std::string &data)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::hide();
    }
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        // callback
        if ( callback != nullptr )
        {
            callback(false);
        }
        return;
    }
    
    //
    _nReportCount = jsonParser["_report_count"].GetInt();
    
    
    // callback
    if ( callback != nullptr )
    {
        callback(true);
    }
}

void AccountManager::requestReport(bool bPopupLoading, E_REPORT_TYPE eType, std::string strMessage, int nToUseridx, std::string strToPlatform, const std::function<void(bool, int)>& callback/* = nullptr*/)
{
    //
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/user/report";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", getUserIdx());
    req->setData("_type", (int)eType);
    req->setData("_message", strMessage.c_str());
    req->setData("_t_useridx", nToUseridx);
    req->setData("_t_platform", strToPlatform.c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseReport(bPopupLoading, callback, response, dd);
    };
    req->send(cb);
}

void AccountManager::responseReport(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::hide();
    }
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        // callback
        if ( callback != nullptr )
        {
            callback(false, -1);
        }
        return;
    }
    
    //
    int nResult = jsonParser["_result"].GetInt();
    _nReportCount = jsonParser["_report_count"].GetInt();
    
    if ( nResult == 1 )
    {
        if ( callback != nullptr )
        {
            callback(true, nResult);
        }
    }
    else
    {
        if ( callback != nullptr )
        {
            callback(false, nResult);
        }
    }
}

void AccountManager::requestNickChange(bool bPopupLoading, std::string strNick, const std::function<void(bool, int)>& callback/* = nullptr*/)
{
    //
    if ( bPopupLoading == true )
    {
        PopupLoading::show();
    }
    
    //
    std::string url = "/v3/change/name";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", getUserIdx());
    req->setData("_name", strNick.c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseNickChnage(bPopupLoading, callback, response,dd);
    };
    req->send(cb);
}

void AccountManager::responseNickChnage(bool bPopupLoading, const std::function<void(bool, int)>& callback, cocos2d::network::HttpResponse* response,std::string &data)
{
    if ( bPopupLoading == true )
    {
        PopupLoading::hide();
    }
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        if ( callback != nullptr )
        {
            callback(false, -1);
        }
        return;
    }
    
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        _userNick = jsonParser["_nick"].GetString();
        _bUserNickFirst = false;
        
        if ( callback != nullptr )
        {
            callback(true, nResult);
        }
    }
    else
    {
        if ( callback != nullptr )
        {
            callback(false, nResult);
        }
    }
}
