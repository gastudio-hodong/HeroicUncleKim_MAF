//
//  EventVoteManager.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/24.
//

#ifndef EventVoteManager_hpp
#define EventVoteManager_hpp

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoSetting.h"
#include "GameObject/EventVote/InfoEventVoteAll.h"
#include "GameObject/EventVote/InfoEventVoteCandidate.h"
#include "GameObject/EventVote/InfoEventVoteCandidateReward.h"

class EventVoteManager : public MafSingleton<EventVoteManager>
{
private:
    EventVoteManager(void);
    virtual ~EventVoteManager(void);
    virtual bool init();
    
    friend class MafSingleton;
public:
    // update
    void update(float dt);
    
    // load
    void setLoad();
    void setLoadVoteAll();
    void setLoadVoteCandidateReward();
    
    // set, get
    cocos2d::Vector<InfoEventVoteAll*> getListVoteAll();
    cocos2d::Vector<InfoEventVoteCandidate*> getListVoteCandidate();
    cocos2d::Vector<InfoEventVoteCandidate*> getListPrevVoteCandidate();
    InfoEventVoteAll* const getInfoVoteAll(const int idx) const;
    InfoEventVoteCandidate* const getInfoVoteCandidate(const int idx) const;
    InfoEventVoteCandidateReward* const getInfoVoteCandidateReward(const int idx) const;
    
    // set, get :
    const int getResultCode() const;
    const bool isBreakTime() const;
    const int getAllVoteCount();
    const float getPercentageOfCurrentVotes(const int idx);
    const float getPercentageOfVotes(InfoEventVoteCandidate* objCandidate, const int nAllCount);
    
    const int getVoteCoinCount();
    void setVoteCoinCount(const int value);
    
    const int64_t getEndEventTime();
    const int64_t getEndVoteTime();
    const int getAdsCount();
    const int getCurrentCandidateGroupNumber();
    const int getCandidateRank(InfoEventVoteCandidate* candidate);
    // set, get
    
    // set, get : ads
    
    // delegate
    void subDelegateInfo(MafFunction<void(bool, int)>* func);
    void invokeDelegateInfo(bool bResult, int nResult);
    void unSubDelegateInfo(void* obj);
    
    void subDelegateVote(MafFunction<void(bool, int)>* func);
    void invokeDelegateVote(bool bResult, int nResult);
    void unSubDelegateVote(void* obj);
    
    void subDelegateAds(MafFunction<void(bool, int)>* func);
    void invokeDelegateAds(bool bResult, int nResult);
    void unSubDelegateAds(void* obj);
    
    void onEventActivate(int nIdx);
    // network
    void requestInfo(bool bPopupLoading, const std::function<void(bool, int)>& callback = nullptr);
    void requestVoting(bool bPopupLoading, const int idx, const int count, const std::function<void(bool, int)>& callback = nullptr);
    void requestPreviousInfo(bool bPopupLoading, const std::function<void(bool, int)>& callback = nullptr);
    void requestEntireReward(bool bPopupLoading, const int idx, const std::function<void(bool, int)>& callback = nullptr);
    void requestAds(bool bPopupLoading, const std::function<void(bool, int)>& callback = nullptr);
protected:
    // network
    void responseInfo(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseVoting(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responsePreviousInfo(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseEntireReward(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
    void responseAds(bool bPopupLoading, cocos2d::network::HttpResponse* response,std::string &data);
private:
    //
    bool _isEventActivate;
    bool _isBreakTime;
    int _nResult;
    int _nSeason;
    TInteger _nCurrentCandidateGroupNumber;
    TInteger _nVoteAllCount;
    TInteger _nVoteCoin;
    int64_t _nEndEventTime;
    int64_t _nEndVoteTime;
    TInteger _adsCount;
    
    cocos2d::Vector<InfoEventVoteAll*> _listInfoVoteAll;
    cocos2d::Vector<InfoEventVoteCandidate*> _listInfoVoteCandidate;
    cocos2d::Vector<InfoEventVoteCandidate*> _listInfoPrevVoteCandidate;
    cocos2d::Vector<InfoEventVoteCandidateReward*> _listInfoVoteCandidateReward;
    MafDelegate<void(bool, int)> _delegateInfo;
    MafDelegate<void(bool, int)> _delegateVote;
    MafDelegate<void(bool, int)> _delegateAds;
    
    std::function<void(bool, int)> _callbackInfo;
    std::function<void(bool, int)> _callbackVote;
    std::function<void(bool, int)> _callbackAds;
};

#endif /* EventVoteManager_hpp */
