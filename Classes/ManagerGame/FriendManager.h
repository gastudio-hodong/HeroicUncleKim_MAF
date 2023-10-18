//
//  FriendManager.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 13/02/2020.
//

#ifndef FriendManager_h
#define FriendManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoFriend.h"

class FriendManager : public MafSingleton<FriendManager>
{
public:
    FriendManager(void);
    virtual ~FriendManager(void);
    virtual bool init();
    
public:
    // set, get
    cocos2d::Vector<InfoFriend*> getFriendList();
    cocos2d::Vector<InfoFriend*> getFriendRecommendList();
    cocos2d::Vector<InfoFriend*> getFriendWaitList();
    cocos2d::Vector<InfoFriend*> getFriendSearchList();
    
    bool IsFriend(int userIdx, std::string userPlatform);
    void setFriendListAmount(int nAmount);
    int getFriendListAmount();
    int getFriendIsRecommend();
    float getFriendBonusSpeed();
    
    
    void setInviteFriendId(std::string userid, std::string os);
    E_PET getMyPet();
    
    int getPointSendMax();
    int getPointSend();
    
    int getFriendDeleteMax();
    int getFriendDeleteNow();
    
    // sort
    void sortFriendList();
    
    //
    void requestFriendInfo(const std::function<void(bool)>& pCallback);
    void responseFriendInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void requestRecommendInfo(const std::function<void(bool)>& pCallback);
    void responseRecommendInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void requestWaitInfo(const std::function<void(bool)>& pCallback);
    void responseWaitInfo(cocos2d::network::HttpResponse* response,std::string &data);
    void requestSearchInfo(const std::function<void(bool)>& pCallback, std::string strNick);
    void responseSearchInfo(cocos2d::network::HttpResponse* response,std::string &data);
    
    void requestFriendAddRequest(const std::function<void(bool, int)>& pCallback, int nUserIdx, std::string strPlatform, int nType);
    void responseFriendAddRequest(cocos2d::network::HttpResponse* response,std::string &data);
    void requestFriendApply(const std::function<void(bool, int)>& pCallback, int nUserIdx, std::string strPlatform, int nType);
    void responseFriendApply(cocos2d::network::HttpResponse* response,std::string &data);
    void requestFriendDelete(const std::function<void(bool, int)>& pCallback, int nUserIdx, std::string strPlatform);
    void responseFriendDelete(cocos2d::network::HttpResponse* response,std::string &data);
    void requestFriendSearch(const std::function<void(bool, int)>& pCallback, std::string strNick);
    void responseFriendSearch(cocos2d::network::HttpResponse* response,std::string &data);
    
    void requestFriendIdx();
    void responseFriendIdx(cocos2d::network::HttpResponse* response,std::string &data);
    void requestIsRecommend(const std::function<void(bool)>& pCallback, int nType);
    void responseIsRecommend(cocos2d::network::HttpResponse* response,std::string &data);
    void requestChangeMyPet(const std::function<void(bool)>& pCallback, E_PET pet);
    void responseChangeMyPet(cocos2d::network::HttpResponse* response,std::string &data);
    
    void requestPointReceive(const std::function<void(bool, int)>& pCallback, int nIdx, std::string strPlatform);
    void responsePointReceive(cocos2d::network::HttpResponse* response,std::string &data);
    void requestPointSend(const std::function<void(bool, int)>& pCallback, int nIdx, std::string strPlatform);
    void responsePointSend(cocos2d::network::HttpResponse* response,std::string &data);
    void requestPointAll(const std::function<void(bool, int)>& pCallback);
    void responsePointAll(cocos2d::network::HttpResponse* response,std::string &data);


    bool _bBootLinkAdd;//게임 시작 시, 다른 씬에서 추가시 true
    
private:
    cocos2d::Vector<InfoFriend*> _vecFriendList;
    cocos2d::Vector<InfoFriend*> _vecFriendRecommendList;
    cocos2d::Vector<InfoFriend*> _vecFriendWaitList;
    cocos2d::Vector<InfoFriend*> _vecFriendSearchList;
    int _nFriendAmount;
    int _nFriendIsRecommend;
    
    int _nUserIdxFriend;
    int _nUserIdxFriendInvite;
    
    std::string _inviteFriendOS;
    std::string _inviteFriendUserid;
    
    int _nMaxRecievePoint;
    int _nMaxSendPoint;
    int _nRecievePoint;
    int _nSendPoint;
    
    int _nFriendDeleteMax;
    int _nFriendDeleteNow;
    
    E_PET _nPet;
    E_PET _tempPet;

    // callback
    std::function<void(bool)> _callbackFriendInfo;
    std::function<void(bool)> _callbackFriendRecommendInfo;
    std::function<void(bool)> _callbackFriendWaitInfo;
    std::function<void(bool)> _callbackFriendSearchInfo;
    
    std::function<void(bool)> _callbackSetPet;
    std::function<void(bool, int)> _callbackFriendAddRequest;
    std::function<void(bool, int)> _callbackFriendApply;
    std::function<void(bool, int)> _callbackFriendDel;
    std::function<void(bool, int)> _callbackPoint;
    std::function<void(bool)> _callbackIsRecommend;
    
};

#endif /* FriendManager_h */
