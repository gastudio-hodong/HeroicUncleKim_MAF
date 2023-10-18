//
//  FriendManager.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 13/02/2020.
//

#include "FriendManager.h"

#include "GameScene/HelloWorldScene.h"
#include "GameScene/DevilDomScene.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ServerMissionManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

FriendManager::FriendManager(void) :
_nFriendAmount(0),
_nFriendIsRecommend(1),

_nUserIdxFriend(0),
_nUserIdxFriendInvite(0),
_bBootLinkAdd(false),

_nMaxRecievePoint(0),
_nMaxSendPoint(0),
_nRecievePoint(0),
_nSendPoint(0),

_nFriendDeleteMax(0),
_nFriendDeleteNow(0),

_nPet(E_PET::PET_NONE),
_tempPet(E_PET::PET_NONE),

_callbackFriendInfo(nullptr),
_callbackFriendRecommendInfo(nullptr),
_callbackFriendWaitInfo(nullptr),
_callbackSetPet(nullptr),
_callbackFriendAddRequest(nullptr),
_callbackFriendApply(nullptr),
_callbackFriendDel(nullptr),
_callbackPoint(nullptr)
{
    _inviteFriendOS = "";
    _inviteFriendUserid = "";
}

FriendManager::~FriendManager(void)
{
    _vecFriendList.clear();
}

bool FriendManager::init()
{
    return true;
}

#pragma mark - add, get
Vector<InfoFriend*> FriendManager::getFriendList()
{
    return _vecFriendList;
}

Vector<InfoFriend*> FriendManager::getFriendRecommendList()
{
    return _vecFriendRecommendList;
}

Vector<InfoFriend*> FriendManager::getFriendWaitList()
{
    return _vecFriendWaitList;
}

Vector<InfoFriend*> FriendManager::getFriendSearchList()
{
    return _vecFriendSearchList;
}

bool FriendManager::IsFriend(int userIdx, std::string userPlatform)
{
    bool bResult = false;
    
    for (auto info : _vecFriendList)
    {
        if ( info->getUseridx() == userIdx && info->getPlatform().compare(userPlatform) == 0 )
        {
            bResult = true;
            break;
        }
    }
    
    return bResult;
}

void FriendManager::setFriendListAmount(int nAmount)
{
    _nFriendAmount = nAmount;
}

int FriendManager::getFriendListAmount()
{
    int nSize = _nFriendAmount;
    if ( nSize < 0 )
        nSize = 0;
    
    return nSize;
}

int FriendManager::getFriendIsRecommend()
{
    return _nFriendIsRecommend;
}

float FriendManager::getFriendBonusSpeed()
{
    float speed = (float)getFriendListAmount()/2;
    
    if (speed > 10)
    {
        speed = 10;
    }
    return speed;
}

void FriendManager::setInviteFriendId(std::string userid, std::string os)
{
    if ( os == "android" )
    {
        os = "aos";
    }
    
    if(_inviteFriendUserid == userid && _inviteFriendOS == os)
    {
        return;
    }
    
    _inviteFriendUserid = userid;
    _inviteFriendOS = os;
    
    auto sceneGameLayer = UtilsScene::getInstance()->getRunningLayer();
    if ( dynamic_cast<HelloWorldScene *>(sceneGameLayer) == nullptr && dynamic_cast<DevilDomScene *>(sceneGameLayer) == nullptr)
    {
        _bBootLinkAdd = true;
    }
    else
    {
        requestFriendIdx();
    }
}

E_PET FriendManager::getMyPet()
{
    return _nPet;
}

int FriendManager::getPointSendMax()
{
    return _nMaxSendPoint;
}

int FriendManager::getPointSend()
{
    return _nSendPoint;
}

int FriendManager::getFriendDeleteMax()
{
    return _nFriendDeleteMax;
}

int FriendManager::getFriendDeleteNow()
{
    return _nFriendDeleteNow;
}

#pragma mark - sort
void FriendManager::sortFriendList()
{
    Vector<InfoFriend*> newlist;
    int count = 0;
    for (auto info : _vecFriendList)
    {
        if(info->getDibson() == true)
        {
            newlist.pushBack(info);
            count++;
        }
    }
    
    for (auto info : _vecFriendList)
    {
        if(info->getDibson() == false)
        {
            newlist.pushBack(info);
        }
    }
    _vecFriendList = newlist;
}

#pragma mark - network
void FriendManager::requestFriendInfo(const std::function<void(bool)>& pCallback)
{
    //
    _callbackFriendInfo = pCallback;
    
    //
    std::string url = "/user/friend/list/";
    url.append( MafUtils::toString(AccountManager::getInstance()->getUserIdx()) );
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());

    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseFriendInfo(response,dd);
    };
    req->send(cb);
}

void FriendManager::responseFriendInfo(cocos2d::network::HttpResponse* response,std::string &data)
{
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
        if ( _callbackFriendInfo != nullptr )
        {
            _callbackFriendInfo(false);
            _callbackFriendInfo = nullptr;
        }
        return;
    }
    
    _vecFriendList.clear();
    
    //
    _nMaxRecievePoint = jsonParser["max_receive_point"].GetInt();
    _nMaxSendPoint = jsonParser["max_send_point"].GetInt();
        
    _nRecievePoint = jsonParser["receive_point"].GetInt();
    _nSendPoint = jsonParser["send_point"].GetInt();
        
    int nFriendPoint = jsonParser["total_point"].GetInt();
    ItemsManager::getInstance()->setItemsServer(E_ITEMS::FRIEND_POINT, 0, nFriendPoint);
    
    _nPet = ((E_PET)(jsonParser["pet"].GetInt()));
    
    _nFriendIsRecommend = jsonParser["is_recommend"].GetInt();
    
    _nFriendDeleteMax = jsonParser["max_delete_friend"].GetInt();
    _nFriendDeleteNow = jsonParser["delete_friend"].GetInt();

    //
    auto friendlist = jsonParser["friends_list"].GetArray();
    for ( int i = 0; i < friendlist.Size(); i++ )
    {
        const rapidjson::Value& jsonProduct = friendlist[i];
        
        InfoFriend* info = InfoFriend::create();
        info->setUseridx(jsonProduct["useridx"].GetInt());
        info->setPlatform(jsonProduct["platform"].GetString());
        info->setNickname(jsonProduct["nick"].GetString());
        info->setPointReceiveAvailable(jsonProduct["receive"].GetBool());
        info->setPointSendAvailable(jsonProduct["send"].GetBool());
        info->setUserPet(((E_PET)(jsonProduct["pet"].GetInt())));
        info->setUserPetEvolution(jsonProduct["f_isAwakening"].GetInt());
        info->setLoginTime(jsonProduct["latest_login_date"].GetInt64());

        std::string strKey = MafUtils::format("dibs_friend_%d", info->getUseridx());
        bool bDibs = UserDefault::getInstance()->getBoolForKey(strKey.c_str(), false);
        if ( bDibs == true )
        {
            info->setDibson(true);
        }
        
        std::vector<int> list;
        list.push_back(atoi(jsonProduct["weapons"].GetString()));
        auto array = jsonProduct["costume"].GetString();
        std::vector<std::string> strlist = MafUtils::split(array, ',');
            
        for ( int i = 0; i < strlist.size(); i++ )
        {
            list.push_back(atoi(strlist.at(i).c_str()));
        }
        info->setCostume(list);
        
        _vecFriendList.pushBack(info);
    }
    
    //
    _nFriendAmount = (int)_vecFriendList.size();
    
    //
    sortFriendList();
    
    //
    if ( _callbackFriendInfo != nullptr )
    {
        _callbackFriendInfo(true);
        _callbackFriendInfo = nullptr;
    }
}

void FriendManager::requestRecommendInfo(const std::function<void(bool)>& pCallback)
{
    //
    _callbackFriendRecommendInfo = pCallback;
    
    //
    std::string url = "/user/friend/recommendlist/";
    url.append( MafUtils::toString(AccountManager::getInstance()->getUserIdx()) );
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseRecommendInfo(response,dd);
    };
    req->send(cb);
}

void FriendManager::responseRecommendInfo(cocos2d::network::HttpResponse* response,std::string &data)
{
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
        if ( _callbackFriendRecommendInfo != nullptr )
        {
            _callbackFriendRecommendInfo(false);
            _callbackFriendRecommendInfo = nullptr;
        }
        return;
    }
    
    //
    _vecFriendRecommendList.clear();
    
    //
    auto friendinvite = jsonParser.GetArray();
    for ( int i = 0; i < friendinvite.Size(); i++ )
    {
        const rapidjson::Value& jsonProduct = friendinvite[i];
        
        InfoFriend* info = InfoFriend::create();
        info->setUseridx(jsonProduct["useridx"].GetInt());
        info->setPlatform(jsonProduct["platform"].GetString());
        info->setNickname(jsonProduct["nick"].GetString());
        info->setUserPet(((E_PET)(jsonProduct["pet"].GetInt())));
        info->setUserPetEvolution(jsonProduct["f_isAwakening"].GetInt());
        
        std::vector<int> list;
        list.push_back(atoi(jsonProduct["weapons"].GetString()));
        auto array = jsonProduct["costume"].GetString();
        std::vector<std::string> strlist = MafUtils::split(array, ',');
        
        for ( int i = 0; i < strlist.size(); i++ )
        {
            list.push_back(atoi(strlist.at(i).c_str()));
        }
                
        info->setCostume(list);
        
        _vecFriendRecommendList.pushBack(info);
    }
    
    //
    if( _callbackFriendRecommendInfo != nullptr)
    {
        _callbackFriendRecommendInfo(true);
        _callbackFriendRecommendInfo = nullptr;
    }
}

void FriendManager::requestWaitInfo(const std::function<void(bool)>& pCallback)
{
    //
    _callbackFriendWaitInfo = pCallback;
    
    //
    std::string url = "/user/friend/waitList/";
    url.append( MafUtils::toString(AccountManager::getInstance()->getUserIdx()) );
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseWaitInfo(response,dd);
    };
    req->send(cb);
}
void FriendManager::responseWaitInfo(cocos2d::network::HttpResponse* response,std::string &data)
{
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
        if ( _callbackFriendWaitInfo != nullptr )
        {
            _callbackFriendWaitInfo(false);
            _callbackFriendWaitInfo = nullptr;
        }
        return;
    }
    
    //
    _vecFriendWaitList.clear();
    
    //
    auto friendlist = jsonParser["friends_list"].GetArray();
    for ( int i = 0; i < friendlist.Size(); i++ )
    {
        const rapidjson::Value& jsonProduct = friendlist[i];
        
        InfoFriend* info = InfoFriend::create();
        info->setUseridx(jsonProduct["useridx"].GetInt());
        info->setPlatform(jsonProduct["platform"].GetString());
        info->setNickname(jsonProduct["nick"].GetString());
        info->setUserPet(((E_PET)(jsonProduct["pet"].GetInt())));
        info->setUserPetEvolution(jsonProduct["f_isAwakening"].GetInt());
        info->setLoginTime(jsonProduct["latest_login_date"].GetInt64());
        
        std::vector<int> list;
        list.push_back(atoi(jsonProduct["weapons"].GetString()));
        auto array = jsonProduct["costume"].GetString();
        std::vector<std::string> strlist = MafUtils::split(array, ',');
        
        for ( int i = 0; i < strlist.size(); i++ )
        {
            list.push_back(atoi(strlist.at(i).c_str()));
        }
                
        info->setCostume(list);
        
        _vecFriendWaitList.pushBack(info);
    }
    
    //
    if( _callbackFriendWaitInfo != nullptr)
    {
        _callbackFriendWaitInfo(true);
        _callbackFriendWaitInfo = nullptr;
    }
}

void FriendManager::requestSearchInfo(const std::function<void(bool)>& pCallback, std::string strNick)
{
    //
    _callbackFriendSearchInfo = pCallback;
    
    //
    std::string url = "/user/friend/searchName";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_nick", strNick.c_str());
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseSearchInfo(response,dd);
    };
    req->send(cb);
}

void FriendManager::responseSearchInfo(cocos2d::network::HttpResponse* response,std::string &data)
{
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
        if ( _callbackFriendSearchInfo != nullptr )
        {
            _callbackFriendSearchInfo(false);
            _callbackFriendSearchInfo = nullptr;
        }
        return;
    }
    
    //
    _vecFriendSearchList.clear();
    
    //
    auto friendinvite = jsonParser.GetArray();
    for ( int i = 0; i < friendinvite.Size(); i++ )
    {
        const rapidjson::Value& jsonProduct = friendinvite[i];
        
        InfoFriend* info = InfoFriend::create();
        info->setUseridx(jsonProduct["useridx"].GetInt());
        info->setPlatform(jsonProduct["platform"].GetString());
        info->setNickname(jsonProduct["nick"].GetString());
        info->setUserPet(((E_PET)(jsonProduct["pet"].GetInt())));
        info->setUserPetEvolution(jsonProduct["f_isAwakening"].GetInt());
        
        std::vector<int> list;
        list.push_back(atoi(jsonProduct["weapons"].GetString()));
        auto array = jsonProduct["costume"].GetString();
        std::vector<std::string> strlist = MafUtils::split(array, ',');
        
        for ( int i = 0; i < strlist.size(); i++ )
        {
            list.push_back(atoi(strlist.at(i).c_str()));
        }
                
        info->setCostume(list);
        
        _vecFriendSearchList.pushBack(info);
    }
    
    //
    if( _callbackFriendSearchInfo != nullptr)
    {
        _callbackFriendSearchInfo(true);
        _callbackFriendSearchInfo = nullptr;
    }
}

void FriendManager::requestFriendAddRequest(const std::function<void(bool, int)>& pCallback, int nUserIdx, std::string strPlatform, int nType)
{
    //
    _callbackFriendAddRequest = pCallback;
    _nUserIdxFriend = nUserIdx;
    
    //
    std::string url = "/v1/user/friend/request";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_friend", _nUserIdxFriend);
    req->setData("_friend_platform", strPlatform.c_str());
    req->setData("_type", nType);
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseFriendAddRequest(response,dd);
    };
    req->send(cb);
}

void FriendManager::responseFriendAddRequest(cocos2d::network::HttpResponse* response,std::string &data)
{
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
        if ( _callbackFriendAddRequest != nullptr )
        {
            _callbackFriendAddRequest(false, -1);
            _callbackFriendAddRequest = nullptr;
        }
        return;
    }
    
    /*
    0 # 해당 친구가 친구 많음
    1 # 친구 추가 성공
    3 # 이미 친구요청을 보냄
    4 # 친구의 친구대기리스트 꽉참
    5 # 이미 친구 임
    6 # 자신은 친구가 될수없음
    */
    auto nResult = jsonParser["result"].GetInt();
    
    // 추천 리스트에서 삭제
    for ( auto invite : _vecFriendRecommendList )
    {
        if ( invite->getUseridx() == _nUserIdxFriend )
        {
            _vecFriendRecommendList.eraseObject(invite);
            break;
        }
    }
    for ( auto invite : _vecFriendSearchList )
    {
        if ( invite->getUseridx() == _nUserIdxFriend )
        {
            _vecFriendSearchList.eraseObject(invite);
            break;
        }
    }
    
    _nUserIdxFriend = 0;
    
    // callback
    if ( _callbackFriendAddRequest != nullptr )
    {
       _callbackFriendAddRequest(true, nResult);
       _callbackFriendAddRequest = nullptr;
    }
}

void FriendManager::requestFriendApply(const std::function<void(bool, int)>& pCallback, int nUserIdx, std::string strPlatform, int nType)
{
    //
    _callbackFriendApply = pCallback;
    _nUserIdxFriend = nUserIdx;
    
    //
    std::string url = "/v1/user/friend/apply";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_friend", _nUserIdxFriend);
    req->setData("_friend_platform", strPlatform.c_str());
    req->setData("_accept", nType);
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseFriendApply(response,dd);
    };
    req->send(cb);
}

void FriendManager::responseFriendApply(cocos2d::network::HttpResponse* response,std::string &data)
{
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
        if ( _callbackFriendApply != nullptr )
        {
            _callbackFriendApply(false, -1);
            _callbackFriendApply = nullptr;
        }
        return;
    }
    
    /*
    0 # 해당 친구가 친구 많음
    1 # 친구 추가 성공
    3 # 내 친구 꽉참
    5 # 이미 친구 임 ( 요청 삭제 불가
    6 # 자신은 친구가 될수없음
    */
    auto nResult = jsonParser["result"].GetInt();
    if ( nResult == 1 )
    {
        // 버프 효과를 위해 가상 오브젝트 추가
        _nFriendAmount++;
    }
    
    //
    if ( nResult == 1 || nResult == 7 )
    {
        //1 # 친구 추가 성공
        for ( auto invite : _vecFriendWaitList )
        {
            if ( invite->getUseridx() == _nUserIdxFriend )
            {
                _vecFriendWaitList.eraseObject(invite);
                break;
            }
        }
    }
    _nUserIdxFriend = 0;
    
    
    // callback
    if ( _callbackFriendApply != nullptr )
    {
       _callbackFriendApply(true, nResult);
       _callbackFriendApply = nullptr;
    }
}

void FriendManager::requestFriendDelete(const std::function<void(bool, int)>& pCallback, int nUserIdx, std::string strPlatform)
{
    //
    _callbackFriendDel = pCallback;
    _nUserIdxFriend = nUserIdx;
    
    //
    std::string url = "/user/friend/delete";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_friend", _nUserIdxFriend);
    req->setData("_friend_platform", strPlatform.c_str());
    
        MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseFriendDelete(response,dd);
    };
    req->send(cb);
}

void FriendManager::responseFriendDelete(cocos2d::network::HttpResponse* response,std::string &data)
{
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
        if ( _callbackFriendDel != nullptr )
        {
            _callbackFriendDel(false, -1);
            _callbackFriendDel = nullptr;
        }
        return;
    }
    
    int nResult = jsonParser["result"].GetInt();
    if ( nResult == 1 )
    {
        _nFriendAmount--;
        
        std::string strKey = MafUtils::format("dibs_friend_%d", _nUserIdxFriend);
        UserDefault::getInstance()->deleteValueForKey(strKey.c_str());
    }
    _nUserIdxFriend = 0;
    
    //
    if ( _callbackFriendDel != nullptr )
    {
        _callbackFriendDel(true, nResult);
        _callbackFriendDel = nullptr;
    }
    
}
void FriendManager::requestFriendIdx()
{
    if ( _inviteFriendUserid == "" || _inviteFriendOS == "" )
    {
        return;
    }
    
    std::string url = "/v3/user/info";
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_userid", _inviteFriendUserid.c_str());
    req->setData("_platform",_inviteFriendOS.c_str());
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseFriendIdx(response,dd);
    };
    req->send(cb);
}

void FriendManager::responseFriendIdx(cocos2d::network::HttpResponse *response, std::string &data)
{
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
    
    if ( bError == false && jsonParser.HasMember("_useridx") == false )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        return;
    }
    
    //
    int result = jsonParser["_result"].GetInt();
    if ( result== 1 )
    {
        int nUserIdx = jsonParser["_useridx"].GetInt();
        requestFriendAddRequest(nullptr, nUserIdx, _inviteFriendOS, 1);
    }
    
    //
    _inviteFriendOS = "";
    _inviteFriendUserid = "";
    _bBootLinkAdd = false;
}

void FriendManager::requestIsRecommend(const std::function<void(bool)>& pCallback, int nType)
{
    //
    _callbackIsRecommend = pCallback;
    
    //
    std::string url = "/user/friend/recommend/update";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_flag", nType);
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseIsRecommend(response,dd);
    };
    req->send(cb);
}

void FriendManager::responseIsRecommend(cocos2d::network::HttpResponse* response,std::string &data)
{
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
        if ( _callbackIsRecommend != nullptr )
        {
            _callbackIsRecommend(false);
            _callbackIsRecommend = nullptr;
        }
        return;
    }

    auto nResult = jsonParser["result"].GetInt();
    if ( nResult == 1 )
    {
        _nFriendIsRecommend = jsonParser["flag"].GetInt();
        
        if ( _callbackIsRecommend != nullptr )
        {
            _callbackIsRecommend(true);
            _callbackIsRecommend = nullptr;
        }
    }
    else
    {
        if ( _callbackIsRecommend != nullptr )
        {
            _callbackIsRecommend(false);
            _callbackIsRecommend = nullptr;
        }
    }
}

void FriendManager::requestChangeMyPet(const std::function<void(bool)>& pCallback, E_PET pet)
{
    if ( pet == _nPet )
    {
        pet = E_PET::PET_NONE;
    }
    _tempPet = pet;
    _callbackSetPet = pCallback;
    
    //
    std::string url = "/user/friend/pet";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_pet", pet);
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data)
    {
        std::string dd = data;
        responseChangeMyPet(response,dd);
    };
    req->send(cb);
    
}
void FriendManager::responseChangeMyPet(cocos2d::network::HttpResponse *response, std::string &data)
{
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
        if ( _callbackSetPet != nullptr )
        {
            _callbackSetPet(false);
            _callbackSetPet = nullptr;
        }
        return;
    }

    auto nResult = jsonParser["result"].GetInt();
    if ( nResult == 1 )
    {
        _nPet = _tempPet;
        if( _callbackSetPet != nullptr)
        {
            _callbackSetPet(true);
            _callbackSetPet = nullptr;
        }
    }
    else
    {
        if( _callbackSetPet != nullptr)
        {
            _callbackSetPet(false);
            _callbackSetPet = nullptr;
        }
    }
}

void FriendManager::requestPointReceive(const std::function<void(bool, int)>& pCallback, int nIdx, std::string strPlatform)
{
    _callbackPoint = pCallback;
    
    //
    std::string url = "/user/friend/point/receive";
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_friend", nIdx);
    req->setData("_friend_platform", strPlatform.c_str());
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data)
    {
        std::string dd = data;
        responsePointReceive(response,dd);
    };
    req->send(cb);
}

void FriendManager::responsePointReceive(cocos2d::network::HttpResponse* response,std::string &data)
{
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
        if ( _callbackPoint != nullptr )
        {
            _callbackPoint(false, -1);
            _callbackPoint = nullptr;
        }
        return;
    }
    
    auto nResult = jsonParser["result"].GetInt();
    if ( nResult == 1 )
    {
        if ( _callbackPoint != nullptr )
        {
            _callbackPoint(true, nResult);
            _callbackPoint = nullptr;
        }
    }
    else
    {
        if ( _callbackPoint != nullptr )
        {
            _callbackPoint(false, nResult);
            _callbackPoint = nullptr;
        }
    }
}

void FriendManager::requestPointSend(const std::function<void(bool, int)>& pCallback, int nIdx, std::string strPlatform)
{
    _callbackPoint = pCallback;
    
    //
    std::string url = "/user/friend/point/send";
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_friend", nIdx);
    req->setData("_friend_platform", strPlatform.c_str());
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data)
    {
        std::string dd = data;
        responsePointSend(response,dd);
    };
    req->send(cb);
}

void FriendManager::responsePointSend(cocos2d::network::HttpResponse* response,std::string &data)
{
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
        if ( _callbackPoint != nullptr )
        {
            _callbackPoint(false, -1);
            _callbackPoint = nullptr;
        }
        return;
    }
    
    auto nResult = jsonParser["result"].GetInt();
    if ( nResult == 1 )
    {
        ServerMissionManager::getInstance()->setServerMission(E_DAILY_QUEST::FRIEND_SEND, 1);
        if ( _callbackPoint != nullptr )
        {
            _callbackPoint(true, nResult);
            _callbackPoint = nullptr;
        }
    }
    else
    {
        if ( _callbackPoint != nullptr )
        {
            _callbackPoint(false, nResult);
            _callbackPoint = nullptr;
        }
    }
}

void FriendManager::requestPointAll(const std::function<void(bool, int)>& pCallback)
{
    _callbackPoint = pCallback;
    
    //
    std::string url = "/user/friend/point/all";
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data)
    {
        std::string dd = data;
        responsePointAll(response,dd);
    };
    req->send(cb);
}

void FriendManager::responsePointAll(cocos2d::network::HttpResponse* response,std::string &data)
{
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
        if ( _callbackPoint != nullptr )
        {
            _callbackPoint(false, -1);
            _callbackPoint = nullptr;
        }
        return;
    }
    int nFriendPoint = 0;
    if ( jsonParser.HasMember("total_point") == true )
    {
        nFriendPoint = jsonParser["total_point"].GetInt();
        ItemsManager::getInstance()->setItemsServer(E_ITEMS::FRIEND_POINT, 0, nFriendPoint);
    }

    if(jsonParser.HasMember("send_count"))
    {
        int cnt = jsonParser["send_count"].GetInt();
        ServerMissionManager::getInstance()->setServerMission(E_DAILY_QUEST::FRIEND_SEND, cnt);
    }
    
    auto nResult = jsonParser["result"].GetInt();
    
    if ( nResult == 1 )
    {
        if ( _callbackPoint != nullptr )
        {
            _callbackPoint(true, nResult);
            _callbackPoint = nullptr;
        }
    }
    else
    {
        if ( _callbackPoint != nullptr )
        {
            _callbackPoint(false, nResult);
            _callbackPoint = nullptr;
        }
    }
}
